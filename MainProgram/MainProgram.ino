#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

// Hartje animatie (klein en groot)
uint8_t hartje1[8][12] = {  
  {0,0,0,1,0,0,0,1,0,0,0,0},
  {0,0,1,1,1,0,1,1,1,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0}
};

uint8_t hartje2[8][12] = {  
  {0,0,1,1,0,0,0,0,1,1,0,0},
  {0,1,1,1,1,0,0,1,1,1,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {0,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,0,0,0}
};

// Grondvochtsensor
int sensorPin = A0;
int sensorValue = 0;

// Relais pins
int pompPin = 12;       // Aansturen pomp op basis van vochtigheid
int ventilatorPin = 13; // Aansturen ventilator op tijd

// Droogte drempel voor pomp
int droogDrempel = 400;

// Ventilator tijdschema
unsigned long ventilatorInterval = 5000; // ventilator aan om de 5 seconden
unsigned long ventilatorDuration = 2000; // ventilator draait 2 seconden
unsigned long lastVentilatorTime = 0;

void setup() {
  matrix.begin();
  pinMode(sensorPin, INPUT);
  pinMode(pompPin, OUTPUT);
  pinMode(ventilatorPin, OUTPUT);

  digitalWrite(pompPin, LOW);
  digitalWrite(ventilatorPin, LOW);

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Lees grondvochtigheid
  sensorValue = analogRead(sensorPin);
  Serial.print("Grondvochtigheid: ");
  Serial.println(sensorValue);

  // 2. Pomp regelen op basis van vochtigheid
  if(sensorValue < droogDrempel){
    digitalWrite(pompPin, HIGH); // te droog → pomp aan
  } else {
    digitalWrite(pompPin, LOW);  // voldoende vocht → pomp uit
  }

  // 3. Ventilator regelen op basis van tijd
  if(currentMillis - lastVentilatorTime >= ventilatorInterval){
    // Start ventilator
    digitalWrite(ventilatorPin, HIGH);
    lastVentilatorTime = currentMillis;
  }

  // Ventilator uitschakelen na duration
  if(digitalRead(ventilatorPin) == HIGH && currentMillis - lastVentilatorTime >= ventilatorDuration){
    digitalWrite(ventilatorPin, LOW);
  }

  // 4. Hartjes animatie
  matrix.renderBitmap(hartje1, 8, 12);
  delay(100);
  matrix.renderBitmap(hartje2, 8, 12);
  delay(100);
}
