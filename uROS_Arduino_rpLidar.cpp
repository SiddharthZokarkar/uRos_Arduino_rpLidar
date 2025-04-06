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

#include "uROS_Arduino_rpLidar.h"

void uRosLidar::execute(void){
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
        lidar.startScan();
    }
}

uRos::uRos(const int baudRate, const int rxPin, const int txPin,
          const int pwmPin, const int pwmChannel){
    
    Serial2.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    lidar.begin(Serial2);
    ledChannel = pwmChannel;
    ledcSetup(ledChannel, PWMfreq, PWMresolution);
    ledcAttachPin(pwmPin, ledChannel);
    ledcWrite(ledChannel, 255);

    laser_scan_msg = (sensor_msgs__msg__LaserScan*) malloc(sizeof(sensor_msgs__msg__LaserScan));
    laser_scan_msg->angle_min = 0.0;
    laser_scan_msg->angle_max = 2 * M_PI;
    laser_scan_msg->angle_increment = M_PI/ 180;

    header_msg.frame_id.data = "laser_frame";
    header_msg.frame_id.size = strlen("laser_frame");

    laser_scan_msg->ranges.data = laserCloud;
    laser_scan_msg->ranges.size = 360;
    laser_scan_msg->range_min = 0.1;
    laser_scan_msg->range_max = 10.0;
    laser_scan_msg->ranges.capacity = sizeof(laserCloud);
    laser_scan_msg->intensities.size = 0;
}

void uRosLidar::setRotationalSpeed(const void * msgin){
    const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
    if (msg->data <= 255) ledcWrite(ledChannel, msg->data);
}

sensor_msgs__msg__LaserScan *uRosLidar::publish(void) {
 
  int point_count = 0;
  int scanIterations = 0;
  memset(laserCloud, 0, POINTS_PER_REVOLUTION * sizeof(float));

  while(scanIterations < scanXTimesBeforeSending){
    while (point_count < POINTS_PER_REVOLUTION) {
      if (IS_OK(lidar.waitPoint())) {
        int angle_to_index = round(lidar.getCurrentPoint().angle / 360.0 * POINTS_PER_REVOLUTION);
        float distance = lidar.getCurrentPoint().distance;
        // Update the laserCloud
        if (angle_to_index >= 0 && angle_to_index < POINTS_PER_REVOLUTION && distance) {
          laserCloud[angle_to_index] = distance / 1000.0;  // Convert distance to meters
        }
        point_count++;
      }
    }
    point_count = 0;
    scanIterations++;
  }

  /* --------------------------------------------------------------------*/
  int64_t milliseconds = rmw_uros_epoch_millis();
  rcl_time_point_value_t seconds = milliseconds / 1000;
  rcl_time_point_value_t nanoseconds = (milliseconds % 1000) * 1000000;
  builtin_interfaces__msg__Time time_msg;
  time_msg.sec = seconds;
  time_msg.nanosec = nanoseconds;
  header_msg.stamp = time_msg;
  /* --------------------------------------------------------------------*/
  laser_scan_msg->header = header_msg;
  return laser_scan_msg;
}

uRos::~uRos() {
    if (laser_scan_msg != nullptr) {
        free(laser_scan_msg);  // Free the memory
        laser_scan_msg = nullptr;  
    }
}
