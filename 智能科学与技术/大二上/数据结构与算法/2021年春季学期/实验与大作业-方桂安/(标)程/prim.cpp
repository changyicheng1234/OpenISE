#include <stdio.h>

const int max_n = 100000;
const int max_m = 500000;
const int MAXINT = 100000000;

int heap[max_n+1];
int position[max_n+1];
int lowcost[max_n+1];
int n, m, size;

void decrease_value(int x) {
	int f=x/2; //得到父节点位置
	int a;
	a = heap[x]; //得到当前结点
	//因为要进行不断调整，所以需要用while;
	while(f > 0 && lowcost[a] < lowcost[heap[f]]) 
	{ //当前结点的lowcost与父亲结点比较
		heap[x] = heap[f]; position[heap[x]] = x;//若比父亲结点的lowcost小，那么交换位置
		x = f; f = x / 2;
	}
	heap[x] = a; position[heap[x]] = x;//进行双向映射
}

int delete_min() {
	int r,a;
	r = heap[1];
	position[r] = 0;//表明取出根结点，无法在堆中索引到结点r
	a = heap[size]; size--;
	int i = 1,ch = 2;//从根节点开始向下调整
	while (ch <= size) {
		if (ch < size && lowcost[heap[ch]] > lowcost[heap[ch + 1]]) ch++;
		if (lowcost[a] < lowcost[heap[ch]]) break;
		heap[i] = heap[ch]; position[heap[i]] = i;
		i = ch; ch += ch;
	}
	heap[i] = a; position[heap[i]] = i;
	return r;
}

int a[max_m + 1]; //从1开始
int b[max_m + 1];
int c[max_m + 1];

int p[max_n + 2];//有n+1个指针 所以应该创建max_n + 2的空间
int linkto[2 * max_m + 1];
int linkcost[2 * max_m + 1];//？

void input_data() {
	scanf("%d %d", &n, &m);
	size = n;
	for (int i = 1; i <= m; i++) {
		scanf("%d %d %d", &a[i], &b[i], &c[i]);
	}
	for (int i = 1; i <= m; i++) {//保存每个顶点的度
		p[a[i]]++;
		p[b[i]]++;
	}
	for (int i = 1; i <= n; i++) p[i] = p[i] + p[i - 1];//相当于指针，方便遍历
	p[n + 1] = p[n];//trick 多一个指针指向最后一个元素
	for (int i = 1; i <= m; i++) {//构建邻接链表
		linkto[p[a[i]]] = b[i];
		linkcost[p[a[i]]] = c[i];
		p[a[i]]--;
		linkto[p[b[i]]] = a[i];
		linkcost[p[b[i]]] = c[i];
		p[b[i]]--;

	}
}

void prim() {
	for (int i = 1; i <= n; i++) {
		heap[i] = i; position[i] = i; lowcost[i] = MAXINT;
	}
	lowcost[1] = 0;
	for (int i = 1; i <= n; i++) { //执行n次，保证遍历完每个顶点
		int r = delete_min();
		if (r > 1) {
			printf("%d\n", lowcost[r]);
		}
		for (int j = p[r] + 1; j <= p[r + 1]; j++) {
			int b = linkto[j];
			if (lowcost[b] > linkcost[j]) {
				lowcost[b] = linkcost[j];
				if (position[b] > 0) {
					decrease_value(position[b]);
				}
			}
		}
	}
}

int main() {
	input_data();
	prim();
	return 0;
}
