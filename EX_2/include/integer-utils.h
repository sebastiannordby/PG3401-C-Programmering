
#ifndef __INTEGER_UTILS__
#define __INTEGER_UTILS__
#include <stdbool.h>
#include <math.h>

/* 
   This function uses a for loop to iterate through the numbers 1 to n-1 and 
   checks if each number is a divisor of n. If it is, it adds the divisor to a 
   sum. After the loop finishes, the function checks if the sum of the divisors is
   greater than n. If it is, the function returns true (1), indicating that n is 
   an abundant number. If the sum is not greater than n, the function returns 
   false (0), indicating that n is not an abundant number.
*/
bool isAbundantNumber(int n);

/* 
   This function first computes the cubic root of n using the cbrt function 
   from the math.h library. It then checks if the cbrt of the cbrt root of n 
   is equal to n. If it is, the function returns true (1), otherwise, it returns 
   false (0)..
*/
bool isCubeNumber(int n);

/* 
   This function uses a for loop to iterate through the numbers 1 to n-1 and 
   checks if each number is a divisor of n. If it is, it adds the divisor to a 
   sum. After the loop finishes, the function checks if the sum of the divisors is
   less than n. If it is, the function returns true (1), indicating that n is a 
   deficient number. If the sum is not less than n, the function returns false 
   (0), indicating that n is not a deficient number.
*/
bool isDeficientNumber(int n);

/* 
   This function uses a while loop to iterate through the Fibonacci sequence 
   until it either finds a number equal to n or a number greater than n. If a 
   number equal to n is found, the function returns true (1), otherwise, it 
   returns false (0).
*/
bool isFibonacci(int n);

/* 
   This function first computes the square root of n using the sqrt function 
   from the math.h library. It then checks if the square of the square root of n 
   is equal to n. If it is, the function returns true (1), otherwise, it returns 
   false (0).
*/
bool isSquareNumber(int n);

/* 
   This function checks if the remainder of n divided by 2 is not equal to 0.
   If it is not, the function returns true (1), indicating that n is an odd 
   number. If the remainder is equal to 0, the function returns false (0), 
   indicating that n is an even number.
*/
bool isOdd(int n);

/* 
   This function uses a for loop to iterate through the numbers 1 to n-1 and 
   checks if each number is a divisor of n. If it is, it adds the divisor to a 
   sum. After the loop finishes, the function checks if the sum of the divisors is
   equal to n. If it is, the function returns true (1), indicating that n is a 
   perfect number. If the sum is not equal to n, the function returns false (0), 
   indicating that n is not a perfect number.
*/
bool isPerfectNumber(int n);

/* 
   This function uses a for loop to iterate through the numbers 2 to n-1. If n
   is divisible by any of these numbers, it returns false (0), indicating that n 
   is not a prime number. If the loop finishes without finding a divisor, the 
   function returns true (1), indicating that n is a prime number.
*/
bool isPrime(int n);

#endif