/*
 * object_detection.h
 *
 *  Created on: Apr 19, 2021
 *      Author: njbruck
 */

#ifndef OBJECT_DETECTION_H_
#define OBJECT_DETECTION_H_


typedef struct{
    float angle; //degrees
    float distance; //cm
    float width; //cm
    int type; //1 or 2 to differentiate material
} object_data;

typedef struct{
    float ping_Distance;  //cm
    float IR_Distance;  //cm
} scan_data;

typedef struct{
    double x_pos;   //cm
    double y_pos;  //cm
    double width;  //cm
} previous_objects;



void array_init(object_data objects[]); //initialize object array to 0

int detect_objects(object_data objects[], double x_pos_mm, double y_pos_mm, float absolute_angle, previous_objects previous[]);  //detects object distance, width, angle, and type


#endif /* OBJECT_DETECTION_H_ */
