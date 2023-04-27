
#include <stdbool.h>

/* This function uses a while loop to iterate through the Fibonacci sequence 
until it either finds a number equal to n or a number greater than n. If a 
number equal to n is found, the function returns true (1), otherwise, it 
returns false (0).*/


bool isFibonacci(int n) {
   int a = 0, b = 1, c = a + b;
   while (c < n) {
      a = b;
      b = c;
      c = a + b;
   }
   return (c == n);
}
