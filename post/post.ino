#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

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
}

bool is_200_ok = false;
WiFiClient client;

void loop() {
  if (!is_200_ok) {
    if (WiFi.status() == WL_CONNECTED) {
      String base_url="http://192.168.0.15:3000/canciones";

      DynamicJsonDocument doc(1024);
      doc["nombre"]="actualizacion cancion";
      doc["author"]="actualizacion author";
      doc["duracion"]["min"]=2;
      doc["duracion"]["seg"]=55;

      doc["publicado"]=true;
      String json;
      serializeJson(doc, json);

        HTTPClient clientHTTP;
        clientHTTP.begin(client, base_url);
        clientHTTP.addHeader("Content-type", "application/json");
        int codigo=clientHTTP.POST(json);
      
      if (codigo == 201 ) {
        Serial.print("Codigo: ");
        Serial.println(codigo);
        is_200_ok = true;
      }
    }
    delay(500);
  }
}