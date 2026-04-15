#include<stdio.h>
#pragma warning(disable:4996)
#define CRT_SECURE_NO_WARNINGS

const int maxn = 1000000;
int n,r,count;//n表示顶点个数，r为主干上的核心点
int a[maxn+1];
int b[maxn+1];
int flag[maxn + 1] = {0};//记录叶子结点，若为叶子节点，置1
int f[maxn + 2];//用于邻接表
int linkto[2*maxn + 2];
int f1[maxn + 1] = {0};//用于找到最边上的核心点
int lable[maxn + 1] = {0};//顶点编号
int linkcost[2*maxn + 2];//边值编号
int visited[maxn+1] = {0};//防止点进行重复遍历

void input_data() {
	scanf("%d", &n);
	count = n;
	for (int i = 1; i < n; i++) {
		scanf("%d %d", &a[i], &b[i]);
	}
	for (int i = 1; i < n; i++) {
		f[a[i]]++;
		f[b[i]]++;
	}
	for (int i = 1; i <=n; i++) {
		if (f[i] == 1) {
			flag[i] = 1;
		}
	}
	for (int i = 1; i <= n; i++) {
		f[i] = f[i] + f[i - 1];
	}
	f[n + 1] = f[n];
	for (int i = 1; i < n; i++) {
		int j = a[i];
		int k = b[i];
		linkto[f[j]] = k;
		f[j]--;
		linkto[f[k]] = j;
		f[k]--;
	}

}

void mark(int x) {
	int y = 0;
	if (count > 1) {
		flag[x] = 1;
		for (int i = f[x] + 1; i <= f[x + 1]; i++) {
			if (lable[linkto[i]] != 0) continue;//若相邻的点已经被标记，跳过
			if (flag[linkto[i]] == 0) {//主干上的点最后标记
				y = i;
				continue;
			}
			linkcost[i] = --count;//边的权值递减
			if (linkcost[i] >= lable[x]) {
				lable[linkto[i]] = lable[x] + linkcost[i];
			}
			else {
				if (linkcost[i] + lable[x] >= n) {
					lable[linkto[i]] = lable[x] - linkcost[i];
				}
				else {
					int t1, t2;
					t1 = lable[x] - linkcost[i];
					t2 = lable[x] + linkcost[i];
					if (visited[t1] == 1) {
						lable[linkto[i]] = t2;
					}
					else lable[linkto[i]] = t1;
				}

			}
			visited[lable[linkto[i]]] = 1;//将编号的值标记，防止重复
		}
		//单独对核心点做处理
		linkcost[y] = --count;
		if (linkcost[y] >= lable[x]) {
			lable[linkto[y]] = lable[x] + linkcost[y];
		}
		else {
			if (linkcost[y] + lable[x] >= n) {
				lable[linkto[y]] = lable[x] - linkcost[y];
			}
			else {
				int t1, t2;
				t1 = lable[x] - linkcost[y];
				t2 = lable[x] + linkcost[y];
				if (visited[t1] == 1) {
					lable[linkto[y]] = t2;
				}
				else lable[linkto[y]] = t1;
			}

		}
		visited[lable[linkto[y]]] = 1;
		mark(linkto[y]);//访问核心点
	}
}
void find_r() {
	for (int i = 1; i <= n; i++) {
		if (flag[i] == 0) {//找到度大于1的点
			for (int j = f[i] + 1; j <= f[i + 1]; j++) {//遍历，找到相邻的度大于1的点
				if (flag[linkto[j]] == 0) {
					f1[i]++;
				}
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		if (f1[i] == 1) {//获得主干上最边上的点
			r = i;
			break;
		}
	}
	lable[r] = n;//将该点编号为最大
	visited[n] = 1;//表示数值n已经遍历
	mark(r);
	for (int i = 1; i <= n; i++) {
		printf("%d\n", lable[i]);
	}

}

int main() {
	input_data();
	find_r();
	return 0;
}
