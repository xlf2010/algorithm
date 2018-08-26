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

//查找换出基变量,返回矩阵所在的行位置，当出现两个比值都为最小时，选取下标最小的基变量为换出变量
int find_out_vector(matrix_t *st_matrix,int pos,vector_t *feasible_solution,int_arr_t *basic_position){
	double min=-1;
	//换出基变量位置为[pos,j]
	int i,j=-1,k;

	for(i=0;i<basic_position->size;i++){
		// 基可行解中基变量，并且约束矩阵中换入变量位置元素大于0
		// 非基约束向量小于等于0的不用计算
		if(st_matrix->vt[pos].data[i].d <= 0)
			continue;
		double d = feasible_solution->data[basic_position->data[i]].d / st_matrix->vt[pos].data[i].d;
//		printf("pos=%d,t_matrix->vt[pos].data[%d].d=%lf,i=%d,min=%lf,d=%lf\n",pos,k,st_matrix->vt[pos].data[k].d,i,min,d);
		if(min<0 || d>0 && d<min){
			min=d;
			j=i;
		}
	}
	printf("fit out vector position is:%d\n",j);
	return j;
}

// 计算Zj
double calc_check_param(vector_t *dest,matrix_t *st_matrix,int pos,int_arr_t *basic_position){
	double res=0;
	int i;
	for(i=0;i<basic_position->size;i++){
		//非基变量不参与计算
		res += (dest->data[basic_position->data[i]].d * st_matrix->vt[pos].data[i].d);
	}
	return res;
}

//查找换入基变量位置,CB-Aij
int find_in_vector(vector_t *dest,matrix_t *st_matrix,int_arr_t *basic_position,vector_t *check_param){
	//检验参数最大值
	double max;
	int i,pos=-1;
	for(i=0;i<st_matrix->n;i++){
		data_t dt;
		//检验参数 Cj-Zj
		dt.d=dest->data[i].d-calc_check_param(dest,st_matrix,i,basic_position);
		dt.basic_flag=NOT_BASIC_DATA;
		if(dt.d>max){
			max=dt.d;
			pos=i;
		}
		check_param->data[i]=dt;
	}
	if(max<0){
		printf("all not basic dest are less than 0\n");
	}
	printf("fit in vector position is:%d\n",pos);
	return pos;
}

// 对矩阵中主元位置st_matrix[in][out]的元素行初等变换
// 当前行与主元所在的行一致，该行所有元素除主元值，将主元值化成1。如果不是则将主元对应的列向量其他元素换成0，使主元所在的列向量为：主元为1，其余元素为0的列向量
int matrix_element_transform(vector_t *dest,matrix_t *st_matrix,int in,int out,vector_t *feasible_solution,int_arr_t *basic_position){
	
	// 1.查找换出基向量
	double pivot=st_matrix->vt[in].data[out].d;

	int i,j;
	//主元所在的行除以主元的值，将主元化成1
	for(i=0;i<st_matrix->n;i++){
		st_matrix->vt[i].data[out].d/=pivot;
	}
	
	// 解向量也需变换
	feasible_solution->data[basic_position->data[out]].d/=pivot;

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
		feasible_solution->data[basic_position->data[i]].d -= (d * feasible_solution->data[basic_position->data[out]].d);
	}

	//解向量出入替换，基标志位替换
	feasible_solution->data[in].basic_flag = BASIC_DATA;
	feasible_solution->data[basic_position->data[out]].basic_flag = NOT_BASIC_DATA;
	double d = feasible_solution->data[in].d;
	feasible_solution->data[in].d = feasible_solution->data[basic_position->data[out]].d;
	feasible_solution->data[basic_position->data[out]].d = d;
	
	st_matrix->vt[in].basic_flag=BASIC_DATA;
	st_matrix->vt[basic_position->data[out]].basic_flag=NOT_BASIC_DATA;
	basic_position->data[out]=in;

	return 0;
}

// 计算解，目标函数非基变量不参与计算，Z=CX= 
double find_solution(vector_t *dest,int_arr_t *basic_position,vector_t *feasible_solution){
	double res = 0;
	int i;
	// 将基向量所在的值
	for(i=0;i<basic_position->size;i++){
		res += (dest->data[basic_position->data[i]].d * feasible_solution->data[basic_position->data[i]].d);
	}
	return res;
}

int check_vector_all_less_d(vector_t *vt,double d){
	int i;
	for(i=0;i<vt->m;i++){
		if(vt->data[i].d>d)	return 0;
	}
	return 1;
}

/* 检查检验参数，判断是否有解，达到最优解，无穷解，无界解等等
 1. 所有检验参数都小于等于0，非基变量小于0，则达到最优解
 2. 所有检验参数都小于等于0，存在某个非基变量检验参数等于0，则有无穷解
 3. 存在某个变量大于0，该变量对应的约束矩阵相应的分向量的所有值都小于0，则是无界解
 4. 存在某个变量大于0，该变量对应的约束矩阵相应的分量有大于0情况，则需继续变换
 5. 存在人工变量不为0，暂是没有考虑人工变量情况，不考虑无解
*/
 int check_soultion(vector_t *check_param,matrix_t *st_matrix){
	int i,can_do_next = 0,endless_soultion=0 ;
	for(i=0;i<check_param->m;i++){
		// 存在检验参数大于0，判断所在的约束矩阵分向量情况，全部小于0，返回无界解
		if(check_param->data[i].d>0){
			if(check_vector_all_less_d(st_matrix->vt + i,0))
				return UNBOUNDED_SOULTION;
			else 
				can_do_next = CAN_DO_NEXT;
		}
		//存在检验参数等于0，如果是非基变量，则有无穷解
		if(check_param->data[i].d == 0 && st_matrix->vt[i].basic_flag != BASIC_DATA){
			endless_soultion = ENDLESS_SOULTION;
		}
	}
	// 如果全部参数都小于0，则有最优解
	return can_do_next ? CAN_DO_NEXT : endless_soultion ? ENDLESS_SOULTION : FIT_SOULTION;
}



int main(int argc,char **argv){
	if(argc > 1){
		file_name = argv[1];
	}
	
    //目标函数系数向量
    vector_t *dest = (vector_t *)malloc(sizeof(vector_t));
    // 约束矩阵
    matrix_t *st_matrix = (matrix_t *)malloc(sizeof(matrix_t));;
    //基可行解
    vector_t *feasible_solution = (vector_t *)malloc(sizeof(vector_t));
	
	//检验参数 
	vector_t *check_param = (vector_t *)malloc(sizeof(vector_t));
	
	// 基向量位于矩阵的位置 
	int_arr_t *basic_position = (int_arr_t *)malloc(sizeof(int_arr_t));

	// 文件读取数据
	read_data(dest,st_matrix,feasible_solution,basic_position,check_param);
	
	int cnt=1,in,out,check_soultion_res;
	//迭代查找解
	while(1){
		printf("\nNO:%d transform\n",cnt++);
		print_vector(dest);
		print_matrix(st_matrix);
		print_vector(feasible_solution);
		print_array(basic_position);
		in=find_in_vector(dest,st_matrix,basic_position,check_param);
		check_soultion_res = check_soultion(check_param,st_matrix);
		if(check_soultion_res != CAN_DO_NEXT) break;
		out=find_out_vector(st_matrix,in,feasible_solution,basic_position);
		printf("change pivot is in=%d,out=%d,val=%lf,res=%lf\n",in,out,st_matrix->vt[in].data[out].d,find_solution(dest, basic_position,feasible_solution));
		matrix_element_transform(dest,st_matrix,in,out,feasible_solution,basic_position);
		printf("\nafter matrix element tranform :\n");
		print_matrix(st_matrix);
		print_array(basic_position);
		print_vector(feasible_solution);
		print_vector(check_param);
	}
	
	//解判断
	switch(check_soultion_res){
		case FIT_SOULTION : 
			printf("find fitness soultion, answer is : %lf\n",find_solution(dest, basic_position,feasible_solution));
			print_vector(feasible_solution);
			break;
		case ENDLESS_SOULTION :
			printf("find endless soultion, one of is %lf\n",find_solution(dest, basic_position,feasible_solution));
			print_vector(feasible_solution);
			break;
		case UNBOUNDED_SOULTION :
			printf("UNBOUNDED_SOULTION\n");
			break;
		case NO_SOULTION : 
			printf("NO_SOULTION \n");
			break;
		default :
			printf("unknow soultion type \n");
		}
	return 0;
}
