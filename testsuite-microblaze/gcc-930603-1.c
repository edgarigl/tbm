#include "sys.h"

float fx (x)
     float x;
{
  return 1.0 + 3.0 / (2.302585093 * x);
}

void check_gcc_930603_1 (void)
{
  float fx (), inita (), initc (), a, b, c;
  a = inita ();
  c = initc ();
  f ();
  b = fx (c) + a;
  f ();
  if (a != 3.0 || b < 4.3257 || b > 4.3258 || c != 4.0)
    err();
}

float inita () { return 3.0; }
float initc () { return 4.0; }
f () {}

