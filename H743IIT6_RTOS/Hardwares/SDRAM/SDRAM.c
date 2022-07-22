//
// Created by admin on 2022/6/29.
//

#include "SDRAM.h"

/*
 ******************************************************************************************************
 * 函 数 名: SDRAM 初始化序列
 * 功能说明: 完成 SDRAM 序列初始化
 * 形 参: hsdram: SDRAM 句柄
 * Command: 命令结构体指针
 * 返 回 值: None
 ******************************************************************************************************
 *//**
  * @brief  对SDRAM芯片进行初始化配置
  * @param  None.
  * @retval None.
 */
void SDRAM_InitSequence(void)
{
  uint32_t tmpr = 0;

  /* Step 1 ----------------------------------------------------------------*/
  /* 配置命令：开启提供给SDRAM的时钟 */
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE; //时钟配置使能
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;     //目标SDRAM存储区域
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
  /* 发送配置命令 */
  if(HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT)!=HAL_OK){
    printf("HAL_SDRAM_SendCommand error\r\n");
  }

  /* Step 2: 延时100us */

  HAL_Delay(2);

  /* Step 3 ----------------------------------------------------------------*/
  /* 配置命令：对所有的bank预充电 */
  Command.CommandMode = FMC_SDRAM_CMD_PALL;    //预充电命令
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;    //目标SDRAM存储区域
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
  /* 发送配置命令 */
  if(HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT)!=HAL_OK){
    printf("HAL_SDRAM_SendCommand error\r\n");
  }
  /* Step 4 ----------------------------------------------------------------*/
  /* 配置命令：自动刷新 */
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;  //自动刷新命令
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber = 8;  //设置自刷新次数
  Command.ModeRegisterDefinition = 0;
  /* 发送配置命令 */
  if(HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT)!=HAL_OK){
          printf("HAL_SDRAM_SendCommand error\r\n");
      }

  /* Step 5 ----------------------------------------------------------------*/
  /* 设置sdram寄存器配置 */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |  //设置突发长度:1(可以是1/2/4/8)
         SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |  //设置突发类型:连续(可以是连续/间隔)
         SDRAM_MODEREG_CAS_LATENCY_3           |   //设置CAS值:3(可以是2/3)
         SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
         SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;    //设置突发写模式:1,单点访问

  /* 配置命令：设置SDRAM寄存器 */
  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;  //加载模式寄存器命令
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = tmpr;
  /* 发送配置命令 */
  if(HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT)!=HAL_OK){
  printf("HAL_SDRAM_SendCommand error\r\n");
  }

  /* Step 6 ----------------------------------------------------------------*/

  /* 设置自刷新速率 */

  //刷新频率计数器(以SDCLK频率计数),计算方法:
  //COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
  //我们使用的SDRAM刷新周期为64ms,SDCLK=200/2=100Mhz,行数为8192(2^13).
  //所以,COUNT=64*1000*100/8192-20=761
  if(HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 918)!=HAL_OK){
    printf("HAL_SDRAM_SendCommand error\r\n");
  }
}

/**
  * @brief  向sdram写入数据 ，在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节
  * @param  pBuffer: 指向数据的指针
  * @param  WriteAddr: 要写入的SDRAM内部地址
  * @param  n:要写入的字节数
  * @retval None.
 */

void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n)
{

  /* 检查SDRAM标志，等待至SDRAM空闲 */
  while ( HAL_SDRAM_GetState(&hsdram1) != RESET)
  {
  }
  for(;n!=0;n--)
  {
    *(__IO uint8_t*)(SDRAM_BANK_ADDR+WriteAddr)=*pBuffer;
    WriteAddr++;
    pBuffer++;
  }
}

/**
  * @brief  从sdram读数据 ，在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
  * @param  pBuffer: 指向数据的指针
  * @param  ReadAddr: 要读出的起始地址
  * @param  n:要读出的字节数
  * @retval None.
 */
void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n)
{

  /* 检查SDRAM标志，等待至SDRAM空闲 */
  while ( HAL_SDRAM_GetState(&hsdram1) != RESET)
  {
  }
  for(;n!=0;n--)
  {
    *pBuffer++=*(__IO uint8_t*)(SDRAM_BANK_ADDR+ReadAddr);
    ReadAddr++;
  }
}

//SDRAM内存测试
void fsmc_sdram_test()
{
  __IO uint32_t i=0;
  __IO uint32_t temp=0;
  __IO uint32_t sval=0;	//在地址0读到的数据

  //每隔16K字节,写入一个数据,总共写入2048个数据,刚好是32M字节
  for(i=0;i<32*1024*1024;i+=16*1024)
  {
    *(__IO uint32_t*)(SDRAM_BANK_ADDR+i)=temp;
    temp++;
  }
  //依次读出之前写入的数据,进行校验
  for(i=0;i<32*1024*1024;i+=16*1024)
  {
    temp=*(__IO uint32_t*)(SDRAM_BANK_ADDR+i);
    if(i==0)sval=temp;
    else if(temp<=sval)break;//后面读出的数据一定要比第一次读到的数据大.
    printf("SDRAM Capacity:%dKB\r\n",(uint16_t)(temp-sval+1)*16);//打印SDRAM容量
  }
}