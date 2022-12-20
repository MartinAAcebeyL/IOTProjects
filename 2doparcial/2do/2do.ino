#include <ArduinoWiFiServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

byte LED = 16; //D0
byte LED_ALARMA = 5; //D1
byte BTN = 4; //D2
byte PIN_TRIG = 0; //D3
byte PIN_ECHO = 14; //D5

WiFiClient client;
bool BTN_STATE = false;

void setup() {
  Serial.print("Conectando desde setUp");

  pinMode(BTN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_ALARMA, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(9600);
  WiFi.begin("wifiAcebey", "hacbyo63");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

bool on = false;

void loop() {
  BTN_STATE = digitalRead(BTN);
  if (BTN_STATE == HIGH)
    on = !on;

  if (on == true)
    sistema_on();
  else
    sistema_off();
  
  delay(150);
}

float calcular_distancia(){
  digitalWrite(PIN_TRIG,LOW); 
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH); 
  delayMicroseconds(10);

  float tiempo = pulseIn(PIN_ECHO, HIGH);

  return tiempo*0.017;
}

void sistema_on(){
  digitalWrite(LED, HIGH);
  float distancia=calcular_distancia();
  if(distancia<20){
    digitalWrite(LED_ALARMA, HIGH);
    if (WiFi.status() == WL_CONNECTED)
      post(distancia); 
    delay(2000);
  }else
    digitalWrite(LED_ALARMA, LOW);
}

int post(float distancia){
  String base_url="http://192.168.0.15:3000/parcial2";

  DynamicJsonDocument doc(1024);
  doc["nombre"]="sensor ultrasónico";
  doc["evento"]="prendido de alarma";
  doc["distancia"]=distancia;
  
  String json;
  serializeJson(doc, json);

  HTTPClient clientHTTP;
  clientHTTP.begin(client, base_url);
  clientHTTP.addHeader("Content-type", "application/json");
  int codigo=clientHTTP.POST(json);
  while(codigo!=201){
    Serial.print("-");
  }
  Serial.println("Registro echo");
  return codigo;
}

void sistema_off(){
  digitalWrite(LED, LOW);
}