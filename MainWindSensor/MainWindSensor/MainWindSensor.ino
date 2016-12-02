
#include "HMC5883.h"
#include "DS3231.h"
#include <SPI.h>
#include <SD.h>
#include <XBee.h>

//***************** Define values *********************************** 

#define encoderPin  2       // Declare pin 2 to read pulses from Encoder sensor.
#define rainPin     3       // Declare pin 3 to read signal data from rain sensor.


//***************** Declare variable *********************************** 
unsigned int rpm = 0;         // rpm (rev/min)
volatile byte pulses;         // the pulses per second
unsigned long timeOld1 = 0;   // Time to get rmp of encoder
unsigned long timeOld2 = 0;   // Time to send and write data 
byte pulsesPerTurn = 20;      // Số xung trên đĩa.



//***************** Declare Object *********************************** 
HMC5883 WindSensor;           // Declare WindSensor object
DS3231 Time;                  // Declare RealTime object
File file;                    // Declare file object

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
//create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40F1ED02);
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


// ******************** Setup **************************************
void setup() {
 Serial.begin(9600);
 pinMode(encoderPin, INPUT);
 WindSensor.setUp();
 xbee.begin(Serial);
 if(!SD.begin(10))
 {
   Serial.println("SDE");
   return ;
 }
 else
 {
   Serial.println("SDO");
 }
 attachInterrupt(0, counter, FALLING);
}


void loop() {

xbee.readPacket();   
if (xbee.getResponse().isAvailable()) {
  // got something
  
  if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
       Serial.println("SUCCESS");
      } else {
        Serial.println("FAIL");
      }
  } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
      xbee.getResponse().getModemStatusResponse(msr);        
      if (msr.getStatus() == ASSOCIATED) {
        //Serial.println("ASSOCIATED");
      } else if (msr.getStatus() == DISASSOCIATED) {
        //Serial.println("DISASSOCIATED");
      } else {
       //Serial.println("Another status");
      }
    } else {
      //Serial.println("Not something we were expecting");  
    }
} else if (xbee.getResponse().isError()) {
      //Serial.print("Error reading packet.  Error code: ");  
      //Serial.println(xbee.getResponse().getErrorCode());
    }

//************* Counter pulses of Encoder ***************************

  if((unsigned long)(millis() - timeOld1) >= 1000)
  {
    detachInterrupt(0);  
    rpm = (60 * pulses) / pulsesPerTurn; // pulsesPerTurn
    pulses = 0;
    
    timeOld1 = millis();
    attachInterrupt(0, counter, FALLING);
  }

//************* Write data to SD Card and Send data to xbee ***********************

  if((unsigned long)(millis() - timeOld2) >= 5000)
  {
    String data = "";
    data = Time.getTime();
    data += ",";
    data += "1";
    data += ",";
    data += rpm;
    data += ",";
    data += WindSensor.getAngle();
    data += ",";
    data += digitalRead(rainPin);
    
    Serial.println(data);
    writeDataWind(data, Time.getFileName()); // Write data to SD Card
    sendData(data);                          // Send data

    
    timeOld2 = millis();
  }
}

//************************* Đếm xung sử dụng cho ngắt ********************
void counter()
{
  pulses++;
}


//************************* Send data ***********************************
void sendData(String sensorData)
{
    int lastStringLength = sensorData.length();
    uint8_t payload[lastStringLength];
    for(int i = 0; i < lastStringLength; i++)
    {
      payload[i]= sensorData.charAt(i);
    }
    ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
    xbee.send(zbTx);
}

//************************* Write data **********************************
void writeDataWind(String data, String fileName)
{
  
  if(SD.exists(fileName))
  {
    
    file = SD.open(fileName, FILE_WRITE);
  
    if(file)
    {
      file.println(data);
      file.close();
    }
    else
    {
      Serial.println("EOF");
    }
  }
  else
  {
    file = SD.open(fileName, FILE_WRITE);
    file.println("Time,NodeID,WindVelocity,WindDirection,RainStatus");
    file.println(data);
    file.close();
  }
}

////************************* Check ram **********************************
//int getMemoryFree() {
//  // Trong trường hợp này, ta có thể hiểu extern sẽ khai báo một biến toàn cục trong chương trình (nếu chưa có) hoặc include một biến toàn cục đã được extern trước đó
//  extern int __heap_start;
//  extern int *__brkval; 
//  
//  //Dấu & phía trước tên biến / tên con trỏ sẽ cho ta biết vị trí ô nhớ mà nó đang đứng
//  //Lưu ý: bài viết này không dành cho beginner và bạn cần tưởng tượng một chút để có thể mườn tượng vấn đề
//  return (int) SP - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
//}
