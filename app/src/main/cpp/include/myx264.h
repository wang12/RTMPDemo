//
// Created by wangqiangqiang on 2017/7/4.
//

#ifndef RTMPDEMO_MYX264_H
#define RTMPDEMO_MYX264_H

#include "x264.h"

int X264_Init(int, int);

int X264_Start();

int X264_Stop();

int X264_CodeingX264(unsigned char*,long,long);

int X264_Realse();


#endif //RTMPDEMO_MYX264_H
