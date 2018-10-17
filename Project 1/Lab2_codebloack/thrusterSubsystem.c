#include"structures.h"
#include"bool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h> // for exit


void thrusterSystem(void* thrusterStruct){

    static unsigned long count = 0;

    if((GC - count) % MC != 0 ){
        return;
    }

    count = GC;

    thrusterData *tData = (thrusterData*) thrusterStruct;
    unsigned int *thrusterCommand = tData -> thrusterCommand;
    unsigned short *fuelLevel = tData -> fuelLevel;


    uint16_t Mask = 0xF;


    uint8_t direction = *thrusterCommand & Mask;
    uint8_t Magnitude = (*thrusterCommand >> 4) & Mask;
    uint8_t Duration = (*thrusterCommand >> 8) & Mask;

    double fuelCost;


    fuelCost = 0.00012845* Magnitude * Duration;

    if(fuelLevel == 0){
        printf("out of fuel");
    exit(0);
    }else {
         *fuelLevel =  *fuelLevel - (unsigned short) fuelCost;
    }
}
