/* Red Pitaya C API example Acquiring a signal from a buffer  
 * This application acquires a signal on a specific channel */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "redpitaya/rp.h"

#define NSMP 300000
typedef struct node
{
    float data;
    struct node* next;
} node;


node* create(float data,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
 
    return new_node;
}

node* prepend(node* head,float data)
{
    node* new_node = create(data,head);
    head = new_node;
    return head;
}

node* reverse(node* head)
{
    node* prev    = NULL;
    node* current = head;
    node* next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
}

void display(node* head, FILE *f)
{
    node* cursor = head;
    while(cursor != NULL)
    {
	if(cursor != NULL){
        	fprintf(f,"%f\n",cursor->data);
	}
        cursor = cursor->next;
    }
}

void dispose(node *head)
{
    node *cursor, *tmp;
 
    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}
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
	
	//node* head = NULL;
	float dat [NSMP];
	int cnt = 0;
	uint32_t posnow = 0;
	uint32_t posold = 0;
        uint32_t buff_size = 16384;
        float *buff = (float *)malloc(buff_size * sizeof(float));
	float freq=0;
        rp_AcqReset();
	rp_AcqSetArmKeep(true);
        rp_AcqSetDecimation(RP_DEC_64);
        rp_AcqSetTriggerLevel(RP_CH_1, 0);
        rp_AcqSetTriggerDelay(0);



        /* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/
        /* Here we have used time delay of one second but you can calculate exact value taking in to account buffer*/
        /*length and smaling rate*/


        rp_AcqStart();
	sleep(1);
        rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);

	rp_AcqGetWritePointer(&posnow);
	//int j;
	int i;
	while(1){
	//for(j = 0; j < 30; j++){
		buff_size=16384;
		posold=posnow;
		rp_AcqGetWritePointer(&posnow);
             
        	rp_AcqGetDataPosV(RP_CH_1,posold,posnow, buff, &buff_size);
        	for(i = 1; i < buff_size; i++){
                	//printf("%f\n", buff[i]);
			//fprintf(f, "%f\n", buff[i]);
			//head = prepend(head,buff[i]);
			if(cnt<NSMP){
				dat[cnt]=buff[i];
				cnt++;
			}
        	}
		//fprintf(f,"TOMETOYOU\n");
		if(cnt==NSMP){break;}
	}
	for(i = 0; i < NSMP; i++){
		fprintf(f,"%f\n",dat[i]);
	}
	//head=reverse(head);
	//display(head,f);
	//dispose(head);
	rp_AcqGetSamplingRateHz(&freq);
	printf("Smp Freq = %f\n",freq);
        /* Releasing resources */
        free(buff);
        rp_Release();
        return 0;
}
        
