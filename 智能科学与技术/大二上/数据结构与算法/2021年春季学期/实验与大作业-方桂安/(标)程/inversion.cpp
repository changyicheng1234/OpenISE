#include <iostream>

long long int k = 0; //全局变量 
int a[1100000];
int b[1100000];
long long int MergeSort(int s[], int left, int middle, int right)
{
   int i = left, j = middle;
   int index = 0;
   long long int sum = 0;
   int t = 0;
   while (i < middle && j <= right)
   {
       if (s[i] > s[j])
       {
           b[index++] = s[j++];
           t++;
       }
       if (s[i] <= s[j] || j == right + 1)
       {// j == right + 1是防止j已经到达最右边啦，i却没有
       //因此我们需要进入if条件语句中而不是直接跳出
           sum += t * (middle - i);
           b[index++] = s[i++];
           t = 0;
       }
   }

   while (i < middle)
       b[index++] = s[i++];
   while (j <= right)
       b[index++] = s[j++];

   index = 0;
   for (int m = left; m <= right; m++)
       s[m] = b[index++];
   return sum ;//最后返回给k值
}


void Merge(int s[], int low, int high)
{
   if (low < high)
   {
       int mid = (low + high) / 2;
       Merge(s, low, mid);
       Merge(s, mid + 1, high);
       k += MergeSort(s, low, mid + 1, high);
   }
}

int main()
{    
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
        
   Merge(a, 0, n - 1);
   printf("%lld\n", k);
   return 0;
}

