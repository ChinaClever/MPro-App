/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_line.h"

Data_Line::Data_Line()
{

}

void Data_Line::lineWork()
{
    int size = mDev->info.lineNum;
    if(size == 0) size = 1;
    int num = mDev->info.loopNum / size;
    for(int i=0; i<size; ++i) {
        int start = i * num;
        int end = (i+1) * num;
        lineData(i, start, end);
    }
}
