#include <upc.h>
int main(void) {
   volatile shared int *p = 0;
   volatile int i = 0;

   int a = p        ? 0 : 1; // Translated correctly
   int b = (p && i) ? 0 : 1; // Translated incorrectly

   return 0;
}
