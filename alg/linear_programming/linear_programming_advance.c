#include<stdio.h>
#include<math.h>
#include<stdlib.h>
/*
    线性规划C语言，改进单纯型法，基于矩阵计算，
	不需计算约束矩阵的每个元素,即非基变量中不属于代入基变量的各列数字在迭代中没有用到，当非基变量越多时，计算量减少越明显
    求解松弛型线性规划

    例如：
    max z=4x1+2x2
    st: -x1+2x2<=6
         x1+x2 <=9
        3x1-x2 <=15
         x1,x2>=0

    1.化成松弛型有：其中x3,x4,x5为剩余变量
    max z=4x1+2x2+0x3+0x4+0x5
    st: -x1+2x2+x3       =6
         x1+x2    +x4    =9
        3x1-x2       +x5 =15
        x1,x2,x3,x4,x5>=0


*/

#define BASIC_DATA 1
#define NOT_BASIC_DATA 0

// 检查检验参数，判断是否有解，达到最优解，无穷解，无界解等等
// 1. 所有检验参数都小于等于0，非基变量小于0，则达到最优解
// 2. 所有检验参数都小于等于0，存在某个非基变量检验参数等于0，则有无穷解
// 3. 存在某个变量大于0，该变量对应的约束矩阵对应的分向量的所有值都小于0，则是无界解
// 4. 

//解定义
#define CAN_DO_NEXT 1				//未达到最优解，继续执行
#define FIT_SOULTION 2 				//存在最优解
#define ENDLESS_SOULTION 3			//无穷解
#define UNBOUNDED_SOULTION 4		//无界解
#define NO_SOULTION 5				//无解

//定义整型数组
typedef struct {
	double d;			// 数据
	int basic_flag;		// 是否对应为基变量
}data_t;

typedef struct{
	char *name;
	int size;
	int *data;
}int_arr_t;

//定义向量结构体
typedef struct {
    char *name;		//向量名称
    int m;			//向量维数
    data_t *data;	//向量值
	int basic_flag;	//是否为基向量
}vector_t;

// 定义矩阵结构体
typedef struct{
    char *name;		//矩阵名称
    int n;			//矩阵向量个数
    vector_t *vt;	//向量值
}matrix_t;


char *file_name="linear_programming_data_unbounded.txt";

int read_vector(FILE *f,vector_t *vt,int basic_start,int basic_end,const char *name){
	int i;
	vt->name=(char *)malloc(sizeof(char) * strlen(name));
	memcpy(vt->name,name,strlen(name));
	//读取向量维数，基向量起始结束位置
	vt->data = (data_t *)malloc(sizeof(data_t) * vt->m);
	for(i=0;i<vt->m;i++){
		data_t dt;
		fscanf(f,"%lf",&dt.d);
		if(i>=basic_start && i<=basic_end){
			dt.basic_flag=BASIC_DATA;
		}else{
			dt.basic_flag=NOT_BASIC_DATA;
		}
		vt->data[i]=dt;
	}


	return 0;
}

int read_data(vector_t *dest,matrix_t *st_matrix,vector_t *feasible_solution,int_arr_t *basic_position,vector_t *check_param){
    FILE *f=fopen(file_name,"r");
    int i,j,basic_start,basic_end;
    // 1. 读取目标函数非基变量
	// 读取向量维数，基向量起始结束位置
    fscanf(f,"%d%d%d",&dest->m,&basic_start,&basic_end);
	basic_position->name="basic position arr";
	basic_position->size=basic_end - basic_start + 1;
	basic_position->data=(int *)malloc(sizeof(int) * basic_position->size);
	check_param->name="check_param vector";
	check_param->m=dest->m;
	check_param->data=(data_t *)malloc(sizeof(data_t) * dest->m);
	
	read_vector(f,dest,basic_start,basic_end,"dest");
	// 2. 读取约束条件矩阵
	int m,k=0;
	fscanf(f,"%d%d%d%d",&m,&st_matrix->n,&basic_start,&basic_end);
    st_matrix->name="st_matrix";
    st_matrix->vt=(vector_t *)malloc(sizeof(vector_t) * st_matrix->n);
	 
	for(i=0;i<st_matrix->n;i++){
		vector_t vt;
		vt.m=m;
		vt.data=(data_t *)malloc(sizeof(data_t) * m);
		//		read_vector(f,vt,basic_start,basic_end,"");
		if(i>=basic_start && i<=basic_end){
			vt.basic_flag=BASIC_DATA;
			basic_position->data[k++]=i;
		}else{
			vt.basic_flag=NOT_BASIC_DATA;
		}
		st_matrix->vt[i] = vt;
	}
	
	for(i=0;i<m;i++){
		for(j=0;j<st_matrix->n;j++){
			data_t dt;
			fscanf(f,"%lf",&dt.d);
			st_matrix->vt[j].data[i]=dt;
		}
	}

    //3 读取方程组系数，即初始解
    fscanf(f,"%d%d%d",&feasible_solution->m,&basic_start,&basic_end);	
	read_vector(f,feasible_solution,basic_start,basic_end,"feasible_solution");
	fclose(f);
    return 0;
}

void print_vector(vector_t *v){
    int i;
    printf("vector name : %s\n   ",v->name);
    for(i=0;i<v->m;i++){
        printf("%lf,%d ",v->data[i].d,v->data[i].basic_flag);
    }
    printf("\n");
}

void print_array(int_arr_t *arr){
	printf("\narray name : %s\n",arr->name);
	int i,size=arr->size;
	for(i=0;i<size;i++){
		printf("%d ",arr->data[i]);
	}
	printf("\n");
}

void print_matrix(matrix_t *mx){
    int i,j;
    printf("matrix name : %s\n",mx->name);
    for(i=0;i<mx->n;i++){
		vector_t *vt=mx->vt+i;
		printf("basic_flag:%d ",vt->basic_flag);
        for(j=0;j<vt->m;j++){
            printf("%lf ",vt->data[j].d);
        }
        printf("\n");
    }
}


//两个向量交换元素
int switch_element_in_vector(vector_t *t1,int pos1,vector_t *t2,int pos2){
	double d=t1->data[pos1];
	t1->data[pos1]=t2->data[pos2];
	t2->data[pos2]=d;
}

// 矩阵两列对换
int switch_vector_in_matrix(matrix_t *t1,int pos1,matrix_t *t2,int pos2){
	if(pos1>=t1->n){
		printf("pos1 %d is greater then matrix t1.n %d \n",pos1,t1->n);
		return -1;
	}
	if(pos2>=t2->n){
		printf("pos2 %d is greater then matrix t2.n %d \n",pos2,t2->n);
		return -1;
	}
	
	int i;
	for (i=0;i<t1->m;i++){
		double d=t1->data[pos1][i];
		t1->data[pos1][i]=t2->data[pos2][i];
		t2->data[pos2][i]=d;
	}
	return 0;
}

// 查找换入基变量,在目标函数非基变量中查找大于0的最大值的位置
int find_in_vector(vector_t *dest){
	int i,pos=-1;
	double max=0;
	for(i=0;i<dest->len;i++){
		if(dest->data[i]>max){
			max=dest->data[i];
			pos=i;
		}
	}
	return pos;
}

//查找换出基变量
//basic_matrix 为基矩阵，in_pos_n为换入基变量的列,即not_basic_matrix所在的列向量,
void find_out_vector(matrix_t *basic_matrix,matrix_t *not_basic_matrix,int in_pos_n,vector_t *feasible_solution){
	
}

int main(int argc,char **argv){
    //非基变量在目标函数系数向量
    vector_t *dest = (vector_t *)malloc(sizeof(vector_t));
    //基变量在目标函数系数向量
    vector_t *dest_basic = (vector_t *)malloc(sizeof(vector_t));
    //非基矩阵
    matrix_t *not_basic_matrix = (matrix_t *)malloc(sizeof(matrix_t));
    // 基矩阵
    matrix_t *basic_matrix = (matrix_t *)malloc(sizeof(matrix_t));;
    //基可行解
    vector_t *feasible_solution = (vector_t *)malloc(sizeof(vector_t));
    
	// 文件读取数据
	read_data(dest,dest_basic,not_basic_matrix,basic_matrix,feasible_solution);

    print_vector(dest);
    print_vector(dest_basic);
    print_matrix(not_basic_matrix);
    print_matrix(basic_matrix);
    print_vector(feasible_solution);
	return 0;
}
