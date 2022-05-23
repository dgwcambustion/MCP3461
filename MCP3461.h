/*
  MCP3461.h - 
  
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
#ifndef MCP3461_h
#define MCP3461_h

#include <Arduino.h>
#include <SPI.h>

/*****************************************************************************/
/************************** ADC Address Definitions **************************/
/*****************************************************************************/

//ADC registers
#define ADCDATA 0x0
#define CONFIG0 0x1
#define CONFIG1 0x2
#define CONFIG2 0x3
#define CONFIG3 0x4
#define IRQ 0x5
#define MUX 0x6
#define SCAN 0x7
#define TIMER 0x8
#define OFFSETCAL 0x9
#define GAINCAL 0xA
#define LOCK 0xD
#define CRCCFG 0xF

#define DEVICE_ADDR 0x1 //device address, in practice this is always 1

//Commands
#define STATIC_READ 0x1
#define WRITE 0x2
#define READ 0x3

#define RESET				0x78					// Resets the chip to default


class MCP3461class
{
		const uint8_t _SS_PIN;     ///< slave select pin (active low)
		SPISettings _spi_settings;  ///< object to hold SPI configuration settings
	public:
			
			MCP3461class(const uint8_t pin, SPISettings spi_settings = SPISettings(1000000, MSBFIRST, SPI_MODE0));
			void writeMCP3461 (uint8_t ui8address, uint8_t ui8value);
			uint32_t readMCP3461 (uint8_t ui8address);
			void beginMCP3461();

};

//extern MCP3461class MCP3461;

#endif
