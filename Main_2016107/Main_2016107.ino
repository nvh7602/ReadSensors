/****************************************************************************************************
 * The
 *
 * @Created by Ngo Van Hoa            <nvhoa7602@gmail.com>.
 *             Nguyen Dang Nhut Tan   <>
 *             Tran Nhut Nguyen       <>
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/

//******************* Include libraries ************************************
#include "CONFIG.h"

//***************** Declare Object ***************************************

DS3231 ds3231;
SDCard sdcard;
DHT dht(DHTPIN, DHT11); 
DS18B20 ds18b20;
BH1750 lightMeter;
GetData data;
SoftwareSerial xBee(RX,TX); // 6,7



// ***************** Declare sensor value ******************************* 

byte AirTemperature, AirHumidity, SoilTemperature, SoilMoisture;
unsigned int LightIntensity; // up to 65535 lux
unsigned long time = 0; // Set default time = 0

//***************** Starting set up *****************************************
void setup()
{
  Wire.begin(); // SDCard
  Serial.begin(9600);
  xBee.begin(9600); // Start xbee 9600
  //ds3231.setDateTime(4,28,9,16,22,16,50); // ******* Note: Set time for DS3231
  dht.begin(); // Start DHT sensor
  lightMeter.begin(); //Start BH1750

//************** Install SD card ****************************************
  sdcard.setUpSDCard(); // set CS pin (pin 10) for SD Card Reader
}



//***********************************************************************
void loop()
{
//***************** Write data to SD Card and Send data to xbee *******************************

  if((unsigned long)(millis() - time) > 5000) // Set time to write and send data
  {
    AirTemperature = dht.readTemperature(); // Read data from DHT sensor
    AirHumidity = dht.readHumidity(); // Read data from DHT sensor
    SoilTemperature = ds18b20.getGndTemp(); // Read data from DS18B20 ( temp of soil)
    SoilMoisture = map(analogRead(A3), 0, 1023, 0, 255); // Read data from soilSensor 
    LightIntensity = lightMeter.readLight();  // Read data from light sensor 

    // Control at here
    
    sdcard.writeData(data.dataWrite(ds3231.getTime(), NodeId, AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity ), ds3231.getDayName());  // Write data to SD Card

    // Code use to send data at here

    Serial.println(data.dataSend()); // Print data to serial monitor
    
    time = millis();
  }
}


