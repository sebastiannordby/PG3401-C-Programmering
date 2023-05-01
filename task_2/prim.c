// This file was was downloaded from the link below and provided in the exam text.
// Only modification was changing the function name to snake case.
// http://www.eastwill.no/pg3401/eksamen_v23_oppgave2_prim.c

#include <stdbool.h>

/* This function uses a for loop to iterate through the numbers 2 to n-1. If n
is divisible by any of these numbers, it returns false (0), indicating that n 
is not a prime number. If the loop finishes without finding a divisor, the 
function returns true (1), indicating that n is a prime number.*/

bool is_prime(int n) {
   if (n <= 1) return false;
   for (int i = 2; i < n; i++) {
      if (n % i == 0) {
         return false;
      }
   }
   return true;
}
