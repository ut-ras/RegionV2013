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

#include <comm/ptam.h>

static comm::master comm("ptam");
 
Vector ptam_heading(void) {
    double data[3];
    comm.read('p', data, sizeof(data));

    return Vector(1.0, data[2]);
}

Vector ptam_location(void) {
    double data[3];
    comm.read('p', data, sizeof(data));

    return Vector(
        sqrt(data[0]*data[0] + data[1]*data[1])
        atan2(data[1], data[0])
    );
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

void Vector::operator=(Vector x){
    this->mag = x.mag;
    this->angle = x.angle;
}

Vector getDiskVector(){
    Vector filler;
    return filler;
} 

r() {
    // Pop next disk location off of stack retrieved from A-star
}
