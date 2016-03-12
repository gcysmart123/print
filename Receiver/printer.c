/*****************************************************************
2015 by quanzhou noraml university
******************************************************************/
#include "global.h"
#include "usart.h"
#include "tm1629.h"
#include "key.h"
/************************************************

1B 40   //打印机初始化
1B 61 01   //居中
57 65 6C 63 6F 6D 65  //welcome
0A  //换行
1B 21 10  // 倍高
4E 6F 2E 20 30 30 31 33  //NO. 0013
1B 21 00  //取消倍高
0A  //换行
77 61 69 74 20 6E 75 6D 62 65 72 3A 30 39 //wait number:02
0A //换行
1B 4A 21 //进纸
1B 4A 21 //进纸
1B 4A 21 //进纸


**************************************************/

/*********************************************
 initialization
*********************************************/
void printer58mm_print(void)
{
	unsigned char i=0,temp=0;
 //   Usart1_SendData(); /*switch to uart1*/
    SendData(0x1B);      /*send command to printer for initialization*/
    SendData(0x40);     //初始化

// temp test
    SendData(0x1B);   //设置居中
    SendData(0x61);
    SendData(0x01);

    SendData(0x57);   //welcome
    SendData(0x65);
    SendData(0x6C);
    SendData(0x63);
    SendData(0x6F);
    SendData(0x6D);
    SendData(0x65);
	
    SendData(0x0A); //换行
    SendData(0x0A); //换行
	
    SendData(0x1B); //倍高
    SendData(0x21);
    SendData(0x10);
	
    SendData(0x4E); //No. 
    SendData(0x6F);
    SendData(0x2E);
    SendData(0x20);
 
    SendData(Line_number/100+0x30);
    SendData(0x20);
    SendData(Line_number%100/10+0x30);
    SendData(0x20);	
    SendData(Line_number%10+0x30);
    SendData(0x20);
    
    SendData(0x1B);//取消倍高
    SendData(0x21);
    SendData(0x00);
	
    SendData(0x0A);//换行
    SendData(0x0A);//换行
	
    SendData(0x77);//wait number 
    SendData(0x61);
    SendData(0x69);
    SendData(0x74);
    SendData(0x20);
    SendData(0x6E);
    SendData(0x75);
    SendData(0x6D);
    SendData(0x62);
    SendData(0x65);
    SendData(0x72);
    SendData(0x3A);

	if(Transmit_Data > (Line_number-1))
	{
		temp = 0;
	}
	else
	{
		temp=Line_number - Transmit_Data-1;
	}
    SendData(temp/100+0x30);
    SendData(temp%100/10+0x30);
    SendData(temp%10+0x30);
	
	
    SendData(0x0A);//换行
	
    SendData(0x1B); //进纸
    SendData(0x4A);
    SendData(0x21);

    SendData(0x1B);//进纸
    SendData(0x4A);
    SendData(0x21);

    SendData(0x1B);//进纸
    SendData(0x4A);
    SendData(0x21);

    SendData(0x1B);//进纸
    SendData(0x4A);
    SendData(0x21);

    SendData(0x1B);//进纸
    SendData(0x4A);
    SendData(0x21);	

// temp test
}

/*********************************************
 set the printer mode before use the printer
*********************************************/
void printer58mm_setmode(unsigned char mode)
{
    SendData(0x1B);
    SendData(0x21);
    SendData(mode);
}

