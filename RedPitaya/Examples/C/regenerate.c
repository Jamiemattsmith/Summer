
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846

int main(int argc, char **argv){

	int i;
	int buff_size = 32768;

	/* Print error, if rp_Init() function failed */
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
	}

	float *t = (float *)malloc(buff_size * sizeof(float));
	float *x = (float *)malloc(buff_size * sizeof(float));

	for(i = 1; i < buff_size; i++){
		t[i] = (2 * M_PI) / buff_size * i;
	}

	for (int i = 0; i < buff_size; ++i){
		x[i] = sin(t[i]);
	}
	float x1[16384];
	float x2[16384];
	memcpy(x1,x,16384*sizeof(float));
	memcpy(x2,&x[16384],16384*sizeof(float));
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
	rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS);
	rp_GenArbWaveform(RP_CH_1, x1, 16384);
	rp_GenArbWaveform(RP_CH_1, x2, 16384);
	rp_GenAmp(RP_CH_1, 0.7);

	rp_GenFreq(RP_CH_1, 4000.0);

	rp_GenOutEnable(RP_CH_1);
	/* Releasing resources */
	free(x);
	free(t);
	rp_Release();
}
