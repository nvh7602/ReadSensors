/*Standard data: Data(Time, NodeID, AirTemperature, AirHumidity, SoilTemperature, SoilMoisture, LightIntensity)
 * 
 * Time: Thời gian mỗi lần mà nút ghi/gửi dữ liệu, kiểu định dạng là "hh:mm:ss".
 * NodeID(Node identity): Địa chỉ của các nút dữ liệu. Mỗi nút có một ID khác nhau.
 * AirTemperature: Nhiệt độ của không khí, giá trị lưu trữ nằm trong khoảng 0 - 255.
 * AirHumidity: Độ ẩm của không khí, giá trị lưu trữ nằm trong khoảng 0 - 255.
 * SoilTemperature: Nhiệt độ đất, giá trị lưu trữ nằm trong khoảng 0 - 255.
 * SoilMoisture: Độ ẩm đất, giá trị lưu trữ nằm trong khoảng 0 - 255.
 * LightIntensity: Cường độ ánh sáng(Lux), giá trị đo được nằm trong khoảng từ 0 đến 65535(lux).
 * 
 * 
Phần WindVelocity, WindDirector, Rain sẽ đưa qua 1 arduino khác.
 *
  Trước mắt thì dữ liệu định dạng như sau: Data(Time, NodeID, WinVelocity, WindDirection, Rain)
 * WindVelocity: Tốc độ gió. (Khoảng đo: N/a)
 * WindDirection: Hướng gió. (Khoảng đo: N/a)
 * Rain: Cảm biến gió. chỉ có giá trị 1(không có mưa) hoặc 0(có mưa).
*/ 

//******************* Include libraries ************************************
#include "CONFIG.h"

//***************** Declare Object ***************************************

DS3231 ds3231;
SDCard sdcard;
DHT dht(DHTPIN, DHT11); 
DS18B20 ds18b20;
BH1750 lightMeter;

SoftwareSerial xBee(RX,TX); // 6,7



// ***************** Declare sensor value ******************************* 

byte NodeId = 123;
byte AirTemperature, AirHumidity, SoilTemperature, SoilMoisture;
unsigned int LightIntensity; // up to 65535 lux
unsigned long time = 0; // Set default time = 0

// Tân
int lastStringLength;
int i;

// End: Tân



//***************** Starting set up *****************************************
void setup()
{
  Wire.begin(); // SDCard
  Serial.begin(9600);
  xBee.begin(9600); // Start xbee 9600
  //ds3231.setDateTime(4,28,9,16,22,16,50); // ******* Note: Set time for DS3231
  dht.begin(); // Start DHT sensor
  lightMeter.begin(); //Start BH1750
  


//************** Install SD card ****************************************
  sdcard.setUpSDCard(); // set CS pin (pin 10) for SD Card Reader
}



//***********************************************************************
void loop()
{
  
  
//***************** Write data to SD Card and Send data to xbee *******************************

  if((unsigned long)(millis() - time) > 5000)
  {
    AirTemperature = dht.readTemperature(); // Read data from DHT sensor
    AirHumidity = dht.readHumidity(); // Read data from DHT sensor
    SoilTemperature = ds18b20.getGndTemp(); // Read data from DS18B20 ( temp of soil)
    SoilMoisture = map(analogRead(A3), 0, 1023, 0, 255); // Read data from soilSensor 
    LightIntensity = lightMeter.readLight();  // Read data from light sensor 


      String dataW = "";
      dataW += ds3231.getTime();
      dataW += ",";
      dataW += NodeId;
      dataW += ",";
      dataW += AirTemperature;
      dataW += ",";
      dataW += AirHumidity;
      dataW += ",";
      dataW += SoilTemperature;
      dataW += ",";
      dataW += SoilMoisture;
      dataW += ",";
      dataW += LightIntensity;
    
      String dataS = "(";
      dataS += dataW;
      dataS += ")";
    
      sdcard.writeData(dataW, ds3231.getDayName());  // Write data to SD Card
      sendStatus(dataS); // Send data to Xbee
   
  
    time = millis();
  }
}


//Tân
void sendStatus(String dataSend) {
  lastStringLength = dataSend.length();

  for(i = 0; i < lastStringLength; i++)
  {
     xBee.write(dataSend.charAt(i));
     Serial.print(dataSend.charAt(i));
    }
    Serial.println("");
}

String receiveStatus()
{
  //Serial.println("begin receive");
  String dataReceive;
  while(1)
  {
    if(xBee.available()){
        char receiveChar = xBee.read();
        if (receiveChar == '#') break;
        dataReceive += receiveChar;   
        }
  }
    //Serial.println("end receive");
    return dataReceive;
}

//End: Tân


