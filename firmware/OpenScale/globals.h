// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

// #define USING_USB
#define USING_TTL

// extern int globalVar;  // Declaration (not definition)

// global defaults for MDU
const long UART_SPEED = 115200; 
const byte UNITS = 0; //0 = kilograms
const unsigned int REPORT_RATE = 200;
const long CALIBRATION_FACTOR = 0; 
const long TARE_POINT = 1000; 
const uint8_t TIMESTAMP_ENABLE = false; 
const byte DECIMAL_PLACES = 4; 
const byte AVERAGE_AMOUNT = 5; 
const byte LOCAL_TEMP_ENABLE = false; 
const byte REMOTE_TEMP_ENABLE = false; 
const byte STATUS_ENABLE = true; 
const byte SERIAL_TRIGGER_ENABLE = true; 
const byte RAW_READING_ENABLE = false; 
const byte TRIGGER_CHAR = '!'; 
const boolean SETUP_MODE = false; 
const byte ESCAPE_CHAR = 'x'; 
const int MIN_POWERCYCLE_TIME = 500; 
const byte STATUS_LED = 13;  

// global methods
void ClearRxBuffer(void);
char ClearAndReadChar(uint8_t delayAmount);
const char* ClearAndReadLine(void);
void ClearAndSendChar(char charToSend);
void ClearAndSendHandshake(void);

#endif
