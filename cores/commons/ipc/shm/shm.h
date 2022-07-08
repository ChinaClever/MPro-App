#ifndef SHM_H
#define SHM_H
#include "commons.h"

class Shm
{
public:
    static void *sharedMemory();
    static void initShm();
};

#endif // SHM_H
