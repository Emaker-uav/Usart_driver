#ifndef _USART_H
#define _USART_H

#define HAL_Library
#ifdef 	HAL_Library

#include "stm32f1xx_hal.h"
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3)) 

typedef struct
{
	uint8_t offline_U1;
	uint8_t offline_U2;
	uint8_t offline_U3;
	uint8_t offline_U4;
	uint8_t offline_U5;
}_offline_st_st;
extern _offline_st_st   offline;
void Check_U1(uint8_t dT_ms);
void Check_U2(uint8_t dT_ms);
void Check_U3(uint8_t dT_ms);
void Check_U4(uint8_t dT_ms);
void Check_U5(uint8_t dT_ms);
static void U1_Data_Anl(uint8_t *data,uint8_t len);
static void U2_Data_Anl(uint8_t *data,uint8_t len);
static void U3_Data_Anl(uint8_t *data,uint8_t len);
static void U4_Data_Anl(uint8_t *data,uint8_t len);
static void U5_Data_Anl(uint8_t *data,uint8_t len);
void drvU1GetByte(uint8_t data);
void drvU2GetByte(uint8_t data);
void drvU3GetByte(uint8_t data);
void drvU4GetByte(uint8_t data);
void drvU5GetByte(uint8_t data);
void DrvUartDataCheck(void);
#else
#include "stm32f1xx.h"
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3)) 
typedef struct
{
	uint8_t offline_U1;
	uint8_t offline_U2;
	uint8_t offline_U3;
	uint8_t offline_U4;
	uint8_t offline_U5;
}_offline_st;
void Check_U1(uint8_t dT_ms);
void Check_U2(uint8_t dT_ms);
void Check_U3(uint8_t dT_ms);
void Check_U4(uint8_t dT_ms);
void Check_U5(uint8_t dT_ms);
static void U1_Data_Anl(uint8_t *data,uint8_t len);
static void U2_Data_Anl(uint8_t *data,uint8_t len);
static void U3_Data_Anl(uint8_t *data,uint8_t len);
static void U4_Data_Anl(uint8_t *data,uint8_t len);
static void U5_Data_Anl(uint8_t *data,uint8_t len);
void drvU1GetByte(uint8_t data);
void drvU2GetByte(uint8_t data);
void drvU3GetByte(uint8_t data);
void drvU4GetByte(uint8_t data);
void drvU5GetByte(uint8_t data);
void DrvUartDataCheck(void);
#endif

#endif
