#include<stdio.h>
#include<stdlib.h>

const int maxn=1000000;
char Q[maxn+1];

int checkP(){
	char s;
	int top=0;
	scanf("%c",&s);
	while(s!='&'){
		Q[top++]=s;
		scanf("%c",&s);
	}

	scanf("%c",&s);
	while(s!='@'&& top>0)
		if(s==Q[--top]) scanf("%c",&s);
		else return 0;
	if (top == 0 && s == '@') return 1;
	else return 0;
}

int main(){
	printf("%d", checkP());
	return 0;	
}
