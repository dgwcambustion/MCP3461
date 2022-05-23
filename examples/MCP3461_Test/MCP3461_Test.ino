/*
 * MCP3461 Test
 *
 * Fetch and print values from a 
 * MCP3461 A2D over SPI
 * 
 * 
 */

#include <MCP3461.h> //ADC

#define SLAVE_SELECT_PIN 52
MCP3461class device1(SLAVE_SELECT_PIN);

float voltage =0;

void setup() {
  Serial.begin(115200); // start Serial communication
  SPI.begin(); // start SPI communication
  
  MCP3461_INIT(device1); // run sensor initialization
}

void loop() {
  // the sensor returns 0 when new data is ready
  Read_MCP3461(device1,voltage);
  Serial.println("");
  delay( 1000 ); // Slow down sampling to 10 Hz. This is just a test.

}

void MCP3461_INIT(MCP3461class A2D){ //initalise A2D for Voltage measurement
  Serial.println("initialising MCP3461");
  A2D.beginMCP3461();
  A2D.writeMCP3461 (RESET, 0xFF);   //Resets the part for initial use
  delay(100);
  A2D.writeMCP3461 (CONFIG1, 0x30);  // CONFIG1 register value (oversample ratio 40960)
  A2D.writeMCP3461 (CONFIG2, 0x8B);  // CONFIG2 register value (bias current, gain, autozero disabled)
  A2D.writeMCP3461 (CONFIG3, 0xC0);  // CONFIG3 register value (continuous conversion, dataformat, CRC, cal)
  A2D.writeMCP3461 (IRQ, 0x04);  // 
  //A2D.writeMCP3461 (OFFSETCAL, 0x0A);  //
  A2D.writeMCP3461 (MUX, 0x01);  //
  A2D.writeMCP3461 (CONFIG0, 0x23);  // CONFIG0 register value (internal clock, no current source, ADC covnersion mode)
  Serial.println("MCP3461 initialised");
  Serial.println("MCP3461 config regs");
  Serial.println(A2D.readMCP3461(CONFIG0));
  Serial.println(A2D.readMCP3461(CONFIG1));
  Serial.println(A2D.readMCP3461(CONFIG2));
  Serial.println(A2D.readMCP3461(CONFIG3));
  Serial.println(A2D.readMCP3461(MUX));
  Serial.println(A2D.readMCP3461(SCAN));
  Serial.println(A2D.readMCP3461(TIMER));
  Serial.println(A2D.readMCP3461(OFFSETCAL));
  Serial.println(A2D.readMCP3461(GAINCAL));
  Serial.println(A2D.readMCP3461(LOCK));
  Serial.println(A2D.readMCP3461(CRCCFG));
}

void Read_MCP3461(MCP3461class A2D, float &voltage){
  //SPI comms with MCP3461
  //check if new data is available
  uint16_t ui32AdcStatus = A2D.readMCP3461(IRQ);
  Serial.println(ui32AdcStatus);
  if(!(ui32AdcStatus & 0x40)){
    int16_t ui32AdcData = A2D.readMCP3461(ADCDATA);
    voltage = 2.5*float(ui32AdcData)/32768.0; 
    Serial.print("Voltage: ");
    Serial.println(voltage);
  }
}
