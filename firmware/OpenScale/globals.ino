#include "globals.h"

void ClearRxBuffer(void)
{
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
}

char ClearAndReadChar(uint8_t delayAmount)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();

  //Read command
  while (!Serial.available())
  {
    //Wait for user to type a character
    if (delayAmount > 0)
    {
      delay(delayAmount);
    }
  }

  return Serial.read();
}

const char* ClearAndReadLine(const uint8_t bufferSize)
{
  ClearRxBuffer();
  //Read user input
  //Max 15 characters for calibrate: "12.5765" = 8 characters (includes trailing /0)
  //Max 8 characters for baud rate
  if (bufferSize < 8 || bufferSize > 15)
  {
    return "0";
  }
  else
  {
    static char newSetting[15];
    read_line(newSetting, sizeof(newSetting));

    return newSetting;
  }
}

void ClearAndSendChar(char charToSend)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  
  Serial.write(charToSend);
}

void ClearAndSendHandshake(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  
  Serial.write("z\n");
}

void ClearAndSendDone(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  
  Serial.write("d\n");
}