#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAX 1 << 16
typedef struct {
	double x,y;
}point;
point p[100000];
int a[100000];

double dis(point *p1,point* p2){
	return sqrt((p1->x-p2->x)*(p1->x-p2->x)+(p1->y-p2->y)*(p1->y-p2->y));
}

double min(double a,double b){
	return a<b?a:b;
}

int compx(const void *a,const void *b){
	return ((point *)a)->x-((point *)b)->x;
}
int compy(const void *a,const void *b){
//	int *aa=((int *)a);
//	int *bb=((int *)b);
	return p[*((int *)a)].y-p[*((int *)b)].y;
}

double closest(int low,int high){
	if(low+1==high){
		return dis(&p[low],&p[high]);
	}
	if(low+2==high){
		return min(dis(&p[low],&p[high]),min(dis(&p[low],&p[low+1]),dis(&p[low+1],&p[high])));
	}
	int mid=(low+high)>>1;
	double ans=min(closest(low,mid),closest(mid+1,high));
	int i,j,cnt=0;
	//找出正负ans的点
	for(i=low;i<=high;i++){
		if(p[i].x-p[mid].x<=ans||p[mid].x-p[i].x<=ans){
			a[cnt++]=i;
		}
//		printf("%d %d\n",a[cnt-1],cnt);
	}
	qsort(a,cnt,sizeof(int),compy);
	for(i=0;i<cnt;i++){
		for(j=i+1;j<cnt;j++){
			if(p[a[j]].y-p[a[i]].y>=ans) break;
			ans=min(ans,dis(&p[a[i]],&p[a[j]]));
		}
	}
	return ans;
}



int main(){
	int n,i,j;
	double min,d;
	while(1){
		scanf("%d",&n);
		if(n==0) break;
		min=MAX;
		for(i=0;i<n;i++){
			scanf("%lf%lf",&p[i].x,&p[i].y);
		}
		qsort(p,n,sizeof(point),compx);
		min=closest(0,n-1)/2;
		printf("%.2lf\n",min);
	}
	return 0;
}
