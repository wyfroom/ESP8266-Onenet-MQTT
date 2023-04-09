/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "usart.h"



/*-------------------------printf重映射---------------------------*/
#include <stdio.h>
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    //具体哪个串口可以更改huart1为其它串口
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1 , 0xffff);
    return ch;
}



/*---------------------------串口接收中断--------------------------*/

extern void ESP8266_IRQHandler(void);
extern unsigned short esp8266_cnt;
extern unsigned char esp8266_buf[128];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		if(huart == &huart3)//esp8266接收云平台数据
		{
			if(esp8266_cnt >= sizeof(esp8266_buf))
			{            
				esp8266_cnt = 0; //防止串口被刷爆
			}
			esp8266_buf[esp8266_cnt++] = Uart3_RxData;

			HAL_UART_Receive_IT(&huart3,(uint8_t *)&Uart3_RxData, 1); //&取地址
		}
}



