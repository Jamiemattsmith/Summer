/* Red Pitaya C API example Acquiring a signal from a buffer  
 * This application acquires a signal on a specific channel */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "redpitaya/rp.h"

int main(int argc, char **argv){

        /* Print error, if rp_Init() function failed */
        if(rp_Init() != RP_OK){
                fprintf(stderr, "Rp api init failed!\n");
        }
	
	FILE *f = fopen("data.txt", "w");
	if (f == NULL)
	{
    		printf("Error opening file!\n");
    		exit(1);
	}

	uint32_t posnow = 0;
	uint32_t posold = 0;
        uint32_t buff_size = 16384;
        float *buff = (float *)malloc(buff_size * sizeof(float));

        rp_AcqReset();
        rp_AcqSetDecimation(8192);
        rp_AcqSetTriggerLevel(RP_CH_1, 0);
        rp_AcqSetTriggerDelay(0);



        /* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/
        /* Here we have used time delay of one second but you can calculate exact value taking in to account buffer*/
        /*length and smaling rate*/


        rp_AcqStart();
	sleep(1);
        rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);
        rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;

        /*while(1){
                rp_AcqGetTriggerState(&state);
                if(state == RP_TRIG_STATE_TRIGGERED){
                break;
                }
        }*/
	rp_AcqGetWritePointer(&posnow);
int j;
int i;
int n;
for(j = 0; j < 2; j++){
	posold=posnow;
	rp_AcqGetWritePointer(&posnow);
	printf("%d\n",posold);
	printf("%d\n",posnow); 
	if((posnow-posold)>0){
		n = posnow-posold;
	}
	else{
		n=16384+posnow-posold;
	}               
       rp_AcqGetDataPosV(RP_CH_1,posold,posnow, buff, &buff_size);
        	for(i = 0; i < n; i++){
                	printf("%f\n", buff[i]);
			fprintf(f, "%f\n", buff[i]);
        	}
		printf("n = %d\n", n);
	//}
	/*else{
		for(i=posold;i<16384;i++){
                	printf("%f\n", buff[i]);
			fprintf(f, "%f\n", buff[i]);
        	}
		for(i=0;i<posnow;i++){
                	printf("%f\n", buff[i]);
			fprintf(f, "%f\n", buff[i]);
        	}
	}*/	
	//printf("%d\n",n);
	printf("%d\n",posold);
	printf("%d\n",posnow);
	}
        /* Releasing resources */
        free(buff);
        rp_Release();
        return 0;
}
        
