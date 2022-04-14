//----------------------Skeleton Code--------------------//
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//    Can be client or even host   //
#ifndef STASSID
#define STASSID "LTE_E510" // Add your network credentials
#define STAPSK "connect@e510"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

void OTAsetup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.print("Connecting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.begin();
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());
}

void OTAloop()
{
  ArduinoOTA.handle();
}

#include <Wire.h>
#include "mpu6050.h"
#include <Arduino.h>

#include <esp32PWMUtilities.h>

MPU6050 mpu6050;

void mpu6050_begin()
{
  Wire.begin();
  Serial.print("MPU6050: Starting calibration; leave device flat and still ... ");
  int error = mpu6050.begin();
  Serial.println(mpu6050.error_str(error));
}

float mpu6050_yaw()
{
  MPU6050_t data = mpu6050.get();
  while (data.dir.error != 0)
  {
    Serial.println(mpu6050.error_str(data.dir.error));
    Wire.begin();
    data = mpu6050.get();
  }
  return data.dir.yaw;
}

// Motors ===================================================
Motor Motor1;
Motor Motor2;

void motor_begin(bool backward)
{
  if (backward)
  {
    Motor2.attach(14, 16, 17);
    Motor1.attach(15, 18, 19);
  }
  else
  {
    Motor1.attach(14, 17, 16);
    Motor2.attach(15, 19, 18);
  }

  Serial.println("Motors : ok");
}

// Set motor A to given speed (-255..+255); 0 switches off
void motor_A_set(int speed)
{
  Motor1.moveMotor(speed);
}

// Set motor B to given speed (-255..+255); 0 switches off
void motor_B_set(int speed)
{
  Motor2.moveMotor(speed);
}

#define MOTOR_NOMINAL 180

#define PID_K_p 30.0
#define PID_K_i 1.2
#define PID_K_d 1.0

void motor_off()
{
  motor_A_set(0);
  motor_B_set(0);
}

void motor_forward(int delta)
{
  motor_A_set(MOTOR_NOMINAL - delta);
  motor_B_set(MOTOR_NOMINAL + delta);
}

// PID ===================================================

float i_input;
float d_last;

void pid_begin()
{
  i_input = 0;
  d_last = 0;
  Serial.println("PID    : ok");
}

int pid(float error)
{
  float p_input;
  float d_input;

  p_input = error;
  i_input = constrain(i_input + error, -50, +50);
  d_input = error - d_last;
  d_last = error;

  return p_input * PID_K_p + i_input * PID_K_i + d_input * PID_K_d;
}

// Main ===================================================

bool drive_squares; // When true, drives a drive_squaress, otherwise straight
uint32_t last;      // last time (in ms) we turned 90 degrees at the corner of a drive_squares

int state;

void setup()
{

  Serial.begin(115200);
  OTAsetup();

  bool back;

  state = 2;
  if (state == 0)
  {
    back = false;
    Serial.println("Stopped");
  }
  else if (state == 1)
  {
    back = false;
    drive_squares = 0;
    Serial.println("Driving straight");
  }
  else if (state == 2)
  {
    back = false;
    drive_squares = 1;
    Serial.println("Driving Square");
  }
  else if (state == 3)
  {
    back = true;
    drive_squares = 0;
    Serial.println("Driving backwards");
  }
  else if (state == 4)
  {
    back = true;
    drive_squares = 1;
    Serial.println("Driving backwards square");
  }

  motor_begin(back);
  pid_begin();
  mpu6050_begin();

  motor_off();

  Serial.println();
  last = millis();
}

float target_dir = 0.0;
// Square
// int td = 2500;
// int inc_angle = 90;
// Circle
// int td = 1;
// int inc_angle = 0.05;
// Straight Line
int td = 1;
int inc_angle = 0.00;

void loop()
{
  OTAloop();

  float current_dir;
  int steer;

  current_dir = mpu6050_yaw();
  Serial.print("Angle:");
  Serial.print(current_dir, 2);
  Serial.print(",");
  Serial.print("Desired:");
  Serial.print(target_dir, 2);
  Serial.println("");
  steer = pid(target_dir - current_dir);
  Serial.print(" steer=");
  Serial.println(steer);
  motor_forward(steer);

  // Change the next target angle to trace custom trajectory
  if (drive_squares)
  {
    if (millis() - last > td)
    {
      target_dir += inc_angle;
      last = millis();
    }
  }
}
