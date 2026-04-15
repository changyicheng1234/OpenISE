#include<stdio.h>
#include<string.h>

const int maxn=1000001;
int pi[maxn];
char s[maxn];

void pi_function(){
	int i=1,j=0;
	int m=strlen(s);
	while(i<m)
	{
		if(s[i]==s[j]) pi[++i] = ++j;
		else if (j == 0) pi[++i] = j;
		else j = pi[j];
	}
}

int main()
{
	gets(s);
	pi_function();
	
	int Len =strlen(s);
	for(int j=1;j<=Len;j++){
		int k=j;
		int flag=0;
		while(k!=0 && flag<10){
		    printf("%d ",k);
		    k=pi[k];
		    flag++;
		}
		printf("\n");
	}
	return 0; 
}
