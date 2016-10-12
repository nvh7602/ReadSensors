/*****************************************************************************
*
@Created by Ngo Van Hoa <nvhoa7602@gmail.com> 08, October, 2016.
*
@Copyright (C) 2016.
*
*****************************************************************************/
#include "GetData.h"


/****************************** GetData() *************************************
Input: N/A
Output: An object of class GetData.
Remarks: This is default constructor of the class GetData
********************************************************************************/
GetData::GetData()
{
	
}




/****************************** DataWrite() **********************************

Input:  Time: 				The actual time taken from DS3231 sensor
		NodeId: 			Identify of each nodes.
		AirTemperature: 	The temperature of air sensor (DHT).
		AirHumidity: 		The humidity of air sensor (DHT).
		SoilTemperature: 	The temperature of soilsensor(DS18B20).
		SoilMoisture: 		The moisture of soil sensor.
		LightIntensity: 	The intensity of light sensor.
Output: dataW: 				The string contain sensor values that separated with a comma (Ex: 12:0:1,123,20,78,30,255,25000)
Remarks: 
********************************************************************************/
String GetData::getData(String Time, byte NodeId, byte AirTemperature, byte AirHumidity, byte SoilTemperature, byte SoilMoisture, unsigned int LightIntensity)
{
	dataW = "";
	dataW += Time;
	dataW += ",";
	dataW += NodeId;
	dataW += ",";
	dataW += AirTemperature;
	dataW += ",";
	dataW += AirHumidity;
	dataW += ",";
	dataW += SoilTemperature;
	dataW += ",";
	dataW += SoilMoisture;
	dataW += ",";
	dataW += LightIntensity;
	  
	return dataW;
}




/****************************** dataSend() *********************************
Input: dataW: the string get from dataWrite(...) function.
Output: Return a string was inserting characters "(" at the top and characters ")" at the end of the string. (EX: (12:0:1,123,20,78,30,255,25000)).
Remarks: Must call dataWrite() function first to get dataW string that use for dataSend() function.
********************************************************************************/
//String GetData::dataSend(String Time, byte NodeId, byte AirTemperature, byte AirHumidity, byte SoilTemperature, byte SoilMoisture, unsigned int LightIntensity)
//{
////	String dataS ="(";
//	dataW = "(";
//	dataW += Time;
//	dataW += ", ";
//	dataW += NodeId;
//	dataW += ", ";
//	dataW += AirTemperature;
//	dataW += ", ";
//	dataW += AirHumidity;
//	dataW += ", ";
//	dataW += SoilTemperature;
//	dataW += ", ";
//	dataW += SoilMoisture;
//	dataW += ", ";
//	dataW += LightIntensity;
//    dataW += ")";
//    
//	return "123";
//}

