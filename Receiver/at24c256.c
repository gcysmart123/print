#include "at24c256.h"
#include "key.h"
#include "usart.h"
#include "menu.h"
#include "gd5800.h"

bit ack;	              //Ӧ���־λ
Env_t	EEPROM;
void DelayUs2x(unsigned char t)
{
	while (--t);
}
/*------------------------------------------------
mS��ʱ����������������� unsigned char t���޷���ֵ
unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
void DelayMs(unsigned char t)
{
	while (t--)
	{
		//������ʱ1mS
		DelayUs2x(245);
		DelayUs2x(245);
	}
}


/*------------------------------------------------
��������
------------------------------------------------*/
void Start_I2c()
{
	SDA = 1;   //������ʼ�����������ź�
	_Nop();
	SCL = 1;
	_Nop();    //��ʼ��������ʱ�����4.7us,��ʱ
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA = 0;     //������ʼ�ź�
	_Nop();    //��ʼ��������ʱ�����4��
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;    //ǯסI2C���ߣ�׼�����ͻ��������
	_Nop();
	_Nop();
}
/*------------------------------------------------
��������
------------------------------------------------*/
void Stop_I2c()
{
	SDA = 0;    //���ͽ��������������ź�
	_Nop();   //���ͽ���������ʱ���ź�
	SCL = 1;    //������������ʱ�����4��
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA = 1;    //����I2C���߽����ź�
	_Nop();
	_Nop();
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
�ֽ����ݴ��ͺ���
����ԭ��: void  SendByte(unsigned char c);
����:  ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0 ��)
��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
------------------------------------------------------------------*/
void  SendByte(unsigned char c)
{
	unsigned char BitCnt;
	for (BitCnt = 0; BitCnt<8; BitCnt++)  //Ҫ���͵����ݳ���Ϊ8λ
	{
		if ((c << BitCnt) & 0x80)
			SDA = 1;   //�жϷ���λ
		else
			SDA = 0;
		_Nop();
		SCL = 1;               //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
		_Nop();
		_Nop();             //��֤ʱ�Ӹߵ�ƽ���ڴ���4��
		_Nop();
		_Nop();
		_Nop();
		SCL = 0;
	}
	_Nop();
	_Nop();
	SDA = 1;               //8λ��������ͷ������ߣ�׼������Ӧ��λ
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();
	_Nop();
	if (SDA == 1)ack = 0;
	else ack = 1;        //�ж��Ƿ���յ�Ӧ���ź�
	SCL = 0;
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
�ֽ����ݴ��ͺ���
����ԭ��: unsigned char  RcvByte();
����:  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
���������Ӧ������
------------------------------------------------------------------*/
unsigned char  RcvByte()
{
	unsigned char retc;
	unsigned char BitCnt;
	retc = 0;
	SDA = 1;             //��������Ϊ���뷽ʽ
	for (BitCnt = 0; BitCnt<8; BitCnt++)
	{
		_Nop();
		SCL = 0;       //��ʱ����Ϊ�ͣ�׼����������λ
		_Nop();
		_Nop();      //ʱ�ӵ͵�ƽ���ڴ���4.7us
		_Nop();
		_Nop();
		_Nop();
		SCL = 1;       //��ʱ����Ϊ��ʹ��������������Ч
		_Nop();
		_Nop();
		retc = retc << 1;
		if (SDA == 1)retc = retc + 1; //������λ,���յ�����λ����retc��
		_Nop();
		_Nop();
	}
	SCL = 0;
	_Nop();
	_Nop();
	return(retc);
}


/*----------------------------------------------------------------
Ӧ���Ӻ���
ԭ��:  void Ack_I2c(void);

----------------------------------------------------------------*/
void Ack_I2c(void)
{
	SDA = 0;
	_Nop();
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();              //ʱ�ӵ͵�ƽ���ڴ���4��
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;               //��ʱ���ߣ�ǯסI2C�����Ա��������
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
��Ӧ���Ӻ���
ԭ��:  void NoAck_I2c(void);

----------------------------------------------------------------*/
void NoAck_I2c(void)
{
	SDA = 1;
	_Nop();
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();              //ʱ�ӵ͵�ƽ���ڴ���4��
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;                //��ʱ���ߣ�ǯסI2C�����Ա��������
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
�����ӵ�ַ�������Ͷ��ֽ����ݺ���
����ԭ��: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
�������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
----------------------------------------------------------------*/
bit ISendStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no)
{
	unsigned char i;
	Start_I2c();               //��������
	SendByte(sla);             //����������ַ
	if (ack == 0)return(0);
	SendByte(suba >> 8);            //���������ӵ�ַ
	if (ack == 0)return(0);
	SendByte(suba);
	if (ack == 0)return(0);
	for (i = 0; i<no; i++)
	{
		SendByte(*s);            //��������
		if (ack == 0)return(0);
		s++;
	}
	Stop_I2c();                  //��������
	return(1);
}


/*----------------------------------------------------------------
�����ӵ�ַ������ȡ���ֽ����ݺ���
����ԭ��: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
�������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
----------------------------------------------------------------*/
bit IRcvStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no)
{
	unsigned char i;
	Start_I2c();               //��������
	SendByte(sla);             //����������ַ
	if (ack == 0)return(0);
	SendByte(suba >> 8);            //���������ӵ�ַ
	if (ack == 0)return(0);
	SendByte(suba);            //���������ӵ�ַ
	if (ack == 0)return(0);
	Start_I2c();
	SendByte(sla + 1);
	if (ack == 0)return(0);
	for (i = 0; i<no - 1; i++)
	{
		*s = RcvByte();              //��������
		Ack_I2c();                //���;ʹ�λ 
		s++;
	}
	*s = RcvByte();
	NoAck_I2c();                 //���ͷ�Ӧλ
	Stop_I2c();                    //��������
	return(1);
}

bit register_call_function(RF_def *pRF)
{
	uint16_t addr;
	uint16_t i;
	RF_def RFtmp;
	uint16_t tmp = 0;
	addr = CALL_DATA_START;

	for (i = 0; i < CALL_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (!(RFtmp.rf & 0xff000000))
		{

			if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
			{
				RFtmp.rf &= 0x00fffff0;
				pRF->rf &= 0x00fffff0;
			}

			if (RFtmp.rf == pRF->rf)    //�����RF�����棬���¸���
			{
				memcpy(Two_Menu_F1_E1, RFtmp.region, 4);
				Two_Menu_F1_E1[4] = 0;
				GD5800_select_chapter_new(SHIBAI);
				return 1;
			}
		}
		else
		{
			if (!tmp) tmp = addr;
		}
		addr += sizeof(RF_def);
	}
	ISendStr(I2C_ADDRESS, tmp, (uint8_t *)pRF, sizeof(RF_def));
	if (tmp != 0)
	{
		GD5800_select_chapter_new(CHENGGONG);
	}
	else
	{
		GD5800_select_chapter_new(SHIBAI);
		return 1;
	}
	//EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
	return 0;
}


bit register_host_function(RF_def *pRF)
{
	uint16_t addr;
	uint16_t i;
	RF_def RFtmp;
	uint16_t tmp = 0;
	addr = HOST_DATA_START;

	for (i = 0; i < HOST_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (!(RFtmp.rf & 0xff000000))
		{

			if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
			{
				RFtmp.rf &= 0x00fffff0;
				pRF->rf &= 0x00fffff0;
			}

			if (RFtmp.rf == pRF->rf)    //�����RF�����棬���¸���
			{
				memcpy(Two_Menu_F1_E2, RFtmp.region, 4);
				Two_Menu_F1_E2[4] = 0;
				GD5800_select_chapter(SHIBAI);
				return 1;
			}
		}
		else
		{
			if (!tmp) tmp = addr;
		}
		addr += sizeof(RF_def);
	}
	ISendStr(I2C_ADDRESS, tmp, (uint8_t *)pRF, sizeof(RF_def));
	//EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
	if (tmp != 0)
	{
		GD5800_select_chapter_new(CHENGGONG);
	}
	else
	{
		GD5800_select_chapter_new(SHIBAI);
		return 1;
	}
	return 0;
}

bit register_alarm_function(RF_def *pRF)
{
	uint16_t addr;
	uint16_t i;
	RF_def RFtmp;
	uint16_t tmp = 0;
	addr = ALARM_DATA_START;

	for (i = 0; i < ALARM_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (!(RFtmp.rf & 0xff000000))
		{

			if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
			{
				RFtmp.rf &= 0x00fffff0;
				pRF->rf &= 0x00fffff0;
			}

			if (RFtmp.rf == pRF->rf)    //�����RF�����棬���¸���
			{
				memcpy(Two_Menu_F1_E3, RFtmp.region, 4);
				Two_Menu_F1_E3[4] = 0;
				GD5800_select_chapter(SHIBAI);
				return 1;
			}
		}
		else
		{
			if (!tmp) tmp = addr;
		}
		addr += sizeof(RF_def);
	}
	ISendStr(I2C_ADDRESS, tmp, (uint8_t *)pRF, sizeof(RF_def));
	//EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
	if (tmp != 0)
	{
		GD5800_select_chapter_new(CHENGGONG);
	}
	else
	{
		GD5800_select_chapter_new(SHIBAI);
		return 1;
	}
	return 0;
}

bit register_cancel_function(RF_def *pRF)
{
	uint16_t addr;
	uint16_t i;
	RF_def RFtmp;
	uint16_t tmp = 0;
	addr = CANCEL_DATA_START;

	for (i = 0; i < CANCEL_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (!(RFtmp.rf & 0xff000000))
		{

			if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
			{
				RFtmp.rf &= 0x00fffff0;
				pRF->rf &= 0x00fffff0;
			}

			if (RFtmp.rf == pRF->rf)    //�����RF�����棬���¸���
			{
				memcpy(Two_Menu_F1_E4, RFtmp.region, 4);
				Two_Menu_F1_E4[4] = 0;
				GD5800_select_chapter(SHIBAI);
				return 1;
			}
		}
		else
		{
			if (!tmp) tmp = addr;
		}
		addr += sizeof(RF_def);
	}
	ISendStr(I2C_ADDRESS, tmp, (uint8_t *)pRF, sizeof(RF_def));
	//EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
	if (tmp != 0)
	{
		GD5800_select_chapter_new(CHENGGONG);
	}
	else
	{
		GD5800_select_chapter_new(SHIBAI);
		return 1;
	}
	return 0;
}

bit delete_call_function(unsigned char *buf)//bufΪ���������ָ��
{
	RF_def RFtmp;
	uint16_t addr = CALL_DATA_START;
	uint16_t i;
	unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	if (*(volatile uint32_t*)buf == 0x00000000ul)
	{
		for (i = 0; i<128; i++)
		{
			ISendStr(I2C_ADDRESS, addr + (i << 5), dofly, 32);                   //д��24c02
			delay10ms();
		}

	}
	else
	{
		for (i = 0; i < CALL_NUMBER; i++)
		{
			IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			if (!(RFtmp.rf & 0xff000000ul))
			{
				if (*(volatile uint32_t*)buf == *(volatile uint32_t*)(RFtmp.region))
				{
					memset(&RFtmp, 0xff, sizeof(RF_def));
					ISendStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					//EEP_WriteBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					break;
				}
			}
			addr += sizeof(RF_def);
		}
	}
	return 0;
}

bit delete_host_function(unsigned char *buf)//bufΪ���������ָ��
{
	RF_def RFtmp;
	uint16_t addr = HOST_DATA_START;
	uint16_t i;
	unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	if (*(volatile uint32_t*)buf == 0x00000000ul)
	{
		for (i = 0; i<8; i++)
		{
			ISendStr(I2C_ADDRESS, addr+( i << 5), dofly, 32);                   //д��24c02
			delay10ms();
		}

	}
	else
	{
		for (i = 0; i < HOST_NUMBER; i++)
		{
			IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			if (!(RFtmp.rf & 0xff000000ul))
			{
				if (*(volatile uint32_t*)buf == *(volatile uint32_t*)(RFtmp.region))
				{
					memset(&RFtmp, 0xff, sizeof(RF_def));
					ISendStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					//EEP_WriteBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					break;
				}
			}
			addr += sizeof(RF_def);
		}
	}
	return 0;
}

bit delete_alarm_function(unsigned char *buf)//bufΪ���������ָ��
{
	RF_def RFtmp;
	uint16_t addr = ALARM_DATA_START;
	uint16_t i;
	unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	if (*(volatile uint32_t*)buf == 0x00000000ul)
	{
		for (i = 0; i<16; i++)
		{
			ISendStr(I2C_ADDRESS, addr + (i << 5), dofly, 32);                   //д��24c02
			delay10ms();
		}

	}
	else
	{
		for (i = 0; i < ALARM_NUMBER; i++)
		{
			IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			//	EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			if (!(RFtmp.rf & 0xff000000ul))
			{
				if (*(volatile uint32_t*)buf == *(volatile uint32_t*)(RFtmp.region))
				{
					memset(&RFtmp, 0xff, sizeof(RF_def));
					ISendStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					//EEP_WriteBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					break;
				}
			}
			addr += sizeof(RF_def);
		}
	}
	return 0;
}

bit delete_cancel_function(unsigned char *buf)//bufΪ���������ָ��
{
	RF_def RFtmp;
	uint16_t addr = CANCEL_DATA_START;
	uint16_t i;
	unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	if (*(volatile uint32_t*)buf == 0x00000000ul)
	{
		for (i = 0; i<16; i++)
		{
			ISendStr(I2C_ADDRESS, addr + (i << 5), dofly, 32);                   //д��24c02
			delay10ms();
		}

	}
	else
	{
		for (i = 0; i < CANCEL_NUMBER; i++)
		{
			IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
			if (!(RFtmp.rf & 0xff000000ul))
			{
				if (*(volatile uint32_t*)buf == *(volatile uint32_t*)(RFtmp.region))
				{
					memset(&RFtmp, 0xff, sizeof(RF_def));
					ISendStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					//EEP_WriteBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));
					break;
				}
			}
			addr += sizeof(RF_def);
		}
	}
	return 0;
}

void Delete_all_data(void)
{
//	uint8_t i,j;
//	int8_t buf[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
//	uint16_t addr = 0;
//	int8_t buf1[32];
////	memset(buf, 0xff, sizeof(buf));
//	addr = 0;
//	for (i = 0; i<168; i++)
//	{	
//		addr = i << 5;
//		ISendStr(I2C_ADDRESS, addr, buf, 32);
//		delay10ms();
//		//EEP_WriteBytes(addr, buf, sizeof(buf));
//		//addr += 32;
//	}
//	uart_printf("24c read:");
//	for (i = 0; i<168; i++)
//	{
//		IRcvStr(I2C_ADDRESS, addr, buf, 32);
//		delay10ms();
//		for (j = 0; j < 32; j++)
//		{
//			uart_printf("%02x ", (unsigned int)buf[j]);
//		}
//		uart_printf("\n\r");
//		//EEP_WriteBytes(addr, buf, sizeof(buf));
//		addr += sizeof(buf);
//	}

	unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	unsigned int k;
	for (k = 0; k<168; k++)
	{
		ISendStr(I2C_ADDRESS, 0 + (k << 5), dofly, 32);                   //д��24c02
		delay10ms();
	}
}


int8_t Find_RF_EEPROM_Call(RF_def *p, uint32_t dat)
{
	RF_def RFtmp;
	uint16_t addr;
	uint16_t i;
	addr = CALL_DATA_START;

	for (i = 0; i<CALL_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
		{
			RFtmp.rf &= 0x00fffff0;
			dat &= 0x00fffff0;
		}

		if (RFtmp.rf == dat)
		{
			memcpy(p, &RFtmp, sizeof(RF_def));
			return 1;
		}
		addr += sizeof(RF_def);
	}
	return 0;
}

int8_t Find_RF_EEPROM_Host(RF_def *p, uint32_t dat)
{
	RF_def RFtmp;
	uint16_t addr;
	uint16_t i;
	addr = HOST_DATA_START;

	for (i = 0; i<HOST_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
		{
			RFtmp.rf &= 0x00fffff0;
			dat &= 0x00fffff0;
		}

		if (RFtmp.rf == dat)
		{
			memcpy(p, &RFtmp, sizeof(RF_def));
			return 1;
		}
		addr += sizeof(RF_def);
	}
	return 0;
}

int8_t Find_RF_EEPROM_Alarm(RF_def *p, uint32_t dat)
{
	RF_def RFtmp;
	uint16_t addr;
	uint16_t i;
	addr = ALARM_DATA_START;

	for (i = 0; i<ALARM_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
		{
			RFtmp.rf &= 0x00fffff0;
			dat &= 0x00fffff0;
		}

		if (RFtmp.rf == dat)
		{
			memcpy(p, &RFtmp, sizeof(RF_def));
			return 1;
		}
		addr += sizeof(RF_def);
	}
	return 0;
}

int8_t Find_RF_EEPROM_Cancel(RF_def *p, uint32_t dat)
{
	RF_def RFtmp;
	uint16_t addr;
	uint16_t i;
	addr = CANCEL_DATA_START;

	for (i = 0; i<CANCEL_NUMBER; i++)
	{
		IRcvStr(I2C_ADDRESS, addr, (uint8_t *)&RFtmp, sizeof(RF_def));
		//EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

		if (return_Two_Menu_F8_E1() == 2)      //���ģʽ��λΪ0
		{
			RFtmp.rf &= 0x00fffff0;
			dat &= 0x00fffff0;
		}

		if (RFtmp.rf == dat)
		{
			memcpy(p, &RFtmp, sizeof(RF_def));
			return 1;
		}
		addr += sizeof(RF_def);
	}
	return 0;
}

int8_t Find_RF_EEPROM(RF_def *p, uint32_t dat)
{
	if (Find_RF_EEPROM_Cancel(p, dat))
	{
		return 1;
	}
	else if (Find_RF_EEPROM_Alarm(p, dat))
	{
		return 2;
	}
	else if (Find_RF_EEPROM_Call(p, dat))
	{
		return 3;
	}
	else
	{
		return 0;
	}
}