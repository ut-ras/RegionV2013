#ifndef COMM_MASTER_H
#define COMM_MASTER_H

#include <stdint.h>
#include <cstdio>

namespace comm {

class master {
private:
    // File descriptor
    int comm;

    // prevent copy constructors
    master(const master&);
    master &operator=(const master&);

public:
    // Constructor and destructor
    master(const char *comm_name);
    ~master(void);

    // Number of attempts to make per request
    int attempts;

    // Timeout for requests in milliseconds
    int timeout; 

    // Sets a device to the specified value
    bool write(char device, void *value, size_t len);

    // Reads a value from a specified device
    bool read(char device, void *value, size_t len);

    // Pings the channel and responds true if successful
    bool ping(void);

    // Resets anything on the channel. Good for emergency stops.
    void reset(void);
};

}

#endif
