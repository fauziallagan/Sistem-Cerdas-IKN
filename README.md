# Sistem-Cerdas-IKN
Run Website -> 192.168.4.1

# Schematic Lora Server
![Logo](https://github.com/multimedia-dan-robotika/Sistem-Cerdas-IKN/blob/main/skematikServerFinal.png)

# Schematic Lora Client
![Logo](https://github.com/multimedia-dan-robotika/Sistem-Cerdas-IKN/blob/main/schematicFinalloraSensors.PNG)

# Documentation

## Lora TTGO T-Beam Pin Use to Logic Converter

| PIN Lora | Type     | Pin Logic Converter           | Pin Mega| 
| :-------- | :------- | :------------------------- |  :------- |
| `RX` | `Communication Serial` |  `RX`|`TX` |
| `TX` | `Communication Serial` | `TX` |`RX`|
| `GND` | `Ground` | `GND` |`GND`|
| `3.3V` | `VCC` | `LV/HV` | `5V`|


## Example Receiver arduino Mega -> Logic Converter <- Lora TTGO

```c++
  void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(9600);
}
void loop() {
  Serial.println("Message Received: ");
  Serial.println(Serial3.readString());
```
## Example SEND packet lora client to Server or other Lora
```c++
 LoRa.beginPacket();
  LoRa.print(soilmoisturepercent);
  LoRa.print('#');
  LoRa.print(rssi);
  LoRa.print('#');
  LoRa.endPacket();
```
## Example RECEIVER packet from lora Client or other Lora
```c++

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
```
## Example Lora Web Server
Using port 80 for running webserver
```c++
AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;
```
Reading data from sensor and change data tipe from integer to String
```c++
String getSensorReadings()
{
  readings["kelembabanLahan1FromArduino"] = String(moist1); // edited
  readings["natriumLahan1FromArduino"] = String(nitro1);
  readings["potasiumLahan1FromArduino"] = String(phos1);
  readings["kaliumLahan1FromArduino"] = String(kal1);
  readings["phLahan1FromArduino"] = String(pH1);
   String jsonString = JSON.stringify(readings);
  return jsonString;
```
example run server and request webserver using SPIFFS and execute file index.html
```c++
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getSensorReadings();
    request -> send (200, "application/json", json);
    json = String(); });

  events.onConnect([](AsyncEventSourceClient *client)
                   { client->send("Request -> ", NULL, millis(), 10000); });

  server.addHandler(&events);

  server.begin();
```

## Rumus pH Tanah
```c++
void pHRead()
{
  pHAnalog  = analogRead (pH);
  pHValue   = (0.002888143 * pHAnalog)-0.50;
  msgPH     = String (pHValue);
}
```

## Rumus Soil Moisture
```c++
void moistRead()
{
  moistAnalog   = analogRead (soil);
  moistPercent  = map (moistAnalog, 1620, 4095, 100, 0);
  msgMoist      = String (moistPercent);
}
```

## Rumus NPK

```c++

const byte nitro[]  = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[]   = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte kal[]    = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte NValue, PValue, KValue;
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

```

