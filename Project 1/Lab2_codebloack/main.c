#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "bool.h"
#include <unistd.h>

unsigned long GC = 0;


// function declaration of main task methods
void main(void)
{
    // declare global variables used in the tasks, initialized to specified value
    unsigned int thrusterCommand = 0;
    unsigned short batteryLevel = 100;
    unsigned short fuelLevel = 100;
    unsigned short powerConsumption = 0;
    unsigned short powerGeneration = 0;
    Bool solarPanelState = FALSE;
    Bool fuelLow = FALSE;
    Bool batteryLow = FALSE;


    // define queue of TCB subtasks, as well as pointer to access TCB elements
    TCB* queue[6];
    TCB powerSubsystem;
    TCB thrusterSubsystem;
    TCB satelliteCommunications;
    TCB consoleDisplay;
    TCB warningAlarm;
    TCB* aTCBPtr;

    // initialize instances of data structs used to track data in each task
    powerData myPowerSubsystemData;
    thrusterData myThrusterSubsystemData;
    satelliteData mySatelliteSubsystemData;
    displayData myDisplayData;
    alarmData myAlarmData;

    // initialize data pointers to specific global variables
    myPowerSubsystemData.batteryLevel = &batteryLevel;
    myPowerSubsystemData.powerConsumption = &powerConsumption;
    myPowerSubsystemData.powerGeneration = &powerGeneration;
    myPowerSubsystemData.solarPanelState = &solarPanelState;

    myThrusterSubsystemData.fuelLevel = &fuelLevel;
    myThrusterSubsystemData.thrusterCommand = &thrusterCommand;

    mySatelliteSubsystemData.batteryLevel = &batteryLevel;
    mySatelliteSubsystemData.batteryLow = &batteryLow;
    mySatelliteSubsystemData.fuelLevel = &fuelLevel;
    mySatelliteSubsystemData.fuelLow = &fuelLow;
    mySatelliteSubsystemData.powerConsumption = &powerConsumption;
    mySatelliteSubsystemData.powerGeneration = &powerGeneration;
    mySatelliteSubsystemData.solarPanelState = &solarPanelState;
    mySatelliteSubsystemData.thrusterCommand = &thrusterCommand;

    myDisplayData.batteryLevel = &batteryLevel;
    myDisplayData.batteryLow = &batteryLow;
    myDisplayData.fuelLevel = &fuelLevel;
    myDisplayData.fuelLow = &fuelLow;
    myDisplayData.powerConsumption = &powerConsumption;
    myDisplayData.powerGeneration = &powerGeneration;
    myDisplayData.solarPanelState = &solarPanelState;

    myAlarmData.batteryLevel = &batteryLevel;
    myAlarmData.batteryLow = &batteryLow;
    myAlarmData.fuelLevel = &fuelLevel;
    myAlarmData.fuelLow = &fuelLow;

    // Initialize TCB's to point to data structure, as well as method name
    powerSubsystem.taskDataPtr = (void*)&myPowerSubsystemData;
     powerSubsystem.taskPtr = powerSystem;

    thrusterSubsystem.taskDataPtr = (void*)&myThrusterSubsystemData;
    thrusterSubsystem.taskPtr = thrusterSystem;

    satelliteCommunications.taskDataPtr = (void*)&mySatelliteSubsystemData;
    satelliteCommunications.taskPtr = satelliteCommSystem;

    consoleDisplay.taskDataPtr = (void*)&myDisplayData;
    consoleDisplay.taskPtr = consoleDisplayer;

    warningAlarm.taskDataPtr = (void*)&myAlarmData;
    warningAlarm.taskPtr = warningSystem;

    // Set TCB in queue, specifying order
    queue[0] = &consoleDisplay;
    queue[1] = &satelliteCommunications;
    queue[2] = &thrusterSubsystem;
    queue[3] = &powerSubsystem;
    queue[4] = &warningAlarm;
    // Leave queue[5] block for future application

    int i = 0;
    while(1) {
        aTCBPtr = queue[i];
        aTCBPtr->taskPtr( (aTCBPtr->taskDataPtr));


        if(i == 4){
                if(GC % MC == 0){
            usleep(5000);

        }
       GC++;

    }
     i = (i+1) % 5;
}
return;
}

