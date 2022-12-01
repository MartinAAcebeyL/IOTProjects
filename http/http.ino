#include <ArduinoJson.h>

#include <WiFiClient.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;

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

void loop() {
  if (!is_200_ok) {

    if (WiFi.status() == WL_CONNECTED) {
      String base_url="http://192.168.43.13:3000/canciones/1";
      
      HTTPClient clientHTTP;
      clientHTTP.begin(client, base_url);

      String data_server = clientHTTP.getString();
      int codigo = clientHTTP.GET();
      
      StaticJsonDocument<500> jsonDoc;
      DeserializationError error = deserializeJson(jsonDoc, data_server);
      //if(error)
        //Serial.println("Json erroneo");
      String nombre = jsonDoc["nombre"];
      if (codigo == 200 && data_server!="" && !error) {
        //Serial.print("Data: ");
        //Serial.println(data_server);
        Serial.print("Nombre: ");
        Serial.println(nombre);
        Serial.print("Codigo: ");
        Serial.println(codigo);
        is_200_ok = true;
      }
    }
    delay(500);
  }
}