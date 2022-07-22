//
// Created by admin on 2022/7/9.
//

#ifndef H743IIT6_EMPTY_DISPLAY_H
#define H743IIT6_EMPTY_DISPLAY_H

#include "main.h"

//显示设备通用参数
typedef struct
{
  uint16_t width;     //宽
  uint16_t height;    //高
  uint8_t  dir;       //方向
}DISPLAY_HandleTypeDef;

extern DISPLAY_HandleTypeDef display_dev;
#endif // H743IIT6_EMPTY_DISPLAY_H
