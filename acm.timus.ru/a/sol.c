int main() {
  int i;
  scanf("%d", &i);
  printf("%s\n", (i*(i+1)/2) % 2 ? "grimy" : "black");
  return 0;
}
