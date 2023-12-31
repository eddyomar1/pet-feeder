#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Configuración del LCD para el alimento
LiquidCrystal_I2C lcdFood(0x27, 16, 2);  // Dirección I2C, columnas, filas


// Configuración del LCD para el agua
LiquidCrystal_I2C lcdWater(0x3F, 16, 2);  // Dirección I2C, columnas, filas

// Configuración de los sensores ultrasónicos
const int foodTrigPin = 2;
const int foodEchoPin = 3;
const int waterTrigPin = 4;
const int waterEchoPin = 5;

// Configuración del servo motor
Servo myServo;
const int servoPin = 6;

// Configuración de los botones
const int buttonAPin = 7;
const int buttonBPin = 8;
const int buttonPPin = 9;
const int buttonQPin = 10;
const int buttonRPin = 11;
const int buttonS1Pin = 12;
const int buzzAlert = 13;
//Configuración de LDR
const int ldr = A3; 

// Variables para el estado del sistema
bool ageSelected = false;
int foodLevel = 0;
int waterLevel = 0;
int dailyFeedCount = 0;

void setup() {
  
  lcdFood.init();
  lcdFood.backlight();
  lcdFood.begin(16, 2);
  lcdFood.setCursor(2, 0);
  lcdFood.print("Healthy Pet");
  
  lcdWater.init();
  lcdWater.backlight();
  lcdWater.begin(16, 2);
  lcdWater.setCursor(2, 0);
  lcdWater.print("Healthy Pet");

  pinMode(foodTrigPin, OUTPUT);
  pinMode(foodEchoPin, INPUT);
  pinMode(waterTrigPin, OUTPUT);
  pinMode(waterEchoPin, INPUT);

  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);
  pinMode(buttonPPin, INPUT_PULLUP);
  pinMode(buttonQPin, INPUT_PULLUP);
  pinMode(buttonRPin, INPUT_PULLUP);
  pinMode(buttonS1Pin, INPUT_PULLUP);

  myServo.attach(servoPin);

  delay(2000); //tiempo de espera para mostrar datos de inicio

}

void loop() {
  
  if (digitalRead(buttonAPin) == LOW) {
    mostrarNivelAlimento();
  }

  else if (digitalRead(buttonBPin) == LOW) {

    lcdFood.clear();
    lcdFood.setCursor(0, 0);      
    lcdFood.print("elige la edad");   
    
    if (!ageSelected) {
      while(!ageSelected){
      if (digitalRead(buttonPPin) == LOW) {
        seleccionarEdad("Cachorro", 4);
      }

      if (digitalRead(buttonQPin) == LOW) {
        seleccionarEdad("Adolescente", 3);
      }

      if (digitalRead(buttonRPin) == LOW) {
        seleccionarEdad("Adulto", 2);
      }
    }      
    }else{reiniciarSistema();}
    

  }

  else if (digitalRead(buttonS1Pin) == LOW) {
    mostrarNivelAgua();
  }else{

  lcdFood.clear();
  lcdFood.setCursor(2, 0);
  lcdFood.print("Healthy Pet");
  
  lcdWater.clear();
  lcdWater.setCursor(2, 0);
  lcdWater.print("Healthy Pet");
    
  }

buzz();

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------

void mostrarNivelAlimento() {

  digitalWrite(foodTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(foodTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(foodTrigPin, LOW);

  float flevel = (pulseIn(foodEchoPin, HIGH)*0.0343)/2;
  float foodLevel = (((27 - flevel)/27)*100); 

  lcdFood.clear();
  lcdFood.setCursor(0, 0);
  lcdFood.print("   Alimento:    ");
  lcdFood.setCursor(4, 1);
  lcdFood.print(      foodLevel);
  lcdFood.print("%       ");
  delay(5000);
   
}

void mostrarNivelAgua() {

  digitalWrite(waterTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(waterTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(waterTrigPin, LOW);

  float rlevel = (pulseIn(waterEchoPin, HIGH)*0.0343)/2;
  float waterLevel = (((17 - rlevel)/17)*100); 

  lcdWater.clear();
  lcdWater.setCursor(0, 0);
  lcdWater.print("Nivel de agua:");
  lcdWater.setCursor(4, 1);
  lcdWater.print(waterLevel);
  lcdWater.print("%");
  delay(5000);

}

void reiniciarSistema() {
  ageSelected = false;
  foodLevel = 0;
  waterLevel = 0;
  dailyFeedCount = 0;

  lcdFood.clear();
  lcdFood.setCursor(2, 0);
  lcdFood.print("Healthy Pet");

  lcdWater.clear();
  lcdWater.setCursor(2, 0);
  lcdWater.print("Healthy Pet");
}

void seleccionarEdad(String edad, int feedPerDay) {
  lcdFood.clear();
  lcdFood.setCursor(0, 0);
  lcdFood.print("Edad seleccionada:");
  lcdFood.setCursor(2, 1);
  lcdFood.print(edad);

  ageSelected = true;
  dailyFeedCount = feedPerDay;
  delay(5000);
  lcdFood.clear();
}

void alimentarMascota() {
  if (dailyFeedCount > 0 || ldr) {
    lcdFood.clear();
    lcdFood.setCursor(0, 0);
    lcdFood.print("Alimentando...");
    delay(1000);

    myServo.write(0);
    delay(5000);
    myServo.write(360);
    delay(3000);
    dailyFeedCount--;

    lcdFood.clear();
  } else {
    lcdFood.clear();
    lcdFood.setCursor(2, 0);
    lcdFood.print("Ya alimentado");
    delay(1000); 
    lcdFood.clear();
  }
}

void buzz() {
   
  digitalWrite(foodTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(foodTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(foodTrigPin, LOW);

  float flevel = (pulseIn(foodEchoPin, HIGH)*0.0343)/2;
  float foodLevel = (((27 - flevel)/27)*100); 


  digitalWrite(waterTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(waterTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(waterTrigPin, LOW);

  float rlevel = (pulseIn(waterEchoPin, HIGH)*0.0343)/2;
  float waterLevel = (((17 - rlevel)/17)*100); 
  

while(foodLevel < 10 || waterLevel < 10){

  tone(buzzAlert, 440, 500);
  delay(1000);
}

}