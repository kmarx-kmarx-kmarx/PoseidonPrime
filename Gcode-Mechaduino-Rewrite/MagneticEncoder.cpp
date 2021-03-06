/*
  -----------------------------------------------------------------------------
  DESCRIPTION: This file contains the functions necessary to initialize and read from the magnetic rotary encoder

        encoder_setup()   initialize the encoder.

        uint16_t encoder_read()    read a value from the encoder; value ranges from 0 to 2^14-1
        
        
  SHARED VARIABLES: None

  GLOBAL VARIABLES: none

  INCLUDES:
        Arduino.h Standard board utilities
        SPI.h     For SPI communication with the encoder
        MagneticEncoder.h For macros and function headers
  -----------------------------------------------------------------------------
  */

#include <Arduino.h>
#include <SPI.h>
#include "MagneticEncoder.h"

/*
  -----------------------------------------------------------------------------
  DESCRIPTION: encoder_setup() starts SPI communications with the encoder and keeps the transaction open.

  OPERATION:   The settings for the AS5047D magnetic encoder are set, then a SPI transaction is opened using these settings. The transaction remains open and is not closed.

  ARGUMENTS/RETURNS: none

  INPUTS / OUTPUTS:
        The MISO pin is set as an input and the MOSI, CLK, and CS pins are set as an output; signals are input/output along these lines to initialize the transaction.

  LOCAL VARIABLES: 
        SPISettings settingsA:  used to store the SPI settings for the magnetic encoder.

  SHARED VARIABLES / GLOBAL VARIABLES: none

  DEPENDENCIES:
        Arduino.h: for pin manipulations
        SPI.h:     for SPI communications
  -----------------------------------------------------------------------------
*/
void encoder_setup(){
  // Set the chip select pin as an output
  pinMode(CS_PIN, OUTPUT); 

  // Settings for the magnetic encoder 
  SPISettings encoder(SPI_SPEED, MSBFIRST, SPI_MODE1);
  // Begin SPI
  SPI.begin();
  // Wait for the device to receive the command    
  delay(SPI_PAUSE);
  // We have only 1 SPI device so we will keep the transaction ongoing for easy and fast reading.
  SPI.beginTransaction(encoder);

  return;
}

/*
  -----------------------------------------------------------------------------
  DESCRIPTION: encoder_read() reads the angle value from the magnetic encoder

  OPERATION:   We read in the bytes from the encoder and put them into a long.

  ARGUMENTS/RETURNS: none

  INPUTS / OUTPUTS:
        The MISO pin is set as an input and the MOSI and CLK pin is set as an output; signals are input/output along these lines to perform the transaction.

  LOCAL VARIABLES: 
        int angle, for storing the two byte angle measurement (ints are 4 bytes on the SAMD21)
        bytes b1, b2, for storing the reading from the encoder

  SHARED VARIABLES / GLOBAL VARIABLES: none

  DEPENDENCIES:
        SPI.h:     for SPI communications
        MagneticEncoder.h: has macro for setting the chip select pin
  -----------------------------------------------------------------------------
*/
uint16_t encoder_read(){
  uint16_t angleTemp;
  
  CHIPSELECT_LOW();

  // Read in two bytes
  byte b1 = SPI.transfer(0xFF);
  byte b2 = SPI.transfer(0xFF);

  // Put the two bytes into the int and mask off extraneous high bits
  angleTemp = (((b1 << 8) | b2) & 0B0011111111111111);

  CHIPSELECT_HIGH();
  
  return angleTemp;
}