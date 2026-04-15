#include<stdio.h>
#include<stdlib.h>

int n , ans = 0; //n为a数组正整数个数,ans存储结果 
int a[50001],tmp[50001];
//a数组为n个不同的正整数，tmp数组用来排序 

//归并排序 
void merge(int arr[], int left, int middle, int right){
	int i, j;
	int n1=middle-left+1, n2=right-middle;
	int L[n1], R[n2];
	for(i=0;i<n1;i++) L[i]=arr[left+i];
	for(j=0;j<n2;j++) R[j]=arr[middle+1+j];

	i=j=0;
	int k=left;
	while(i<n1 && j<n2)
		if(L[i]<=R[j]) arr[k++] = L[i++];
		else  arr[k++] = R[j++];
	while (i<n1) arr[k++] = L[i++];
	while (j<n2) arr[k++] = R[j++]; 
}

void mergesort(int arr[],int left, int right){
	if(left<right){
		int middle=(left+ right)/2;
		mergesort(arr, left, middle);
		mergesort(arr, middle+1,right);
		merge(arr, left, middle, right);
	}
}



int fastwpl(int a[]){
	if(n<=1)
		return ans;
	else{
		int t = a[1] + a[2];
		ans += t;
		for(int i = 1;i<=n;i++)a[i] = a[i+2];
		n -= 2;
		int sign= 1;
		for(;sign<=n;sign++)
			if(t<=a[sign])break;
		n += 1;
		for(int i = n;i > sign;i--)a[i] = a[i-1];
		a[sign] = t;
	}
	fastwpl(a);
}
    

int main(){	
    scanf("%d",&n);
    for(int i=1; i <=n; i++)scanf("%d",&a[i]); 
	mergesort(a,1,n);
		
	if(n==1)printf("%d",a[1]);
	else
		printf("%d",fastwpl(a)); 
	return 0;
}
