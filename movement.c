/*
 * movement.c
 *
 *  Created on: Feb 10, 2021
 *      Author: jcheatum
 */

#include "movement.h"
#include "open_interface.h"
#include "lcd.h"
#include <math.h>




double move_forward(oi_t *sensor_data, double distance_mm) {
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(500,500); //move forward at full speed
    lcd_init();
    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}

double move_backward(oi_t *sensor_data, double distance_mm) {
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-500,-500); //move forward at full speed
    lcd_init();
    while (sum > -distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}


void turn_right(oi_t *sensor, double degrees) {
    double sum = 0;
    oi_setWheels(20, -20);

    while(sum < degrees * .70) {
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0,0);
}

void turn_left(oi_t *sensor, double degrees) {
    double sum = 0;
    oi_setWheels(-20, 20);
    while(sum < degrees * .70) {
        oi_update(sensor);
        sum += -sensor->angle;
    }
    oi_setWheels(0,0);

}

void nav_forward(oi_t *sensor_data, double distance_mm) {
    double sum = 0;
    oi_setWheels(500, 500);
    while(sum < distance_mm) {
        oi_update(sensor_data);

        uint32_t left = sensor_data->bumpLeft;
        uint32_t right = sensor_data->bumpRight;
        if(left || right) {
            oi_setWheels(0,0);
            move_backward(sensor_data, 150);
            sum -= 150;

            if(left) {
                turn_right(sensor_data, 90);
                move_forward(sensor_data, 250);
                turn_left(sensor_data, 90);
            } else { // else if hit left or both
                turn_left(sensor_data, 90);
                move_forward(sensor_data, 250);
                turn_right(sensor_data, 90);
            }

            oi_setWheels(500,500);
        }
        sum += sensor_data -> distance;
    }
    oi_setWheels(0,0);
}
