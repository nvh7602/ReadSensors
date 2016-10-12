#ifndef XBeePro_H
#define XBeePro_H

/**********************************************************
********
*++
* Author : Nguyen Dang Nhut Tan
* Module : XBeePro.h
* 
* Decription : Class XBeePro
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
* Mod.History : 22/9/2016 - Created
*                            Xbee Communication
*                            Decription : Xbee Communication 
*                                         
***********************************************************
********/
class XBeePro{
	public:
	
		// This function used to initialize object
		XBeePro(void);
		
		// This function used to setup xbee
		void setupXbee();
		
		// This function used to receive data from xbee connected Raspberry
		String receiveData();
		
		// This function used to send data to xbee connected Raspberry
		void sendData(String data);
		
		// This function used to check state of xbee
		bool checkAvailable();
};

#endif

