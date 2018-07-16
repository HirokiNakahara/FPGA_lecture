#include <stdio.h>
#include <stdlib.h>

#define ERROR_THRESH 0.1

void float_mult(float a, float b,float *c)
{
  *c =  a * b;
}
 
int main(void)
{
  float fres,fia,fib,fref, err;
  int ec = 0;
  for (unsigned int i = 0; i < 100; i++) {
    fia = (rand() % 1000) / 100.0;
    fib = (rand() % 1000) / 100.0;
    float_mult(fia, fib, &fres);
    fref = fia * fib;

    err = fref - fres;
    if( err < 0) err *= -1.0;
    
    printf("fia=%f fib=%f fres=%f fref=%f", fia, fib, fres, fref);
    if ( err > ERROR_THRESH) {
    	ec++;
    	printf(" ERROR\n");
    } else {
    	printf("\n");
    }
 }

 if( ec == 0) printf("[Test Passed]\n"); else printf("[Test Failed]\n");

 return 0;
}
