#include "Arduino_LED_Matrix.h"
#include <WiFi.h>
#include <WebServer.h>

ArduinoLEDMatrix matrix;

// WiFi credentials
const char* ssid = "JouwWiFi";      // <-- pas aan
const char* password = "Wachtwoord"; // <-- pas aan

WebServer server(80);

// Sensor en relais
int sensorPin = A0;
int pompPin = 12;
int ventilatorPin = 13;
int sensorValue = 0;
int droogDrempel = 400;

// Ventilator tijdschema
unsigned long ventilatorInterval = 5000; // ventilator aan om de 5 seconden
unsigned long ventilatorDuration = 2000; // ventilator draait 2 seconden
unsigned long lastVentilatorTime = 0;

void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='2'></head><body>";
  html += "<h1>Greenhouse Dashboard</h1>";
  html += "<p>Grondvochtigheid: " + String(sensorValue) + "</p>";
  html += "<p>Pomp: " + String(digitalRead(pompPin) ? "Aan" : "Uit") + "</p>";
  html += "<p>Ventilator: " + String(digitalRead(ventilatorPin) ? "Aan" : "Uit") + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  matrix.begin();
  pinMode(sensorPin, INPUT);
  pinMode(pompPin, OUTPUT);
  pinMode(ventilatorPin, OUTPUT);
  digitalWrite(pompPin, LOW);
  digitalWrite(ventilatorPin, LOW);

  Serial.begin(9600);

  // Verbinden met WiFi
  WiFi.begin(ssid, password);
  Serial.print("Verbinden met WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Verbonden! IP-adres: ");
  Serial.println(WiFi.localIP()); // <-- hier zie je het IP in de Serial Monitor

  // Webserver starten
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server gestart");
}

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();

  // Grondvochtigheid lezen
  sensorValue = analogRead(sensorPin);
  Serial.print("Grondvochtigheid: ");
  Serial.println(sensorValue);

  // Pomp aan/uit
  if(sensorValue < droogDrempel) digitalWrite(pompPin, HIGH);
  else digitalWrite(pompPin, LOW);

  // Ventilator op tijd
  if(currentMillis - lastVentilatorTime >= ventilatorInterval){
    digitalWrite(ventilatorPin, HIGH);
    lastVentilatorTime = currentMillis;
  }
  if(digitalRead(ventilatorPin) == HIGH && currentMillis - lastVentilatorTime >= ventilatorDuration){
    digitalWrite(ventilatorPin, LOW);
  }
}
