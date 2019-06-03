/********************************************************************/
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>
#include <RCSwitch.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
#define CLK 6
#define DIO 7
TM1637Display display(CLK, DIO);

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
/********************************************************************/
RCSwitch sender = RCSwitch();
void setup(void) {
  /********************************************************************/
  // activate jumper for device identification
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  // start transmitter
  sender.enableTransmit(10);
}

void loop(void) {
  display.setBrightness(0x0f);
  // read jumper for device identification
  int DEVICEID = 0;
  if (digitalRead(A0) == LOW) {DEVICEID = 0;}
  if (digitalRead(A1) == LOW) {DEVICEID = 1;}
  if (digitalRead(A2) == LOW) {DEVICEID = 2;}
  if (digitalRead(A3) == LOW) {DEVICEID = 3;}
  if (digitalRead(A4) == LOW) {DEVICEID = 4;}
  if (digitalRead(A5) == LOW) {DEVICEID = 5;}
  if (digitalRead(A6) == LOW) {DEVICEID = 6;}

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  /********************************************************************/
  //"DeviceID: " + 
  String deviceID = String(DEVICEID);
  Serial.println(String("DeviceID: " + deviceID));
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");
  /********************************************************************/
  Serial.print("Temperature is: ");
  int temp = sensors.getTempCByIndex(0) * 100;
  Serial.println(temp);
  String tempString = String(temp);
  String transmitValue = String("gmonit;" + deviceID + ";" + tempString);
  Serial.println(transmitValue);
  // showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  display.showNumberDecEx(temp, 0b11100000, false, 4, 0);
  // send number over network

  char* pEnd;
/*  byte *p = transmitValue.c_str();
  String binString = "";
  while (*p != 0)  {
    binString = String(binString + String(*p, BIN)); p++;
  }*/
  Serial.println(transmitValue);
  byte val = strtol(transmitValue.c_str(), &pEnd, 2);
  sender.send(val);
  
  /*int i = 0;
  const char* str = transmitValue.c_str();
  for (char *p = str; *p; p++) {
      sender.send((i<<8) + *p, 16);
      i++;
  }*/
  // sender.send(transmitValue, 24);
  delay(5000);
}
