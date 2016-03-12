#ifndef __KEY_H_
#define __KEY_H_

#include "STC15F2K60S2.h"
#include "intrins.h"
#include "global.h"
#include "tpyedef.h"

#define KEY_FUNC	3
#define KEY_UP     	11
#define KEY_DOWN	19
#define KEY_RETURN	27

#ifdef TRANSMIT_KEY
#define KEY_01 1
#define KEY_02 2
#define KEY_03 3
#define KEY_04 4
#define KEY_05 5
#define KEY_06 6
#define KEY_07 7
#define KEY_08 8
#define KEY_09 9
#define KEY_10 10
#define KEY_11 11
#define KEY_12 12
#define KEY_13 13
#define KEY_14 14
#define KEY_15 15
#define KEY_16 16
#define KEY_17 17
#define KEY_18 18
#define KEY_19 19
#define KEY_20 20
#define KEY_21 21
#define KEY_22 22
#define KEY_23 23
#define KEY_24 24
#define KEY_25 25
#define KEY_26 26
#define KEY_27 27
#define KEY_28 28
#define KEY_29 29
#define KEY_30 30
#define KEY_31 31
#define KEY_32 32

#endif

#define HKeyPort  P1
#define LKeyPort  P2

#ifdef TRANSMIT_KEY
extern uint8_t   Transmit_Data_FANGQU;
extern uint8_t   Transmit_Data_set_FANGQU;
extern uint16_t Line_number;
extern uint16_t Transmit_Data;
extern uint8_t Send_status; //0代表接收，1代表发射
extern void Transmit_set_data(unsigned char temp);
#endif

extern unsigned char Two_Menu_F1_E1[5];
extern unsigned char Two_Menu_F1_E2[5];
extern unsigned char Two_Menu_F1_E3[5];
extern unsigned char Two_Menu_F1_E4[5];
extern unsigned char Two_Menu_F2_E1[5];
extern unsigned char Two_Menu_F2_E2[5];
extern unsigned char Two_Menu_F2_E3[5];
extern unsigned char Two_Menu_F2_E4[5];

typedef struct
{
	unsigned char index;
	unsigned char up;//Add index number
	unsigned char down;//Reduced index number
	unsigned char enter;//Enter the index number
	unsigned char esc;//Exit index number
	void(*index_operation)();//Function index
} key_table;

extern unsigned int KeyScan(void);//Keyboard scan function
extern void delay10ms(void);// Delay 10 ms function
extern void delaymulti10ms(unsigned int delay);// Delay 100 ms function
extern unsigned char KeyDecoder(void);//解码函数
extern void KeyProcess(void);//按键处理函数

extern unsigned char return_func_index(void);
extern void set_func_index(unsigned char temp);
extern unsigned char return_Two_Menu_F3_E1(void);
extern unsigned char return_Two_Menu_F3_E2(void);
extern unsigned char return_Two_Menu_F4_E1(void);
extern unsigned char return_Two_Menu_F5_E1(void);
extern unsigned char return_Two_Menu_F6_E1(void);
extern unsigned char return_Two_Menu_F6_E2(void);
extern unsigned char return_Two_Menu_F6_E3(void);
extern unsigned char return_Two_Menu_F6_E4(void);
extern unsigned char return_Two_Menu_F6_E5(void);
extern unsigned char return_Two_Menu_F6_E6(void);
extern unsigned char return_Two_Menu_F6_E7(void);
extern unsigned char return_Two_Menu_F6_E8(void);
extern unsigned char return_Two_Menu_F7_E1(void);
extern unsigned char return_Two_Menu_F7_E2(void);
extern unsigned char return_Two_Menu_F7_E3(void);
extern unsigned char return_Two_Menu_F7_E4(void);
extern unsigned char return_Two_Menu_F8_E1(void);
extern unsigned char return_Two_Menu_F8_E2(void);
extern unsigned char return_Two_Menu_FC_E1(void);
extern unsigned char return_Two_Menu_Fb_E1(void);

extern unsigned char return_sound_table(void);
extern void set_sound_table(unsigned char temp);
extern void env_load(void);
extern void env_init(void);
extern void var_init(void);
extern void key_init(void);
#endif

