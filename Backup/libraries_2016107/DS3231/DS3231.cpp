/****************************************************************
*
@Created by Ngo Van Hoa <nvhoa7602@gmail.com> 25, September, 2016.
*
@Copyright (C) 2016.
*
*****************************************************************/
#include "DS3231.h"
#define DS3231_I2C_ADDRESS 104


DS3231::DS3231()
{
	
}


/********************************* decToBcd *********************************
Input: 
Output: 
Remarks: 
********************************************************************************/
byte DS3231::decToBcd(byte val)
{
	return ( (val/10*16) + (val%10) );
}



/********************************* setDateTime *********************************
Input:  thu 	- the days of week
		ngay 	- the days of month
		thang	- the months of year
		nam		- year
		gio		- hour
		phut	- minute
		giay	- second
Output: Time has been installed for DS3231 sensor.
Remarks: The time just set once in SetUp() function of Arduino struct.
********************************************************************************/
void DS3231::setDateTime(byte thu, byte ngay, byte thang, byte nam, byte gio, byte phut, byte giay)
{
 	Wire.beginTransmission(DS3231_I2C_ADDRESS);
  	Wire.write(0x00);
 	Wire.write(decToBcd(giay));
 	Wire.write(decToBcd(phut));
	Wire.write(decToBcd(gio));
	Wire.write(decToBcd(thu));
	Wire.write(decToBcd(ngay));
	Wire.write(decToBcd(thang));
	Wire.write(decToBcd(nam));
	Wire.endTransmission();
}




/********************************* getDayName **********************************
Input: N/A
Output: This function return a string format as: yyyy-mm-dd.csv ( example: 2016-9-25.csv, 2016-10-25.csv,.....)
Remarks: 
********************************************************************************/
String DS3231::getDayName()
{
	getDateTime();
	
	String day = "";
	day += "20";
	day += year;
  	day += month;
  	day += date;
  	day += ".csv";
  	return day;
}



/********************************* getTime *************************************
Input: 
Output: 
Remarks: 
********************************************************************************/
String DS3231::getTime()
{
	getDateTime();
	
	String time = "";
	time += hours;
	time += ":";
	time += minutes;
	time += ":";
	time += seconds;
	
	return time;
}



/********************************* getDateTime *************************************
Input: 
Output: 
Remarks: 
********************************************************************************/
void DS3231::getDateTime()
{
  String Time = "";
  // send request to receive data starting at register 0
  Wire.beginTransmission(DS3231_I2C_ADDRESS); // 104 is DS3231 device address
  Wire.write(0x00); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes

  if(Wire.available())
  {
      seconds = Wire.read(); // get seconds
      minutes = Wire.read(); // get minutes
      hours   = Wire.read();   // get hours
      day     = Wire.read();
      date    = Wire.read();
      month   = Wire.read(); //temp month
      year    = Wire.read();
         
      seconds = (((seconds & B11110000)>>4)*10 + (seconds & B00001111)); // convert BCD to decimal
      minutes = (((minutes & B11110000)>>4)*10 + (minutes & B00001111)); // convert BCD to decimal
      hours   = (((hours & B00110000)>>4)*10 + (hours & B00001111)); // convert BCD to decimal (assume 24 hour mode)
      day     = (day & B00000111); // 1-7
      date    = (((date & B00110000)>>4)*10 + (date & B00001111)); // 1-31
      month   = (((month & B00010000)>>4)*10 + (month & B00001111)); //msb7 is century overflow
      year    = (((year & B11110000)>>4)*10 + (year & B00001111));
  }
  else 
  {
    //No data
  }

}



