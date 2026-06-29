// Arduino.Pi.h
#ifndef ARDUINO.PI_H
#define ARDUINO.PI_H

// FROM RAPSBERRY PI --> ARDUINO
const char CMD_OPEN_MENU = 'M';                         //Capital, so different than internal openscale commands
const char CMD_CLOSE_MENU = 'X';      
const char CMD_TARE_SCALE = 'T';
const char CMD_WEIGHT_REQUEST = 'W';

// FROM ARDUINO --> RASPBERRY PI
const char CMD_EXECUTED = 'D';                          //Command from raspberry pi exucuted
const char CMD_WEIGHT_COMPLETE = 'F';


//These probabaly do not belong in this file but these are the COMMANDS from teh raspberry pi

//----------------------------------------------------------------------------------------------------------------------
//Normal tare opperation, not during calibration 
//Raspberry pi asks for scale to tare. sends a 'T'. Openscale reads the 'T' and goes to 'CMD_TAR_SCALE'. Then the goes to 'tare_scale(false)' and tares scale.
if (incoming == CMD_TARE_SCALE)
{
    tare_scale(false);                                   
}
// ----------------------------------------------------------------------------------------------------------------------
// Open Menu opperations
if (incoming == CMD_OPEN_MENU)
{
    //Not a command fo raspberry pi?
}
// ----------------------------------------------------------------------------------------------------------------------
//Close Menu opperations
if (incoming == CMD_CLOSE_MENU)
{
    //Not a command fo raspberry pi?
}
//-----------------------------------------------------------------------------------------------------------------------
//
if (incoming == CMD_WEIGHT_REQUEST)
{
    serial_trigger();
}