#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>
#include <RH_ASK.h>
#include <SPI.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
#define CLK 6
#define DIO 7
TM1637Display display(CLK, DIO);
RH_ASK rf_driver;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void) {
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
  sensors.begin();
  rf_driver.init();
}

String str_out;

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
  String deviceID = String(DEVICEID);
  Serial.println(String("DeviceID: " + deviceID));
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");
  Serial.print("Temperature is: ");
  int temp = sensors.getTempCByIndex(0) * 100;
  Serial.println(temp);
  str_out = "gmonit;" + String(deviceID) + ";" + String(temp);
  char *transmitValue = str_out.c_str();
  Serial.println(transmitValue);
  display.showNumberDecEx(temp, 0b11100000, false, 4, 0);
  Serial.println(strlen(transmitValue));

  // send data over 433Mhz network
  rf_driver.send((uint8_t *)transmitValue, strlen(transmitValue));
  rf_driver.waitPacketSent();
  delay(20000);
}
