#ifndef __AT24C256_H_
#define __AT24C256_H_

#include "STC15F2K60S2.H"
#include "intrins.h"
#include "tpyedef.h"
#include "string.h"

#define  _Nop()  _nop_()        //�����ָ��

sbit SDA = P3 ^ 4;            //ģ��I2C���ݴ���λ
sbit SCL = P3 ^ 5;            //ģ��I2Cʱ�ӿ���λ

typedef struct Env_struct{
	unsigned char Two_Menu_F3_E1;     //��ʱģʽ�����Ŷ�ģʽ
	unsigned char Two_Menu_F3_E2;     //���д洢����
	unsigned char Two_Menu_F4_E1;     //����ʱ��
	unsigned char Two_Menu_F5_E1;     //ѭ��ʱ��
	unsigned char Two_Menu_F6_E1;    //����ģʽ
	unsigned char Two_Menu_F6_E2;      //������������
	unsigned char Two_Menu_F6_E3;   //ѭ��ʱ���Ƿ񱨶�
	unsigned char Two_Menu_F6_E4;  //������С����
	unsigned char Two_Menu_F6_E5;  //��ʾ��LED���ȵ���
	unsigned char Two_Menu_F6_E6;    //��������
	unsigned char Two_Menu_F6_E7;     //��Ӣ��
	unsigned char Two_Menu_F6_E8;     //��˸
	unsigned char Two_Menu_F7_E1;     //��������
	unsigned char Two_Menu_F8_E1;     //����λ���Ƕ��λ
	unsigned char Two_Menu_Fb_E1;     //��������
	unsigned char Two_Menu_FC_E1;     //ʱ���л�

}Env_t;

//RF�洢��EE�е�λ��Ϊ0X0000~0X1C00
typedef struct {
	uint8_t region[4];      //����0~9~A~Z; //��ŵ�ASCII��
	uint32_t rf;            //RF��ַ�룬��8λΪ0��ʾ�˿��Ѿ���ʹ�á�����Ϊδʹ�� 
}RF_def;


#define  _Nop()  _nop_()        //�����ָ��
#define  I2C_ADDRESS 0xa0

#define  CALL_NUMBER   512	
#define  ALARM_NUMBER  64
#define  CANCEL_NUMBER 64
#define  HOST_NUMBER   32
#define  CALL_DATA_START   0X0020
#define  ALARM_DATA_START  0X1020
#define  CANCEL_DATA_START 0X1220
#define  HOST_DATA_START   0X1420
#define  SIN_KEY           0X1520
#define  MUL_KEY           0X1530
#define  BACK              0X1540

extern Env_t	EEPROM;

extern void DelayUs2x(unsigned char t);//�������� 
extern void DelayMs(unsigned char t);
extern void Start_I2c();
extern void Stop_I2c();
extern void SendByte(unsigned char c);
extern unsigned char RcvByte();
extern bit ISendStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no);
extern bit IRcvStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no);
extern void Ack_I2c(void);
extern void NoAck_I2c(void);

extern bit register_call_function(RF_def *pRF);
extern bit delete_call_function(unsigned char *buf);
extern bit register_host_function(RF_def *pRF);
extern bit delete_host_function(unsigned char *buf);
extern bit register_alarm_function(RF_def *pRF);
extern bit delete_alarm_function(unsigned char *buf);
extern bit register_cancel_function(RF_def *pRF);
extern bit delete_cancel_function(unsigned char *buf);
extern void Delete_all_data(void);
extern int8_t Find_RF_EEPROM(RF_def *p, uint32_t dat);
extern int8_t Find_RF_EEPROM_Call(RF_def *p, uint32_t dat);
extern int8_t Find_RF_EEPROM_Host(RF_def *p, uint32_t dat);
extern int8_t Find_RF_EEPROM_Alarm(RF_def *p, uint32_t dat);
extern int8_t Find_RF_EEPROM_Cancel(RF_def *p, uint32_t dat);

#endif