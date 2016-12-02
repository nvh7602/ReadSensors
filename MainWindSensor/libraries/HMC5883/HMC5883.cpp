#include "HMC5883.h"

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

HMC5883::HMC5883()
{
	
}
void HMC5883::setUp()
{
	Serial.println("Setup HMC5883....");
	if(!mag.begin())
  	{
	    /* There was a problem detecting the HMC5883 ... check your connections */
	    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
	    while(1);
	    
  	}
}
	

int HMC5883::getAngle()
{
	float headingDegrees = 0;
	/* Get a new sensor event */ 
	sensors_event_t event; 
	mag.getEvent(&event);
	
	float heading = atan2(event.magnetic.y, event.magnetic.x);
	float declinationAngle = 0.22;
  	heading += declinationAngle;
  
	// Correct for when signs are reversed.
	if(heading < 0)
	    heading += 2*PI;
	    
	// Check for wrap due to addition of declination.
	if(heading > 2*PI)
		heading -= 2*PI;
   
  	// Convert radians to degrees for readability.
  	headingDegrees = heading * 180/M_PI; 
  	return headingDegrees;
  	
}

byte HMC5883::getDirection()
{
	int ag = getAngle();
	if(ag > 11.25 && ag <= 33.77) 		return 1;	// NNE
	if(ag > 33.77 && ag <= 60.27) 		return 2;	// NE
	if(ag > 60.27 && ag <= 82.77) 		return 3; 	// ENE
	if(ag > 82.77 && ag <= 105.27) 		return 4; 	// E
	if(ag > 105.27 && ag <= 127.77) 	return 5; 	// SES
	if(ag > 127.77 && ag <= 150.27 ) 	return 6; 	// SE
	if(ag > 150.27 && ag <= 172.77) 	return 7; 	// SSE
	if(ag > 172.77 && ag <= 195.27) 	return 8; 	// S
	if(ag > 195.27 && ag <= 217.77) 	return 9; 	// SSW
	if(ag > 217.77 && ag <= 240.27) 	return 10; 	// SW
	if(ag > 240.27 && ag <= 262.77) 	return 11; 	// WSW
	if(ag > 262.77 && ag <= 285.27) 	return 12; 	// W
	if(ag > 285.27 && ag <= 307.77) 	return 13; 	// WNW
	if(ag > 307.77 && ag <= 330.27) 	return 14; 	// NW
	if(ag > 330.27 && ag <= 352.77) 	return 15; 	// NNW
	else 								return 16; 	// N
}
