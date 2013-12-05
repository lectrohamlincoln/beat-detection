/*
 11-18-2013
 John Fitzpatrick
 Arduino example to display the Beat Detector's current BPM hypothesis 
 on 7segement display. Data is transfered over Software Serial.
 The Beat Detector transfers one unsigned byte every 10ms (100Hz) in 
 ranging of 70 to 188 or 0 if it cannot determine the BPM.
 
 
 Code modified from Example:
 Spark Fun Electronics
 Nathan Seidle
 http://github.com/sparkfun/Serial7SegmentDisplay/wiki/Special-Commands
 
 SoftSerial Example 
 by Tom Igoe
 based on Mikal Hart's example
 
 
 To get this code to work, attached an OpenSegment to an Arduino Uno using the following pins:
 Pin 10 on Uno (CS) to CS on OpenSegment
 Pin 11 to MOSI
 Pin 12 to MISO
 Pin 13 to SCK
 VIN to PWR
 GND to GND
 
 * RX is digital pin 8 (connect to TX of other device)
 * TX is digital pin 9 (connect to RX of other device)

*/

#include <SoftwareSerial.h>
#include <SPI.h>


SoftwareSerial mySerial(8, 9); // RX, TX

int csPin = 10; //You can use any IO pin but for this example we use 10

int bpm = 120;

void setup() 
{
  mySerial.begin(9600); //start soft serial

  Serial.begin(9600); //Start serial communication at 9600 for debug statements
  Serial.println("OpenSegment Example Code");

  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH); //By default, don't be selecting OpenSegment

  Serial.begin(9600); //Start serial communication at 9600 for debug statements
  Serial.println("OpenSegment Example Code");

  SPI.begin(); //Start the SPI hardware
  SPI.setClockDivider(SPI_CLOCK_DIV64); //Slow down the master a bit

  //Send the reset command to the display - this forces the cursor to return to the beginning of the display
  digitalWrite(csPin, LOW); //Drive the CS pin low to select OpenSegment
  SPI.transfer('v'); //Reset command
}

void loop() 
{
  int new_bpm = 0;
  if(mySerial.available()){
    bpm = mySerial.read();
  }
  
  if(new_bpm != bpm){
    new_bpm = bpm; 
    Serial.print("BPM: ");
    Serial.println(bpm);
    spiSendValue(bpm); //Send the four characters to the display
    delay(50); //If we remove the slow debug statements, we need a very small delay to prevent flickering
  }

}

//Given a number, spiSendValue chops up an integer into four values and sends them out over spi
void spiSendValue(int tempCycles)
{
  digitalWrite(csPin, LOW); //Drive the CS pin low to select OpenSegment

  SPI.transfer(tempCycles / 1000); //Send the left most digit
  tempCycles %= 1000; //Now remove the left most digit from the number we want to display
  SPI.transfer(tempCycles / 100);
  tempCycles %= 100;
  SPI.transfer(tempCycles / 10);
  tempCycles %= 10;
  SPI.transfer(tempCycles); //Send the right most digit

  digitalWrite(csPin, HIGH); //Release the CS pin to de-select OpenSegment
}
