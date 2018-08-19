#include<stdio.h>
#include<math.h>
#include<stdlib.h>
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

//定义向量结构体
typedef struct {
    char *name;
    int len;
    double *data;
}vector_t;

// 定义矩阵结构体
typedef struct{
    char *name;
    int m,n;
    double **data;
}matrix_t;


char *file_name="linear_programming_data.txt";

int readData(vector_t *dest,vector_t *dest_basic,matrix_t *not_basic_matrix,matrix_t *basic_matrix,vector_t *feasible_solution){
    FILE *f=fopen(file_name,"r");
    int i,j;
    // 1. 读取目标函数非基变量
    dest->name="dest";
    fscanf(f,"%d",&dest->len);
    dest->data = (double *)malloc(sizeof(double *) * dest->len);
    for(i=0;i<dest->len;i++){
       fscanf(f,"%lf",dest->data+i);
    }
    // 2. 读取目标函数基变量
    fscanf(f,"%d",&dest_basic->len);
    dest_basic->name="dest_basic";
    dest_basic->data = (double *)malloc(sizeof(double *) * dest_basic->len);
    for(i=0;i<dest_basic->len;i++){
       fscanf(f,"%lf",dest_basic->data+i);
    }
    // 3. 读取约束条件非基矩阵
    fscanf(f,"%d%d",&not_basic_matrix->m,&not_basic_matrix->n);
    not_basic_matrix->name="not_basic_matrix";
    not_basic_matrix->data=(double **)malloc(sizeof(double **) * not_basic_matrix->m);
    for(i=0;i<not_basic_matrix->m;i++){
        not_basic_matrix->data[i] = (double *)malloc(sizeof(double *) * not_basic_matrix->n);
        for(j=0;j<not_basic_matrix->n;j++){
            fscanf(f,"%lf",not_basic_matrix->data[i] + j);
        }
    }
    // 4. 读取约束条件基矩阵
    fscanf(f,"%d%d",&basic_matrix->m,&basic_matrix->n);
    basic_matrix->name="basic_matrix";
    basic_matrix->data=(double **)malloc(sizeof(double **) * basic_matrix->m);
    for(i=0;i<basic_matrix->m;i++){
        basic_matrix->data[i] = (double *)malloc(sizeof(double *) * basic_matrix->n);
        for(j=0;j<basic_matrix->n;j++){
            fscanf(f,"%lf",basic_matrix->data[i] + j);
        }
    }
    //5 读取基可行解
    fscanf(f,"%d",&feasible_solution->len);
    feasible_solution->name="feasible_solution";
    feasible_solution->data = (double *)malloc(sizeof(double *) * feasible_solution->len);
    for(i=0;i<feasible_solution->len;i++){
       fscanf(f,"%lf",feasible_solution->data + i);
    }
    fclose(f);
    return 0;
}

void printVector(vector_t *v){
    int i;
    printf("vector name : %s\n   ",v->name);
    for(i=0;i<v->len;i++){
        printf("%lf ",v->data[i]);
    }
    printf("\n");
}

void printMatrix(matrix_t *mx){
    int i,j;
    printf("matrix name : %s\n ",mx->name);
    for(i=0;i<mx->m;i++){
        for(j=0;j<mx->n;j++){
            printf("%lf ",mx->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc,char **argv){
    //目标函数向量
    vector_t *dest = (vector_t *)malloc(sizeof(vector_t));
    //基变量在目标函数系数向量
    vector_t *dest_basic = (vector_t *)malloc(sizeof(vector_t));
    //非基矩阵
    matrix_t *not_basic_matrix = (matrix_t *)malloc(sizeof(matrix_t));
    // 基矩阵行
    matrix_t *basic_matrix = (matrix_t *)malloc(sizeof(matrix_t));;
    //基可行解个数
    vector_t *feasible_solution = (vector_t *)malloc(sizeof(vector_t));
    // 文件读取数据
	readData(dest,dest_basic,not_basic_matrix,basic_matrix,feasible_solution);

    printVector(dest);
    printVector(dest_basic);
    printMatrix(not_basic_matrix);
    printMatrix(basic_matrix);
    printVector(feasible_solution);

}
