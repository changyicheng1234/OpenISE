#include <stdio.h>

#pragma comment(linker, "/STACK:1073741824")

const int maxn=1000001;
int A[maxn];

void swap(int &a, int &b) {
  int c = a; a = b; b = c;
}

int partition(int arr[], int low, int high) {
  int pivot = arr[high], i = low;
  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot){
      swap(arr[i], arr[j]); i++;
    }
  }
  swap(arr[i], arr[high]);
  return i;
}

void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int k = partition(arr, low, high);
    quickSort(arr, low, k-1);
    quickSort(arr, k+1, high);
  }
}

int main() {
  int n;
  scanf( "%d", &n);
  for (int i=0; i<n; i++) scanf("%d", &A[i]);
  quickSort(A, 0, n-1);
  for (int i=0; i<n; i++) printf("%d ", A[i]);
  return 0; 
}
