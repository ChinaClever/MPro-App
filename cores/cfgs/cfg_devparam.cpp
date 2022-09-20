#include "cfg_devparam.h"

Cfg_devParam::Cfg_devParam()
{

}


bool Cfg_devParam::initialParam()
{
    sDevData *dev = cm::masterDev();
    dev->cfg.param.isBreaker = 1;
    initDevInfo(dev->cfg.nums);
    return true;
}

void Cfg_devParam::initDevInfo(sDevNums &it)
{
    it.boardNum = 3;
    it.lineNum = LINE_NUM;
    it.loopNum = LOOP_NUM / 2;
    it.outputNum = OUTPUT_NUM / 2;
    for(uint i=0; i<it.loopNum; ++i) {
        it.loopEnds[i] = 8*(i+1);
        it.loopStarts[i] = 8*i;
    }
}
