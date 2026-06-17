/*
  Lots of serial menus and visual stuff so user can configure the OpenScale
*/

#include "globals.h"

//We use this at startup and for the configuration menu
//Saves us a few dozen bytes
#ifndef USING_USB //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void DisplaySystemHeader(void)
{
  Serial.print(F("\r\nSerial Load Cell Converter version "));
  Serial.println(F(FIRMWARE_VERSION));
  Serial.println(F("By SparkFun Electronics"));

  //Look to see if we have an external or remote temp sensor attached
  if (remoteSensor.search(remoteSensorAddress) == 0)
  {
    remoteSensorAttached = false;
    Serial.println(F("No remote sensor found"));
  }
  else
  {
    remoteSensorAttached = true;
    Serial.println(F("Remote temperature sensor detected"));
  }
}
#endif //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef USING_USB
void InitializeSystem(void)
{
//Flow Rate Tester defaults found in "globals.h"
//reset uart speed to 115200
  setting_uart_speed = UART_SPEED;

  //reset units to grams
  setting_units = UNITS;

  //reset report rate to 200ms = 5Hz
  setting_report_rate = REPORT_RATE;

  //calibration factor not used in flow rate tester, but set to 1000 by default
  setting_calibration_factor = CALIBRATION_FACTOR;

  //reset tare point to 1000
  setting_tare_point = TARE_POINT;

  //reset timestamp to true
  setting_timestamp_enable = TIMESTAMP_ENABLE;

  //reset decimal places to 4
  setting_decimal_places = DECIMAL_PLACES;

  //reset average amount to 63
  setting_average_amount = AVERAGE_AMOUNT;

  //local temperature not used in flow rate tester
  setting_local_temp_enable = LOCAL_TEMP_ENABLE;

  //remote temperature not used in flow rate tester
  setting_remote_temp_enable = REMOTE_TEMP_ENABLE;

  //reset LED blinking to true
  setting_status_enable = STATUS_ENABLE;

  //reset serial trigger to true
  setting_serial_trigger_enable = SERIAL_TRIGGER_ENABLE; 

  //reset raw reading to false
  setting_raw_reading_enable = RAW_READING_ENABLE;

  //reset trigger character to 'm'
  setting_trigger_character = TRIGGER_CHARACTER;

  record_system_settings(); //Commit these new settings to memory
  Serial.flush();
}
#endif

//How openscale opperates
void system_setup(void)
{
  #ifdef USING_USB

  while (1)
  {
    DisplaySystemHeader(); //Product title and firmware version

    Serial.println(F("System Configuration"));

    Serial.print(F("1) Tare scale to zero ["));
    Serial.print(setting_tare_point);
    Serial.println(F("]"));

    //NO CALIBRATION IN FLOW RATE TESTER (ALREADY SET)
    Serial.print(F("2) Calibrate scale ["));
    Serial.print(setting_calibration_factor);
    Serial.println(F("]"));

    Serial.print(F("3) Timestamp [O"));
    if (setting_timestamp_enable == true) Serial.print(F("n"));
    else Serial.print(F("ff"));
    Serial.println(F("]"));

    Serial.print(F("4) Set report rate ["));
    Serial.print(setting_report_rate);
    Serial.println(F("]"));

    Serial.print(F("5) Set baud rate ["));
    Serial.print(setting_uart_speed);
    Serial.println(F(" bps]"));

    //might need to change const UNITS to UNITS_G in globals.h for this to work
    Serial.print(F("6) Change units of measure ["));
    if (setting_units == UNITS_KG) Serial.print(F("kg"));
    if (setting_units == UNITS_G) Serial.print(F("g"));
    Serial.println(F("]"));

    Serial.print(F("7) Decimals ["));
    Serial.print(setting_decimal_places);
    Serial.println(F("]"));

    Serial.print(F("8) Average amount ["));
    Serial.print(setting_average_amount);
    Serial.println(F("]"));

    Serial.print(F("9) Local temp [O"));
    if (setting_local_temp_enable == true) Serial.print(F("n"));
    else Serial.print(F("ff"));
    Serial.println(F("]"));

    Serial.print(F("r) Remote temp [O"));
    if (setting_remote_temp_enable == true) Serial.print(F("n"));
    else Serial.print(F("ff"));
    Serial.println(F("]"));

    Serial.print(F("s) Status LED ["));
    if (setting_status_enable == true) Serial.print(F("Blink"));
    else Serial.print(F("Off"));
    Serial.println(F("]"));

    Serial.print(F("q) Raw reading [O"));
    if (setting_raw_reading_enable == true) Serial.print(F("n"));
    else Serial.print(F("ff"));
    Serial.println(F("]"));

    Serial.print(F("t) Serial trigger [O"));
    if (setting_serial_trigger_enable == true) Serial.print(F("n"));
    else Serial.print(F("ff"));
    Serial.println(F("]"));

    Serial.print(F("c) Trigger character: ["));
    Serial.print(setting_trigger_character);
    Serial.print(F(" / '"));
    Serial.write(setting_trigger_character);
    Serial.println(F("']"));


    Serial.println(F("x) Exit"));

    //clear the buffer of any stray characters before character entry
    ClearRxBuffer();

    Serial.print(F(">"));

    //Read command
    while (!Serial.available()) ; //Wait for user to type a character
    char command = Serial.read();

    toggleLED();

    //Execute command
    if (command == '1')
    {
     tare_scale(false); // normal tare operation, not during calibration
    }
    else if (command == '2')
    {
      calibrate_scale();
    }
    else if (command == '3')
    {
      Serial.print(F("\n\rToggling Timestamp ... "));
      toggle_timestamp();
    }
    else if (command == '4')
    {
      rate_setup();
    }
    else if (command == '5')
    {
      baud_setup();
    }
    else if (command == '6')
    {
      Serial.print(F("\n\rUnits set to ... "));
      toggle_units();
    }
    else if (command == '7')
    {
      decimal_setup();
    }
    else if (command == '8')
    {
      average_reading_setup();
    }
    else if (command == '9')
    {
      Serial.print(F("\n\rLocal temp o"));
      local_temp();
    }
    else if (command == 'r')
    {
      Serial.print(F("\n\rRemote temp o"));
      remote_temp();
    }
    else if (command == 's')
    {
      Serial.print(F("\n\rStatus LED ..."));
      status_LED();
    }
    else if (command == 't')
    {
      Serial.print(F("\n\rSerial trigger ..."));
      serial_trigger();
    }
    else if (command == 'q')
    {
      Serial.print(F("\n\rRaw reading ..."));
      raw_reading();
    }
    else if (command == 'c')
    {
      ClearRxBuffer();
      Serial.print(F("\n\rEnter new trigger character: "));

      while (Serial.available() == false) delay(1);

      setting_trigger_character = Serial.read();

      Serial.println();
      Serial.print(F("\n\rNew character: "));
      Serial.print(setting_trigger_character);
    }
    else if (command == 'x')
    {
      //Do nothing, just exit
      Serial.println(F("Exiting"));
      exit_menu();
      return;
    }

  }
}

-------------------------------------------------------------------------------------------------------------------
// "1" TARE SCLAE
void tare_scale(boolean calibrating)
{
  #ifdef USING_USB // 
  
  if (calibrating)
  {
    Serial.print(F("\n\rGetting Tare point (calibrating): "));
  }
  else
  {    
    Serial.print(F("\n\rGetting Tare point: "));
  }

  scale.tare(); //Reset the scale to 0
  setting_tare_point = scale.read_average(10); //Get 10 readings from the HX711 and average them
  Serial.print(F("Tare: "));
  Serial.println(setting_tare_point);

  #else // 
  // Not using the USB

  scale.tare(); //Reset the scale to 0
  setting_tare_point = scale.read_average(10); //Get 10 readings from the HX711 and average them

  if (calibrating)
  {
    ClearandSendHandshaketoPi();
  }
  else
  {
    ClearandSendFinished();
  }

  #endif // END OF TARE SCALE
}

-------------------------------------------------------------------------------------------------------------------
// "2" CALIBRATE SCALE
void calibrate_scale(void)
{
  #ifdef USING_USB // 

  Serial.println(); // Two blanks lines before calibration menu
  Serial.println();
  Serial.println(F("Scale calibration"));

  Serial.println(F("Remove all weight from the scale. Press 'c' when scale is clear and stable."));

  //clear and tare the scale before calibrating
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == false) ; //Wait for user to press "c""
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

  tare_scale(false); //even though we are calibrating, we only need to let tare know if using TTL not USB

   Serial.println(F("Place known weight on scale. Press 'r' and hit enter when weight is in place and stable."));

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == false) ; //Wait for user to press key
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
   
  long rawReading = scale.read_average(setting_average_amount); //Take average reading 63 times (AVERAGE_AMOUNT)
  Serial.print(F("Raw: "));
  Serial.println(rawReading);

  Serial.print(F("Current Reading: "));
  Serial.print(scale.get_units(setting_average_amount), 4); //Show 4 decimals during calibration
  if (setting_units == UNITS_G) Serial.print(F("g")); // Again may need to set UNITS to UNITS_G in globals.h for this to work
  if (setting_units == UNITS_KG) Serial.print(F("kg"));
  Serial.println();

  Serial.print(F("Calibration Factor: "));
  Serial.print(setting_calibration_factor);
  Serial.println();

  while (Serial.available()) Serial.read(); //clear again

  Serial.print(F("Please enter the weight currently sitting on the scale: "));

    //Read user input
  char newSetting[15]; //Max 15 characters: "12.5765" = 8 characters (includes trailing /0)
  read_line(newSetting, sizeof(newSetting));

  float weightOnScale = atof(newSetting); //Convert this string to a float
  Serial.println();

  Serial.print(F("User entered: "));
  Serial.println(weightOnScale, 4);

  //Convert this weight to a calibration factor

  //tare: 210193
  //raw: 246177
  //User Input: 0.5276 kg
  //avg: 4 times

  //get_units = (raw-OFFSET) / calibration_factor
  //0.5276 = (246177-210193) / cal_factor
  //114185 / .45 = 256744

  setting_calibration_factor = (rawReading - setting_tare_point) / weightOnScale;

  Serial.print(F("New Calibration Factor: "));
  Serial.print(setting_calibration_factor);
  Serial.println();

  scale.set_scale(setting_calibration_factor); //Go to this new cal factor

  Serial.print(F("New Scale Reading: "));
  Serial.print(scale.get_units(setting_average_amount), 4); //Show 4 decimals during calibration
  Serial.print(F(" "));
  if (setting_units == UNITS_G) Serial.print(F("g"));
  if (setting_units == UNITS_KG) Serial.print(F("kg"));
  Serial.println();

  #else // 

  const long interval = 30000; //30 seconds to calibrate before we reset back to defaults
  unsigned long previousMillis = 0; //Start counting from now (initial time)
  unsigned long currentMillis = millis(); // Get the current time from initial time

  if (currentMillis - previousMillis >= interval) // interval is less than or equal to 30 seconds
  {
    previousMillis = currentMillis; //Reset the timer
  }
  ClearandReadCharacter(0); //user indicating scale is ready for tare
  TareScale(true);  //passing true send handshake char instead of done char, only for ttl
  ClearAndReadCharBlocking(0);  //user indicating calibration weight is on scale
  long rawReading = scale.read_average(setting_average_amount); //Take average reading over a given number of times

  //float weightOnScale = atof(ClearAndReadLine(15)); //Convert this string to a float
  float weightOnScale = 0.5;

  //convert this weight to a calibration factor
  setting_calibration_factor = (rawReading - setting_tare_point) / weightOnScale

  // float newFactor = (float)setting_calibration_factor * 1000; //Convert the calibration factor from g to kg
  // setting_calibration_factor = (long)newFactor;
  scale.set_scale(setting_calibration_factor); //Go to this new cal factor

  ClearAndSendFinished(); //Tell user we are done calibrating

    #endif // END OF CALIBRATION 
}

---------------------------------------------------------------------------------------------------------------------------
// "3" TOGGLE TIMESTAMP
void toggle_timestamp(void)
{
  #ifdef USING_USB

  if (setting_timestamp_enable == true) // if it is on "true" turn it off "false"
  {
    setting_timestamp_enable = false;
  }
  else
  {
    setting_timestamp_enable = true; // if it is off "false" turn it on "true"
  }

  #else // 

  setting_timestamp_enable = TIMESTAMP_ENABLE; //Reset to default value
  ClearAndSendFinished(); //Tell user we are done

  #endif  //END OF TOGGLE TIMESTAMP 
}

--------------------------------------------------------------------------------------------------------------------------
// "4" SET REPORT RATE
void rate_setup(void)
{
  #ifdef USING_USB 

  //Calculate the minimum time between reports
  unsigned int minTime = CalcMinimumReadTime();

  Serial.println(F("\n\n\rSet time between reports"));

  Serial.print(F("Minimum: "));
  Serial.print(minTime);
  Serial.println(F("ms"));

  if (setting_report_rate < minTime) setting_report_rate = minTime;

  Serial.print(F("Current Time: "));
  Serial.print(setting_report_rate);
  Serial.println(F("ms"));

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

  //Read user input
  Serial.print(F("Enter new time (ms): "));
  char newSetting[8]; //Max at 1000000 = 1000 seconds
  read_line(newSetting, sizeof(newSetting));

  unsigned int newReportRate = strtolong(newSetting); //Convert this string to an int

  //Error check
  if (newReportRate > minTime)
  {
    setting_report_rate = newReportRate; //Go to this new time

    //Record this new value to EEPROM
    record_system_settings();

    Serial.print(F("Time between reports now: "));
    Serial.print(setting_report_rate);
    Serial.println(F("ms"));
  }
  else
  {
    Serial.println(F("Error: Out of bounds"));
  }

  #else 

   //Calculate the minimum time between reports
  unsigned int minTime = CalcMinReadTime();

  setting_report_rate = minTime + 1;

  ClearAndSendDone();

  #endif // END OF REPORT RATE
}

-------------------------------------------------------------------------------------------------------------------------
//"5" SET BAUD RATE
//Configure what baud rate to communicate at
void baud_setup(void)
{
  #ifdef USING_USB

  Serial.print(F("\n\n\rCurrent rate: "));
  Serial.print(setting_uart_speed, DEC);
  Serial.println(F(" bps"));

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  Serial.println(F("Enter new baud rate ('m' to abort):"));

  //Print prompt
  Serial.print(F(">"));

  //Read user input
  char newBaud[8]; //Max at 1000000
  read_line(newBaud, sizeof(newBaud));

  //Look for escape character
  if (newBaud[0] == 'x')
  {
    Serial.println(F("Exiting"));
    return;
  }

  long newRate = strtolong(newBaud); //Convert this string to a long

  if (newRate < BAUD_MIN || newRate > BAUD_MAX)
  {
    Serial.println(F("Out of bounds"));
  }
  else
  {
    Serial.print(F("Going to "));
    Serial.print(newRate);
    Serial.println(F("bps"));

    //Record this new baud rate
    setting_uart_speed = newRate;
    record_system_settings();

    //Go to this speed
    Serial.end();
    Serial.begin(setting_uart_speed);
  }

  #else

  // long newRate = strtolong(ClearAndReadLine(8)); //Convert this string to a long

  // if (newRate < BAUD_MIN)
  // {
  //   newRate = 9600;
  // }
  // else if (newRate > BAUD_MAX)
  // {
  //   newRate = 115200;
  // }

  //making this simply toggle now to force 115200 baud rate
  //115200 bps is what we will always be using on the MDU

  //Record this new baud rate
  setting_uart_speed = UART_SPEED;

  //Go to this speed
  Serial.end();
  Serial.begin(setting_uart_speed);

  ClearAndSendFinished();

  #endif //END OF BAUD RATE
}

-----------------------------------------------------------------------------------------------------------------
//"6" CHANGE UNITS
void toggle_units(void)
{
  #ifdef USING_USB

  if (setting_units == UNITS_KG) // if it is kg change to g
  {
    setting_units = UNITS_G;
    float newFactor = (float)setting_calibration_factor * 0.001000; //Convert the calibration factor from kg to g
    setting_calibration_factor = (long)newFactor;
  }
  else if (setting_units == UNITS_G) // if it is g change to kg
  {
    setting_units = UNITS_KG;
    float newFactor = (float)setting_calibration_factor * 1000.000; //Convert the calibration factor from g to kg
    setting_calibration_factor = (long)newFactor;
  }
  scale.set_scale(setting_calibration_factor); //assign this new unit to the scale

  #else

  // setting_units = UNITS;
  // float newFactor = (float)setting_calibration_factor * 2.20462; //Convert the calibration factor from lbs to kg
  // setting_calibration_factor = (long)newFactor;
  // scale.set_scale(setting_calibration_factor); //Assign this new factor to the scale>
  ClearAndSendFinished();

  #endif // END OF TOGGLE UNITS
}

----------------------------------------------------------------------------------------------------------------------------------
// "7" DECIMAL PLACES
//Configure how many decimals to show
void decimal_setup(void)
{
  #ifdef USING_USB

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

  //Get user input
  Serial.print(F("\n\n\rEnter the number of decimals to display (0 to 4): "));
  char newSetting[8]; //Max 7 characters
  read_line(newSetting, sizeof(newSetting));

  int newDecimalPlaces = strtolong(newSetting); //Convert this string to an int

  //Error check
  if (newDecimalPlaces >= 0 && newDecimalPlaces <= 4)
  {
    setting_decimal_places = newDecimalPlaces;

    //Record this new value to EEPROM
    record_system_settings();

    Serial.print(F("Decimal places: "));
    Serial.println(setting_decimal_places);
  }
  else
  {
    Serial.println(F("Error: Out of bounds"));
  }

  #else

   // int newDecimalPlaces = strtolong(ClearAndReadLine(8)); //Convert this string to an int

  // if (newDecimalPlaces < 0)
  // {
  //   newDecimalPlaces = 0;
  // }
  // else if (newDecimalPlaces > 4)
  // {
  //   newDecimalPlaces = 4;
  // }

  setting_decimal_places = DECIMAL_PLACES;
  ClearAndSendDone();

  #endif // END OF DECIMAL PLACES
}

-----------------------------------------------------------------------------------------------------------------------------------
//"8" AVERAGE AMOUNT
//Configure how many readings to average together
void average_reading_setup(void)
{
  #ifdef USING_USB

  Serial.print(F("\n\n\rEnter the number of readings to average together (1 to 64): "));

  //Get user input
  int newAverageAmount = strtolong(ClearAndReadLine(8)); //Convert this string to an int

  if (newAverageAmount < 1)
  {
    newAverageAmount = 1;
  }
  
    setting_average_amount = newAverageAmount;

    #else

    setting_average_amount = AVERAGE_AMOUNT;
    ClearAndSendFinished();
    
    #endif // END OF AVERAGE AMOUNT
}


----------------------------------------------------------------------------------------------------------------------
//"9" LOCAL TEMPERATURE
void local_temp(void)
{
  #ifdef USING_USB

  if (setting_local_temp_enable == true) // if it is on "true" turn it off "false"
  {
    Serial.println(F("ff"));
    setting_local_temp_enable = false;
  }
  else
  {
    Serial.println(F("n"));
    setting_local_temp_enable = true; // if it is off "false" turn it on "true"
  }

  #else

  setting_local_temp_enable = LOCAL_TEMP_ENABLE; //Reset to default value
  ClearAndSendFinished(); //Tell user we are done

  #endif // END OF LOCAL TEMPERATURE
}

----------------------------------------------------------------------------------------------------------------------
//"r" REMOTE TEMPERATURE
void remote_temp(void)
{
  #ifdef USING_USB

  if (setting_remote_temp_enable == true) // if it is on "true" turn it off "false"
  {
    Serial.println(F("ff"));
    setting_remote_temp_enable = false;
  }
  else
  {
    Serial.println(F("n"));
    setting_remote_temp_enable = true; // if it is off "false" turn it on "true"
  }

  #else

  setting_remote_temp_enable = REMOTE_TEMP_ENABLE; //Reset to default value
  ClearAndSendFinished(); //Tell user we are done

  #endif // END OF REMOTE TEMPERATURE
}

-----------------------------------------------------------------------------------------------------------------------
//"s" STATUS LED
void status_LED(void)
{
  #ifdef USING_USB

  if (setting_status_enable == true) // if it is on "true" turn it off "false"
  {
    Serial.println(F("Off"));
    setting_status_enable = false;
    digitalWrite(statusLED, LOW); // Turn off the LED right away if we are turning it off, don't wait for the next reading to do it
  }
  else
  {
    Serial.println(F("Blink"));
    setting_status_enable = true; // if it is off "false" turn it on "true"
  }

  #else

  setting_status_enable = STATUS_ENABLE; //Reset to default value
  ClearAndSendFinished(); //Tell user we are done

  #endif // END OF STATUS LED
}
-----------------------------------------------------------------------------------------------------------------------------------
//"t" SERIAL TRIGGER
void serial_trigger(void)
{
  #ifdef USING_USB

  if (setting_serial_trigger_enable == true) // if it is on "true" turn it off "false"
  {
    Serial.println(F("Off"));
    setting_serial_trigger_enable = false;
  }
  else
  {
    Serial.println(F("On"));
    setting_serial_trigger_enable = true; // if it is off "false" turn it on "true"
  }

  #else

  setting_serial_trigger_enable = SERIAL_TRIGGER_ENABLE; //Reset to default value
  ClearAndSendFinished(); //Tell user we are done

  #endif // END OF SERIAL TRIGGER
}
------------------------------------------------------------------------------------------------------------------------------------
//"q" RAW READING
void raw_reading(void)
{
  #ifdef USING_USB

  if(setting_raw_reading_enable == true)
  {
    setting_raw_reading_enable = false;
  }
  else
  {
    setting_raw_reading_enable = true;
  }

  #else

  setting_raw_reading_enable = RAW_READING_ENABLE;
  ClearAndSendFinished();

  #endif // END OF RAW READING
}

----------------------------------------------------------------------------------------------------------------------------------------------





// CURRENT READ TIME CALCULATION
//Determine how much time we need between measurements
//Takes into account current baud rate
//Takes into account the time to read various sensors
//Takes into account raw reading printing
unsigned int CalcMinimumReadTime(void)
{
  //The first few reads take too little time
  scale.get_units();
  scale.get_units();

  //Establish out much time it takes to do a standard scale read
  unsigned long startTime = millis();
  scale.get_units(setting_average_amount); //Do a dummy read and time it
  unsigned int averageReadTime = ceil((millis() - startTime));
  unsigned int sensorReadTime = averageReadTime;

  //Assume we will need to print a minimum of 7 characters at this baud rate per loop
  //1 / 9600 = 1ms * 10bits per byte = 9.6ms per byte
  float characterTime = 1 / (float)setting_uart_speed;

  #ifdef USING USB
  Serial.println(characterTime);
  #endif

  //Calculate number of characters per report
  int characters = 0;

  if (setting_timestamp_enable == true) characters += strlen("51588595,"); //Timestamp has characters

  if (setting_local_temp_enable)
  {
    //I don't need this
    unsigned long startTime = millis();
    for (byte x = 0 ; x < 8 ; x++)
      getLocalTemperature(); //Do a dummy read and time it 
    averageReadTime = ceil((millis() - startTime) / (float)8);
    sensorReadTime += averageReadTime; //In ms

    characters += strlen("24.75,"); //Add the time it takes to print the characters as well
  }

  if (setting_remote_temp_enable)
  {
    //I don't need this either
    unsigned long startTime = millis();
    for (byte x = 0 ; x < 8 ; x++)
      getRemoteTemperature(); //Do a dummy read and time it
    averageReadTime = ceil((millis() - startTime) / (float)8);
    sensorReadTime += averageReadTime; //In ms

    characters += strlen("27.81,"); //Add the time it takes to print the characters as well
  }

  characters += strlen("123,"); //Basic weight without decimals

  if (setting_decimal_places > 0) characters += setting_decimal_places + 1; //For example 4: 3 decimal places and the '.'

  if (setting_units == UNITS_G) characters += strlen("g");
  if (setting_units == UNITS_KG) characters += strlen("kg");

  if (setting_raw_reading_enable == true)
  {
    //Establish how much time it takes to do a raw read
    long startTime = millis();
    scale.read_average(setting_average_amount); //Do a dummy read and time it
    averageReadTime = ceil((millis() - startTime));
    sensorReadTime += averageReadTime; //In ms

    characters += strlen("8355808");
  }

  #ifdef USING_USB 
  Serial.print("characterTime: ");
  Serial.println(ceil((float)characters * characterTime));

  //Combine the total amount of sensor read time with the time it takes to print all the characters
  Serial.println(characters);
  Serial.println(sensorReadTime);
  #endif

  return (sensorReadTime + ceil((float)characters * characterTime));
}
