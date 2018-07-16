#include <stdio.h>
#include "platform.h"
#include "xfloat_mult.h"
#include "xparameters.h"

typedef union _ftoi {
  float f;
  unsigned int u;
} ftoi;

// from xparameters.h
#define XF_DEVICE_ID  	XPAR_FLOAT_MULT_0_DEVICE_ID

XFloat_mult XFT;// Instance for Vivado HLS Generated IP

// Testbench
float ref_float_mult( float a, float b)
{
  return a * b;
}

int main()
{
  int i,Status;
  XFloat_mult_Config *ConfigPtr;
  ftoi ref_a, ref_b, out_c;
  ftoi in_a, in_b;
  int cycle;

  // Initialization
  init_platform();
  XFloat_mult_Initialize( &XFT,XPAR_FLOAT_MULT_0_DEVICE_ID);

  ConfigPtr = XFloat_mult_LookupConfig(XF_DEVICE_ID);
  Status =  XFloat_mult_CfgInitialize(&XFT, ConfigPtr);
  if (Status != XST_SUCCESS) {
    return XST_FAILURE;
  }
  Status = XFloat_mult_IsIdle(&XFT);

  for (i=0;i<10;i++) {
    printf("TEST LOOP %d:\n",i);
    ref_a.f = (float)i;
    ref_b.f = (float)(i*100 + i);
    while(!XFloat_mult_IsReady(&XFT)) ;  // Check Ready Signal
    XFloat_mult_Set_a(&XFT, ref_a.u);  // Set register a
    XFloat_mult_Set_b(&XFT, ref_b.u);  // Set register b
    in_a.u = XFloat_mult_Get_a(&XFT);
    in_b.u = XFloat_mult_Get_b(&XFT);
    printf("  Get value a = %f(%x), b = %f(%x)\n",
                in_a.f,in_a.u,in_b.f,in_b.u);
    XFloat_mult_Start(&XFT);         // Start HW
    while (!XFloat_mult_IsDone(&XFT)) ;  // Wait Done signal

    out_c.u = (unsigned int)XFloat_mult_Get_c(&XFT); // Load register c
    cycle = XFloat_mult_Get_return(&XFT);

    printf("%d:  Get value c = %f(%x)\n", cycle, out_c.f,out_c.u);


    if ( ref_float_mult(ref_a.f, ref_b.f) != out_c.f )  // Comparison
      printf("  ERROR! ref = %f, val = %f\n",
                  ref_float_mult(ref_a.f, ref_b.f),out_c.f);
  }

  printf("float_mult test done.\n");
  cleanup_platform();
  return 0;
}
