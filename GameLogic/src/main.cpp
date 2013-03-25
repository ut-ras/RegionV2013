#include <stdio.h>
#include <stdint.h>
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
    Vector ERR_V (1,0.08727);   // 5 degree error (in radians), mag error of +/- 1
    // InitCall();   // 8======D
    for(i=0; i < 6; i++) {   // number of disks, increment i when disk picked up
        PTAM PTAMdata = getPTAM();
        Vector vRobot = getRobotVector(PTAMdata);
        Vector vDisk = getDiskVector();
        while((vRobot <= (vDisk+ERR_V)) ||(vRobot >= (vDisk-ERR_V))) {
            PTAM PTAMdata = getPTAM();
            Vector vHeading = getHeading(PTAMdata);         // Vector Class definitions in location.h
            vRobot = getRobotVector(PTAMdata);
            Vector vDesired = (vRobot-vDisk); 
            vDisk = getDiskVector();
            double angleDiff = getAngle(vHeading) -  getAngle(vDesired);
            while((angleDiff >= ERR_A) || (angleDiff <= -ERR_A)) {
                angleDiff = getAngle(vHeading) - getAngle(vDesired); 
                if(angleDiff < 0) {
                    // turn right using PID
                    char speed = kp*angleDiff;  //kp = PID const must be char for comm
                    board.write(MOTOR_RIGHT,&speed,1);
                    // Send Right motor controls to RAS board
                } else if(angleDiff > 0) {
                    // turn left using PID
                    char speed = kp*angleDiff;  //kp = PID const must be char for comm
                    board.write(MOTOR_LEFT,&speed,1);
                    // Send left motor controls to RAS board
                }
            } 
            // Check Map for obstacles, if flag returned stating there is an obstacle
            while(!obstacle) {
                drive();
            } 
            avoidObs();
        }
        diskPickUp();
        vDisk = GetDiskVector(i);
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

