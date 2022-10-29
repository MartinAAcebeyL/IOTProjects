#include <ESP8266WiFi.h>
void setup() {
  Serial.begin(9600);

  
  WiFi.begin("acby", "123456789");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
  delay(1000);
}