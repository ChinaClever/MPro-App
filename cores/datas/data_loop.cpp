/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_loop.h"

Data_Loop::Data_Loop()
{

}

void Data_Loop::loopWork()
{
    int size = mDev->info.loopNum;
    for(int i=0; i<size; ++i) {
        int start = mDev->info.loopStarts[i];
        int end = mDev->info.loopEnds[i];
        loopData(i, start, end);
    }
}
