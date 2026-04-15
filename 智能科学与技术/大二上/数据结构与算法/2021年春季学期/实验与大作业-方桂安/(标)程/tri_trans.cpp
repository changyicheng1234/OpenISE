#include<stdio.h>

const int Maxn = 5000;

int a[Maxn + 1][3], b[Maxn + 1][3];

void tri_trans() {
	b[0][0] = a[0][1];
	b[0][1] = a[0][0];
	b[0][2] = a[0][2];
	int k = 1;
	
	for (int i = 1; i <= a[0][1]; i++)
		for (int j = 1; j <= a[0][2]; j++)
			if (a[j][1] == i)
			{
				b[k][0] = i;
				b[k][1] = a[j][0];
				b[k][2] = a[j][2];
				k++;
			}
									
	for (int i = 0; i <= b[0][2]; i++)
	{
		for (int j = 0; j <= 2; j++) 
			printf("%d ", b[i][j]);
		printf("\n");
	}	
}

int tri_pos(int x,int y) {
	for (int i = 1; i <= b[0][2]; i++)
		if (b[i][0] == x && b[i][1] == y)
			return(b[i][2]);
	return 0;
}


int main() {
	for (int j = 0; j <= 2; j++) 
		scanf("%d", &a[0][j]);	
	for (int i = 1; i <= a[0][2]; i++)
		for (int j = 0; j <= 2; j++) 
			scanf("%d", &a[i][j]);			
	tri_trans();
	
	int x , y;
	scanf("%d %d",&x,&y);
	printf("%d",tri_pos(x,y));	
	return 0;
}

/*
6 7 8 
1 2 12
1 3 9
3 1 -3
3 6 14
4 3 24
5 2 18
6 1 15
6 4 -7
*/
