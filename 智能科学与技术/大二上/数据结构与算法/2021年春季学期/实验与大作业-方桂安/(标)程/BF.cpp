#include <stdio.h>

int n, m;
char a[500000], b[500000];

int Index(char*S, char*T, int pos){
	int i=pos-1, j=0; 
	while(i<n){
		if(S[i]==T[j]){
			++i; ++j;
			if(j==m) return i-m+1;
		}else{
			i=i-j+1; j=0; 
		}
	}
	return 0;
} 

int main() {
	int pos=0;
	scanf("%d", &n); getchar(); gets(a);
	scanf("%d", &m); getchar(); gets(b);
    scanf("%d", &pos);
	printf("%d", Index(a,b,pos));
    return 0;
}


