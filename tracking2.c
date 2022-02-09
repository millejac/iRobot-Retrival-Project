/*
 * tracking2.c
 *
 *  Created on: Apr 26, 2021
 *      Author: millejac
 */
/*

#include <math.h>

#include "tracking2.h"
#include "movement.h"
#include "sensors.h"

#define PI 3.1415926535897932384626

void _move_forward(tracked_t *self, double dist_mm) {
    move_forward_sensor(self->sensor_data, dist_mm,t);
    self->x_pos_mm += dist_mm * cos(self->degrees * (2*PI / 360));
    self->y_pos_mm += dist_mm * sin(self->degrees * (2*PI / 360));
}

void _move_reverse(tracked_t *self, double dist_mm) {
    Some_back(self->sensor_data, dist_mm,t);
    self->x_pos_mm -= dist_mm * cos(self->degrees * (2*PI / 360));
    self->y_pos_mm -= dist_mm * sin(self->degrees * (2*PI / 360));
}

void _turn_left(tracked_t *self, double degrees) {
    turn_left_sensor(self->sensor_data, degrees,t);
    if(self->degrees + degrees <= 360) {
        self->degrees += degrees;
    } else {
        self->degrees = self->degrees + degrees - 360;
    }
}

void _turn_right(tracked_t *self, double degrees) {
    turn_right_sensor(self->sensor_data, degrees,t);
    if(self->degrees - degrees >= 0) {
        self->degrees -= degrees;
    } else {
        self->degrees = self->degrees - degrees + 360;
    }
}

tracked_t *new_tracked(oi_t *sensor_data) {
    tracked_t *t = (tracked_t *) malloc(sizeof(tracked_t));
    t->x_pos_mm = 0;
    t->y_pos_mm = 0;
    t->degrees = 0;
    t->sensor_data = sensor_data;

    t->x_hit;
    t->y_hit;


    t->move_fwd = &_move_forward;
    t->move_rev = &_move_reverse;
    t->turn_left = &_turn_left;
    t->turn_right = &_turn_right;

    return t;
}

;
*/
