#ifndef COMM_SLAVE_H
#define COMM_SLAVE_H

#include <stdint.h>
#include <cstdio>
#include <map>
#include <set>

namespace comm {

// Internally used by slave
struct dev {
    size_t size;
    bool (*call)(char, void*);
};

class slave {
private:
    // File designators
    int comm;
    std::set<int> channels;

    // Internally used function maps
    std::map<char, struct dev> wmap;
    std::map<char, struct dev> rmap;
    bool (*pcall)(void);
    void (*rcall)(void);

    // prevent copy constructors
    slave(const slave&);
    slave &operator=(const slave&);

protected:
    // Internally used helper funciton
    virtual bool handle(char, int);

public:
    // Filename for socket
    char *filename;

    // Constructor and destructor
    slave(const char *comm_name);
    ~slave(void);

    // Register a write device
    void reg_write(char dev, bool (*call)(char, void*), size_t len);

    // Register a read device
    void reg_read(char dev, bool (*call)(char, void*), size_t len);

    // Template functions for registering
    template <class T>
    void reg_write(char dev, bool (*call)(char, T*), size_t len) {
        reg_write(dev, (bool (*)(char, void*))call, len);
    }

    template <class T>
    void reg_read(char dev, bool (*call)(char, T*), size_t len) {
        reg_read(dev, (bool (*)(char, void*))call, len);
    }

    // Register a function for ping
    void reg_ping(bool (*call)(void));

    // Register a function for reset
    void reg_reset(void (*call)(void));

    // Check for any requests
    // Same as check(0)
    void check(void);

    // Check and wait for a given time in millis
    void check(int ms);

    // Loop on requests
    void loop(void);
};

}

#endif
