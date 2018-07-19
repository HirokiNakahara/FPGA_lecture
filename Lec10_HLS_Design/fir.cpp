#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ap_int.h"

#define N 11
#define PREC 65536 // 2**16 sign + 15bit precision

/* comment out for bitwith optmization part */
typedef ap_int<16> data_t;
typedef ap_int<16> coef_t;
typedef ap_int<24> acc_t;
/*
typedef int data_t;
typedef int coef_t;
typedef int acc_t;
*/
void fir_array_partition(int *y, int x)
{
	coef_t c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
		-136, -397, -87, 3004, 8338, 11142, 8338,
		3004, -87, -397, -136, };
#pragma HLS array_partition variable=c complete

	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;
	SHIFT_REG: for (i = N - 1; i > 0; i--) {
#pragma HLS unroll
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	MACs: for (i = N - 1; i >= 0; i--) {
#pragma HLS unroll
		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}

void fir_unroll(int *y, int x)
{
	coef_t c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
		-136, -397, -87, 3004, 8338, 11142, 8338,
		3004, -87, -397, -136, };

	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;
	FIR_LOOP: for (i = N - 1; i >= 0; i--) {
#pragma HLS unroll factor=4
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


void fir_loop_fission(int *y, int x)
{
	coef_t c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
		-136, -397, -87, 3004, 8338, 11142, 8338,
		3004, -87, -397, -136, };

	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;
	SHIFT_REG: for (i = N - 1; i > 0; i--) {
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	MACs: for (i = N - 1; i >= 0; i--) {
		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}


void fir_hoisting(int *y, int x)
{
	coef_t c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
		-136, -397, -87, 3004, 8338, 11142, 8338,
		3004, -87, -397, -136, };

	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;
	FIR_LOOP_NOIF: for (i = N - 1; i > 0; i--) {
		shift_reg[i] = shift_reg[i - 1];
		acc += shift_reg[i] * c[i];
	}

	acc += x * c[0];
	shift_reg[0] = x;

	*y = acc;
}


void fir(int *y, int x)
{
	coef_t c[N] = { // 0.17 = 20KHz/44.1KHz, LPF, Hamming Window
		-136, -397, -87, 3004, 8338, 11142, 8338,
		3004, -87, -397, -136, };

	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;
	FIR_LOOP: for (i = N - 1; i >= 0; i--) {
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

int main()
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

	return 0;
}
