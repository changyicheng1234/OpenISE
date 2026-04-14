#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

const int nmax=100;
int f[nmax+1];
int s[nmax+1][nmax+1];
int tot;
char a[nmax];

int GetDepth(int x){
	if(!s[x][0])
		return 1;
	
	int ans=1;
	for(int i=1;i<=s[x][0];i++)
		ans=max(ans,GetDepth(s[x][i])+1);
	
	return ans;
}

void Print(int tot, int idx, char c)
{
	for(int i=1;i<idx;i++)
		cout<<'#';
	cout<<c;
	for(int i=0;i<tot-idx;i++)
		cout<<'#';
	cout<<endl;
}
void DFS(int x,int d){
	Print(tot,d,a[x]);
	
	if(!s[x][0])
		return;
	
	for(int i=1;i<=s[x][0];i++)
		DFS(s[x][i],d+1);
	
	return;
}

int main()
{
	int n;
	cin>>n;
	for(int i=1;i<=n;i++){
		cin>>a[i]>>f[i];
		int p=f[i];
		s[p][0]++;
		s[p][s[p][0]]=i;
	}
	
	tot=GetDepth(1);

	
	DFS(1,1);
	
	return 0;
}

