#include <comm/master.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <signal.h>

int main() {
    comm::master comm("board_raw");

    if (!comm.ping()) {
        fprintf(stderr, "RAS Board not responding!\n");
        exit(1);
    }

    printf("\033[?25l\n");
    printf("Driving Spokebot\n\n");
    printf("controls:");
    printf("    w a s d = move\n");
    printf("    space   = stop\n");
    printf("    q e     = crane\n\n");

    if (system("stty raw")); // ignore
        
    unsigned char left = 128;
    unsigned char right = 128;
    unsigned char crane = 0;

    while (1) {
        printf("\r[ %3d %3d | %3d] : ", left, right, crane);
        comm.write('m'+0, &left, 1);
        comm.write('m'+1, &right, 1);
        comm.write('c', &crane, 1);

        switch (getc(stdin)) {
            case 'w':
                left = 255;
                right = 255;
                break;
            case 'a':
                left = 192;
                right = 64;
                break;
            case 'd':
                left = 64;
                right = 192;
                break;
            case 's':
                left = 0;
                right = 0;
                break;
            case ' ':
                left = 128;
                right = 128;
                break;
            case 'q':
                crane = 0;
                break;
            case 'e':
                crane = 255;
                break;
            case 3: //die
                if(system("stty cooked")); // ignore
                printf("\033[?25h\n\n");
                exit(0);
        }
    }
}
