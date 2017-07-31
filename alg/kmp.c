#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int common_search(char *a,char *b){
	int lena=strlen(a),lenb=strlen(b),i=0,j=0,k=0;
	printf("lena=%d lenb=%d\n",lena,lenb);
 /* 	while(i<=lena-lenb ){
		j=0;
		while(j<lenb){
			k++;
			printf("%d %d\n",i,j);
			if(a[i+j]!=b[j++]){
				i++;
				break;
			}else if(j==lenb){
				printf("find str at : %d\n",i);
				return k;
			}
		}
	}
	return k;  */
	int found=0;
	while(i<lena&&j<lenb){
		k++;
		printf("%d %d\n",i,j);
		if(a[i]==b[j]){
			i++;
			j++;
		}else{
			i=i-j+1;
			j=0;
		}
	}
	if(j>=lenb) found=(i-lenb);
	printf("found=%d\n",found);
	return k;
}

void next(char *c,int *nxt,int len){
	int i,k,val;
	nxt[0]=0;
	for(i=1;i<len;i++){
		if(c[i] == c[nxt[i-1]]){
			nxt[i]=nxt[i-1]+1;
		}else{
			k=nxt[i-1];
			while(c[i]!=c[k]&&k>0){
				k=nxt[k-1];
			}
			if(c[i]==c[k]){
				nxt[i]=k+1;
			}else{
				nxt[i]=0;
			}
		}
	}
}

void next1(char *c,int *nxt,int len){
	int i=1,j=-1,k;
	nxt[0]=-1;
	while(i<len){
		if(j==-1||c[i]==c[j]){
			j++;
			i++;
			nxt[i]=j;
		}else{
			j=nxt[j];
		}

	}
	for(k=0;k<len;k++){
		printf("%d ",nxt[k]);
	}
		printf("\n");	
	i=1,j=-1,k;
	nxt[0]=-1;
	while(i<len){
		if(j==-1||c[i]==c[j]){
			j++;
			i++;
			nxt[i]=j;
		}else{
			j=nxt[j];
		}
	}
	for(k=0;k<len;k++){
		printf("%d ",nxt[k]);
	}
	printf("\n");
}

int kmp(char *a,char *b){
	int lena=strlen(a);
	int lenb=strlen(b);
//	b[0]=lenb;
	int *nxt=(int *)malloc(sizeof(int) * lenb);
	int found=0,i=0,j=0,k=0;
	for(i=0;i<lenb;i++){
		nxt[i]=0;
	}
	next1(b,nxt,lenb);
	printf("\n\nnext:\n");
	for(i=0;i<lenb;i++){
		printf("%d ",nxt[i]);
	}
	printf("\n");
	for(i=0;i<lenb;i++){
		printf("%c ",b[i]);
	}
	printf("\n");
	i=0;
  	while(i<lena&&j<lenb){
		k++;
		printf("%d %d\n",i,j);
		if(j==-1 || a[i]==b[j]){
			i++;
			j++;
		}else{
			j=nxt[j];
		}
	}
	if(j>=lenb) found=(i-lenb);
	printf("found1=%d,k=%d\n",found,k);  
	return k;
	
}
int main(){
//	char a[1000],b[100];
//	stpcpy(a,"abcdabcdabcdabcdabce");
//	stpcpy(b,"abce");
//	char *b="abce";
//	char *b="agctagcagctagcta";
	char *a="acabaabaabcacaabc";
//	char *a="abcabd";
	char *b="abaabcac";
	printf("find str : %s at %s times : %d\n",b,a,common_search(a,b));
	kmp(a,b);
	return 0;
}