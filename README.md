# uRos_Arduino_rpLidar
Arduino Library for integrating rpLidar with micro-ROS 

 Tested With
✔️ The library is independent of micro-Ros versions.
✔️ ESP32 Dev Kit v1
✔️ rpLidar A1 (via UART + PWM)

Usage:
| Hardware |

Example Pin Connections:

| ESP32 Pin | rpLidar Pin |
|-----------|-------------|
| 16        | TX          |
| 17        | RX          |
| 4         | PWM         |

| Software |

1. Always include the #include<uRos_Arduino_rpLidar.h> after the #include <micro_ros_arduino.h>.
2. An object of type uRosLidar needs to be created. It will take 4 arguments, baudRate, rxPin, txPin,
   pwmPin, pwmChannel. Ex: uRosLidar uRos_Lidar(115200, 16, 17, 4, 0).
   These are the respective arguments required during the serial communication with the rpLidar.
3. publish(): Contains the data to be published.
4. setRotationalSpeed(): It is used to control the rotational speed of the rpLidar.
5. execute(): It is used inside the void loop() which needs to be called continously.
   
