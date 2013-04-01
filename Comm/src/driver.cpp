#include <comm/master.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <signal.h>

int main(int argc, char **argv) {
    comm::master comm("board");
    unsigned char speed;

    if (argc > 1)
        speed = atoi(argv[1]) & 0xff;
    else
        speed = 120;


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
        
    signed char left = 0;
    signed char right = 0;
    signed char crane = 0;

    while (1) {
        printf("\r[ %3d %3d | %3d] : ", left, right, crane);
        comm.write('m'+0, &left, 1);
        comm.write('m'+1, &right, 1);
        comm.write('c', &crane, 1);

        switch (getc(stdin)) {
            case 'w':
                left = speed;
                right = speed;
                break;
            case 'a':
                left = speed;
                right = 0;
                break;
            case 'd':
                left = 0;
                right = speed;
                break;
            case 's':
                left = -speed;
                right = -speed;
                break;
            case ' ':
                left = 0;
                right = 0;
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
