#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include<string.h>

int min(int a,int b)
{
	//finding minimum of two lements
    return(a<b?a:b);
}
struct stackNode
{
   int u;
   struct stackNode *next;
}*top = NULL;


void push(int value)
{
	// this function pushes int value into a stack
   struct stackNode *newNode;
   newNode = (struct stackNode*)malloc(sizeof(struct stackNode));
   newNode->u = value;
   if(top == NULL)
      newNode->next = NULL;
   else
      newNode->next = top;
   top = newNode;
   printf("\n inserted %d", top->u);
}

struct node
{
 int val;
 struct node* next;
};

struct graph
{
 int v; //no of vertices
 struct node** arr; // adjacency list
};
struct graph* createGraph(int v)
{
	// returns an empty graph with vertices
    int i;
    struct graph* temp =(struct graph*)malloc(sizeof(struct graph));
    temp->v=v;
    for(i=0;i<v;i++)
     temp->arr=(int**)malloc(sizeof(int**)*v);
    for(i=0;i<v;i++)
     temp->arr[i]=NULL;
    return temp;
}
void addEdge(struct graph* g,int u,int v)
{	
	// ADDS EDGE (U,V) TO graph g	
	if (u == v){
		return;
	}
	printf(" adding edge between %d and %d  \n", u+1, v+1);
    struct node* temp =(struct node*)malloc(sizeof(struct node));
    temp->val = v;
    temp->next = g->arr[u];
    g->arr[u] = temp;
}
void apUtil(struct graph * g, int vert, int bredge[vert][vert], int charge_edge[vert][vert], int node,int* isVisited,int* des,int* parent,int* low,int* ap, int* charge_vert)
{	
	/* This function extracts Articulation points and bridges and stores the status of the corresponding verices
		and edges in array 'ap' and matrix 'bredge'. That is, articulation point status of a vertex is stored in ap and bridge status of edge 
		is stored in matrix 'bredge' . Modification of 'charge_edge' and 'charge_vert' represents operations charged to edge and vertex. Charging scheme is explained in documentation.*/ 
    struct node* temp=NULL;
    static int time=0;
    int children=0;
    temp = g->arr[node];
    isVisited[node]=1;
    time++;
    des[node]=low[node]=time; // des : discovery time, low : low time
    printf("VERTEX %d", node +1);
    charge_vert[node]+=5;// chargina vertex for initializing adjacency list, children discovery time low time
    while(temp!=NULL) 
    {
        printf("\n EDGE %d %d ", node+1, temp->val +1);
	    if(!isVisited[temp->val])
        {

		  charge_edge[node][temp->val] += 1; // charging for one if	condition checking visit status
          children++;
		  parent[temp->val]=node;
		  charge_edge[node][temp->val] += 1; //charging edge for parent assigning
          apUtil(g,vert,bredge,charge_edge,temp->val,isVisited,des,parent,low,ap,charge_vert);

          low[node]= min(low[node],low[temp->val]);
          charge_edge[node][temp->val] += 1; //charging edge for assigning low

			/////////// STORING BRIDGES SO THAT IT CAN BE PRINTED IN A SORTED WAY //////
		//checking condition for bridges 
            if (des[node]<low[temp->val]){
             if (node < temp->val){
			bredge[node][temp->val] = 1 ;
            }
            else {
              bredge[temp->val][node] = 1;
            }
			charge_edge[node][temp->val] += 2; // charging edge for bridge assigning and first if statement - comparing des & low
			}
			////////////// bridge stored//////////////////////////////////////////////////


			//////// STORING AP SO THAT IT CAN BE PRINTED IN A SORTED WAY ///////////
			//checking condition for AP
		  if(parent[node]==-1 && children>1){
             ap[node]=1;
			 charge_edge[node][temp->val] += 2;// charging edge for AP assigning and if
			 }
			
		//checking another condition for AP
          else if(parent[node]!=-1 && des[node]<=low[temp->val]){
            ap[node]=1;
			charge_edge[node][temp->val] += 3; // charging edge for AP assigning and 2 ifs
			}
			//////////////// AP STORED! ////////////////////////////////////////////
			else{
			charge_edge[node][temp->val] += 2; //// charging edge for 1 if and 1 else if
			}
			
        }
        else if(isVisited[temp->val] && temp->val!=parent[node])
        {
		// back edge found	
            low[node]=min(low[node],des[temp->val]);
            charge_edge[node][temp->val] += 3; // charging edge for LOW assigning and 1 if & 1 else if
        }
		else {	
        charge_edge[node][temp->val] += 2; //charging edge for 1 if and 1 else if
		}

       temp= temp->next;
      }
}

void bcUtil(struct graph * g, int vert, int bc[vert][vert],int charge_edge[vert][vert],int node,int* isVisited,int* des,int* parent,int* low, int* charge_vert)
{	
	/* This function extracts biconnected components & stores the status of the corresponding verices in the component in matrix 'bc'.
	. Modification of 'charge_edge' and 'charge_vert' represents operations charged to edge and vertex. Charging scheme is explained in documentation.*/ 
    struct node* temp=NULL;
    static int time=0;
    int children=0;
    int last_a=999;
    temp = g->arr[node];
    isVisited[node]=1;
    time++;
    des[node]=low[node]=time;
	charge_vert[node]+=5;

    while(temp!=NULL)
    {
	    if(!isVisited[temp->val])
        {
			charge_edge[node][temp->val] += 1; // charging for one if	
          children++;
			// push new vertex adhacent to 'node' into stack
		  push((temp->val) + 1); //// charging edge for pushing
		  charge_edge[node][temp->val]+=1;
		  parent[temp->val]=node;
		  charge_edge[node][temp->val]+=1;  // charging edge for assigning parent
		  
          bcUtil(g,vert,bc,charge_edge,temp->val,isVisited,des,parent,low,charge_vert);

          low[node]= min(low[node],low[temp->val]);
          charge_edge[node][temp->val]+=1; // charging edge for assigning low
		  //condition for checking biconnected components
		  if(parent[node]==-1 && children>1){
              bc[node][node] += 1;
				// START POPPING until edge(node+1, temp->val + 1) is reached 
              while(last_a!=(temp->val) + 1){
                    struct stackNode *tempo = top;
                    last_a = top->u;

                    bc[node][top->u - 1] = bc[node][node];
                    top = tempo->next;
                    free(tempo);
                    }
			  charge_edge[node][temp->val]+=2; // charging edge for if, pop

			 }
			//another condition for checking biconnected components
          else if(parent[node]!=-1 && des[node]<=low[temp->val]){
			// vertex 'node' an be in more than one component, so bc[node][node] represents the no. of components of which node is  a part
		    bc[node][node] += 1;
			// START POPPING until edge(node+1, temp->val + 1) is reached 
              while(last_a!=(temp->val) + 1){
                    struct stackNode *tempo = top;
                    last_a = top->u;

                    bc[node][top->u - 1] = bc[node][node]; //for eg: if this is node '0' second component, then bc[0][0] = 3, all other elements in bc[0][:], relevant during this iteration will be 3, representing that these vertices are in node '0's third component  
                    top = tempo->next;
                    free(tempo);
                    }
			charge_edge[node][temp->val]+=3; // charging edge for 2 ifs, pop

			}
			/**/
			else{
			charge_edge[node][temp->val]+=2; // charging edge for 2 ifs
			}
        }
        else if( temp->val!=parent[node] && des[temp->val]<low[node])
        {
            low[node]=des[temp->val];
			charge_edge[node][temp->val]+=3; // charging edge for 1 if 1 else if and assigning low
           
        }
		else{
		charge_edge[node][temp->val]+=2; // charging edge for 1 if 1 else if 
			}
       temp= temp->next;
      }

}
void AP(struct graph* g)
{	/*
	This function initializes variables required to extract ariculation points, biconnected component and bridges. It sends over empty arrays - for AP & matrices - 	for bridges & biconnected components to APUtil & bcUtil function respectively, where they get filled up. FInally, this function outputs 5 text file for a given input graph, in the required format using the filled up arrays and matrices.
	*/
    int i;
    int* des = (int*)malloc(sizeof(int)*g->v); // discovery time
    int* isVisited = (int*)malloc(sizeof(int)*g->v);// visit status			
    int* parent = (int*)malloc(sizeof(int)*g->v); // parent	
    int* low = (int*)malloc(sizeof(int)*g->v); // low time
    int* ap = (int*)malloc(sizeof(int)*g->v); // array containing status of articulation point
	int* charge_vert = (int*)malloc(sizeof(int)*g->v);
    int x,y;
    //initializing  matrix denoting status of bridges 	
    int bredge[(g->v)][(g->v)];
	int charge_edge[(g->v)][(g->v)];//initializing  matrix denoting status of charges to the edge 	
	int bc[g->v][g->v];//initializing  matrix denoting status of vertices in a biconnected component 
	for (x=0;x<(g->v);x++)
	{
		for (y=0;y<(g->v);y++)
		{
			bredge[x][y]=0;
			charge_edge[x][y]=0;
			bc[x][y] = 0;
		}
	}

    for(i=0;i<g->v;i++)
    {
        isVisited[i]=0;
        parent[i]=-1;
        ap[i]=0;
		charge_vert[i] = 0;
    }
    for(i=0;i<g->v;i++)
    {
        if(isVisited[i]==0)
        {
            apUtil(g,g->v,bredge,charge_edge,i,isVisited,des,parent,low,ap, charge_vert);// performing dfs

        }
    }
	FILE *a, *b;
	a = fopen("Aa.txt", "w");
	b = fopen("Ab.txt", "w");
    printf("\n");
    // traversing ap array in a sequential manner to print APs in a sorted way
    for(i=0;i<g->v;i++)
    {

		if (ap[i]!=0)
		{
        	printf(" %d ",i+1 );
			fprintf(a, "%d ", i+1);
		}
    }
  
	fclose(a);
    // traversing 'bredge' matrix in a sequential manner to print bridges in a sorted way
    for (x=0;x<(g->v);x++)
	{
		for (y=0;y<(g->v);y++)
		{
			if(bredge[x][y]!=0)
			{ fprintf(b, "%d %d\n",x+1,y+1);
			}
		}
	}
	fclose(b);
	
	des = (int*)malloc(sizeof(int)*g->v);
    isVisited = (int*)malloc(sizeof(int)*g->v);
    parent = (int*)malloc(sizeof(int)*g->v);
    low = (int*)malloc(sizeof(int)*g->v);
	for(i=0;i<g->v;i++)
    {
        isVisited[i]=0;

        
    }
	for(i=0;i<g->v;i++)
    {
        if(isVisited[i]==0)
        {
            push(i+1);// pushing starting vertex
            bcUtil(g,g->v,bc,charge_edge,i,isVisited,des,parent,low, charge_vert);// Performing DFS
        }
    }
    printf("\n");
	FILE *c;
	c = fopen("Ac.txt", "w");
    for (x=0;x<(g->v);x++)
	{   int index = bc[x][x];
        while(index>0)//writing vertices which are part of the same component as vertex x. 
		{
         
		for (y=0;y<(g->v);y++)
		{

			if(bc[x][y]==index)

			{
			    fprintf(c, "%d ",y+1);
			}
        }
		fprintf(c,"\n");
        bc[x][x]--;
        index--; // moving to the next component of which vertex x is a part
		}
	}
	fclose(c);
	FILE *op;
	op = fopen("B.txt", "w");
	int v_op_count = 0;
	int e_op_count = 0;
	for(i=0;i<g->v;i++)
    {
        	fprintf(op, "vertex %d has been charged %d times\n",i+1,charge_vert[i]);
			v_op_count+=charge_vert[i];
    }
	for (x=0;x<(g->v);x++)
	{
		for (y=0;y<(g->v);y++)
		{
			if (x<y){
			charge_edge[x][y]+=charge_edge[y][x];
			
			if(charge_edge[x][y]!=0){
			fprintf(op,"edge %d %d has been charged %d times\n",x+1,y+1, charge_edge[x][y]);
			e_op_count+=charge_edge[x][y];
			}
			}
			
		}
	}
	fclose(op);
	FILE *total;
	total = fopen("C.txt", "w");
	fprintf(total,"Total number of operations charged to all vertices is: %d",v_op_count);
	fprintf(total,"\nTotal number of operations charged to all edges is: %d",e_op_count);
	fprintf(total,"\nTotal number of operations is: %d",v_op_count + e_op_count);
	fclose(total);
}

int main(int argc, char *argv[])
{

	FILE *fp;
	
	if (argc < 2)
	{
		printf("too few arguments !");
		return 0;
	}

	// read input from file
	fp = fopen(argv[1], "r");

	int num_v = 0;
	char l[256];
	while (fgets(l, sizeof l, fp) != NULL) num_v++;
	rewind(fp);
	struct graph* g = createGraph(num_v);
    char line[1024];
	while (fgets(line, sizeof line, fp) != NULL)
	{
    int src_vertex, dst_vertex;
		char * pch;
		pch = strtok(line, " \n\r");
		sscanf(pch, "%d", &src_vertex);
		pch = strtok(NULL, " \n\r");

		while (pch != NULL)
		{
			// there is an edge from src_vertex to dst_vertex
			sscanf(pch, "%d", &dst_vertex);

      	addEdge(g,src_vertex-1, dst_vertex-1);
			pch = strtok(NULL, " \n\r");
		}

	}

	fclose(fp);

    AP(g);


}
