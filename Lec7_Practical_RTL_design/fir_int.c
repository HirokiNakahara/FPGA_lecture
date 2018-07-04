#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 11
#define PREC 65536 // 2**16 sign + 15bit precision

void fir(int *y, int x)
{
	int c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
		-136, -397, -87, 3004, 8338, 11142, 8338,
		3004, -87, -397, -136, };
	
	static int shift_reg[N];
	int acc;
	int i;

	acc = 0;
	for (i = N - 1; i >= 0; i--) {
		if (i == 0) {
			acc += x * c[0];
			shift_reg[0] = x;
		} else {
			shift_reg[i] = shift_reg[i - 1];
			acc += shift_reg[i] * c[i];
		}
	}
	*y = acc;
}

void main()
{
	float fs = 44100.0;
	int len = 1000;

	float f0 = 20000.0;
	float sin_wave;
	int fir_out;

	int i;

	for( i = 0; i < len; i++){
		sin_wave = sin( 2.0 * M_PI * f0 * i / fs);

		fir( &fir_out, (int)(sin_wave * PREC));

		printf("%d %f %f\n", i, sin_wave, (float)fir_out / PREC);
		f0 = f0 - 10.0;
	}
}
