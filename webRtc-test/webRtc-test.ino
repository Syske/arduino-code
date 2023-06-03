#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  WiFi.begin("matrix-sysker", "sys...0912");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  String ip = WiFi.localIP().toString();
  Serial.println("ip:" + ip);
  udp.begin(1234);
}

void loop() {
  char packetBuffer[255];
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.print("Received packet: ");
    Serial.println(packetBuffer);
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write((const uint8_t*)"ACK", 3);
    udp.endPacket();
  }
}
