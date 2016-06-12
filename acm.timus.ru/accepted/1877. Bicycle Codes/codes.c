int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  printf("%s\n", (a % 2 == 0 || b % 2 == 1) ? "yes" : "no");
  return 0;
}
