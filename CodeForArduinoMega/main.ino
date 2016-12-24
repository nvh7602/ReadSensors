/****************************************************************************************************
 * 
 *
 * @Created by Ngo Van Hoa            <nvhoa7602@gmail.com>.
 *             Nguyen Dang Nhut Tan   <ndnhuttan@outlook.com>main
 *             Tran Nhat Nguyen       <avimodam@gmail.com>
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/

//******************* Include libraries ************************************
#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 256
#include "CONFIG.h"            // Config Pin for Arduino.
#include "DS3231.h"            // Real time
#include "SDCard.h"            // Module SD Card
#include "DS18B20.h"           // Soil Sensor
#include "BH1750.h"            // Light Sensor
#include "GetData.h"           // Data use to write in SD Card
#include "DHT.h"               // Air Sensor ( Air Temperature vs Air Humidity)

// Control
#include "Control.h"
#include "SDCardConfig.h"
String configString;
String* configList;
Control control;
SDCardConfig sdCardConfig;

//***************** Declare Object ***************************************

DS3231    RealTime;                     // Initialize Real Time object ( used to get the time and date).
SDCard    sdcard;                       // Initialize SD Card object ( used to write sensor data to memory stick).
DS18B20   SoilSensor;                   // Initialize Soild Sensor ( used to read soil temperature).
BH1750    LightSensor;                  // Initialize Light Sensor ( used to read light intensity).
GetData   data;                         // Initialize Data object ( used to return a string sensor values that using for write and send data). 
DHT       AirSensor(DHTPIN, DHT11);     // Initialize Air Sensor object ( used to get air temperature and humidity).

// ***************** Declare sensor values ********************************* 
#include <XBee.h>

/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40F1ED02);
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

byte  AirTemperature,                 // variable used to store air temperature values
      AirHumidity,                    // variable used to store air humidity values
      SoilTemperature,                // variable used to store soil temperature values
      SoilMoisture;                   // variable used to store soil moisture values
      
unsigned int  LightIntensity;         // return a value up to 65535 lux
unsigned long timeOld = 0;            // Set default time = 0


char config_start_available = '[';
char config_end_available = ']';
String command = "";
String configuration = "";

//***************** Starting setup *****************************************
void setup()
{
  Wire.begin();           // Start Module sd card.
  Serial.begin(9600);     // Start serial monitor.
  xbee.begin(Serial);
  AirSensor.begin();      // Start DHT11 sensor.
  LightSensor.begin();    // Start BH1750 senser.

 //RealTime.setDateTime(3,15,11,16,20,43,40); // Note: Set time for Real Time DS3231.
 
//************** Install SD card ****************************************
  sdcard.setUp();   // Setup Module SD Card.
}



//***********************************************************************
void loop()
{
  if(configuration != "") {
    sdCardConfig.writeConfig(configuration);
    configuration = "";
  }

  configString = sdCardConfig.readConfig();
  
  String commandList[10];
  configList = control.getConfig(configString);

     xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
            
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
     
            String temp = "";
            for (int i = 0; i < rx.getDataLength(); i++) { 
                char unitcom = rx.getData(i);
                temp += unitcom;
            }
            
            if((char)rx.getData(0) == config_start_available && (char)rx.getData(rx.getDataLength() - 1) == config_end_available)
            {
              for(int i = 1; i < temp.length() - 1; i++){
                configuration += temp.charAt(i);
                }
              
              Serial.println(configuration);//debug
              Serial.println("Get config----------------");
              }
              else
              {
                command = temp;
                Serial.println(command);//debug
                Serial.println("Get command+++++++++++++");
                }
        }
        
      } else if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
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
            Serial.println("ASSOCIATED");
          } else if (msr.getStatus() == DISASSOCIATED) {
            Serial.println("DISASSOCIATED");
          } else {
           Serial.println("Another status");
          }
        } else {
          Serial.println("Not something we were expecting");  
        }
    } else if (xbee.getResponse().isError()) {
      Serial.print("Error reading packet.  Error code: ");  
      Serial.println(xbee.getResponse().getErrorCode());
    }

//***************** Reading data from sensors *******************************

    AirTemperature = AirSensor.readTemperature();         // Read the air temperature.
    
    AirHumidity = AirSensor.readHumidity();               // Read the air humidity.
    
    SoilTemperature = SoilSensor.getTemperature();        // Read the soil temperature.
    
    SoilMoisture = map(analogRead(A3), 0, 1023, 100, 0);  // Read the soil moisture.
    
    LightIntensity = LightSensor.readLight();             // Read the light intensity.

    // Control
    control.setCommandList(AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity, configList, commandList, configString, command);
    command="";



//************* Write data to SD Card and Send data to xbee ***********************

  if((unsigned long)(millis() - timeOld) > 5000)      
  {
    String sensorData = "";
    sensorData = data.getData(RealTime.getTime(), NodeId, AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity );
    Serial.println(sensorData);
    sdcard.writeData(sensorData, RealTime.getFileName());
    
    int lastStringLength = sensorData.length();
    uint8_t payload[lastStringLength];
      for(int i = 0; i < lastStringLength; i++)
      {
        payload[i]= sensorData.charAt(i);
      }
      
      ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
      xbee.send(zbTx);
    
    timeOld = millis();
  }
}

