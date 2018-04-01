#include<stdio.h>
#include<stdlib.h>

//红黑树颜色定义
typedef enum {
    RED,BLACK
} color_enum;

//红黑树的节点定义
typedef struct node {
    int data;	//节点数据内容
    struct node *left;	//左节点指针
    struct node *right;	//右节点指针
    struct node *parent;	//父节点指针
    color_enum color;		//颜色值
	int height;   //当前节点高度
} node_t;

//红黑树结构体定义
typedef struct {
    node_t *root;	//树的根节点
    int node_num;	//树的节点数量
} tree_t;


// 对x节点左旋
void left_rotate(tree_t *t,node_t* x)
{
 
   node_t *y = x->right;
 
   // 将y的左节点放到x的右指针上
	x->right=y->left;		
	if(y->left!=NULL){
		y->left->parent=x;
	}
	
	//y的父节点置为x的父节点，如果x父节点为空，说明x是树的根节点，如果不是，则将x的父节点保存的左/右指针指向y
	y->parent=x->parent;
    if(x->parent==NULL) {
        t->root=y;
    } else {
        if(x->parent->left==x) {
            x->parent->left=y;
        } else {
            x->parent->right=y;
        }
    }

    // y的左指针设置为x，x的父指针指向y
    y->left=x;
    x->parent=y;
}

// 对x节点右旋，与左旋相反
void right_rotate(tree_t *t,node_t *x)
{
    node_t *y=x->left;
	
	x->left=y->right;
	if(y->right!=NULL){
		y->right->parent=x;
	}
	y->parent=x->parent;
	if(x->parent==NULL) {
        t->root=y;
    } else {
        if(x->parent->left==x) {
            x->parent->left=y;
        } else {
            x->parent->right=y;
        }
    }

    y->right=x;
    x->parent=y;
}

//求树颜色，如果节点为空，则节点为叶子节点，返回黑色
static inline color_enum color_of_node(node_t *x){
	return x==NULL?BLACK:x->color;
}

/*调整红黑树，分三种情况，以下case都是基于父节点为祖父节点的左节点来讨论，右节点则做相反操作即可
* case1. 当前节点的父节点为红色，父节点的兄弟节点为红色，此时将父节点与父节点的兄弟置为黑色，祖父节点置为红色，将当前节点指向祖父节点继续调整。
* case2. 当前节点的父节点为红色，父节点的兄弟节点为黑色，且当前节点为父节点的右节点，则对父节点进行左旋操作转换成case3
* case3. 当前节点的父节点为红色，父节点的兄弟节点为黑色，且当前节点为父节点的左节点，则将父节点置为黑色，祖父节点置为红色，对祖父节点进行右旋操作。 
*/
void fix_after_insert(tree_t *t,node_t *z)
{
    while(z!=NULL && color_of_node(z->parent)==RED) {
        //父节点为祖父节点的左节点
        if(z->parent==z->parent->parent->left) {
            //找到父节点的兄弟节点
            node_t *y=z->parent->parent->right;
            // case 1 start : 父节点为红色，父节点的兄弟节点为红色
            if(color_of_node(y)==RED) {
                z->parent->color=BLACK;     // 父节点置为红色
                z->parent->parent->color=RED;   //祖父节点置为红色
                y->color=BLACK;         //父节点的兄弟节点置为黑色
                z=z->parent->parent;    //当前节点置为祖父节点往上调整
            }
            //case 1 end
            else {
                // case 2 start : 父节点为红色，父节点的兄弟节点为黑色，z为父节点的右孩子
                if(z==z->parent->right) {
                    z=z->parent;
                    left_rotate(t,z);
                }
                //case 2 end ,转化成case 3
                z->parent->color=BLACK;
                z->parent->parent->color=RED;
                right_rotate(t,z->parent->parent);
            }
        } else {
			// 父节点为祖父节点的右节点，调整方式与左节点一样，反向操作
            if(z->parent==z->parent->parent->right) {
                node_t *y=z->parent->parent->left;
                if(color_of_node(y)==RED) {
                    z->parent->color=BLACK;
                    y->color=BLACK;
                    z->parent->parent->color=RED;
                    z=z->parent->parent;
                } else {
                    if(z->parent->left==z) {
                        z=z->parent;
                        right_rotate(t,z);
                    }
                    z->parent->color=BLACK;
                    z->parent->parent->color=RED;
                    left_rotate(t,z->parent->parent);
                }
            }
        }
    }
	//调整后，将根节点置黑
    t->root->color=BLACK;
}

//红黑树插入
int insert_node(tree_t *t,int data)
{
	//创建一个节点
    node_t *node=(node_t *)malloc(sizeof(node_t));
    node->data=data;
	//所以插入的节点颜色默认为红色，由后面调整
    node->color=RED;
	node->left=node->right=NULL;
	node->parent=NULL;
    node->height=0;
    if(t==NULL) t=(tree_t *)malloc(sizeof(tree_t));
	//如果为空树，则直接设置返回
    if(t->root==NULL) {
        t->root=node;
        t->node_num=1;
        node->color=BLACK;
        return 0;
    }
    //在树中找到node适合的位置，确保树是有序的，parent保存node的父节点指针。
    node_t *nt=t->root,*parent;
    while(nt!=NULL) {
        parent=nt;
		if(data==nt->data) {
            printf("data:%d has already in the tree,return\n",data);
            return 0;
        } else if(data>nt->data) {
            // 右树中查找
            nt=nt->right;
        } else {
            //左树中查找
            nt=nt->left;
        }
    }
	// 插入树中
    if(data>parent->data) {
        parent->right=node;
    } else {
        parent->left=node;
    }
    node->parent=parent;
	
	//新节点插入树后，可能会引起不满足红黑树的特性，因此需调整树。    
	fix_after_insert(t,node);
    t->node_num++;
    return 0;
}

int count_height(tree_t *t,node_t *n)
{
    if(n==NULL) return 0;
    if(n->height==0&&n->parent!=NULL)
        n->height=n->parent->height+1;
    count_height(t,n->left);
    count_height(t,n->right);
    return 0;
}

void print_tree(tree_t *t,node_t *n,int last_height)
{
    if(n==NULL) return;
    if(n->height!=last_height) {
		printf("\n");
		last_height=n->height;
	}
	printf("data=%d,height=%d,me=%p,parent=%p,color=%d\n",n->data,n->height,n,n->parent,n->color);
	print_tree(t,n->left,last_height);
	print_tree(t,n->right,last_height);
}
/*
* 在树中查找节点
*/
node_t *find_data_in_tree(tree_t *t,int data){
	node_t *z=t->root;
	while(z){
		if(z->data==data){
			return z;
		}else if(z->data>data){
			z=z->left;
		}else{
			z=z->right;
		}
	}
	return z;
}

/*
* 查找当前节点的后继节点
*/
node_t *find_successor(node_t *n){
	
	// 如果当前节点的右节点不为空，则右节点的最左节点为后继节点
	if(n->right){
		n=n->right;
		while(n->left){
			n=n->left;
		}
		return n;
	}
	//右节点为空时，寻找最低祖先节点
	node_t *z=n->parent;
	while(z&&n=z->right){
		n=z;
		z=z->parent;
	}
	return z;
}

/*
* 删除节点
*/
void delete_node(tree_t *t,int data){
	node_t *z=find_data_in_tree(t,data);
	if(z==NULL){
		printf("can not find data:%d in tree,return",data);
		return;
	}
	node_t *y,*x;
	//删除没有子节点
	if(!z->left||z->right){
		y=z;
	}else{
		y=find_successor(z);
	}
	if(y->left){
		x=y->left;
	}else{
		x=y->right;
	}
	if(x){
		x->parent=y->parent;
	}
}

int main(int argc,char **argv)
{
    int i,n=10;
    int ar[]= {1,6,2,3,7,81,1,9,100,10};
	tree_t *t=(tree_t *)malloc(sizeof(tree_t));
    t->root=NULL;
    t->node_num=0;
    for(i=0; i<n; i++) {
		printf("put array[%d]=%d\n",i,ar[i]);
        insert_node(t,ar[i]);
    }
    count_height(t,t->root);
    print_tree(t,t->root,t->root->height);
    return 0;
}
