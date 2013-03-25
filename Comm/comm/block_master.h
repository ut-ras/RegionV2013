#ifndef COMM_BLOCK_MASTER_H
#define COMM_BLOCK_MASTER_H

#include <comm/block.h>
#include <comm/master.h>

namespace comm {

class block_master: master {
private:
    // block storage
    block mem_block;

    // prevent copy constructors
    block_master(const block_master&);
    block_master &operator=(const block_master&);

public:
    // forward constructor
    block_master(const char *comm_name): master(comm_name) {}

    bool get_block(block *mem);

};

}

#endif
