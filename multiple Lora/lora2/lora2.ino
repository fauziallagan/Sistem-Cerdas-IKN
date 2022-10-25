#include <SPI.h>
#include <LoRa.h>

#define CS 10
#define RESET 9
#define DIO0 2


String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;
byte Node2 = 0xCC;

float temperature;
float humidity;

String Mymessage = "";
String incoming = "";

 
void setup() 
{    
    Serial.begin(115200);
 
    if (!LoRa.begin(915E6)) 
    {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}
 
void loop() 
{
 temperature = 1024;
  humidity = 1023;


  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    // Serial.println("error: message length does not match length");
    ;
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node2 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
  Serial.println(incoming);
  int Val = incoming.toInt();
  if (Val == 20)
  {
    Mymessage = Mymessage + temperature + "," + humidity;
    sendMessage(Mymessage, MasterNode, Node2);
    delay(100);
    Mymessage = "";
  }

}

void sendMessage(String outgoing, byte MasterNode, byte Node2) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node2);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
