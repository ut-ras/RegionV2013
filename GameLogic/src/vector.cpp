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
    PTAM filler; 
    return filler;        
}

double Vector::getMag(void) {
    return this->mag;
}

double Vector::getAngle(void) {
    return this->angle;
}

Vector Vector::subVectors(Vector* x, Vector y){
    Vector vResult;
    vResult.mag = x->mag - y.mag;
    vResult.angle = x->angle - y.angle;
    return vResult;
}

Vector Vector::addVectors(Vector* x, Vector y){
    Vector vResult; 
    vResult.mag = x->mag + y.mag;
    vResult.angle = x->angle + y.angle;
    return vResult;
}

Vector Vector::operator+(Vector x){
    return addVectors(this, x);
}

Vector Vector::operator-(Vector x){
    return subVectors(this, x);
}

bool Vector::operator<=(Vector x){
    if(this->mag <= x.mag){     // implicit parameter this, i.e., class Vector
        if(this->angle <= x.angle){
            return true;
        }
    }else{
        return false;
    }
}

bool Vector::operator>=(Vector x){
    if(this->mag >= x.mag){
        if(this->angle >= x.angle){
            return true;
        }
    }else{
        return false;
    }
}

Vector getDiskVectoVector Vector::subVectors(Vector* x, Vector y){
    Vector vResult;
    vResult.mag = x->mag - y.mag;
    vResult.angle = x->angle - y.mag;
    return vResult;
}
r() {
    // Pop next disk location off of stack retrieved from A-star
}

Vector PTAM::getHeading(void) {
    Vector result;
    result.mag = 0;
    result.angle = this->rotation;  // implicit parameter this, i.e., class PTAM
    return result;
}

Vector PTAM::getRobotVector(void) {
    Vector vRobot;
    double magsquared = (this->x)*(this->x) + (this->y)*(this->y); // implicit parameter this, i.e., class PTAM
    vRobot.mag = sqrt(magsquared);
    vRobot.angle = atan(this->y/this->x);
    return vRobot;
}
