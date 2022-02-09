/*
 * movement.h
 *
 *  Created on: Feb 9, 2021
 *      Author: njbruck
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"
#include "lcd.h"
#include "math.h"

typedef struct {

    double distance_total;
    double angle_total;
    char spacing[9];
} info;

double move_forward (oi_t *sensor_data, double distance_mm, info data);

double turn_right (oi_t *sensor_data, double degrees, info data);

double turn_left (oi_t *sensor_data, double degrees, info data);

#endif /* MOVEMENT_H_ */
