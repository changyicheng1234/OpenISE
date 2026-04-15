#include<iostream>
using namespace std;

int main() {
	int f[100][100];
	int m, n;
	int b[100];
	int max_sum = INT_MIN;
	cin >> m >> n;
	for(int i=0;i<m;i++)
		for (int j = 0; j < n; j++) {
			cin >> f[i][j];
		}
	int sum = 0;
	int i;
	for (int i = 0; i < m; i++) {
		for (int t = 0; t < n; t++)
			b[t] = 0;
		for (int j = i; j < m; j++) {
			sum = 0;
			for (int k = 0; k < n; k++) {
				b[k] += f[j][k];
				if (sum >= 0) {
					sum += b[k];
				}
				else
					sum = b[k];
				if (sum > max_sum) {
					max_sum = sum;
				}
			}
		}
	}
	cout << max_sum;
	return 0;
}