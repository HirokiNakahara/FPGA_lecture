#include <stdio.h>
#include <stdlib.h>

#define ERROR_THRESH 0.1

typedef union _ftoi {
  float f;
  unsigned int u;
} ftoi;

int float_mult(float a, float b, float *c)
{
#pragma HLS INTERFACE s_axilite register port=c bundle=slv0
#pragma HLS INTERFACE s_axilite port=b bundle=slv0
#pragma HLS INTERFACE s_axilite port=a bundle=slv0
#pragma HLS INTERFACE s_axilite register port=return bundle=slv0

  static int cycle = 0;

  *c =  a * b;
  cycle++;

  return cycle;
}
 
int main(void)
{
//  float fres,fia,fib,fref, err;
  float fref, err;
  ftoi fres,fia, fib;
  int cycle;
  int ec = 0;
  for (unsigned int i = 0; i < 100; i++) {
    fia.f = (float)(rand() % 1000) / 100.0;
    fib.f = (float)(rand() % 1000) / 100.0;
    cycle = float_mult( fia.f, fib.f, &fres.f);
    fref = fia.f * fib.f;

    err = fref - fres.f;
    if( err < 0) err *= -1.0;
    
    printf("%d: fia=%f fib=%f fres=%f fref=%f", cycle, fia.f, fib.f, fres.f, fref);
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
