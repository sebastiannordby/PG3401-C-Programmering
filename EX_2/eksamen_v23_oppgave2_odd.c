
#include <stdbool.h>

/* This function checks if the remainder of n divided by 2 is not equal to 0.
If it is not, the function returns true (1), indicating that n is an odd 
number. If the remainder is equal to 0, the function returns false (0), 
indicating that n is an even number.*/

bool isOdd(int n) {
   return (n % 2 != 0);
}
