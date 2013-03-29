#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

void sgenrand (unsigned long seed);
unsigned long genrand();
int rand_reg (int begin, int end);
double rand_double ();

#ifdef  __cplusplus
}
#endif
