/*
 * object_detection.c
 *
 *  Created on: Apr 19, 2021
 *      Author: njbruck
 */

#include "ping.h"
#include "servo.h"
#include "adc.h"
#include "object_detection.h"
#include "math.h"
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include <stdlib.h>

#define pi 3.14159265

void array_init(object_data objects[]) {
    int i;

    for (i = 0; i < 10; i++) {
        objects[i].angle = 0;
        objects[i].distance = 0;
        objects[i].type = 0;
        objects[i].width = 0;
    }
}

int detect_objects(object_data objects[], double x_pos_mm, double y_pos_mm, float absolute_angle, previous_objects previous[]) {

    int object = 0;
    int ir_raw;
    int angle; //for tracking the width of objects when sorting raw data
    scan_data raw_data[181]; //for storing angle, ping, ir data for 180 scan
    int i, t;
    float x_delete, y_delete, width, corrected_angle;

    array_init(objects);

    x_pos_mm = x_pos_mm/10; //convert to cm
    y_pos_mm = y_pos_mm/10;

    for (i = 0; i <= 180; i++) {

       servo_move(i); //angle
       timer_waitMillis(20);
       ir_raw = adc_read_average();
       raw_data[i].IR_Distance = adc_convert(ir_raw);   //ir distance
       raw_data[i].ping_Distance = ping_getDistance();  //ping distance

    }
    for (i = 0; i <=180; i++) {

       if (raw_data[i].IR_Distance >= 10 && raw_data[i].IR_Distance <= 80 ) {

           angle = 0;
           i++;

           while ( abs(raw_data[i-1].IR_Distance - raw_data[i].IR_Distance) <= 2) {
               i++;
               angle++;
           }
           objects[object].distance = raw_data[i-angle/2].ping_Distance;
           objects[object].width = 2*pi*objects[object].distance * angle/360;  //calc width
           objects[object].angle = i - angle/2;  //angle of the scan the object is at

           if (objects[object].angle <= 90) { //calc distance from object to closest point on cybot hull

               objects[object].distance = raw_data[i-angle/2].ping_Distance - ((-.1499)*(i-angle/2)+12.704);
               if(objects[object].distance >=30){
                   objects[object].angle = objects[object].angle + ((-.177)*objects[object].angle + 16.796);
               }else {
                   objects[object].angle = objects[object].angle + (-.002*pow(objects[object].angle,2) - 0.1208*objects[object].angle +25.751);
               }
           }else if (objects[object].angle > 90) {

               objects[object].distance = raw_data[i-angle/2].ping_Distance - ((-.1499)*(180-i-angle/2)+12.704);
               if(objects[object].distance >=30){
                  objects[object].angle = objects[object].angle - ((-.0014) * pow(objects[object].angle, 2) + (.5394*objects[object].angle) - 38.5);
               }else {
                  objects[object].angle = objects[object].angle - ((-.00007 * pow(objects[object].angle,3)) + (.0247*pow(objects[object].angle , 2)) - (2.4235*objects[object].angle) + 69.782);
               }
           }

           if (objects[object].width < 10 && objects[object].width > 6.2) {
               objects[object].type = 1;
           }else {   //if detected object is <6cm or >10cm we discard that object
               objects[object].width = 0;
               objects[object].angle = 0;
               objects[object].distance = 0;
               objects[object].type = 0;
               object--;
           }
           for (t = 0; t < 10; t++) {  //this loop checks if the detected object falls within the range of previously captured objects
                                        //and will erase the detected object as needed
               corrected_angle = (90 + absolute_angle - objects[object].angle) * (pi/180);  //corrects angle as cybot deviates from 0 degrees
               x_delete = x_pos_mm + (cos(corrected_angle) * (objects[object].distance + 17)) + (cos(corrected_angle) * (objects[object].width /2));  //calculate absolute x,y values
               y_delete = y_pos_mm + (sin(corrected_angle) * (objects[object].distance + 17)) + (sin(corrected_angle) * (objects[object].width /2));  //of detected objects
               width = previous[t].width;   //width of the previously scanned object. can add more for tolerance

               if (x_delete < (previous[t].x_pos + width) &&
                   x_delete > (previous[t].x_pos - width) &&
                   y_delete < (previous[t].y_pos + width) &&
                   y_delete > (previous[t].y_pos - width)) {

                   objects[object].width = 0;
                   objects[object].angle = 0;
                   objects[object].distance = 0;
                   objects[object].type = 0;
                   object--;
                   break;

               }
           }
           object++;
       }
    }


    return object;
}
