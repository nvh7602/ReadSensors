/****************************************************************************************************
 * The ReadSensor Arduino Library is an open sources for every body who work with sensor and arduino.
 *
 * @Created by Ngo Van Hoa <nvhoa7602@gmail.com>.
 *
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/
#include "OneWire.h"
#include "DallasTemperature.h"

typedef unsigned char byte;

class DS18B20{
	public:
		DS18B20();
		byte getTemperature();
		
};
