#include<iostream>
#include<stdio.h>
#include<memory.h>
using namespace std;

const int maxn=510;

int dis[maxn][maxn],n,path[maxn][maxn];

void Print(int x,int y){
	if(!path[x][y]){
		printf(" %d",y);
		return;
	}
	
	Print(x,path[x][y]);
	Print(path[x][y],y);
}

int main()
{
	memset(dis,0x7f,sizeof(dis));
	
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++){
			scanf("%d",&dis[i][j]);
			if(dis[i][j]<1)
				dis[i][j]=1e9;
		}
		
	for(int k=1;k<=n;k++)
		for(int i=1;i<=n;i++)
			for(int j=1;j<=n;j++)
				if(dis[i][k]+dis[k][j]<dis[i][j]){
					dis[i][j]=dis[i][k]+dis[k][j];
					path[i][j]=k;
				}
			
	for(int i=1;i<n;i++){
		if(dis[i][i+1]>1e8){
			printf("-1\n");
			continue;
		}
		printf("%d",i);
		Print(i,i+1);
		printf("\n");
	}
	
	return 0;
}
