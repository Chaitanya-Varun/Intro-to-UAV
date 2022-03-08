#include <Arduino.h>

#include <WiFi.h>
#include <esp32PWMUtilities.h>

#include <DabbleESP32.h>

#define WIFI_NETWORK "LTE_510"
#define WIFI_PASSWORD "connect@e510"
#define WIFI_TIMEOUT_MS 60000

void connectToWiFi(){
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD);
  unsigned long attempt_start_time = millis();
  while(WiFi.status() != WL_CONNECTED && millis() - attempt_start_time < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed to connect!");
  }
  else{
    Serial.print("Connected to ");
    Serial.println(WiFi.localIP());
  }

}

Motor Motor1;

Motor Motor2;
void _setup() {
  Serial.begin(9600);
  // Serial.println("----SERIAL----");
  // connectToWiFi();

  Motor1.attach(14, 16, 17);

  Motor2.attach(15, 18, 19);

  Dabble.begin("SCV_ESP32");
}

void _loop() {
  Dabble.processInput();
}

void setup() {
  _setup();
}

void loop() {
  _loop();
  if (GamePad.isPressed(0)) {
    Motor1.moveMotor(2.55 * 100);
    Motor2.moveMotor(2.55 * 100);
  }
  else {
    if (GamePad.isPressed(1)) {
      Motor1.moveMotor(-2.55 * 100);
      Motor2.moveMotor(-2.55 * 100);
    }
    else {
      if (GamePad.isPressed(3)) {
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(-2.55 * 100);
      }
      else {
        if (GamePad.isPressed(2)) {
          Motor1.moveMotor(-2.55 * 100);
          Motor2.moveMotor(2.55 * 100);
        }
        else {
          Motor1.lockMotor();
          Motor2.lockMotor();
        }
      }
    }
  }
}
