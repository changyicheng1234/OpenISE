#include <stdio.h>
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)

const int maxn = 100000;
const int maxm = 500000;

int parent[maxn + 1];
int ancestor[maxn + 1];
int rank[maxn + 1];
int node[maxn+2];//结点的查询数
int queryto[2*maxm + 2];
int f[maxn+1];//存储每个结点的父亲结点
int query[maxm+1][3];
bool visited[maxn + 1];
int position[2 * maxm + 2];
int result[maxm + 1];
int p[maxn+1];
int child_[maxn+1];

int n, m; //n个结点，m个查询

void makeset(int x) {
	parent[x] = x;
}

int find(int x) {
	if(x != parent[x])
	{
		parent[x]= find(parent[x]);
	}
	return parent[x];
}

void Union(int x, int y) {
	int s = find(x);
	int r = find(y);
	if (rank[s] > rank[r]) {
		parent[r] = s;
	}
	else if (rank[s] < rank[r]) {
		parent[s] = r;
	}
	else
	{
		rank[r]++;
		parent[s] = r;
	}
}

void input_data() 
{
	scanf("%d %d", &n, &m);
	for (int i = 2; i <=n; i++) {
		scanf("%d", &f[i]);
	}
	for (int i = 1; i <= m; i++) {
		scanf("%d %d", &query[i][1], &query[i][2]);
	}
	for (int i = 1; i <= m; i++) {
		node[query[i][1]]++;
		node[query[i][2]]++;
	}//得到每个点的查询数
	for (int i = 2; i <= n; i++)
	{
		node[i] = node[i] + node[i - 1];//得到每个点的位置
	}
	node[n + 1] = node[n];
	for (int i = 1; i <= m; i++) 
	{
		int a = query[i][1];
		int b = query[i][2];
		queryto[node[a]] = b;
		position[node[a]] = i;
		node[a]--;
		queryto[node[b]] = a;
		position[node[b]] = i;
		node[b]--;
	}
	for(int i=1;i<=n;i++)
	{
		p[f[i]]++;
	}
	for(int i=1;i<=n;i++)
	{
		p[i+1]+=p[i];
	}
	p[n+1]=p[n];
	for(int i=2;i<=n;i++)
	{
		child_[p[f[i]]]=i;
		p[f[i]]--;
	}
}



  
void TarjanOLCA(int u) {
	makeset(u);
	ancestor[u] = u;
	for (int i = p[u]+1; i <= p[u+1]; i++) 
	{
		int c=child_[i];
		if (f[c]== u) 
		{
			TarjanOLCA(c);
			Union(c, u);
			ancestor[find(u)] = u;
		}
	}
	visited[u] = 1;
	for (int i = node[u] + 1; i <= node[u+1]; i++) {
		int v = queryto[i];
		if (visited[v] == 1) {
			int k = position[i];//对应原来查询的位置
			result[k] = ancestor[find(v)];
		}
	}
}



void output() {
	for (int i = 1; i <= m; i++) {
		printf("%d\n", result[i]);
	}
}

int  main() {
	int u = 1;
	input_data();
	TarjanOLCA(u); 
	output();
}
