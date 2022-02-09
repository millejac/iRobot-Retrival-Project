
/*
 * tracking2.h
 *
 *  Created on: Apr 26, 2021
 *      idiot: millejac
 */



#ifndef TRACKING2_H_
#define TRACKING2_H_

#include "open_interface.h"
#include "sensors.h"
/**
 * Contains the position and direction of the tracked
 * cyBot, the oi_t object to allow moving it, and
 * functions moving the cyBot while tracking it's position.
 *//*
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
*/
/**
 * Allocates and returns a new tracked_t object with the given
 * oi_t struct.
 */
//tracked_t *new_tracked(oi_t *sensor_data);

typedef struct PosData{
        float x_pos_mm;
        float y_pos_mm;
        float degrees;
    } Position;


#endif /* TRACKING2_H_ */





