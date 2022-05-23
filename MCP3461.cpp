/*
  MCP3461.cpp - 
  When the IRQ pin is in High-Z mode, an external
	pull-up resistor must be connected between DVDD and
	the IRQ pin. The device needs to be able to detect a
	Logic High state when no interrupt occurs in order to
	function properly (the pad has a Schmitt Trigger input
	to detect the state of the IRQ pin just like the user is
	seeing it). The pull-up value can be equal to
	100-200 k for a weak pull-up using the typical clock
	frequency. The pull-up resistor value needs to be
	chosen in relation with the load capacitance of the IRQ
	output, the MCLK frequency and the DVDD supply
	voltage, so that all interrupts can be detected correctly
	by the SPI host device.
  
*/

#include <Arduino.h>
#include <SPI.h>
#include "MCP3461.h"

/**************************************************************************/
/*!
@brief  Constructs a new MCP3461 object. 

@param    pin
		  the slave select pin of the sensor
@param    spi_settings
		  SPI configuration settings. Default SPI settings use 4000 KHz SPI
*/
/**************************************************************************/
MCP3461class::MCP3461class(const uint8_t pin, SPISettings spi_settings)
: _SS_PIN(pin), _spi_settings(spi_settings) {}

//MCP3461class::MCP3461class(const uint8_t pin): _SS_PIN(pin) {}
/**************************************************************************/

uint32_t MCP3461class::readMCP3461 (uint8_t ui8address)
{
	uint8_t ui8AdcUpperCodes = 0;			// Data register bits 15-8
	uint8_t ui8AdcLowerCodes = 0;			// Data register bits 7-0
	int16_t ui32AdcCodes = 0;
	uint8_t command = DEVICE_ADDR << 6 | ui8address << 2 | READ;

	if (ui8address == ADCDATA)
	{	
		SPI.beginTransaction(_spi_settings);
		digitalWrite(_SS_PIN,LOW);
		SPI.transfer(command); 
		ui8AdcUpperCodes = SPI.transfer(0x00);			// 
		ui8AdcLowerCodes = SPI.transfer(0x00);			// 
		digitalWrite(_SS_PIN,HIGH);
		SPI.endTransaction();
		ui32AdcCodes = ((int16_t)ui8AdcUpperCodes << 8) | ui8AdcLowerCodes;
		
		// Serial.print("ADC Data Register Read : ");  //Debug serial prints
		// Serial.println(ui32AdcCodes,2);			
		// Serial.print("ADC Bits 15-8 : ");		
		// Serial.println(ui8AdcUpperCodes,2);
		// Serial.print("ADC Bits 7-0 : ");  
		// Serial.println(ui8AdcLowerCodes,2);
	}
	else
	{	
		SPI.beginTransaction(_spi_settings);
		digitalWrite(_SS_PIN,LOW);
		SPI.transfer(command); 
		ui8AdcLowerCodes = SPI.transfer(0x00);			// register read
		digitalWrite(_SS_PIN,HIGH);	
		SPI.endTransaction();
		ui32AdcCodes = ((uint16_t)ui8AdcUpperCodes << 8) | ui8AdcLowerCodes;
		
		// Serial.print("ADC Command:");		//Debug serial prints
		// Serial.print(command, HEX);
		// Serial.print(" Register ");
		// Serial.print(ui8address, HEX);
		// Serial.print(" Reading Register Value :");
		// Serial.println(ui32AdcCodes, HEX);
	}

	return ui32AdcCodes;
}

void MCP3461class::writeMCP3461 (uint8_t ui8address, uint8_t ui8value)
{	
	uint8_t command = DEVICE_ADDR << 6 | ui8address << 2 | WRITE;
	
	if (ui8address != RESET)
	{
		
		SPI.beginTransaction(_spi_settings);
		digitalWrite(_SS_PIN,LOW);
		SPI.transfer(command); 
		SPI.transfer(ui8value);
		digitalWrite(_SS_PIN,HIGH);
		SPI.endTransaction();
		//Serial.println("Write Command");		//Debug serial prints
	}
	else
	{
		SPI.beginTransaction(_spi_settings);
		digitalWrite(_SS_PIN,LOW);
		SPI.transfer(ui8address);
		digitalWrite(_SS_PIN,HIGH);
		SPI.endTransaction();
		//Serial.println("Reset Command");			//Debug serial prints
	}
	
	// Serial.print("ADC Command ");		//Debug serial prints
	// Serial.print(command, HEX);
	// Serial.print(" Register ");
	// Serial.print(ui8address, HEX);
	// Serial.println(" Written");
	// Serial.print("With Register Value ");
	// Serial.println(ui8value, HEX);
}
	
void MCP3461class::beginMCP3461(){
			pinMode(_SS_PIN, OUTPUT);
			digitalWrite(_SS_PIN, HIGH);
		}
