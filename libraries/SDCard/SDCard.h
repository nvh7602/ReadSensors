/****************************************************************************************************
 * The ReadSensor Arduino Library is an open sources for every body who work with sensor and arduino.
 *
 * @Created by Ngo Van Hoa <nvhoa7602@gmail.com> 25, September, 2016.
 *
 * @Copyright (C) 2016.
 *
 * Full sources: https://github.com/nvh7602/ReadSensors
 *
 *****************************************************************************************************/



/****** NOTE **********
 SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** CS - pin 10
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
*********************/

#include <SPI.h>
#include <SD.h>

typedef unsigned char byte;

class SDCard{
		
	public:
		SDCard();
		void setUpSDCard();
		void writeData(String, String);
};
