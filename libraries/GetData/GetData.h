#include <Arduino.h>

typedef unsigned char byte;

class GetData{
	public:
		String dataW = "";
	public:
		GetData();
		String dataSend();
		String dataWrite(String , byte, byte, byte, byte, byte , unsigned int); // (time, nodeId, AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity) 
	
};
