#ifndef __UART_H__
#define __UART_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define RXBUFFERSIZE1  256     				//�������ֽ���  ����1
char Uart1_RxBuffer[RXBUFFERSIZE1];   	//��������
uint8_t Uart1_RxData;									//�����жϻ���
uint8_t Uart1_Rx_Cnt = 0;							//���ջ������

#define RXBUFFERSIZE3  256     				//�������ֽ���	����3
char Uart3_RxBuffer[RXBUFFERSIZE3];   	//��������
uint8_t Uart3_RxData;									//�����жϻ���
uint8_t Uart3_Rx_Cnt = 0;							//���ջ������


#endif /* __UART_H__ */


