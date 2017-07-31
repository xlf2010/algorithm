#include<stdio.h>
#define VEXNUM_X 4
#define VEXNUM_Y 4
#define W 1
typedef enum vex_type{
	COM,START,END,BLOCK
}vex_type;

typedef struct node{
	int x,y,g,h,is_in_open,is_in_close;
	vex_type tp;
	struct node *parent;
}node;
node open_t[VEXNUM_X*VEXNUM_Y];
node close_t[VEXNUM_X*VEXNUM_Y];
node node_map[VEXNUM_X][VEXNUM_Y];
char map[VEXNUM_X][VEXNUM_Y] = {{'S','.','X','.'},{'.','.','X','.'},{'.','.','X','D'},{'.','.','.','.'}};
int open_index=0,close_index=0;
node *start_n,*end_n;

int abs(int a){
	return a>0?a:-a;
}

//计算两点哈密顿距离，即A star 中的h值
int dis_h(node *curr_nd,node *end_nd){
	return abs(curr_nd->x - end_nd->x) + abs(curr_nd->y - end_nd->y);
}

void swap_nd(node *a,node *b){
	node tmp = *a;
	*a=*b;
	*b=tmp;
}

//建堆并调整最小堆
void adjust_heap(int index){
//	if(index <= 1 || index >= open_index) return;
	int curr = index;
	int parent = (curr - 1) >> 1;
	int child = (curr << 1) + 1;
	//当前节点是左还是右
	//int lef = (index & 1);
	//1.调整后当前节点小于父节点,往上调整
	while((open_t[curr].g + open_t[curr].h) < (open_t[parent].g+open_t[parent].h) && parent >= 0){
		swap_nd(&open_t[curr],&open_t[parent]);
		curr=parent;
		parent =(curr - 1) >> 1;
	}
	// 2. 调整后当前值大于子节点，往下调整，要比较左右孩纸
	while(child < open_index){
		//左孩纸
		if((open_t[curr].g + open_t[curr].h) > (open_t[child].g + open_t[child].h )){
			swap_nd(&open_t[curr],&open_t[child]);
			curr = child;
			child = (curr << 1) + 1;
		}else if((open_t[curr].g+open_t[curr].h) > (open_t[child + 1].g+open_t[child + 1].h )){
			swap_nd(&open_t[curr],&open_t[child+1]);
			curr = child + 1;
			child = (curr << 1) + 1;
		}else{
			break;
		}
	}
}

void insert_open_table(node *curr_n,node *neighbor,int w){
	int i;
	if(!neighbor->is_in_close){
		if(neighbor->is_in_open){
			if(neighbor->g>curr_n->g+w){
				node_map[neighbor->x][neighbor->y].g=curr_n->g+w;
				node_map[neighbor->x][neighbor->y].parent=curr_n;
				for(i=0;i<open_index;i++){
					if(open_t[i].x==neighbor->x && open_t[i].y==neighbor->y){
						break;
					}
				}
				adjust_heap(i);
			}
		}else{
			neighbor->parent=curr_n;
			neighbor->is_in_open=1;
			neighbor->h=dis_h(neighbor,end_n);
			open_t[open_index++]=*neighbor;
		}
	}
}

//查找邻居节点
void get_neighbor(node *curr_n){
	int x=curr_n->x;
	int y=curr_n->y;
	//有左节点并且不是障碍物
	if(x-1>=0 && node_map[x-1][y].tp != BLOCK){
		insert_open_table(curr_n,&(node_map[x-1][y]),1);
	}
	if(x+1<VEXNUM_X && node_map[x+1][y].tp != BLOCK){
		insert_open_table(curr_n,&(node_map[x+1][y]),1);
	}
	if(y-1>=0 && node_map[x][y-1].tp != BLOCK){
		insert_open_table(curr_n,&(node_map[x][y-1]),1);
	}
	if(y+1<VEXNUM_Y && node_map[x][y+1].tp != BLOCK){
		insert_open_table(curr_n,&(node_map[x][y+1]),1);
	}
}



int main(){
//	int ei=2,ej=3;
	int i,j,is_found;

	//初始化数据
	for(i=0;i<VEXNUM_X;i++){
		for(j=0;j<VEXNUM_Y;j++){
			node_map[i][j].x=i;
			node_map[i][j].y=j;
			if(map[i][j] == '.'){
				node_map[i][j].tp=COM;
			}else if(map[i][j] == 'X'){
				node_map[i][j].tp=BLOCK;
			}else if(map[i][j] == 'S'){
				node_map[i][j].tp=START;
				start_n=&(node_map[i][j]);
			}else if(map[i][j] == 'D'){
				node_map[i][j].tp=END;
				end_n=&(node_map[i][j]);
			}
			node_map[i][j].g=1;
			node_map[i][j].h=0;
			node_map[i][j].is_in_open=0;
			node_map[i][j].is_in_close=0;
		}
	}
	if(start_n->x == end_n->x && start_n->y == end_n->y){
		printf("start node same with end node ,return ...");
		return 0;
	}
	//1.把起始点加入open table；
	start_n->is_in_open=1;
	start_n->h=dis_h(start_n,end_n);
	open_t[open_index++]=*start_n;
	is_found=0;
	node *curr;
	int k=0;
	while(1){
		curr = &(node_map[open_t[0].x][open_t[0].y]);
		printf("第%d次%d %d\n",++k,curr->x,curr->y);
		curr->is_in_close=1;
		close_t[close_index++]=*curr;
		open_t[0]=open_t[--open_index];
		adjust_heap(0);
		if(curr->x==end_n->x && curr->y == end_n->y){
			is_found=1;
			break;
		}
		get_neighbor(curr);
		if(open_index <=0){break;}
	}
	printf("\nis_found=%d\n",is_found);
    k=0;
	if(is_found){
        curr=end_n;
        while(curr!=start_n){
            printf("(%d,%d)-->",curr->x,curr->y);
            curr=curr->parent;
            if(k++>100){break;}
        }
        printf("(%d,%d)",start_n->x,start_n->y);
	}
	printf("\nk=%d\n",k);
    for(k=0;k<close_index;k++){
        printf("(%d,%d,0x%p)-->",close_t[k].x,close_t[k].y,close_t[k].parent);
    }
	return 0;
}
