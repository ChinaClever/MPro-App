#ifndef PROCUSB_H
#define PROCUSB_H

#include "procstart.h"

class ProcUsb : public ProcStart
{
public:
    explicit ProcUsb();

private:
    void usb_run();
};

#endif // PROCUSB_H
