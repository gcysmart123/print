#include "ev1527.h"
#include "timer.h"
#include "key.h"
#include "at24c256.h"
#include "menu.h"

/********************���뷽ʽר�ñ���*******************************/
unsigned char Save_RF_trans1 = 0;
unsigned char Save_RF_trans0 = 0;
unsigned char RF_trans0 = 0;
unsigned char RF_trans1 = 0;
unsigned char RF_BIT_COUNTER = 0; 
unsigned char Timer0_interrupt_count = 0; //�����жϺ�400MS  �޽�� �ָ���ʼ������
unsigned char P_RF_INT = 0x00;
unsigned char RF_RECE_REG[3] = { 0 };

unsigned int measure_sync_count1 = 70; //�����������
unsigned int measure_sync_count2 = 120;//ͬ��
unsigned int RF_trans1_test_count = 0; //�볤���Ա���
unsigned int RF_trans0_test_count = 0; //�볤���Ա���
unsigned int RF_trans_count = 0;	   //�볤���Ա���
unsigned int measure_sync_count1_saved = 70;
unsigned int measure_sync_count2_saved = 120;
unsigned int tunning_finish_count = 0;

unsigned char receive_rf_decoder_finished = 0;      //һ�ν�����ɱ�־λ
unsigned char again_receive_rf_decoder_finished = 0;//���ν�����ɱ�־λ
unsigned char rx_table = 0;							//���ν���switch�������
unsigned char again_and_again_decoder_table = 0;     //���α���ɹ���־λ

unsigned char register_manager = 0;





unsigned char old1_RF_RECE_REG[3] = { 0 };//����У��
unsigned char old2_RF_RECE_REG[3] = { 0 };//���ڶ�ȡ

void exint0_init(void) //�ⲿ�ж�0 ��ʼ��
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
}

void exint0() interrupt 0 //�ⲿ�ж�0�жϷ�����
{
	EX0 = 0;
	if (TR1 == 0)
	{
		//Init_Timer1();//sjz add for initialization timer0 before enable it
		TR1 = 1;
	}
}

void RF_ini_receive(void) //��ʼ�����н������õ��ı���
{
	Save_RF_trans1 = 0; Save_RF_trans0 = 0;
	RF_trans0 = RF_trans1 = 0;
	RF_BIT_COUNTER = 0;
	P_RF_INT = 0x00;
	Timer0_interrupt_count = 0;//sjz
}

void RF_decode_main(void)
{
	if (P3_RF_RXD == 1)
	{
		++RF_trans1;
	}
	else
	{
		++RF_trans0;
	}
	Timer0_interrupt_count++;
	if (Timer0_interrupt_count>4000)/*4000*100us=400ms,*/
	{
		TR1 = 0;/*shut down timer0 interrupt sjz*/
		EX0 = 1;
		RF_ini_receive();
		return;

	}
	switch (P_RF_INT)
	{
	case 0:  //�ȴ����͵�ƽͬ��
		if (RF_trans1 > 0)
		{
			//�����ĸ�����̫��
			RF_ini_receive();
			EX0 = 1;
			TR1 = 0;
			return;
		}
		if (RF_trans0 >70)//measure_sync_count1)
		{
			//����ͬ�����
			P_RF_INT++;
			RF_trans0 = RF_trans1 = 0;
		}
		break;
	case 1: //�ȴ�������ͬ��ͷ
		if ((RF_trans0 + RF_trans1) >140 )//measure_sync_count2)/*sjz change from 150 to 120,to shorten the detection when  the header of synchronization coming*/
		{
			//10mSû��ͬ��������
			RF_ini_receive();
			EX0 = 1;
			TR1 = 0;
			return;
		}
		if (RF_trans1 > 1)
		{
			//�ߵ�ƽͬ������(���ټ�⵽ 2 ��)
			if (EX0 == 1)
			{
				EX0 = 0;/*double check,since we have already detect the synchronization,if EXTINT0 is enable,need to disable  sjz*/
			}
			P_RF_INT++;
			RF_trans0 = 0;
			// RF_L_wait_H = 1;
		}
		break;
	case 2:
		if (RF_trans0 > 1)
		{
			Save_RF_trans1 = RF_trans1;
			RF_trans1 = 0;
			P_RF_INT++;
		}
		if (RF_trans1>25)//((measure_sync_count2 + measure_sync_count1) >> 3)) /*if the high level is bigger than 25*100us,Then should be setted as noise instead of useful signal sjz*/
		{
			RF_ini_receive();//KEY_HOLD = 0;
			EX0 = 1;
			TR1 = 0;
			return;
		}
		break;
	case 3:if (RF_trans1 > 1)
	{
			   Save_RF_trans0 = RF_trans0;
			   RF_trans0 = 0;
			   P_RF_INT = 2;
			   RF_RECE_REG[RF_BIT_COUNTER / 8] <<= 1;
			   if (Save_RF_trans1 >Save_RF_trans0)
			   {
				   RF_RECE_REG[(RF_BIT_COUNTER) / 8] |= 0x01;
			   }
			   ++RF_BIT_COUNTER;
			   if (RF_BIT_COUNTER >23)
			   {
#ifndef DECODER_DOUBLE_CHECK_PATCH
				   //P_RF_INT = 4;
				   TR1 = 0;
				   RF_ini_receive();
				   receive_rf_decoder_finished = 1;
				   EX0 = 1;
				   break;
#endif

#ifdef DECODER_DOUBLE_CHECK_PATCH
				   P_RF_INT = 4;
				   //TR1 = 0;
				   //RF_ini_receive();
				   //receive_rf_decoder_finished = 1;
				   //EX0 = 1;
				   break;
#endif
			   }
	}
		   if (RF_trans0> 25)//((measure_sync_count2 + measure_sync_count1) >> 3))
		   {
			   RF_ini_receive();
			   EX0 = 1;
			   TR1 = 0;
			   return;
		   }
		   break;
#ifdef DECODER_DOUBLE_CHECK_PATCH
	case 4:
		if (RF_trans0 > 0)
		{
			Save_RF_trans1 = RF_trans1;
			RF_trans1 = 0;
			P_RF_INT++;
		}
		if (RF_trans1>((measure_sync_count2 + measure_sync_count1) >> 3)) /*if the high level is bigger than 25*100us,Then should be setted as noise instead of useful signal sjz*/
		{
			RF_ini_receive();//KEY_HOLD = 0;
			EX0 = 1;
			TR1 = 0;
			return;
		}
		break;
	case 5:
		if (RF_trans1 > 0)
		{
			if (RF_trans0 > measure_sync_count2)
			{
				TR1 = 0;
				//RF_ini_receive();
				receive_rf_decoder_finished = 1;
				EX0 = 1;
			}
			RF_ini_receive();
		}
		if (RF_trans0>(measure_sync_count2 + measure_sync_count1))
		{
			RF_ini_receive();
			EX0 = 1;
			TR1 = 0;
			receive_rf_decoder_finished = 0;
			return;
		}
		break;
#endif

	default: //�쳣����
	{
				 RF_ini_receive();
				 EX0 = 1;
				 TR1 = 0;
	}break;
	}
}

void receive_rf_decoder(void)
{

	unsigned char func_index_temp = 0;
	uint32_t dat;
	RF_def RFtmp;
	func_index_temp = return_func_index();

	if (receive_rf_decoder_finished == 1)
	{
		receive_rf_decoder_finished = 0;
		EX0 = 0;
		switch (rx_table)
		{
		case 0:	//��һ��ȡ����
			old1_RF_RECE_REG[0] = RF_RECE_REG[0];
			old1_RF_RECE_REG[1] = RF_RECE_REG[1];
			old1_RF_RECE_REG[2] = RF_RECE_REG[2];
			rx_table = 1;
			break;
		case 1:
			if (old1_RF_RECE_REG[0] == RF_RECE_REG[0] && old1_RF_RECE_REG[1] == RF_RECE_REG[1] && old1_RF_RECE_REG[2] == RF_RECE_REG[2])
			{
				if (old2_RF_RECE_REG[0] == RF_RECE_REG[0] && old2_RF_RECE_REG[1] == RF_RECE_REG[1] && old2_RF_RECE_REG[2] == RF_RECE_REG[2])
				{
					if (again_and_again_decoder_table == 0) //�����λ������0  ��������һ�ν���������
					{
						old2_RF_RECE_REG[0] = RF_RECE_REG[0];
						old2_RF_RECE_REG[1] = RF_RECE_REG[1];
						old2_RF_RECE_REG[2] = RF_RECE_REG[2];
						again_receive_rf_decoder_finished = 1;
						clear_again_and_again_time();
					}

				}
				else
				{
					old2_RF_RECE_REG[0] = RF_RECE_REG[0];
					old2_RF_RECE_REG[1] = RF_RECE_REG[1];
					old2_RF_RECE_REG[2] = RF_RECE_REG[2];
					again_receive_rf_decoder_finished = 1;
					clear_again_and_again_time();
				}
				
				again_and_again_decoder_table = 1;
				clear_return_standby_time();
				dat = ((uint32_t)old2_RF_RECE_REG[0]) << 16 | ((uint32_t)old2_RF_RECE_REG[1]) << 8 | ((uint32_t)old2_RF_RECE_REG[2]);

				if (Find_RF_EEPROM_Host(&RFtmp, dat))
				{
					register_manager = 1;
					set_sound_table(1);
					clear_return_standby_time();
				}

				RF_RECE_REG[0] = 0;
				RF_RECE_REG[1] = 0;
				RF_RECE_REG[2] = 0;
				old1_RF_RECE_REG[0] = 0;
				old1_RF_RECE_REG[1] = 0;
				old1_RF_RECE_REG[2] = 0;
				if (func_index_temp != TWO_MENU_F8_E2_SET)
				{
					//set_sound_table(1);
				}
				//func_f1_1_4_table = 1;
				rx_table = 0;
			}
			else
			{
				old1_RF_RECE_REG[0] = RF_RECE_REG[0];
				old1_RF_RECE_REG[1] = RF_RECE_REG[1];
				old1_RF_RECE_REG[2] = RF_RECE_REG[2];
			}
			break;
		}
		EX0 = 1;	
	}
}

void RF_decode_main_sjz_test(void)
{

	if (P3_RF_RXD == 1)
	{
		++RF_trans1;
	}
	else
	{
		++RF_trans0;
	}
	Timer0_interrupt_count++;
	if (Timer0_interrupt_count>4000)/*4000*100us=400ms,*/
	{
		TR1 = 0;/*shut down timer0 interrupt sjz*/
		EX0 = 1;
		RF_ini_receive();
		return;
	}
	switch (P_RF_INT)
	{
	case 0:  //�ȴ����͵�ƽͬ��
		if (RF_trans1 > 0)
		{
			//�����ĸ�����̫��

			EX0 = 1;
			TR1 = 0;
			RF_ini_receive();
			return;
		}
		if (RF_trans0 >(60 + RF_trans1_test_count*TUNNING_STEP))
		{
			//����ͬ�����
			P_RF_INT++;
			measure_sync_count1 = 60 + RF_trans1_test_count*TUNNING_STEP;
			RF_trans0 = RF_trans1 = 0;
		}
		break;
	case 1: //�ȴ�������ͬ��ͷ
		if ((RF_trans0 + RF_trans1) >(120 + RF_trans0_test_count*TUNNING_STEP))//120)/*sjz change from 150 to 120,to shorten the detection when  the header of synchronization coming*/
		{
			//10mSû��ͬ��������
			// measure_sync_count2=RF_trans0;
			if (RF_trans1 == 0)
			{
				RF_trans0_test_count++;
				RF_trans1_test_count++;
			}
			RF_ini_receive();
			EX0 = 1;
			TR1 = 0;
			return;
		}

		if (RF_trans1 > 1)
		{
			measure_sync_count2 = 120 + RF_trans0_test_count*TUNNING_STEP;
			RF_trans0_test_count = 0;
			RF_trans1_test_count = 0;
			//�ߵ�ƽͬ������(���ټ�⵽ 2 ��)
			if (EX0 == 1)
			{
				EX0 = 0;/*double check,since we have already detect the synchronization,if EXTINT0 is enable,need to disable  sjz*/
			}
			P_RF_INT++;
			RF_trans0 = 0;
			// RF_L_wait_H = 1;
		}
		break;
	case 2:
		if (RF_trans0 > 0)
		{
			Save_RF_trans1 = RF_trans1;
			RF_trans1 = 0;
			P_RF_INT++;
		}
		if (RF_trans1>((measure_sync_count2 + measure_sync_count1) >> 3))//20) /*if the high level is bigger than 25*100us,Then should be setted as noise instead of useful signal sjz*/
		{
			RF_ini_receive();//KEY_HOLD = 0;
			EX0 = 1;
			TR1 = 0;
			return;
		}
		break;
	case 3:if (RF_trans1 > 0)
	{
			   Save_RF_trans0 = RF_trans0;
			   RF_trans0 = 0;
			   P_RF_INT = 2;
			   RF_RECE_REG[RF_BIT_COUNTER / 8] <<= 1;
			   if (Save_RF_trans1 >Save_RF_trans0)
			   {
				   RF_RECE_REG[(RF_BIT_COUNTER) / 8] |= 0x01;
			   }
			   ++RF_BIT_COUNTER;
			   if (RF_BIT_COUNTER >23)
			   {
				   TR1 = 0;
				   RF_trans_count = Save_RF_trans1 + Save_RF_trans0;
				   RF_ini_receive();
					//   receive_rf_decoder_finished = 1;
				   /*sjz*/
				   tunning_finish_count++;
				   measure_sync_count2_saved = measure_sync_count2_saved + measure_sync_count2;
				   measure_sync_count2 = measure_sync_count2_saved >> 1;
				   measure_sync_count2_saved = measure_sync_count2;

				   if ((tunning_finish_count>1) && ((measure_sync_count1 - measure_sync_count1_saved <= 30) || (measure_sync_count1_saved - measure_sync_count1 <= 30)))
				   {
					   measure_sync_count1_saved = measure_sync_count1_saved + measure_sync_count1;
					   measure_sync_count1 = measure_sync_count1_saved >> 1;
					   measure_sync_count1_saved = measure_sync_count1;
					 //  decoder_speed_test_finish = 1;
				   }

				   EX0 = 1;
				   break;
			   }
	}
		   if (RF_trans0>((measure_sync_count2 + measure_sync_count1) >> 3))//20)
		   {
			   RF_ini_receive();
			   EX0 = 1;
			   TR1 = 0;
			   return;
		   }
		   break;
	default: //�쳣����
	{
				 RF_ini_receive();
				 EX0 = 1;
				 TR1 = 0;
	}break;
	}
}

unsigned char return_again_receive_rf_decoder_finished(void)
{
	unsigned char temp;
	temp = again_receive_rf_decoder_finished;
	return temp;
}

void clear_again_receive_rf_decoder_finished(void)
{
	again_receive_rf_decoder_finished = 0;
}

unsigned char return_again_and_again_decoder_table(void)
{
	unsigned char temp;
	temp = again_and_again_decoder_table;
	return temp;
}

void clear_again_and_again_decoder_table(void)
{
	again_and_again_decoder_table = 0;
}

unsigned char return_register_manager(void)
{
	unsigned char temp;
	temp = register_manager;
	return temp;
}

void clear_register_manager(void)
{
	register_manager = 0;
}

unsigned int return_RF_trans_count(void)
{
	unsigned int temp;
	temp = RF_trans_count;
	return temp;
}
