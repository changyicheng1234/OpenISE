#include<stdio.h>

int fun(int m,int n){
	if (m==1) return 1;
	if (n==1) return 1;
	if (m<n) return fun(m,m);
	if (m==n) return 1+fun(m,n-1);
	
	return fun(m,n-1)+fun(m-n,n);
}

int main(){
	int m,n,a;
	scanf("%d %d", &m, &n);
	a = fun(m,n);
	printf("%d\n",a);
	
}
