// This file was was downloaded from the link below and provided in the exam text.
// Only modification was changing the function name to snake case.
// http://www.eastwill.no/pg3401/eksamen_v23_oppgave2_cube.c

#include <math.h>
#include <stdbool.h>

/* This function first computes the cubic root of n using the cbrt function 
from the math.h library. It then checks if the cbrt of the cbrt root of n 
is equal to n. If it is, the function returns true (1), otherwise, it returns 
false (0)..*/

bool is_cube_number(int n) {
   int cube_n = (int)cbrt(n);
   return (cube_n * cube_n * cube_n == n);
}
