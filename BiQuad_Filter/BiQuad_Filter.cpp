// float samplerate … sampling freq.
// float freq … cut-off freq.
// float q    … Q value, must q>0, typicall it is set to 1/root(2)

void BiQuad_Filter_for_Highpass(float input[], float output[], int size, float samplerate, float freq, float q)
{
	// filter coefficient
	float omega = 2.0f * 3.14159265f *  freq　/　samplerate;
	float alpha = sin(omega) / (2.0f * q);
 
	float a0 =   1.0f + alpha;
	float a1 =  -2.0f * cos(omega);
	float a2 =   1.0f - alpha;
	float b0 =  (1.0f + cos(omega)) / 2.0f;
	float b1 = -(1.0f + cos(omega));
	float b2 =  (1.0f + cos(omega)) / 2.0f;
 
	// buffers
	float in1  = 0.0f;
	float in2  = 0.0f;
	float out1 = 0.0f;
	float out2 = 0.0f;
 
	// BiQuad Filtering
	for(int i = 0; i < size; i++)
	{
		output[i] = b0/a0 * input[i] + b1/a0 * in1  + b2/a0 * in2
		                             - a1/a0 * out1 - a2/a0 * out2;
 
		in2  = in1;      
		in1  = input[i]; 
 
		out2 = out1;     
		out1 = output[i];
	}
}
