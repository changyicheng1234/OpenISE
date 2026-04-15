#include <stdio.h>
const int N=600;
struct _
{
	int x,y;
}list[N*2],q[N];
int n,k,head[N],line,a[N][N],f[N][10],g[N][10],fa[N],num,b[N][N],d[N];
int find(int x){return x==fa[x]?x:fa[x]=find(fa[x]);}
void add(int x,int y){list[++line].x=head[x],list[line].y=y,head[x]=line;}
void dfs(int x,int fa)
{
	f[x][0]=fa,d[x]=d[fa]+1;
	if (a[fa][x]==1)g[x][0]=1;else g[x][0]=0;
	for (int i=1;i<10;++i)f[x][i]=f[f[x][i-1]][i-1],g[x][i]|=g[f[x][i-1]][i-1];
	for (int i=head[x];i;i=list[i].x)
	{
		int y=list[i].y;
		if (y==fa)continue;
		dfs(y,x);
	}
} 
int find_lca(int x,int y)
{
	if (d[x]<d[y]){int t=x;x=y,y=t;}
	int flag=0;
	for (int i=9;i>=0;--i)
	if (d[f[x][i]]>=d[y])
	{
		flag|=g[x][i];
		x=f[x][i];
	}
	if (x==y)return flag?x:0;
	for (int i=9;i>=0;--i)
	if (f[x][i]!=f[y][i])
	{
		flag|=g[x][i]|g[y][i];
		x=f[x][i],y=f[y][i];
	}
	flag|=g[x][0]|g[y][0];
	if (flag)return f[x][0];
	return 0;
}
void solve()
{
	if (num==k)return;
	for (int i=1;i<=n;++i)
	for (int j=i+1;j<=n;++j)
	if (a[i][j]==2&&b[i][j]==0)
	{
		int z=find_lca(i,j);
		if (z==0)continue;
		int x=i;
		while (x!=z)
		{
			if (a[f[x][0]][x]==1)break;
			x=f[x][0];
		}
		if (x==z)
		{
			x=j;
			while (x!=z)
			{
			if (a[f[x][0]][x]==1)break;
			x=f[x][0];
			}
		}
		b[x][f[x][0]]=b[f[x][0]][x]=0;
		b[i][j]=b[j][i]=1;
		line=0;
		for (int k=1;k<=n;++k)head[k]=0;
		for (int k=1;k<n;++k)
		{
			if ((q[k].x==x&&q[k].y==f[x][0])||(q[k].x==f[x][0]&&q[k].y==x))
			{
				q[k].x=i,q[k].y=j;
			}
		}
		for (int k=1;k<n;++k)add(q[k].x,q[k].y),add(q[k].y,q[k].x);
		dfs(1,0);
		num++;
		if (num==k)return;
	}
}
int main()
{
	scanf("%d%d",&n,&k);
	for (int i=1;i<=n;++i)
	for (int j=1;j<=n;++j)
	scanf("%d",&a[i][j]);
	for (int i=1;i<=n;++i)fa[i]=i;
	int t=0;
	for (int i=1;i<=n;++i)
	for (int j=i+1;j<=n;++j)
	if (a[i][j]==1)
	{
		int fx=find(i),fy=find(j);
		if (fx==fy)continue;
		fa[fx]=fy;
		t++,num++;
		q[t].x=i,q[t].y=j;
		b[i][j]=b[j][i]=1;
		add(i,j),add(j,i);
	}
	for (int i=1;i<=n;++i)
	for (int j=i+1;j<=n;++j)
	if (a[i][j]==2)
	{
		int fx=find(i),fy=find(j);
		if (fx==fy)continue;
		fa[fx]=fy;
		b[i][j]=b[j][i]=1;
		t++,num+=2;
		q[t].x=i,q[t].y=j;
		add(i,j),add(j,i);
	}
	if (t<n-1){printf("-1");return 0;}
	if (num>k){printf("-1");return 0;}
	dfs(1,0);
	solve();
	if (num<k){printf("-1");return 0;}
	for (int i=1;i<=n;++i,printf("\n"))
		for (int j=1;j<=n;++j)printf(" %d"+(j==1),b[i][j]);
}
