#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#define MAXN 50001
using namespace std;
struct Edge{
	int to;
	int v;
	Edge *next;
};


int n, m;
int pos[MAXN], heap_size,t, heap[MAXN];
int dis[MAXN];
Edge *first[MAXN];
bool in_heap[MAXN];

void add_edge(int u, int v, int len)
{
	Edge* temp = new Edge;
	temp->to = v;
	temp->v = len;
	temp->next = first[u];
	first[u] = temp;
}
void swapp(int i, int j)//调整指针
{
	int temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
	pos[heap[j]] = j;
	pos[heap[i]] = i;
}
void shift_up(int now)//调整位置
{
	int next = 0;
	while (now > 1)
	{
		next = now >> 1;
		if (dis[heap[next]] > dis[heap[now]])
			swapp(next, now);
		now = next;
	}
}
void put(int x)//插入堆
{
	in_heap[x] = true;
	heap[++heap_size] = x;
	pos[x] = heap_size;
	shift_up(heap_size);
}
int get()//取堆顶元素
{
	int now = 1, next, res = heap[1];
	in_heap[heap[1]] = false;
	heap[1] = heap[heap_size--];
	pos[heap[1]] = 1;
	while (now * 2 <= heap_size)
	{
		next = now << 1;
		if (next < heap_size && dis[heap[next + 1]] < dis[heap[next]])
			++next;
		if (heap[now] <= heap[next])
			return res;
		swapp(now, next);
		now = next;
	}
	return res;
}
void dijkstra()
{
	put(1);
	dis[1] = 0;
	while (heap_size > 0)
	{
		int top = get();
		if (top == t)
			break;
		Edge* temp = first[top];
		while (temp != NULL)
		{
			if (dis[temp->to] > dis[top] + temp->v)
			{
				dis[temp->to] = dis[top] + temp->v;
				//结点在堆中就只调整位置，否则插入堆并调整位置
				if (in_heap[temp->to])
					shift_up(pos[temp->to]);
				else
					put(temp->to);
			}
			temp = temp->next;
		}
	}
}

void input_data()
{
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= m; i++) {
		int x, y, z;
		scanf("%d %d %d", &x, &y, &z);
		add_edge(x, y, z);
		add_edge(y, x, z);
	}

}

int main()
{
	memset(dis, 127, sizeof(dis));
	input_data();
	dijkstra();
	for (int i = 2; i <= n; i++) 
	  printf("%d ", dis[i]);
	return 0;
}




