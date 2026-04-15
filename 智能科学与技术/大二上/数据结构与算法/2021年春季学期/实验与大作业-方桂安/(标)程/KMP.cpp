#include <stdio.h>

const int maxn = 1000000;

int n, m, pos;
char S[maxn + 1], T[maxn + 1];
int pi[maxn+1];

void compute_pi(){
  int i=1, j = 0; //pi[1] = 0;
  while (i < m){
    if (T[i] == T[j]) pi[++i] = ++j;
    else if (j == 0) pi[++i] = j;
    else j = pi[j];
  }
}

int string_Index(){
  int i = pos-1, j = 0;
  while (i < n){
	  if (S[i] == T[j]){
        i++; j++;
		if (j==m) return i - m + 1;
	  }
      else if (j == 0) i++;  
      else j = pi[j];
  }
  return 0;
}

int main(){
	scanf("%d", &n); getchar(); gets(S);
	scanf("%d", &m); getchar(); gets(T);
	scanf("%d", &pos);
	compute_pi();
    printf("%d", string_Index());
	return 0;
}
