/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include "resetSimulation.h"
#include "servo.h"
#include "object_detection.h"
#include "adc.h"
#include <stdio.h>
#include "open_interface.h"
#include "sensors.h"
#include <stdint.h>
#include <string.h>
#include "tracking2.h"
#include <stdlib.h>
#include "uart.h"

// Uncomment or add any include directives that are needed

#warning "Possible unimplemented functions"
#define REPLACEME 0
#define pi 3.14159629
extern int interrupt_flag;

int main(void) {
	lcd_init();
	ping_init();
	servo_init();
	adc_init();
	uart_init();

	previous_objects previous[10];
	object_data scanned_objects[10];
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
	tracked_t *t = new_tracked(sensor_data);


	bool mining = false;
	int num_objects, i;
	double x_pos = 0;
	double y_pos = 0;
	float angle = 0;
	double home_dist;
	for (i = 0; i < 10; i++) {
	        previous[i].width = 0;
	        previous[i].x_pos = 0;
	        previous[i].y_pos = 0;
	}
	//Leave starting area
	move_forward_sensor(sensor_data, 800, t);

	num_objects = detect_objects(scanned_objects, t->x_pos_mm, t->y_pos_mm, t->degrees, previous);
	if(num_objects > 0){
	    mining = true;
	}


	int j;
	int k = 0;
	while(k < 3){
	    while(mining == false){
	        move_forward_sensor(sensor_data, 450, t);
	        num_objects = detect_objects(scanned_objects, t->x_pos_mm, t->y_pos_mm, t->degrees, previous);
	        if(num_objects > 0){
	            mining = true;
	        }
	        interrupt_flag = 0;
	    }

        while(mining == true) {
            for(j = 0; j < num_objects; j++) {
                if(scanned_objects[j].angle < 90 && scanned_objects[j].angle != 0) {
                    t->turn_left(t,90-scanned_objects[j].angle);
                } else if (scanned_objects[j].angle > 90 && scanned_objects[j].angle != 0){
                    t->turn_right(t,scanned_objects[j].angle-90);
                }
                move_forward_sensor(sensor_data, (scanned_objects[j].distance - 3) * 10, t);
                if(interrupt_flag == 1) {
                    break;
                }
                previous[k].width = scanned_objects[j].width;
                previous[k].x_pos = (t->x_pos_mm / 10) + (17+3+scanned_objects[j].width/2)*cos(t->degrees *pi/180);
                previous[k].y_pos = (t->y_pos_mm / 10) + (17+3+scanned_objects[j].width/2)*sin(t->degrees *pi/180);
                k++;
                Some_back(sensor_data, (scanned_objects[j].distance - 2) * 10, t);
                if(scanned_objects[j].angle < 90 && scanned_objects[j].angle != 0) {
                       t->turn_right(t,90-scanned_objects[j].angle);
                } else if (scanned_objects[j].angle > 90 && scanned_objects[j].angle != 0){
                       t->turn_left(t,scanned_objects[j].angle-90);
                }
            }
            interrupt_flag = 0;
            num_objects = detect_objects(scanned_objects, t->x_pos_mm, t->y_pos_mm, t->degrees, previous);

            if(num_objects == 0){
                mining = false;
            }
        }
	}

	while(Distance_home(sensor_data,t) > 20){
        home_dist = Distance_home(sensor_data,t);
        //char homeDist[30];
        //sprintf(homeDist, "%lf", home_dist);
        //uart_sendStr(homeDist);
        Face_home(sensor_data,t);
        move_forward_sensor(sensor_data,home_dist,t);
	}
	int l;
	for(l = 0; l < k; l++){
	    char print[45];
	    sprintf(print, "\nObject %d\nWidth:%.1lf\nY position: %.1lf\nX position: %.1lf\n", l+1, previous[l].width, previous[l].y_pos, previous[l].x_pos);
	    uart_sendStr(print);
	}

}
