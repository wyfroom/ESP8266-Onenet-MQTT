/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
//#include "stm32f10x.h"

//�����豸
#include "esp8266.h"

//Э���ļ�
#include "onenet.h"
#include "mqttkit.h"

//Ӳ������
//#include "usart.h"
//#include "delay.h"
//#include "led.h"
#include "main.h"

//C��
#include <string.h>
#include <stdio.h>


#define PROID		"59xx31"

#define AUTH_INFO	"12xx56"

#define DEVID		"1067xx6734"


extern unsigned char esp8266_buf[128];


//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	printf("OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:printf("Tips:	���ӳɹ�\r\n");status = 0;break;
					
					case 1:printf("WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:printf("WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					
					default:printf("ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		printf("WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

		
unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[30];
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, "{");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"hum\":%d,", 0);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"temp\":%d,",0);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"mq4\":%d,",0);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"bmp180\":%.2f",0.0);
	strcat(buf, text);
	
	strcat(buf, "}");
	
	return strlen(buf);

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[128];
	
	short body_len = 0, i = 0;
	
	printf("Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 3, &mqttPacket) == 0)							//���
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
			printf("Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
			printf("WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
				printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
					printf("Tips:	Send CmdResp\r\n");
					
					ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf("Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, ':');					//����':'

	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		
		if(strstr((char *)req_payload, "redled"))		//����"redled"
		{
			if(num == 1)								//�����������Ϊ1������
			{
				//Led5_Set(LED_ON);
                printf("Led5_Set(LED_ON");
			}
			else if(num == 0)							//�����������Ϊ0�������
			{
				//Led5_Set(LED_OFF);
                printf("Led5_Set(LED_OFF");
			}
		}
														//��ͬ
		else if(strstr((char *)req_payload, "greenled"))
		{
			if(num == 1)
			{
				//Led4_Set(LED_ON);
                printf("Led4_Set(LED_ON");
			}
			else if(num == 0)
			{
				//Led4_Set(LED_OFF);
                printf("Led4_Set(LED_OFF");
			}
		}
		else if(strstr((char *)req_payload, "yellowled"))
		{
			if(num == 1)
			{
				//Led3_Set(LED_ON);
                printf("Led3_Set(LED_ON");
			}
			else if(num == 0)
			{
				//Led2_Set(LED_OFF);
                printf("Led2_Set(LED_OFF");
			}
		}
		else if(strstr((char *)req_payload, "blueled"))
		{
			if(num == 1)
			{
				//Led2_Set(LED_ON);
                printf("Led2_Set(LED_ON");
			}
			else if(num == 0)
			{
				//Led2_Set(LED_OFF);
                printf("Led2_Set(LED_OFF");
			}
		}
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
