/*!
 * @Create: 2021/12/13
 * @Author: AkiNya
 * @description: TOUCH触摸驱动
 */

#include "TOUCH.h"

TOUCH_HandleTypeDef touchdev=
{
  TP_Init,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};

//默认为touchtype=0的数据.
uint8_t CMD_RDX=0XD0;
uint8_t CMD_RDY=0X90;
const uint8_t GT9147_CFG_TBL[]=
{
        0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
        0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
        0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
        0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
        0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
        0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
        0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
        0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
        0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
        0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
        0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
        0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
        0XFF,0XFF,0XFF,0XFF,
};
const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

/*!  I2C基础操作函数定义  !*/

void delay_us(uint32_t udelay)
{
  __IO uint32_t Delay = udelay * 72 / 8;//(SystemCoreClock / 8U / 1000000U)
                                         //见stm32f1xx_hal_rcc.c -- static void RCC_Delay(uint32_t mdelay)
  do
  {
    __NOP();
  }
  while (Delay --);
}

//电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_Initure;

#if(F4_CT_IO)
  __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
  __HAL_RCC_GPIOF_CLK_ENABLE();           	//开启GPIOF时钟
  __HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOC时钟
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
  GPIO_Initure.Pin=GPIO_PIN_0; 				      //PB0 T_SCK
  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  GPIO_Initure.Pin=GPIO_PIN_2; 				      //PB2 T_MISO
  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  GPIO_Initure.Pin=GPIO_PIN_11; 				    //PF11 T_MOSI
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
  GPIO_Initure.Pin=GPIO_PIN_13; 				    //PC13 T_CS
  HAL_GPIO_Init(GPIOC,&GPIO_Initure);
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  	    //上拉输入
  GPIO_Initure.Pin=GPIO_PIN_1; 				      //PB1 T_PEN
  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
#elif (H7_CT_IO)
  __HAL_RCC_GPIOH_CLK_ENABLE();           	//开启GPIOB时钟
  __HAL_RCC_GPIOI_CLK_ENABLE();           	//开启GPIOF时钟
  __HAL_RCC_GPIOG_CLK_ENABLE();           	//开启GPIOC时钟

  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速

  GPIO_Initure.Pin=GPIO_PIN_6; 			// T_SCK
  HAL_GPIO_Init(GPIOH,&GPIO_Initure);

  GPIO_Initure.Pin=GPIO_PIN_3; 		// T_MOSI
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);

  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速

  GPIO_Initure.Pin=GPIO_PIN_8; 		// T_CS
  HAL_GPIO_Init(GPIOI,&GPIO_Initure);

  GPIO_Initure.Mode=GPIO_MODE_INPUT;  	    //上拉输入
  GPIO_Initure.Pin=GPIO_PIN_7; 			// T_PEN
  HAL_GPIO_Init(GPIOH,&GPIO_Initure);

  //  GPIO_Initure.Pin=GPIO_PIN_3; 			// T_MISO
  //  HAL_GPIO_Init(GPIOG,&GPIO_Initure);


#endif
  CT_SDA(1);
  CT_SCL(1);

}
//产生IIC起始信号
void CT_IIC_Start(void)
{
  //CT_SDA_OUT(); //SDA(PF11)设为输出
  CT_SDA(1);
  CT_SCL(1);
  CT_Delay();
  CT_SDA(0);
  CT_Delay();
  CT_SCL(0);
  CT_Delay();
}
//产生IIC停止信号
void CT_IIC_Stop(void)
{
  //CT_SDA_OUT(); //sda线输出
  CT_SDA(0);
  CT_Delay();
  CT_SCL(1);
  CT_SDA(1);
  CT_Delay();
}
//IIC 5us延时
void CT_Delay(void)
{
  delay_us(2);
}
/*!
 * 等待应答信号到来
 * @return 1,接收应答失败
 * @return 0,接收应答成功
 */
uint8_t CT_IIC_Wait_Ack(void)
{
  uint8_t ucErrTime=0;
  uint8_t rack = 0;
  CT_SDA(1);
  CT_Delay();
  CT_SCL(1);
  CT_Delay();
  CT_SDA_IN();
  while(CT_SDA_READ == 1)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      CT_IIC_Stop();
      rack = 1;
      break;
    }
  }
  CT_SCL(0);//时钟输出0
  CT_SDA_OUT();
  CT_Delay();
  return rack;
}
//产生ACK应答
void CT_IIC_Ack(void)
{
  //CT_SCL(0);
  //CT_SDA_OUT();
  CT_SDA(0);
  CT_Delay();
  CT_SCL(1);
  CT_Delay();
  CT_SCL(0);
  CT_Delay();
  CT_SDA(1);
  CT_Delay();
}
//不产生ACK应答
void CT_IIC_NAck(void)
{
  //CT_SDA_OUT();
  CT_SDA(1);
  CT_Delay();
  CT_SCL(1);
  CT_Delay();
  CT_SCL(0);
  CT_Delay();
}
/*!
 * IIC发送一个字节,返回从机有无应答
 * @return 1,有应答
 * @return 0,无应答
 */
void CT_IIC_Send_Byte(uint8_t txd)
{
  uint8_t t;
  //CT_SDA_OUT();
  //CT_SCL(0);    //拉低时钟开始数据传输
  //CT_Delay();
  for(t=0;t<8;t++)
  {
    CT_SDA((txd&0x80)>>7);
    CT_Delay();
    CT_SCL(1);
    CT_Delay();
    CT_SCL(0);
    txd<<=1;
  }
  CT_SDA(1);
}
/*!
 * IIC读1个字节
 * @param ack 1:发送ACK;0:发送nACK
 * @return 读取字节
 */
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
  uint8_t i,receive=0;
//  CT_SDA_IN();
//  delay_us(30);
  for(i=0;i<8;i++ )
  {
    receive<<=1;
    CT_SCL(1);
    CT_Delay();

    if(CT_SDA_READ==1)receive++;
    CT_SCL(0);
    CT_Delay();
  }
  if (!ack)CT_IIC_NAck(); //发送nACK
  else CT_IIC_Ack();      //发送ACK
  return receive;
}

/*!  GT9147操作函数定义  !*/

uint8_t GT9147_Init(void)
{
  GPIO_InitTypeDef GPIO_Initure;
  uint8_t temp[5];

  CT_IIC_Init();      	//初始化电容屏的I2C总线
  CT_RST(0);            //复位
  HAL_Delay(10);
  CT_RST(1);            //释放复位
  HAL_Delay(10);

#if(F4_CT_IO)
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
  GPIO_Initure.Pin=GPIO_PIN_1;    //PB1 T_PEN(CT_INT)修改为输出
  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
#elif (H7_CT_IO)
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  	//推挽输出
  GPIO_Initure.Pull=GPIO_NOPULL;         //不带上下拉，浮空输入
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  //高速
  GPIO_Initure.Pin=GPIO_PIN_7;    //不带上下拉，浮空输入
  HAL_GPIO_Init(GPIOH,&GPIO_Initure);
#endif
  HAL_Delay(100);
  GT9147_RD_Reg(GT_PID_REG,temp,4);	//读取产品ID
  temp[4]=0;

  HAL_Delay(200);
  printf("CTP ID:%s\r\n",temp);		  //打印ID
  if(strcmp((char*)temp,"917S")==0 || strcmp((char*)temp,"9147")==0)	//ID==9147
  {
    temp[0]=0X02;
    GT9147_WR_Reg(GT_CTRL_REG,temp,1);  //软复位GT9147
    GT9147_RD_Reg(GT_CFGS_REG,temp,1);  //读取GT_CFGS_REG寄存器
    if(temp[0]<0X60)  //默认版本比较低,需要更新flash配置
    {
      printf("Default Ver:%d\r\n",temp[0]);
      GT9147_Send_Cfg(1); //更新并保存配置
    }
    HAL_Delay(10);
    temp[0]=0X00;
    GT9147_WR_Reg(GT_CTRL_REG,temp,1);  //结束复位
    return 1;
  }
  return 0;
}
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
  uint8_t i;
  uint8_t ret=0;
  CT_IIC_Start();
  CT_IIC_Send_Byte(GT_CMD_WR);  //发送写命令
  CT_IIC_Wait_Ack();
  CT_IIC_Send_Byte(reg>>8);     //发送高8位地址
  CT_IIC_Wait_Ack();
  CT_IIC_Send_Byte(reg&0XFF);   //发送低8位地址
  CT_IIC_Wait_Ack();
  for(i=0;i<len;i++)
  {
    CT_IIC_Send_Byte(buf[i]);   //发数据
    ret=CT_IIC_Wait_Ack();
    if(ret)break;
  }
  CT_IIC_Stop();  //产生一个停止条件
  return ret;
}
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
  uint8_t i;
  CT_IIC_Start();
  CT_IIC_Send_Byte(GT_CMD_WR);  //发送写命令
  CT_IIC_Wait_Ack();
  CT_IIC_Send_Byte(reg>>8);     //发送高8位地址
  CT_IIC_Wait_Ack();
  CT_IIC_Send_Byte(reg&0XFF);   //发送低8位地址
  CT_IIC_Wait_Ack();
  CT_IIC_Start();
  CT_IIC_Send_Byte(GT_CMD_RD);  //发送读命令
  CT_IIC_Wait_Ack();
  for(i=0;i<len;i++)
  {
    buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1);  //发送数据
  }
  CT_IIC_Stop();  //产生一个停止条件
}
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
  uint8_t buf[2];
  uint8_t i=0;
  buf[0]=0;
  buf[1]=mode;  //是否写入到GT9147 FLASH中,即是否掉电保存
  for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i]; //计算校验和
  buf[0]=(~buf[0])+1;
  GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL)); //发送寄存器配置
  GT9147_WR_Reg(GT_CHECK_REG,buf,2);  //写入校验和,和配置更新标记
  return 0;
}
uint8_t GT9147_Scan(uint8_t mode)
{
  uint8_t buf[4];
  uint8_t i=0;
  uint8_t res=0;
  uint8_t temp;
  uint8_t tempsta;
  static uint8_t t=0;   //控制查询间隔,从而降低CPU占用率
  t++;
  if((t%10)==0||t<10)   //空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
  {
    GT9147_RD_Reg(GT_GSTID_REG,&mode,1);    //读取触摸点的状态
    if(mode&0X80&&((mode&0XF)<6))
    {
      temp=0;
      GT9147_WR_Reg(GT_GSTID_REG,&temp,1);  //清标志
    }
    if((mode&0XF)&&((mode&0XF)<6))
    {
      temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配touchdev.sta定义
      tempsta=touchdev.sta;			//保存当前的touchdev.sta值
      touchdev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
      touchdev.x[4]=touchdev.x[0];	//保存触点0的数据
      touchdev.y[4]=touchdev.y[0];
      for(i=0;i<5;i++)
      {
        if(touchdev.sta&(1<<i))	//触摸有效
        {
          GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
          if(touchdev.touchtype&0X01)//横屏
          {
            touchdev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
            touchdev.x[i]=800-(((uint16_t)buf[3]<<8)+buf[2]);
          }else {
            touchdev.x[i] = ((uint16_t) buf[1] << 8) + buf[0];
            touchdev.y[i] = ((uint16_t) buf[3] << 8) + buf[2];
          }
        }
      }
      res=1;
      if(touchdev.x[0]>lcddev.width||touchdev.y[0]>lcddev.height)//非法数据(坐标超出了)
      {
        if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
        {
          touchdev.x[0]=touchdev.x[1];
          touchdev.y[0]=touchdev.y[1];
          t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
        }else					//非法数据,则忽略此次数据(还原原来的)
        {
          touchdev.x[0]=touchdev.x[4];
          touchdev.y[0]=touchdev.y[4];
          mode=0X80;
          touchdev.sta=tempsta;	//恢复touchdev.sta
        }
      }else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
    }
  }
  if((mode&0X8F)==0X80)//无触摸点按下
  {
    if(touchdev.sta&TP_PRES_DOWN)	//之前是被按下的
    {
      touchdev.sta&=~(1<<7);	//标记按键松开
    }else						//之前就没有被按下
    {
      touchdev.x[0]=0xffff;
      touchdev.y[0]=0xffff;
    }
  }
  if(t>240)t=10;//重新从10开始计数
  return res;
}

/*!
 * 目前仅支持GT9147
 * @return 1:初始化成功 0:初始化失败，暂未支持
 */
uint8_t TP_Init(void)
{
  if(lcddev.id==0X5510)				            //4.3寸电容触摸屏
  {
    if(GT9147_Init()==1)			            //是GT9147
    {
      touchdev.scan=GT9147_Scan;	        //扫描函数指向GT9147触摸屏扫描
    }else
    {
      printf("Unkonw Dev!\r\n");
    }
    touchdev.touchtype|=0X80;			        //电容屏
    touchdev.touchtype|=lcddev.dir&0X01;  //横屏还是竖屏
    return 1;
  }else {
    printf("Unkonw Dev!\r\n");
    return 0;
  }
}
uint8_t TP_Touch(){
  if((touchdev.sta)&(1<<7))
    return 1;
  else
    return 0;
}



