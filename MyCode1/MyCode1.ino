/****** NOTE ******
 SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** CS - pin 10
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
*/

//***************** Include libraries ************************************
#include <SoftwareSerial.h>
#include "DHT.h"
#include "BH1750.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include <SPI.h>
#include <SD.h>





//***************** Setup signal pins ************************************
byte rainSensor = 4; // Set pin 6 to get values from rainSensor.
byte DHTPIN = 3; // Set pin 3 to get values from DHT sensor.
byte DHTTYPE = DHT11; // Type of sensor (DHT11 or DHT22)
byte chipSelect = 10; // Set pin for SD card
#define ONE_WIRE_BUS 2





//***************** Declare Object ***************************************
DHT dht(DHTPIN, DHTTYPE); // Create dht object
BH1750 lightMeter;
SoftwareSerial xbee(6,7); // Rx, Tx

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature gndTemp(&oneWire);
File file;




// ***************** Declare sensor value ******************************* 
//Standard: Data(nodeID, airTemp, airHumid, gndTemp, gndHumid, light, windVeloc, windDirec, rain)

byte sensorVal[] = {123,0,0,1}; //Data(nodeID, airTemp, airHumid, gndTemp, rain)
int gndHumVal, windVelVal, windDirVal;
unsigned int lightVal; // up to 65535 lux
unsigned long time = 0; // Set default time = 0




//***************** Start set up *****************************************
void setup() {
  Serial.begin(9600);
  xbee.begin(9600); // Start xbee 9600
  dht.begin(); // Start DHT sensor
  lightMeter.begin(); //Start BH1750
  gndTemp.begin(); //Start DS18B20
  pinMode(rainSensor, INPUT); // Set pin 6 input value

  



//************** Install SD card ***********************************************

 Serial.println("Initializing SD card....");
 if(!SD.begin(chipSelect))
 {
    Serial.println("Card failed, or not present");
    return;
 }
 else
 {
  Serial.println("Card initialized...");
  
  if(!SD.exists("Data.csv"))
  {
    file = SD.open("Data.csv", FILE_WRITE);
    file.println("Node ID,Air Humidity,Air Temperature,Gnd Temperature,Gnd Humidity,Light,Wind Veloc,Wind Director,Rain");
    file.close();
  }
 }
}




//***********************************************************************
void loop() {

// Read data from DHT sensor
  sensorVal[1] = dht.readTemperature();

// Read data from DHT sensor
  sensorVal[2] = dht.readHumidity();  

// Read data from DS18B20 ( temp of soil)
  gndTemp.requestTemperatures();
  sensorVal[3] = gndTemp.getTempCByIndex(0); 

// Read data from rainSensor, if it don't rain => 1
  sensorVal[4] = digitalRead(rainSensor);
  
// Read data from soilSensor 
  gndHumVal = analogRead(A3) ;

// Read data from light sensor 
  uint16_t lux = lightMeter.readLightLevel();
  lightVal = lux;

// Read data from windSensor ( Veloc)
  windVelVal = 6;

// Read data from windsensor (Direct)
  windDirVal = 7;

  String data = "";
  data += sensorVal[0]; // node ID
  data += ",";
  data += sensorVal[1]; // airTemp
  data += ",";
  data += sensorVal[2]; // airHumid
  data += ",";
  data += sensorVal[3]; // gndTemp
  data += ",";
  data += gndHumVal; // gndHumid
  data += ",";
  data += lightVal; // light
  data += ",";
  data += windVelVal; // windVeloc
  data += ",";
  data += windDirVal; // windDirec
  data += ",";
  data += sensorVal[4]; // rain



//Serial.println(millis());
//***************** Write data to SD Card and Send data to xbee *******************************
 
  if((unsigned long)(millis() - time) > 2237)
  {
    //Serial.println(data); // Show data to see it right ?
    
    // ************* Write data to SD Card *******************************
     
      file = SD.open("Data.csv", FILE_WRITE);
      if(file)
      {
        file.println(data);
        // Need time to save data at here
        file.close();
      }
      else
        Serial.println("Error opening datalog.csv");

    // ************** Send data to Xbee *************************************
   
    //********Convert String to Char array *********
      char charData[data.length() + 1];
      data.toCharArray(charData, data.length() + 1);
  
      //xbee.println(data);
      // Có cần time delay cho xbee send data ??
      Serial.println(charData);
      //Serial.println(millis());
    time = millis();
  }
}
