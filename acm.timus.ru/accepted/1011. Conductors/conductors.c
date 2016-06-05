/*
Background
Everyone making translations from English to Russian knows an English phrase
"Naked conductor runs along the bus". It has two very different meanings.
Problem
Every bus in the Ekaterinburg city has a special man (or woman) called
conductor. When you ride the bus, you have to give money to the conductor. We
know that there are more than P% conductors and less than Q% conductors of all
citizens of Ekaterinburg. Your task is to determine a minimal possible number of
Ekaterinburg citizens. By percentage, we know that there are more than P%
conductors and less than Q% conductors of all Russian citizens in this city
Input
Two numbers P,Q such that 0.01 ≤ P, Q ≤ 99.99. Numbers are given with 2 digits
precision. These numbers are separated by some spaces or "end of line" symbols.
Output
The minimal number of Ekaterinburg citizens.
Sample input
13
14.1
Sample output
15

Notes
If there are 15 citizens and 2 conductors among them in Ekaterinburg, then there
are 13 1/3 % conductors of all citizens.
*/
int main() {
  typedef long long int num_t;
#define X(x) x##ll
  const num_t s1 = X(1000);
  const num_t s2 = s1 * X(100);
  num_t np, nq;
  {
    double p, q;
#if ONLINE_JUDGE
    scanf("%lg%lg", &p, &q);
#else
    p = 24.80;
    q = 24.83;
#endif
    const double E = 1e-9;
    np = (p + E) * (double)s1;
    nq = (q + E) * (double)s1;
  }

  // printf("p = %lld, q = %lld\n", np, nq);

  num_t citizens, min_conductors, max_conductors;

  for (citizens = 1; citizens < 20000; ++citizens) {
    min_conductors = citizens * np;
    max_conductors = citizens * nq;

    num_t a1 =
        min_conductors + ((min_conductors % s2) ? s2 - min_conductors % s2 : 0);
    num_t b1 = max_conductors - max_conductors % s2;

    if (a1 > b1) {
      continue;
    }

    if (min_conductors < a1 && a1 < max_conductors)
      break;
    if (b1 - a1 > s2)
      break;
  }
  printf("%d\n", (int)citizens);
  return 0;
}
