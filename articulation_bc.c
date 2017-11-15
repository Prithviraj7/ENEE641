#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include<string.h>

int min(int a,int b)
{
    return(a<b?a:b);
}
struct stackNode
{
   int u;
	int v;
   struct stackNode *next;
}*top = NULL;


void push(int value)
{
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
 int v;
 struct node** arr;
};
struct graph* createGraph(int v)
{
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
{	if (u == v){
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
    struct node* temp=NULL;
    static int time=0;
    int children=0;
    temp = g->arr[node];
    isVisited[node]=1;
    time++;
    des[node]=low[node]=time;
    printf("VERTEX %d", node +1);
    charge_vert[node]+=5;
    while(temp!=NULL)
    {
        printf("\n EDGE %d %d ", node+1, temp->val +1);
	    if(!isVisited[temp->val])
        {



			//charge_edge[temp->val][node] += 1;
          children++;
		  parent[temp->val]=node;
		  charge_edge[node][temp->val] += 1;
          apUtil(g,vert,bredge,charge_edge,temp->val,isVisited,des,parent,low,ap,charge_vert);

          low[node]= min(low[node],low[temp->val]);
          charge_edge[node][temp->val] += 1;
            if (des[node]<low[temp->val]){
             if (node < temp->val){
			bredge[node][temp->val] = 1 ;
			charge_edge[node][temp->val] += 1;
            }
            else {
              bredge[temp->val][node] = 1;
              charge_edge[node][temp->val] += 1;
            }
			}
			charge_edge[node][temp->val] += 1;
		  if(parent[node]==-1 && children>1){
             ap[node]=1;
             charge_vert[node]+=5;
			 }

          else if(parent[node]!=-1 && des[node]<=low[temp->val]){
            ap[node]=1;
            charge_vert[node]+=1;
			}
			charge_edge[node][temp->val] += 2;

        }
        else if(isVisited[temp->val] && temp->val!=parent[node])
        {
            //printf("\n vert %d has been visited", node+1);
            //printf("\n BACKEDGE %d %d ", node+1, temp->val +1);
            low[node]=min(low[node],des[temp->val]);
            charge_edge[node][temp->val] += 1;
        }
        charge_edge[node][temp->val] += 1;
        //printf("vert %d has been visited!!\n",temp->val +1 );
       temp= temp->next;
      }
}

void bcUtil(struct graph * g, int vert, int bc[vert][vert],int node,int* isVisited,int* des,int* parent,int* low)
{
    struct node* temp=NULL;
    static int time=0;
    int children=0;
    int backEdge = 0;
    int last_a=999;
    temp = g->arr[node];
    isVisited[node]=1;
    time++;
    des[node]=low[node]=time;
    //printf("VERTEX %d", node +1);
    while(temp!=NULL)
    {
	    if(!isVisited[temp->val])
        {
          children++;
		  //printf("\nedddddd: %d %d",node+1, (temp->val) + 1);
		  //printf("\n**add %d",(temp->val)+1);
		  push((temp->val) + 1);
		  parent[temp->val]=node;
          bcUtil(g,vert,bc,temp->val,isVisited,des,parent,low);

          low[node]= min(low[node],low[temp->val]);

		  if(parent[node]==-1 && children>1){

		      //printf("\n**add %d",temp->val + 1);
              //printf("\n** NEW COMPONENT, start vert %d", node + 1);
              bc[node][node] += 1;
              //printf("\n %d has been encountered", node+1);
              while(last_a!=(temp->val) + 1){
                    struct stackNode *tempo = top;
                    last_a = top->u;
                    printf("\n=============== POPPED ELEMENT %d",top->u);

                    bc[node][top->u - 1] = bc[node][node];
                    top = tempo->next;
                    free(tempo);
                    }

			 //pop((temp->val) + 1);
			 }
          if(parent[node]!=-1 && des[node]<=low[temp->val]){
             //printf("\n**add %d",temp->val + 1);
            printf("\n** NEW COMPONENT, start vert %d", node + 1);
		    bc[node][node] += 1;
              printf("\n %d has been encountered", node+1);
              while(last_a!=(temp->val) + 1){
                    struct stackNode *tempo = top;
                    last_a = top->u;
                    printf("\n=============== POPPED ELEMENT %d",top->u);

                    bc[node][top->u - 1] = bc[node][node];
                    top = tempo->next;
                    free(tempo);
                    }

			}
        }
        else if( temp->val!=parent[node] && des[temp->val]<low[node])
        {
            //printf("\n backedge found");
            low[node]=des[temp->val];
            //printf("\n**add %d",node +1);
			//push((temp->val) + 1, node+1);
        }

       temp= temp->next;
      }

}
void AP(struct graph* g)
{
    int i;
    int* des = (int*)malloc(sizeof(int)*g->v);
    int* isVisited = (int*)malloc(sizeof(int)*g->v);
    int* parent = (int*)malloc(sizeof(int)*g->v);
    int* low = (int*)malloc(sizeof(int)*g->v);
    int* ap = (int*)malloc(sizeof(int)*g->v);
	int* charge_vert = (int*)malloc(sizeof(int)*g->v);
    int x,y;
    int bredge[(g->v)][(g->v)];
	int charge_edge[(g->v)][(g->v)];
	for (x=0;x<(g->v);x++)
	{
		for (y=0;y<(g->v);y++)
		{
			bredge[x][y]=0;
			charge_edge[x][y]=0;
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
            apUtil(g,g->v,bredge,charge_edge,i,isVisited,des,parent,low,ap, charge_vert);

        }
    }
	FILE *a, *b;
	a = fopen("Aa.txt", "w");
	b = fopen("Ab.txt", "w");
    printf("\n");
    for(i=0;i<g->v;i++)
    {

		if (ap[i]!=0)
		{
        	printf(" %d ",i+1 );
			fprintf(a, "%d ", i+1);
		}
    }
    for(i=0;i<g->v;i++)
    {

        	printf("\n vert %d has been charged %d times",i+1,charge_vert[i] );


    }
	fclose(a);
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
	for (x=0;x<(g->v);x++)
	{
		for (y=0;y<(g->v);y++)
		{
			if(charge_edge[x][y]!=0)
			{ printf("edge %d %d has been charged %d times\n",x+1,y+1, charge_edge[x][y]);
			}
		}
	}
}
void BCP(struct graph* g)
{
    int i;
    int* des = (int*)malloc(sizeof(int)*g->v);
    int* isVisited = (int*)malloc(sizeof(int)*g->v);
    int* parent = (int*)malloc(sizeof(int)*g->v);
    int* low = (int*)malloc(sizeof(int)*g->v);
    int x,y;
    int bc[(g->v)][(g->v)];
	for (x=0;x<(g->v);x++)
	{
		for (y=0;y<(g->v);y++)
		{
			bc[x][y]=0;
		}
	}

    for(i=0;i<g->v;i++)
    {
        isVisited[i]=0;
        //parent[i]=-1;
    }
    for(i=0;i<g->v;i++)
    {
        if(isVisited[i]==0)
        {
            push(i+1);
            bcUtil(g,g->v,bc,i,isVisited,des,parent,low);
			//pop(-1);
        }
    }
    printf("\n");
	FILE *c;
	c = fopen("Ac.txt", "w");
    for (x=0;x<(g->v);x++)
	{   int index = bc[x][x];
        //printf("\n index %d", bc[x][x]);
        while(index>0){
         //printf("\n");
         //printf("\n %d",x+1);
		for (y=0;y<(g->v);y++)
		{

			if(bc[x][y]==index)

			{
			    fprintf(c, "%d ",y+1);
			}
        }
		fprintf(c,"\n");
        bc[x][x]--;
        index--;
		}
	}
	fclose(c);
}

int main(int argc, char *argv[])
{

	FILE *fp;
	/*
	if (argc < 2)
	{
		printf("too few arguments !");
		return 0;
	}*/

	// read input from file
	fp = fopen("C:/Users/admin/Downloads/programming project_documents/project_documents/input_files/in2.txt", "r");

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
      // TODO : please comment the following line
      addEdge(g,src_vertex-1, dst_vertex-1);
			pch = strtok(NULL, " \n\r");
		}

	}

	fclose(fp);

    AP(g);
	BCP(g);

}
