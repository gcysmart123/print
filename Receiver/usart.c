/***********************************************************
**************************************************************/
#include "usart.h"
#ifdef COPY_TWO_MACHINE
#include "key.h"
#include "menu.h"
#include "tm1629.h"
#endif
bit busy;
uint8_t usart2_enable = 0;
uint8_t usart2_data[4] = { 0, 0, 0,'\0' };
uint8_t usart2_num = 0;
uint8_t usart2_right = 0;
uint8_t usart2_begin = 0;
static const unsigned char* OKSPI = "OKSPI";
static const unsigned char* SPIOK = "SPIOK";
static const unsigned char* OK = "OK";

#ifdef COPY_TWO_MACHINE
uint8_t usart1_begin = 0;
uint8_t usart1_num = 0;
#endif
/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4 using 1
{
	unsigned char temp=0;
    if (RI)
    {
        RI = 0;                 //���RIλ
#ifdef COPY_TWO_MACHINE
		temp = SBUF;
		if (return_func_index() == TWO_MENU_FE_E1_SET && Two_Menu_FE_status == 1)
		{
			if (temp == 0xAA) //AA��ʾ������ȷ д2 ��ַ��8
			{
				Two_Menu_FE_status = 2;
			}
			else if (temp == 0x55) //55��ʾ���մ��� д0�ط�
			{
				Two_Menu_FE_status = 0;
			}
		}
		else if (return_func_index() == TWO_MENU_FE_E2_SET)
		{
			if (temp == 0xAA && usart1_begin == 0)
			{
				Two_Menu_FE_E2_data[0] = 0xAA; //��λ
				usart1_num = 1;//�������
				usart1_begin = 1;//��ʼλ��
			}
			else if (usart1_begin == 1)
			{
				Two_Menu_FE_E2_data[usart1_num] = temp;
				usart1_num++;
				if (usart1_num == 10)
				{
					usart1_num = 0;
					usart1_begin = 0;
					if (Two_Menu_FE_E2_data[0] == 0xAA && Two_Menu_FE_E2_data[9] == 0x55) //�ж���ȷ
					{
						Two_Menu_FE_E2_table = 1;
					}
					else
					{
						Usart1_SendData(0x55);
					}

				}
			}
		}

#endif
      //  computer_send_data_to_mcu();
      //  P0 = SBUF;              //P0��ʾ��������
      //  P22 = RB8;              //P2.2��ʾУ��λ
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy = 0;               //��æ��־
    }
}

/*----------------------------
UART2 �жϷ������
-----------------------------*/
void Uart2() interrupt 8 using 1
{
	uint8_t temp=0;
	if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;         //���S2RIλ
		temp = S2BUF;
		if (usart2_enable == 1 )
		{
			if (usart2_begin == 1)
			{
				usart2_data[usart2_num++] = temp;
			}

			if (temp == 'O'  &&  usart2_begin == 0)
			{
				usart2_begin = 1;
				usart2_num = 1;
				usart2_data[0] = temp;
			}
		}
		if (usart2_num == 2)
		{
			usart2_enable = 0;
			usart2_begin = 0;
			if (strcmp(usart2_data, OK) == 0)
			{
				usart2_right = 1;
			}
		}

		//P0 = S2BUF;             //P0��ʾ��������
		//P2 = (S2CON & S2RB8);   //P2.2��ʾУ��λ
	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;         //���S2TIλ
		busy = 0;               //��æ��־
	}
}

/*----------------------------
���ʹ�������with 8 bits
----------------------------*/
void Usart1_SendData(BYTE dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //����У��λΪ0
#endif
    }
    busy = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void Usart1_SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
		Usart1_SendData(*s++);         //���͵�ǰ�ַ�
    }
}

/*----------------------------
���ʹ�������
----------------------------*/
void Usart2_SendData(BYTE dat)
{
	while (busy);               //�ȴ�ǰ������ݷ������
	ACC = dat;                  //��ȡУ��λP (PSW.0)
	if (P)                      //����P������У��λ
	{
#if (PARITYBIT == ODD_PARITY)
		S2CON &= ~S2TB8;        //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
		S2CON |= S2TB8;         //����У��λΪ1
#endif
	}
	else
	{
#if (PARITYBIT == ODD_PARITY)
		S2CON |= S2TB8;         //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
		S2CON &= ~S2TB8;        //����У��λΪ0
#endif
	}
	busy = 1;
	S2BUF = ACC;                //д���ݵ�UART2���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void Usart2_SendString(char *s)
{
	while (*s)                  //����ַ���������־
	{
		Usart2_SendData(*s++);         //���͵�ǰ�ַ�
	}
}

#ifdef PRINTER
/*----------------------------
���ʹ�������with 8 bits
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //����У��λΪ0
#endif
    }
    busy = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}
#endif
/*uart init with baud settings----transfer mode-----open serial port*/
void UsartInit(void)
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
    
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
//  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

	P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
	//  P_SW2 |= S2_S0;             //S2_S0=1 (P4.6/RxD2_2, P4.7/TxD2_2)

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8λ�ɱ䲨����
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
#endif

#if (PARITYBIT == NONE_PARITY)
	S2CON = 0x50;               //8λ�ɱ䲨����
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
	S2CON = 0xda;               //9λ�ɱ䲨����,У��λ��ʼΪ1
#elif (PARITYBIT == SPACE_PARITY)
	S2CON = 0xd2;               //9λ�ɱ䲨����,У��λ��ʼΪ0
#endif


    T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                     //ʹ�ܴ����ж�
	IE2 = 0x01;                 //ʹ�ܴ���2�ж�
    EA = 1;

	//Usart1_SendString("STC15F2K60S2\r\nUart Test !\r\n");
	//Usart2_SendString("STC15F2K60S2\r\nUart2 Test !\r\n");
	
}

void uart_printf(const char *fmt, ...)
{
	va_list ap;
	char xdata string[256];//�����ڲ���չRAM���Ƿ����ⲿRAM�����ܳ����ڲ���չRAM��С(�˴�Ϊ1024)  

	va_start(ap, fmt);
	vsprintf(string, fmt, ap);//�˴�Ҳ����ʹ��sprintf�������÷���࣬�Լ��޸ļ��ɣ��˴���ȥ  
	Usart1_SendString(string);
	va_end(ap);
}

///*Switch to uart2 port by hardware*/
//void usart2_SendData(void)
//{
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC; 
//}
//
///*Switch to uart1 port by hardware*/
//void usart1_SendData(void)
//{
//    ACC = P_SW1;
//    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
//}

