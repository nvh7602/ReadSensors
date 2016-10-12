/******************************************************************
******** 
* ++ 
* Author: Nguyen Dang Nhut Tan 
* Module Name : XBeePro.cpp
*
* Description : Source file for class XBeePro
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* 
* Mod. History : 
*				+ 22.09.2016 - File first created

Standard_C/C++ Coding Convention V1/3
09ae-HD/PM/HDCV/FSOFT Internal use 7/30
* CR# PROJECT 
* Description 
* 
* -- 

*******************************************************************
*****/
#include <SoftwareSerial.h>
#include "XBeePro.h"
extern SoftwareSerial xb;

/**********************************************************
********
*++
* Method name : XBeePro::XBeePro(void)
* Decription  : This method used to set information of the XBeePro
* 
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* Parameter : no
* Global available : no
* Return value : no
*               
*--
***********************************************************
********/
XBeePro::XBeePro(void)
{
	}
	
/**********************************************************
********
*++
* Method name : XBeePro::setupXbee()
* Decription  : This method used to set up begin for the xb
* 
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* Parameter : no
* Global available : no
* Return value : no
*               
*--
***********************************************************
********/
void XBeePro::setupXbee(){
	xb.begin(9600);
}

/**********************************************************
********
*++
* Method name : XBeePro::receiveData()
* Decription  : This method used to receive data from xbee connected Raspberry
* 
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* Parameter : no
* Global available : no
* Return value : data
*               
*--
***********************************************************
********/
String XBeePro::receiveData()
{
  String data = "";
  while(1)
  {
    if(xb.available()){
        char receiveChar = xb.read();
        if (receiveChar == '#') break;
        data += receiveChar;   
        }
  }
    return data;
  }

/**********************************************************
********
*++
* Method name : XBeePro::sendData(String data)
* Decription  : This method used to send data to xbee connected Raspberry
* 
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* Parameter : data
* Global available : no
* Return value : no
*               
*--
***********************************************************
********/
void XBeePro::sendData(String data) {
  int lastStringLength = data.length();

  for(int i = 0; i < lastStringLength; i++)
  {
     xb.write(data.charAt(i));
    }
}

/**********************************************************
********
*++
* Method name : XBeePro::checkAvailable()
* Decription  : This method used to check state of xbee
* 
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* Parameter : no
* Global available : no
* Return value : true/false
*               
*--
***********************************************************
********/
bool XBeePro::checkAvailable() {
	if(xb.available()){
		return true;
	}
	return false;
}