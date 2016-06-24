int main() {
	int maxA, maxB, C;
#if ONLINE_JUDGE
	scanf("%d%d%d", &maxA, &maxB, &C);
#else
	maxA = 2;
	maxB = 7;
	C = 5;
#endif
	if(maxA + maxB < C) {
		printf("Impossible\n");
	} else {
		int a = maxA;
		if(a > C) {
			a = C;
		}
		printf("%d %d\n", a, C - a);
	}
  return 0;
}
