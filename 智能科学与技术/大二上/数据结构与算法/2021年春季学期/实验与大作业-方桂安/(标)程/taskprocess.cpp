#include<stdio.h>

const int Maxn = 100;
const int MaxConsume = 200;

int n, r[Maxn + 1], p[Maxn + 1];

int taskProcess() {
	int cnt = 0, t = 0, tasknum = n; 
	while (tasknum) {
		int min = MaxConsume + 1, minIndex;
		for (int i = 1; i <= n; i++)
			if (t >= r[i] && p[i] > 0 && p[i] < min) {
				min = p[i]; minIndex = i;
			}
		p[minIndex]--; t++;
		if (!p[minIndex]){
			cnt += t; tasknum--;
		}
	}
	return cnt;
}

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d", &r[i]);
	for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
	printf("%d", taskProcess());
	return 0;
}
