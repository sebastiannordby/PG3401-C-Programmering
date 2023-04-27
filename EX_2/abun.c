
#include <stdbool.h>

/* This function uses a for loop to iterate through the numbers 1 to n-1 and 
checks if each number is a divisor of n. If it is, it adds the divisor to a 
sum. After the loop finishes, the function checks if the sum of the divisors is
greater than n. If it is, the function returns true (1), indicating that n is 
an abundant number. If the sum is not greater than n, the function returns 
false (0), indicating that n is not an abundant number.*/

bool isAbundantNumber(int n) {
   int sum = 0;
   for (int i = 1; i < n; i++) {
      if (n % i == 0) {
         sum += i;
      }
   }
   return (sum > n);
}
