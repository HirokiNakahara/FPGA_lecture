#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 11

void fir(float *y, float x)
{
	float c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
	-4.120289718403869e-03, -1.208600321298122e-02, 	
	-2.650603053411641e-03,  9.166631627169690e-02,
	 2.544318483405623e-01,  3.400000000000001e-01,
	 2.544318483405623e-01,  9.166631627169690e-02,
	-2.650603053411641e-03, -1.208600321298122e-02,	 
	-4.120289718403869e-03, };
	
	static float shift_reg[N];
	float acc;
	int i;

	acc = 0.0;
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
	float fir_out;

	int i;

	for( i = 0; i < len; i++){
		sin_wave = sin( 2.0 * M_PI * f0 * i / fs);

		fir( &fir_out, sin_wave);

		printf("%d %f %f\n", i, sin_wave, fir_out);
		f0 = f0 - 10.0;
	}
}
