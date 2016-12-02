/****************************************************************************************************
 * The DS3131 Sensor library is an open sources for every body who work with sensor and arduino.
 *
 * @Created by Ngo Van Hoa <nvhoa7602@gmail.com>.
 *
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/
#ifndef DS3231_h
#define DS3231_h

#include <Arduino.h>
#include "Wire.h"

typedef unsigned char byte;


class DS3231{
	public:
		byte seconds, minutes, hours, day, date, month, year;
		
	public:
		DS3231();
		byte decToBcd(byte);
		void setDateTime(byte, byte, byte, byte, byte, byte, byte);
		void getDateTime();
		String getFileName();
		String getTime();
			
};

#endif
