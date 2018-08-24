#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
/*
    线性规划C语言
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

//
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


char *file_name="linear_programming_data_1.txt";

int read_vector(FILE *f,vector_t *vt,int basic_start,int basic_end,const char *name){
	int i;
	vt->name=(char *)malloc(sizeof(char) * strlen(name));
	memcpy(vt->name,name,strlen(name));
	//读取向量维数，基向量起始结束位置
	vt->data = (data_t *)malloc(sizeof(data_t *) * vt->m);
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

int read_data(vector_t *dest,matrix_t *st_matrix,vector_t *feasible_solution,int_arr_t *basic_position){
    FILE *f=fopen(file_name,"r");
    int i,j,basic_start,basic_end;
    // 1. 读取目标函数非基变量
	// 读取向量维数，基向量起始结束位置
    fscanf(f,"%d%d%d",&dest->m,&basic_start,&basic_end);
	basic_position->name="basic position arr";
	basic_position->size=basic_end - basic_start + 1;
	basic_position->data=(int *)malloc(sizeof(int) * basic_position->size);
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
    fscanf(f,"%d",&feasible_solution->m);
	basic_start=basic_end=-1;	
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
	printf("\n\narray name : %s\n",arr->name);
	int i,size=arr->size;
	for(i=0;i<size;i++){
		printf("%d ",arr->data[i]);
	}
	printf("\n\n");
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
    printf("\n");
}

//查找换出基变量,返回矩阵所在的行位置
int find_out_vector(matrix_t *st_matrix,int pos,vector_t *feasible_solution){
	double min=-1;
	//换出基变量位置为[pos,j]
	int i,j=-1;

	for(i=0;i<feasible_solution->m;i++){
		// 基可行解中基变量，并且约束矩阵中换入变量位置元素大于0
		// 非基约束向量小于等于0的不用计算
		if(st_matrix->vt[pos].data[i].d <= 0)
			continue;
		double d = feasible_solution->data[i].d / st_matrix->vt[pos].data[i].d;
//		printf("pos=%d,t_matrix->vt[pos].data[%d].d=%lf,i=%d,min=%lf,d=%lf\n",pos,k,st_matrix->vt[pos].data[k].d,i,min,d);
		if(min<0 || d>0 && d<min){
			min=d;
			j=i;
		}
	}
	return j;
}

// 计算Zj
double calc_check_param(vector_t *dest,matrix_t *st_matrix,int pos){
	double res=0;
	int i;
	for(i=0;i<dest->m;i++){
		//非基变量不参与计算
		if(dest->data[i].basic_flag != BASIC_DATA)
			continue;
		res += (dest->data[i].d * st_matrix->vt[pos].data[i].d);
	}
	return res;
}

//查找换出基变量位置,CB-Aij
int find_in_vector(vector_t *dest,matrix_t *st_matrix){
	//检验参数最大值
	double check_param[dest->m];
	double max;
	int i,pos=-1;
	for(i=0;i<dest->m;i++){
		//检验参数 Cj-Zj
		check_param[i]=dest->data[i].d-calc_check_param(dest,st_matrix,i);
		if(check_param[i]>max){
			max=check_param[i];
			pos=i;
		}
	}
	if(max<0){
		printf("all not basic dest are less than 0\n");
	}
	printf("fit vector position is:%d\n",pos);
	return pos;
}

int comp_basic_pos(const void* bp1,const void* bp2){
	return *(int *)bp1-*(int *)bp2;
}

// 对矩阵中主元位置st_matrix[in][out]的元素行初等变换
int matrix_element_transform(vector_t *dest,matrix_t *st_matrix,int in,int out,vector_t *feasible_solution,int_arr_t *basic_position){
	// 1.查找换出基向量
	double pivot=st_matrix->vt[in].data[out].d;

	int i,j;
	//主元所在的行除以主元的值，将主元化成1
	for(i=0;i<st_matrix->n;i++){
		st_matrix->vt[i].data[out].d/=pivot;
	}

	// 向量维数,按行向量进行变换
	int matrix_m=st_matrix->vt[in].m;
	for(i=0;i<matrix_m;i++){
		// 所在的行等于换出的行所在，前面已处理
		if(i==out) continue;
		//主元所在的列向量m对应的第i个元素
		double d=st_matrix->vt[in].data[i].d;
		for(j=0;j<st_matrix->n;j++){
			st_matrix->vt[j].data[i].d -= (d * st_matrix->vt[j].data[out].d);
		}
	}

/*
	for(i=0;i<st_matrix->n;i++){
		double d=st_matrix->vt[i].data[n].d;
		for(j=0;j<st_matrix->vt[i].m;j++){
// 当前行与主元所在的行一致，该行所有元素除主元值，将主元值化成1。如果不是则将主元对应的列向量其他元素换成0，使主元所在的列向量为：主元为1，其余元素为0的列向量
			if(j!=n){
				 st_matrix->vt[i].data[j].d -= (d * st_matrix->vt[m].data[j].d);
			}
		}
	}
*/
	st_matrix->vt[in].basic_flag=1;
	st_matrix->vt[basic_position->data[out]].basic_flag=0;
	basic_position->data[out]=in;
	//交换位置后基在矩阵位置重新排序
	qsort(basic_position->data,basic_position->size,sizeof(int),comp_basic_pos);
	return 0;
}

double find_solution(vector_t *dest,int_arr_t *basic_position){

}

int main(int argc,char **argv){
    //目标函数系数向量
    vector_t *dest = (vector_t *)malloc(sizeof(vector_t));
    // 约束矩阵
    matrix_t *st_matrix = (matrix_t *)malloc(sizeof(matrix_t));;
    //基可行解
    vector_t *feasible_solution = (vector_t *)malloc(sizeof(vector_t));
	// 基向量位于矩阵的位置 
	int_arr_t *basic_position = (int_arr_t *)malloc(sizeof(int_arr_t));

	// 文件读取数据
	read_data(dest,st_matrix,feasible_solution,basic_position);

    print_vector(dest);
    print_matrix(st_matrix);
    print_vector(feasible_solution);
	print_array(basic_position);

	int in=find_in_vector(dest,st_matrix);
	int out=find_out_vector(st_matrix,in,feasible_solution);
	printf("change pivot is in=%d,out=%d,val=%lf\n",in,out,st_matrix->vt[in].data[out].d);
	matrix_element_transform(dest,st_matrix,in,out,feasible_solution,basic_position);	
	printf("after matrix element tranform :\n");
	print_matrix(st_matrix);
	print_array(basic_position);
	


	return 0;
}
