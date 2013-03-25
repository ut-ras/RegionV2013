#include <comm/block_slave.h>

#include <cstring>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <cerrno>
#include <stdexcept>

namespace comm {

block_slave::~block_slave(void) {
    // Stupid c++ iterator names
    std::map<char, struct block_dev>::iterator mit;
    for (mit = mmap.begin(); mit != mmap.end(); ++mit) {
        shmdt(mit->second.data);
        shmctl(mit->second.id, IPC_RMID, 0);
    }
}

void block_slave::reg_mem(char dev, bool (*call)(char, void*), size_t len) {
    if (!call) {
        mmap.erase(dev);
    } else {
        struct block_dev &val = mmap[dev];
        val.size = len;
        val.call = call;
        val.locks = 0;

        val.key = ftok(filename, dev);
        if (val.key == -1) throw std::runtime_error(strerror(errno));

        val.id = shmget(val.key, len, 0666 | IPC_CREAT);
        if (val.id == -1) throw std::runtime_error(strerror(errno));

        val.data = shmat(val.id, 0, 0);
        if (val.data == (void*)(-1)) throw std::runtime_error(strerror(errno));
    }
}

void *block_slave::access(char dev) {
    std::map<char, struct block_dev>::iterator iter;
    iter = mmap.find(dev);

    if (iter != mmap.end())
        return iter->second.data;
    else
        return 0;
}

bool block_slave::handle(char cmd, int chan) {
    std::map<char, struct block_dev>::iterator iter;

    switch (cmd) {
        case 'l':
            if (read(chan, &cmd, 1) != 1) return 0;
            iter = mmap.find(cmd);

            if (iter != mmap.end()) {
                if (iter->second.q_pend.size() <= 0) {
                    iter->second.locks++;

                    char l_resp[2+sizeof(key_t)];
                    l_resp[0] = 'd';
                    l_resp[1] = cmd;
                    memcpy(&l_resp[2], &iter->second.key, sizeof(key_t));

                    if (write(chan, l_resp, 2+sizeof(key_t))); // ignore
                } else {
                    iter->second.l_pend.push_back(chan);
                }
                return 1;
            }
            return 0;

        case 'u':
            if (read(chan, &cmd, 1) != 1) return 0;
            iter = mmap.find(cmd);

            if (iter != mmap.end()) {
                iter->second.locks--;

                char resp[2] = {'a', cmd};
                if (write(chan, resp, 2)); // ignore

                if (iter->second.locks <= 0 && iter->second.q_pend.size() > 0) {
                    if (iter->second.call(cmd, iter->second.data)) {
                        char l_resp[2+sizeof(key_t)];
                        l_resp[0] = 'd';
                        l_resp[1] = cmd;
                        memcpy(&l_resp[2], &iter->second.key, sizeof(key_t));

                        while (iter->second.q_pend.size() > 0) {
                            if (write(iter->second.q_pend.back(), l_resp, 2+sizeof(key_t))); // ignore
                            iter->second.q_pend.pop_back();
                        }
                            
                    }

                    iter->second.locks += iter->second.l_pend.size();
                    while (iter->second.l_pend.size() > 0) {
                        if (write(iter->second.l_pend.back(), resp, 2)); // ignore
                        iter->second.l_pend.pop_back();
                    }
                }
                return 1;
            }
            return 0;

        case 'q':
            if (read(chan, &cmd, 1) != 1) return 0;
            iter = mmap.find(cmd);

            if (iter != mmap.end()) {
                if (iter->second.locks > 0) {
                    iter->second.q_pend.push_back(chan);
                } else {
                    if (!iter->second.call(cmd, iter->second.data))
                        return 0;
                }

                char resp[2] = {'a', cmd};
                if (write(chan, resp, 2)); // ignore
                return 1;
            }
            return 0;

        default:
            return slave::handle(cmd, chan);
    }
}

}
