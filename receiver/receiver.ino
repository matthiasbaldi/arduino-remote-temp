#include <RH_ASK.h>
#include <SPI.h> 
 
RH_ASK rf_driver;

String temp;
String device;
String str_out;
 
void setup() {
    rf_driver.init();
    Serial.begin(9600);
}
 
void loop() {
    // Set buffer to size of expected message
    uint8_t buf[13];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    //Serial.println(rf_driver.recv(buf, &buflen));
    if (rf_driver.recv(buf, &buflen)) {
      str_out = String((char*)buf);
      Serial.println(str_out);
      device = str_out.substring(8, 7);
      temp = str_out.substring(9, 13);
      
      // Print values to Serial Monitor
      Serial.print("Device: ");
      Serial.print(device);
      Serial.print("  - Temperature: ");
      Serial.println(temp);    
    }
}
