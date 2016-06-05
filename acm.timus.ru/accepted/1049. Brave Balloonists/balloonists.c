/*
Ten mathematicians are flying on a balloon over the Pacific ocean. When they are
crossing the equator they decide to celebrate this event and open a bottle of
champagne. Unfortunately, the cork makes a hole in the balloon. Hydrogen is
leaking out and the balloon is descending now. Soon it will fall into the ocean
and all the balloonists will be eaten by hungry sharks.
But not everything is lost yet. One of the balloonists can sacrifice himself
jumping out, so that his friends would live a little longer. Only one problem
still exists: who is the one to get out. There is a fair way to solve this
problem. First, each of them writes an integer ai not less than 1 and not more
than 10000. Then they calculate the magic number N that is the number of
positive divisors of the product a1*a2*…*a10. For example, the number of
positive integer divisors of 6 is 4 (they are 1,2,3,6). The hero (a
mathematician who will be thrown out) is determined according to the last digit
of N. Your task is to find this digit.
Input
Input contains ten integer numbers (each number is in separate line).
Output
Output a single digit from 0 to 9 — the last digit of N.
Sample input
1
2
6
1
3
1
1
1
1
1
Sample output
9
*/
static int primes[1229] = {2, 3, 5};
static void prepare_primes() {
  int cur = 2, num = 7, incr = 4;
  while (cur < 1228) {
    int *p = primes, *end = primes + cur + 1, flag = 1;
    while (p < end && flag) {
      flag = (num % *p++) != 0;
    }
    if (flag) {
      primes[++cur] = num;
    }
    num += incr;
    incr = 6 - incr;
  }
}
int main() {
  int a[10];
  {
#ifdef ONLINE_JUDGE
    int b;
    for (b = 0; b < 10; ++b) {
      scanf("%d", a + b);
    }
#else
    int b[10] = {1, 2, 6, 1, 3, 1, 1, 1, 1, 1};
    memcpy(a, b, sizeof(b));
#endif
  }
  prepare_primes();

  int dc[1229] = {
      0,
  };
  int i, j;
  for (i = 0; i < 10; ++i) {
    int cur = a[i];
    for (j = 0; j < 1229 && cur >= primes[j]; ++j) {
      const int p = primes[j];
      while (cur % p == 0) {
        cur /= p;
        dc[j] += 1;
      }
    }
  }
  int N = 1;
  for (i = 0; i < 1229; ++i) {
    N = (N * (dc[i] + 1)) % 10;
  }
  printf("%d\n", N);

  return 0;
}
