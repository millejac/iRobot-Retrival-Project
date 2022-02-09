/*
 * movement.h
 *
 *  Created on: Feb 9, 2021
 *      Author: njbruck
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "open_interface.h"
#include "lcd.h"
#include <math.h>
#include "tracking2.h"

typedef struct _tracked {

    double x_pos_mm;
    double y_pos_mm;
    double degrees;
    oi_t *sensor_data;
    double x_hit[50];
    double y_hit[50];
    void (*move_fwd)(struct _tracked *, double);
    void (*move_rev)(struct _tracked *, double);
    void (*turn_left)(struct _tracked *, double);
    void (*turn_right)(struct _tracked *, double);
} tracked_t;

tracked_t *new_tracked(oi_t *sensor_data);

double move_forward_sensor (oi_t *sensor_data, double distance_mm,tracked_t *t);

double turn_left_sensor (oi_t *sensor_data, double degrees,tracked_t *t);

double turn_right_sensor (oi_t *sensor_data, double degrees,tracked_t *t);

double turn_around (oi_t *sensor_data, double degrees,tracked_t *t);

double Some_back (oi_t *sensor_data, double distance_mm,tracked_t *t);

double Some_forward (oi_t *sensor_data, double distance_mm,tracked_t *t);

int Interrupt_Check (oi_t *sensor_data);

double Scan_and_face (oi_t *sensor_data,tracked_t *t);

int Bump_Left_React (oi_t *sensor_data,tracked_t *t);

int Bump_Right_React (oi_t *sensor_data,tracked_t *t);

int Cliff_Front_Left_React (oi_t *sensor_data,tracked_t *t);

int Cliff_Front_Right_React (oi_t *sensor_data,tracked_t *t);

int Cliff_Left_React (oi_t *sensor_data,tracked_t *t);

int Cliff_Right_React (oi_t *sensor_data,tracked_t *t);

int Cliff_Front_Left_Signal_React (oi_t *sensor_data,tracked_t *t);

int Cliff_Front_Right_Signal_React (oi_t *sensor_data,tracked_t *t);

//  ------------ INTERRUPT/SCAN FUNCS ----------------------------
//int Interrupt_Check (oi_t *sensor_data);

//double Scan_and_face (oi_t *sensor_data,tracked_t *t);

double Distance_home (oi_t *sensor_data,tracked_t *t);

double Face_degree (oi_t *sensor_data, double degrees, tracked_t *t);

double Face_home (oi_t *sensor_data,tracked_t *t);


#endif /* SENSORS_H_ */
