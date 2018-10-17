#include "structures.h"
#include "bool.h"

void updatePowerConsumption(unsigned short *currentConsumptionLevel);
void updatePowerGeneration(Bool *solarPanelState, unsigned short *currentGenerationLevel, unsigned short *batteryLevel);
void updateBatteryLevel(Bool *solarPanelState, unsigned short *currentConsumptionLevel, unsigned short *currentGenerationLevel, unsigned short *batteryLevel);

void powerSystem (void* powerStruct) {
    // get data from pointers in data struct for use in method.

    static unsigned long count = 0;

    if((GC - count) % MC != 0 ){
        return;
    }

    count = GC;

    powerData *myPowerData = (powerData*)powerStruct;
    unsigned short *batteryLevel = myPowerData->batteryLevel;
    unsigned short *powerConsumptionLevel = myPowerData->powerConsumption;
    unsigned short *powerGenerationLevel = myPowerData->powerGeneration;
    Bool *solarPanelState = myPowerData->solarPanelState;

    // set static variable to record what iteration cycle is occurring in the variable.

    updatePowerConsumption(powerConsumptionLevel);
    updatePowerGeneration(solarPanelState, powerGenerationLevel, batteryLevel);
    updateBatteryLevel(solarPanelState, powerConsumptionLevel, powerGenerationLevel, batteryLevel);

}

void updatePowerGeneration(Bool *solarPanelState, unsigned short *currentGenerationLevel, unsigned short *batteryLevel){

    static int currentGenerationCycles = 0;

    if(*solarPanelState == TRUE){

        if(*batteryLevel > 95){
            *solarPanelState = FALSE;

        }else {
            if(*batteryLevel <= 50){
                if(currentGenerationCycles % 2 == 0){
                    *currentGenerationLevel += 2;
                }else{
                    *currentGenerationLevel += 1;
                }
            }else{
                if(*batteryLevel <= 95){
                    if(currentGenerationCycles % 2 == 0){
                        *currentGenerationLevel += 2;
                    }
                }
            }
        }
    } else{
        if(*batteryLevel <= 10){
            *solarPanelState = TRUE;
        }


    }
    currentGenerationCycles++;
}

    void updatePowerConsumption(unsigned short *currentConsumptionLevel) {
    // set static variable to record what iteration cycle is occurring in the variable.
    static int currentConsumptionCycles = 0;
    static Bool reverseDirection = FALSE; // initial value to false of below5 will increment to ten, and
    if(!reverseDirection) {
        if(currentConsumptionCycles % 2 == 0) {
            *currentConsumptionLevel += 2;
        } else {
            *currentConsumptionLevel += 1;
        }
        if(*currentConsumptionLevel >= 10) {
            reverseDirection = TRUE;
        }
    } else {
        if(currentConsumptionCycles % 2 == 0) {
            *currentConsumptionLevel -= 2;
        } else {
            *currentConsumptionLevel -= 1;
        }
        if(*currentConsumptionLevel <= 5) {
            reverseDirection = FALSE;
        }
    }
    currentConsumptionCycles++;
    if(*currentConsumptionLevel > 10) {
            *currentConsumptionLevel = 10;
    }
}


void updateBatteryLevel(Bool *solarPanelState, unsigned short *currentConsumptionLevel, unsigned short *currentGenerationLevel, unsigned short *batteryLevel){

    if(*solarPanelState == FALSE){
        *batteryLevel = *batteryLevel - (3 * (*currentConsumptionLevel));

    }else {
        *batteryLevel = *batteryLevel - *currentConsumptionLevel + *currentGenerationLevel;
    }

    if(*batteryLevel >= 100){

        *batteryLevel = 100;
    }
}
