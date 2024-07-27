#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menucongig' to and enable it
#endif

BluetoothSerial SerialBT;

// PINS

const int speedButton = 21;
const int stopButton = 18;

const int greenLed = 27;
const int yellowLed = 25;
const int redLed = 32;

const int buzzer = 15;


// STATES

int speedButtonState = 0;
int stopButtonState = 0;

int speedButtonLastState = 0;
int stopButtonLastState = 0;

int speedLevel = 0;

String message = "";
char incomingChar;

void turnBuzzerOn();
void setMode(int level);



void setup()
{

  pinMode(speedButton, INPUT);
  pinMode(stopButton, INPUT);

  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  pinMode(buzzer, OUTPUT);


  Serial.begin(921600);
  SerialBT.begin("ESP32test");

  Serial.println("Setup");
}

void loop() {
  
  if(Serial.available()) {
    SerialBT.write(Serial.read());
  }

  if(SerialBT.available()) {
    char incomingChar = SerialBT.read();

    if(incomingChar != '\n') {
      message += String(incomingChar);
    } else {
      message = "";
    }
    Serial.write(incomingChar);
  }

  if(message == "mode_slow") {
    speedLevel = 1;
    turnBuzzerOn();
  }
  else if(message == "mode_medium") {
    speedLevel = 2;
    turnBuzzerOn();
  }
  else if(message == "mode_fast") {
    speedLevel = 3;
    turnBuzzerOn();
  }
  else if(message == "stop") {
    speedLevel = 0;
    turnBuzzerOn();
  }


  speedButtonState = digitalRead(speedButton);
  stopButtonState = digitalRead(stopButton);

  if(speedButtonState == HIGH && speedButtonLastState == LOW) {
    Serial.println("b1");
    speedLevel++;

    if(speedLevel > 3)
      speedLevel = 1;

    turnBuzzerOn();
    
  } 


  if(stopButtonState == HIGH && stopButtonLastState == LOW) {
    speedLevel = 0;
    Serial.println("b2");

    turnBuzzerOn();
    
  }


  setMode(speedLevel);
  speedButtonLastState = speedButtonState;
  stopButtonLastState = stopButtonState;

  delay(20);
}

void setMode(int level) {
  switch (level) {
    case 0:
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, LOW);
      break;
    
    case 1:
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, LOW);
      break;
      
    case 2:
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, LOW);
      break;

    case 3:
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, HIGH);
      break;


    default:
      break;
  }

  
  
}


void turnBuzzerOn() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
}
