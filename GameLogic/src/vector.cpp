//***********************************************************************
//  Vector.cpp
//  Vector functions:
//      - public functions to access vector data
//      - vector manipulation functions
//      - PTAM function calls
//
//  Created 23-March-2013
//  IEEE - RAS
//***********************************************************************

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "inc/navigation.h"
 
PTAM getPTAM(void) {
    
}

double Vector::getMag(Vector x) {
    return x.mag;
}

double Vector::getAngle(Vector x) {
    return x.angle;
}

Vector Vector::subVectors(Vector x, Vector y) {
    Vector vResult;
    vResult.mag = x.mag - y.mag;
    vResult.angle = x.angle - y.angle;
    return vResult;
}

Vector Vector::addVectors(Vector x, Vector y) {
    Vector vResult;
    vResult.mag = x.mag + y.mag;
    vResult.angle = x.angle + y.angle;
    return vResult;
}

Vector getDiskVector() {
    // Pop next disk location off of stack retrieved from A-star
}

Vector getHeading(PTAM x) {
    Vector result;
    result.mag = 0;
    result.angle = x.rotation;
    return result
}

Vector getRobotVector(PTAM PTAMdata) {
    Vector vRobot;
    double magsquared = (PTAMdata.x)*(PTAMdata.x) + (PTAMdata.y)*(PTAMdata.y);
    vRobot.mag = sqrt(magsquared);
    vRobot.angle = atan(PTAMdata.y/PTAMdata.x);
    return vRobot;
}

Vector Vector::operator+(Vector x, Vector y){
   return addVectors(x, y); 
}

Vector Vector::operator-(Vecotr x, Vector y){
    return subVectors(x, y);
}
