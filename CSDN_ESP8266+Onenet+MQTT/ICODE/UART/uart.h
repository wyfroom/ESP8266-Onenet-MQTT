#ifndef __UART_H__
#define __UART_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define RXBUFFERSIZE1  256     				//最大接收字节数  串口1
char Uart1_RxBuffer[RXBUFFERSIZE1];   	//接收数据
uint8_t Uart1_RxData;									//接收中断缓冲
uint8_t Uart1_Rx_Cnt = 0;							//接收缓冲计数

#define RXBUFFERSIZE3  256     				//最大接收字节数	串口3
char Uart3_RxBuffer[RXBUFFERSIZE3];   	//接收数据
uint8_t Uart3_RxData;									//接收中断缓冲
uint8_t Uart3_Rx_Cnt = 0;							//接收缓冲计数


#endif /* __UART_H__ */


