#ifndef COMM_BLOCK_SLAVE_H
#define COMM_BLOCK_SLAVE_H

#include <comm/slave.h>

#include <sys/types.h>
#include <vector>
#include <map>

namespace comm {

// Internally used by block_slave
struct block_dev {
    size_t size;
    void *data;

    int locks;
    std::vector<int> l_pend;
    std::vector<int> q_pend;

    int id; 
    key_t key;

    bool (*call)(char, void*);
};

class block_slave: public slave {
private:
    // internally used function map
    std::map<char, struct block_dev> mmap;

    // prevent copy constructors
    block_slave(const block_slave&);
    block_slave &operator=(const block_slave&);

protected:
    // Overwrites internal handler
    virtual bool handle(char, int);

public:
    // Constructor and destructor
    block_slave(const char *comm_name): slave(comm_name) {}
    ~block_slave(void);

    // Register a memory handler
    void reg_mem(char dev, bool (*call)(char, void*), size_t len);

    // Template function for registering
    template <class T>
    void reg_mem(char dev, bool (*call)(char, T*), size_t len) {
        reg_mem(dev, (bool (*)(char, void*))call, len);
    }

    // Access a memory block allocated by reg_mem
    // Warning! the block is only locked when given through 
    // the reg_mem callback. Use only if you know its safe
    void *access(char dev);
};

}

#endif
