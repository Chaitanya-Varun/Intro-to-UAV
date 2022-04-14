# UGV Geomentry
### Extra materials required
1. MP6050

### This implementation uses the gyroscope data from MP6050 to get current orientation of UGV, then we use PID controller to correct the position. 

### Circuit diagram
![alt text](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/12/MPU6050_ESP32_Wiring-Schematic-Diagram.png?resize=726%2C687&quality=100&strip=all&ssl=1)

1. Change your network credentials in main.cpp
```
#define MOTOR_NOMINAL  18

#define PID_K_p 30.0
#define PID_K_i  2
#define PID_K_d  1.0
int td = 3000;
int inc_angle = 90;
```
2. Variable td, inc_angle will vary according to the shape. Variables MOTOR_NOMINAL, PID_K_p, PID_K_i and PID_K_d are hyper hyperparamters, these are needed to be tuned based on experiments.

### References
[1] https://github.com/maarten-pennings/MPU6050
