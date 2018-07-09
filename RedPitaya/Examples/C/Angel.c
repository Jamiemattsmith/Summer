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

        rp_AcqStart();

        /* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/
        /* Here we have used time delay of one second but you can calculate exact value taking in to account buffer*/
        /*length and smaling rate*/

        sleep(1);
        rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);
	rp_AcqGetWritePointer(&posnow);
int j;
for(j = 0; j < 100; j++){
	posold=posnow;
	rp_AcqGetWritePointer(&posnow);                
        rp_AcqGetDataPosV(RP_CH_1,posold,posnow, buff, &buff_size);
        int i;
        	for(i = 0; i < posnow-posold; i++){
                	//printf("%f\n", buff[i]);
			fprintf(f, "%f\n", buff[i]);
        	}
	printf("%d\n",rp_AcqGetWritePointer(&posnow));
	printf("%d\n",posnow);
	}
        /* Releasing resources */
        free(buff);
        rp_Release();
        return 0;
}
        
