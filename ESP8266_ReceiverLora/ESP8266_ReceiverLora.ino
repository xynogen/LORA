#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#define ss 15
#define rst 16
#define dio0 2


HTTPClient http;
WiFiClient wifiClient;

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* serverName = "http://192.168.43.99:5000/data/";
String node, keterangan;

struct datasend {
  uint8_t nodeID;
  uint8_t sensorpir;
  uint8_t sensorflame;
  uint8_t konfirmasi;
};

long unsigned currentTime = 0;

datasend* data_to_recv = NULL;

IPAddress local_IP(192, 168, 43, 100);
IPAddress gateway(192, 168, 43, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


void setup() {
  Serial.begin(115200);
  //  initialized Buzzer
  pinMode(D1, OUTPUT); 
  data_to_recv = (struct datasend*) malloc (sizeof(struct datasend));

  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  Serial.println("LoRa Initializing OK!");
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
    if(WiFi.status()== WL_CONNECTED){
      onReceive(LoRa.parsePacket());
    }       
    else {
      Serial.println("WiFi Disconnected");
    }
}


void sendMessage(struct datasend* my_data_to_recv) {
  datasend* data_to_send = NULL;
  data_to_send = (struct datasend*) malloc (sizeof(struct datasend));
  
  data_to_send->nodeID = 0x00;
  uint8_t sensor_pir = my_data_to_recv->sensorpir;
  uint8_t sensor_flame = my_data_to_recv->sensorflame;
  data_to_send->sensorpir = sensor_pir;
  data_to_send->sensorflame = sensor_flame;
  data_to_send->konfirmasi = 0x01;
  
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
   LoRa.readBytes((uint8_t*) data_to_recv, sizeof(struct datasend));

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

  sendMessage(data_to_recv);
  LoRa.receive();
  if(data_to_recv->nodeID == 1) {
      node = "1";
  }else {
      node="2";
  }
  
  //Keterangan 0 jika ada orang dan 1 jika ada api
  if (data_to_recv->sensorpir) {
    String keterangan = "0";
    String httpRequestData = "keterangan=" + keterangan + "&node=" + node;           
    http.begin(wifiClient, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(httpRequestData);
    http.end();
  }
  
  if (data_to_recv->sensorflame) {
    keterangan = "1";
    String httpRequestData = "keterangan=" + keterangan + "&node=" + node;           
    http.begin(wifiClient, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.end();
    digitalWrite(D1, HIGH);
    delay(100);
  }else{
    digitalWrite(D1, LOW);
  }
}
