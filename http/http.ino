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


  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

bool is_200_ok = false;

void loop() {
  if (!is_200_ok) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient clientHTTP;
      clientHTTP.begin(client, "http://example.com/");

      String data_server = clientHTTP.getString();
      int codigo = clientHTTP.GET();



      if (codigo == 200 && data_server!="") {
        Serial.print("Data: ");
        Serial.println(data_server);
        Serial.print("Codigo: ");
        Serial.println(codigo);
        is_200_ok = true;
      }
    }
    delay(500);
  }
}