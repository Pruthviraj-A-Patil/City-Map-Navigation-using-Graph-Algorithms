#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define INF 9999

typedef struct gnode{
    int index;
    int dist;
    struct gnode * next;
}GNODE;

GNODE *adjlist[20];
int visit[10] = {0};
char  cities[15][20];

void creategraph(int);
void displaygraph(int);
int shortestpath(int , int, int);
int mindist(int [], int [], int);
void printpath(int [], int);
void roundtrip(void);


void main(){
    int n,choice;
    printf("WELCOME TO MAP!!!\n");
    
    printf("Enter the number of city for your map:\n");
    
    scanf("%d",&n);
    creategraph(n);

    while(1){
        printf("\nEnter\n 1->Display\n 2->Shortest Path\n 3->Total round trip cost\n 4->Exit\n");  // Fixed
        scanf("%d",&choice);

        switch(choice){
            case 1:{
                displaygraph(n);
            }break;
            case 2:{
                int src, dest;
                printf("Enter source city index and destination city index: ");
                scanf("%d %d", &src, &dest);

                if(src < 0 || src >= n || dest < 0 || dest >= n){
                    printf("Invalid city indices!\n");
                    break;
                }
                int distance = shortestpath(n, src, dest);
                
                if(distance == -1){
                    printf("No path exists between %s and %s\n", cities[src], cities[dest]);
                } else {
                    printf("Shortest distance from %s to %s is: %d km\n", 
                           cities[src], cities[dest], distance);
                }
            }break;
            case 3:{
                roundtrip();
            }break;
            default:exit(0);
        }
    }
}

void creategraph(int n){

    for(int i = 0; i < n; i++){
        adjlist[i] = NULL;
    }
    printf("\nEnter names of %d cities:\n", n);
    for(int i = 0; i < n; i++){
        printf("City %d: ", i);
        scanf("%s", cities[i]);
    }
    printf("\nEnter connections (source city, destination city, distance):\n");
    printf("Enter '-1 -1  0' to stop\n");
    
    while(1){
        int src, dest, distance;
        printf("Enter connection: ");
        scanf("%d %d %d", &src, &dest, &distance);
        
        if(src == -1 || dest == -1){
            break;
        }
        
        if(src < 0 || src >= n || dest < 0 || dest >= n){
            printf("Invalid city index! Try again.\n");
            continue;
        }
        GNODE *newnode = (GNODE*)malloc(sizeof(GNODE));
        newnode->index = dest;
        newnode->dist = distance;
        newnode->next = adjlist[src];
        adjlist[src] = newnode;
        
        GNODE *newnode2 = (GNODE*)malloc(sizeof(GNODE));
        newnode2->index = src;
        newnode2->dist = distance;
        newnode2->next = adjlist[dest];
        adjlist[dest] = newnode2;
    }
    
    printf("\nGraph created successfully!\n");

}


int shortestpath(int n,int src,int dest){
   int dist[10];
   int visited[10];
   int parent[10];
   int m,v,weight;
   
   
   for(int i=0;i<n;i++){
    dist[i]=INF;
    visited[i]=0;
    parent[i]=-1;
   }
   dist[src]=0;

   for(int count=0;count<n;count++){
    m=mindist(dist,visited,n);
    if (m == -1) {
        break;
    }
    visited[m]=1;

    GNODE *temp = adjlist[m];
    while(temp!=NULL){
        v = temp->index;
        weight = temp->dist;
        if(!visited[v] && dist[m]+weight<dist[v]){
            dist[v] = dist[m] + weight;
            parent[v] = m;
        }
        temp=temp->next;
    }
   }
   if(dist[dest]==INF){
    return -1;
   }
   return dist[dest];
   
}


int mindist(int dist[], int visited[], int n) {
    int min = INF;
    int index = -1;

    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[i] < min) {
            min = dist[i];
            index = i;
        }
    }
    return index;
}


void printpath(int parent[], int v) {
    if (v == -1)
        return;

    printpath(parent, parent[v]);
    printf("%s ", cities[v]);
}


void displaygraph(int n)
{
    for(int i=0;i<n;i++)
    {
        printf("%d\t%s\t->",i,cities[i]);
        GNODE* temp=adjlist[i];

        while(temp!=NULL)
        {
            printf("[%s,%d]\t->",cities[temp->index],temp->dist);
            temp=temp->next;
        }                                 
        printf("NULL");
        printf("\n");
    }
}



void roundtrip()
{
    int srcIndex = -1, destIndex = -1;
    float mileage, fuelprice;
    char src[50], dest[50];

    printf("Enter source city: ");
    scanf("%s", src);
    printf("Enter destination city: ");
    scanf("%s", dest);
    printf("Enter vehicle mileage in km per litre: ");
    scanf("%f", &mileage);
    printf("Enter fuel price per litre: ");
    scanf("%f", &fuelprice);

    if(fuelprice <= 0 || mileage <= 0){
        printf("Invalid info - price & mileage Must be > 0.\n");
        return;
    }
    
    // Find city indices
    for(int i = 0; i < 20; i++){
        if(adjlist[i] == NULL) break;
        if(strcmp(cities[i], src) == 0){
            srcIndex = i;
        }
        if(strcmp(cities[i], dest) == 0){
            destIndex = i;
        }
    }
    
    if(srcIndex == -1 || destIndex == -1){
        printf("City not found!\n");
        return;
    }
    
    int shortdist = shortestpath(20, srcIndex, destIndex);
    
    if(shortdist == -1){
        printf("No path exists between %s and %s\n", src, dest);
        return;
    }
    
    float rounddistance = shortdist * 2;
    float fuelneeded = rounddistance / mileage;
    float totalcost = fuelneeded * fuelprice;

    printf("\nROUND TRIP REPORT\n");
    printf("Source: %s\n", src);
    printf("Destination: %s\n", dest);
    printf("Shortest distance: %d km\n", shortdist);
    printf("Roundtrip distance: %.2f km\n", rounddistance);
    printf("Fuel needed: %.2f litres\n", fuelneeded);
    printf("Total round-trip cost: ₹%.2f\n", totalcost);
}
