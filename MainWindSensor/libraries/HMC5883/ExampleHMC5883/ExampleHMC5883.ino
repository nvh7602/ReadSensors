#include "HMC5883.h"

unsigned long time0 = 0;
HMC5883 compass;

void setup() {
  Serial.begin(9600);
  compass.setUp();
}

void loop() {

 if((unsigned long)(millis() - time0) > 1000)
 {
   Serial.print("Angle : ");
   Serial.print(compass.getAngle());
   Serial.print(" degree - Direction: ");
   Serial.println(compass.getDirection());

   time0 = millis();
 }
}
