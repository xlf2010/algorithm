#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
/*
    ���Թ滮C����
    ����ɳ������Թ滮

    ���磺
    max z=4x1+2x2
    st: -x1+2x2<=6
         x1+x2 <=9
        3x1-x2 <=15
         x1,x2>=0

    1.�����ɳ����У�����x3,x4,x5Ϊʣ�����
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
	double d;			// ����
	int basic_flag;		// �Ƿ��ӦΪ������
}data_t;

typedef struct{
	char *name;
	int size;
	int *data;
}int_arr_t;

//���������ṹ��
typedef struct {
    char *name;		//��������
    int m;			//����ά��
    data_t *data;	//����ֵ
	int basic_flag;	//�Ƿ�Ϊ������
}vector_t;

// �������ṹ��
typedef struct{
    char *name;		//��������
    int n;			//������������
    vector_t *vt;	//����ֵ
}matrix_t;


char *file_name="linear_programming_data_1.txt";

int read_vector(FILE *f,vector_t *vt,int basic_start,int basic_end,const char *name){
	int i;
	vt->name=(char *)malloc(sizeof(char) * strlen(name));
	memcpy(vt->name,name,strlen(name));
	//��ȡ����ά������������ʼ����λ��
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
    // 1. ��ȡĿ�꺯���ǻ�����
	// ��ȡ����ά������������ʼ����λ��
    fscanf(f,"%d%d%d",&dest->m,&basic_start,&basic_end);
	basic_position->name="basic position arr";
	basic_position->size=basic_end - basic_start + 1;
	basic_position->data=(int *)malloc(sizeof(int) * basic_position->size);
	read_vector(f,dest,basic_start,basic_end,"dest");
	// 2. ��ȡԼ����������
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

    //3 ��ȡ������ϵ��������ʼ��
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

//���һ���������,���ؾ������ڵ���λ��
int find_out_vector(matrix_t *st_matrix,int pos,vector_t *feasible_solution){
	double min=-1;
	//����������λ��Ϊ[pos,j]
	int i,j=-1;

	for(i=0;i<feasible_solution->m;i++){
		// �����н��л�����������Լ�������л������λ��Ԫ�ش���0
		// �ǻ�Լ������С�ڵ���0�Ĳ��ü���
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

// ����Zj
double calc_check_param(vector_t *dest,matrix_t *st_matrix,int pos){
	double res=0;
	int i;
	for(i=0;i<dest->m;i++){
		//�ǻ��������������
		if(dest->data[i].basic_flag != BASIC_DATA)
			continue;
		res += (dest->data[i].d * st_matrix->vt[pos].data[i].d);
	}
	return res;
}

//���һ���������λ��,CB-Aij
int find_in_vector(vector_t *dest,matrix_t *st_matrix){
	//����������ֵ
	double check_param[dest->m];
	double max;
	int i,pos=-1;
	for(i=0;i<dest->m;i++){
		//������� Cj-Zj
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

// �Ծ�������Ԫλ��st_matrix[in][out]��Ԫ���г��ȱ任
int matrix_element_transform(vector_t *dest,matrix_t *st_matrix,int in,int out,vector_t *feasible_solution,int_arr_t *basic_position){
	// 1.���һ���������
	double pivot=st_matrix->vt[in].data[out].d;

	int i,j;
	//��Ԫ���ڵ��г�����Ԫ��ֵ������Ԫ����1
	for(i=0;i<st_matrix->n;i++){
		st_matrix->vt[i].data[out].d/=pivot;
	}

	// ����ά��,�����������б任
	int matrix_m=st_matrix->vt[in].m;
	for(i=0;i<matrix_m;i++){
		// ���ڵ��е��ڻ����������ڣ�ǰ���Ѵ���
		if(i==out) continue;
		//��Ԫ���ڵ�������m��Ӧ�ĵ�i��Ԫ��
		double d=st_matrix->vt[in].data[i].d;
		for(j=0;j<st_matrix->n;j++){
			st_matrix->vt[j].data[i].d -= (d * st_matrix->vt[j].data[out].d);
		}
	}

/*
	for(i=0;i<st_matrix->n;i++){
		double d=st_matrix->vt[i].data[n].d;
		for(j=0;j<st_matrix->vt[i].m;j++){
// ��ǰ������Ԫ���ڵ���һ�£���������Ԫ�س���Ԫֵ������Ԫֵ����1�������������Ԫ��Ӧ������������Ԫ�ػ���0��ʹ��Ԫ���ڵ�������Ϊ����ԪΪ1������Ԫ��Ϊ0��������
			if(j!=n){
				 st_matrix->vt[i].data[j].d -= (d * st_matrix->vt[m].data[j].d);
			}
		}
	}
*/
	st_matrix->vt[in].basic_flag=1;
	st_matrix->vt[basic_position->data[out]].basic_flag=0;
	basic_position->data[out]=in;
	//����λ�ú���ھ���λ����������
	qsort(basic_position->data,basic_position->size,sizeof(int),comp_basic_pos);
	return 0;
}

double find_solution(vector_t *dest,int_arr_t *basic_position){

}

int main(int argc,char **argv){
    //Ŀ�꺯��ϵ������
    vector_t *dest = (vector_t *)malloc(sizeof(vector_t));
    // Լ������
    matrix_t *st_matrix = (matrix_t *)malloc(sizeof(matrix_t));;
    //�����н�
    vector_t *feasible_solution = (vector_t *)malloc(sizeof(vector_t));
	// ������λ�ھ����λ�� 
	int_arr_t *basic_position = (int_arr_t *)malloc(sizeof(int_arr_t));

	// �ļ���ȡ����
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
