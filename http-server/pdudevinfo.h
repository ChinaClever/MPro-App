#ifndef PDUDEVINFO_H
#define PDUDEVINFO_H
#include "pdurpcobj.h"

class PduDevInfo : public PduRpcObj
{
public:
    static void devInfoExport();
    static void pduGetInfo(jsonrpc_request *r );
};
#endif // PDUDEVINFO_H
