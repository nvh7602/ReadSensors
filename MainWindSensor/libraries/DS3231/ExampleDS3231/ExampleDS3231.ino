#include "DS3231.h"

unsigned long time0 = 0;

DS3231 RealTime; // Khởi tạo đối tượng thời gian thực.

void setup(){
  Serial.begin(9600);
  //ds3231.setDateTime(3,1,11,16,22,12,40); //Cài đặt thời gian cho đồng hồ (thứ,ngày,tháng,năm,giờ,phút,giây).
}

void loop() {
  
  if((unsigned long)(millis() - time0) > 1000)
  {
    
    Serial.print("File name: ");
    Serial.print(RealTime.getFileName());
    Serial.print(" - Time: ");
    Serial.println(RealTime.getTime());


    time0 = millis();
  }
}
