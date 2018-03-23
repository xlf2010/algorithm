#include<stdio.h>
#include<stdlib.h>

typedef enum {
    RED,BLACK
} color_enum;

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
    struct node *parent;
    color_enum color;
    int height;
} node_t;

typedef struct {
    node_t *root;
    int node_num;
} tree_t;

static node_t *leaf_node;

/*
  左旋
*/
void left_rotate(tree_t *t,node_t* x)
{
    node_t *y = x->right;
    // 将Y的左节点放到x的右节点上
	x->right=y->left;		
	if(y->left!=NULL){
		y->left->parent=x;
	}
	y->parent=x->parent;
	//y的父节点置为x的父节点
    if(x->parent==NULL) {
        t->root=y;
    } else {
        if(x->parent->left==x) {
            x->parent->left=y;
        } else {
            x->parent->right=y;
        }
    }

    // y的左节点置为x
    y->left=x;
    x->parent=y;
}

/*
    右旋
*/
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

static inline color_enum color_of_node(node_t *x){
	return x==NULL?BLACK:x->color;
}

void fix_after_insert(tree_t *t,node_t *z)
{
    while(z!=NULL&&color_of_node(z->parent)==RED) {
        //父节点为父父节点的左节点
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
    t->root->color=BLACK;
}

int insert_node(tree_t *t,int data)
{
    node_t *node=(node_t *)malloc(sizeof(node_t));
    node->data=data;
    node->color=RED;
//    node->left=node->right=leaf_node;
	node->left=node->right=NULL;
	node->parent=NULL;
    node->height=0;
    if(t==NULL) t=(tree_t *)malloc(sizeof(tree_t));
    if(t->root==NULL) {
        t->root=node;
        t->node_num=1;
        node->color=BLACK;
        return 0;
    }
    //find the position on the tree
    node_t *nt=t->root,*parent;
    while(nt!=NULL) {
        parent=nt;
		if(data==nt->data) {
            printf("data:%d has already in the tree,return\n",data);
            return 0;
        } else if(data>nt->data) {
            // right tree;
            nt=nt->right;
        } else {
            //left tree
            nt=nt->left;
        }
    }
    if(data>parent->data) {
        parent->right=node;
    } else {
        parent->left=node;
    }
    node->parent=parent;
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

int main(int argc,char **argv)
{
    int i,n=10;
    int ar[]= {1,6,2,3,7,81,1,9,100,10};
	leaf_node=(node_t *)malloc(sizeof(node_t));
	leaf_node->color=BLACK;
	leaf_node->data=0;
	leaf_node->left=leaf_node->right=leaf_node->parent=NULL;
	
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
