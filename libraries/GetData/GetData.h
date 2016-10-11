/****************************************************************************************************
 * The ReadSensor Arduino Library is an open sources for every body who work with sensor and arduino.
 *
 * @Created by Ngo Van Hoa <nvhoa7602@gmail.com> 08, October, 2016.
 *
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/

#include <Arduino.h>

typedef unsigned char byte;

class GetData{
	public:
		String dataW = "";
	public:
		GetData();
		String dataSend();
		String dataWrite(String , byte, byte, byte, byte, byte , unsigned int); // (time, nodeId, AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity) 
	
};
