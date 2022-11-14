/*
  * LORA32 to Soil Moisture Sensor
  * VCC = 3.3V
  * Data = 25
  * GND = GND
*/

/*
  * LORA32 to pH Sensor
  * Data = 4
  * GND = GND
*/

/*
  * LORA32 to MAX485 
  * VCC = 5V
  * GND = GND
  * (Through Logic-Level Converter)
  * DI (HV4)  = TX (LV4)
  * DE (HV3)  = 32 (LV3)
  * RE (HV2)  = 33 (LV2)
  * RO (HV1)  = RX (LV1)
  * HV        = 5V
  * LV        = 3.3V
  * GND       = GND   
*/

/*
  * NPK Sensor to MAX485
  * Brown   = 5-30V (Adaptor/Step-Down)
  * Black   = GND   (Adaptor/Step-Down)
  * Yellow  = A
  * Blue    = B
*/

#include <SPI.h>
#include <LoRa.h>
#include <elapsedMillis.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BAND  915E6
#define SCK   5
#define MISO  19
#define MOSI  27
#define SS    18
#define RST   14
#define DIO0  26

#define OLED_SDA      21
#define OLED_SCL      22
#define OLED_RST      16
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64 

#define soil        25
#define pH          4
#define DE_MAX485   32
#define RE_MAX485   33

elapsedMillis readSensorMillis;
elapsedMillis OLEDPrintMillis;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

const byte nitro[]  = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[]   = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte kal[]    = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[11];

int moistAnalog, moistPercent;
int pHAnalog; float pHValue;
byte NValue, PValue, KValue;
byte NPKReading = 1;  
byte LoRaStatus = 0;

String area     = "L4";
String areaDesc = "LoRa Lahan 4";
String areaName = "Lahan 4";
String msgArea, msgMoist, msgPH, msgNitro, msgPhos, msgKal;

unsigned long readSensorInterval = 10000;
unsigned long OLEDPrintInterval = 5000;

int x, minX;
char message[] = "29M LstWyPt, 345M StPt, rec#89";

void setup()
{
  Serial.begin(4800);
  
  pinMode (OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  initLoRa();
  initSensor();
  initOLED();

  delay (3000);
}

void loop() 
{ 
  if (LoRaStatus == 1) 
  {
    int packetSize = LoRa.parsePacket();
    if (packetSize) 
    {
      while (LoRa.available()) 
      {
        msgArea = LoRa.readStringUntil ('#');
        if (msgArea == area)
        {
          onResponse(msgArea, msgMoist, msgPH, msgNitro, msgPhos, msgKal);
        }
      }
    }
  }
  else
  {
    //Do Nothing
  }
  
  if (readSensorMillis >= readSensorInterval)
  {
    if (NPKReading == 1)
    {
      NPKRead();
      NPKReading = 0; 
      LoRaStatus = 1;     
    }
    else
    {
      //Do Nothing
    }
    pHRead();
    moistRead();
    readSensorMillis = 0;
  }

  if (OLEDPrintMillis >= OLEDPrintInterval)
  {
    OLEDPrint();
    OLEDPrintMillis = 0;
  }  
}

void initLoRa()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(BAND))
  {
    while (1);
  }
}

void initSensor()
{
  pinMode (soil, INPUT);
  pinMode (pH, INPUT);
  pinMode (DE_MAX485, OUTPUT);
  pinMode (RE_MAX485, OUTPUT);

  digitalWrite (DE_MAX485, LOW);
  digitalWrite (RE_MAX485, LOW);  
}

void initOLED()
{
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  { 
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(areaDesc);
  display.display();
}

void onResponse(String field, String sensor1, String sensor2, String sensor3, String sensor4, String sensor5)
{
  LoRa.beginPacket ();
  LoRa.print (field);
  LoRa.print ("#");
  LoRa.print (sensor1);
  LoRa.print ("#");
  LoRa.print (sensor2);
  LoRa.print ("#");
  LoRa.print (sensor3);
  LoRa.print ("#");
  LoRa.print (sensor4);
  LoRa.print ("#");
  LoRa.print (sensor5);
  LoRa.print ("#");
  LoRa.endPacket ();
}
void pHRead()
{
  pHAnalog  = analogRead (pH);
  pHValue   = (0.002888143 * pHAnalog)-0.50;
  msgPH     = String (pHValue);
}

void moistRead()
{
  moistAnalog   = analogRead (soil);
  moistPercent  = map (moistAnalog, 1620, 4095, 100, 0);
  msgMoist      = String (moistPercent);
}

void NPKRead()
{
  NValue = NRead();
  delay (250);
  PValue = PRead();
  delay (250);
  KValue = KRead();
  delay (250);
  msgNitro  = String (NValue, HEX);
  msgPhos   = String (PValue, HEX);
  msgKal    = String (KValue, HEX);
}

byte NRead()
{
  // clear the receive buffer
  Serial.flush();

  // switch RS-485 to transmit mode
  digitalWrite(DE_MAX485, HIGH);
  digitalWrite(RE_MAX485, HIGH);
  delay(10);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++) Serial.write(nitro[i]);

  // wait for the transmission to complete
  Serial.flush();

  // switching RS485 to receive mode
  digitalWrite(DE_MAX485, LOW);
  digitalWrite(RE_MAX485, LOW);

  // delay to allow response bytes to be received!
  delay(500);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte PRead()
{
  Serial.flush();
  digitalWrite(DE_MAX485, HIGH);
  digitalWrite(RE_MAX485, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(phos); i++) Serial.write(phos[i]);

  Serial.flush();
  digitalWrite(DE_MAX485, LOW);
  digitalWrite(RE_MAX485, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte KRead()
{
  Serial.flush();
  digitalWrite(DE_MAX485, HIGH);
  digitalWrite(RE_MAX485, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(kal); i++) Serial.write(kal[i]);

  Serial.flush();
  digitalWrite(DE_MAX485, LOW);
  digitalWrite(RE_MAX485, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

void OLEDPrint()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(areaName);
  display.setCursor(0, 10);
  display.print("N : ");
  display.setCursor(0, 20);
  display.print("P : ");
  display.setCursor(0, 30);
  display.print("K : ");
  display.setCursor(0, 40);
  display.print("pH : ");
  display.setCursor(0, 50);
  display.print("Soil : ");

  display.setCursor(40, 10);
  display.print(NValue);
  display.setCursor(40, 20);
  display.print(PValue);
  display.setCursor(40, 30);
  display.print(KValue);
  display.setCursor(40, 40);
  display.print(pHValue);
  display.setCursor(40, 50);
  display.print(moistPercent);

  display.display();

/*
  * Dynamic Variables:
  * moistPercent
  * pHValue
  * NValue
  * PValue
  * KValue
*/
}
