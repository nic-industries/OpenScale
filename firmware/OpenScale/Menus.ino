/*
  Lots of serial menus and visual stuff so user can configure the OpenScale
*/

#include "globals.h"

//We use this at startup and for the configuration menu
//Saves us a few dozen bytes
#ifdef USING_USB
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
#endif

//Configure how OpenScale operates
#ifdef USING_USB
void SystemSetup(void)
{
  while (1)
  {
    DisplaySystemHeader(); //Product title and firmware version

    Serial.println(F("System Configuration"));

    Serial.print(F("1) Tare scale to zero ["));
    Serial.print(setting_tare_point);
    Serial.println(F("]"));

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

    Serial.print(F("6) Change units of measure ["));
    if (setting_units == UNITS_KG) Serial.print(F("kg"));
    if (setting_units == UNITS_LBS) Serial.print(F("lbs"));
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

    // clear buffer just before prompting user for character entry
    ClearRxBuffer();

    Serial.print(F(">"));

    //Read command
    while (!Serial.available()) ; //Wait for user to type a character
    char command = Serial.read();

    toggleLED();

    //Execute command
    if (command == '1')
    {
      TareScale();
    }
    else if (command == '2')
    {
      CalibrateScale();
    }
    else if (command == '3')
    {
      Serial.print(F("\n\rToggling Timestamp..."));
      ToggleTimestamp();
    }
    else if (command == '4')
    {
      RateSetup();
    }
    else if (command == '5')
    {
      BaudSetup();
    }
    else if (command == '6')
    {
      Serial.print(F("\n\rToggling Units..."));
      ToggleUnits();
    }
    else if (command == '7')
    {
      DecmialSetup();
    }
    else if (command == '8')
    {
      AverageReadingSetup();
    }
    else if (command == '9')
    {
      Serial.print(F("\n\rToggling Local Temp..."));
      ToggleLocalTemp();
    }
    else if (command == 'r')
    {
      Serial.print(F("\n\rToggling Remote Temp..."));
      ToggleRemoteTemp();
    }
    else if (command == 's')
    {
      Serial.print(F("\n\rToggling Status LED..."));
      ToggleStatusLED();
    }
    else if (command == 't')
    {
      Serial.print(F("\n\rToggling Serial Trigger..."));
      ToggleSerialTrigger();
    }
    else if (command == 'q')
    {
      Serial.print(F("\n\rToggling Raw Reading..."));
      ToggleRawReading();
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
      Serial.println(F("Exiting"));
      ExitMenu();
      return;
    }
  }
}
#else
void SystemSetup(void)
{
  ClearAndSendHandshake();
  char command = ClearAndReadChar(0);

  if (command == '1')
  {
    TareScale(0);
  }
  else if (command == '2')
  {
    CalibrateScale();
  }
  else if (command == '3')
  {
    ToggleTimestamp();
  }
  else if (command == '4')
  {
    RateSetup();
  }
  else if (command == '5')
  {
    BaudSetup();
  }
  else if (command == '6')
  {
    ToggleUnits();
  }
  else if (command == '7')
  {
    DecmialSetup();
  }
  else if (command == '8')
  {
    AverageReadingSetup();
  }
  else if (command == '9')
  {
    ToggleLocalTemp();
  }
  else if (command == 'r')
  {
    ToggleRemoteTemp();
  }
  else if (command == 's')
  {
    ToggleStatusLED();
  }
  else if (command == 't')
  {
    ToggleSerialTrigger();
  }
  else if (command == 'q')
  {
    ToggleRawReading();
  }
  else if (command == 'c')
  {
    SetTriggerCharacter();
  }
  else if (command == 'x')
  {
    ExitMenu();
  }
}
#endif

// places tare functionality into a method
#ifdef USING_USB
void TareScale(void)
{
  Serial.print(F("\n\rGetting Tare point: "));
  scale.tare(); //Reset the scale to 0
  setting_tare_point = scale.read_average(10); //Get 10 readings from the HX711 and average them
  Serial.print(F("Tare: "));
  Serial.println(setting_tare_point);
}
#else
void TareScale(bool calibrating)
{
  if (!calibrating)
  {
    ClearAndSendHandshake();
  }
  
  scale.tare(); //Reset the scale to 0
  setting_tare_point = scale.read_average(10); //Get 10 readings from the HX711 and average them

  if (!calibrating)
  {
    ClearAndSendDone();
  }
}
#endif

//Gives user the ability to set a known weight on the scale and calculate a calibration factor
#ifdef USING_USB
void CalibrateScale(void)
{
  Serial.println();
  Serial.println();
  Serial.println(F("Scale calibration"));

  Serial.println(F("Remove all non-permanent objects from scale. Press a key and hit enter when all objects have been removed."));

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == false) ; //Wait for user to press key
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

  TareScale();

  Serial.println(F("Place known weight on scale. Press a key and hit enter when weight is in place and stable."));

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == false) ; //Wait for user to press key
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

  long rawReading = scale.read_average(setting_average_amount); //Take average reading over a given number of times
  Serial.print(F("Raw: "));
  Serial.println(rawReading);

  Serial.print(F("Current Reading: "));
  Serial.print(scale.get_units(setting_average_amount), 4); //Show 4 decimals during calibration
  if (setting_units == UNITS_LBS) Serial.print(F("lbs"));
  if (setting_units == UNITS_KG) Serial.print(F("kg"));
  Serial.println();

  Serial.print(F("Calibration Factor: "));
  Serial.print(setting_calibration_factor);
  Serial.println();

  // just in case a button was pressed
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

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
  if (setting_units == UNITS_LBS) Serial.print(F("lbs"));
  if (setting_units == UNITS_KG) Serial.print(F("kg"));
  Serial.println();
}
#else
void CalibrateScale(void)
{
  ClearAndSendHandshake();
  ClearAndReadChar(0);  //user indicating scale is ready for tare
  TareScale(1);
  ClearAndSendHandshake(); 
  ClearAndReadChar(0);  //user indicating calibration weight is on scale
  long rawReading = scale.read_average(setting_average_amount); //Take average reading over a given number of times
  ClearAndSendHandshake(); 

  float weightOnScale = atof(ClearAndReadLine(15)); //Convert this string to a float

  //Convert this weight to a calibration factor
  setting_calibration_factor = (rawReading - setting_tare_point) / weightOnScale;

  scale.set_scale(setting_calibration_factor); //Go to this new cal factor

  ClearAndSendDone();
}
#endif

void ToggleTimestamp(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_timestamp_enable == true)
  {
    setting_timestamp_enable = false;
  }
  else
  {
    setting_timestamp_enable = true;
  }

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

#ifdef USING_USB
//Allow user to input the time between readings
void RateSetup(void)
{
  //Calculate the minimum time between reports
  unsigned int minTime = CalcMinReadTime();

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
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  char newSetting[8]; //Max at 1000000 = 1000 seconds
  read_line(newSetting, sizeof(newSetting));

  unsigned int newReportRate = strtolong(newSetting); //Convert this string to an int

  //Error check
  if (newReportRate > minTime)
  {
    setting_report_rate = newReportRate; //Go to this new time

    //Record this new value to EEPROM
    RecordSystemSettings();

    Serial.print(F("Time between reports now: "));
    Serial.print(setting_report_rate);
    Serial.println(F("ms"));
  }
  else
    Serial.println(F("Error: Out of bounds"));
}
#else
void RateSetup(void)
{
  ClearAndSendHandshake();

  //Calculate the minimum time between reports
  unsigned int minTime = CalcMinReadTime();

  setting_report_rate = minTime + 1;

  ClearAndSendDone();
}
#endif

#ifdef USING_USB
//Configure what baud rate to communicate at
void BaudSetup(void)
{
  Serial.print(F("\n\n\rCurrent rate: "));
  Serial.print(setting_uart_speed, DEC);
  Serial.println(F(" bps"));

  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  Serial.println(F("Enter new baud rate ('x' to abort):"));

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
    RecordSystemSettings();

    //Go to this speed
    Serial.end();
    Serial.begin(setting_uart_speed);
  }
}
#else
void BaudSetup(void)
{
  ClearAndSendHandshake();

  long newRate = strtolong(ClearAndReadLine(8)); //Convert this string to a long

  if (newRate < BAUD_MIN)
  {
    newRate = 9600;
  }
  else if (newRate > BAUD_MAX)
  {
    newRate = 115200;
  }

  //Record this new baud rate
  setting_uart_speed = newRate;
  //Go to this speed
  Serial.end();
  Serial.begin(setting_uart_speed);

  ClearAndSendDone();
}
#endif

void ToggleUnits(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_units == UNITS_KG)
  {
    setting_units = UNITS_LBS;
    float newFactor = (float)setting_calibration_factor * 0.453592; //Convert the calibration factor from kg to lbs
    setting_calibration_factor = (long)newFactor;
  }
  else if (setting_units == UNITS_LBS)
  {
    setting_units = UNITS_KG;
    float newFactor = (float)setting_calibration_factor * 2.20462; //Convert the calibration factor from lbs to kg
    setting_calibration_factor = (long)newFactor;
  }
  scale.set_scale(setting_calibration_factor); //Assign this new factor to the scale

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

#ifdef USING_USB
//Configure how many decimals to show
void DecmialSetup(void)
{
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
    RecordSystemSettings();

    Serial.print(F("Decimal places: "));
    Serial.println(setting_decimal_places);
  }
  else
    Serial.println(F("Error: Out of bounds"));
}
#else
void DecmialSetup(void)
{
  ClearAndSendHandshake();

  int newDecimalPlaces = strtolong(ClearAndReadLine(8)); //Convert this string to an int

  if (newDecimalPlaces < 0)
  {
    newDecimalPlaces = 0;
  }
  else if (newDecimalPlaces > 4)
  {
    newDecimalPlaces = 4;
  }

  setting_decimal_places = newDecimalPlaces;

  ClearAndSendDone();
}
#endif
  
//Configure how many readings to average together
void AverageReadingSetup(void)
{
  //Get user input
  #ifdef USING_USB
  Serial.print(F("\n\n\rEnter the number of readings to average together (1 to 64): "));
  #else
  ClearAndSendHandshake();
  #endif

  int newAverageAmount = strtolong(ClearAndReadLine(8)); //Convert this string to an int

  if (newAverageAmount < 1)
  {
    newAverageAmount = 1;
  }
  else if (newAverageAmount > 64)
  {
    newAverageAmount = 64;
  }

  setting_average_amount = newAverageAmount;

  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif
}

void ToggleLocalTemp(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_local_temp_enable == true)
  {
    setting_local_temp_enable = false;
  }
  else
  {
    setting_local_temp_enable = true;
  }

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

void ToggleRemoteTemp(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_remote_temp_enable == true)
  {
    setting_remote_temp_enable = false;
  }
  else
  {
    setting_remote_temp_enable = true;
  }

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

void ToggleStatusLED(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_status_enable == true)
  {
    setting_status_enable = false;
    digitalWrite(statusLED, LOW); //Turn off the LED
  }
  else
  {
    setting_status_enable = true;
  }

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

void ToggleSerialTrigger(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_serial_trigger_enable == true)
  {
    setting_serial_trigger_enable = false;
  }
  else
  {
    setting_serial_trigger_enable = true;
  }

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

void ToggleRawReading(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  if (setting_raw_reading_enable == true)
  {
    setting_raw_reading_enable = false;
  }
  else
  {
    setting_raw_reading_enable = true;
  }

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

void SetTriggerCharacter(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  setting_trigger_character = ClearAndReadChar(1);

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

void ExitMenu(void)
{
  #ifndef USING_USB
  ClearAndSendHandshake();
  #endif

  //Do nothing, just exit
  ClearRxBuffer();
  RecordSystemSettings();
  Serial.flush();

  #ifndef USING_USB
  ClearAndSendDone();
  #endif
}

//Determine how much time we need between measurements
//Takes into account current baud rate
//Takes into account the time to read various sensors
//Takes into account raw reading printing
unsigned int CalcMinReadTime(void)
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

  #ifdef USING_USB
  Serial.println(characterTime);
  #endif

  //Calculate number of characters per report
  unsigned int characters = 0;

  if (setting_timestamp_enable == true) characters += strlen("51588595,"); //Timestamp has characters

  if (setting_local_temp_enable)
  {
    //Establish how much time it takes to do a local temp read
    unsigned long startTime = millis();
    for (byte x = 0 ; x < 8 ; x++)
      getLocalTemperatureF(); //Do a dummy read and time it
    averageReadTime = ceil((millis() - startTime) / (float)8);
    sensorReadTime += averageReadTime; //In ms

    characters += strlen("24.75,"); //Add the time it takes to print the characters as well
  }

  if (setting_remote_temp_enable)
  {
    //Establish how much time it takes to do a remote temp read
    unsigned long startTime = millis();
    for (byte x = 0 ; x < 8 ; x++)
      getRemoteTemperatureF(); //Do a dummy read and time it
    averageReadTime = ceil((millis() - startTime) / (float)8);
    sensorReadTime += averageReadTime; //In ms

    characters += strlen("27.81,"); //Add the time it takes to print the characters as well
  }

  characters += strlen("123,"); //Basic weight without decimals

  if (setting_decimal_places > 0) characters += setting_decimal_places + 1; //For example 4: 3 decimal places and the '.'

  // if (setting_units == UNITS_LBS) characters += strlen("lbs");
  // if (setting_units == UNITS_KG) characters += strlen("kg");

  if (setting_raw_reading_enable == true)
  {
    //Establish how much time it takes to do a raw read
    unsigned long startTime = millis();
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

  return (unsigned int)(sensorReadTime + ceil((float)characters * characterTime));
}