// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#define USING_USB
//#define USING_TTL

// extern int globalVar;  // Declaration (not definition)

void ClearRxBuffer(void);
char ClearAndReadChar(uint8_t delayAmount);
const char* ClearAndReadLine(void);
void ClearAndSendChar(char charToSend);
void ClearAndSendHandshakeChar(void);

#endif
