#include<stdio.h>

const int Maxn = 5000;

int a[Maxn + 1][3], a_rpos[Maxn + 1], b[Maxn + 1][3], b_rpos[Maxn + 1], temp[Maxn + 1], Q[Maxn + 1][3];

void tri_multi() {
	if (a[0][1] != b[0][0])return; //无法相乘

	Q[0][0] = a[0][0];
	Q[0][1] = b[0][1];
	Q[0][2] = 0;

	int row = 1, col = 1;
		
	while(row <= Q[0][0]){		
		for (int i = a_rpos[row]; i < a_rpos[row+1]; i++)
			for (int j = b_rpos[a[i][1]]; j < b_rpos[a[i][1]+1]; j++)	{
				temp[b[j][1]] += a[i][2] * b[j][2];
			}
				
		
	    for(col = 1; col <= Q[0][1]; col++)
	    {
	  		if (temp[col]) //如果结果非零，则记录到结果三元组中
	  		{
	        if(++Q[0][2] > Maxn)
	          return;
	        int k = 0;
	        k = Q[0][2];
	  			Q[k][0] = row;
	  			Q[k][1] = col;
	  			Q[k][2] = temp[col];
	  		temp[col] = 0;
	  		}
	    }
			row ++;
	}

	//输出结果三元组
	for (int i = 0; i <= Q[0][2]; i++)
	{
		for (int j = 0; j <= 2; j++)
			printf("%d ", Q[i][j]);
		printf("\n");
	}
}


int main() {
	for (int j = 0; j <= 2; j++)
		scanf("%d", &a[0][j]);
	for (int i = 1; i <= a[0][2]; i++)
		for (int j = 0; j <= 2; j++)
			scanf("%d", &a[i][j]);
	int sign = 1, k = 1;
	while(k <= a[0][2])
	{
		if (a[k][0] == sign){
			a_rpos[sign++] = k;
			k++;
		}
		else if(a[k][0] < sign){
			k++;
		}
		else{
			a_rpos[sign++] = k;
			a_rpos[sign++] = k;
		}	 
	}
	a_rpos[sign] = a[0][2] + 1;	
	getchar();

	for (int j = 0; j <= 2; j++)
		scanf("%d", &b[0][j]);
	for (int i = 1; i <= b[0][2]; i++)
		for (int j = 0; j <= 2; j++)
			scanf("%d", &b[i][j]);
			
	sign = 1, k = 1;
	while(k <= b[0][2])
	{
		if (b[k][0] == sign){
			b_rpos[sign++] = k;
			k++;
		}
		else if(b[k][0] < sign){
			k++;
		}
		else{
			b_rpos[sign++] = k;
			b_rpos[sign++] = k;
		}	 
	}
  	b_rpos[sign] = b[0][2] + 1;

	tri_multi();
	return 0;
}


