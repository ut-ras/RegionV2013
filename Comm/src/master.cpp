#include "comm/master.h"

#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#include <cerrno>
#include <stdexcept>

namespace comm {

master::master(const char *comm_name) {
    struct sockaddr_un addr;

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, COMM_PATH"/");
    strcat(addr.sun_path, comm_name);

    size_t len = strlen(addr.sun_path) + sizeof(addr.sun_family);

    struct stat info;
    stat(addr.sun_path, &info);

    if (!S_ISSOCK(info.st_mode)) {
        comm = open(addr.sun_path, O_RDWR | O_NONBLOCK);
    } else {
        comm = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0); // Should check
        connect(comm, (struct sockaddr*)&addr, len);
    }

    attempts = 3;
    timeout = -1;
}

master::~master(void) {
    close(comm);
}

bool master::write(char device, void *value, size_t len) {
    char head[2] = {'w', device};
    char resp[2];

    if (comm < 0) throw std::runtime_error("bad comm");

    fd_set set;
    FD_ZERO(&set);
    FD_SET(comm, &set);

    for (int i=0; i<attempts || attempts < 0; i++) {
        if (::write(comm, head,  2) != 2) continue;
        if (::write(comm, value, len) != (int)len) continue;

        struct timeval time;
        time.tv_sec = timeout / 1000;
        time.tv_usec = (timeout % 1000) * 1000;

        select(comm+1, &set, 0, 0, timeout == -1 ? 0 : &time);

        if (::read(comm, resp, 2) != 2) continue;

        if (resp[0] == 'a' && resp[1] == device)
            return 1;
    }

    return 0;
}


bool master::read(char device, void *value, size_t len) {
    char val[2] = {'r', device};
    char resp[2];

    if (comm < 0) throw std::runtime_error("bad comm");

    fd_set set;
    FD_ZERO(&set);
    FD_SET(comm, &set);

    for (int i=0; i<attempts || attempts < 0; i++) {
        if (::write(comm, val, 2) != 2) continue;

        struct timeval time;
        time.tv_sec = timeout / 1000;
        time.tv_usec = (timeout % 1000) * 1000;

        select(comm+1, &set, 0, 0, timeout == -1 ? 0 : &time);

        if (::read(comm, resp, 2) != 2) continue;

        if (resp[0] == 'd' && resp[1] == device) {
            if (::read(comm, value, len) == (int)len) {
                return 1;
            }
        }
    }

    value = 0;
    return 0;
}

bool master::ping(void) {
    char val = 'p';

    if (comm < 0) return 0;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(comm, &set);

    if (::write(comm, &val, 1) != 1) return 0;

    struct timeval time;
    time.tv_sec = timeout / 1000;
    time.tv_usec = (timeout % 1000) * 1000;

    select(comm+1, &set, 0, 0, timeout == -1 ? 0 : &time);


    if (::read(comm, &val, 1) != 1) return 0;

    return val == 'c';
}

void master::reset(void) {
    char val = '!';
    if (::write(comm, &val, 1)); // ignore
}

}
