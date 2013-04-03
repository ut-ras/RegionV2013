#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "inc/navigation.h"
#include <comm/board.h>  
#define ERR_A 5
#define kp 100       // P-constant 
#define UDELAY 500

extern bool board_checkDisk();

        



//************************************************************
//  More efficient plan (if time):                          //
//  --> PID line follow using position vectors              //
//************************************************************
int main(void) {
    int i;
    Vector vHome (8,0.78539816339);   // pi/4 radians, 8 inches in square(if 0 at corner). 
                                      //If 0 at initial start, make len=0
    char r_speed=100;
    char l_speed=100;
    Vector vDisk;
    Vector ERR_V (1,0.08727);   // 5 degree error (in radians), mag error of +/- 1
    InitCall();   
    for(i=0; i < 7; i++) {   // number of disks, increment i when disk picked up, when disk=6, return 
        
        /* Rather than the if-else statement, push the home vector onto the disk array */
        /*Vector vRobot = ptam_location();
        if(i == 6) {
            // Received all disks, pop home vector
            vDisk = vHome;            
        } else {
            vDisk = getDiskVector(i);
        }*/ 
        while((vRobot <= (vDisk + ERR_V)) ||(vRobot >= (vDisk - ERR_V))) {
            Vector vRobot = ptam_location();
            Vector vHeading = ptam_heading();         // Vector Class definitions in location.h
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
            if(!obstacle) {
                board.write(MOTOR_RIGHT, &r_speed,1);
                board.write(MOTOR_LEFT, &l_speed,1);
                usleep(UDELAY);
            } 
            avoidObs();
        }
         if(!diskPickUp()) {
            //crap
        } 
    }
}

void InitCall() {
    // Initialize PTAM
    // Read-file
    // Generate A-star Path, returns array of locations in order
    // Init Timer-int routine, after nth call (equates to 4minutes 15sec, return)
    // Sonar_Init (master command to RAS board)
    comm::master board("board"); 
}

bool diskPickUp(void) {
    // Comm with RAS board, call pickup function
    // c-function call from RASboard returns true or false
    return (board_checkDisk());    
}
