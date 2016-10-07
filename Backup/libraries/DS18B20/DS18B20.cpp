/****************************************************************
*
@Created by Ngo Van Hoa <nvhoa7602@gmail.com> 25, September, 2016.
*
@Copyright (C) 2016.
*
*****************************************************************/

#include "DS18B20.h"

#define ONE_WIRE_BUS 2 // Set pin for DS18B20

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature gndTemp(&oneWire);

DS18B20::DS18B20()
{
	
}



/********************************* getGndTemp*********************************
Input: 
Output: 
Remarks: 
********************************************************************************/
byte DS18B20::getGndTemp()
{
	gndTemp.requestTemperatures();
	return gndTemp.getTempCByIndex(0); 
}