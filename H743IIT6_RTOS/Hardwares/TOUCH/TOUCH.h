/*!
 * @Create: 2021/12/13
 * @Author: AkiNya
 * @description: TOUCH触摸驱动，电阻屏部分删除
 */

#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "main.h"
#include "MCU_LCD.h"
#include "string.h"
#include <stdio.h>

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define TP_PRES_DOWN 0x80  		//触屏被按下
#define TP_CATH_PRES 0x40  		//有按键按下了
#define CT_MAX_TOUCH  5    		//电容屏支持的点数,固定为5点

//触摸屏控制器
typedef struct
{
    uint8_t (*init)(void);      //初始化触摸屏控制器函数
    uint8_t (*scan)(uint8_t);   //扫描触摸屏函数 0,屏幕扫描;1,物理坐标;
    void (*adjust)(void);       //触摸屏校准
    uint16_t x[CT_MAX_TOUCH];   //当前x坐标
    uint16_t y[CT_MAX_TOUCH];   //当前y坐标
    //电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用x[4],y[4]存储第一次按下时的坐标.
    uint8_t  sta;               //笔的状态
    //bit7:   按下1/松开0;
    //bit6:   0,没有按键按下;1,有按键按下.
    //bit5:   保留
    //bit4~0: 电容触摸屏按下的点数(0,表示未按下,1表示按下)
    //触摸屏校准参数(电容屏不需要校准)
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    //新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
    //bit0:   0,竖屏(适合左右为X坐标,上下为Y坐标的TP) 1,横屏(适合左右为Y坐标,上下为X坐标的TP)
    //bit1~6: 保留.
    //bit7:   0,电阻屏 1,电容屏
    uint8_t touchtype;
    uint16_t  xt;
    uint16_t  yt;
}TOUCH_HandleTypeDef;

extern TOUCH_HandleTypeDef touchdev;  //外部声明

/*!
 * @name 触摸基础操作
 *
 */

//电容屏
uint8_t TP_Init(void);                //初始化
uint8_t TP_Touch();
/*!
 * @name GT9147 基础操作
 *
 */
//I2C读写命令
#define GT_CMD_WR 		0X28     	//写命令
#define GT_CMD_RD 		0X29		  //读命令
//GT9147 部分寄存器定义
#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0X8140   	//GT9147产品ID寄存器
#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址

uint8_t GT9147_Init(void);
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t GT9147_Scan(uint8_t mode);
uint8_t GT9147_Send_Cfg(uint8_t mode);


#define F4_CT_IO 0
#define H7_CT_IO 1
#if(F4_CT_IO)
/*!
 * @name I2C基础操作
 * 硬件I2C易与FSMC冲突，固使用软件模拟
 * 触摸芯片使用I2C协议，最多占用以下5个IO口
 * GPIO     Base    GT9147
 * PB0      T_SCK   (CT_SCL)
 * PB1      T_PEN   (CT_INT)
 * PF11     T_MOSI  (CT_SDA)
 * PC13     T_CS    (CT_RST)
 * PB2      T_MISO  (NO_USE)
 */
#define CT_SCL(IO) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,IO)
#define CT_SDA(IO) HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,IO)
#define CT_RST(IO) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,IO)
#define CT_INT(IO) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,IO)
//IO方向设置
#define CT_SDA_IN()  {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11输入模式
#define CT_SDA_OUT() {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11输出模式
#define CT_SDA_READ HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11)
#elif (H7_CT_IO)
/*!
 * @name I2C基础操作
 * 硬件I2C易与FSMC冲突，固使用软件模拟
 * 触摸芯片使用I2C协议，最多占用以下5个IO口
 * GPIO     Base    GT9147
 * PH6      T_SCK   (CT_SCL)
 * PH7      T_PEN   (CT_INT)
 * PI3      T_MOSI  (CT_SDA)
 * PI8      T_CS    (CT_RST)
 * PG3      T_MISO  (NO_USE)
 */
#define CT_SCL(IO) HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,IO)
#define CT_SDA(IO) HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,IO)
#define CT_RST(IO) HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,IO)
#define CT_INT(IO) HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7,IO)
//IO方向设置
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(2*3));GPIOI->MODER|=0<<2*3;}	//PI3输入模式
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(2*3));GPIOI->MODER|=1<<2*3;} 	//PI3输出模式
#define CT_SDA_READ HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_3)
#endif
//IIC所有操作函数
void CT_IIC_Init(void);                	      //初始化IIC的IO口
void CT_IIC_Start(void);				              //发送IIC开始信号
void CT_IIC_Stop(void);	  				            //发送IIC停止信号
void CT_IIC_Send_Byte(uint8_t txd);			      //IIC发送一个字节
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t CT_IIC_Wait_Ack(void); 				        //IIC等待ACK信号
void CT_IIC_Ack(void);					              //IIC发送ACK信号
void CT_IIC_NAck(void);					              //IIC不发送ACK信号
void CT_Delay(void);
//us延时函数
static uint32_t fac_us=0;
//void delay_us(uint32_t nus);

#endif
