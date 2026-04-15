#include<stdio.h>

const int MAXN = 12000;
int n, a[MAXN + 1], dp[MAXN + 1];

int longestNum(){
	dp[1] = 1;
	for (int i = 2; i <= n; i++) {
		dp[i] = 1;
		for (int j = 1; j < i; j++)
			if (a[j] < a[i] && dp[j] + 1 > dp[i]) dp[i] = dp[j] + 1;
	}
	
	int max = 0;
	for (int i = 1; i <= n; i++)
		if (dp[i] > max) max = dp[i];
	return max;
}

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	printf("%d", longestNum());
	return 0;
}
