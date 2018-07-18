#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "redpitaya/rp.h"
#define M_PI 3.14159265358979323846

int main(int argc, char **argv){
	int len=0;
	int c;
	//float j;
	int i;
	int n;
	int cnt=0;
	
	//int buff_size = 16384;
	uint32_t posnow = 0;
	uint32_t posold =0;
	float zeros[16384]={0.0};
	/* Print error, if rp_Init() function failed */
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
	}

	
	FILE *f = fopen("data.txt", "r");
	if (f == NULL)
	{
    		printf("Error opening file!\n");
    		exit(1);
	}

	for (c=getc(f);c!= EOF; c= getc(f)){
		if (c=='\n'){
			printf("%c  %d\n",(char)c,len);
			len++;
		}
	}
	printf("Counted Data: %d\n",len);
	rewind(f);
	float half[len];
	printf("Gone to top of file\n");
	for (i=0; i<len; i++){
		fscanf(f,"%f\n",&half[i]);
		printf("%d\n",i);
	}
	printf("Saved Data\n");
	for (i=0; i<len; i++){
		printf("%f\n",half[i]);
	}
	printf("Presented Data\n");
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_DC);
	rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS);
	rp_GenAmp(RP_CH_1, 1.0);

	rp_GenFreq(RP_CH_1, 120.0);
	/*for(j=0.0;j<len;j++){
		half[(int) j]=j/len;
	}*/
	rp_updateData(RP_CH_1, zeros, 0,16384);
	rp_GenOutEnable(RP_CH_1);
	//sleep(2);
	while(1){
		posold=posnow;
		rp_GetReadPointer(&posnow);
		n=posnow-posold;
		n=n>0? n:16384+n;
		if (cnt !=len){
			if (cnt+n>len){
				n=len-cnt;
			}
			rp_updateData(RP_CH_1, half+cnt, posold,n);
			cnt = cnt+n;
		}
	}

	rp_Release();
	fclose(f);
}
