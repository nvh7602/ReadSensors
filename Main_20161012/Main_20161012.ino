/****************************************************************************************************
 * The
 *
 * @Created by Ngo Van Hoa            <nvhoa7602@gmail.com>.
 *             Nguyen Dang Nhut Tan   <ndnhuttan@outlook.com>
 *             Tran Nhut Nguyen       <>
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/

//******************* Include libraries ************************************
#include "CONFIG.h"
#include <SoftwareSerial.h>
SoftwareSerial xb(RX, TX); //
#include "XBeePro.h"


//***************** Declare Object ***************************************

DS3231 ds3231;
SDCard sdcard;
DHT dht(DHTPIN, DHT11); 
DS18B20 ds18b20;
BH1750 lightMeter;
GetData data;
XBeePro xBeeObject;
//Adafruit_AM2315 am2315; // Cảm biến nhiệt độ độ ẩm AM2315





// ***************** Declare sensor value ******************************* 

byte AirTemperature, AirHumidity, SoilTemperature, SoilMoisture;
unsigned int LightIntensity; // up to 65535 lux
unsigned long time1 = 0; // Set default time = 0
String myID = "40F1ED40";
String confirmChar = "O";
String commandValue = "";


//***************** Starting setup *****************************************
void setup()
{
  Wire.begin(); // SDCard
  Serial.begin(9600);
  
  pinMode(DRIP_IRRIGATION, OUTPUT);
  
  xBeeObject.setupXbee(); // Start xbee 9600
  ds3231.setDateTime(4,12,10,16,23,59,40); // ******* Note: Set time for DS3231
  dht.begin(); // Start DHT sensor
  lightMeter.begin(); //Start BH1750

//************** Install SD card *******************************
  sdcard.setUpSDCard();
}



/******* Turn ON/OFF Devices *******/
void turnON(int deviceCode) {
  digitalWrite(deviceCode, HIGH);
}

void turnOFF(int deviceCode) {
  digitalWrite(deviceCode, LOW);
}

/******* Device Status *******/
boolean isON(unsigned char deviceCode) {
  return digitalRead(deviceCode);
}

/******* Drip Irrigation Control *******/
void dripIrr(int value, int threshold, int interval, String controlCode) {
    static unsigned long previous_time = 0;
  
    if (!isON(DRIP_IRRIGATION)) {
      previous_time = millis();
    }
    
    if (value > threshold && !isON(DRIP_IRRIGATION)) {
      turnON(DRIP_IRRIGATION);
      //Serial.println("Bat thiet bi tuoi nho giot");
      previous_time = millis();
    }
    
    if ((millis() - previous_time > interval) && isON(DRIP_IRRIGATION)) {
      turnOFF(DRIP_IRRIGATION);
      //Serial.println("Tat thiet bi tuoi nho giot");
    }
  
    if (controlCode.startsWith(DRIP_IRR_ON_CODE) && !isON(DRIP_IRRIGATION)) {
      turnON(DRIP_IRRIGATION);
      //Serial.println("Bat thiet bi tuoi nho giot");
      previous_time = millis();
    }
  
    if (controlCode.startsWith(DRIP_IRR_OFF_CODE) && isON(DRIP_IRRIGATION)) {
      turnOFF(DRIP_IRRIGATION);
      //Serial.println("Tat thiet bi tuoi nho giot");
    }
  
}






//***********************************************************************
void loop()
{
  
//***************** Check available xbee*************************************
  if(xBeeObject.checkAvailable())
   {
      String receiveData = xBeeObject.receiveData();
      //Serial.println("--" + receiveData + "--");//for debugging
      //delay(2000);
     if(receiveData == myID)
     {
        //xBeeObject.sendData(confirmChar);
        commandValue = xBeeObject.receiveData();
        //Serial.println(commandValue);//for debugging
      }
    }


    
//***************** Reading data from sensors *******************************

    AirTemperature = dht.readTemperature(); // Read data from DHT sensor
    //AirTemperature = am2315.readTemperature(); // Read data from AM2315 sensor
    
    AirHumidity = dht.readHumidity(); // Read data from DHT sensor
    //AirHumidity = am2315.readHumidity(); // Read data from AM2315 sensor
    
    SoilTemperature = ds18b20.getGndTemp(); // Read data from DS18B20 ( temp of soil)
    
    SoilMoisture = map(analogRead(A3), 0, 1023, 0, 255); // Read data from soilSensor 
    
    LightIntensity = lightMeter.readLight();  // Read data from light sensor 




//***************** Control devices *****************************************
    dripIrr(LightIntensity, 2500, 15000, commandValue);
    //Serial.println(commandValue);
    commandValue = "";


   
//************* Write data to SD Card and Send data to xbee *****************
  if((unsigned long)(millis() - time1) > 5000) // Set time to write and send data
  {
    String sensorData = data.getData(ds3231.getTime(), NodeId, AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity );
  
    sdcard.writeData(sensorData, ds3231.getDayName());  // Write data to SD Card

    xBeeObject.sendData('('+ sensorData +')');
     
    Serial.println(sensorData); // Print data to serial monitor
    
    time1 = millis();
  }
}


