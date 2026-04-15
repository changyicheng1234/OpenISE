#include <iostream>
#include <stdio.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */


long long int m[401][401];//m为存储最优结果的二维矩阵,相当于备忘录，m[i][j]储存的是i矩阵到j矩阵相乘的最小运算值 
int p[401];
void MatrixChainOrder(int *p,int n)
{
    int l,i,j,k;
    long long int q =0;
    
    //m[i][i]只有一个矩阵，所以相乘次数为0，即m[i][i]=0;
    for(i=1;i<=n;i++)
    {
        m[i][i]=0;
    }
    //l表示矩阵链的长度
    // l=2时，计算 m[i,i+1],i=1,2,...,n-1 (长度l=2的链的最小代价)
    for(l=2;l<=n;l++)
    {
        for(i=1;i<=n-l+1;i++)
        {
            j=i+l-1; //以i为起始位置，j为长度为l的链的末位，
            m[i][j]=0x7fffffff;
            //k从i到j-1,以k为位置划分
            for(k=i;k<=j-1;k++)
            {
                q=m[i][k]+m[k+1][j]+p[i-1]*p[k]*p[j];
                if(q<m[i][j])
                {
                    m[i][j]=q;
                }
            }
        }
    }
    printf("%lld",m[1][n]); 
}

int main()
{
	int n,i;	
	scanf("%d", &n); //有n个矩阵 

	for (i=0; i<=n; i++){
		scanf("%d",&p[i]);
	} 
    MatrixChainOrder(p,n);
    return 0;
} 
