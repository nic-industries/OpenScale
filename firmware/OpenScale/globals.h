// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#define USING_USB
//#define USING_TTL

// global defaults for Flow Rate Tester OpenScale
const long UART_SPEED = 115200;
const byte UNITS = 1; //0 for kg, 1 for g
const unsigned int REPORT_RATE = 200; // How often to display the menus settings in ms
const long CALIBRATION_FACTOR = 0; //Value used to convert the load cell reading to g or kg. try 1000 if 0 does not work
const long TARE_POINT = 0;
const uint8_t TIMESTAMP_ENABLE = true; //Prints the number of miliseconds since boot next to weight reading
const byte DECIMAL_PLACES = 4; //How many decimals to show with weight
const byte LOCAL_TEMP_ENABLE = false; //Prints the local temperature in C
const byte REMOTE_TEMP_ENABLE = false; //Prints the remote temperature in C
const byte AVERAGE_AMOUNT = 5;
const byte TRIGGER_CHARACTER = 'w';
const boolean SETUP_MODE = false;
const byte STATUS_ENABLE = true;
const byte SERIAL_TRIGGER_ENABLE = false;
const byte RAW_READING_ENABLE = false; //Prints the raw, 24bit, long from the HX711, ex: 8355808
const byte ESCAPE_CHAR = 'm';
const int MIN_POWERCYCLE_TIME = 500;
const byte STATUS_LED = 13;  //Flashes with each reading
unsigned long bootTime = 0;

// global methods

void ClearRXBuffer (void);
char ClearAndReadChar(uint8_t delayAmount);
const char ClearAndReadLine(void);
void ClearAndSendChar(char charToSend);
void ClearandSendHandshaketoPi(void);


#endif