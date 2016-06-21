#if ONLINE_JUDGE||1
static char buf[4096];
static int buf_pos = 0;
static int buf_max = 0;
static int get() {
  if (buf_pos >= buf_max) {
    buf_pos = 0;
    buf_max = read(0, buf, sizeof(buf));
  }
  if (buf_max < 1) {
    return 0;
  }
  return buf[buf_pos++];
}
#else
static char test[] = "wwstdaadierfflitzzz";
static int test_pos = 0;
static int get() { return test[test_pos++]; }
#endif
static char msg[200000];
int main() {
	char * begin = msg;
	char * pos = begin;
	char c;
	while((c = get()) >= ' ') {
		if(pos == begin) {
			*pos++ = c;
			continue;
		}
		if(c == pos[-1]) {
			--pos;
		} else {
			*pos++ = c;
		}
	}
	write(1, begin, pos - begin);
	write(1, "\n", 1);
	return 0;
}
