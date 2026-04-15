#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<int> num;
void scanf() {
	int n;
	cin >> n;
	num.push_back(n);
}
int main() {
	void scanf();
	int n;
	cout << "How many numbers do you want to enter?" << endl;
	cin >> n;
	cout << "Please enter your numbers:"<<endl;
	for (int i=0;i<n;i++)
		scanf();
	cout << "The sum of adjacent elements:" << endl;
	if (num.size()%2 == 0)
		for (int i = 0; i < num.size() - 1; i++)
			cout << num[i] + num[i + 1] << ",";
	else {
		for (int i = 0; i < num.size() - 1; i++)
			cout << num[i] + num[i + 1] << ",";
		cout << "The last element is not summated.";
	}
	return 0;
}