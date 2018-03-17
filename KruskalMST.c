/**************************************************************
* 
*	KRUSKALS MINIMUM SPANNING TREE
*	Developer : Christos Papantonis 
*	            A.M : 2012 215 
*
*          Lets us find the  Kruskals' MST of the following graph
*                                 8                    7
*	         2 ---------------3 ---------------4
*	       / |	   	    /		   |  \       
*	 4  /   |		  / 2		   |     \	9
*	    /    |		/		   |        \
*           1        | 11     9		           14 |	5
*	   \     |	     /      \		   |         /
*	 8 \    |	7 /          \  6		   |        /  10
*	       \ |	 /              \		   |     /
*	         8----------------7---------------  6   
*		         1	                   2
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "SparseMst.h"

struct graphVertex{
	int value ,rank;
	struct graphVertex* parent;
	struct graphVertex* left;
	struct graphVertex* right;
};

struct graphEdge{
	int i, j; 
	int weight;
	struct graphEdge* left;
	struct graphEdge* right;
};

//struct to wrap and modify multiple values in a function 
struct listHeaders{
	struct graphVertex* header_V;
	struct graphEdge* header_E;
	struct  graphEdge * header_mst;
	int V ; //number of vertices in graph
	int sumWeight;
};

/***************FUNCTIONS DECLARATIONS*****************/
void kruskalsMST(struct Array *SparseArray);
struct listHeaders* VerticesNumber(struct Array* SparseArray, struct listHeaders*headers);
struct listHeaders* putVerticesOnList(struct listHeaders* headers, struct graphVertex* vertexToBeInserted);
struct listHeaders* putGraphEdgesOnList(struct listHeaders* headers,struct graphEdge *edgeAux);
struct listHeaders *recallEdges(struct Array* SparseArray , struct listHeaders* headers);
struct listHeaders* putMstEdgesOnList(struct listHeaders* headers,struct graphEdge *edgeAux);
struct graphVertex* drainVertex(struct listHeaders*headers , int z);
struct  graphEdge *CopyEdge(struct graphEdge* in);
struct graphVertex* find (struct listHeaders*headers , int x);
void Union (struct listHeaders* headers , int x , int y);
void printMst(struct listHeaders* headers);
int vertexExists (struct listHeaders* headers, struct graphVertex *existAux);

/*************************************MAIN FUNCTION***************************8*/
void main (){
	printf("************************************************************\n");
	printf("WELCOME TO KRUSKALS MINIMUM SPANNING TREE \n");
	printf("************************************************************\n");
	fflush (stdin);
	char choice = 0;
	struct Array* SparseArray = sparseConstructor(); 

	

	while(choice !='q'){
		printf("HELLO!! What do you want to do??\n");
		printf("Press e(edit graph) , s(Spanning tree MST ) or Quit (q)\n");
		scanf("%c",&choice);
		getchar();
		
		switch (choice){
			case 'e':
				menuSparse(SparseArray);
				break;
			case 's':
				kruskalsMST(SparseArray);
				break;
			case 'q' :
				printf("GOODBYE HF\n");
				break;	
			default :
				printf("Wrong Selection\n");	

		}	
	}
}
/*******************************USED FUNCTIONS FOR THE IMPLEMANTATION OF MST ****************************/

int vertexExists (struct listHeaders* headers, struct graphVertex *existAux){
	int exists = 0;
	struct graphVertex* existCurrent;
	existCurrent =headers -> header_V;
	if (existCurrent != NULL){
		while(existCurrent ->right != NULL ){
			if (existCurrent->value == existAux ->value)
			{
				//printf("%d ", existCurrent->value);
				//printf("Vertex exists \n");
				exists =1;
				break;
			}
			else{
				existCurrent = existCurrent->right;
			}
		}
		if (existCurrent ->value == existAux ->value ){
		exists = 1;
		}	
	}		
	
	return exists;
}
struct listHeaders* putVerticesOnList(struct listHeaders* headers, struct graphVertex* vertexToBeInserted){
	
	if (headers -> header_V ==NULL){
		headers -> header_V = vertexToBeInserted;
	}
	else{
		struct graphVertex* current  = headers -> header_V;
		while (current ->right !=NULL){
			current =  current ->right;
		}
		vertexToBeInserted ->right = current->right;
		current ->right = vertexToBeInserted;
		vertexToBeInserted ->left = current;
	}
	headers->V += 1;
	return headers;
}

//put edges of sparse in a list in a non increasing order..
struct listHeaders* putGraphEdgesOnList(struct listHeaders* headers,struct graphEdge *edgeAux){
	struct graphEdge* edgeCurrent  = headers ->header_E;
	if (headers -> header_E == NULL){
		headers -> header_E = edgeAux;
	}
	else if(edgeAux->weight < edgeCurrent->weight){ // to  aux bainei brosta
		edgeAux->right = headers -> header_E;
		headers -> header_E ->left = edgeAux;
		headers -> header_E = edgeAux;}
	else{	
		while (edgeCurrent ->right !=NULL && edgeAux->weight > edgeCurrent->right->weight){
			edgeCurrent = edgeCurrent ->right;
		}
			edgeAux->right =  edgeCurrent ->right;
			edgeCurrent ->right =  edgeAux;
			edgeAux->left = edgeCurrent;
		
	}
	
	return headers;
}

struct listHeaders* putMstEdgesOnList(struct listHeaders* headers,struct graphEdge *edgeAux){
	struct graphEdge* edgeCurrent  = headers ->header_mst;
	if (headers -> header_mst == NULL){
		headers -> header_mst = edgeAux;
	}
	else{	
		while (edgeCurrent ->right !=NULL && edgeAux->weight > edgeCurrent->right->weight){
			edgeCurrent = edgeCurrent ->right;
		}
		edgeAux->right =  edgeCurrent ->right;
		edgeCurrent ->right =  edgeAux;
		edgeAux->left = edgeCurrent;
		
	}
	headers -> sumWeight += edgeAux ->weight;
	return headers;
}

// drains Edges from Sparse  and puts the on list
struct listHeaders *recallEdges(struct Array* SparseArray , struct listHeaders* headers){
	int i  ;
	for (i= 0 ; i < 15 ; i++){
		struct edge* current = NULL;
		if (SparseArray->HeaderRows[i]->right != NULL){	
			current =SparseArray->HeaderRows[i]->right;
			while (current->right !=NULL){
				struct graphEdge* edgeToBeInserted  = (struct graphEdge*)malloc(sizeof(struct graphEdge));
				struct  graphVertex* src = (struct graphVertex*)malloc(sizeof(struct graphVertex));
				struct  graphVertex* dst  =(struct graphVertex*)malloc(sizeof(struct graphVertex));
				edgeToBeInserted ->i = current ->i;
				edgeToBeInserted ->j = current ->j;
				edgeToBeInserted -> weight = current ->weight; 
				src ->value = current ->i;
				src ->parent = src;
				dst ->value = current ->j;
				dst ->parent = dst;

				//examines whether the vertices exist in the list 
				if (!vertexExists(headers , src)){
					headers = putVerticesOnList(headers , src);}
				else{free(src);}	
				if(!vertexExists(headers , dst)){ 
					headers = putVerticesOnList(headers , dst);}
				else{free(dst);}	
				
				headers  = putGraphEdgesOnList(headers , edgeToBeInserted);
				current = current ->right;
			}
			// last iteration cause  while func does not examine last element 
			struct  graphVertex* src = (struct graphVertex*)malloc(sizeof(struct graphVertex));
			struct  graphVertex* dst  =(struct graphVertex*)malloc(sizeof(struct graphVertex));
			struct graphEdge* edgeToBeInserted  = (struct graphEdge*)malloc(sizeof(struct graphEdge));
			src ->value = current ->i;
			src ->parent = src;
			dst ->value = current ->j;
			dst ->parent = dst;
			if (!vertexExists(headers , src)){
				headers = putVerticesOnList(headers , src);
			}
			else{free(src);}	

			if(!vertexExists(headers , dst)){
				headers = putVerticesOnList(headers , dst);
			}
			else{free(dst);}	
		
			edgeToBeInserted ->i = current ->i;
			edgeToBeInserted ->j = current ->j;
			edgeToBeInserted -> weight = current ->weight; 
			headers  = putGraphEdgesOnList(headers , edgeToBeInserted);
		}		
	}
	return headers;	
}

//DRAINS THE WANTED VERTEX FROM THE LIST AND RETURN IT  SO IT CAN BE 
//COMPARED OR UPDATE ITS POINTERS
struct graphVertex* drainVertex(struct listHeaders*headers , int z){
	struct graphVertex* aux = NULL;
	struct graphVertex* drained = NULL;
	aux = headers->header_V;
	while (aux ->right != NULL ){
		if(aux ->value == z){
			drained =  aux;
			break ;
		}
		else{
			aux = aux ->right;
		}
	}
	if (aux->value == z){
		drained = aux;
	}
	return drained;
}
//creates copies of structs so the initials' struct pointers stay intact
struct  graphEdge *CopyEdge(struct graphEdge* in){
	struct  graphEdge* out = (struct graphEdge*)malloc(sizeof(struct graphEdge));
	out ->i = in ->i;
	out->j = in->j;
	out ->weight = in ->weight;
	return out;
} 
// FINDS THE VERTEX THAT HAS PARENT ITSELF
struct graphVertex* find (struct listHeaders*headers , int x){
	struct graphVertex* aux = NULL;
	aux =  drainVertex(headers , x);  
	while(aux ->parent != aux){
		aux = aux ->parent; 
	}	
	return aux  -> parent;
}

///FUCTION TO UPDATE THE PARENT OF EACH VERTEX INCLUDE IN THE MST 
// TO AVOID CIRCLES 
void Union (struct listHeaders* headers , int x , int y){
	struct graphVertex* temp1 = NULL;
	struct graphVertex* temp2 = NULL;
	temp1 = drainVertex(headers , x);
	temp2 = drainVertex(headers , y);
	if(temp1->rank < temp2 ->rank){
		temp1 ->parent  = temp2;
	}
	else if (temp1 ->rank < temp2 -> rank){
		temp2 ->parent = temp1;
	}
	else{
		temp2 ->parent = temp1;
		temp1 -> rank  += 1;
	}
}

/************************ PRINTING SESSION *********************************************/
void printEdges(struct listHeaders* headers){
	struct graphEdge *current ;
	current = headers -> header_E;
	printf("-------------------- The graph edges in a non decreasing weight  order --------------\n");
	while(current->right !=NULL){
		printf("%d----%d : weight : %d \n",current->i , current->j,current->weight );
		current = current ->right;
	}
	printf("%d----%d : weight : %d \n",current->i , current->j,current->weight );
}

void printVertices(struct listHeaders* headers){
	struct graphVertex *current = headers -> header_V;
	printf("-------------------- The graph Vertices with their parents and rank --------------\n");
	while(current->right !=NULL){
		printf("# %d , parent : %d  , rank : %d  \n", current->value , current->parent->value , current -> rank);
		current = current ->right;
	}
	printf("# %d , parent : %d  , rank : %d  \n", current->value , current->parent->value , current -> rank);
}

void printMst(struct listHeaders* headers){
	struct graphEdge *current ;
	current = headers -> header_mst;
	printf("-------------------- The final mst edges  --------------\n");
	while(current->right !=NULL){
		printf("%d----%d : weight : %d \n",current->i , current->j,current->weight );
		current = current ->right;
	}
	printf("%d----%d : weight : %d \n",current->i , current->j,current->weight );
	printf(" \nThe total Σw(i,j) of the MST is :%d \n", headers -> sumWeight);
	printf("\n");
}
/*********************************************************************************/

void kruskalsMST(struct Array *SparseArray){
	int E = 0;
	struct listHeaders* headers = (struct listHeaders*)malloc(sizeof(struct listHeaders));
	headers = recallEdges(SparseArray,headers );
	printEdges(headers);
	printf("The number of vertices in the graph is : %d\n",headers->V );
	struct graphEdge* nextEdge =  headers ->header_E;
	
	while (E < headers->V -1){
		struct graphVertex* x  = find(headers , nextEdge -> i );
		struct graphVertex* y  = find(headers , nextEdge -> j );

		if (x != y ){
			struct graphEdge* tempCopy = CopyEdge(nextEdge);
			headers = putMstEdgesOnList(headers , tempCopy);
			Union(headers , x -> value , y ->value);
			E++;
		}
		if(nextEdge->right != NULL){		
			nextEdge = nextEdge -> right ;	
		}
		else{
			break;
		}
	}
	printVertices (headers);
	printMst(headers);
}


/*************************************************************************************
* The MST Edges of the above Graph are the following:
*
*	7----8 : weight : 1 						
*	6----7 : weight : 2 						
* 	3----9 : weight : 2 
*	3----6 : weight : 4 		
*	1----2 : weight : 4 
*	3----4 : weight : 7 
*	2----3 : weight : 8 
*	4----5 : weight : 9	
*
*	The total Σw(i,j) of the MST is :37
*
*  And the Updated parents of the vertices are: 
*
*	# 1 , parent : 3  , rank : 1  
*	# 2 , parent : 1  , rank : 0  					3
*	# 8 , parent : 7  , rank : 0  				        /    /  \    \     \
*	# 3 , parent : 3  , rank : 4                                                        /   /      \    \     \
*	# 4 , parent : 3  , rank : 0 	--->                                       1   7        4    9   5
*	# 6 , parent : 7  , rank : 0                                                      /   /  \
*	# 9 , parent : 3  , rank : 0                                                    /    /     \
*	# 5 , parent : 3  , rank : 0                                                   2    8      6
*	# 7 , parent : 3  , rank : 1 
*
****************************************************************************************/