#include "GetData.h"

GetData::GetData()
{
	
}

String GetData::dataWrite(String Time, byte NodeId, byte AirTemperature, byte AirHumidity, byte SoilTemperature, byte SoilMoisture, unsigned int LightIntensity)
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

String GetData::dataSend()
{
	String dataS ="";
	dataS += "(";
	dataS += dataW;
    dataS += ")";
    
	return dataS;
}

