int main() {
  int i;
  scanf("%d", &i);
  const char *result = "legion";
  if (i < 5) {
    result = "few";
  } else if (i < 10) {
    result = "several";
  } else if (i < 20) {
    result = "pack";
  } else if (i < 50) {
    result = "lots";
  } else if (i < 100) {
    result = "horde";
  } else if (i < 250) {
    result = "throng";
  } else if (i < 500) {
    result = "swarm";
  } else if (i < 1000) {
    result = "zounds";
  }
	printf("%s\n", result);
  return 0;
}
