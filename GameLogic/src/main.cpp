#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "inc/navigation.h"
#include <comm/board.h>  
#define ERR_A 5
#define ERR_D 1
#define kp 100       // P-constant 
#define UDELAY 500

extern bool board_checkDisk();
        
comm::master board("board");


int main(void) {
    Vector vHome (8,0.78539816339);   // pi/4 radians, 8 inches in square(if 0 at corner). 
                                      //If 0 at initial start, make len=0
    char r_speed;
    char l_speed;

    Vector ERR_V (1,0.08727);   // 5 degree error (in radians), mag error of +/- 1


    while(1) {       
        Vector vRobot = ptam_location();
        Vector vHeading = ptam_heading();         // Vector Class definitions in location.h
        Target disk = get_target(vRobot, vHeading);

        Vector vDesired = subVectors(vRobot, disk.dest); 
        double angleDiff = vHeading.getAngle() -  vDesired.getAngle();

        while((angleDiff >= ERR_A) || (angleDiff <= -ERR_A)) {
            angleDiff = vHeading.getAngle() - vDesired.getAngle(); 
            if(angleDiff < 0) {
                // turn right using PID
                r_speed = kp*abs(angleDiff);  //kp = PID const must be char for comm
                l_speed = -(kp*abs(angleDiff));
                board.write(MOTOR_RIGHT,&r_speed,1);
                board.write(MOTOR_LEFT,&l_speed,1);
                // Send Right motor controls to RAS board
            } else if(angleDiff > 0)  {             // turn left using PID
                l_speed = kp*abs(angleDiff);  //kp = PID const must be char for comm
                r_speed = -(kp*abs(angleDiff));
                board.write(MOTOR_LEFT,&l_speed,1);
                board.write(MOTOR_RIGHT, &r_speed,1);
                // Send left motor controls to RAS board
            }
        }

        while(distance(vRobot,vDesired) > ERR_D) {
            char speed = 100;
            board.write(MOTOR_RIGHT,&speed,1);
            board.write(MOTOR_LEFT,&speed,1);
        } 

        r_speed = 0; 
        l_speed = 0;
        board.write(MOTOR_RIGHT, &r_speed,1);
        board.write(MOTOR_LEFT, &l_speed,1);

        if(disk.type == Target::DELRIN) {
            if(!diskPickUp()) {
                //move and retry
            } 
        } 

        if(disk.type == Target::END_OF_GAME) {
            exit(0);
        }
    }
}

bool diskPickUp(void) {
    // Comm with RAS board, call pickup function
    // c-function call from RASboard returns true or false
    char speed = 0x50;
    board.write(CRANE,&speed,1);  // runs crane pickup command at speed 0x50
    return (true); // TODO: make comm.read function call to receive bool from board   
}
