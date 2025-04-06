/*
    uROS_Arduino_rpLidar - Arduino Library for integrating rpLidar with micro-ROS  
    Author: Siddharth Zokarkar  
    Created: April 2025  
    License: MIT  

    Copyright (c) 2025 Siddharth Zokarkar  

    This library is licensed under the MIT License, which means:  
    - You are free to use, modify, and distribute this software for any purpose.  
    - You must include this copyright notice in any copies or substantial portions of the software.  
    - The software is provided "as is," without warranty of any kind.  
    - The author is not liable for any damages or issues caused by the use of this software.  

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef UROS_ARDUINO_RPLIDAR
#define UROS_ARDUINO_RPLIDAR

#include "RPLidar.h"
#include <rcl/rcl.h>
#include <rcl/time.h>
#include <rcl/error_handling.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/header.h>
#include <sensor_msgs/msg/laser_scan.h>
#include <rmw_microros/rmw_microros.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <math.h>

#define POINTS_PER_REVOLUTION        360    // If increased, more the points, lesser the real-time behaviour, lesser the range.
#define PWMfreq                      10000 // 10kHz
#define PWMresolution                 8   // 0-255
#define scanXTimesBeforeSending       1  // If increased, more the points, lesser the real-time behaviour. No effect on Range.

/***************************Classes**********************************/

class uRos {
protected:
    uRos(const int baudRate, const int rxPin, const int txPin, const int pwmPin, const int pwmChannel);
    ~uRos();
    RPLidar lidar;
    int ledChannel = 0;
    std_msgs__msg__Header header_msg = {};
    sensor_msgs__msg__LaserScan *laser_scan_msg = nullptr;
    float laserCloud[POINTS_PER_REVOLUTION] = {0};
};

class uRosLidar : public uRos{
  public: 
    uRosLidar(const int baudRate, const int rxPin, const int txPin,
    const int pwmPin, const int pwmChannel)
    :uRos(baudRate, rxPin, txPin, pwmPin, pwmChannel){}
    void execute(void);
    sensor_msgs__msg__LaserScan* publish(void);
    void setRotationalSpeed(const void * msgin);
};

#endif