#include <ezButton.h>
#include <ArduinoWiFiServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

byte LED = 5; //D0
byte LED_S1 = 16; //D1
byte BTN = 4; //D2
byte PIN_TRIG = 0; //D3
byte PIN_ECHO = 14; //D5
byte SWITCH = 12; //D6
byte LED_S2 = 13; //D7

WiFiClient client;

ezButton toggleSwitch(SWITCH);  // create ezButton object that attach to pin 7;

void setup() {
  Serial.begin(9600);
  toggleSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds

  Serial.print("Conectando desde setUp");

  pinMode(BTN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_S1, OUTPUT);
  pinMode(LED_S2, OUTPUT);
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

void loop() {
  toggleSwitch.loop(); // MUST call the loop() function first

  
  if (toggleSwitch.isPressed())
    post("cambio a Modo Secuencia");

  if (toggleSwitch.isReleased())
    post("cambio a Modo Deteccion");
  
  int state = toggleSwitch.getState();
  if (state == HIGH)//Modo Deteccion
    deteccion();
  else//Modo Secuencia
    secuencia();

  delay(1000);
}

void deteccion(){
  apagar();
  float distancia=calcular_distancia();
  Serial.println(distancia);
  if(distancia<10){
    digitalWrite(LED, HIGH);
    delay(2000);
  }else
    digitalWrite(LED, LOW);
}

void apagar(){
  digitalWrite(LED_S1, LOW);
  digitalWrite(LED_S2, LOW);
}

void secuencia(){
  digitalWrite(LED_S1, HIGH);
  digitalWrite(LED_S2, LOW);
  delay(500);
  digitalWrite(LED_S2, HIGH);
  digitalWrite(LED_S1, LOW);
  delay(500);
}

int post(String modo){
  String base_url="http://192.168.0.13:3000/final";

  DynamicJsonDocument doc(1024);
  doc["evento"]=modo;
  
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

float calcular_distancia(){
  digitalWrite(PIN_TRIG,LOW); 
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH); 
  delayMicroseconds(10);

  float tiempo = pulseIn(PIN_ECHO, HIGH);

  return tiempo*0.017;
}
