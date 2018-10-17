#ifndef STRUCTURES
#define STRUCTURES

# include "bool.h"
extern unsigned long GC;
#define MC 10000000


typedef struct powerSubystemData {
    Bool *solarPanelState;
    unsigned short *batteryLevel;
    unsigned short *powerConsumption;
    unsigned short *powerGeneration;
} powerData;

typedef struct thrusterSubsystemData {
    unsigned int *thrusterCommand;
    unsigned short *fuelLevel;
} thrusterData;

typedef struct satelliteComsData {
    Bool *fuelLow;
    Bool *batteryLow;
    Bool *solarPanelState;
    unsigned short *batteryLevel;
    unsigned short *fuelLevel;
    unsigned short *powerConsumption;
    unsigned short *powerGeneration;
    unsigned int *thrusterCommand;
} satelliteData;

typedef struct consoleDisplayData {
    Bool *fuelLow;
    Bool *batteryLow;
    Bool *solarPanelState;
    unsigned short *batteryLevel;
    unsigned short *fuelLevel;
    unsigned short *powerConsumption;
    unsigned short *powerGeneration;
} displayData;

typedef struct warningAlarmData {
    Bool *fuelLow;
    Bool *batteryLow;
    unsigned short *batteryLevel;
    unsigned short *fuelLevel;
} alarmData;

typedef struct
{
 void *taskDataPtr;
 void (*taskPtr)(void*);

}
TCB;

void consoleDisplayer(void* consoleStruct);
void powerSystem(void* powerStruct);
void thrusterSystem(void* thrusterStruct);
void warningSystem(void* alarmStruct);
void satelliteCommSystem(void* satelliteStruct);

#endif
