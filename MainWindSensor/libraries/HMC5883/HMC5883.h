#ifndef HMC5883_h
#define HMC5883_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

typedef unsigned char byte;

class HMC5883
{
	public:
		HMC5883();
		void setUp();
		int getAngle();
		byte getDirection();
		
};

#endif
