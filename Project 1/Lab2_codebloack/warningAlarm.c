#include "structures.h"
#include "bool.h"
#include <stdio.h>

void updateWarningSystem(unsigned short *currBatteryLevel, unsigned short *currFuelLevel);
void ClearScreen();
void delay(unsigned long delay);

void warningSystem(void* alarmStruct) {
    alarmData *myAlarmData = (alarmData*)alarmStruct;
    Bool *fuelLowIndicator = myAlarmData->fuelLow;
    Bool *batteryLowIndicator = myAlarmData->batteryLow;
    unsigned short *currentBatteryLevel = myAlarmData->batteryLevel;
    unsigned short *currentFuelLevel = myAlarmData->fuelLevel;

    //updateWarningSystem(currentBatteryLevel, currentFuelLevel);

}

void updateWarningSystem(unsigned short *currBatteryLevel, unsigned short *currFuelLevel) {
    static unsigned long delayTime = 5000000;
    if(*currFuelLevel < 10) {
        printf("FUEL");
        delay(delayTime);
        printf("%c", 0x0d);
        ClearScreen();
        delay(delayTime);
    } else if(*currBatteryLevel < 10) {
        delayTime = 10000000;
        printf("BATTERY");
        delay(delayTime);
        printf("%c", 0x0d);
        ClearScreen();
        delay(delayTime);
    } else if(*currFuelLevel < 50) {
        delayTime = 5000000;
        printf("FUEL");
        delay(delayTime);
        printf("%c", 0x0d);
        ClearScreen();
        delay(delayTime);
    } else if(*currBatteryLevel < 50) {
        delayTime = 10000000;
        printf("BATTERY");
        delay(delayTime);
        printf("%c", 0x0d);
        ClearScreen();
        delay(delayTime);
    } else {
        printf("FUEL BATTERY");
    }
}


void ClearScreen() /* method to simulate clearing screen */{
    volatile int i; // initialize loop variable
    for(i = 0; i < 8;i++) { // run loop 8 times
        printf(" "); // print spaces to simulate clearing screen
    }
    printf("%c", 0x0d); // move cursor to beginning of console
}

void delay(unsigned long delay) /*method to create time delay */ {
    volatile int i; // initialize loop variable
    for(i = 0; i < delay;) { // simulate delay for loop
        i++;
    }
 }
