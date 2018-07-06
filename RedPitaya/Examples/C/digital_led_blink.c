#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "redpitaya/rp.h"

int main (int argc, char **argv) {
    int unsigned period = 1000000; // uS
    int unsigned led;

    // index of blinking LED can be provided as an argument
    if (argc > 1) {
        led = atoi(argv[1]);
    // otherwise LED 0 will blink
    } else {
        led = 0;
    }
    printf("Blinking LED[%u]\n", led);
    led += RP_LED0;

    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }


FILE *f = fopen("file.txt", "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

/* print some text */
const char *text = "Write this to the file";
fprintf(f, "Some text: %s\n", text);

/* print integers and floats */
int i = 1;
float py = 3.1415927;
fprintf(f, "Integer: %d, float: %f\n", i, py);

/* printing single chatacters */
char c = 'A';
fprintf(f, "A character: %c\n", c);

fclose(f);



    int unsigned retries = 1000;
    while (retries--){
        rp_DpinSetState(led, RP_HIGH);
        usleep(period/2);
        rp_DpinSetState(led, RP_LOW);
        usleep(period/2);
    }

    // Releasing resources
    rp_Release();

    return EXIT_SUCCESS;
}
