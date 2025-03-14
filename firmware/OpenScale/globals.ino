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

void ClearAndSendChar(char charToSend)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();

  Serial.write(charToSend);
}

void ClearAndSendHandshakeChar(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();

  Serial.write("z");
}

void ClearAndSendDoneChar(void)
{
  // clear buffer just before prompting user for character entry
  ClearRxBuffer();

  Serial.write("d");
}