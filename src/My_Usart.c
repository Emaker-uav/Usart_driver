#include "My_Usart.h"

//定时器回调函数以及串口接收中断回调函数需要操作的地方
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
// 	if(htim->Instance == TIM1)
// 	{
// 			DrvUartDataCheck();
// 	}
// }
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart->Instance == USART1)
// 	{
// 		com_data1 = USART1->DR;
// 		drvU1GetByte(com_data1);		
// 		HAL_UART_Receive_IT(&huart1,&com_data1,1);
// 	}
// }
#define U1_OFFLINE_TIME_MS 2000
#define U2_OFFLINE_TIME_MS 2000
#define U3_OFFLINE_TIME_MS 2000
#define U4_OFFLINE_TIME_MS 2000
#define U5_OFFLINE_TIME_MS 2000

_offline_st_st   offline;
uint16_t U1_offline_check_time = 0;
uint16_t U2_offline_check_time = 0;
uint16_t U3_offline_check_time = 0;
uint16_t U4_offline_check_time = 0;
uint16_t U5_offline_check_time = 0;
static uint8_t u1_rxbuf[100];
static uint8_t u2_rxbuf[100];
static uint8_t u3_rxbuf[100];
static uint8_t u4_rxbuf[100];
static uint8_t u5_rxbuf[100];

void Check_U1(uint8_t dT_ms)
{
	if(U1_offline_check_time<U1_OFFLINE_TIME_MS)
	{
		U1_offline_check_time += dT_ms;
	}
	else
	{
		offline.offline_U1 = 1;
	}
}
static void U1_Check_Reset()
{
	U1_offline_check_time = 0;
	offline.offline_U1 = 0;
}



void Check_U2(uint8_t dT_ms)
{
	if(U2_offline_check_time<U2_OFFLINE_TIME_MS)
	{
		U2_offline_check_time += dT_ms;
	}
	else
	{
		offline.offline_U2 = 1;
	}
}
static void U2_Check_Reset()
{
	U2_offline_check_time = 0;
	offline.offline_U2 = 0;
}


void Check_U3(uint8_t dT_ms)
{
	if(U3_offline_check_time<U3_OFFLINE_TIME_MS)
	{
		U3_offline_check_time += dT_ms;
	}
	else
	{
		offline.offline_U3 = 1;
	}
}
static void U3_Check_Reset()
{
	U3_offline_check_time = 0;
	offline.offline_U3 = 0;
}


void Check_U4(uint8_t dT_ms)
{
	if(U4_offline_check_time<U4_OFFLINE_TIME_MS)
	{
		U4_offline_check_time += dT_ms;
	}
	else
	{
		offline.offline_U4 = 1;
	}
}
static void U4_Check_Reset()
{
	U4_offline_check_time = 0;
	offline.offline_U4 = 0;
}


void Check_U5(uint8_t dT_ms)
{
	if(U5_offline_check_time<U5_OFFLINE_TIME_MS)
	{
		U5_offline_check_time += dT_ms;
	}
	else
	{
		offline.offline_U5 = 1;
	}
}
static void U5_Check_Reset()
{
	U5_offline_check_time = 0;
	offline.offline_U5 = 0;
}
//UART1--------------------------------------------
void U1_Data_Receive(uint8_t data)
{
	static uint8_t u1_data_len = 0,u1_data_cnt = 0;
	static uint8_t u1_rxstate = 0;
	
	if(u1_rxstate == 0 && data == 0XAA)
	{
		u1_rxstate = 1;
		u1_rxbuf[0] = data;
	}
	else if(u1_rxstate == 1 && data == 0XFF)
	{
		u1_rxstate = 2;
		u1_rxbuf[1] = data;
	}
	else if(u1_rxstate == 2) //����֡CMD�ֽ�
	{
		u1_rxstate = 3;
		u1_rxbuf[2] = data;
	}
	//�������ݳ����ֽ�
	else if(u1_rxstate == 3 && data < 50)
	{
		u1_rxstate = 4;
		u1_rxbuf[3] = data;
		u1_data_len = data;                  
		u1_data_cnt = 0;
	}
	//����������
	else if(u1_rxstate == 4 && u1_data_len >0)
	{
		u1_data_len--;                        
		u1_rxbuf[4 + u1_data_cnt++] = data;   
		if(u1_data_len == 0)                  
			u1_rxstate = 5;
	}
	//sc��У��
	else if(u1_rxstate == 5)                 
	{
		u1_rxstate = 6;
		u1_rxbuf[4 + u1_data_cnt++] = data;  
	}
	//ac����У��
	else if(u1_rxstate == 6)                     
	{
		u1_rxstate = 0;
		u1_rxbuf[4 + u1_data_cnt] = data; 
		u1_data_cnt = u1_data_cnt + 5;
		U1_Data_Anl(u1_rxbuf,u1_data_cnt);
	}
	else 
		u1_rxstate = 0;
}

static void U1_Data_Anl(uint8_t *data,uint8_t len)
{
	uint8_t sc = 0, ac = 0;
	if (*(data + 3) != (len - 6))
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		sc += *(data + i);
		ac += sc;
	}
	if ((sc != *(data + len - 2)) || (ac != *(data + len - 1))) 
		return;
	if (*(data) != 0xAA || (*(data + 1) != 0XFF))
		return;
	switch( *(data+2))
	{
		case 0XF5 :{

					U1_Check_Reset();
					};break;
								
		default:
							 break;
	}
}
//UART2------------------------------------------------
void U2_Data_Receive(uint8_t data)
{
	static uint8_t u2_data_len = 0,u2_data_cnt = 0;
	static uint8_t u2_rxstate = 0;
	
	if(u2_rxstate == 0 && data == 0XAA)
	{
		u2_rxstate = 1;
		u2_rxbuf[0] = data;
	}
	else if(u2_rxstate == 1 && data == 0XFF)
	{
		u2_rxstate = 2;
		u2_rxbuf[1] = data;
	}
	else if(u2_rxstate == 2) //����֡CMD�ֽ�
	{
		u2_rxstate = 3;
		u2_rxbuf[2] = data;
	}
	//�������ݳ����ֽ�
	else if(u2_rxstate == 3 && data < 50)
	{
		u2_rxstate = 4;
		u2_rxbuf[3] = data;
		u2_data_len = data;                  
		u2_data_cnt = 0;
	}
	//����������
	else if(u2_rxstate == 4 && u2_data_len >0)
	{
		u2_data_len--;                        
		u2_rxbuf[4 + u2_data_cnt++] = data;   
		if(u2_data_len == 0)                  
			u2_rxstate = 5;
	}
	//sc��У��
	else if(u2_rxstate == 5)                 
	{
		u2_rxstate = 6;
		u2_rxbuf[4 + u2_data_cnt++] = data;  
	}
	//ac����У��
	else if(u2_rxstate == 6)                     
	{
		u2_rxstate = 0;
		u2_rxbuf[4 + u2_data_cnt] = data; 
		u2_data_cnt = u2_data_cnt + 5;
		U2_Data_Anl(u2_rxbuf,u2_data_cnt);
	}
	else 
		u2_rxstate = 0;
}

static void U2_Data_Anl(uint8_t *data,uint8_t len)
{
	uint8_t sc = 0, ac = 0;
	if (*(data + 3) != (len - 6))
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		sc += *(data + i);
		ac += sc;
	}
	if ((sc != *(data + len - 2)) || (ac != *(data + len - 1))) 
		return;
	if (*(data) != 0xAA || (*(data + 1) != 0XFF))
		return;
	switch( *(data+2))
	{
		case 0XF6 :{

					U2_Check_Reset();
					};break;
								
		default:
							 break;
	}
}
//UART3-------------------------------------------
void U3_Data_Receive(uint8_t data)
{
	static uint8_t u3_data_len = 0,u3_data_cnt = 0;
	static uint8_t u3_rxstate = 0;
	
	if(u3_rxstate == 0 && data == 0XAA)
	{
		u3_rxstate = 1;
		u3_rxbuf[0] = data;
	}
	else if(u3_rxstate == 1 && data == 0XFF)
	{
		u3_rxstate = 2;
		u3_rxbuf[1] = data;
	}
	else if(u3_rxstate == 2) //����֡CMD�ֽ�
	{
		u3_rxstate = 3;
		u3_rxbuf[2] = data;
	}
	//�������ݳ����ֽ�
	else if(u3_rxstate == 3 && data < 50)
	{
		u3_rxstate = 4;
		u3_rxbuf[3] = data;
		u3_data_len = data;                  
		u3_data_cnt = 0;
	}
	//����������
	else if(u3_rxstate == 4 && u3_data_len >0)
	{
		u3_data_len--;                        
		u3_rxbuf[4 + u3_data_cnt++] = data;   
		if(u3_data_len == 0)                  
			u3_rxstate = 5;
	}
	//sc��У��
	else if(u3_rxstate == 5)                 
	{
		u3_rxstate = 6;
		u3_rxbuf[4 + u3_data_cnt++] = data;  
	}
	//ac����У��
	else if(u3_rxstate == 6)                     
	{
		u3_rxstate = 0;
		u3_rxbuf[4 + u3_data_cnt] = data; 
		u3_data_cnt = u3_data_cnt + 5;
		U3_Data_Anl(u3_rxbuf,u3_data_cnt);
	}
	else 
		u3_rxstate = 0;
}

static void U3_Data_Anl(uint8_t *data,uint8_t len)
{
	uint8_t sc = 0, ac = 0;
	if (*(data + 3) != (len - 6))
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		sc += *(data + i);
		ac += sc;
	}
	if ((sc != *(data + len - 2)) || (ac != *(data + len - 1))) 
		return;
	if (*(data) != 0xAA || (*(data + 1) != 0XFF))
		return;
	switch( *(data+2))
	{
		case 0XF3 :{
					U3_Check_Reset();
					};break;
								
		default:
							 break;
	}
}
//UART4------------------------------------------
void U4_Data_Receive(uint8_t data)
{
	static uint8_t u4_data_len = 0,u4_data_cnt = 0;
	static uint8_t u4_rxstate = 0;
	
	if(u4_rxstate == 0 && data == 0XAA)
	{
		u4_rxstate = 1;
		u4_rxbuf[0] = data;
	}
	else if(u4_rxstate == 1 && data == 0XFF)
	{
		u4_rxstate = 2;
		u4_rxbuf[1] = data;
	}
	else if(u4_rxstate == 2) //����֡CMD�ֽ�
	{
		u4_rxstate = 3;
		u4_rxbuf[2] = data;
	}
	//�������ݳ����ֽ�
	else if(u4_rxstate == 3 && data < 50)
	{
		u4_rxstate = 4;
		u4_rxbuf[3] = data;
		u4_data_len = data;                  
		u4_data_cnt = 0;
	}
	//����������
	else if(u4_rxstate == 4 && u4_data_len >0)
	{
		u4_data_len--;                        
		u4_rxbuf[4 + u4_data_cnt++] = data;   
		if(u4_data_len == 0)                  
			u4_rxstate = 5;
	}
	//sc��У��
	else if(u4_rxstate == 5)                 
	{
		u4_rxstate = 6;
		u4_rxbuf[4 + u4_data_cnt++] = data;  
	}
	//ac����У��
	else if(u4_rxstate == 6)                     
	{
		u4_rxstate = 0;
		u4_rxbuf[4 + u4_data_cnt] = data; 
		u4_data_cnt = u4_data_cnt + 5;
		U4_Data_Anl(u4_rxbuf,u4_data_cnt);
	}
	else 
		u4_rxstate = 0;
}

static void U4_Data_Anl(uint8_t *data,uint8_t len)
{
	uint8_t sc = 0, ac = 0;
	if (*(data + 3) != (len - 6))
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		sc += *(data + i);
		ac += sc;
	}
	if ((sc != *(data + len - 2)) || (ac != *(data + len - 1))) 
		return;
	if (*(data) != 0xAA || (*(data + 1) != 0XFF))
		return;
	switch( *(data+2))
	{
		case 0XF1 :{
									U4_Check_Reset();
								};break;
								
		default:
							 break;
	}
}
//UART5-----------------------------------------------
void U5_Data_Receive(uint8_t data)
{
	static uint8_t u5_data_len = 0,u5_data_cnt = 0;
	static uint8_t u5_rxstate = 0;
	
	if(u5_rxstate == 0 && data == 0XAA)
	{
		u5_rxstate = 1;
		u5_rxbuf[0] = data;
	}
	else if(u5_rxstate == 1 && data == 0XFF)
	{
		u5_rxstate = 2;
		u5_rxbuf[1] = data;
	}
	else if(u5_rxstate == 2) //����֡CMD�ֽ�
	{
		u5_rxstate = 3;
		u5_rxbuf[2] = data;
	}
	//�������ݳ����ֽ�
	else if(u5_rxstate == 3 && data < 50)
	{
		u5_rxstate = 4;
		u5_rxbuf[3] = data;
		u5_data_len = data;                  
		u5_data_cnt = 0;
	}
	//����������
	else if(u5_rxstate == 4 && u5_data_len >0)
	{
		u5_data_len--;                        
		u5_rxbuf[4 + u5_data_cnt++] = data;   
		if(u5_data_len == 0)                  
			u5_rxstate = 5;
	}
	//sc��У��
	else if(u5_rxstate == 5)                 
	{
		u5_rxstate = 6;
		u5_rxbuf[4 + u5_data_cnt++] = data;  
	}
	//ac����У��
	else if(u5_rxstate == 6)                     
	{
		u5_rxstate = 0;
		u5_rxbuf[4 + u5_data_cnt] = data; 
		u5_data_cnt = u5_data_cnt + 5;
		U5_Data_Anl(u5_rxbuf,u5_data_cnt);
	}
	else 
		u5_rxstate = 0;
}

static void U5_Data_Anl(uint8_t *data,uint8_t len)
{
	uint8_t sc = 0, ac = 0;
	if (*(data + 3) != (len - 6))
		return;
	for (uint8_t i = 0; i < len - 2; i++)
	{
		sc += *(data + i);
		ac += sc;
	}
	if ((sc != *(data + len - 2)) || (ac != *(data + len - 1))) 
		return;
	if (*(data) != 0xAA || (*(data + 1) != 0XFF))
		return;
	switch( *(data+2))
	{
		case 0XF1 :{
									U5_Check_Reset();
								};break;
								
		default:
							 break;
	}
}

//���ڽ��շ��Ϳ��ٶ��壬ֱ���޸Ĵ˴��ĺ������ƺ꣬�޸ĳ��Լ��Ĵ��ڽ����ͷ��ͺ������Ƽ��ɣ�ע�⺯��������ʽ��ͳһ
#define U1GetOneByte	U1_Data_Receive
#define U2GetOneByte	U2_Data_Receive
#define U3GetOneByte	U3_Data_Receive
#define U4GetOneByte	U4_Data_Receive
#define U5GetOneByte	U5_Data_Receive	
//====uart1
uint8_t U1RxDataTmp[100];
uint8_t U1RxInCnt = 0;
uint8_t U1RxoutCnt = 0;
void drvU1GetByte(uint8_t data)
{
	U1RxDataTmp[U1RxInCnt++] = data;
	if(U1RxInCnt >= 100)
		U1RxInCnt = 0;
}
void drvU1DataCheck(void)
{
	while(U1RxInCnt!=U1RxoutCnt)
	{
		U1GetOneByte(U1RxDataTmp[U1RxoutCnt++]);
		if(U1RxoutCnt >= 100)
			U1RxoutCnt = 0;
	}
}
uint8_t U2RxDataTmp[100];
uint8_t U2RxInCnt = 0;
uint8_t U2RxoutCnt = 0;
void drvU2GetByte(uint8_t data)
{
	U2RxDataTmp[U2RxInCnt++] = data;
	if(U2RxInCnt >= 100)
		U2RxInCnt = 0;
}
void drvU2DataCheck(void)
{
	while(U2RxInCnt!=U2RxoutCnt)
	{
		U2GetOneByte(U2RxDataTmp[U2RxoutCnt++]);
		if(U2RxoutCnt >= 100)
			U2RxoutCnt = 0;
	}
}
//====uart3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t U3RxDataTmp[100];
uint8_t U3RxInCnt = 0;
uint8_t U3RxoutCnt = 0;
void drvU3GetByte(uint8_t data)
{
	U3RxDataTmp[U3RxInCnt++] = data;
	if(U3RxInCnt >= 100)
		U3RxInCnt = 0;
}
void drvU3DataCheck(void)
{
	while(U3RxInCnt!=U3RxoutCnt)
	{
		U3GetOneByte(U3RxDataTmp[U3RxoutCnt++]);
		if(U3RxoutCnt >= 100)
			U3RxoutCnt = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====uart4
uint8_t U4RxDataTmp[100];
uint8_t U4RxInCnt = 0;
uint8_t U4RxoutCnt = 0;
void drvU4GetByte(uint8_t data)
{
	U4RxDataTmp[U4RxInCnt++] = data;
	if(U4RxInCnt >= 100)
		U4RxInCnt = 0;
}
void drvU4DataCheck(void)
{
	while(U4RxInCnt!=U4RxoutCnt)
	{
		U4GetOneByte(U4RxDataTmp[U4RxoutCnt++]);
		if(U4RxoutCnt >= 100)
			U4RxoutCnt = 0;
	}
}
//====uart5
uint8_t U5RxDataTmp[100];
uint8_t U5RxInCnt = 0;
uint8_t U5RxoutCnt = 0;
void drvU5GetByte(uint8_t data)
{
	U5RxDataTmp[U5RxInCnt++] = data;
	if(U5RxInCnt >= 100)
		U5RxInCnt = 0;
}
void drvU5DataCheck(void)
{
	while(U5RxInCnt!=U5RxoutCnt)
	{
		U5GetOneByte(U5RxDataTmp[U5RxoutCnt++]);
		if(U5RxoutCnt >= 100)
			U5RxoutCnt = 0;
	}
}
void DrvUartDataCheck(void)
{
	drvU1DataCheck();
	drvU2DataCheck();
	drvU3DataCheck();
	drvU4DataCheck();
	drvU5DataCheck();
}