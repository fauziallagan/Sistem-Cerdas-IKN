/*
   Serial Value 1 = AREA
   Serial Value 2 = MOISTURE PERCENT
*/
#include <elapsedMillis.h>
#include <DS3231.h>

#define WLAtas 9
#define WLBawah 8

#define relayBawahIN1 7 // 2
#define relayBawahIN2 3 // 3
#define relayBawahIN3 6 // 6
#define relayBawahIN4 2 // 7
#define relayAtasIN1  4 // 4
#define relayAtasIN2  5 // 5

DS3231 rtc(SDA, SCL);
Time t;
elapsedMillis readSensorMillis;
elapsedMillis areaRelayOutputMillis;
elapsedMillis reservoirRelayOutputMillis;
elapsedMillis RTCMillis;

unsigned long readSensorInterval            = 3000; // 3000  3 detik
unsigned long areaRelayOutputInterval       = 2000; // 2000 2 detik
unsigned long reservoirRelayOutputInterval  = 2000; // 2000 2 detik
unsigned long RTCInterval                   = 10000; // 10000 10 detik

String value1, value2;
String moist1, moist2, moist3, moist4;
String area1    = "L1";
String area2    = "L2";
String area3    = "L3";
String area4    = "L4";
String message  = "";
String statusSensorsOn = "ON";
String statusSensorsOff = "OFF";
//String statusAllSensors = "";

int WLBawahRead, WLAtasRead;

String dataReciver;

const byte onHour   = 13;
const byte onMin    = 40;
const byte offHour  = 13;
const byte offMin   = 43;

void setup()
{
  Serial.begin (115200);
  Serial3.begin (9600); // 9600
  rtc.begin();

  pinMode(WLAtas, INPUT);
  pinMode(WLBawah, INPUT);

  pinMode(relayBawahIN1, OUTPUT);
  pinMode(relayBawahIN2, OUTPUT);
  pinMode(relayBawahIN3, OUTPUT);
  pinMode(relayBawahIN4, OUTPUT);
  pinMode(relayAtasIN1, OUTPUT);
  pinMode(relayAtasIN2, OUTPUT);


//  rtc.setTime(16, 00, 0);
//  rtc.setDate(5, 11, 2022);
}

void loop()
{

  //  Serial.println("data Receiver :");
  //  dataReciver = Serial3.readString();
  if (Serial3.available())
  {
    while (Serial3.available() > 0)
    {
      value1 = Serial3.readStringUntil ('#');
      value2 = Serial3.readStringUntil ('#');
      sensorMap (value1, value2);
    }

  }

  if (readSensorMillis >= readSensorInterval) // 
  {
   reservoirRelayOutput();
    readSensorMillis = 0;
  }

  if (areaRelayOutputMillis >= areaRelayOutputInterval)
  {
    areaRelayOutput (moist1, relayBawahIN1, area1);
    areaRelayOutput (moist2, relayBawahIN2, area2);
    areaRelayOutput (moist3, relayBawahIN3, area3);
    areaRelayOutput (moist4, relayBawahIN4, area4);
    areaRelayOutputMillis = 0;
  }

  if (reservoirRelayOutputMillis >= reservoirRelayOutputInterval)
  {
    reservoirRelayOutput();
    reservoirRelayOutputMillis = 0;
  }

  if (RTCMillis >= RTCInterval)
  {
//    areaRelayOutput()
    RTCRead (moist1, relayBawahIN1);
    RTCRead (moist2, relayBawahIN2);
    RTCRead (moist3, relayBawahIN3);
    RTCRead (moist4, relayBawahIN4);
    RTCMillis = 0;
  }
}


void sensorMap (String area, String sensor)
{
  if (area == area1)
  {
    moist1 = sensor;
  }
  else if (area == area2)
  {
    moist2 = sensor;
  }
  else if (area == area3)
  {
    moist3 = sensor;
  }
  else if (area == area4)
  {
    moist4 = sensor;
  }
}


void areaRelayOutput (String moistValue, int relayArea, String area)
{
  int moistPercentA = moistValue.toInt();
  Serial.print(area +" = ");
  Serial.println(moistPercentA);

if (moistPercentA >= 1 && moistPercentA <= 40)
  {
    //Turn ON Designated Area Relay
    digitalWrite(relayArea, LOW); //high
    //    Serial.println(statusSensorsOn);
  }
  else if (moistPercentA >= 41) {
    digitalWrite(relayArea, HIGH); // low

  }


}

void reservoirRelayOutput ()
{
   WLAtasRead    = digitalRead (WLAtas);
  WLBawahRead   = digitalRead (WLBawah);
  if (WLBawahRead == 0)
  {
    //Turn OFF Pump and Strobe
    digitalWrite(relayAtasIN1,LOW);
    Serial.println("Status : Pompa Nyala");
    digitalWrite(relayAtasIN2, LOW);
  }
  else if (WLAtasRead == 1) {
    digitalWrite(relayAtasIN1, HIGH);
    Serial.println("Status : Pompa Mati");
    digitalWrite(relayAtasIN2, HIGH);
  }
//  else {
//    //Turn ON Pump and Strobe
//    digitalWrite(relayAtasIN1, LOW);
//    Serial.println("Value WLBawah : HIGH");
//    digitalWrite(relayAtasIN2, HIGH);
//  }


}
void RTCRead(String moistValue, int relayArea)
{
  t = rtc.getTime();
  
//  int moistPercentB = moistValue.toInt(); // use if need soil value

  
    if (t.hour == onHour && t.min == onMin)
    {
      digitalWrite(relayArea, HIGH);
      Serial.println("RTC ON");
    }
    else if (t.hour == offHour && t.min == offMin)
    {
      digitalWrite(relayArea, LOW);
      Serial.println("RTC OFF");
    }
  
}
