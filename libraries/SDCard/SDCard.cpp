/****************************************************************
*
@Created by Ngo Van Hoa <nvhoa7602@gmail.com> 25, September, 2016.
*
@Copyright (C) 2016.
*
*****************************************************************/

#include "SDCard.h"

#define chipSelect 10	// Set CS pin for SD Card reader

File file;

SDCard::SDCard()
{
	
}

/****************************** Set up SD Card *********************************
Input: chipSelect ( Pin 10 of Arduino Uno R3
Output: Notification: install SD Card failed or finished
Remarks: 
********************************************************************************/
void SDCard::setUpSDCard()
{
	Serial.println("Initializing SD card....");
   	if(!SD.begin(chipSelect))
   	{
    	Serial.println("Card failed, or not present");
    	//return ;
  	}
   	else
   	{
    	Serial.println("Card initialized...");
   	}
}


/************** Write fileName(16925.csv) file  to SD Card ********************
Input: data - A string that consit of data fields need to save to SD Card.
	   fileName - A name of file in SD Card ( yyyymmdd.csv).
Output: The fileName.csv saved to SD Card follow day format.
Remarks: 
********************************************************************************/
void SDCard::writeData(String data, String fileName)
{
	if(!SD.exists(fileName))
  	{
	    file = SD.open(fileName, FILE_WRITE);
	    file.println("Time,Node ID,Air Humidity,Air Temperature,Gnd Temperature,Gnd Humidity,Light");
	    file.close();
  	}
  	else
  	{
    	file = SD.open(fileName, FILE_WRITE);
    	if(file)
      	{
        	file.println(data);
        	// do we need time to save data at here ?
        	file.close();
      	}
      	else
      	{
      		Serial.print("Error opening ");
        	Serial.println(fileName);
      	}
  	}    
}
