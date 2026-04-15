#include<stdio.h>
#include<stdlib.h>

int a[10001],b[10001],c[10001],k=1;
//a数组为中序遍历数组、b数组为后序遍历数组 

void orders(int as,int ae,int bs,int be){
	c[k++]=b[be]; 
	int root,sign;
	for(int i=as; i <=ae; i++)
		if(a[i]==b[be]) root=i;
	sign=bs+root-as-1;
	
	if(root-1 > as)orders(as,root-1,bs,sign);
	else if(root-1 == as)c[k++]=a[as]; 
	
	if(root+1 < ae)orders(root+1,ae,sign+1,be-1);
	else if(root+1 == ae)c[k++]=a[ae];
}
    
int main(){
	int n;
    scanf("%d",&n);
    for(int i=1; i <=n; i++)scanf("%d",&a[i]); 
	for(int i=1; i <=n; i++)scanf("%d",&b[i]); 
	orders(1,n,1,n);
	for(int i=1; i <=n; i++)printf("%d ",c[i]); 
	return 0;
}
