//******************* Include libraries ************************************

#include <SoftwareSerial.h>
#include "DS3231.h"
#include "SDCard.h"
#include "DHT.h"
#include "DS18B20.h"
#include "BH1750.h"
#include "GetData.h"

//************ Node ID ***********
#define NodeId 123


/******* Define Devices *******/
#define DRIP_IRRIGATION 8
#define SPRAY_IRRIGATION 9
#define FAN 1
#define SHUTTLE 2
/******* Define control code *******/
#define DRIP_IRR_ON_CODE "000:1"
#define DRIP_IRR_OFF_CODE "000:0"
#define SPRAY_IRR_ON_CODE "001:1"
#define SPRAY_IRR_OFF_CODE "001:0"
#define FAN_ON_CODE "002:1"
#define FAN_OFF_CODE "002:0"
#define SHUTTLE_ON_CODE "003:1"
#define SHUTTLE_OFF_CODE "003:0"

//************ XBEE **************
#define RX 6
#define TX 7


//*********** DHT ****************
#define DHTPIN 3 // Set pin 3 to get values from DHT sensor.// Type of sensor (DHT11 or DHT22)

//************ SDCard ************
//#define chipSelect 10	// Set CS pin for SD Card reader
 /** CS - pin 10
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13 */
 
 
 
 
//************ Gnd Temperature ******
 /* Pin 2 */
 
 
 
 
//*********** Gnd Humidity **********
 /* Pin Analog A3 */
 
 
 
 
//************ Light (I2C) ****************
 /* SDA pin A4
 	SCL pin A5 */


 	




