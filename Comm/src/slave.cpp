#include "comm/slave.h"

#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cerrno>
#include <stdexcept>

namespace comm {

// Some default functions
bool def_ping() {
    return 1;
}

void def_reset() {}



slave::slave(const char *comm_name) {
    struct sockaddr_un addr;

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, COMM_PATH"/");
    strcat(addr.sun_path, comm_name);

    size_t len = strlen(addr.sun_path) + 1;
    filename = new char[len];
    strcpy(filename, addr.sun_path);
    len += sizeof(addr.sun_family);

    unlink(filename);

    comm = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (comm < 0) throw std::runtime_error(strerror(errno));

    int err = bind(comm, (struct sockaddr*)&addr, len);
    if (err < 0) throw std::runtime_error(strerror(errno));

    err = listen(comm, 5);
    if (err < 0) throw std::runtime_error(strerror(errno));

    channels.insert(comm);


    pcall = &def_ping;
    rcall = &def_reset;
}

slave::~slave(void) {
    // Stupid c++ iterator names
    std::set<int>::iterator sit;
    for (sit = channels.begin(); sit != channels.end(); ++sit) {
        close(*sit);
    }

    close(comm);
    delete[] filename;
}

void slave::reg_write(char dev, bool (*call)(char, void*), size_t len) {
    if (!call) {
        wmap.erase(dev);
    } else {
        struct dev &val = wmap[dev];
        val.size = len;
        val.call = call;
    }
}

void slave::reg_read(char dev, bool (*call)(char, void*), size_t len) {
    if (!call) {
        rmap.erase(dev);
    } else {
        struct dev &val = rmap[dev];
        val.size = len;
        val.call = call;
    }
}

void slave::reg_ping(bool (*call)(void)) {
    pcall = call ? call : def_ping;
}

void slave::reg_reset(void (*call)(void)) {
    rcall = call ? call : def_reset;
}


bool slave::handle(char cmd, int chan) {
    std::map<char, struct dev>::iterator iter;
    
    switch (cmd) {
        case 'w':
            if (read(chan, &cmd, 1) != 1) return 0;
            iter = wmap.find(cmd);

            if (iter != wmap.end()) {
                size_t len = iter->second.size;
                char *data = new char[len];
                if (read(chan, data, len) != (int)len) {
                    delete[] data;
                    return 0;
                }

                if (iter->second.call(cmd, data)) {
                    char resp[2] = {'a', cmd};
                    if (write(chan, resp, 2)); // ignore
                    delete[] data;
                    return 1;
                }

                delete[] data;
            }
            return 0;

        case 'r':
            if (read(chan, &cmd, 1) != 1) return 0;
            iter = rmap.find(cmd);

            if (iter != rmap.end()) {
                size_t len = iter->second.size + 2;
                char *data = new char[len];
                data[0] = 'd';
                data[1] = cmd;

                if (iter->second.call(cmd, data+2)) {
                    if (write(chan, data, len)); // ignore
                    delete[] data;
                    return 1;
                }

                delete[] data;
            }
            return 0;

        case 'p':
            if (pcall()) {
                char resp = 'c';
                if (write(chan, &resp, 1)); // ignore
                return 1;
            }
            return 0;

        case '!':
            rcall();
            return 1;

        default:
            return 0;
    }
}

void slave::check(int ms) {
    // Create the descriptor set
    fd_set chset;
    FD_ZERO(&chset);

    // Stupid c++ iterator names
    std::set<int>::iterator sit;

    for (sit = channels.begin(); sit != channels.end(); ++sit) {
        FD_SET(*sit, &chset);
    }


    struct timeval time;
    time.tv_sec = ms / 1000;
    time.tv_usec = (ms % 1000) * 1000;

    select(1+*channels.rbegin(), &chset, 0, 0, ms == -1 ? 0 : &time);


    for (sit = channels.begin(); sit != channels.end(); ++sit) {
        if (FD_ISSET(*sit, &chset)) {

            // New Connection
            if (*sit == comm) {
                int new_channel = accept(comm, 0, 0);

                if (new_channel > 0)
                    channels.insert(new_channel);

            // Communication
            } else {
                char cmd;

                if (read(*sit, &cmd, 1) != 1 || !handle(cmd, *sit)) {

                    if (errno == EAGAIN) { // socket was closed
                        channels.erase(sit);
                    } else {
                        char resp = 'x';
                        if (write(*sit, &resp, 1)); // ignore
                    }
                }
            }
        }
    }
}

void slave::check(void) {
    check(0);
}

void slave::loop(void) {
    while (1) {
        check(-1);
    }
}

}
