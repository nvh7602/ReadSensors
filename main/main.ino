//Standard: Data(Time, Node ID, Air Temperature, Air Humidity, Gnd Temperature, Gnd Humidity, Light)
// Phần Wind Veloc, Wind Director, Rain sẽ đưa qua arduino chính.

//******************* Include libraries ************************************

#include <SoftwareSerial.h>
#include "DS3231.h"
#include "SDCard.h"
#include "DHT.h"
#include "DS18B20.h"
#include "BH1750.h"


//***************** Setup signal pins ************************************

// Nguyên
#define TUOI_NHO_GIOT 8
#define TUOI_PHUN_SUONG 2 // Trùng với GndTemp
#define MANG_CHE 3
#define QUAT 4

// End: Nguyên



//***************** Declare Object ***************************************

DS3231 ds3231;
SDCard sdcard;
DHT dht(3, DHT11); // Set pin 3 to get values from DHT sensor.// Type of sensor (DHT11 or DHT22)
DS18B20 ds18b20;
BH1750 lightMeter;

SoftwareSerial xBee(6,7); // Rx, Tx



// ***************** Declare sensor value ******************************* 

byte NodeId = 123;
byte AirTemperature, AirHumidity, GndTemperature, GndHumidity;
unsigned int LightIntensity; // up to 65535 lux
unsigned long time = 0; // Set default time = 0

// Tân
int lastStringLength;
int i;

// End: Tân



//***************** Starting set up *****************************************
void setup()
{
  Wire.begin(); // SDCard
  Serial.begin(9600);
  xBee.begin(9600); // Start xbee 9600
  ds3231.setDateTime(5,25,9,16,23,59,50); // ******* Note: Set time for DS3231
  dht.begin(); // Start DHT sensor
  lightMeter.begin(); //Start BH1750


  // Nguyên
  pinMode(TUOI_NHO_GIOT, OUTPUT);
  pinMode(TUOI_PHUN_SUONG, OUTPUT);
  pinMode(MANG_CHE, OUTPUT);
  pinMode(QUAT, OUTPUT);
  // End: Nguyên


//************** Install SD card ****************************************
  sdcard.setUpSDCard(); // set CS pin (pin 10) for SD Card Reader
}



//***********************************************************************
void loop()
{
  ReadSensor();

  // Controls at here:

  String dataWrite = "";
  dataWrite += ds3231.getTime(); // Time
  dataWrite += ",";
  dataWrite += NodeId; // node ID
  dataWrite += ",";
  dataWrite += AirTemperature; // airTemp
  dataWrite += ",";
  dataWrite += AirHumidity; // airHumid
  dataWrite += ",";
  dataWrite += GndTemperature; // gndTemp
  dataWrite += ",";
  dataWrite += GndHumidity; // gndHumid
  dataWrite +=',';
  dataWrite += LightIntensity; // light

  String dataSend = "";
  dataSend += "(";
  dataSend += dataWrite;
  dataSend += ")";

//Serial.println(millis());

//***************** Write data to SD Card and Send data to xbee *******************************

  if((unsigned long)(millis() - time) > 3000)
  {
      sdcard.writeData(dataWrite, ds3231.getDayName());     // Write data to SD Card
      
      sendStatus(dataSend);   // Send data to Xbee
      
      if (xBee.available())
      {
          String lightValue = receiveStatus();
          Serial.println(lightValue);
      }  
   

      
  
    time = millis();
  }
}


void ReadSensor()
{
  AirTemperature = dht.readTemperature(); // Read data from DHT sensor
  AirHumidity = dht.readHumidity(); // Read data from DHT sensor
  GndTemperature = ds18b20.getGndTemp(); // Read data from DS18B20 ( temp of soil)
  GndHumidity = map(analogRead(A3), 0, 1023, 0, 255); // Read data from soilSensor 
  LightIntensity = lightMeter.readLight();  // Read data from light sensor 
}

//Tân
void sendStatus(String dataSend) {
  lastStringLength = dataSend.length();

  for(i = 0; i < lastStringLength; i++)
  {
     xBee.write(dataSend.charAt(i));
     Serial.print(dataSend.charAt(i));
    }
    Serial.println("");
}

String receiveStatus()
{
  //Serial.println("begin receive");
  String dataReceive;
  while(1)
  {
    if(xBee.available()){
        char receiveChar = xBee.read();
        if (receiveChar == '#') break;
        dataReceive += receiveChar;   
        }
  }
    //Serial.println("end receive");
    return dataReceive;
}

//End: Tân



// Nguyên

/******* Turn ON/OFF Devices *******/
void turnON(int deviceCode) {
  digitalWrite(deviceCode, HIGH);
}

void turnOFF(int deviceCode) {
  digitalWrite(deviceCode, LOW);
}

void tuoiNhoGiot(int anhSang, int nguongAnhSang, int maThietBi) {
  if (anhSang >= nguongAnhSang) {
    turnON(TUOI_NHO_GIOT);
  }
  if (anhSang < nguongAnhSang) {
    turnOFF(TUOI_NHO_GIOT);
  }
}

/******* Control Devices *******/
void controlDevices(int maThietBi, boolean flag) {
  switch(maThietBi)
  {
    case 000:
    {
      if (flag == 1) turnON(TUOI_NHO_GIOT);
      else turnOFF(TUOI_NHO_GIOT);
    }
    case 001:
    {
      if (flag == 1) turnON(TUOI_PHUN_SUONG);
      else turnOFF(TUOI_PHUN_SUONG);
    }
    case 002:
    {
      if (flag == 1) turnON(QUAT);
      else turnOFF(QUAT);
    }
    case 003:
    {
      if (flag == 1) turnON(MANG_CHE);
      else turnOFF(MANG_CHE);
    }
  }
}


//End: NGuyên

