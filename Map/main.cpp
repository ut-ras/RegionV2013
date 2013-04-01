#include <cstdlib>
#include <tinyxml2.h>

#include "mapper.h"

#include <comm/slave.h>



int main() {

    comm::slave comm("map");
    
    mapper test("map.xml", "Locatio.csv");



    printf("\ninitialized\n\n");


    comm.loop();
}
