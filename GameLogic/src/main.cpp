#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "inc/navigation.h"
#include <comm/board.h>  
#define ERR_A 5
#define kp 1 

//************************************************************
//  More efficient plan (if time):                          //
//  --> PID line follow using position vectors              //
//************************************************************
int main(void) {
    int i;
    char r_speed=100;
    char l_speed=100;
    Vector ERR_V (1,0.08727);   // 5 degree error (in radians), mag error of +/- 1
    // InitCall();   // 8======D
    for(i=0; i < 6; i++) {   // number of disks, increment i when disk picked up
        Vector PTAMloc = ptam_location();
      }
        Vector vRobot = PTAMdata;
        Vector vDisk = getDiskVector(i);
        while((vRobot <= (vDisk + ERR_V)) ||(vRobot >= (vDisk - ERR_V))) {
            Vector PTAMhead = ptam_heading();
            Vector PTAMloc = ptam_location();
            Vector vHeading = PTAMhead;         // Vector Class definitions in location.h
            vRobot = PTAMloc;
            Vector vDesired = (vRobot - vDisk); 
            double angleDiff = vHeading.getAngle() -  vDesired.getAngle();
            while((angleDiff >= ERR_A) || (angleDiff <= -ERR_A)) {
                angleDiff = vHeading.getAngle() - vDesired.getAngle(); 
                if(angleDiff < 0) {
                    // turn right using PID
                    char speed = kp*abs(angleDiff);  //kp = PID const must be char for comm
                    board.write(MOTOR_RIGHT,&speed,1);
                    // Send Right motor controls to RAS board
                } else if(angleDiff > 0) {
                    // turn left using PID
                    char speed = kp*abs(angleDiff);  //kp = PID const must be char for comm
                    board.write(MOTOR_LEFT,&speed,1);
                    // Send left motor controls to RAS board
                }
            } 
            // Check Map for obstacles, if flag returned stating there is an obstacle
            while(!obstacle) {
                board.write(MOTOR_RIGHT, &r_speed,1);
                board.write(MOTOR_LEFT, &l_speed,1);
            } 
            avoidObs();
        }
        diskPickUp();
    }
    returnhome();
}

void InitCall() {
    // Initialize PTAM
    // Read-file
    // Generate A-star Path, returns array of locations in order
    // Init Timer-int routine, after nth call (equates to 4minutes 15sec, return)
    // Sonar_Init (master command to RAS board)
    comm::master board("board"); 
}

