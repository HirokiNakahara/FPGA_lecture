#include <stdio.h>
#include <math.h>

typedef float data_t;
typedef float cmpt_t;

void DFT( data_t real[16], data_t imag[16])
{
    cmpt_t tmpReal[16], tmpImag[16];

    DFT_LOOP: for (int i = 0; i < 16; i++) {

        tmpReal[i] = 0.0;
        tmpImag[i] = 0.0;

        float d = 2.0 * M_PI * i / 16;

        DFT_MAC: for (int j = 0; j < 16; j++) {

            float phase = d * j;

            tmpReal[i] += real[j] * cos(phase);
            tmpImag[i] -= real[j] * sin(phase);
        }
    }

    WB: for (int i = 0; i < 16; i++) {
        real[i] = (data_t)tmpReal[i];
        imag[i] = (data_t)tmpImag[i];
    }
}

int main (int argc, const char * argv[])
{
    data_t real[16], imag[16];

    data_t d = 2.0 * M_PI / 16;

    for (int i = 0; i < 16; i++) {
        real[i] = sin(1.0 * i * d); //1Hz Sin Wave
        real[i] += sin(3.0 * i * d + M_PI_4); // 3Hz Sin Wave
        real[i] += sin(5.0 * i * d + M_PI_2); // 5Hz Sin Wave
        imag[i] = 0.0;
    }

    //　Discrete Fourier Transform
    DFT( real, imag);

    for (int i = 0; i < 16; i++) {
        printf("%dHz %f\n", i, sqrt(real[i] * real[i] + imag[i] * imag[i]));
    }

    return 0;
}
