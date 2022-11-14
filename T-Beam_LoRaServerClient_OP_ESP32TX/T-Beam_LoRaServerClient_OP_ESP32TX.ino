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
#include "SPIFFS.h"

#include <SPI.h>
#include <LoRa.h>
// Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiAP.h>
//#include <Hash.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <elapsedMillis.h>

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

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

const char *ssid = "Smart Monitor";
const char *password = "PPURSTUG";

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

AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;
elapsedMillis requestMillis;
elapsedMillis OLEDMillis;
elapsedMillis serialMillis;
unsigned int requestInterval = 3000;
unsigned int OLEDInterval = 1000;
unsigned int serialInterval = 1500;

void initFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Gagal");
  }

  Serial.println("SPIFFS Sukses");
}

String getSensorReadings()
{
  readings["kelembabanLahan1FromArduino"] = String(moist1); // edited
  readings["natriumLahan1FromArduino"] = String(nitro1);
  readings["potasiumLahan1FromArduino"] = String(phos1);
  readings["kaliumLahan1FromArduino"] = String(kal1);
  readings["phLahan1FromArduino"] = String(pH1);
  // section 2
  readings["kelembabanLahan2FromArduino"] = String(moist2); // edited
  readings["natriumLahan2FromArduino"] = String(nitro2);
  readings["potasiumLahan2FromArduino"] = String(phos2);
  readings["kaliumLahan2FromArduino"] = String(kal2);
  readings["phLahan2FromArduino"] = String(pH2);
  // Section 3
  readings["kelembabanLahan3FromArduino"] = String(moist3); // edited
  readings["natriumLahan3FromArduino"] = String(nitro3);
  readings["potasiumLahan3FromArduino"] = String(phos3);
  readings["kaliumLahan3FromArduino"] = String(kal3);
  readings["phLahan3FromArduino"] = String(pH3);
  // section 4
  readings["kelembabanLahan4FromArduino"] = String(moist4); // edited
  readings["natriumLahan4FromArduino"] = String(nitro4);
  readings["potasiumLahan4FromArduino"] = String(phos4);
  readings["kaliumLahan4FromArduino"] = String(kal4);
  readings["phLahan4FromArduino"] = String(pH4);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void setup()
{
  Serial.begin(9600);
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
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  Serial.println(IP);
  initFS();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    String json = getSensorReadings();
    request -> send (200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient * client)
  {
    client->send("Request -> ", NULL, millis(), 30000); // 1000 change
  });

  server.addHandler(&events);

  server.begin();

  initLoRa();
  initOLED();

  turn = 1;
}

void loop()
{
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
  if (requestMillis >= requestInterval)
  {
    events.send("Success", NULL, 2000);
    events.send(getSensorReadings().c_str(), "new_readings", 2000);
    requestMillis = 0;
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
  display.setCursor(30, 0);
  display.print("Lora SERVER");
  display.setCursor(0, 50);
  display.print("IP 192.168.4.1");
  display.display();
  {
    Serial.println(moisturePercent);
    if (moisturePercent >= 0 && moisturePercent <= 40)
    {
    
      Serial.println(moisturePercent);
      display.setCursor(0, YValue);
      display.setTextSize(1);
      display.println(areaName);
      display.setCursor(50, YValue);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("KERING");
      display.display();

    }
    else if (moisturePercent >= 40 && moisturePercent <= 50)
    {

      display.setCursor(0, YValue);
      display.setTextSize(1);
      display.println(areaName);
      display.setCursor(50, YValue);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("NORMAL");
      display.display();
     
    }
    else if (moisturePercent >= 80 && moisturePercent <= 100)
    {
    
      display.setCursor(0, YValue);
      display.setTextSize(1);
      display.println(areaName);
      display.setCursor(50, YValue);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println("BASAH");
      display.display();
    
    }
  }
}

void waitResponse()
{
  String area, moist, pH, nitro, phos, kal;
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    while (LoRa.available())
    {
      area = LoRa.readStringUntil('#');
      moist = LoRa.readStringUntil('#');
      pH = LoRa.readStringUntil('#');
      nitro = LoRa.readStringUntil('#');
      phos = LoRa.readStringUntil('#');
      kal = LoRa.readStringUntil('#');
      sensorSend(area, moist, pH, nitro, phos, kal);
    }
  }
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
  }
  else if (field == target2)
  {
    moist2 = sensor1;
    pH2 = sensor2;
    nitro2 = sensor3;
    phos2 = sensor4;
    kal2 = sensor5;
    sendSerial(field, moist2);
  }
  else if (field == target3)
  {
    moist3 = sensor1;
    pH3 = sensor2;
    nitro3 = sensor3;
    phos3 = sensor4;
    kal3 = sensor5;
    sendSerial(field, moist3);
  }
  else if (field == target4)
  {
    moist4 = sensor1;
    pH4 = sensor2;
    nitro4 = sensor3;
    phos4 = sensor4;
    kal4 = sensor5;
    sendSerial(field, moist4);
  }
}

void sendSerial(String field, String sensor1)
{
  Serial.print(field);
  Serial.print('#');
  Serial.print(sensor1);
  Serial.print('#');
}
