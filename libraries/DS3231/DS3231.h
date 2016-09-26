/****************************************************************************************************
 * The OpenSensor Arduino Library is an open sources for every body who work with sensor and arduino.
 *
 * @Created by Ngo Van Hoa <nvhoa7602@gmail.com>.
 *
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/
#include <Arduino.h>
#include "Wire.h"

typedef unsigned char byte;


class DS3231{
	public:
		byte seconds, minutes, hours, day, date, month, year;
		char weekDay[4];
		
	public:
		DS3231();
		byte decToBcd(byte);
		void setDateTime(byte, byte, byte, byte, byte, byte, byte);
		void getDateTime();
		String getDayName();
		String getTime();
			
};

