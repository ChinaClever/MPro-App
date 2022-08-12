#ifndef SHM_H
#define SHM_H
#include "commons.h"

class Shm
{
public:
    static void *sharedMemory();
    static void initShm();
    static void delShm();
};

#endif // SHM_H
