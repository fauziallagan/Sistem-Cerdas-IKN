  /*
    Serial Value 1 = AREA
    Serial Value 2 = MOISTURE PERCENT
*/

/*
    LoRa Value 1 = AREA
    LoRa Value 2 = MOISTURE PERCENT
    LoRa Value 3 = pH (0-14)
    LoRa Value 4 = NITROGEN (MG/KG)
    LoRa Value 5 = PHOSPHORUS (MG/KG)
    LoRa Value 6 = KALIUM/POTASSIUM (MG/KG)
*/

#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

#include <SPI.h>
#include <LoRa.h>
// Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <elapsedMillis.h>



#define BAND 915E6
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


elapsedMillis sendMillis;
elapsedMillis OLEDPrintMillis;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

unsigned long sendInterval = 2000;
unsigned long OLEDPrintInterval = 3000;

String target1 = "L1";
String target2 = "L2";
String target3 = "L3";
String target4 = "L4";

byte turn;

String
moist1,
pH1, nitro1, phos1, kal1,
moist2, pH2, nitro2, phos2, kal2,
moist3, pH3, nitro3, phos3, kal3,
moist4, pH4, nitro4, phos4, kal4;



String area1OLED = "Lahan 1 : ";
String area2OLED = "Lahan 2 : ";
String area3OLED = "Lahan 3 : ";
String area4OLED = "Lahan 4 : ";

const char* ssid     = "muro";
const char* password = "Piscok2000";
const char* serverName = "http://192.168.0.110/webserver/post-data.php";
String apiKeyValue = "esp";
void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  // SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  // setup LoRa transceiver module  
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  Serial.println("LoRa Initializing OK!");
  //
    initLoRa();
    initOLED();


}

void loop()
{

//  webserverStatus();
//    systemRun();
waitResponse();

}
void systemRun() {
  if (sendMillis >= sendInterval)
  {
    if (turn == 1)
    {
      sendRequest(target1);
      turn = 2;
    }
    else if (turn == 2)
    {
      sendRequest(target2);
      turn = 3;
    }
    else if (turn == 3)
    {
      sendRequest(target3);
      turn = 4;
    }
    else if (turn == 4)
    {
      sendRequest(target4);
      turn = 1;
    }
    sendMillis = 0;
  }

  if (OLEDPrintMillis >= OLEDPrintInterval)
  {
    OLEDPrint();

    OLEDPrintMillis = 0;
  }


  waitResponse();
}
void initLoRa()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(BAND))
  {
    while (1)
      ;
  }
}

void initOLED()
{
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  {
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA Starting... ");
  display.display();
  display.clearDisplay();
}

void sendRequest(String target)
{
  LoRa.beginPacket();
  LoRa.print(target);
  LoRa.print("#");
  LoRa.endPacket();
}

void OLEDPrint()
{
  moistOLED(moist1, 10, area1OLED);
  moistOLED(moist2, 20, area2OLED);
  moistOLED(moist3, 30, area3OLED);
  moistOLED(moist4, 40, area4OLED);
}

void moistOLED(String msgMoist, int YValue, String areaName)
{
  int moisturePercent = msgMoist.toInt();
  if (moisturePercent >= 0 && moisturePercent <= 100)
  {
    display.setCursor(30, 0);
    display.print("Lora SERVER");
    display.setCursor(0, 50);
    display.print("IP 192.168.4.1");
    display.display();
//    Serial.println(moisturePercent);
    if (moisturePercent >= 0 && moisturePercent <= 40)
    {

//      Serial.println(moisturePercent);
      display.setCursor(0, YValue);
      display.setTextSize(1);
      display.println(areaName);
      display.setCursor(50, YValue);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("KERING");
      display.display();

    }

    else if (moisturePercent >= 41 && moisturePercent <= 100)
    {
      display.setCursor(0, YValue);
      display.setTextSize(1);
      display.println(areaName);
      display.setCursor(50, YValue);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("BASAH");
      display.display();
      display.clearDisplay();
      delay(200);
    }

  }

}

void waitResponse()
{
  String area, moist, pH, nitro, phos, kal;
  int packetSize = LoRa.parsePacket();
  while (LoRa.available())
  {
  if (!packetSize == 0) 
    {
     Serial.println("this packet is Zero!");
    }
     area = LoRa.readStringUntil('#');
      moist = LoRa.readStringUntil('#');
      pH = LoRa.readStringUntil('#');
      nitro = LoRa.readStringUntil('#');
      phos = LoRa.readStringUntil('#');
      kal = LoRa.readStringUntil('#');
      sensorSend(area, moist, pH, nitro, phos, kal);
      webserver(area, moist, pH, nitro, phos, kal);
  } 
}


void webserver(String nama, String kelembaban, String n, String p, String k, String ph) {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
//    String nama = "Lora";
//    int kelembaban1 = random(100);
//    int np= random(255);
//    int pn=random(255);
//    int kn =random(255);
//    int php = random(12);
//
//    String kelembaban = String(kelembaban1);
//    String n = String(np);
//    String p = String(pn);
//    String k = String(kn);
//    String ph = String(php);
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&nama=" + nama+"&sensor_kelembaban=" + kelembaban + "&sensor_n=" + n+ "&sensor_p=" + p+ "&sensor_k=" + k+ "&sensor_ph=" + ph;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(1000);  

}
void sensorSend(String field, String sensor1, String sensor2, String sensor3, String sensor4, String sensor5)
{
  if (field == target1)
  {
    moist1 = sensor1;
    pH1 = sensor2;
    nitro1 = sensor3;
    phos1 = sensor4;
    kal1 = sensor5;
    sendSerial(field, moist1);
    Serial.println("Lahan 1 : ");
  }
  else if (field == target2)
  {
    moist2 = sensor1;
    pH2 = sensor2;
    nitro2 = sensor3;
    phos2 = sensor4;
    kal2 = sensor5;
    sendSerial(field, moist2);
    Serial.println("Lahan 2 : ");
  }
  else if (field == target3)
  {
    moist3 = sensor1;
    pH3 = sensor2;
    nitro3 = sensor3;
    phos3 = sensor4;
    kal3 = sensor5;
    sendSerial(field, moist3);
    Serial.println("Lahan 3 : ");

  }
  else if (field == target4)
  {
    moist4 = sensor1;
    pH4 = sensor2;
    nitro4 = sensor3;
    phos4 = sensor4;
    kal4 = sensor5;
    sendSerial(field, moist4);
    Serial.println("Lahan 4 : ");
  }
}

void sendSerial(String field, String sensor1)
{
  Serial.print(field);
  Serial.print('#');
  Serial.print(sensor1);
  Serial.print('#');
}
