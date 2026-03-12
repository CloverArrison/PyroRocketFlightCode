#include "RADIO.h"

int LoRaPin;
myLoRa::myLoRa(int loraPin){
  LoRaPin = loraPin;
}

void myLoRa::LoRaStart() {
  Serial.println("LoRa Sender");
  while (!Serial);
  LoRa.setPins(LoRaPin);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa init");
}
void myLoRa::sendPacket(String packet) {
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
}

String myLoRa::getPacket() {
  // try to parse packet
  String packet;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      packet = (char)LoRa.read() + packet;
    }
    return packet;
  }
  return "no packet";
}