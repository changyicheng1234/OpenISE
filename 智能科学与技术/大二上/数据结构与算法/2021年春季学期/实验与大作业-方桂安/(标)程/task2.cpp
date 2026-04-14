#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void graycode(int n, int a[]){
 	if(n==1){
 		a[0]=0; a[1]=1;
 		return;
	 }
	 graycode(n-1,a);
	 int L = 1<<(n-1);   // 得到2^(n-1)
	 int i = 0;
	 for (i ; i < L; i++)  a[L + L - i - 1] = a[i] + L;
 }


void display(int n,int G[]){
	int m = 1<<n; 
	int s =0 ;
	int remainder[200] ;
	int k = 0;	
	int a,b,i,j;
	
	//将输入的G[s]，转化为n位二进制输出 
	for( s ; s < m; ++s){
		k = 0 ;
		a = G[s];
		//十进制转化位二进制 
		while(a != 0) 
		{
			b=a/2;
       		k++;
        	remainder[k]=a-b*2;
        	a=a/2;	
		}
	
		// k代表转化为二进制的位数，当位数小于要求的位数n时，在前面补0 
		for(j = k; j < n ; j++ ) printf("0");
		//输出该二进制数 
		for(i=k; i>=1; i--)
		{	
			printf("%d",remainder[i]);
		}
		printf("\n"); 				
	}
}



int main(int argc, char *argv[]) {
	
	int k ;
	scanf("%d", &k); getchar();	
	
	int m = 1 <<k ;
	int *a ;
	a=(int *)calloc(m,sizeof(int)); 
	
	graycode(k,a);
	display(k,a);
	free(a);
	return 0;
}
