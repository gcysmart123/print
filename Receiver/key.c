#include "key.h"
#include "usart.h"
#include "tm1629.h"
#include "menu.h"
#include "timer.h"
#include "ds1302.h"
#include "at24c256.h"
#include "gd5800.h"
#include "ev1527.h"
#include "global.h"
#include "pca.h"
#include "printer.h"

unsigned char sound_table = 0;  /*���������Ļ������sjz*/
unsigned char accumulate_decoder = 0;

unsigned char func_index = 0; //�༶�˵���������
void(*current_operation_index)();// �༶�˵�����ָ��

unsigned char Two_Menu_F1_E1[5] = { 0, 0, 0, 1 }; //F1_E1
unsigned char Two_Menu_F1_E2[5] = { 0, 0, 0, 1 }; //F1_E2
unsigned char Two_Menu_F1_E3[5] = { 0, 0, 0, 1 }; //F1_E3
unsigned char Two_Menu_F1_E4[5] = { 0, 0, 0, 1 }; //F1_E4
unsigned char Two_Menu_F2_E1[5] = { 0, 0, 0, 1 }; //F2_E1
unsigned char Two_Menu_F2_E2[5] = { 0, 0, 0, 1 }; //F2_E2
unsigned char Two_Menu_F2_E3[5] = { 0, 0, 0, 1 }; //F2_E3
unsigned char Two_Menu_F2_E4[5] = { 0, 0, 0, 1 }; //F2_E4

unsigned char Two_Menu_F3_E1 = 1; //��ʱģʽ�����Ŷ���ʾ
unsigned char Two_Menu_F3_E2 = 1; //����ʱ��洢����

unsigned char Two_Menu_F4_E1 = 0; //����ʱ��
unsigned char Two_Menu_F5_E1 = 0; //ѭ�����ʱ��

unsigned char Two_Menu_F6_E1 = 0; //�򵥱���
unsigned char Two_Menu_F6_E2 = 1; //������������
unsigned char Two_Menu_F6_E3 = 0; //ѭ��ʱ���Ƿ񱨶�
unsigned char Two_Menu_F6_E4 = 6; //������С����
unsigned char Two_Menu_F6_E5 = 3; //��ʾ��LED���ȵ���
unsigned char Two_Menu_F6_E6 = 1; //������������/*sjz���������������� �ε��������߱�����ͱ���*/
unsigned char Two_Menu_F6_E7 = 0; //������������
unsigned char Two_Menu_F6_E8 = 0; //��˸�볣��

unsigned char Two_Menu_F7_E1 = 11; // E1Ĭ�ϼ��̹��� 999*9
unsigned char Two_Menu_F7_E2 = 0; // E2�������̹��� 9999*9
unsigned char Two_Menu_F7_E3 = 0; // E3�������̹��� 999*99
unsigned char Two_Menu_F7_E4 = 0; // E4�������̹��� 9999*99

unsigned char Two_Menu_F8_E1 = 2; // �������� �ఴ���л�

unsigned char Two_Menu_F8_E2 = 0; // ��λ����

unsigned char Two_Menu_Fb_E1 = 1; // ����������û�����Ź���
unsigned char Two_Menu_FC_E1 = 1; // ����������������----�������л�
unsigned char Two_Menu_Fd_E1 = 1; // E1 E2 E3 E4 E5 E6

#ifdef TRANSMIT_KEY
uint8_t   Transmit_Data_FANGQU=0;
uint8_t   Transmit_Data_set_FANGQU=0;
uint16_t Transmit_Data=0;
uint16_t Line_number=1;
uint8_t Send_status=0; //0������գ�1������
uint8_t transmit_confirm_key_press=0;
#endif

key_table code table[MENU_SIZE] =
{	// Ŀ������		    ��				��          ȷ��		 �˳�         ����
	{ MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, (*fun0) }, //����
#ifdef COPY_TWO_MACHINE
	{ ONE_MENU_F0, ONE_MENU_F1, ONE_MENU_FE, TWO_MENU_F0_MINUTE, MENU_STANDBY, (*fun1) }, //F0-F9
#else
	{ ONE_MENU_F0, ONE_MENU_F1, ONE_MENU_Fd, TWO_MENU_F0_MINUTE, MENU_STANDBY, (*fun1) }, //F0-F9
#endif
	{ ONE_MENU_F1, ONE_MENU_F2, ONE_MENU_F0, TWO_MENU_F1_E1   , MENU_STANDBY, (*fun2) },
	{ ONE_MENU_F2, ONE_MENU_F3, ONE_MENU_F1, TWO_MENU_F2_E1   , MENU_STANDBY, (*fun3) },
	{ ONE_MENU_F3, ONE_MENU_F4, ONE_MENU_F2, TWO_MENU_F3_E1   , MENU_STANDBY, (*fun4) },
	{ ONE_MENU_F4, ONE_MENU_F5, ONE_MENU_F3, TWO_MENU_F4_SET  , MENU_STANDBY, (*fun5) },
	{ ONE_MENU_F5, ONE_MENU_F6, ONE_MENU_F4, TWO_MENU_F5_SET  , MENU_STANDBY, (*fun6) },
	{ ONE_MENU_F6, ONE_MENU_F7, ONE_MENU_F5, TWO_MENU_F6_E1   , MENU_STANDBY, (*fun7) },
	{ ONE_MENU_F7, ONE_MENU_F8, ONE_MENU_F6, TWO_MENU_F7_E1   , MENU_STANDBY, (*fun8) },
	{ ONE_MENU_F8, ONE_MENU_F9, ONE_MENU_F7, TWO_MENU_F8_E1   , MENU_STANDBY, (*fun9) },
	{ ONE_MENU_F9, ONE_MENU_FA, ONE_MENU_F8, TWO_MENU_F9_E1   , MENU_STANDBY, (*fun10) },
	{ ONE_MENU_FA, ONE_MENU_Fb, ONE_MENU_F9, ONE_MENU_FA, MENU_STANDBY, (*fun11) }, //FA
	{ ONE_MENU_Fb, ONE_MENU_FC, ONE_MENU_FA, ONE_MENU_Fb, MENU_STANDBY, (*fun12) }, //Fb
	{ ONE_MENU_FC, ONE_MENU_Fd, ONE_MENU_Fb, ONE_MENU_FC, MENU_STANDBY, (*fun13) }, //FC
#ifdef COPY_TWO_MACHINE
	{ ONE_MENU_Fd, ONE_MENU_FE, ONE_MENU_FC, ONE_MENU_Fd, MENU_STANDBY, (*fun14) }, //Fd
#else
	{ ONE_MENU_Fd, ONE_MENU_F0, ONE_MENU_FC, ONE_MENU_Fd, MENU_STANDBY, (*fun14) }, //Fd

#endif
	{ TWO_MENU_F0_YEAR  , TWO_MENU_F0_YEAR  , TWO_MENU_F0_YEAR  , TWO_MENU_F0_WEEK , ONE_MENU_F0, (*fun15) }, //������ ��
	{ TWO_MENU_F0_MOUTH , TWO_MENU_F0_MOUTH , TWO_MENU_F0_MOUTH , TWO_MENU_F0_YEAR   , ONE_MENU_F0, (*fun16) }, //������ ��
	{ TWO_MENU_F0_DAY   , TWO_MENU_F0_DAY   , TWO_MENU_F0_DAY   , TWO_MENU_F0_MOUTH  , ONE_MENU_F0, (*fun17) }, //������ ��
	{ TWO_MENU_F0_WEEK  , TWO_MENU_F0_WEEK  , TWO_MENU_F0_WEEK  , TWO_MENU_F0_MINUTE  , ONE_MENU_F0, (*fun18) }, //������ ��
	{ TWO_MENU_F0_HOUR  , TWO_MENU_F0_HOUR  , TWO_MENU_F0_HOUR  , TWO_MENU_F0_DAY, ONE_MENU_F0, (*fun19) }, //������ Сʱ
	{ TWO_MENU_F0_MINUTE, TWO_MENU_F0_MINUTE, TWO_MENU_F0_MINUTE, TWO_MENU_F0_HOUR  , ONE_MENU_F0, (*fun20) }, //������ ����

	{ TWO_MENU_F1_E1, TWO_MENU_F1_E2, TWO_MENU_F1_E4, TWO_MENU_F1_E1_D4, ONE_MENU_F1, (*fun21) }, //F1�Ӳ˵�E1
	{ TWO_MENU_F1_E2, TWO_MENU_F1_E3, TWO_MENU_F1_E1, TWO_MENU_F1_E2_D4, ONE_MENU_F1, (*fun22) }, //F1�Ӳ˵�E2
	{ TWO_MENU_F1_E3, TWO_MENU_F1_E4, TWO_MENU_F1_E2, TWO_MENU_F1_E3_D4, ONE_MENU_F1, (*fun23) }, //F1�Ӳ˵�E3
	{ TWO_MENU_F1_E4, TWO_MENU_F1_E1, TWO_MENU_F1_E3, TWO_MENU_F1_E4_D4, ONE_MENU_F1, (*fun24) }, //F1�Ӳ˵�E4

	{ TWO_MENU_F2_E1, TWO_MENU_F2_E2, TWO_MENU_F2_E4, TWO_MENU_F2_E1_D4, ONE_MENU_F2, (*fun25) }, //F2�Ӳ˵�E1
	{ TWO_MENU_F2_E2, TWO_MENU_F2_E3, TWO_MENU_F2_E1, TWO_MENU_F2_E2_D4, ONE_MENU_F2, (*fun26) }, //F2�Ӳ˵�E2
	{ TWO_MENU_F2_E3, TWO_MENU_F2_E4, TWO_MENU_F2_E2, TWO_MENU_F2_E3_D4, ONE_MENU_F2, (*fun27) }, //F2�Ӳ˵�E3
	{ TWO_MENU_F2_E4, TWO_MENU_F2_E1, TWO_MENU_F2_E3, TWO_MENU_F2_E4_D4, ONE_MENU_F2, (*fun28) }, //F2�Ӳ˵�E4

	{ TWO_MENU_F3_E1, TWO_MENU_F3_E2, TWO_MENU_F3_E2, TWO_MENU_F3_E1_SET, ONE_MENU_F3, (*fun29) }, //F3�Ӳ˵�E1
	{ TWO_MENU_F3_E2, TWO_MENU_F3_E1, TWO_MENU_F3_E1, TWO_MENU_F3_E2_SET, ONE_MENU_F3, (*fun30) }, //F3�Ӳ˵�E2

	{ TWO_MENU_F4_SET, TWO_MENU_F4_SET, TWO_MENU_F4_SET, TWO_MENU_F4_SET, ONE_MENU_F4, (*fun31) }, //F4�Ӳ˵�
	{ TWO_MENU_F5_SET, TWO_MENU_F5_SET, TWO_MENU_F5_SET, TWO_MENU_F5_SET, ONE_MENU_F5, (*fun32) }, //F5�Ӳ˵�

	{ TWO_MENU_F6_E1, TWO_MENU_F6_E2, TWO_MENU_F6_E8, TWO_MENU_F6_E1_SET, ONE_MENU_F6, (*fun33) }, //F6�Ӳ˵�E1
	{ TWO_MENU_F6_E2, TWO_MENU_F6_E3, TWO_MENU_F6_E1, TWO_MENU_F6_E2_SET, ONE_MENU_F6, (*fun34) }, //F6�Ӳ˵�E2
	{ TWO_MENU_F6_E3, TWO_MENU_F6_E4, TWO_MENU_F6_E2, TWO_MENU_F6_E3_SET, ONE_MENU_F6, (*fun35) }, //F6�Ӳ˵�E3
	{ TWO_MENU_F6_E4, TWO_MENU_F6_E5, TWO_MENU_F6_E3, TWO_MENU_F6_E4_SET, ONE_MENU_F6, (*fun36) }, //F6�Ӳ˵�E4
	{ TWO_MENU_F6_E5, TWO_MENU_F6_E6, TWO_MENU_F6_E4, TWO_MENU_F6_E5_SET, ONE_MENU_F6, (*fun37) }, //F6�Ӳ˵�E5

	{ TWO_MENU_F7_E1, TWO_MENU_F7_E2, TWO_MENU_F7_E4, TWO_MENU_F7_E1_SET, ONE_MENU_F7, (*fun38) }, //F7�Ӳ˵�E1
	{ TWO_MENU_F7_E2, TWO_MENU_F7_E3, TWO_MENU_F7_E1, TWO_MENU_F7_E2_SET, ONE_MENU_F7, (*fun39) }, //F7�Ӳ˵�E2
	{ TWO_MENU_F7_E3, TWO_MENU_F7_E4, TWO_MENU_F7_E2, TWO_MENU_F7_E3_SET, ONE_MENU_F7, (*fun40) }, //F7�Ӳ˵�E3
	{ TWO_MENU_F7_E4, TWO_MENU_F7_E1, TWO_MENU_F7_E3, TWO_MENU_F7_E4_SET, ONE_MENU_F7, (*fun41) }, //F7�Ӳ˵�E4	

	{ TWO_MENU_F8_E1, TWO_MENU_F8_E2, TWO_MENU_F8_E2, TWO_MENU_F8_E1, ONE_MENU_F8, (*fun42) }, //F8�Ӳ˵�E1
	{ TWO_MENU_F8_E2, TWO_MENU_F8_E1, TWO_MENU_F8_E1, TWO_MENU_F8_E2_SET, ONE_MENU_F8, (*fun43) }, //F8�Ӳ˵�E2

	{ TWO_MENU_F9_E1, TWO_MENU_F9_E2, TWO_MENU_F9_E2, 0, ONE_MENU_F9, (*fun44) }, //F9�Ӳ˵�E1
	{ TWO_MENU_F9_E2, TWO_MENU_F9_E1, TWO_MENU_F9_E1, 0, ONE_MENU_F9, (*fun45) }, //F9�Ӳ˵�E2

	{ TWO_MENU_FA_SET, TWO_MENU_FA_SET, TWO_MENU_FA_SET, TWO_MENU_FA_SET, ONE_MENU_FA, (*fun46) }, //FA�Ӳ˵�
	{ TWO_MENU_Fb_SET, TWO_MENU_Fb_SET, TWO_MENU_Fb_SET, TWO_MENU_Fb_SET, ONE_MENU_Fb, (*fun47) }, //Fb�Ӳ˵�
	{ TWO_MENU_FC_SET, TWO_MENU_FC_SET, TWO_MENU_FC_SET, TWO_MENU_FC_SET, ONE_MENU_FC, (*fun48) }, //FC�Ӳ˵�
	{ TWO_MENU_Fd_SET, TWO_MENU_Fd_SET, TWO_MENU_Fd_SET, TWO_MENU_Fd_SET, ONE_MENU_Fd, (*fun49) }, //Fd�Ӳ˵�

	{ TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1, (*fun50) }, //F1_E1ע�������
	{ TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1, (*fun51) }, //F1_E1ע�������
	{ TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1, (*fun52) }, //F1_E1ע�������
	{ TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1, (*fun53) }, //F1_E1ע�������

	{ TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2, (*fun54) }, //F1_E2ע�������
	{ TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2, (*fun55) }, //F1_E2ע�������
	{ TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2, (*fun56) }, //F1_E2ע�������
	{ TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2, (*fun57) }, //F1_E2ע�������

	{ TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3, (*fun58) }, //F1_E3ע�ᱨ����
	{ TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3, (*fun59) }, //F1_E3ע�ᱨ����
	{ TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3, (*fun60) }, //F1_E3ע�ᱨ����
	{ TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3, (*fun61) }, //F1_E3ע�ᱨ����

	{ TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4, (*fun62) }, //F1_E3ע��ȡ����
	{ TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4, (*fun63) }, //F1_E3ע��ȡ����
	{ TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4, (*fun64) }, //F1_E3ע��ȡ����
	{ TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4, (*fun65) }, //F1_E3ע��ȡ����

	{ TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1, (*fun66) }, //F2_E1ɾ��������
	{ TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1, (*fun67) }, //F2_E1ɾ��������
	{ TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1, (*fun68) }, //F2_E1ɾ��������
	{ TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1, (*fun69) }, //F2_E1ɾ��������

	{ TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2, (*fun70) }, //F2_E2ɾ��������
	{ TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2, (*fun71) }, //F2_E2ɾ��������
	{ TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2, (*fun72) }, //F2_E2ɾ��������
	{ TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2, (*fun73) }, //F2_E2ɾ��������

	{ TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3, (*fun74) }, //F2_E3ɾ��������
	{ TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3, (*fun75) }, //F2_E3ɾ��������
	{ TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3, (*fun76) }, //F2_E3ɾ��������
	{ TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3, (*fun77) }, //F2_E3ɾ��������

	{ TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4, (*fun78) }, //F2_E4ɾ��ȡ����
	{ TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4, (*fun79) }, //F2_E4ɾ��ȡ����
	{ TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4, (*fun80) }, //F2_E4ɾ��ȡ����
	{ TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4, (*fun81) }, //F2_E4ɾ��ȡ����

	{ TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1, (*fun82) }, //F3_E1�����Ŷ���ʾ����ѭ����ʾ
	{ TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2, (*fun83) }, //F3_E2���ö��и���

	{ TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1, (*fun84) }, //F6_E1����ѡ��
	{ TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2, (*fun85) }, //F6_E2������������
	{ TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3, (*fun86) }, //F6_E3ѭ��ʱ���Ƿ񱨶�
	{ TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4, (*fun87) }, //F6_E4������С����
	{ TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5, (*fun88) }, //F6_E5��Ļ���ȵ���

	{ TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1, (*fun89) }, //F7_E1 999*9
	{ TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2, (*fun90) }, //F7_E2 9999*9
	{ TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3, (*fun91) }, //F7_E3 999*99
	{ TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4, (*fun92) }, //F7_E4 9999*99

	{ TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1, (*fun93) }, //F8_E1 ����λ�����λ�л�
	{ TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2, (*fun94) }, //F8_E2 ��λ����

	{ DECODER_MENU, DECODER_MENU, DECODER_MENU, DECODER_MENU, DECODER_MENU, (*fun95)},

	{ TWO_MENU_F6_E6, TWO_MENU_F6_E7, TWO_MENU_F6_E5, TWO_MENU_F6_E6_SET, ONE_MENU_F6, (*fun96) },
	{ TWO_MENU_F6_E6_SET, TWO_MENU_F6_E6_SET, TWO_MENU_F6_E6_SET, TWO_MENU_F6_E6_SET, TWO_MENU_F6_E6, (*fun97) },
	{ TWO_MENU_F6_E7, TWO_MENU_F6_E8, TWO_MENU_F6_E6, TWO_MENU_F6_E7_SET, ONE_MENU_F6, (*fun98) },
	{ TWO_MENU_F6_E7_SET, TWO_MENU_F6_E7_SET, TWO_MENU_F6_E7_SET, TWO_MENU_F6_E7_SET, TWO_MENU_F6_E7, (*fun99) },
	{ TWO_MENU_F6_E8, TWO_MENU_F6_E1, TWO_MENU_F6_E7, TWO_MENU_F6_E8_SET, ONE_MENU_F6, (*fun100) },
	{ TWO_MENU_F6_E8_SET, TWO_MENU_F6_E8_SET, TWO_MENU_F6_E8_SET, TWO_MENU_F6_E8_SET, TWO_MENU_F6_E8, (*fun101) },
#ifdef COPY_TWO_MACHINE
	{ ONE_MENU_FE, ONE_MENU_F0, ONE_MENU_Fd, ONE_MENU_FE, MENU_STANDBY, (*fun102) }, //Fd
	{ TWO_MENU_FE_E1, TWO_MENU_FE_E2, TWO_MENU_FE_E2, TWO_MENU_FE_E1_SET, ONE_MENU_FE, (*fun103) },
	{ TWO_MENU_FE_E2, TWO_MENU_FE_E1, TWO_MENU_FE_E1, TWO_MENU_FE_E2_SET, ONE_MENU_FE, (*fun104) },
	{ TWO_MENU_FE_E1_SET, TWO_MENU_FE_E1_SET, TWO_MENU_FE_E1_SET, TWO_MENU_FE_E1_SET, TWO_MENU_FE_E1, (*fun105) },
	{ TWO_MENU_FE_E2_SET, TWO_MENU_FE_E2_SET, TWO_MENU_FE_E2_SET, TWO_MENU_FE_E2_SET, TWO_MENU_FE_E2, (*fun106) },
#endif
};

unsigned int KeyScan(void)  //Keyboard scan function
{
	unsigned int Val = 0;
	unsigned char main_press_time_temp = 0;
	unsigned char single_long_press_process_complice = 0;
	unsigned char temp = 0;

	if (!(P33&&P36&&P37&&P25))//Press button
	{
		delay10ms();  //Remove jitter
		if (!(P33&&P36&&P37&&P25))   //Press button
		{
			clear_return_standby_time();
			if (!P33)  Val = 0x0038;
			if (!P36)  Val = 0x0034;
			if (!P37)  Val = 0x002c;
			if (!P25)  Val = 0x003c;
				if (Val == 0x0038)
				{
					clear_main_press_time();	//����˵������µ�ʱ��������
					set_main_press_time_table(1); //������Ӧ�ı�־λ����ʼ����ʱ��
					while (!P33 && !single_long_press_process_complice)
					{
						if ((return_Two_Menu_Fb_E1() == 0 && func_index == MENU_STANDBY) || (return_Two_Menu_Fb_E1() == 1 && (func_index == MENU_STANDBY || func_index == DECODER_MENU)))  //�������==0  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								func_index = ONE_MENU_F1;
								Val = 0;
								P55 = 1;
								P17 = 0;
								tm1629_f(0x01);
								if (return_Two_Menu_F6_E6() == 1)
								{
									GD5800_select_chapter(CUNCHUSHEZHI);
								}
								else
								{
									GD5800_select_chapter_new(DI);
								}
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E1_D1 || func_index == TWO_MENU_F2_E1_D2 || func_index == TWO_MENU_F2_E1_D3 || func_index == TWO_MENU_F2_E1_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_call_function(Two_Menu_F2_E1))
								{
									GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E2_D1 || func_index == TWO_MENU_F2_E2_D2 || func_index == TWO_MENU_F2_E2_D3 || func_index == TWO_MENU_F2_E2_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_host_function(Two_Menu_F2_E2))
								{
										GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E3_D1 || func_index == TWO_MENU_F2_E3_D2 || func_index == TWO_MENU_F2_E3_D3 || func_index == TWO_MENU_F2_E3_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_alarm_function(Two_Menu_F2_E3))
								{
									GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E4_D1 || func_index == TWO_MENU_F2_E4_D2 || func_index == TWO_MENU_F2_E4_D3 || func_index == TWO_MENU_F2_E4_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_cancel_function(Two_Menu_F2_E4))
								{
										GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == ONE_MENU_Fb)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_Fb_SET;
								temp = return_Two_Menu_Fb_E1();
								Show_one_number(temp);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == ONE_MENU_FC)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_FC_SET;
								temp = return_Two_Menu_FC_E1();
								Show_one_number(temp);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == ONE_MENU_Fd)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_Fd_SET;
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
#ifdef COPY_TWO_MACHINE
						else if (func_index == ONE_MENU_FE)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_FE_E1;
								tm1629_E(0x01);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
#endif
						else if (func_index == TWO_MENU_F8_E1)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_F8_E1_SET;
								temp = return_Two_Menu_F8_E1();
								Show_one_number(temp);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F9_E2)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								GD5800_select_chapter(CHENGGONG);
								var_init();
								env_load();
								IAP_CONTR = 0X60;
								
								//tm1629_init();
								//env_init();
								func_index = MENU_STANDBY;
								single_long_press_process_complice = 1;
								clear_main_press_time();
								WDT_CONTR = 0x30;  /* 4 seconds to ι��*/
							}
						}
						else if (func_index == TWO_MENU_F9_E1)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								GD5800_select_chapter(CHENGGONG);
								var_init();
								key_init();
								env_load();
								ISendStr(I2C_ADDRESS, SIN_KEY, single_key, 16);
								delay10ms();
								ISendStr(I2C_ADDRESS, MUL_KEY, multiple_key, 16);
								delay10ms();
								Delete_all_data();
								delay10ms();
								IAP_CONTR = 0X60;

								//tm1629_init();
								//env_init();
								func_index = MENU_STANDBY;
								single_long_press_process_complice = 1;
								clear_main_press_time();
								WDT_CONTR = 0x30;  /* 4 seconds to ι��*/
							}
						}

					}
				}
				while (!(P33&&P36&&P37&&P25));
				delay10ms();
				while (!(P33&&P36&&P37&&P25));
				set_main_press_time_table(0);//�����ͷţ������Ӧ�ı�־λ
				set_logout_cycle_table(0);//ѭ�����������¼���
				if (func_index != TWO_MENU_F8_E2_SET && ((return_Two_Menu_F6_E6()) ? func_index > ONE_MENU_Fd : 1))
				{
					GD5800_select_chapter_new(DI);
				}
				if (single_long_press_process_complice == 0)
				{
					sound_table = 1;
				}
				else
				{
					sound_table = 0;
				}
				return Val;
		}
	}
#ifdef TRANSMIT_KEY

	if (tm1629_read_key() !=0xff)//Press button
	{
		delay10ms();  //Remove jitter
		if (tm1629_read_key() !=0xff)   //Press button
		{
			Val = tm1629_read_key();
			clear_return_standby_time();
				if (Val == KEY_03)
				{
					clear_main_press_time();	//����˵������µ�ʱ��������
					set_main_press_time_table(1); //������Ӧ�ı�־λ����ʼ����ʱ��
					while (tm1629_read_key()==KEY_03 && !single_long_press_process_complice)
					{
						if ((return_Two_Menu_Fb_E1() == 0 && func_index == MENU_STANDBY) || (return_Two_Menu_Fb_E1() == 1 && (func_index == MENU_STANDBY || func_index == DECODER_MENU)))  //�������==0  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								func_index = ONE_MENU_F1;
								Val = 0;
								P55 = 1;
								P17 = 0;
								tm1629_f(0x01);
								if (return_Two_Menu_F6_E6() == 1)
								{
									GD5800_select_chapter(CUNCHUSHEZHI);
								}
								else
								{
									GD5800_select_chapter_new(DI);
								}
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E1_D1 || func_index == TWO_MENU_F2_E1_D2 || func_index == TWO_MENU_F2_E1_D3 || func_index == TWO_MENU_F2_E1_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_call_function(Two_Menu_F2_E1))
								{
									GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E2_D1 || func_index == TWO_MENU_F2_E2_D2 || func_index == TWO_MENU_F2_E2_D3 || func_index == TWO_MENU_F2_E2_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_host_function(Two_Menu_F2_E2))
								{
										GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E3_D1 || func_index == TWO_MENU_F2_E3_D2 || func_index == TWO_MENU_F2_E3_D3 || func_index == TWO_MENU_F2_E3_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_alarm_function(Two_Menu_F2_E3))
								{
									GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F2_E4_D1 || func_index == TWO_MENU_F2_E4_D2 || func_index == TWO_MENU_F2_E4_D3 || func_index == TWO_MENU_F2_E4_D4)
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 10)
							{
								Val = 0;
								if (!delete_cancel_function(Two_Menu_F2_E4))
								{
										GD5800_select_chapter(CHENGGONG);
								}
								delay10ms();
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == ONE_MENU_Fb)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_Fb_SET;
								temp = return_Two_Menu_Fb_E1();
								Show_one_number(temp);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == ONE_MENU_FC)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_FC_SET;
								temp = return_Two_Menu_FC_E1();
								Show_one_number(temp);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == ONE_MENU_Fd)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_Fd_SET;
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
#ifdef COPY_TWO_MACHINE
						else if (func_index == ONE_MENU_FE)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_FE_E1;
								tm1629_E(0x01);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
#endif
						else if (func_index == TWO_MENU_F8_E1)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								func_index = TWO_MENU_F8_E1_SET;
								temp = return_Two_Menu_F8_E1();
								Show_one_number(temp);
								single_long_press_process_complice = 1;
								clear_main_press_time();
							}
						}
						else if (func_index == TWO_MENU_F9_E2)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								GD5800_select_chapter(CHENGGONG);
								var_init();
								env_load();
								IAP_CONTR = 0X60;
								
								//tm1629_init();
								//env_init();
								func_index = MENU_STANDBY;
								single_long_press_process_complice = 1;
								clear_main_press_time();
								WDT_CONTR = 0x30;  /* 4 seconds to ι��*/
							}
						}
						else if (func_index == TWO_MENU_F9_E1)  //�������==FA  ��Ҫ1�����Ͻ���˵�
						{
							main_press_time_temp = return_main_press_time();
							if (main_press_time_temp >= 20)
							{
								Val = 0;
								GD5800_select_chapter(CHENGGONG);
								var_init();
								key_init();
								env_load();
								ISendStr(I2C_ADDRESS, SIN_KEY, single_key, 16);
								delay10ms();
								ISendStr(I2C_ADDRESS, MUL_KEY, multiple_key, 16);
								delay10ms();
								Delete_all_data();
								delay10ms();
								IAP_CONTR = 0X60;

								//tm1629_init();
								//env_init();
								func_index = MENU_STANDBY;
								single_long_press_process_complice = 1;
								clear_main_press_time();
								WDT_CONTR = 0x30;  /* 4 seconds to ι��*/
							}
						}

					}
				}
				while (tm1629_read_key() !=0xff);
				delay10ms();
				while (tm1629_read_key() !=0xff);
				set_main_press_time_table(0);//�����ͷţ������Ӧ�ı�־λ
				set_logout_cycle_table(0);//ѭ�����������¼���
				if (func_index != TWO_MENU_F8_E2_SET && ((return_Two_Menu_F6_E6()) ? func_index > ONE_MENU_Fd : 1))
				{
					GD5800_select_chapter_new(DI);
				}
				if (single_long_press_process_complice == 0)
				{
					sound_table = 1;
				}
				else
				{
					sound_table = 0;
				}
				return Val;
		}
	}

#endif

	
	return 0x0fff;
}

unsigned char KeyDecoder(void)
{
	unsigned int key_val = 0;
	unsigned char register_manager_temp = 0;
	key_val = KeyScan();
	register_manager_temp = return_register_manager();
	if (key_val == 0x0fff) /*do not key press*/
	{
		/*sjz ��ע���������־*/
		if (register_manager_temp == 1)
		{
			
			if (func_index == MENU_STANDBY || func_index == ONE_MENU_FA || func_index == ONE_MENU_Fb || func_index == ONE_MENU_FC || func_index == ONE_MENU_Fd || func_index == TWO_MENU_F9_E2 || func_index == TWO_MENU_F9_E1)
			{
//				uart_printf("accumulate_decoder %d .\r\n", (unsigned int)accumulate_decoder);
//				uart_printf("accumulate_decoder %d .\r\n", (unsigned int)old2_RF_RECE_REG[2] & 0x0f);
				if ((old2_RF_RECE_REG[2] & 0x0f) == 0x08)
				{
					accumulate_decoder++;
				}
				else
				{
					accumulate_decoder = 0;
				}
				if (accumulate_decoder>3)
				{
					set_main_press_time(20);
					accumulate_decoder = 0;
				}
			}
			clear_register_manager();
			if (return_filter_main() != 0)//ȥ�����ν���˵��� һֱ���ñ�־λ
			{
				sound_table = 0;
			}
			if ( (old2_RF_RECE_REG[2] & 0x0f) != 0x08)//��һ�ν���˵�����������Ĺ���,3S���ޱ�־,����Ҫ��ʱ�� ������־λ
			{
				sound_table = 1;
			}
			switch (old2_RF_RECE_REG[2] & 0x0f)
			{
			case 0x01:
				if (return_filter_other() == 0)
				{
					key_val = 0x003c;
					if (func_index == TWO_MENU_Fd_SET)//��FD�˵��˳�ʱ  ֹͣ����
					{
						GD5800_stop_music();
						delay10ms();
					}
					set_filter_other(5);
				}
				 break;
			case 0x02:
				if (return_filter_other() == 0)
				{
					key_val = 0x0034;
					set_filter_other(5);
				}
				 break;
			case 0x04:
				if (return_filter_other() == 0)
				{
					key_val = 0x002c;
					set_filter_other(5);
				}
				 break;
			case 0x08:
				if (return_filter_main() == 0 && return_filter_other() == 0)
				{
					key_val = 0x0038;
					set_filter_other(5);
				}
				break;
			default:break;
			}
		}
	}
 

#ifdef DEBUG
	if (key_val != 0x0fff)
	uart_printf("key_val is %x .\r\n", key_val); //���԰�����ֵ
#endif
	if(key_val >0 && key_val <=32)
	{
		return key_val;
	}
	else
	{
		switch (key_val)
		{
			case 0x003c:return KEY_RETURN; break;//1 ������Ӧ�ļ���ʾ���Ӧ����ֵ
			case 0x0034:return KEY_DOWN; break;//2  
			case 0x002c:return KEY_UP; break;//4
			case 0x0038:return KEY_FUNC; break;//5 ������Ӧ�ļ���ʾ���Ӧ����ֵ
			default:return 0xff; break;
		}
	}


}

void KeyProcess(void)
{
#ifdef TRANSMIT_KEY
	uint8_t Transmit_buf[3]={0};
#endif
	unsigned char key_value = 0, main_press_time_temp = 0;
	key_value = KeyDecoder();
	switch (key_value)
	{

		case KEY_FUNC:
			if ((return_Two_Menu_Fb_E1() == 0 && func_index == MENU_STANDBY) || (return_Two_Menu_Fb_E1() == 1&&( func_index == MENU_STANDBY || func_index == DECODER_MENU) ) )  //�������==0  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = ONE_MENU_F1;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
			else if (func_index == TWO_MENU_F2_E1_D1 || func_index == TWO_MENU_F2_E1_D2 || func_index == TWO_MENU_F2_E1_D3 || func_index == TWO_MENU_F2_E1_D4 )
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					if (!delete_call_function(Two_Menu_F2_E1))
					{
						if (return_sound_table() == 1)
						{
							GD5800_select_chapter(CHENGGONG);

							set_sound_table(0);
						}
					}
					delay10ms();
					clear_main_press_time();
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == TWO_MENU_F2_E2_D1 || func_index == TWO_MENU_F2_E2_D2 || func_index == TWO_MENU_F2_E2_D3 || func_index == TWO_MENU_F2_E2_D4)
			{
				main_press_time_temp = return_main_press_time();
				if ( main_press_time_temp >= 10)
				{
					if (!delete_host_function(Two_Menu_F2_E2))
					{
						if (return_sound_table() == 1)
						{
							GD5800_select_chapter(CHENGGONG);

							set_sound_table(0);
						}
					}
					delay10ms();
					clear_main_press_time();
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == TWO_MENU_F2_E3_D1 || func_index == TWO_MENU_F2_E3_D2 || func_index == TWO_MENU_F2_E3_D3 || func_index == TWO_MENU_F2_E3_D4)
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					if (!delete_alarm_function(Two_Menu_F2_E3))
					{
						if (return_sound_table() == 1)
						{
							GD5800_select_chapter(CHENGGONG);

							set_sound_table(0);
						}
					}
					delay10ms();
					clear_main_press_time();
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == TWO_MENU_F2_E4_D1 || func_index == TWO_MENU_F2_E4_D2 || func_index == TWO_MENU_F2_E4_D3 || func_index == TWO_MENU_F2_E4_D4)
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					if (!delete_cancel_function(Two_Menu_F2_E4))
					{
						if (return_sound_table() == 1)
						{
							GD5800_select_chapter(CHENGGONG);

							set_sound_table(0);
						}
					}
					delay10ms();
					clear_main_press_time();			
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == ONE_MENU_FA)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = TWO_MENU_FA_SET;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
			else if (func_index == ONE_MENU_Fb)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = TWO_MENU_Fb_SET;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
			else if (func_index == ONE_MENU_FC)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = TWO_MENU_FC_SET;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
			else if (func_index == ONE_MENU_Fd)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = TWO_MENU_Fd_SET;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
#ifdef COPY_TWO_MACHINE
			else if (func_index == ONE_MENU_FE)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = TWO_MENU_FE_E1;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
#endif
			else if (func_index == TWO_MENU_F8_E1)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = TWO_MENU_F8_E1_SET;
					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
				}
			}
			else if (func_index == TWO_MENU_F9_E2)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					GD5800_select_chapter(CHENGGONG);
					var_init();
					env_load();
					IAP_CONTR = 0X60;
					//tm1629_init();
					//env_init();

					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
					WDT_CONTR = 0x30;  /* 4 seconds to ι��*/
				}
			}
			else if (func_index == TWO_MENU_F9_E1)  //�������==FA  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					GD5800_select_chapter(CHENGGONG);
					var_init();
					key_init();
					env_load();
					ISendStr(I2C_ADDRESS, SIN_KEY, single_key, 16);
					delay10ms();
					ISendStr(I2C_ADDRESS, MUL_KEY, multiple_key, 16);
					delay10ms();
					Delete_all_data();
					delay10ms();
					IAP_CONTR = 0X60;
					//tm1629_init();
					//env_init();

					set_filter_main(6);//����˵��� 3S�� �������Ĳ˵�����Ч
					clear_main_press_time();
					WDT_CONTR = 0x30;  /* 4 seconds to ι��*/
				}
			}
			else
			{
				func_index = table[func_index].enter;
				clear_main_press_time();
			}
			 break;
		case KEY_RETURN:	
			if (func_index == DECODER_MENU  && return_Two_Menu_Fb_E1() )
			{
				Logout();
			}
			else
			func_index = table[func_index].esc; 
			break;
		case KEY_UP:
			switch (func_index)
			{
			case TWO_MENU_F0_YEAR: 
				if (time_buf1[1] == 99) time_buf1[1] = 0;	//���������� ��
				else time_buf1[1]++;
				break;
			case TWO_MENU_F0_MOUTH:
				if (time_buf1[2] == 12) time_buf1[2] = 1;	//���������� ��
				else time_buf1[2]++;
				break;
			case TWO_MENU_F0_DAY:
				if (time_buf1[3] == 31) time_buf1[3] = 1;	//���������� ��
				else time_buf1[3]++;
				break;
			case TWO_MENU_F0_WEEK:
				if (time_buf1[7] == 7) time_buf1[7] = 1;	//���������� ����
				else time_buf1[7]++;
				break;
			case TWO_MENU_F0_HOUR:
				if (time_buf1[4] == 23) time_buf1[4] = 0;	//���������� Сʱ
				else time_buf1[4]++;
				break;
			case TWO_MENU_F0_MINUTE:
				if (time_buf1[5] == 59) time_buf1[5] = 0;	//���������� ����
				else time_buf1[5]++;
				break;
			case TWO_MENU_F1_E1_D1:
				if (Two_Menu_F1_E1[0] == 20 ) Two_Menu_F1_E1[0] = 0;	//����F1_E1ǧλ
				else Two_Menu_F1_E1[0]++;
				break;
			case TWO_MENU_F1_E1_D2:
				if (Two_Menu_F1_E1[1] == 9) Two_Menu_F1_E1[1] = 0;	//����F1_E1��λ
				else Two_Menu_F1_E1[1]++;
				break;
			case TWO_MENU_F1_E1_D3:
				if (Two_Menu_F1_E1[2] == 9) Two_Menu_F1_E1[2] = 0;	//����F1_E1ʮλ
				else Two_Menu_F1_E1[2]++;
				break;
			case TWO_MENU_F1_E1_D4:
				if (Two_Menu_F1_E1[3] == 9) Two_Menu_F1_E1[3] = 0;	//����F1_E1��λ
				else Two_Menu_F1_E1[3]++;
				break;
			case TWO_MENU_F1_E2_D1:
				if (Two_Menu_F1_E2[0] == 20) Two_Menu_F1_E2[0] = 0;	//����F1_E2ǧλ
				else Two_Menu_F1_E2[0]++;
				break;
			case TWO_MENU_F1_E2_D2:
				if (Two_Menu_F1_E2[1] == 9) Two_Menu_F1_E2[1] = 0;	//����F1_E2��λ
				else Two_Menu_F1_E2[1]++;
				break;
			case TWO_MENU_F1_E2_D3:
				if (Two_Menu_F1_E2[2] == 9) Two_Menu_F1_E2[2] = 0;	//����F1_E2ʮλ
				else Two_Menu_F1_E2[2]++;
				break;
			case TWO_MENU_F1_E2_D4:
				if (Two_Menu_F1_E2[3] == 9) Two_Menu_F1_E2[3] = 0;	//����F1_E2��λ
				else Two_Menu_F1_E2[3]++;
				break;
			case TWO_MENU_F1_E3_D1:
				if (Two_Menu_F1_E3[0] == 20) Two_Menu_F1_E3[0] = 0;	//����F1_E3ǧλ
				else Two_Menu_F1_E3[0]++;
				break;
			case TWO_MENU_F1_E3_D2:
				if (Two_Menu_F1_E3[1] == 9) Two_Menu_F1_E3[1] = 0;	//����F1_E3��λ
				else Two_Menu_F1_E3[1]++;
				break;
			case TWO_MENU_F1_E3_D3:
				if (Two_Menu_F1_E3[2] == 9) Two_Menu_F1_E3[2] = 0;	//����F1_E3ʮλ
				else Two_Menu_F1_E3[2]++;
				break;
			case TWO_MENU_F1_E3_D4:
				if (Two_Menu_F1_E3[3] == 9) Two_Menu_F1_E3[3] = 0;	//����F1_E3��λ
				else Two_Menu_F1_E3[3]++;
				break;
			case TWO_MENU_F1_E4_D1:
				if (Two_Menu_F1_E4[0] == 20) Two_Menu_F1_E4[0] = 0;	//����F1_E4ǧλ
				else Two_Menu_F1_E4[0]++;
				break;
			case TWO_MENU_F1_E4_D2:
				if (Two_Menu_F1_E4[1] == 9) Two_Menu_F1_E4[1] = 0;	//����F1_E4��λ
				else Two_Menu_F1_E4[1]++;
				break;
			case TWO_MENU_F1_E4_D3:
				if (Two_Menu_F1_E4[2] == 9) Two_Menu_F1_E4[2] = 0;	//����F1_E4ʮλ
				else Two_Menu_F1_E4[2]++;
				break;
			case TWO_MENU_F1_E4_D4:
				if (Two_Menu_F1_E4[3] == 9) Two_Menu_F1_E4[3] = 0;	//����F1_E4��λ
				else Two_Menu_F1_E4[3]++;
				break;
			case TWO_MENU_F2_E1_D1:
				if (Two_Menu_F2_E1[0] == 20) Two_Menu_F2_E1[0] = 0;	//����F2_E1ǧλ
				else Two_Menu_F2_E1[0]++;
				break;
			case TWO_MENU_F2_E1_D2:
				if (Two_Menu_F2_E1[1] == 9) Two_Menu_F2_E1[1] = 0;	//����F2_E1��λ
				else Two_Menu_F2_E1[1]++;
				break;
			case TWO_MENU_F2_E1_D3:
				if (Two_Menu_F2_E1[2] == 9) Two_Menu_F2_E1[2] = 0;	//����F2_E1ʮλ
				else Two_Menu_F2_E1[2]++;
				break;
			case TWO_MENU_F2_E1_D4:
				if (Two_Menu_F2_E1[3] == 9) Two_Menu_F2_E1[3] = 0;	//����F2_E1��λ
				else Two_Menu_F2_E1[3]++;
				break;
			case TWO_MENU_F2_E2_D1:
				if (Two_Menu_F2_E2[0] == 20) Two_Menu_F2_E2[0] = 0;	//����F2_E2ǧλ
				else Two_Menu_F2_E2[0]++;
				break;
			case TWO_MENU_F2_E2_D2:
				if (Two_Menu_F2_E2[1] == 9) Two_Menu_F2_E2[1] = 0;	//����F2_E2��λ
				else Two_Menu_F2_E2[1]++;
				break;
			case TWO_MENU_F2_E2_D3:
				if (Two_Menu_F2_E2[2] == 9) Two_Menu_F2_E2[2] = 0;	//����F2_E2ʮλ
				else Two_Menu_F2_E2[2]++;
				break;
			case TWO_MENU_F2_E2_D4:
				if (Two_Menu_F2_E2[3] == 9) Two_Menu_F2_E2[3] = 0;	//����F2_E2��λ
				else Two_Menu_F2_E2[3]++;
				break;
			case TWO_MENU_F2_E3_D1:
				if (Two_Menu_F2_E3[0] == 20) Two_Menu_F2_E3[0] = 0;	//����F2_E3ǧλ
				else Two_Menu_F2_E3[0]++;
				break;
			case TWO_MENU_F2_E3_D2:
				if (Two_Menu_F2_E3[1] == 9) Two_Menu_F2_E3[1] = 0;	//����F2_E3��λ
				else Two_Menu_F2_E3[1]++;
				break;
			case TWO_MENU_F2_E3_D3:
				if (Two_Menu_F2_E3[2] == 9) Two_Menu_F2_E3[2] = 0;	//����F2_E3ʮλ
				else Two_Menu_F2_E3[2]++;
				break;
			case TWO_MENU_F2_E3_D4:
				if (Two_Menu_F2_E3[3] == 9) Two_Menu_F2_E3[3] = 0;	//����F2_E3��λ
				else Two_Menu_F2_E3[3]++;
				break;
			case TWO_MENU_F2_E4_D1:
				if (Two_Menu_F2_E4[0] == 20) Two_Menu_F2_E4[0] = 0;	//����F2_E4ǧλ
				else Two_Menu_F2_E4[0]++;
				break;
			case TWO_MENU_F2_E4_D2:
				if (Two_Menu_F2_E4[1] == 9) Two_Menu_F2_E4[1] = 0;	//����F2_E4��λ
				else Two_Menu_F2_E4[1]++;
				break;
			case TWO_MENU_F2_E4_D3:
				if (Two_Menu_F2_E4[2] == 9) Two_Menu_F2_E4[2] = 0;	//����F2_E4ʮλ
				else Two_Menu_F2_E4[2]++;
				break;
			case TWO_MENU_F2_E4_D4:
				if (Two_Menu_F2_E4[3] == 9) Two_Menu_F2_E4[3] = 0;	//����F2_E4��λ
				else Two_Menu_F2_E4[3]++;
			case TWO_MENU_F3_E1_SET:
				if (Two_Menu_F3_E1 == 1) Two_Menu_F3_E1 = 2;	//�Ŷ���ʾ ���� ѭ����ʾ
				else Two_Menu_F3_E1 = 1;
				env_load();
				break;
			case TWO_MENU_F3_E2_SET:
				if (Two_Menu_F3_E2 == 99) Two_Menu_F3_E2 = 1;	//�洢���и���
				else Two_Menu_F3_E2 ++;
				env_load();
				break;
			case TWO_MENU_F4_SET:
				if (Two_Menu_F4_E1 == 99) Two_Menu_F4_E1 = 0;	//����ʱ������
				else Two_Menu_F4_E1++;
				env_load();
				break;
			case TWO_MENU_F5_SET:
				if (Two_Menu_F5_E1 == 99) Two_Menu_F5_E1 = 0;	//ѭ��ʱ������
				else Two_Menu_F5_E1++;
				env_load();
				break;
			case TWO_MENU_F6_E1_SET:
				if (Two_Menu_F6_E1 == 7) Two_Menu_F6_E1 = 0;	//F6_E1����ѡ��
				else Two_Menu_F6_E1++;
				env_load();
				break;
			case TWO_MENU_F6_E2_SET:
#ifndef AWAIT_VOICE_PATCH
#ifdef REPLAY_PATCH
//1111111111111111111
				if (Two_Menu_F6_E2 == 9) Two_Menu_F6_E2 = 1;	//F6_E2������������
				else Two_Menu_F6_E2++;
#else
				if (Two_Menu_F6_E2 == 2) Two_Menu_F6_E2 = 1;	//F6_E2������������
				else Two_Menu_F6_E2++;
#endif
#endif

#ifdef AWAIT_VOICE_PATCH
				if (Two_Menu_F6_E2 == 1) Two_Menu_F6_E2 = 9;	//F6_E2����������������
				else Two_Menu_F6_E2--;
#endif
				env_load();
				break;
			case TWO_MENU_F6_E3_SET:
				if (Two_Menu_F6_E3 == 0) Two_Menu_F6_E3 = 1;	//F6_E3ѭ��ʱ���Ƿ񱨶�
				else Two_Menu_F6_E3 = 0;
				env_load();
				break;
			case TWO_MENU_F6_E4_SET:
				if (Two_Menu_F6_E4 == 9) Two_Menu_F6_E4 = 0;	//F6_E4������С����
				else Two_Menu_F6_E4++;
				env_load();
				break;
			case TWO_MENU_F6_E5_SET:
				if (Two_Menu_F6_E5 == 7) Two_Menu_F6_E5 = 0;	//F6_E5��Ļ���ȵ���
				else Two_Menu_F6_E5++;
				env_load();
				break;
			case TWO_MENU_F7_E1_SET:
#ifndef DECODER_DOUBLE_CHECK_PATCH
				if (Two_Menu_F7_E1 == 10) Two_Menu_F7_E1 = 0;	//F7_E1ԭʼ���� 999*9
				else Two_Menu_F7_E1++;

#endif

#ifdef DECODER_DOUBLE_CHECK_PATCH
				if (Two_Menu_F7_E1 == 11) Two_Menu_F7_E1 = 0;	//F7_E1ԭʼ���� 999*9
				else Two_Menu_F7_E1++;

#endif
				env_load();
				break;
			case TWO_MENU_F7_E2_SET:
				if (Two_Menu_F7_E2 == 11) Two_Menu_F7_E2 = 0;	//F7_E2���ӹ��� 9999*9
				else Two_Menu_F7_E2++;
				break;
			case TWO_MENU_F7_E3_SET:
				if (Two_Menu_F7_E3 == 11) Two_Menu_F7_E3 = 0;	//F7_E3���ӹ��� 999*99
				else Two_Menu_F7_E3++;
				break;
			case TWO_MENU_F7_E4_SET:
				if (Two_Menu_F7_E4 == 11) Two_Menu_F7_E4 = 0;	//F7_E3���ӹ��� 9999*99
				else Two_Menu_F7_E4++;
				break;
			case TWO_MENU_F8_E1_SET:
				if (Two_Menu_F8_E1 == 1) Two_Menu_F8_E1 = 2;	//F8_E1����λ�����λ�л�
				else Two_Menu_F8_E1 = 1 ;
				env_load();
				break;
			case TWO_MENU_F8_E2_SET:
				if (Two_Menu_F8_E2 == 38) Two_Menu_F8_E2 = 0;	//F8_E2��λ����
				else Two_Menu_F8_E2++;
				break;
			case DECODER_MENU:
				CycleUp();
				break;
			case TWO_MENU_FC_SET:
				if (Two_Menu_FC_E1 == 1) Two_Menu_FC_E1 = 2;
				else Two_Menu_FC_E1 = 1;
				env_load();
				break;
			case TWO_MENU_Fb_SET:
				if (Two_Menu_Fb_E1 == 1) Two_Menu_Fb_E1 = 0;
				else Two_Menu_Fb_E1 = 1;
				env_load();
				break;
			case TWO_MENU_F6_E6_SET:
				if (Two_Menu_F6_E6 == 1) Two_Menu_F6_E6 = 0;	
				else Two_Menu_F6_E6 = 1;
				env_load();
				break;
			case TWO_MENU_F6_E7_SET:
				if (Two_Menu_F6_E7 == 1) Two_Menu_F6_E7 = 0;
				else Two_Menu_F6_E7 = 1;
				env_load();
				break;
			case TWO_MENU_F6_E8_SET:
				if (Two_Menu_F6_E8 == 1) Two_Menu_F6_E8 = 0;
				else Two_Menu_F6_E8 = 1;
				env_load();
				break;

				default:break;
			}	
			func_index = table[func_index].up; break;
		case KEY_DOWN:
			switch (func_index)
			{
			case TWO_MENU_F0_YEAR:
				if (time_buf1[1] == 0) time_buf1[1] = 99;	//���������� ��
				else time_buf1[1]--;
				break;
			case TWO_MENU_F0_MOUTH:
				if (time_buf1[2] == 1) time_buf1[2] = 12;	//���������� ��
				else time_buf1[2]--;
				break;
			case TWO_MENU_F0_DAY:
				if (time_buf1[3] == 1) time_buf1[3] = 31;	//���������� ��
				else time_buf1[3]--;
				break;
			case TWO_MENU_F0_WEEK:
				if (time_buf1[7] == 1) time_buf1[7] = 7;	//���������� ����
				else time_buf1[7]--;
				break;
			case TWO_MENU_F0_HOUR:
				if (time_buf1[4] == 0) time_buf1[4] = 23;	//���������� Сʱ
				else time_buf1[4]--;
				break;
			case TWO_MENU_F0_MINUTE:
				if (time_buf1[5] == 0) time_buf1[5] = 59;	//���������� ����
				else time_buf1[5]--;
				break;
			case TWO_MENU_F1_E1_D1:
				if (Two_Menu_F1_E1[0] == 0) Two_Menu_F1_E1[0] = 20;	//����F1_E1ǧλ
				else Two_Menu_F1_E1[0]--;
				break;
			case TWO_MENU_F1_E1_D2:
				if (Two_Menu_F1_E1[1] == 0) Two_Menu_F1_E1[1] = 9;	//����F1_E1��λ
				else Two_Menu_F1_E1[1]--;
				break;
			case TWO_MENU_F1_E1_D3:
				if (Two_Menu_F1_E1[2] == 0) Two_Menu_F1_E1[2] = 9;	//����F1_E1ʮλ
				else Two_Menu_F1_E1[2]--;
				break;
			case TWO_MENU_F1_E1_D4:
				if (Two_Menu_F1_E1[3] == 0) Two_Menu_F1_E1[3] = 9;	//����F1_E1��λ
				else Two_Menu_F1_E1[3]--;
				break;
			case TWO_MENU_F1_E2_D1:
				if (Two_Menu_F1_E2[0] == 0) Two_Menu_F1_E2[0] = 20;	//����F1_E2ǧλ
				else Two_Menu_F1_E2[0]--;
				break;
			case TWO_MENU_F1_E2_D2:
				if (Two_Menu_F1_E2[1] == 0) Two_Menu_F1_E2[1] = 9;	//����F1_E2��λ
				else Two_Menu_F1_E2[1]--;
				break;
			case TWO_MENU_F1_E2_D3:
				if (Two_Menu_F1_E2[2] == 0) Two_Menu_F1_E2[2] = 9;	//����F1_E2ʮλ
				else Two_Menu_F1_E2[2]--;
				break;
			case TWO_MENU_F1_E2_D4:
				if (Two_Menu_F1_E2[3] == 0) Two_Menu_F1_E2[3] = 9;	//����F1_E2��λ
				else Two_Menu_F1_E2[3]--;
				break;
			case TWO_MENU_F1_E3_D1:
				if (Two_Menu_F1_E3[0] == 0) Two_Menu_F1_E3[0] = 20;	//����F1_E3ǧλ
				else Two_Menu_F1_E3[0]--;
				break;
			case TWO_MENU_F1_E3_D2:
				if (Two_Menu_F1_E3[1] == 0) Two_Menu_F1_E3[1] = 9;	//����F1_E3��λ
				else Two_Menu_F1_E3[1]--;
				break;
			case TWO_MENU_F1_E3_D3:
				if (Two_Menu_F1_E3[2] == 0) Two_Menu_F1_E3[2] = 9;	//����F1_E3ʮλ
				else Two_Menu_F1_E3[2]--;
				break;
			case TWO_MENU_F1_E3_D4:
				if (Two_Menu_F1_E3[3] == 0) Two_Menu_F1_E3[3] = 9;	//����F1_E3��λ
				else Two_Menu_F1_E3[3]--;
				break;
			case TWO_MENU_F1_E4_D1:
				if (Two_Menu_F1_E4[0] == 0) Two_Menu_F1_E4[0] = 20;	//����F1_E4ǧλ
				else Two_Menu_F1_E4[0]--;
				break;
			case TWO_MENU_F1_E4_D2:
				if (Two_Menu_F1_E4[1] == 0) Two_Menu_F1_E4[1] = 9;	//����F1_E4��λ
				else Two_Menu_F1_E4[1]--;
				break;
			case TWO_MENU_F1_E4_D3:
				if (Two_Menu_F1_E4[2] == 0) Two_Menu_F1_E4[2] = 9;	//����F1_E4ʮλ
				else Two_Menu_F1_E4[2]--;
				break;
			case TWO_MENU_F1_E4_D4:
				if (Two_Menu_F1_E4[3] == 0) Two_Menu_F1_E4[3] = 9;	//����F1_E4��λ
				else Two_Menu_F1_E4[3]--;
				break;
			case TWO_MENU_F2_E1_D1:
				if (Two_Menu_F2_E1[0] == 0) Two_Menu_F2_E1[0] = 20;	//����F2_E1ǧλ
				else Two_Menu_F2_E1[0]--;
				break;
			case TWO_MENU_F2_E1_D2:
				if (Two_Menu_F2_E1[1] == 0) Two_Menu_F2_E1[1] = 9;	//����F2_E1��λ
				else Two_Menu_F2_E1[1]--;
				break;
			case TWO_MENU_F2_E1_D3:
				if (Two_Menu_F2_E1[2] == 0) Two_Menu_F2_E1[2] = 9;	//����F2_E1ʮλ
				else Two_Menu_F2_E1[2]--;
				break;
			case TWO_MENU_F2_E1_D4:
				if (Two_Menu_F2_E1[3] == 0) Two_Menu_F2_E1[3] = 9;	//����F2_E1��λ
				else Two_Menu_F2_E1[3]--;
				break;
			case TWO_MENU_F2_E2_D1:
				if (Two_Menu_F2_E2[0] == 0) Two_Menu_F2_E2[0] = 20;	//����F2_E2ǧλ
				else Two_Menu_F2_E2[0]--;
				break;
			case TWO_MENU_F2_E2_D2:
				if (Two_Menu_F2_E2[1] == 0) Two_Menu_F2_E2[1] = 9;	//����F2_E2��λ
				else Two_Menu_F2_E2[1]--;
				break;
			case TWO_MENU_F2_E2_D3:
				if (Two_Menu_F2_E2[2] == 0) Two_Menu_F2_E2[2] = 9;	//����F2_E2ʮλ
				else Two_Menu_F2_E2[2]--;
				break;
			case TWO_MENU_F2_E2_D4:
				if (Two_Menu_F2_E2[3] == 0) Two_Menu_F2_E2[3] = 9;	//����F2_E2��λ
				else Two_Menu_F2_E2[3]--;
				break;
			case TWO_MENU_F2_E3_D1:
				if (Two_Menu_F2_E3[0] == 0) Two_Menu_F2_E3[0] = 20;	//����F2_E3ǧλ
				else Two_Menu_F2_E3[0]--;
				break;
			case TWO_MENU_F2_E3_D2:
				if (Two_Menu_F2_E3[1] == 0) Two_Menu_F2_E3[1] = 9;	//����F2_E3��λ
				else Two_Menu_F2_E3[1]--;
				break;
			case TWO_MENU_F2_E3_D3:
				if (Two_Menu_F2_E3[2] == 0) Two_Menu_F2_E3[2] = 9;	//����F2_E3ʮλ
				else Two_Menu_F2_E3[2]--;
				break;
			case TWO_MENU_F2_E3_D4:
				if (Two_Menu_F2_E3[3] == 0) Two_Menu_F2_E3[3] = 9;	//����F2_E3��λ
				else Two_Menu_F2_E3[3]--;
				break;
			case TWO_MENU_F2_E4_D1:
				if (Two_Menu_F2_E4[0] == 0) Two_Menu_F2_E4[0] = 20;	//����F2_E4ǧλ
				else Two_Menu_F2_E4[0]--;
				break;
			case TWO_MENU_F2_E4_D2:
				if (Two_Menu_F2_E4[1] == 0) Two_Menu_F2_E4[1] = 9;	//����F2_E4��λ
				else Two_Menu_F2_E4[1]--;
				break;
			case TWO_MENU_F2_E4_D3:
				if (Two_Menu_F2_E4[2] == 0) Two_Menu_F2_E4[2] = 9;	//����F2_E4ʮλ
				else Two_Menu_F2_E4[2]--;
				break;
			case TWO_MENU_F2_E4_D4:
				if (Two_Menu_F2_E4[3] == 0) Two_Menu_F2_E4[3] = 9;	//����F2_E4��λ
				else Two_Menu_F2_E4[3]--;
				break;
			case TWO_MENU_F3_E1_SET:
				if (Two_Menu_F3_E1 == 1) Two_Menu_F3_E1 = 2;	//�Ŷ���ʾ ���� ѭ����ʾ
				else Two_Menu_F3_E1 = 1;
				env_load();
				break;
			case TWO_MENU_F3_E2_SET:
				if (Two_Menu_F3_E2 == 1) Two_Menu_F3_E2 = 99;	//�洢���и���
				else Two_Menu_F3_E2--;
				env_load();
				break;
			case TWO_MENU_F4_SET:
				if (Two_Menu_F4_E1 == 0) Two_Menu_F4_E1 = 99;	//����ʱ������
				else Two_Menu_F4_E1--;
				env_load();
				break;
			case TWO_MENU_F5_SET:
				if (Two_Menu_F5_E1 == 0) Two_Menu_F5_E1 = 99;	//����ʱ������
				else Two_Menu_F5_E1--;
				env_load();
				break;
			case TWO_MENU_F6_E1_SET:
				if (Two_Menu_F6_E1 == 0) Two_Menu_F6_E1 = 7;	//F6_E1������ѡ��
				else Two_Menu_F6_E1--;
				env_load();
				break;
			case TWO_MENU_F6_E2_SET:
#ifndef AWAIT_VOICE_PATCH				
#ifdef REPLAY_PATCH
				if (Two_Menu_F6_E2 == 1) Two_Menu_F6_E2 = 9;	//F6_E2����������������
				else Two_Menu_F6_E2--;
#else
				if (Two_Menu_F6_E2 == 1) Two_Menu_F6_E2 = 2;	//F6_E2����������������
				else Two_Menu_F6_E2--;
#endif
#endif

#ifdef AWAIT_VOICE_PATCH
				if (Two_Menu_F6_E2 == 1) Two_Menu_F6_E2 = 9;	//F6_E2����������������
				else Two_Menu_F6_E2--;
#endif
				env_load();
				break;
			case TWO_MENU_F6_E3_SET:
				if (Two_Menu_F6_E3 == 0) Two_Menu_F6_E3 = 1;	//F6_E3ѭ��ʱ���Ƿ񱨶�
				else Two_Menu_F6_E3 = 0;
				env_load();
				break;
			case TWO_MENU_F6_E4_SET:
				if (Two_Menu_F6_E4 == 0) Two_Menu_F6_E4 = 9;	//F6_E4������С����
				else Two_Menu_F6_E4--;
				env_load();
				break;
			case TWO_MENU_F6_E5_SET:
				if (Two_Menu_F6_E5 == 0) Two_Menu_F6_E5 = 7;	//F6_E5��Ļ���ȵ���
				else Two_Menu_F6_E5--;
				env_load();
				break;
			case TWO_MENU_F7_E1_SET:
#ifndef DECODER_DOUBLE_CHECK_PATCH				
				if (Two_Menu_F7_E1 == 0) Two_Menu_F7_E1 = 10;	//F7_E1ԭʼ���� 999*9
				else Two_Menu_F7_E1--;
#endif

#ifdef  DECODER_DOUBLE_CHECK_PATCH
				if (Two_Menu_F7_E1 == 0) Two_Menu_F7_E1 = 11;	//F7_E1ԭʼ���� 999*9
				else Two_Menu_F7_E1--;
#endif
				env_load();
				break;
			case TWO_MENU_F7_E2_SET:
				if (Two_Menu_F7_E2 == 0) Two_Menu_F7_E2 = 11;	//F7_E2���ӹ��� 9999*9
				else Two_Menu_F7_E2--;
				break;
			case TWO_MENU_F7_E3_SET:
				if (Two_Menu_F7_E3 == 0) Two_Menu_F7_E3 = 11;	//F7_E3���ӹ��� 999*99
				else Two_Menu_F7_E3--;
				break;
			case TWO_MENU_F7_E4_SET:
				if (Two_Menu_F7_E4 == 0) Two_Menu_F7_E4 = 11;	//F7_E3���ӹ��� 9999*99
				else Two_Menu_F7_E4--;
				break;
			case TWO_MENU_F8_E1_SET:
				if (Two_Menu_F8_E1 == 1) Two_Menu_F8_E1 = 2;	//F8_E1����λ�����λ�л�
				else Two_Menu_F8_E1 = 1;
				env_load();
				break;
			case TWO_MENU_F8_E2_SET:
				if (Two_Menu_F8_E2 == 0) Two_Menu_F8_E2 = 38;	//F8_E2��λ����
				else Two_Menu_F8_E2--;
				break;
			case DECODER_MENU:
				CycleDown();
				break;
			case TWO_MENU_FC_SET:
				if (Two_Menu_FC_E1 == 1) Two_Menu_FC_E1 = 2;
				else Two_Menu_FC_E1 = 1;
				env_load();
				break;
			case TWO_MENU_Fb_SET:
				if (Two_Menu_Fb_E1 == 1) Two_Menu_Fb_E1 = 0;
				else Two_Menu_Fb_E1 = 1;
				env_load();
				break;
			case TWO_MENU_F6_E6_SET:
				if (Two_Menu_F6_E6 == 1) Two_Menu_F6_E6 = 0;
				else Two_Menu_F6_E6 = 1;
				env_load();
				break;
			case TWO_MENU_F6_E7_SET:
				if (Two_Menu_F6_E7 == 1) Two_Menu_F6_E7 = 0;
				else Two_Menu_F6_E7 = 1;
				env_load();
				break;
			case TWO_MENU_F6_E8_SET:
				if (Two_Menu_F6_E8 == 1) Two_Menu_F6_E8 = 0;
				else Two_Menu_F6_E8 = 1;
				env_load();
				break;

				default:break;
			}
			func_index = table[func_index].down; break;
#ifdef TRANSMIT_KEY
		case KEY_01:break;
		case KEY_02:break;
		//case KEY_03:break;
		case KEY_04:
			Transmit_set_data(0);
			break;// 0
		case KEY_05:
			Transmit_set_data(7);
			break; // 7
		case KEY_06:
			Transmit_set_data(4);
			break;// 4
		case KEY_07:	
			Transmit_set_data(1);
			break;//  1
		case KEY_08:
			if(Transmit_Data < 999)
			{
				Transmit_Data++;
			}
			else
			{
				Transmit_Data = 999;
			}
			tm1629_load();
			display();			
			break;//����1  ����+1
		case KEY_09:break;
		case KEY_10:break;
		//case KEY_11:break;
		case KEY_12:
			Transmit_Data = 0;
			tm1629_load();
			display();
			break;//clear
		case KEY_13:
			Transmit_set_data(8);
			break;//8
		case KEY_14:
			Transmit_set_data(5);
			break;//5
		case KEY_15:
			Transmit_set_data(2);
			break;// 2
		case KEY_16:			
			if(Transmit_Data>0)
			{
				Transmit_Data--;
			}
			else
			{
				Transmit_Data = 0;
			}
			tm1629_load();
			display();
			break;//����2 ����-1
		case KEY_17:break;
		case KEY_18:break;
		//case KEY_19:break;
		case KEY_20:break;
		case KEY_21:
			Transmit_set_data(9);
			break;//9
		case KEY_22:
			Transmit_set_data(6);
			break;//6
		case KEY_23:
			Transmit_set_data(3);
			break;// 3
		case KEY_24:
			Transmit_buf[0]= ((Transmit_Data_FANGQU)<<4)|((Transmit_Data%1000)/100);
			Transmit_buf[1]= (((Transmit_Data%100)/10)<<4)|(Transmit_Data%10);	
			Transmit_buf[2]= 0x08;
			EX0 = 0;
			TR1 = 0;
			RF_ini_receive();
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			EX0 = 1;
			transmit_confirm_key_press= 1;
			tm1629_load();
			display();
			break;//����3 ����ȷ��
		case KEY_25:break;
		case KEY_26:break;
		//case KEY_27:break;
		case KEY_28:break;//F4
		case KEY_29:
			printer58mm_print();
			Line_number++;
			tm1629_load();
			display();
			break;//F3//��ӡ
		case KEY_30:
			Transmit_Data = *(display_ram+2)*100+*(display_ram+3)*10+*(display_ram+4)-1;
			Transmit_buf[0]= ((Transmit_Data_FANGQU)<<4)|((Transmit_Data%1000)/100);
			Transmit_buf[1]= (((Transmit_Data%100)/10)<<4)|(Transmit_Data%10);	
			Transmit_buf[2]= 0x08;
			EX0 = 0;
			TR1 = 0;
			RF_ini_receive();
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			EX0 = 1;
			transmit_confirm_key_press= 1;
			tm1629_load();
			display();
			break;//F2//����-1
		case KEY_31:
			Transmit_Data = *(display_ram+2)*100+*(display_ram+3)*10+*(display_ram+4)+1;
			Transmit_buf[0]= ((Transmit_Data_FANGQU)<<4)|((Transmit_Data%1000)/100);
			Transmit_buf[1]= (((Transmit_Data%100)/10)<<4)|(Transmit_Data%10);	
			Transmit_buf[2]= 0x08;
			EX0 = 0;
			TR1 = 0;
			RF_ini_receive();
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			SendIRdata(Transmit_buf[0],Transmit_buf[1],Transmit_buf[2]);
			EX0 = 1;
			transmit_confirm_key_press= 1;		
			tm1629_load();
			display();
			break;//F1 //����+1
		case KEY_32:
			Transmit_Data_set_FANGQU = 1;
			tm1629_load();
			display();
			break;//����4			
#endif
		default:break;
	}

	current_operation_index = table[func_index].index_operation;
	(*current_operation_index)();//ִ�е�ǰ��������
}

void delay10ms(void)   //��� -0.054253472222us
{
	unsigned char a, b, c;
	for (c = 4; c>0; c--)
	for (b = 52; b>0; b--)
	for (a = 220; a>0; a--);
	_nop_();  //if Keil,require use intrins.h
}
void delaymulti10ms(unsigned int delay)   //��� -0.054253472222us
{
	unsigned char a;
	for (a = 0; a<delay; a++)
		delay10ms();
}
unsigned char return_func_index(void)
{
	unsigned char func_index_temp = 0;
	func_index_temp = func_index;
	return func_index_temp;
}

void set_func_index(unsigned char temp)
{
	func_index = temp;
}

unsigned char return_Two_Menu_F3_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F3_E1;
	return temp;
}

unsigned char return_Two_Menu_F3_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F3_E2;
	return temp;
}

unsigned char return_Two_Menu_F4_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F4_E1;
	return temp;
}

unsigned char return_Two_Menu_F5_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F5_E1;
	return temp;
}

unsigned char return_Two_Menu_F6_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E1;
	return temp;
}

unsigned char return_Two_Menu_F6_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E2;
	return temp;
}

unsigned char return_Two_Menu_F6_E3(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E3;
	return temp;
}

unsigned char return_Two_Menu_F6_E4(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E4;
	return temp;
}

unsigned char return_Two_Menu_F6_E5(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E5;
	return temp;
}
unsigned char return_Two_Menu_F6_E6(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E6;
	return temp;
}

unsigned char return_Two_Menu_F6_E7(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E7;
	return temp;
}

unsigned char return_Two_Menu_F6_E8(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E8;
	return temp;
}

unsigned char return_Two_Menu_F7_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E1;
	return temp;
}

unsigned char return_Two_Menu_F7_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E2;
	return temp;
}

unsigned char return_Two_Menu_F7_E3(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E3;
	return temp;
}

unsigned char return_Two_Menu_F7_E4(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E4;
	return temp;
}

unsigned char return_Two_Menu_F8_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F8_E1;
	return temp;
}

unsigned char return_Two_Menu_F8_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F8_E2;
	return temp;
}

unsigned char return_sound_table(void)
{
	unsigned char temp = 0;
	temp = sound_table;
	return temp;
}

void set_sound_table(unsigned char temp)
{
	sound_table = temp;
}

unsigned char return_Two_Menu_FC_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_FC_E1;
	return temp;
}

unsigned char return_Two_Menu_Fb_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_Fb_E1;
	return temp;
}

void env_load(void)
{
	
	EEPROM.Two_Menu_F3_E1 = Two_Menu_F3_E1;
	EEPROM.Two_Menu_F3_E2 = Two_Menu_F3_E2;
	EEPROM.Two_Menu_F4_E1 = Two_Menu_F4_E1;
	EEPROM.Two_Menu_F5_E1 = Two_Menu_F5_E1;
	EEPROM.Two_Menu_F6_E1 = Two_Menu_F6_E1;
	EEPROM.Two_Menu_F6_E2 = Two_Menu_F6_E2;
	EEPROM.Two_Menu_F6_E3 = Two_Menu_F6_E3;
	EEPROM.Two_Menu_F6_E4 = Two_Menu_F6_E4;
	EEPROM.Two_Menu_F6_E5 = Two_Menu_F6_E5;
	EEPROM.Two_Menu_F6_E6 = Two_Menu_F6_E6;
	EEPROM.Two_Menu_F6_E7 = Two_Menu_F6_E7;
	EEPROM.Two_Menu_F6_E8 = Two_Menu_F6_E8;
	EEPROM.Two_Menu_F7_E1 = Two_Menu_F7_E1;
	EEPROM.Two_Menu_F8_E1 = Two_Menu_F8_E1;
	EEPROM.Two_Menu_Fb_E1 = Two_Menu_Fb_E1;
	EEPROM.Two_Menu_FC_E1 = Two_Menu_FC_E1;

	ISendStr(I2C_ADDRESS, BACK, (uint8_t *)&EEPROM, 16);
	delay10ms();
}

void env_init(void)
{
	IRcvStr(I2C_ADDRESS, SIN_KEY, single_key, 16);
	delay10ms();
	IRcvStr(I2C_ADDRESS, MUL_KEY, multiple_key, 16);
	delay10ms();
	IRcvStr(I2C_ADDRESS, BACK, (uint8_t *)&EEPROM, 16);
	delay10ms();

	Two_Menu_F3_E1 =  EEPROM.Two_Menu_F3_E1 ;
	Two_Menu_F3_E2 =  EEPROM.Two_Menu_F3_E2 ;
	Two_Menu_F4_E1 =  EEPROM.Two_Menu_F4_E1 ;
	Two_Menu_F5_E1 =  EEPROM.Two_Menu_F5_E1 ;
	Two_Menu_F6_E1 =  EEPROM.Two_Menu_F6_E1 ;
	Two_Menu_F6_E2 =  EEPROM.Two_Menu_F6_E2 ;
	Two_Menu_F6_E3 =  EEPROM.Two_Menu_F6_E3 ;
	Two_Menu_F6_E4 =  EEPROM.Two_Menu_F6_E4 ;
	Two_Menu_F6_E5 =  EEPROM.Two_Menu_F6_E5 ;
	Two_Menu_F6_E6 =  EEPROM.Two_Menu_F6_E6 ;
	Two_Menu_F6_E7 =  EEPROM.Two_Menu_F6_E7 ;
	Two_Menu_F6_E8 =  EEPROM.Two_Menu_F6_E8 ;
	Two_Menu_F7_E1 =  EEPROM.Two_Menu_F7_E1 ;
	Two_Menu_F8_E1 =  EEPROM.Two_Menu_F8_E1 ;
	Two_Menu_Fb_E1 =  EEPROM.Two_Menu_Fb_E1;
	Two_Menu_FC_E1 =  EEPROM.Two_Menu_FC_E1;

	if (EEPROM.Two_Menu_F3_E1 > 2)
	{
		Two_Menu_F3_E1 = 1;
	}
	if (EEPROM.Two_Menu_F3_E2 > 99)
	{
		Two_Menu_F3_E2 = 1;
	}
	if (EEPROM.Two_Menu_F4_E1 > 99)
	{
		Two_Menu_F4_E1 = 1;
	}
	if (EEPROM.Two_Menu_F5_E1 > 99)
	{
		Two_Menu_F5_E1 = 1;
	}
	if (EEPROM.Two_Menu_F6_E1 > 7)
	{
		Two_Menu_F6_E1 = 0;
	}
#ifndef AWAIT_VOICE_PATCH
#ifdef REPLAY_PATCH
       if (EEPROM.Two_Menu_F6_E2 > 9)
	{
		Two_Menu_F6_E2 = 1;
	}
#else
	if (EEPROM.Two_Menu_F6_E2 > 2)
	{
		Two_Menu_F6_E2 = 1;
	}
#endif
#endif
	if (EEPROM.Two_Menu_F6_E3 > 1)
	{
		Two_Menu_F6_E3 = 0;
	}
	if (EEPROM.Two_Menu_F6_E4 > 9)
	{
		Two_Menu_F6_E4 = 6;
	}
	if (EEPROM.Two_Menu_F6_E5 > 7)
	{
		Two_Menu_F6_E5 = 3;
	}
	if (EEPROM.Two_Menu_F6_E6 > 1)
	{
		Two_Menu_F6_E6 = 0;
	}
	if (EEPROM.Two_Menu_F6_E7 > 1)
	{
		Two_Menu_F6_E7 = 0;
	}
	if (EEPROM.Two_Menu_F6_E8 > 1)
	{
		Two_Menu_F6_E8 = 0;
	}
#ifndef DECODER_DOUBLE_CHECK_PATCH
	if (EEPROM.Two_Menu_F7_E1 > 10)
	{
		Two_Menu_F7_E1 = 10;
	}
#endif

#ifdef DECODER_DOUBLE_CHECK_PATCH
	if (EEPROM.Two_Menu_F7_E1 > 11)
	{
		Two_Menu_F7_E1 = 11;
	}
#endif

	if (EEPROM.Two_Menu_F8_E1 > 2)
	{
		Two_Menu_F8_E1 = 2;
	}
	if (EEPROM.Two_Menu_Fb_E1 > 1)
	{
		Two_Menu_Fb_E1 = 1;
	}
	if (EEPROM.Two_Menu_FC_E1 > 2)
	{
		Two_Menu_FC_E1 = 1;
	}
}

void var_init(void)
{
	Two_Menu_F3_E1 = 1; //��ʱģʽ�����Ŷ���ʾ
	Two_Menu_F3_E2 = 1; //����ʱ��洢����

	Two_Menu_F4_E1 = 0; //����ʱ��
	Two_Menu_F5_E1 = 0; //ѭ�����ʱ��

	Two_Menu_F6_E1 = 0; //�򵥱���
	Two_Menu_F6_E2 = 1; //������������
	Two_Menu_F6_E3 = 0; //ѭ��ʱ���Ƿ񱨶�
	Two_Menu_F6_E4 = 6; //������С����
	Two_Menu_F6_E5 = 3; //��ʾ��LED���ȵ���
	Two_Menu_F6_E6 = 1; //������������
	Two_Menu_F6_E7 = 0; //������������
	Two_Menu_F6_E8 = 0; //������������

	Two_Menu_F7_E1 = 10; // E1Ĭ�ϼ��̹��� 999*9
	Two_Menu_F7_E2 = 0; // E2�������̹��� 9999*9
	Two_Menu_F7_E3 = 0; // E3�������̹��� 999*99
	Two_Menu_F7_E4 = 0; // E4�������̹��� 9999*99

	Two_Menu_F8_E1 = 2; // �������� �ఴ���л�

	Two_Menu_F8_E2 = 0; // ��λ����


}

void key_init(void)
{
	unsigned char i;
	for (i = 0; i < 16; i++)
	{
		single_key[i] = 0x01;
	}

	multiple_key[0] = 0X01;
	multiple_key[1] = QUXIAO - QUXIAO;
	multiple_key[2] = JIEZHANG - QUXIAO;
	multiple_key[3] = DIANDANG - QUXIAO;
	multiple_key[4] = JIUSHUI - QUXIAO;
	multiple_key[5] = 0X01;
	multiple_key[6] = 0x01;
	multiple_key[7] = JIASHUI - QUXIAO;
	multiple_key[8] = HUJIAO - QUXIAO;
	multiple_key[9] =  0x01;
	multiple_key[10] = 0x01;
	multiple_key[11] = 0x01;
	multiple_key[12] = 0x01;
	multiple_key[13] = 0x01;
	multiple_key[14] = 0x01;
	multiple_key[15] = 0x01;
}


#ifdef TRANSMIT_KEY

void Transmit_set_data(unsigned char temp)
{
	if(Transmit_Data_set_FANGQU == 1)
	{
		Transmit_Data_FANGQU =temp;
		Transmit_Data_set_FANGQU = 0;
	}
	else
	{
		if(transmit_confirm_key_press == 1)
		{
			Transmit_Data = 0;
			transmit_confirm_key_press = 0;
		}
		
		if(Transmit_Data < 100)
		{
			Transmit_Data =Transmit_Data*10 + temp;
		}
	}
	tm1629_load();
	display();
}


#endif