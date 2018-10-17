#include "structures.h"
#include "bool.h"
#include <stdlib.h>
#include <stdint.h>
int randomInteger(int low, int high);

void satelliteCommSystem(void* satelliteStruct) {
    static unsigned long count = 0;

    if((GC - count) % MC != 0 ){
        return;
    }

    count = GC;
    satelliteData *mySatelliteData = (satelliteData*)satelliteStruct;
    unsigned short *currentBatteryLevel = mySatelliteData->batteryLevel;
    Bool *batteryLowStatus = mySatelliteData->batteryLow;
    unsigned short *currentFuelLevel = mySatelliteData->fuelLevel;
    Bool *fuelLowStatus = mySatelliteData->fuelLow;
    unsigned short *currentPowerConsumption = mySatelliteData->powerConsumption;
    unsigned short *currentPowerGeneration = mySatelliteData->powerGeneration;
    Bool *currentSolarPanelState = mySatelliteData->solarPanelState;
    unsigned int *currentThrusterCommand = mySatelliteData->thrusterCommand;

    *currentThrusterCommand = randomInteger(0,65536) % 65536; // 65536 max value of 2^16
    uint16_t correctDirectionFactor = 0xFFF3; // equal to 1111111111110011
    *currentThrusterCommand &= correctDirectionFactor;

}
