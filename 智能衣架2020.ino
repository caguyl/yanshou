/*-----------------------------------WiFi Start-----------------------------------*/
#include <Wire.h>                                  //调用库  
#include <ESP8266.h>
#include <I2Cdev.h>                                //调用库
#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 
#define EspSerial mySerial
#define UARTSPEED  9600
#endif
#define SSID        F("neopmao")//WIFI名
#define PASSWORD    F("BK6180DK1")//WIFI密码
#define HOST_NAME   ("api.heclouds.com")//域名
#define HOST_PORT   (80)//端口
#define DEVICEID    "578080224"//设备ID
String apiKey = "j3YuAfvJclkAwAi4fWWJ0ZYACDM=";//与你的设备绑定的APIKey

/*-------------------------------------------------------------------------最重要的一行！！！！！-------------------------------------------------------------------------*/
static const byte  GETDATA[]  PROGMEM = {
  "GET http://api.heclouds.com/devices/578080224/datastreams/Movement HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:j3YuAfvJclkAwAi4fWWJ0ZYACDM=\r\nConnection: close\r\n\r\n"
};//向Flash中写入数据，节省内存
ESP8266 wifi(&EspSerial);
/*-----------------------------------WiFi Finish------------------------------------*/

/*-----------------------------------Motor Start------------------------------------*/
#include <Microduino_Motor.h>
Motor MotorLeft(MOTOR0_PINA, MOTOR0_PINB);
/*-----------------------------------Motor Finish-----------------------------------*/

/*-----------------------------------Sensor Start-----------------------------------*/
#include <Microduino_SHT2x.h>         //Tem & Hum Sensor
#define INTERVAL_SENSOR   5000        //定义传感器采样时间间隔
#define INTERVAL_NET      5000        //定义发送时间
String jsonToSend;
char buf[10];
float sensor_tem, sensor_hum;                    //传感器湿度   
char  sensor_tem_c[7], sensor_hum_c[7];    //换成char数组传输
unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器

//int SensorData;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数

Tem_Hum_S2 TempMonitor;
/*-----------------------------------Sensor Finish----------------------------------*/

void setup() {
/*-----------------------------------Motor Start------------------------------------*/
  MotorLeft.begin();
/*-----------------------------------Motor Finish-----------------------------------*/

/*-----------------------------------WiFi Start---------------------------------------
---------------------------------连接WiFi，同课上内容-------------------------------*/
  Wire.begin();
    Serial.begin(115200);
    while (!Serial); 
    Serial.print(F("setup begin\r\n"));
    delay(100);

  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

if(wifi.disableMUX()) {
Serial.print("single ok\r\n");
} else {
Serial.print("single err\r\n");
}

Serial.print("setup end\r\n");
}
/*-----------------------------------WiFi Finish----------------------------------*/

void loop(void)
{
/*-----------------------------------WiFi Start-----------------------------------*/
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print(F("create tcp ok\r\n"));
  } else {
    Serial.print(F("create tcp err\r\n"));
  }

  wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //上传数据，获取API返值

  uint8_t buffer[512] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 500);
  if (len > 0) {
    Serial.print(F("Received:["));
    /*---注释部分：查看并确认返回的每个字符具体内容---*/
    /*for (uint32_t i = 0; i < len; i++) {
      Serial.print("i:");
      Serial.print(i);
      Serial.print("=");
      Serial.println((char)buffer[i]);
    }*/
    Serial.print((char)buffer[286]);
    Serial.print(F("]\r\n"));
  }
/*---------------通过湿度传感器数据判断衣物是否干燥------------------------------------
   if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {  
    getSensorData();                                        //读串口中的传感器数据
    sensor_time = millis();
    delay(500);
  }  
    
  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //发送数据时间间隔
  {                
    updateSensorData();                                     //将数据上传到服务器的函数
    net_time1 = millis();
    delay(500);
  }
/*-----------------------------------------------------*/

  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));   //释放TCP
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
  delay(10);
/*-----------------------------------WiFi Finish-----------------------------------*/

/*-----------------------------------Motor Start-----------------------------------*/
  if((char)buffer[286]==117)
  {
     Serial.println("UP! UP!! UP!!!");
     MotorLeft.setSpeed(100);
  }
  else if((char)buffer[286]==115)
  {
     Serial.println("STOP! STOP!! STOP!!!");
     MotorLeft.setSpeed(FREE);
  }
  else if((char)buffer[286]==100)
  {
     Serial.println("DOWN! DOWN!! DOWN!!!");
     MotorLeft.setSpeed(100);
     delay(100);
  }
/*-----------------------------------Motor Finish-----------------------------------*/
}
/*-----------------------------------Sensor Start-----------------------------------*/
void getSensorData(){   
    sensor_hum = TempMonitor.getHumidity();   
    delay(10);
    dtostrf(sensor_hum, 2, 1, sensor_hum_c);
}
void updateSensorData() {

jsonToSend="{\"Humidity\":";
    dtostrf(sensor_hum,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";
    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("Hum send success");
}
/*-----------------------------------Sensor Finish-----------------------------------*/
