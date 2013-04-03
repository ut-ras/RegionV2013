//**********************************************************************
//  Location Header File:
//  
//  Boost vector general includes, description of functions
//  Created 10-March-2013
//  IEEE - RAS
//**********************************************************************

class Vector {
public:
    double x;
    double y;

    double getMag(void);
    double getAngle(void);
    Vector(void) {x = 0; y = 0; }
};

Vector cart(double x, double y);
    
Vector subVectors(Vector &x, Vector &y); 
Vector addVectors(Vector &x, Vector &y);

double distance(Vector &x, Vector &y);


double ptam_heading(void);
Vector ptam_location(void);
Vector getDiskVector(int);
void InitCall(void);
void avoidObs(void);
bool diskPickUp(void);



struct Target {
    enum {
        END_OF_GAME,
        DELRIN,
        NO_DELRIN,
    } type;

    Vector dest;
};

Target get_target(Vector &location, double &rotation);
