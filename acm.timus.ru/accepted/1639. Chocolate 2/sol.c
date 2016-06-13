int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  const char *r[2] = {":=[first", "second]=:"};
  printf("[%s]\n", r[a * b % 2]);
  return 0;
}
