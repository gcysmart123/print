#include "pca.h"

unsigned char flag;
unsigned int count,endcount;
bit OP;

WORD value;
WORD value_send_delay;

   
//CPA �ն˷������
void PCA_isr() interrupt 7 using 1
{	
			CCF0 = 0;                       //���жϱ�־
			CCAP0L = value;
			CCAP0H = value >> 8;            //���±Ƚ�ֵ 520us
			value += T100Hz;
			count++;
			if (flag == 1)

			{

				OP = 1;

			}

			else

			{

				OP = 0;

			}

			P12 = OP;
		
		
		
}
//CPA ��ʼ��
void pca_timer_init()
{
	count = 0;

  	flag = 0;

  	OP = 0;

 	P35 = 0;

  	EA = 1;

	ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = 0x00;                    //����PCAʱ��Դ
                                    //��ֹPCA��ʱ������ж�
	value_send_delay = T20Hz;

	value = T100Hz;
    CCAP0L = value;
    CCAP0H = value >> 8;            //��ʼ��PCAģ��0
    value += T100Hz;
    CCAPM0 = 0x49;                  //PCAģ��0Ϊ16λ��ʱ��ģʽ

	CR = 1;
}

void SendIRdata(unsigned char id_data_3,unsigned char id_data_2,unsigned char id_data_1)

{

  int i;

  char id_data;

  //����ͬ��ͷ

  endcount=1;

  flag=1;
  count=0;

  do{}while(count<endcount);


  endcount=31;

  flag=0;

  count=0;

  do{}while(count<endcount);

  //����bit23-bit16

  id_data=id_data_3;

  for(i=0;i<8;i++)

  {
  	  if(id_data&0x80)  //�ж϶���������λΪ1����0

     {
		endcount=3;  //1Ϊ��ĸߵ�ƽ
		flag=1;
     }

    else

     {

     endcount=1;   //0Ϊխ�ĸߵ�ƽ
	 	flag=1;
     }

    

    count=0;

    do{}while(count<endcount);
	
  endcount=4-endcount;

  flag=0;

  count=0;

  do{}while(count<endcount);

    id_data=id_data<<1;

  }

    //����bit15-bit8

  id_data=id_data_2;

  for(i=0;i<8;i++)

  {
     if(id_data&0x80)

     {

        endcount=3;
		flag=1;
     }

     else

     {

       endcount=1;
	   flag=1;
     }

     flag=1;

     count=0;

     do{}while(count<endcount);

	  endcount=4-endcount;

  flag=0;

  count=0;

  do{}while(count<endcount);

     id_data=id_data<<1;

  }

  //����bit8-bit0

  id_data=id_data_1;

  for(i=0;i<8;i++)

  {
     if(id_data&0x80)

     {

         endcount=3;
		 flag=1;
     }

     else

     {

       endcount=1;
	  flag=1;
     }

     flag=1;

     count=0;

     do{}while(count<endcount);

	  endcount=4-endcount;

  flag=0;

  count=0;

  do{}while(count<endcount);
	 
     id_data=id_data<<1;

  }
}

