#include <comm/master.h>

#include <cstdlib>
#include <cstdio>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [comm]\n", argv[0]);
        exit(1);
    }

    comm::master comm(argv[1]);

    while (1) {
        char cmd, dev;
        int temp;

        uint8_t *data;
        int len;

        printf("req cmd: ");
        while (scanf(" %c", &cmd) != 1);

        if (cmd == 'r') {
            printf("    dev: ");
            while (scanf(" %c", &dev) != 1);
            printf("  count: ");
            while (scanf(" %d", &len) != 1);

            data = new uint8_t[len];

            printf("-> {r%c}\n", dev);
            if (comm.read(dev, data, len)) {
                printf("<- {d%c", dev);
                for (int i=0; i<len; i++)
                    printf("%02x", data[i]);
                printf("}\n");
            } else {
                printf("<- {x}\n");
            }

            delete[] data;
        } else if (cmd == 'w') {
            printf("    dev: ");
            while (scanf(" %c", &dev) != 1);
            printf("  count: ");
            while (scanf(" %d", &len) != 1);

            data = new uint8_t[len];

            for (int i=0; i<len; i++) {
                printf("data[%d]: ", i);
                while (scanf(" %x", &temp) != 1);
                data[i] = 0xff & temp;
            }

            printf("-> {w%c", dev);
            for (int i=0; i<len; i++)
                printf("%02x", data[i]);
            printf("}\n");

            if (comm.write(dev, data, len)) {
                printf("<- {a%c}\n", dev);
            } else {
                printf("<- {x}\n");
            }

            delete[] data;
        } else if (cmd == 'p') {
            printf("-> {p}\n");
            if (comm.ping())
                printf("<- {c}\n");
            else
                printf("<- {x}\n");
        } else if (cmd == '!') {
            printf("-> {!}\n");
            comm.reset();
        } else {
            printf("Bad command: %c\n", cmd);
        }
    }
}
