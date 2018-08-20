#include<stdio.h>
#include<stdlib.h>
typedef struct tree{
	char data;
	int level;
	struct tree *left,*right;
}tree;
char *filename="D:\\code\\algorithm\\alg\\test_tree.txt";
void add_tree_node(tree *root,char c){
/* 	if(c==root->data){
        root->data=c;
        return;
	} */
	tree *t=root;
	tree *pa;
	//找到要插入的节点位置
    while(t!=NULL){
		pa=t;
		if(c<t->data){
			t=t->left;
		}else if(c>t->data){
			t=t->right;
		}else{
//			t->data=c;
			return;
		}
	}
	t=(tree *)malloc(sizeof(tree));
	t->data=c;
	t->level=pa->level+1;
	if(c>pa->data){
		pa->right=t;
	}else{
		pa->left=t;
	}
}

void view_sort_tree(tree *root){
	if(root!=NULL){
		view_sort_tree(root->left);
		printf("%c ",root->data);
		view_sort_tree(root->right);
	}
}

void view_hierarchy(tree *root){
	tree *que[100];
	que[0]=root;
	int k=1,cnt=1,curr=0;
	while(cnt--){
		printf("%d%c ",que[curr]->level,que[curr]->data);
		if(que[curr]->left!=NULL){
			que[k++]=que[curr]->left;
			cnt++;
		}
		if(que[curr]->right!=NULL){
			que[k++]=que[curr]->right;
			cnt++;
		}
		curr++;
	}
	
	printf("\nk=%d",k);
}

int main(){
	FILE *f=fopen(filename,"r");
	char c;
	fscanf(f,"%c",&c);
	tree *root=(tree *)malloc(sizeof(tree));
	root->data=c;
	root->level=0;
	while((fscanf(f,"%c",&c))!=EOF){
		add_tree_node(root,c);
	}
	view_sort_tree(root);
	printf("\nview hierarchy\n");
	view_hierarchy(root);
//	rewind(f);
//	tree *root1=(tree *)malloc(sizeof(tree));
	fclose(f);
	return 0;
}
