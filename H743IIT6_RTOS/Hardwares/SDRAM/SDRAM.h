//
// Created by admin on 2022/6/29.
//

#ifndef H743IIT6_EMPTY_SDRAM_H
#define H743IIT6_EMPTY_SDRAM_H

#include "fmc.h"
#include "stdio.h"

void fsmc_sdram_test();
void SDRAM_InitSequence(void);
void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n);
void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n);

static FMC_SDRAM_CommandTypeDef Command;   //定义SDRAM命令结构体
#define sdramHandle hsdram1
#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)  //定义超时时间

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

#define SDRAM_BANK_ADDR     ((uint32_t)0xC0000000)

#endif // H743IIT6_EMPTY_SDRAM_H
