//
// Created by admin on 2022/6/28.
//

#include "RGB_LCD.h"
#if(RGB_LCD)
LTDC_HandleTypeDef LTDC_Handler;   // LTDC句柄

//根据不同的颜色格式,定义帧缓存数组
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 ||                                 \
    LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
uint32_t ltdc_lcd_framebuf[1280][800] __attribute__((
    at(LCD_FRAME_BUF_ADDR))); //定义最大屏分辨率时,LCD所需的帧缓存数组大小
#else

uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((section(".sdram"))); //定义最大屏分辨率时,LCD所需的帧缓存数组大小

#endif

uint32_t *ltdc_framebuf[2]; // LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域

_ltdc_dev lcdltdc; //管理LCD LTDC的重要参数

//打开LCD开关
// lcd_switch:1 打开,0，关闭
void LTDC_Switch(uint8_t sw) {
  if (sw == 1)
    __HAL_LTDC_ENABLE(&LTDC_Handler);
  else if (sw == 0)
    __HAL_LTDC_DISABLE(&LTDC_Handler);
}

//开关指定层
// layerx:层号,0,第一层; 1,第二层
// sw:1 打开;0关闭
void LTDC_Layer_Switch(uint8_t layerx, uint8_t sw) {
  if (sw == 1)
    __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler, layerx);
  else if (sw == 0)
    __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler, layerx);
  __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

//选择层
// layerx:层号;0,第一层;1,第二层;
void LTDC_Select_Layer(uint8_t layerx) { lcdltdc.activelayer = layerx; }

//设置LCD显示方向
// dir:0,竖屏；1,横屏
void LTDC_Display_Dir(uint8_t dir) {
  lcdltdc.dir = dir; //显示方向
  if (dir == 0)      //竖屏
  {
    lcdltdc.width = lcdltdc.pheight;
    lcdltdc.height = lcdltdc.pwidth;
  } else if (dir == 1) //横屏
  {
    lcdltdc.width = lcdltdc.pwidth;
    lcdltdc.height = lcdltdc.pheight;
  }
}

//画点函数
// x,y:坐标
// color:颜色值
void LTDC_Draw_Point(uint16_t x, uint16_t y, uint32_t color) {
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 ||LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
  if (lcdltdc.dir) //横屏
  {
    *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
                  lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
  } else //竖屏
  {
    *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
                  lcdltdc.pixsize *
                      (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
  }
#else
  if (lcdltdc.dir) //横屏
  {
    *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
  } else //竖屏
  {
    *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +lcdltdc.pixsize *(lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color;
  }
#endif
}

//读点函数
//返回值:颜色值
uint32_t LTDC_Read_Point(uint16_t x, uint16_t y) {
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 ||                                 \
    LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
  if (lcdltdc.dir) //横屏
  {
    return *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
                         lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
  } else //竖屏
  {
    return *(uint32_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
                         lcdltdc.pixsize *
                             (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
  }
#else
  if (lcdltdc.dir) //横屏
  {
    return *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
                         lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
  } else //竖屏
  {
    return *(uint16_t *)((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
                         lcdltdc.pixsize *
                             (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y));
  }
#endif
}

// LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
// color:要填充的颜色
void LTDC_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color) {
  uint32_t psx, psy, pex, pey; //以LCD面板为基准的坐标系,不随横竖屏变化而变化
  uint32_t timeout = 0;
  uint16_t offline;
  uint32_t addr;
  //坐标系转换
  if (lcdltdc.dir) //横屏
  {
    psx = sx;
    psy = sy;
    pex = ex;
    pey = ey;
  } else //竖屏
  {
    psx = sy;
    psy = lcdltdc.pheight - ex - 1;
    pex = ey;
    pey = lcdltdc.pheight - sx - 1;
  }
  offline = lcdltdc.pwidth - (pex - psx + 1);
  addr = ((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
          lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
  __HAL_RCC_DMA2D_CLK_ENABLE();
  RCC->AHB1ENR |= 1 << 23;       //使能DM2D时钟
  DMA2D->CR = 3 << 16;           //寄存器到存储器模式
  DMA2D->OPFCCR = LCD_PIXFORMAT; //设置颜色格式
  DMA2D->OOR = offline;          //设置行偏移
  DMA2D->CR &= ~(1 << 0);        //先停止DMA2D
  DMA2D->OMAR = addr;            //输出存储器地址
  DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); //设定行数寄存器
  DMA2D->OCOLR = color;                //设定输出颜色寄存器
  DMA2D->CR |= 1 << 0;                 //启动DMA2D
  while ((DMA2D->ISR & (1 << 1)) == 0) //等待传输完成
  {
    timeout++;
    if (timeout > 0X1FFFFF)
      break; //超时退出
  }
  DMA2D->IFCR |= 1 << 1; //清除传输完成标志
}
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
// color:要填充的颜色
// void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t
// color)
//{
//	uint32_t psx,psy,pex,pey;
////以LCD面板为基准的坐标系,不随横竖屏变化而变化 	uint32_t timeout=0; 	uint16_t
//offline; 	uint32_t addr;
//    if(ex>=lcdltdc.width)ex=lcdltdc.width-1;
//	if(ey>=lcdltdc.height)ey=lcdltdc.height-1;
//	//坐标系转换
//	if(lcdltdc.dir)	//横屏
//	{
//		psx=sx;psy=sy;
//		pex=ex;pey=ey;
//	}else			//竖屏
//	{
//		psx=sy;psy=lcdltdc.pheight-ex-1;
//		pex=ey;pey=lcdltdc.pheight-sx-1;
//	}
//	offline=lcdltdc.pwidth-(pex-psx+1);
//	addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
//    if(LCD_PIXFORMAT==LCD_PIXEL_FORMAT_RGB565)
//    //如果是RGB565格式的话需要进行颜色转换，将16bit转换为32bit的
//    {
//        color=((color&0X0000F800)<<8)|((color&0X000007E0)<<5)|((color&0X0000001F)<<3);
//    }
//	//配置DMA2D的模式
//	DMA2D_Handler.Instance=DMA2D;
//	DMA2D_Handler.Init.Mode=DMA2D_R2M; //内存到存储器模式
//	DMA2D_Handler.Init.ColorMode=LCD_PIXFORMAT;	//设置颜色格式
//	DMA2D_Handler.Init.OutputOffset=offline;		//输出偏移
//	HAL_DMA2D_Init(&DMA2D_Handler);              //初始化DMA2D
//    HAL_DMA2D_ConfigLayer(&DMA2D_Handler,lcdltdc.activelayer); //层配置
//    HAL_DMA2D_Start(&DMA2D_Handler,color,(uint32_t)addr,pex-psx+1,pey-psy+1);//开启传输
//    HAL_DMA2D_PollForTransfer(&DMA2D_Handler,1000);//传输数据
//    while((__HAL_DMA2D_GET_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC)==0)&&(timeout<0X5000))//等待DMA2D完成
//    {
//        timeout++;
//    }
//    __HAL_DMA2D_CLEAR_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC); //清除传输完成标志
//}

//在指定区域内填充指定颜色块,DMA2D填充
//此函数仅支持uint16_t,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
// color:要填充的颜色数组首地址
void LTDC_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey,uint16_t *color) {
  uint32_t psx, psy, pex, pey; //以LCD面板为基准的坐标系,不随横竖屏变化而变化
  uint32_t timeout = 0;
  uint16_t offline;
  uint32_t addr;
  //坐标系转换
  if (lcdltdc.dir) //横屏
  {
    psx = sx;
    psy = sy;
    pex = ex;
    pey = ey;
  } else //竖屏
  {
    psx = sy;
    psy = lcdltdc.pheight - ex - 1;
    pex = ey;
    pey = lcdltdc.pheight - sx - 1;
  }
  offline = lcdltdc.pwidth - (pex - psx + 1);
  addr = ((uint32_t)ltdc_framebuf[lcdltdc.activelayer] +
          lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
  RCC->AHB1ENR |= 1 << 23;        //使能DM2D时钟
  DMA2D->CR = 0 << 16;            //存储器到存储器模式
  DMA2D->FGPFCCR = LCD_PIXFORMAT; //设置颜色格式
  DMA2D->FGOR = 0;                //前景层行偏移为0
  DMA2D->OOR = offline;           //设置行偏移
  DMA2D->CR &= ~(1 << 0);         //先停止DMA2D
  DMA2D->FGMAR = (uint32_t)color; //源地址
  DMA2D->OMAR = addr;             //输出存储器地址
  DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16); //设定行数寄存器
  DMA2D->CR |= 1 << 0;                                    //启动DMA2D
  while ((DMA2D->ISR & (1 << 1)) == 0)                    //等待传输完成
  {
    timeout++;
    if (timeout > 0X1FFFFF)
      break; //超时退出
  }
  DMA2D->IFCR |= 1 << 1; //清除传输完成标志
}

// LCD清屏
// color:颜色值
void LTDC_Clear(uint32_t color) {
  LTDC_Fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}

// LTDC时钟(Fdclk)设置函数
// PLL3_VCO Input=HSE_VALUE/PLL3M
// PLL3_VCO Output=PLL3_VCO Input * PLL3N
// PLLLCDCLK = PLL3_VCO Output/PLL3R
//假如HSE_VALUE=25MHz，PLL3M=5，PLL3N=160,PLL3R=88
// LTDCLK=PLLLCDCLK=25/5*160/88=9MHz

//返回值:0,成功;1,失败。
uint8_t LTDC_Clk_Set(uint32_t pll3m, uint32_t pll3n, uint32_t pll3r) {
  RCC_PeriphCLKInitTypeDef PeriphClkIniture;
  PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkIniture.PLL3.PLL3M = pll3m;
  PeriphClkIniture.PLL3.PLL3N = pll3n;
  PeriphClkIniture.PLL3.PLL3P = 2;
  PeriphClkIniture.PLL3.PLL3Q = 2;
  PeriphClkIniture.PLL3.PLL3R = pll3r;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture) ==
      HAL_OK) //配置像素时钟，这里配置为时钟为18.75MHZ
  {
    return 0; //成功
  } else
    return 1; //失败
}

// LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
//注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
// layerx:层值,0/1.
// sx,sy:起始坐标
// width,height:宽度和高度
void LTDC_Layer_Window_Config(uint8_t layerx, uint16_t sx, uint16_t sy,
                              uint16_t width, uint16_t height) {
  HAL_LTDC_SetWindowPosition(&LTDC_Handler, sx, sy, layerx); //设置窗口的位置
  HAL_LTDC_SetWindowSize(&LTDC_Handler, width, height, layerx); //设置窗口大小
}

// LTDC,基本参数设置.
//注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
// layerx:层值,0/1.
// bufaddr:层颜色帧缓存起始地址
// pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
// alpha:层颜色Alpha值,0,全透明;255,不透明
// alpha0:默认颜色Alpha值,0,全透明;255,不透明
// bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
// bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
// bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
void LTDC_Layer_Parameter_Config(uint8_t layerx, uint32_t bufaddr,uint8_t pixformat, uint8_t alpha,uint8_t alpha0, uint8_t bfac1, uint8_t bfac2,uint32_t bkcolor) {
  LTDC_LayerCfgTypeDef pLayerCfg;
  pLayerCfg.WindowX0 = 0;               //窗口起始X坐标
  pLayerCfg.WindowY0 = 0;               //窗口起始Y坐标
  pLayerCfg.WindowX1 = lcdltdc.pwidth;  //窗口终止X坐标
  pLayerCfg.WindowY1 = lcdltdc.pheight; //窗口终止Y坐标
  pLayerCfg.PixelFormat = pixformat;    //像素格式
  pLayerCfg.Alpha = alpha;   // Alpha值设置，0~255,255为完全不透明
  pLayerCfg.Alpha0 = alpha0; //默认Alpha值
  pLayerCfg.BlendingFactor1 = (uint32_t)bfac1 << 8; //设置层混合系数
  pLayerCfg.BlendingFactor2 = (uint32_t)bfac2 << 8; //设置层混合系数
  pLayerCfg.FBStartAdress = bufaddr;     //设置层颜色帧缓存起始地址
  pLayerCfg.ImageWidth = lcdltdc.pwidth; //设置颜色帧缓冲区的宽度
  pLayerCfg.ImageHeight = lcdltdc.pheight; //设置颜色帧缓冲区的高度
  pLayerCfg.Backcolor.Red =
      (uint8_t)(bkcolor & 0X00FF0000) >> 16; //背景颜色红色部分
  pLayerCfg.Backcolor.Green =
      (uint8_t)(bkcolor & 0X0000FF00) >> 8; //背景颜色绿色部分
  pLayerCfg.Backcolor.Blue = (uint8_t)bkcolor & 0X000000FF; //背景颜色蓝色部分
  HAL_LTDC_ConfigLayer(&LTDC_Handler, &pLayerCfg, layerx); //设置所选中的层
}

//读取面板参数,新增了判断悬空全一的排查
// PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
// M2:M1:M0
// 0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
// 0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
// 0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
// 0 :1 :1	//7寸1280*800 RGB屏,ID=0X7018
// 1 :0 :0	//4.3寸800*480 RGB屏,ID=0X4384
// 1 :0 :1      //10.1寸1280*800,RGB屏,ID=0X1018
// 1 :1 :1      //VGA显示器delet
//返回值:LCD ID:0,非法;其他值,ID;
uint16_t LTDC_PanelID_Read(void) {
  uint8_t idx = 0;
  GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOG_CLK_ENABLE(); //使能GPIOG时钟
  __HAL_RCC_GPIOI_CLK_ENABLE(); //使能GPIOI时钟
  GPIO_Initure.Mode = GPIO_MODE_INPUT;            //输入
  GPIO_Initure.Pull = GPIO_PULLUP;                //上拉
  GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
  //LTDC_R7 PG6
  //LTDC_G7 PI2
  //LTDC_B7 PI7
  GPIO_Initure.Pin = GPIO_PIN_6;                        // PG6-R7
  HAL_GPIO_Init(GPIOG, &GPIO_Initure);  //初始化
  GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_7;           // PI2,7
  HAL_GPIO_Init(GPIOI, &GPIO_Initure);  //初始化
  idx = (uint8_t)HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6);       //读取M0
  idx |= (uint8_t)HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2) << 1; //读取M1
  idx |= (uint8_t)HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_7) << 2; //读取M2
  if (idx == 0)
    return 0X4342; // 4.3寸屏,480*272分辨率
  else if (idx == 1)
    return 0X7084; // 7寸屏,800*480分辨率
  else if (idx == 2)
    return 0X7016; // 7寸屏,1024*600分辨率
  else if (idx == 3)
    return 0X7018; // 7寸屏,1280*800分辨率
  else if (idx == 4)
    return 0X4384; // 4.3寸屏,800*480分辨率
  else if (idx == 5)
    return 0X1018; // 10.1寸屏,1280*800分辨率
  else
    return 0;
}

// LCD初始化函数
void LTDC_Init(void) {
  uint16_t lcdid = 0;
  lcdid = LTDC_PanelID_Read(); //读取LCD面板ID
  printf("LCD ID:%04x\r\n",lcdid);
  if (lcdid == 0X4342) {
    lcdltdc.pwidth = 480;     //面板宽度,单位:像素
    lcdltdc.pheight = 272;    //面板高度,单位:像素
    lcdltdc.hsw = 1;          //水平同步宽度
    lcdltdc.vsw = 1;          //垂直同步宽度
    lcdltdc.hbp = 40;         //水平后廊
    lcdltdc.vbp = 8;          //垂直后廊
    lcdltdc.hfp = 5;          //水平前廊
    lcdltdc.vfp = 8;          //垂直前廊
    LTDC_Clk_Set(5, 160, 88); //设置像素时钟 9Mhz
  } else if (lcdid == 0X7084) {
    lcdltdc.pwidth = 800;  //面板宽度,单位:像素
    lcdltdc.pheight = 480; //面板高度,单位:像素
    lcdltdc.hsw = 1;       //水平同步宽度
    lcdltdc.vsw = 1;       //垂直同步宽度
    lcdltdc.hbp = 46;      //水平后廊
    lcdltdc.vbp = 23;      //垂直后廊
    lcdltdc.hfp = 210;     //水平前廊
    lcdltdc.vfp = 22;      //垂直前廊
    LTDC_Clk_Set(5, 160,28); //设置像素时钟 33M(如果开双显,需要降低DCLK到:18.75Mhz,pll3r=43,才会比较好)
  } else if (lcdid == 0X7016) {
    lcdltdc.pwidth = 1024;    //面板宽度,单位:像素
    lcdltdc.pheight = 600;    //面板高度,单位:像素
    lcdltdc.hsw = 20;         //水平同步宽度
    lcdltdc.vsw = 3;          //垂直同步宽度
    lcdltdc.hbp = 140;        //水平后廊
    lcdltdc.vbp = 20;         //垂直后廊
    lcdltdc.hfp = 160;        //水平前廊
    lcdltdc.vfp = 12;         //垂直前廊
    LTDC_Clk_Set(5, 160, 20);//设置像素时钟  40Mhz
  } else if (lcdid == 0X7018) {
    lcdltdc.pwidth = 1280; //面板宽度,单位:像素
    lcdltdc.pheight = 800; //面板高度,单位:像素
  } else if (lcdid == 0X4384) {
    lcdltdc.pwidth = 800;     //面板宽度,单位:像素
    lcdltdc.pheight = 480;    //面板高度,单位:像素
    lcdltdc.hbp = 88;         //水平后廊
    lcdltdc.hfp = 40;         //水平前廊
    lcdltdc.hsw = 48;         //水平同步宽度
    lcdltdc.vbp = 32;         //垂直后廊
    lcdltdc.vfp = 13;         //垂直前廊
    lcdltdc.vsw = 3;          //垂直同步宽度
    LTDC_Clk_Set(5, 160, 24); //设置像素时钟 33M
  } else if (lcdid == 0X1018) // 10.1寸1280*800 RGB屏
  {
    lcdltdc.pwidth = 1280;    //面板宽度,单位:像素
    lcdltdc.pheight = 800;    //面板高度,单位:像素
    lcdltdc.hbp = 140;        //水平后廊
    lcdltdc.hfp = 10;         //水平前廊
    lcdltdc.hsw = 10;         //水平同步宽度
    lcdltdc.vbp = 10;         //垂直后廊
    lcdltdc.vfp = 10;         //垂直前廊
    lcdltdc.vsw = 3;          //垂直同步宽度
    LTDC_Clk_Set(5, 160, 16); //设置像素时钟 50MHz
  }
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 ||LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
  ltdc_framebuf[0] = (uint32_t *)&ltdc_lcd_framebuf;
  lcdltdc.pixsize = 4; //每个像素占4个字节
#else
  lcdltdc.pixsize = 2; //每个像素占2个字节
  ltdc_framebuf[0] = (uint32_t *)&ltdc_lcd_framebuf;
#endif
  // LTDC配置
  LTDC_Handler.Instance=LTDC;
  LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //水平同步极性
  LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //垂直同步极性
  LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //数据使能极性
  LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        //像素时钟极性
  LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          //水平同步宽度
  LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            //垂直同步宽度
  LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; //水平同步后沿宽度
  LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; //垂直同步后沿高度
  LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth-1;//有效宽度
  LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight-1;//有效高度
  LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth+lcdltdc.hfp-1;   //总宽度
  LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight+lcdltdc.vfp-1;  //总高度
  LTDC_Handler.Init.Backcolor.Red = 0;   //屏幕背景层红色部分
  LTDC_Handler.Init.Backcolor.Green = 0; //屏幕背景层绿色部分
  LTDC_Handler.Init.Backcolor.Blue = 0;  //屏幕背景色蓝色部分
  HAL_LTDC_Init(&LTDC_Handler);

  //层配置
  LTDC_Layer_Parameter_Config(0, (uint32_t)ltdc_framebuf[0], LCD_PIXFORMAT, 255,0, 6, 7, 0X000000); //层参数配置
  //层窗口配置,以LCD面板坐标系为基准,不要随便修改!
  LTDC_Layer_Window_Config(0, 0, 0, lcdltdc.pwidth,lcdltdc.pheight);


  RGB_LCD_BL(1);                  //点亮背光
  LTDC_Display_Dir(1);         //1横0竖
  LTDC_Select_Layer(0);     //选择第1层
  LTDC_Clear(0xFCB2);       //清屏
}
#endif