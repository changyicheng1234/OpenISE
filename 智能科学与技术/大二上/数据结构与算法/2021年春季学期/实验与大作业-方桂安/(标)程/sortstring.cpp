#include<stdio.h>

const int maxn = 50000;
char charseq[21 * maxn];

bool smaller(int i, int j){  //check whether 
	    // the string starting at the i-th position is smaller than the string starting at the j-th position
    while (charseq[i]!='\0'&&charseq[j]!='\0'){
        if (charseq[i]==charseq[j]){
            i++; j++;
        }else return (charseq[i]<charseq[j]);
    }
	return (charseq[j] != '\0' && charseq[i] == '\0');
}

int n;
int p[maxn];
int s[maxn];    //  The i-th word (0<i<n) W[i] starts at charseq[s[i]].

void swap(int &a, int &b){
	int c = a; a = b; b = c;
}

void sort(int l, int r){
	if (l >= r) return;
	int key = s[p[r]];
	int i = l;
	for (int j = l; j < r; j++)
		if ( smaller(s[p[j]], key)){
			swap(p[i], p[j]); i++;
		}
	swap(p[i], p[r]);
	sort(l, i-1);
	sort(i+1, r);
}

int main(){
	scanf("%d", &n); getchar();
	int x = 0;
	for(int i=0;i<n;i++){
		s[i] = x;
		gets(charseq + x);
		while (charseq[x] != '\0') x++;
		x++;
		p[i] = i;
	} 
	sort(0, n-1);
	for(int i=0; i<n; i++) printf("%s\n",charseq + s[p[i]]);
	return 0;
}
