#include<stdio.h>

int main() {
    char s, Q[8], x = 'A';  
    int count = 0, top = 0;
    scanf("%c", &s);
    while (count < 8) {
		if (top > 0 && Q[top-1] == s){
				printf("%c",Q[top-1]); 
				top--; scanf("%c", &s); count++; 
        }
		else if (x <= 'H')	Q[top++] = x++;
		else {printf("0"); return 0;}
    }
	printf("1"); return 0;
}

