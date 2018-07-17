#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "redpitaya/rp3.h"
#define M_PI 3.14159265358979323846

int main(int argc, char **argv){

	int i;
	float half[16384];
	int buff_size = 16384;
	uint32_t posnow = 0;
	int *readings=(int *)malloc(2000 * sizeof(int));
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
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
	rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS);
	rp_GenArbWaveform(RP_CH_1, x, 16384);
	rp_GenAmp(RP_CH_1, 0.7);

	rp_GenFreq(RP_CH_1, 120.0);
	for(i=0;i<16384;i++){
		half[i]=0.5;
	}
	rp_GenOutEnable(RP_CH_1);
	for(i=0;i<2000;i++){
	rp_GetReadPointer(&posnow);
	generate_updateData(RP_CH_1, half, posnow-1,1);
	}
	for(i=0;i<2000;i++){
	printf("%d\n",readings[i]);
	}
	/* Releasing resources */
	free(x);
	free(t);
	rp_Release();
}
