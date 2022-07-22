/*!
* @Create: 2021/12/13
* @Author: AkiNya
* @description: LCD显示驱动
*/

#ifndef __MCU_LCD_H__
#define __MCU_LCD_H__
#include "main.h"
#include <stdio.h>
//LCD参数结构体
typedef struct
{
 uint16_t width;			  //LCD 宽度
 uint16_t height;			//LCD 高度
 uint16_t id;				  //LCD ID
 uint8_t  dir;			    //横屏还是竖屏控制：0，竖屏；1，横屏。
 uint16_t wramcmd;		  //开始写gram指令
 uint16_t setxcmd;		  //设置x坐标指令
 uint16_t setycmd;		  //设置y坐标指令
}LCD_HandleTypeDef;

//寄存器结构体
typedef struct
{
 __IO uint16_t LCD_REG;
 __IO uint16_t LCD_RAM;
} LCD_TypeDef;

//F4
// #define LCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
//H7
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上
#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左
#define DFT_SCAN_DIR  L2R_U2D //默认从左到右,从上到下

//画笔颜色
#define WHITE   0xFFFF
#define BLACK   0x0000

//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		  //水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊
#define SSD_VER_PULSE_WIDTH		1		  //垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)
//LCD MPU保护参数
#define LCD_REGION_NUMBER		MPU_REGION_NUMBER0		//LCD使用region0
#define LCD_ADDRESS_START		(0X60000000)			//LCD区的首地址
#define LCD_REGION_SIZE			MPU_REGION_SIZE_256MB           //LCD区大小

void LCD_WR_REG(__IO uint16_t regval);
void LCD_WR_DATA(__IO uint16_t data);
uint16_t LCD_RD_DATA(void);
//初始化
void LCD_Init(void);
//设置屏扫描方向
void LCD_Scan_Dir(uint8_t dir);
//设置屏幕显示方向
void LCD_Display_Dir(uint8_t dir);
//设置窗口
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
//开显示
void LCD_DisplayOn(void);
//关显示
void LCD_DisplayOff(void);
//清屏
void LCD_Clear(uint32_t Color);
//设置光标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
//画点
void LCD_DrawPoint(uint16_t x,uint16_t y);
//快速画点
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color);
//读点
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y);
//画线
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
//填充单色
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
//填充指定颜色
void LCD_Color_Fill(uint32_t sx,uint32_t sy,uint32_t ex,uint32_t ey,uint32_t* color);
//写寄存器
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
//读寄存器
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
//准备写RAM
void LCD_WriteRAM_Prepare(void);
//写RAM
void LCD_WriteRAM(uint16_t RGB_Code);
//SSD1963 背光控制
void LCD_SSD_BackLightSet(uint8_t pwm);

//LCD的画笔颜色和背景色
extern uint32_t POINT_COLOR;  //默认红色
extern uint32_t BACK_COLOR;   //背景颜色.默认为白色
//声明LCD参数结构体
extern LCD_HandleTypeDef lcddev;

#endif