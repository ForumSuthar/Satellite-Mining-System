#include"structures.h"
#include"bool.h"
#include <stdio.h>
#include <stdlib.h>




void consoleDisplayer(void * consoleStruct){

    static unsigned long count = 0;

    if((GC - count) % MC != 0 ){
        return;
    }

    count = GC;

    displayData *dData = (displayData*) consoleStruct;
    Bool *solarPanelState = dData->solarPanelState;
    Bool *fuelLow = dData->fuelLow;
    Bool *batteryLow = dData->batteryLow;
    unsigned short *batteryLevel = dData->batteryLevel;
    unsigned short *fuelLevel = dData->fuelLevel;
    unsigned short *powerConsumption = dData->powerConsumption;
    unsigned short *powerGeneration = dData->powerGeneration;

    char *solarpanelPrint = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelStatePrint = (*fuelLow) ? "YES" : "NO";
    char *batteryStatePrint = (*batteryLow) ? "YES" : "NO";

    static int currentDisplayCycles = 0;


   if(currentDisplayCycles % 4  == 0 || currentDisplayCycles % 4  == 1){
   printf("SATELLITE STATUS: \n"
	       "Solar Panels: %9s\n "
           "Battery Level: %3hu\n "
           "Fuel Level: %3hu\n "
           "Power Consumption: %2hu\n "
		   "Power Generation: %2hu\n",
		   solarpanelPrint, *batteryLevel, *fuelLevel, *powerConsumption, *powerGeneration);
   } else {

	printf("ANNUNCIATION:\n "
		   "Battery Low: %3s\n "
		   "Fuel Low: %3s\n\n",
		   batteryStatePrint, fuelStatePrint);
   }
   currentDisplayCycles ++;

}
