#include <comm/master.h>
#include "inc/navigation.h"


static comm::master mapper("map");


Target get_target(Vector &location, double &heading) {
    Target temp;
    temp.type = Target::DELRIN;
    temp.dest = cart(30, 30);
    return temp;
}

