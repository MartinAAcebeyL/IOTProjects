#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

float _distancia(){
  // Paso 6
    digitalWrite(PIN_TRIG,LOW); 
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH); 
    delayMicroseconds(10);

    // Paso 7
    tiempo = pulseIn(PIN_ECHO, HIGH); 

    // Paso 8
    distancia= tiempo / 2 / 29.1;
  
    return distancia;
}

int post(float distancia){    
  DynamicJsonDocument doc(1024);
  doc["nombre"]="ultra 1";
  doc["distancia"]=distancia;

  String json;
  serializeJson(doc, json);

  HTTPClient clientHTTP;
  clientHTTP.begin(client, base_url);
  clientHTTP.addHeader("Content-type", "application/json");
  int codigo=clientHTTP.POST(json);
      
  return codigo;
}

int put(float distancia){
  DynamicJsonDocument doc(1024);
  doc["nombre"]="ultra 1";
  doc["distancia"]=distancia;

  String json;
  serializeJson(doc, json);

  HTTPClient clientHTTP;
  clientHTTP.begin(client, base_url+"1");
  clientHTTP.addHeader("Content-type", "application/json");
  int codigo=clientHTTP.PUT(json);
      
  return codigo;
}

void setup() {
  Serial.begin(9600);


  WiFi.begin("acby", "123456789");

  Serial.print("Conectando desde setUp");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

bool is_200_ok = false;
WiFiClient client;
float tiempo;
float distancia;
boolean validation = false;

int PIN_TRIG = 14; //D5
int PIN_ECHO = 12; //D6
String base_url="http://192.168.0.15:3000/garaje/";

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float distancia = _distancia();

    Serial.print("Distancia ");
    Serial.print(distancia);
    Serial.print(" cm");
    Serial.println("");
    delay(1000);

    if (distancia <= 10) {
      Serial.print("Distancia: ");
      Serial.println(distancia);
      if(!validation){
        int codigo = post();
        if (!is_200_ok) {
          if(codigo==201){
            Serial.print("Codigo: ");
            Serial.println(codigo);
            is_200_ok = true;
            validation=true;
          }
        }
      }else{
        int codigo = put();
          if(codigo==201){
            Serial.print("Codigo: ");
            Serial.println(codigo);
            // is_200_ok = true;
          }
      }
    }
  }
  delay(500);
}





void conectarse(){
  if (WiFi.status() == WL_CONNECTED) {
      String base_url="http://192.168.0.15:3000/canciones/";

      DynamicJsonDocument doc(1024);
      doc["nombre"]="actualizado cancion 2";
      doc["author"]="actualizado author 2";
      doc["duracion"]["min"]=2;
      doc["duracion"]["seg"]=55;
      doc["publicado"]=true;

      String json;
      serializeJson(doc, json);

      HTTPClient clientHTTP;
      clientHTTP.begin(client, base_url+"2");
      clientHTTP.addHeader("Content-Type", "application/json");
      int codigo=clientHTTP.PUT(json);
      
      if (codigo == 200 ) {
        Serial.print("Codigo: ");
        Serial.println(codigo);
        is_200_ok = true;
      }
  }
}