#include<stdio.h>
#include<stdlib.h>
#define VEXNUM_X 7
#define VEXNUM_Y 7
typedef enum vex_type{
	COM,START,END,BLOCK
}vex_type;

typedef struct node{
	int x,y,is_visit;
	vex_type tp;
//	struct node *parent;
}node;

node node_map[VEXNUM_X][VEXNUM_Y];

int abs(int a){return a>0?a:-a;}
int m,n,t,flag=0;
node *start_n,*end_n;
int forward[4][2]={{-1,0},{1,0},{0,-1},{0,1}};

void dfs(node *curr,int r_t){
	int i;
	int x,y;

    if(curr->x==end_n->x && curr->y==end_n->y && t==cnt){
        flag=1;
        return ;
    }
    if(cnt > t) {
        flag=0;
        return ;
    }
    //4¸ö·½ÏòÅÐ¶Ï
    for(i=0;i<4;i++){
        x=curr->x+forward[i][0];
        y=curr->y+forward[i][1];
        if(x>=0 && x<n && y>=0 && y<m && node_map[x][y].tp!=BLOCK && node_map[x][y].is_visit!=1){
            node_map[x][y].is_visit=1;
            dfs(&(node_map[x][y]),r_t--);
            node_map[x][y].is_visit=0;
            if(flag) return;
        }
    }
}

int main(){
   char c;
   int i,j;

//   FILE *f=fopen("D:\\code\\gcc\\1010.txt","r");
   while(scanf("%d%d%d",&n,&m,&t)!=EOF){
	   if (m==0&&n==0&&t==0) break;
		scanf("%c",&c);
		for(i=0;i<n;i++){
			for(j=0;j<=m;j++){
				scanf("%c",&c);
				if(c=='\n') break;
				node_map[i][j].x=i;
				node_map[i][j].y=j;
				if(c == '.'){
					node_map[i][j].tp=COM;
				}else if(c == 'X'){
					node_map[i][j].tp=BLOCK;
				}else if(c == 'S'){
					node_map[i][j].tp=START;
					start_n=&(node_map[i][j]);
				}else if(c == 'D'){
					node_map[i][j].tp=END;
					end_n=&(node_map[i][j]);
				}
				node_map[i][j].is_visit=0;
			}
		}
		start_n->is_visit=1;
        if((abs(start_n->x-end_n->x) + abs(start_n->y-end_n->y) - t) & 1){
            printf("NO\n");
            continue;
        }
		dfs(start_n,t);
		if(flag){
            printf("YES\n");
		}else{
            printf("NO\n");
		}
//       printf("%d\n",dfs(start_n,0));
   }

//   fclose(f);
   return 0;
}
