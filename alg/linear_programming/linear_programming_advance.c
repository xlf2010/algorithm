#include<stdio.h>
#include<math.h>
#include<stdlib.h>
/*
    ���Թ滮C���ԣ��Ľ������ͷ������ھ�����㣬
	�������Լ�������ÿ��Ԫ��,���ǻ������в����ڴ���������ĸ��������ڵ�����û���õ������ǻ�����Խ��ʱ������������Խ����
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

// ������������ж��Ƿ��н⣬�ﵽ���Ž⣬����⣬�޽��ȵ�
// 1. ���м��������С�ڵ���0���ǻ�����С��0����ﵽ���Ž�
// 2. ���м��������С�ڵ���0������ĳ���ǻ����������������0�����������
// 3. ����ĳ����������0���ñ�����Ӧ��Լ�������Ӧ�ķ�����������ֵ��С��0�������޽��
// 4. 

//�ⶨ��
#define CAN_DO_NEXT 1				//δ�ﵽ���Ž⣬����ִ��
#define FIT_SOULTION 2 				//�������Ž�
#define ENDLESS_SOULTION 3			//�����
#define UNBOUNDED_SOULTION 4		//�޽��
#define NO_SOULTION 5				//�޽�

//������������
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


char *file_name="linear_programming_data_unbounded.txt";

int read_vector(FILE *f,vector_t *vt,int basic_start,int basic_end,const char *name){
	int i;
	vt->name=(char *)malloc(sizeof(char) * strlen(name));
	memcpy(vt->name,name,strlen(name));
	//��ȡ����ά������������ʼ����λ��
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
    // 1. ��ȡĿ�꺯���ǻ�����
	// ��ȡ����ά������������ʼ����λ��
    fscanf(f,"%d%d%d",&dest->m,&basic_start,&basic_end);
	basic_position->name="basic position arr";
	basic_position->size=basic_end - basic_start + 1;
	basic_position->data=(int *)malloc(sizeof(int) * basic_position->size);
	check_param->name="check_param vector";
	check_param->m=dest->m;
	check_param->data=(data_t *)malloc(sizeof(data_t) * dest->m);
	
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


//������������Ԫ��
int switch_element_in_vector(vector_t *t1,int pos1,vector_t *t2,int pos2){
	double d=t1->data[pos1];
	t1->data[pos1]=t2->data[pos2];
	t2->data[pos2]=d;
}

// �������жԻ�
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

// ���һ��������,��Ŀ�꺯���ǻ������в��Ҵ���0�����ֵ��λ��
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

//���һ���������
//basic_matrix Ϊ������in_pos_nΪ�������������,��not_basic_matrix���ڵ�������,
void find_out_vector(matrix_t *basic_matrix,matrix_t *not_basic_matrix,int in_pos_n,vector_t *feasible_solution){
	
}

int main(int argc,char **argv){
    //�ǻ�������Ŀ�꺯��ϵ������
    vector_t *dest = (vector_t *)malloc(sizeof(vector_t));
    //��������Ŀ�꺯��ϵ������
    vector_t *dest_basic = (vector_t *)malloc(sizeof(vector_t));
    //�ǻ�����
    matrix_t *not_basic_matrix = (matrix_t *)malloc(sizeof(matrix_t));
    // ������
    matrix_t *basic_matrix = (matrix_t *)malloc(sizeof(matrix_t));;
    //�����н�
    vector_t *feasible_solution = (vector_t *)malloc(sizeof(vector_t));
    
	// �ļ���ȡ����
	read_data(dest,dest_basic,not_basic_matrix,basic_matrix,feasible_solution);

    print_vector(dest);
    print_vector(dest_basic);
    print_matrix(not_basic_matrix);
    print_matrix(basic_matrix);
    print_vector(feasible_solution);
	return 0;
}
