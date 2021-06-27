#include <SPI.h>              
#include <LoRa.h>
#define pir A0
#define flame A1
        
long unsigned lastSendTime = 0;    
long unsigned interval = 1000;          

struct datasend {
  uint8_t nodeID;
  uint8_t sensorpir;
  uint8_t sensorflame;
  uint8_t konfirmasi;
};

datasend* data_to_send = NULL;
datasend* data_to_recv = NULL;

void setup() {
  Serial.begin(9600);                  
  Serial.println("LoRa Duplex");

  LoRa.setPins(10, 9, 2);
  if (!LoRa.begin(915E6)) {            
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       
  }
  
  Serial.println("LoRa init succeeded.");

  data_to_send = (struct datasend*) malloc (sizeof(struct datasend));
  
  data_to_send->nodeID = 0x01;
  data_to_send->konfirmasi = 0x00;
  
  pinMode(pir, INPUT);
  pinMode(flame, INPUT);
  LoRa.receive();
}

void loop() {
  if (millis() - lastSendTime >= interval) {
    uint8_t logic_pir = digitalRead(pir);
    uint8_t logic_flame = !digitalRead(flame);
    data_to_send->sensorpir = logic_pir;
    data_to_send->sensorflame = logic_flame;
    sendMessage(data_to_send);
    LoRa.receive();
    lastSendTime = millis();
  }
  
  onReceive(LoRa.parsePacket());
}

void sendMessage(struct datasend* data_to_send) {
  
  LoRa.beginPacket();                   
  LoRa.write((uint8_t*) data_to_send, sizeof(struct datasend));                
  LoRa.endPacket();
  
  Serial.println("------------------------------------------");
  Serial.println("Dikirim : ");
  Serial.print("ID : ");
  Serial.println(data_to_send->nodeID);
  Serial.print("PIR : ");
  Serial.println(data_to_send->sensorpir);
  Serial.print("Flame : ");
  Serial.println(data_to_send->sensorflame);
  Serial.print("Konfirmasi : ");
  Serial.println(data_to_send->konfirmasi);                                    
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;
  data_to_recv = (struct datasend*) malloc (sizeof(struct datasend));
  LoRa.readBytes((uint8_t*) data_to_recv, sizeof(struct datasend));
  if(data_to_recv->nodeID == 0 && data_to_recv->konfirmasi == 1) {
    Serial.println("------------------------------------------");
    Serial.println("Diterima : ");
    Serial.print("ID : ");
    Serial.println(data_to_recv->nodeID);
    Serial.print("PIR : ");
    Serial.println(data_to_recv->sensorpir);
    Serial.print("Flame : ");
    Serial.println(data_to_recv->sensorflame);
    Serial.print("Konfirmasi : ");
    Serial.println(data_to_recv->konfirmasi);
    Serial.println("RSSI : " + String(LoRa.packetRssi()));
  }else {
    return;
  }
  
}
