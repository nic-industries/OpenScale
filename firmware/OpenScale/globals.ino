#include "globals.h"

void ClearRxBuffer(void)
{
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
}

char ClearAndReadCharBlocking(uint8_t delayAmount)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  Serial.flush();

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

char ClearAndReadCharNonBlocking()
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  Serial.flush();
  char returnChar = '\0';

  if (Serial.available() > 0)
  {
    returnChar = Serial.read();
  }

  return returnChar;
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

void ClearAndSendMenuStart(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  delay(100);
  Serial.write("m\n");
}

void ClearAndSendMenuEnd(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  delay(100);
  Serial.write("n\n");
}

void ClearAndSendDone(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  delay(100);
  Serial.write("d\n");
}

void ClearAndSendHandshake(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();
  delay(100);
  Serial.write("z\n");
}