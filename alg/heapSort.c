#include<stdio.h>
#include <string.h>
#define LEN 10
#define MAX 1 << 16 - 1

void swapab(int *a,int *b){
//	printf("swap src a=%d,b=%d\n",*a,*b);
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
void createArrayHeap(int *a,int *b){
	int i,j,k;
	b[0]=a[0];
	for(i=1;i<LEN;i++){
		b[i]=a[i];
		j=i;
		//插入的子节点大于父节点则需交换位置
		while(b[j-1]<b[j/2-1] && j>1){
			swapab(&b[j-1],&b[j/2-1]);
			j/=2;
		}
	}
}
void sortArrayHeap(int *b,int *c){
	int i,j,remind;
	for(i=0;i<LEN;i++){
		remind=LEN-i-1;
//		printf("b[1]=%d remind=%d\n",b[1],remind);
		c[i]=b[0];
		//1. 最后一个元素赋值在第1位
		b[0]=b[remind];
		b[remind]=MAX;
		//2. 重新调整堆
		//2.1 找出定点下的最小的子节点
		j=1;
		while(j*2-1<=remind){
			//2.2 已经是最小值了，不需调整了
			if(b[j-1]<b[j*2-1] && b[j-1]<b[j*2]) break;
			//2.3 父节点下沉
			if(b[j*2-1]<b[j*2]){
				swapab(&b[j-1],&b[j*2-1]);
				j=j*2;
			}else{
				swapab(&b[j-1],&b[j*2]);
				j=j*2+1;
			}
		}
	}
}
//建堆并调整最小堆
void adjust_heap(int *b,int index,int size){
	if(index <= 1 || index >= size) return;
	int curr = index;
	int parent = (curr - 1) >> 1;
	int child = (curr << 1) + 1;
	//当前节点是左还是右
	//int lef = (index & 1);
	//1.调整后当前节点小于父节点,往上调整
	while(b[curr] < b[parent] && parent >= 0){
		swapab(&b[curr],&b[parent]);
		curr=parent;
		parent =(curr - 1) >> 1;
	}
	// 2. 调整后当前值大于子节点，往下调整，要比较左右孩纸
	while(child < size){
		//左孩纸
		if(b[curr] > b[child]){
			swapab(&b[curr],&b[child]);
			curr = child;
			child = (curr << 1) + 1;
		}else if(b[curr] > b[child + 1]){
			swapab(&b[curr],&b[child+1]);
			curr = child + 1;
			child = (curr << 1) + 1;
		}else{
			break;
		}
	}
}

int main(){
	int a[LEN] = {11,4,1,5,2,3,0,8,6,12};
	int b[LEN];
	int c[LEN];
	createArrayHeap(a,b);
	int i;
	printf("\n result : \n");
	for(i=0;i<LEN;i++){
		printf("%d ",b[i]);
	}
	sortArrayHeap(b,c);
	printf("\nsort res:\n");
	for(i=0;i<LEN;i++){
		printf("%d ",c[i]);
	}
	memcpy(b,a,sizeof(int)*LEN);
	printf("\nbefore adjust\n");
	for(i=0;i<LEN;i++){
//		b[i]=a[i];
		printf("%d ",b[i]);
	}	
	printf("\nafter adjust\n");
	for(i=0;i<LEN;i++){
		adjust_heap(b,i,i+1);
	}
	for(i=0;i<LEN;i++){
		printf("%d ",b[i]);
	}
	sortArrayHeap(b,c);
	printf("\nsort res:\n");
	for(i=0;i<LEN;i++){
		printf("%d ",c[i]);
	}
	return 0;
}