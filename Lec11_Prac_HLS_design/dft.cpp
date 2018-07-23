#include <stdio.h>
#include <math.h>

typedef float data_t;
typedef float cmpt_t;

void DFT(int n, data_t *real, data_t *imag)
{
    cmpt_t tmpReal[n], tmpImag[n];
    
    for (int i = 0; i < n; i++) {
        
        tmpReal[i] = 0.0;
        tmpImag[i] = 0.0;
        
        float d = 2.0 * M_PI * i / n;
        
        for (int j = 0; j < n; j++) {
            
            float phase = d * j;
            
            tmpReal[i] += real[j] * cos(phase);
            tmpImag[i] -= real[j] * sin(phase);
        }
    }
    
    for (int i = 0; i < n; i++) {
        real[i] = (data_t)tmpReal[i];
        imag[i] = (data_t)tmpImag[i];
    }
}

int main (int argc, const char * argv[]) {
    
    int n = 16;
    data_t real[n], imag[n];
    
    data_t d = 2.0 * M_PI / n;
    
    for (int i = 0; i < n; i++) {
        real[i] = sin(1.0 * i * d); //1Hz Sin Wave
        real[i] += sin(3.0 * i * d + M_PI_4); // 3Hz Sin Wave
        real[i] += sin(5.0 * i * d + M_PI_2); // 5Hz Sin Wave
        imag[i] = 0.0;
    }
    
    //　Discrete Fourier Transform
    DFT(n, real, imag);
    
    for (int i = 0; i < n; i++) {
        printf("%dHz %f\n", i, sqrt(real[i] * real[i] + imag[i] * imag[i]));
    }
    
    return 0;
}
