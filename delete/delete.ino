#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(9600);


  WiFi.begin("wifiAcebey", "nmehmj312");

  Serial.print("Conectando desde setUp");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

bool is_200_ok = false;
WiFiClient client;
HTTPClient clientHTTP;

void loop() {
  if (!is_200_ok) {
    if (WiFi.status() == WL_CONNECTED) {
      String base_url="http://192.168.0.15:3000/canciones/";


      clientHTTP.begin(client,base_url+"1");
      int codigo=clientHTTP.sendRequest("DELETE");
      Serial.println(codigo);
      
      if (codigo == 200 ) {
        Serial.print("Codigo: ");
        Serial.println(codigo);
        is_200_ok = true;
      }
    }
    delay(500);
  }
}