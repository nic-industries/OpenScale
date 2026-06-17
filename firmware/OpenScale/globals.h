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
const long TARE_POINT = 1000;
const uint8_t TIMESTAMP_ENABLE = true; //Prints the number of miliseconds since boot next to weight reading
const byte DECIMAL_PLACES = 4; //How many decimals to show with weight
const byte LOCAL_TEMP_ENABLE = false; //Prints the local temperature in C
const byte REMOTE_TEMP_ENABLE = false; //Prints the remote temperature in C
const byte AVERAGE_AMOUNT = 63;
const byte TRIGGER_CHARACTER = 'm';
const byte STATUS_ENABLE = true;
const byte SERIAL_TRIGGER_ENABLE = true;
const byte RAW_READING_ENABLE = false; //Prints the raw, 24bit, long from the HX711, ex: 8355808
const byte ESCAPE_CHAR = 'x';
const int MIN_POWERCYCLE_TIME = 500;
const byte STATUS_LED = 13;  //Flashes with each reading

// global methods



#endif