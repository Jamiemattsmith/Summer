#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "redpitaya/rp.h"
#define M_PI 3.14159265358979323846

int main(int argc, char **argv){
	float j;
	int i;
	int n;
	int cnt=0;
	float half[24576];
	int buff_size = 16384;
	uint32_t posnow = 0;
	uint32_t posold =0;
	float zeros[16384];
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
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_DC);
	rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS);
	//rp_GenArbWaveform(RP_CH_1, x, 16384);
	rp_GenAmp(RP_CH_1, 1.0);

	rp_GenFreq(RP_CH_1, 120.0);
	for(j=0.0;j<24576.0;j++){
		half[(int) j]=j/24576;
	}
	rp_updateData(RP_CH_1, zeros, 0,16384);	
	rp_GenOutEnable(RP_CH_1);

	while(1){
		posold=posnow;
		rp_GetReadPointer(&posnow);
		n=posnow-posold;
		n=n>0? n:16384+n;
		rp_updateData(RP_CH_1, half+cnt, posold,n);
		cnt = cnt+n;
		if (cnt>24576){	
			break;
		}
	}

	free(x);
	free(t);
	rp_Release();
}
