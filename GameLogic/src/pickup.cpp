#include <cstdio>
#include <comm/master.h>

#define X_TARGET 450
#define Y_TARGET 200
#define VARIANCE 5

//comm::master board("board");

bool center() {
    int pos[2];
    comm::master comm("opencv");
    while(true) {
        comm.read('d', pos, 2*sizeof(int));
        printf("X:%d Y:%d\n", pos[0], pos[1]);
        if(pos[0] - VARIANCE > X_TARGET) {
            //Turn right
            continue;
        } else if(pos[0] + VARIANCE < X_TARGET) {
            //Turn left
            continue;
        } else if(pos[1] - VARIANCE > Y_TARGET) {
            //
            continue;
        } else if(pos[1] + VARIANCE < Y_TARGET) {
            //
            continue;
        } else {
            return true;
        }
    }
    return false; //No idea why this would happen
}

int main() {
    if(center()) {
    //Call diskPickUp
    } else {
        printf("Centering failed");
    }
}
