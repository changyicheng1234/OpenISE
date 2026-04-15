#include<stdio.h>
const int maxn = 50010;

struct node{
	int x;
	node * next;
};

int n, m;
node * head[maxn]; // 注意：头节点的x存的是入度。 

void topo(){
	int que[maxn], l = 0, r = 0;
	for (int i = 1; i <= n; i++)
		if (head[i]->x == 0) que[r++] = i;
	while (l < r){
		int s = que[l++];
		printf("%d ", s);
		node * p = head[s]->next;
		while (p){
			head[p->x]->x--;
			if (head[p->x]->x == 0) que[r++] = p->x;
			p = p->next;
		}
	}
}

int main(){
	scanf("%d%d",&n,&m);
	for (int i = 1; i <= n; i++){
		head[i] = new node; 
		head[i]->x = 0;
		head[i]->next = NULL;
	}
	for (int i = 1; i <= m; i++) {
		int x, y;
		scanf("%d%d",&x,&y);
		node*p = new node;
		p->x = y;
		p->next = head[x]->next; 
		head[x]->next = p;
		head[y]->x++; //y的入度加一 
	}
	topo();
	return 0;
}
