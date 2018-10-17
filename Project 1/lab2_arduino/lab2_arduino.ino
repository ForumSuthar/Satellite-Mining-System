#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#define MC 10000000

// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xAA60

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//enum myBool { false = 0, true = 1 };
//typedef enum myBool Bool;

extern unsigned long GC;
unsigned long time1;
unsigned long GC = 0;

struct powerData {
  bool *solarPanelState;
  unsigned short *batteryLevel;
  unsigned short *powerConsumption;
  unsigned short *powerGeneration;
} ;

struct thrusterData {
  unsigned int *thrusterCommand;
  unsigned short *fuelLevel;
} ;

struct satelliteData {
  bool *fuelLow;
  bool *batteryLow;
  bool *solarPanelState;
  unsigned short *batteryLevel;
  unsigned short *fuelLevel;
  unsigned short *powerConsumption;
  unsigned short *powerGeneration;
  unsigned int *thrusterCommand;
} ;

struct displayData {
  bool *fuelLow;
  bool *batteryLow;
  bool *solarPanelState;
  unsigned short *batteryLevel;
  unsigned short *fuelLevel;
  unsigned short *powerConsumption;
  unsigned short *powerGeneration;
} ;

struct alarmData {
  bool *fuelLow;
  bool *batteryLow;
  unsigned short *batteryLevel;
  unsigned short *fuelLevel;
} ;

struct TCB
{
  void *taskDataPtr;
  void (*taskPtr)(void*);

};

void consoleDisplayer(void* consoleStruct);
void powerSystem(void* powerStruct);
void thrusterSystem(void* thrusterStruct);
void warningSystem(void* alarmStruct);
void satelliteCommSystem(void* satelliteStruct);

unsigned int thrusterCommand = 0;
unsigned short batteryLevel = 100;
unsigned short fuelLevel = 45;
unsigned short powerConsumption = 0;
unsigned short powerGeneration = 0;
bool solarPanelState = false;
bool fuelLow = false;
bool batteryLow = false;


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

void setup() {

  Serial.begin(9600);
  #ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
  {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9328;
  
  }
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  //Serial.println("About to Begin");
  // define queue of TCB subtasks, as well as pointer to access TCB elements


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
  //queue[0] = &consoleDisplay;
  queue[0] = &satelliteCommunications;
  queue[1] = &thrusterSubsystem;
  queue[2] = &powerSubsystem;
  queue[3] = &consoleDisplay;
  queue[4] = &warningAlarm;

  // put your setup code here, to run once:

  // put your main code here, to run repeatedly:
  // consoleDisplayer(myDisplayData);
  aTCBPtr = queue[3];
  aTCBPtr->taskPtr( (aTCBPtr->taskDataPtr));
}
void loop() {
  //Serial.print("In void loop");
  time1 = millis()%5000;
    Serial.print("Time in milliseconds at start:  ");
    Serial.print(millis());
    Serial.print("\n");
  for (int i = 0; i < 5; i++) {
    aTCBPtr = queue[i];
    aTCBPtr->taskPtr( (aTCBPtr->taskDataPtr));
    Serial.print("Time in milliseconds:  ");
    Serial.print(millis());
    Serial.print("\n");
    time1 = millis()%5000;

   /* if (i == 4) {
      if (GC % MC == 0) {
        delay(5000);

      }
      GC++;

    }*/
  }
}
void consoleDisplayer(void * consoleStruct) {
  //Serial.print("consoleDisplayer\n");
  /*static unsigned long count = 0;

  if ((GC - count) % MC != 0 ) {
    return;
  }

  count = GC;*/
  //Serial.print("1");
  displayData *dData = (displayData*) consoleStruct;
  bool *solarPanelState = dData->solarPanelState;
  bool *fuelLow = dData->fuelLow;
  bool *batteryLow = dData->batteryLow;
  unsigned short *batteryLevel = dData->batteryLevel;
  unsigned short *fuelLevel = dData->fuelLevel;
  unsigned short *powerConsumption = dData->powerConsumption;
  unsigned short *powerGeneration = dData->powerGeneration;

  char *solarpanelPrint = (*solarPanelState) ? "Deployed" : "Retracted";
  char *fuelStatePrint = (*fuelLow) ? "YES" : "NO";
  char *batteryStatePrint = (*batteryLow) ? "YES" : "NO";

  static int currentDisplayCycles = 0;

  if (currentDisplayCycles % 4  == 0 || currentDisplayCycles % 4  == 1) {
  //if (toggleDisplay == 1){
    Serial.print("\nSATELLITE STATUS: \nSolar Panels:");
    Serial.print(*solarpanelPrint);
    Serial.print("\nBattery Level: ");
    Serial.print(*batteryLevel);
    Serial.print("\nFuel Level: ");
    Serial.print( *fuelLevel);
    Serial.print("\nPower Consumption: ");
    Serial.print(*powerConsumption);
    Serial.print("\nPower Generation: ");
    Serial.print(*powerGeneration);
    Serial.print("\n") ;
    printf("SATELLITE STATUS: \n"
           "Solar Panels: %9s\n "
           "Battery Level: %3hu\n "
           "Fuel Level: %3hu\n "
           "Power Consumption: %2hu\n "
           "Power Generation: %2hu\n",
           solarpanelPrint, *batteryLevel, *fuelLevel, *powerConsumption, *powerGeneration);
  } else {
    Serial.print("\nANNUNCIATION: \nBattery Low: ");
    Serial.print(batteryStatePrint);
    Serial.print("\nFuel Low: ");
    Serial.print(fuelStatePrint);
    Serial.print("\n");
    printf("ANNUNCIATION:\n "
           "Battery Low: %3s\n "
           "Fuel Low: %3s\n\n",
           batteryStatePrint, fuelStatePrint);
  }
  currentDisplayCycles ++;

}
void updatePowerConsumption(unsigned short *currentConsumptionLevel);
void updatePowerGeneration(bool *solarPanelState, unsigned short *currentGenerationLevel, unsigned short *batteryLevel);
void updateBatteryLevel(bool *solarPanelState, unsigned short *currentConsumptionLevel, unsigned short *currentGenerationLevel, unsigned short *batteryLevel);

void powerSystem (void* powerStruct) {
  // get data from pointers in data struct for use in method.
  //Serial.print("powerSystem\n");
  /*static unsigned long count = 0;

  if ((GC - count) % MC != 0 ) {
    return;
  }

  count = GC;
*/
  powerData *myPowerData = (powerData*)powerStruct;
  unsigned short *batteryLevel = myPowerData->batteryLevel;
  unsigned short *powerConsumptionLevel = myPowerData->powerConsumption;
  unsigned short *powerGenerationLevel = myPowerData->powerGeneration;
  bool *solarPanelState = myPowerData->solarPanelState;

  // set static variable to record what iteration cycle is occurring in the variable.

  updatePowerConsumption(powerConsumptionLevel);
  updatePowerGeneration(solarPanelState, powerGenerationLevel, batteryLevel);
  updateBatteryLevel(solarPanelState, powerConsumptionLevel, powerGenerationLevel, batteryLevel);

}

void updatePowerGeneration(bool *solarPanelState, unsigned short *currentGenerationLevel, unsigned short *batteryLevel) {
  //Serial.print("updatePowerGeneration\n");
  static int currentGenerationCycles = 0;

  if (*solarPanelState == true) {

    if (*batteryLevel > 95) {
      *solarPanelState = false;
      *currentGenerationLevel = 0;

    } else {
      if (*batteryLevel <= 50) {
        if (currentGenerationCycles % 2 == 0) {
          *currentGenerationLevel += 2;
        } else {
          *currentGenerationLevel += 1;
        }
      } else {
        if (*batteryLevel <= 95) {
          if (currentGenerationCycles % 2 == 0) {
            *currentGenerationLevel += 2;
          }
        }
      }
    }
  } else {
    if (*batteryLevel <= 10) {
      *solarPanelState = true;
    }


  }
  currentGenerationCycles++;
}

void updatePowerConsumption(unsigned short *currentConsumptionLevel) {
  // set static variable to record what iteration cycle is occurring in the variable.
 // Serial.print("updatePowerConsumption\n");
  static int currentConsumptionCycles = 0;
  static bool reverseDirection = false; // initial value to false of below5 will increment to ten, and
  if (!reverseDirection) {
    if (currentConsumptionCycles % 2 == 0) {
      *currentConsumptionLevel += 2;
    } else {
      *currentConsumptionLevel += 1;
    }
    if (*currentConsumptionLevel >= 10) {
      reverseDirection = true;
    }
  } else {
    if (currentConsumptionCycles % 2 == 0) {
      *currentConsumptionLevel -= 2;
    } else {
      *currentConsumptionLevel -= 1;
    }
    if (*currentConsumptionLevel <= 5) {
      reverseDirection = false;
    }
  }
  currentConsumptionCycles++;
  if (*currentConsumptionLevel > 10) {
    *currentConsumptionLevel = 10;
  }
}


void updateBatteryLevel(bool *solarPanelState, unsigned short *currentConsumptionLevel, unsigned short *currentGenerationLevel, unsigned short *batteryLevel) {
  //Serial.print("updateBatteryLevel\n");
  if (*solarPanelState == false) {
    *batteryLevel = *batteryLevel - (3 * (*currentConsumptionLevel));

  } else {
    *batteryLevel = *batteryLevel - *currentConsumptionLevel + *currentGenerationLevel;
  }

  if (*batteryLevel >= 100) {

    *batteryLevel = 100;
  }
}

int randomInteger(int low, int high);

int seed = 1;



int randomInteger(int low, int high)
{
  double randNum = 0.0;
  int multiplier = 2743;
  int addOn = 5923;
  double max = INT_MAX + 1.0;

  int retVal = 0;

  if (low > high)
    retVal = randomInteger(high, low);
  else
  {
    seed = seed * multiplier + addOn;
    randNum = seed;

    if (randNum < 0)
    {
      randNum = randNum + max;
    }

    randNum = randNum / max;

    retVal =  ((int)((high - low + 1) * randNum)) + low;
  }

  return retVal;
}

int randomInteger(int low, int high);

void satelliteCommSystem(void* satelliteStruct) {
  /*static unsigned long count = 0;
  Serial.print("satelliteCommSystem\n");
  if ((GC - count) % MC != 0 ) {
    return;
  }

  count = GC;*/
  satelliteData *mySatelliteData = (satelliteData*)satelliteStruct;
  unsigned short *currentBatteryLevel = mySatelliteData->batteryLevel;
  bool *batteryLowStatus = mySatelliteData->batteryLow;
  unsigned short *currentFuelLevel = mySatelliteData->fuelLevel;
  bool *fuelLowStatus = mySatelliteData->fuelLow;
  unsigned short *currentPowerConsumption = mySatelliteData->powerConsumption;
  unsigned short *currentPowerGeneration = mySatelliteData->powerGeneration;
  bool *currentSolarPanelState = mySatelliteData->solarPanelState;
  unsigned int *currentThrusterCommand = mySatelliteData->thrusterCommand;

  if(*currentFuelLevel <= 10) {
    *fuelLowStatus = true;
  } else {
    *fuelLowStatus = false;
  }

  if(*currentBatteryLevel <= 10) {
    *batteryLowStatus = true;
  } else {
    *batteryLowStatus = false;
  }
  

  *currentThrusterCommand = randomInteger(0, 65535) % 65535; // 65535 max value of 2^16
  uint16_t correctDirectionFactor = 0xFFF3; // equal to 1111111111110011
  *currentThrusterCommand &= correctDirectionFactor;

}


void thrusterSystem(void* thrusterStruct) {

  static unsigned long count = 0;
 // Serial.print("thrusterSystem\n");
  /*if ((GC - count) % MC != 0 ) {
    return;
  }

  count = GC;
*/
  thrusterData *tData = (thrusterData*) thrusterStruct;
  unsigned int *thrusterCommand = tData -> thrusterCommand;
  unsigned short *fuelLevel = tData -> fuelLevel;


  uint16_t Mask = 0xF;


  uint8_t direction = *thrusterCommand & Mask;
  uint8_t Magnitude = (*thrusterCommand >> 4) & Mask;
  uint8_t Duration = (*thrusterCommand >> 8) & Mask;

  double fuelCost;


  fuelCost = 0.00012845 * Magnitude * Duration;

  if (fuelLevel == 0) {
    printf("out of fuel");
    exit(0);
  } else {
    *fuelLevel =  *fuelLevel - (unsigned short) fuelCost;
  }
}

void updateWarningSystem(unsigned short *currBatteryLevel, unsigned short *currFuelLevel);
void ClearScreen();
void delay1(unsigned long delay1);

void warningSystem(void* alarmStruct) {
  alarmData *myAlarmData = (alarmData*)alarmStruct;
  bool *fuelLowIndicator = myAlarmData->fuelLow;
  bool *batteryLowIndicator = myAlarmData->batteryLow;
  unsigned short *currentBatteryLevel = myAlarmData->batteryLevel;
  unsigned short *currentFuelLevel = myAlarmData->fuelLevel;
  
  
  updateWarningSystem(currentBatteryLevel, currentFuelLevel);

}

void updateWarningSystem(unsigned short *currBatteryLevel, unsigned short *currFuelLevel) {
  tft.setRotation(1);
  tft.setTextSize(4);
  unsigned long time2;
  time2 = millis();
  time1 = millis() % 5000;
  int toggle = 1;
  while (time1 < 5001) {
    if (*currFuelLevel < 50) {
      toggle = - toggle;
      if (toggle == 1) {
        tft.setTextColor(BLACK);
      } else{
        if (*currFuelLevel < 10){
        tft.setTextColor(RED);
      } else {
        tft.setTextColor(ORANGE);
      }
    }
  }
  else {
        tft.setTextColor(GREEN);
  }
  tft.setCursor(0, 0);
    tft.print("FUEL");
    if (*currBatteryLevel < 50) {
      if (*currBatteryLevel < 10) {
        tft.setTextColor(RED);
      } else {
        tft.setTextColor(ORANGE);
      }
      tft.setCursor(130, 0);
      tft.print("BATTERY");
      delay(1000);
      tft.setCursor(130, 0);
      tft.setTextColor(BLACK);
      tft.print("BATTERY");
      delay(1000);

    } else {
      tft.setCursor(130, 0);
      tft.setTextColor(GREEN);
      tft.print("BATTERY");
      delay(2000);
    }

    time1 = millis() - time2;
  }
}



void ClearScreen() { /* method to simulate clearing screen */
  volatile int i; // initialize loop variable
  for (i = 0; i < 8; i++) { // run loop 8 times
    printf(" "); // print spaces to simulate clearing screen
  }
  printf("%c", 0x0d); // move cursor to beginning of console
}

void delay1(unsigned long delay1) { /*method to create time delay */
  volatile int i; // initialize loop variable
  for (i = 0; i < delay1;) { // simulate delay for loop
    i++;
  }
}
