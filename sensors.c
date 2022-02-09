/*
 * movement.c
 *
 *  Created on: Feb 9, 2021
 *      Author: njbruck
 */

#include "sensors.h"
#include "open_interface.h"
#include "object_detection.h"
#include "servo.h"
#include "movement.h"
#include <math.h>
#include "tracking2.h"
#define PI 3.1415926535897932384626

volatile float DaFloat = 0;
volatile int DaInt   = 0;
volatile int DaCliff = 0;

struct Coord{               //IMITATES ORDER LIKE INTERRUPTS
    int x1[15];
    int y1[15];
    float angle1[15];
    int x2[15];
    int y2[15];
    float angle2[15];
};

int interrupt_flag = 0;
double dist = 0;        //How far in told to move
double trav = 0;        //How far ive actually moved, physically
double turn = 90;       //Please keep this here for turn reacts, reg turns dont use this
double back = 25;
double forward = 25;
float M_dist = 1500;
object_data objects[10];

//oi_t *sensors = oi_alloc();
//oi_init(sensors);
//tracked_t *t = new_tracked(sensors);


double move_forward_sensor (oi_t *sensor_data, double distance_mm,tracked_t *t)
{

   double sum = 0;

   oi_setWheels(100,100);                                                                   //This is where GOTO goes On
       while ((Interrupt_Check(sensor_data)) && (sum < distance_mm))           //CHECKS IF IT LITERALLY HAS AN ISSUE
       {
           //oi_update(sensor_data);
           sum += sensor_data -> distance;
           //&Position.x_pos_mm = sum;
           //t->x_pos_mm += distance_mm * cos(t->degrees * (2*PI / 360));
           //t->y_pos_mm += distance_mm * sin(t->degrees * (2*PI / 360));
           //t->move_fwd(t, 500);
           //lcd_printf("%f cm", trunc(sum / 10));
       }
       t->x_pos_mm += (sum * cos(t->degrees * (2*PI / 360)));
       t->y_pos_mm += (sum * sin(t->degrees * (2*PI / 360)));
       //}
   oi_setWheels(0,0);                  //STOP WHEELS BEFORE PROCEEDING


    if (sensor_data->bumpLeft)
    {
        Bump_Left_React (sensor_data,t);                      //Bumps detects
    }

    if (sensor_data->bumpRight)                             //Bumps detects
    {
        Bump_Right_React (sensor_data,t);
    }

    if (2000 < sensor_data->cliffFrontLeftSignal)           //Bound Detects
    {
        Cliff_Front_Left_Signal_React (sensor_data,t);
    }

    if (2000 < sensor_data->cliffFrontRightSignal)          //Bound Detects
    {
        Cliff_Front_Right_Signal_React (sensor_data,t);
    }


    if (sensor_data->cliffLeft)                               //CLIFF LEFT detect?
    {
        Cliff_Left_React (sensor_data,t);
    }

    if (sensor_data->cliffRight)                              //CLIFF RIGHT detect?
    {
        Cliff_Right_React(sensor_data,t);
    }


    if (sensor_data->cliffFrontLeft || sensor_data->cliffFrontRight)        //CHECK IF EITHER SIDE DROPPED
    {
        if ((sensor_data->cliffFrontLeft + sensor_data->cliffFrontRight) == 2)  //BOTh DID?
        {
            Cliff_Front_Left_React (sensor_data,t);                       //Feel this may be dependent on scan info
        }

        else if (sensor_data->cliffFrontLeft)                               //JUST LEFT FRONT?
        {
            Cliff_Front_Left_React (sensor_data,t);
        }

    }

        else if (sensor_data->cliffFrontRight)                              //JUST RIGHT FRONT?
        {
            Cliff_Front_Right_React (sensor_data,t);
        }

    DaInt = 0;      //hopefully when we reach object or decide to scan again this resets, as it returns to main.
    return sum;
}

                    //-------------------- TURN / SUPPLEMENT MOVE FUNCTIONS ----------------------------


double turn_left_sensor (oi_t *sensor_data, double degrees,tracked_t *t)
{
    double angle = 0;
    //degrees=degrees*.98;
    oi_setWheels(-100, 100);

    while (angle < degrees) {
        oi_update(sensor_data);
        angle -= sensor_data->angle;
        //lcd_printf("%f", angle);
    }
    oi_setWheels(0,0);
    return angle;
}

double turn_right_sensor (oi_t *sensor_data, double degrees,tracked_t *t)
{
    double angle = 0;
    //degrees=degrees*.98;
    oi_setWheels(100, -100);

    while (angle < degrees) {
        oi_update(sensor_data);
        angle += sensor_data->angle;
        //lcd_printf("%f", angle);
    }
    oi_setWheels(0,0);
    return angle;
}

//POSSIBLE REMOVE
double turn_around (oi_t *sensor_data, double degrees,tracked_t *t)
{
    degrees = 180;
    double angle = 0;
    //degrees=degrees*.98;
    oi_setWheels(100, -100);

    while (angle < degrees) {
        oi_update(sensor_data);
        angle += sensor_data->angle;
        //lcd_printf("%f", angle);
    }
    oi_setWheels(0,0);
    return 0;
}

double Some_back (oi_t *sensor_data, double distance_mm,tracked_t *t)
{
    double sum = 0;

    oi_setWheels(-100,-100);

        while (sum < distance_mm ) {
            oi_update(sensor_data);
            sum -= sensor_data -> distance;
            //lcd_printf("%f cm", trunc(sum / 10));
        }
        //t->x_pos_mm -= distance_mm * cos(t->degrees * (2*PI / 360));
        //t->y_pos_mm -= distance_mm * sin(t->degrees * (2*PI / 360));
        t->x_pos_mm -= (sum * cos(t->degrees * (2*PI / 360)));
        t->y_pos_mm -= (sum * sin(t->degrees * (2*PI / 360)));

        //self->x_pos_mm -= self->sensor_data * cos(self->degrees * (2*PI / 360));
        //self->y_pos_mm -= self->sensor_data * sin(self->degrees * (2*PI / 360));


     oi_setWheels(0,0);
     sum = -sum;            //SUM IS NEG
     return sum;
}


//POSSIBLE REMOVE
double Some_forward (oi_t *sensor_data, double distance_mm,tracked_t *t)
{
    double sum = 0;

    oi_setWheels(100,100);



    move_forward_sensor (sensor_data, 150,t);
    /*{
        while ((Interrupt_Check(sensor_data)) && (sum < 190))      //moves 19 CM sir, within move_forward_sensor which has
        {                                                           // "interrupts" enabled
            oi_update(sensor_data);
            sum += sensor_data -> distance;
            //lcd_printf("%f cm", trunc(sum / 10));
        }

    }*/

     oi_setWheels(0,0);
     sum = sum;
     return sum;

}


//              ---------------------------------------- FUNCTIONs WITH SCANS----------------------

double Scan_and_face (oi_t *sensor_data,tracked_t *t)
{
    object_data object_info[10];            //struct for object data, angle, distance, type, and width
    //array_init(object_info);

    oi_update(sensor_data);
    //detect_objects(objects[], x_pos_mm, y_pos_mm, previous[]);<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< UN CROSS

    int turn_ang;
    turn_ang = objects[0].angle;        //What angle the thin object is at
    //turn_ang = turn_ang - 90;
lcd_printf("%i ",turn_ang);

    if ((turn_ang > 0) &&(turn_ang < 90))
    {
        turn_ang = (90 - turn_ang)*0.93;                   //for scan left is 0 deg. for oi left is 90deg
        turn_left_sensor (sensor_data, turn_ang,t);
        lcd_printf("Item left");
        //turn_left_sensor(sensor_data, turn);
        //return sensor_data->angle;
        double dist = objects[0].distance;
        dist = (dist * 4);                               //cm to mm
        move_forward_sensor (sensor_data, dist,t);
    }

    else if (turn_ang  >= 90)
    {
        turn_ang = (turn_ang - 90)*0.93;
        turn_right_sensor (sensor_data, turn_ang,t);
        //turn_left_sensor(sensor_data, turn);
        //return sensor_data->angle;
        lcd_printf("Item right");
        double dist = objects[0].distance;
        dist = (dist *4);                                //cm to mm
        move_forward_sensor (sensor_data, dist,t);
    }

    else if (!(object_info[0].type == 1))
    {
        turn_around (sensor_data,turn,t);                 //doesnt matter what double is inputted
    }


    return sensor_data->angle;

}


                            //-----------------------INTERRUPT ---------------------------------------


int Interrupt_Check (oi_t *sensor_data)
{
    oi_update(sensor_data);

    int INTERRUPT = (!(sensor_data->bumpLeft) && !(sensor_data->bumpRight)  //CHECKS IF IT LITERALLY HAS AN ISSUE
          && !(sensor_data->cliffFrontLeft) && !(sensor_data->cliffFrontRight)
          && !(sensor_data->cliffLeft) && !(sensor_data->cliffRight) &&
          !(((2000 < sensor_data->cliffFrontRightSignal)) && (2000 < sensor_data->cliffFrontLeftSignal)) );
                //all these vals deafult to 0, seeing  INTERRUPT = 1 is GOOD
    if(INTERRUPT == 0) {
        interrupt_flag = 1;
    }
    //else {
        //interrupt_flag = 0;
    //}
    return INTERRUPT;
}

                            //-------------------- BUMP REACTS ----------------------------


int Bump_Left_React (oi_t *sensor_data,tracked_t *t)
{/*
    DaInt = DaInt +1;
    dist = dist + Some_back(sensor_data, back);       //when bumper is hit, sum back is a neg val
    oi_update(sensor_data);
    turn_right_sensor(sensor_data, turn);
    move_forward_sensor (sensor_data, 170,tracked_t *t);        //enough to clear said ob
    turn_left_sensor(sensor_data, turn);         //str8nd out    ALSO WHERE WE COULD RE-SCAN
    dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt
    move_forward_sensor (sensor_data, 280,tracked_t *t);     //MAYBE RESCAN HERE
    //move_forward_sensor (sensor_data, dist);     //rest of total dist
    //turn_left_sensor(sensor_data, turn);
    //Some_forward(sensor_data, forward);
    //turn_right_sensor(sensor_data, turn);   //in front of obj now
    */
    Some_back(sensor_data,70,t);
    t->turn_right(t,turn);
    //t->move_fwd(t,170);
    move_forward_sensor(sensor_data,150,t);         //Vertical sidestep
    t->turn_left(t,turn);
    move_forward_sensor(sensor_data,200,t);

    //oi_free(sensor_data);
    dist = M_dist;
    return DaInt;
}

int Bump_Right_React (oi_t *sensor_data,tracked_t *t)
{

    //_move_reverse(t,back);

    //t->move_rev(t,back);
    Some_back(sensor_data,70,t);
    t->turn_left(t,turn);
    //t->move_fwd(t,170);
    move_forward_sensor(sensor_data,150,t);         //Vertical sidestep
    t->turn_right(t,turn);
    move_forward_sensor(sensor_data,200,t);
    //t->move_fwd(t,250);

/*
    DaInt = DaInt +1;
    dist = dist + Some_back(sensor_data, back,t);       //when bumper is hit
    turn_left_sensor(sensor_data, turn,t);         //str8nd out
    move_forward_sensor (sensor_data, 170,t);        //enough to clear said ob  ---  This in mm
    turn_right_sensor(sensor_data, turn,t);
    dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt

    move_forward_sensor (sensor_data, 280,t);     //MAYBE RESCAN HERE
    //turn_right_sensor(sensor_data, turn);   //in front of obj now
    //Some_forward(sensor_data, forward);
    //turn_left_sensor(sensor_data, turn);*/
    //oi_free(sensor_data);
    return 0;


    //return DaInt;
}

                            //-------------------- CLIFF/HOLE REACTS ----------------------------

int Cliff_Front_Left_React (oi_t *sensor_data,tracked_t *t)
{
    DaInt = DaInt +1;
/*
    dist = dist + Some_back(sensor_data, back);
    turn_right_sensor(sensor_data, turn);
    move_forward_sensor (sensor_data, 170,tracked_t *t);        //enough to clear said ob
    turn_left_sensor(sensor_data, turn);
    dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt

    move_forward_sensor (sensor_data, 280,tracked_t *t);     //MAYBE RESCAN HERE*/
    Some_back(sensor_data,70,t);
    t->turn_right(t,turn);
    move_forward_sensor(sensor_data,150,t);         //Vertical sidestep
    t->turn_left(t,turn);
    move_forward_sensor(sensor_data,200,t);
    //oi_free(sensor_data);
    return DaInt;
}

int Cliff_Front_Right_React (oi_t *sensor_data,tracked_t *t)
{
    DaInt = DaInt +1;


    Some_back(sensor_data,70,t);
    t->turn_left(t,turn);
    move_forward_sensor(sensor_data,150,t);         //Vertical sidestep
    t->turn_right(t,turn);
    move_forward_sensor(sensor_data,200,t);


    /*
    dist = dist + Some_back(sensor_data, back,t);
    turn_left_sensor(sensor_data, turn,t);
    move_forward_sensor (sensor_data, 170,t);        //enough to clear said ob
    turn_right_sensor(sensor_data, turn,t);
    dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt

    move_forward_sensor (sensor_data, 280,t);     //MAYBE RESCAN HERE*/
    //oi_free(sensor_data);
    return DaInt;
}

int Cliff_Left_React (oi_t *sensor_data,tracked_t *t)
{
    DaInt = DaInt +1;

    Some_back(sensor_data,70,t);
    t->turn_right(t,turn);
    move_forward_sensor(sensor_data,150,t);         //Vertical sidestep
    t->turn_left(t,turn);
    move_forward_sensor(sensor_data,200,t);

     /*dist = dist + Some_back(sensor_data, back,t);
     turn_right_sensor(sensor_data, 10,t);                           //Just 15 degrees, cause close to right direction
     move_forward_sensor (sensor_data, 170,t);        //enough to clear said ob
     turn_left_sensor(sensor_data, turn,t);
     dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt
     move_forward_sensor (sensor_data, 280,t);     //MAYBE RESCAN HERE*/
     //oi_free(sensor_data);
     return DaInt;
}

int Cliff_Right_React (oi_t *sensor_data,tracked_t *t)
{
     DaInt = DaInt +1;

     Some_back(sensor_data,70,t);
     t->turn_left(t,turn);
     move_forward_sensor(sensor_data,150,t);         //Vertical sidestep
     t->turn_right(t,turn);
     move_forward_sensor(sensor_data,200,t);
     /*dist = dist + Some_back(sensor_data, back,t);
     turn_left_sensor(sensor_data, 10,t);                           //Just 15 degrees, cause close to right direction
     move_forward_sensor (sensor_data, 170,t);        //enough to clear said ob
     turn_right_sensor(sensor_data, turn,t);
     dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt
     move_forward_sensor (sensor_data, 280,t);     //MAYBE RESCAN HERE*/
     //oi_free(sensor_data);
     return DaInt;
}

int Cliff_Front_Left_Signal_React (oi_t *sensor_data,tracked_t *t)        //Same react as Cliff_Left_React, <-------------
{
    DaInt = DaInt +1;

        t->turn_left(t,180);
        move_forward_sensor(sensor_data,200,t);
        t->turn_right(t,turn);
        //move_forward_sensor(sensor_data,200,t);

        /*dist = dist + Some_back(sensor_data, back,t);
        turn_right_sensor(sensor_data, turn,t);
        move_forward_sensor (sensor_data, 170,t);        //enough to clear said ob
        turn_left_sensor(sensor_data, turn,t);
        dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt

        move_forward_sensor (sensor_data, 280,t);     //MAYBE RESCAN HERE*/
        //oi_free(sensor_data);
        return DaInt;
}

int Cliff_Front_Right_Signal_React (oi_t *sensor_data,tracked_t *t)      //Same react as Cliff_Right_React, <-------------
{
    DaInt = DaInt +1;

    t->turn_left(t,180);
    move_forward_sensor(sensor_data,200,t);
    t->turn_left(t,turn);

    /* dist = dist + Some_back(sensor_data, back,t);
    turn_left_sensor(sensor_data, turn,t);
    move_forward_sensor (sensor_data, 170,t);        //enough to clear said ob
    turn_right_sensor(sensor_data, turn,t);
    dist = M_dist - dist;      //total - actual  = dist still to go. plug into next. assum no interrupt

    move_forward_sensor (sensor_data, 280,t);     //MAYBE RESCAN HERE*/
    //oi_free(sensor_data);

    return DaInt;
}

// --------------------------JOSH ZONE --------------------------

#define PI 3.1415926535897932384626

void _move_forward(tracked_t *self, double dist_mm) {
    move_forward_sensor(self->sensor_data, dist_mm,self);
    self->x_pos_mm += dist_mm * cos(self->degrees * (2*PI / 360));
    self->y_pos_mm += dist_mm * sin(self->degrees * (2*PI / 360));
}

/*void _move_reverse(tracked_t *self, double dist_mm) {
    Some_back(self->sensor_data, dist_mm,self);
    self->x_pos_mm -= self->sensor_data * cos(self->degrees * (2*PI / 360));
    self->y_pos_mm -= self->sensor_data * sin(self->degrees * (2*PI / 360));
}*/

void _turn_left(tracked_t *self, double degrees) {
    double angle=0;
   angle = turn_left_sensor(self->sensor_data, degrees,self);

    if(self->degrees + angle <= 360) {
        self->degrees += angle;
    } else {
        self->degrees = self->degrees + angle - 360;
    }
}

void _turn_right(tracked_t *self, double degrees) {
    double angle=0;
    angle = turn_right_sensor(self->sensor_data, degrees,self);

    if(self->degrees - angle >= 0) {
        self->degrees -= angle;
    } else {
        self->degrees = self->degrees - angle + 360;
    }
}

tracked_t *new_tracked(oi_t *sensor_data) {
    tracked_t *t = (tracked_t *) malloc(sizeof(tracked_t));
    t->x_pos_mm = 0;
    t->y_pos_mm = 0;
    t->degrees = sensor_data -> angle;
    t->sensor_data = sensor_data;

    t->x_hit;
    t->y_hit;


    t->move_fwd = &_move_forward;
   // t->move_rev = &_move_reverse;
    t->turn_left = &_turn_left;
    t->turn_right = &_turn_right;

    return t;
}

double Distance_home (oi_t *sensor_data,tracked_t *t)
{
    double Home_dist = sqrt((pow(t->x_pos_mm,2.00) + pow(t->y_pos_mm,2.00)));
    return Home_dist;
}
double Face_degree (oi_t *sensor_data, double degrees, tracked_t *t)
{
    oi_update(sensor_data);
    double differance;
    double over_ang;
    differance = degrees - t->degrees;      //if less than 180 this is good, best way to turn to it, if > more complex


    if (differance <= 180 && (differance > -180))       //t->degrees < degrees
    {
        if (differance > 0)
        {
            t->turn_left(t,differance);
        }
        else if (differance <=0)
        {
            differance = abs(differance);
            t->turn_right(t,differance);
        }
    }

        if (differance > 180 || differance < -180)
        {
            if (differance < -180)
            {
            over_ang = 360- abs(differance);
            t->turn_left(t,over_ang);
            }

            else if (differance > 180)
            {
                over_ang = 360- abs(differance);
                t->turn_right(t,over_ang);
            }
        }
}

double Face_home (oi_t *sensor_data,tracked_t *t)
{
    oi_update(sensor_data);
    double Theta;                                    //angle from position to 0,0
    Theta = abs(atan((t->x_pos_mm)/(t->y_pos_mm)) * (180/PI));                        //ang compared to current position

            //QUAD ONE -----------------------------------
    if ((t->x_pos_mm) > 0 && (t->y_pos_mm) > 0)
    {
        Theta = 270 - Theta;
        Face_degree(sensor_data,Theta,t);           //should face to home
    }
        //QUAD 2 -----------------------------------------------
    if ((t->x_pos_mm) < 0 && (t->y_pos_mm) > 0)
    {
        Theta = 270 + Theta;
        Face_degree(sensor_data,Theta,t);           //should face to home
    }

        //QUAD 3 -------------------------------------------------
    if ((t->x_pos_mm) < 0 && (t->y_pos_mm) < 0)
    {
        Theta = 90 - Theta;
        Face_degree(sensor_data,Theta,t);           //should face to home
    }

        //QUAD 4 ----------------------------------------------------
    if ((t->x_pos_mm) > 0 && (t->y_pos_mm) < 0)
    {
        Theta = 90 + Theta;
        Face_degree(sensor_data,Theta,t);           //should face to home
    }
}

