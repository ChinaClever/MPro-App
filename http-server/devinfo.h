#ifndef DEVINFO_H
#define DEVINFO_H
#include "commons.h"
#include "jsonrpcobj.h"

enum sDevSizeType{Line_Num, Loop_Num, Output_Num};

class DevInfo
{
public:
    static void init();
    static int getLineNum(int id);
    static int getLoopNum(int id);
    static int getOpNum(int id);
    static void pduGetInfo(jsonrpc_request *r );
};
#endif // DEVINFO_H
