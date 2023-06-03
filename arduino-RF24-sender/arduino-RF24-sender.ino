#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(22, 21); // CE, CSN

const byte address[6] = "00001"; // 设置接收地址，注意发送端和接收端地址要一致

void setup() {
  Serial.begin(9600);
  radio.begin();

  radio.setAutoAck(false); // 禁用自动应答
  radio.setDataRate(RF24_250KBPS); // 设置数据传输速率为250kbps
  radio.setPayloadSize(32); // 设置数据包长度为32字节
  //radio.openWritingPipe(address); // 设置发送地址
  radio.openReadingPipe(1, address); // 设置接收地址
  radio.startListening(); // 开始监听接收通道
  radio.printDetails(); // 输出nRF24L01的配置信息
}

void loop() {
  char data[] = "Hello, World!"; // 待发送的数据

  delay(15);

  //radio.startListening(); // 开启接收模式
  if (radio.available()) { // 检查是否有数据可接收
    char recvData[32];
    radio.read(&recvData, sizeof(recvData)); // 读取接收到的数据
    Serial.print("Received data: ");
    Serial.println(recvData);
    if (strcmp(recvData, data) == 0) { // 检查接收到的数据是否与发送的数据匹配
      Serial.println("Data matched!");
    } else {
      Serial.println("Data not matched!");
    }
  } else {
    Serial.println("No data received.");
  }

  delay(1000);
}
