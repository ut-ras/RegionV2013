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
};

class PTAM {
private:
    double x;
    double y;
    double rotation;
public:
    PTAM(void){ x = 0; y = 0; rotation = 0;};
    Vector getHeading(void);
    Vector getRobotVector(void);     // Access x,y location of robot, return Vector Class
};
