
#include <math.h>
#include <stdbool.h>

/* This function first computes the square root of n using the sqrt function 
from the math.h library. It then checks if the square of the square root of n 
is equal to n. If it is, the function returns true (1), otherwise, it returns 
false (0).*/

bool isSquareNumber(int n) {
   int sqrt_n = (int)sqrt(n);
   return (sqrt_n * sqrt_n == n);
}
