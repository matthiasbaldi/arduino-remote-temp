#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() 
{
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Empfänger ist an Interrupt-Pin "0" - Das ist am UNO der Pin2
}

// Return a received byte, or -1 if none is available.
int receivedValue()
{
    if (!mySwitch.available())  // no byte available
        return -1;
    static uint8_t last_byte_number = 0xff;
    uint16_t code = mySwitch.getReceivedValue();
    uint8_t this_byte_number = code >> 8;
    if (last_byte_number == this_byte_number)  // this is a duplicate
        return -1;
    last_byte_number = this_byte_number;
    return code & 0xff;  // LSB is the byte to return
}

static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}

void loop() {
  
  if (mySwitch.available())
  {
    
    
    //byte *str = mySwitch.getReceivedValue();
    Serial.println(mySwitch.getReceivedValue());
    /*Serial.println(mySwitch.getReceivedBitlength());
    Serial.println(mySwitch.getReceivedDelay());

  int* raw = mySwitch.getReceivedRawdata();
      Serial.print("Raw data: ");
  for (unsigned int i=0; i<= mySwitch.getReceivedBitlength()*2; i++) {
    Serial.print(raw[i]);
    Serial.print(",");
  }*/
    const char* b = dec2binWzerofill(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
    Serial.println( b );
    //Serial.println(mySwitch.getReceivedRawdata());
  }
  //mySwitch.setPulseLength(104);
  //int c = receivedValue();
  //if (c != -1)  // if we really got a new byte
  //  Serial.write(c);

  /*if (mySwitch.available()) // Wenn ein Code Empfangen wird...
  {
    int value = mySwitch.getReceivedValue(); // Empfangene Daten werden unter der Variable "value" gespeichert.
  
    if (value == 0) // Wenn die Empfangenen Daten "0" sind, wird "Unbekannter Code" angezeigt.
    {
      Serial.println("Unbekannter Code");
    } 
    
    else // Wenn der Empfangene Code brauchbar ist, wird er hier an den Serial Monitor gesendet.
    {
      Serial.print("Empfangen: ");
      int value = mySwitch.getReceivedValue();
      // first two numbers, sender identification
      int sender = 
      // third number, 1 means +, 0 means -
      boolean positive = true;
      if (value 
      Serial.println(  );
    }

    mySwitch.resetAvailable(); // Hier wird der Empfänger "resettet"
  }*/
  // mySwitch.resetAvailable(); // Hier wird der Empfänger "resettet"
}
