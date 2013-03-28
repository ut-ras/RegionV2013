//**********************************************************************
//  Location Header File:
//  
//  Boost vector general includes, description of functions
//  Created 10-March-2013
//  IEEE - RAS
//**********************************************************************

class Vector {
private:
    double mag;
    double angle;
public:
    double getMag(void);
    double getAngle(void);
    Vector (void) {mag = 0; angle = 0; }; 
    Vector (double inMag, double inAngle) { mag = inMag; angle = inAngle; }; 
    Vector subVectors(Vector* x, Vector y); 
    Vector addVectors(Vector* x, Vector y);
    Vector operator+(Vector x);
    Vector operator-(Vector x);
    bool operator<=(Vector x);
    bool operator>=(Vector x);
    void operator=(Vector x);
};


Vector ptam_heading(void);
Vector ptam_location(void);
Vector getDiskVector(int);
void InitCall(void);
void avoidObs(void);
void diskPickUp(void);

