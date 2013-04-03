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
#include <cmath>
using namespace std;
#include "inc/navigation.h"

#include <comm/ptam.h>

static comm::master ptam("ptam");
 
double ptam_heading(void) {
    float data[3];
    ptam.read('p', data, sizeof(data));

    return (double)data[2];
}

Vector ptam_location(void) {
    float data[3];
    ptam.read('p', data, sizeof(data));

    return cart((double)data[0], (double)data[1]);
}


Vector cart(double x, double y) {
    Vector temp;
    temp.x = x;
    temp.y = y;
    return temp;
}

double Vector::getMag(void) {
    return sqrt(x*x + y*y);
}

double Vector::getAngle(void) {
    return atan2(y, x);
}


Vector subVectors(Vector &a, Vector &b) {
    return cart(a.x-b.x, a.y-b.y);
}

Vector addVectors(Vector &a, Vector &b){
    return cart(a.x+b.x, a.y+b.y);
}

double distance(Vector &a, Vector &b) {
    double x = a.x-b.x;
    double y = a.y-b.y;
    return sqrt(x*x + y*y);
}
