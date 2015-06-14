#include <stdio.h>  
#define N 3
long c;
long A[N];

long P(long l, long r)//partition, pivot - [l], return value: items in prefix are less that pivot and items in suffix are greater than pivot
{
  long x=A[l],//pivot
        i=l-1,
        j=r+1,
        t;
  while(1)
  {
    do{--j; ++c;}
    while(A[j]>x);// find right item in A that is <= x, add suffix len to c

    do{++i; ++c;}
    while(A[i]<x);// find left item in A that is >= x, add prefix len to c

    if(i<j)//if left < right - swap them, now A[i] <= A[j] 
    {
      t=A[i];
      A[i]=A[j];
      A[j]=t;
    }
    else return j;
  }
}

void Q(long l, long r)//quick sort
{
  long n;
  if(l<r)
  {
    n=P(l,r);
    Q(l,n);
    Q(n+1,r);
  }
}

int main(void)
{
  c=0;
  for(long i=0; i<N; ++i) {
    scanf("%ld", &A[i]);
  }
  Q(0,N-1);
  if(c==(N*N+3*N-4)/2) {// D = 3 * 3 + 4 * 4 = 25, sqrt D = 5, roots: -4 1, (N+4)*(N-1)
    printf ("Beutiful Vasilisa");
  } else {
    printf ("Immortal Koshcei");
  }
  return 0;
}

