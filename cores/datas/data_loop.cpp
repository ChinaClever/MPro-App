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
    if(size == 0) size = 1;
    int num = mDev->info.outputNum / size;
    for(int i=0; i<size; ++i) {
        int start = i * num;
        int end = (i+1) * num;
        loopData(i, start, end);
    }
}
