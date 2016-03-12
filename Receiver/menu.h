#ifndef __MENU_H_
#define __MENU_H_

#include "global.h"

//����defineΪ�˵����ֶ���ΪӢ����ĸ������ʶ��

#define MENU_STANDBY 0   //����
#define ONE_MENU_F0  1   //һ���Ӳ˵�F0
#define ONE_MENU_F1  2
#define ONE_MENU_F2  3
#define ONE_MENU_F3  4
#define ONE_MENU_F4  5
#define ONE_MENU_F5  6
#define ONE_MENU_F6  7
#define ONE_MENU_F7  8
#define ONE_MENU_F8  9
#define ONE_MENU_F9  10
#define ONE_MENU_FA  11  //һ���Ӳ˵�FA
#define ONE_MENU_Fb  12  //һ���Ӳ˵�Fb
#define ONE_MENU_FC  13  //һ���Ӳ˵�FC
#define ONE_MENU_Fd  14  //һ���Ӳ˵�Fd

//�������˵�����
#define TWO_MENU_F0_YEAR    15  //������
#define TWO_MENU_F0_MOUTH   16	//������
#define TWO_MENU_F0_DAY     17	//������
#define TWO_MENU_F0_WEEK    18	//��������
#define TWO_MENU_F0_HOUR    19	//����Сʱ
#define TWO_MENU_F0_MINUTE  20	//���÷���

//F1�����˵�
#define TWO_MENU_F1_E1      21  //E1ע�������
#define TWO_MENU_F1_E2      22  //E2ע�������
#define TWO_MENU_F1_E3      23  //E3ע�ᱨ����
#define TWO_MENU_F1_E4      24  //E4ע��ȡ����

//F2�����˵�
#define TWO_MENU_F2_E1      25  //E1ɾ��������
#define TWO_MENU_F2_E2      26  //E2ɾ��������
#define TWO_MENU_F2_E3      27  //E3ɾ��������
#define TWO_MENU_F2_E4      28  //E4ɾ��ȡ����

//F3�����˵�
#define TWO_MENU_F3_E1      29  //E1ɾ��������
#define TWO_MENU_F3_E2      30  //E2ɾ��������

//F4 F5�����˵�
#define TWO_MENU_F4_SET     31  //F4��������
#define TWO_MENU_F5_SET     32  //F5��������

//F6�����Ӳ˵�
#define TWO_MENU_F6_E1      33  //E1����ѡ��
#define TWO_MENU_F6_E2      34  //E2������������
#define TWO_MENU_F6_E3      35  //ѭ��ʱ�Ƿ񱨶�
#define TWO_MENU_F6_E4		36  //������С����
#define TWO_MENU_F6_E5      37  //���ȵ���
#define TWO_MENU_F6_E6      96  //������������
#define TWO_MENU_F6_E7      98  //��Ӣ���л�����
#define TWO_MENU_F6_E8      100  //��Ӣ���л�����

//F7�����Ӳ˵�
#define TWO_MENU_F7_E1      38  //E1 ԭʼ����
#define TWO_MENU_F7_E2      39  //E2 ԭʼ����
#define TWO_MENU_F7_E3      40  //E3 ԭʼ����
#define TWO_MENU_F7_E4      41  //E4 ԭʼ����

//F8�����Ӳ˵�
#define TWO_MENU_F8_E1      42   //E1 ��������л�
#define TWO_MENU_F8_E2      43   //E2��λ����

//F9�����˵�
#define TWO_MENU_F9_E1      44  //E1�ָ����ܺͺ���ȫ������
#define TWO_MENU_F9_E2      45  //E2�ָ����ܳ���

//FA Fb FC�����˵�
#define TWO_MENU_FA_SET     46  //FA��������
#define TWO_MENU_Fb_SET     47  //FB��������
#define TWO_MENU_FC_SET     48  //FC��������
#define TWO_MENU_Fd_SET     49  //Fd��������

//F1_E1�Ӳ˵�����
#define TWO_MENU_F1_E1_D1   50  //ע�������ǧλ
#define TWO_MENU_F1_E1_D2   51  //ע���������λ
#define TWO_MENU_F1_E1_D3   52  //ע�������ʮλ
#define TWO_MENU_F1_E1_D4   53  //ע���������λ

//F1_E2�Ӳ˵�����
#define TWO_MENU_F1_E2_D1   54  //ע�������ǧλ
#define TWO_MENU_F1_E2_D2   55  //ע���������λ
#define TWO_MENU_F1_E2_D3   56  //ע�������ʮλ
#define TWO_MENU_F1_E2_D4   57  //ע���������λ

//F1_E3�Ӳ˵�����
#define TWO_MENU_F1_E3_D1   58  //ע�ᱨ����ǧλ
#define TWO_MENU_F1_E3_D2   59  //ע�ᱨ������λ
#define TWO_MENU_F1_E3_D3   60  //ע�ᱨ����ʮλ
#define TWO_MENU_F1_E3_D4   61  //ע�ᱨ������λ

//F1_E4�Ӳ˵�����
#define TWO_MENU_F1_E4_D1   62  //ע��ȡ����ǧλ
#define TWO_MENU_F1_E4_D2   63  //ע��ȡ������λ
#define TWO_MENU_F1_E4_D3   64  //ע��ȡ����ʮλ
#define TWO_MENU_F1_E4_D4   65  //ע��ȡ������λ

//F2_E1�Ӳ˵�����
#define TWO_MENU_F2_E1_D1   66  //ɾ��������ǧλ
#define TWO_MENU_F2_E1_D2   67  //ɾ����������λ
#define TWO_MENU_F2_E1_D3   68  //ɾ��������ʮλ
#define TWO_MENU_F2_E1_D4   69  //ɾ����������λ

//F2_E2�Ӳ˵�����
#define TWO_MENU_F2_E2_D1   70  //ɾ��������ǧλ
#define TWO_MENU_F2_E2_D2   71  //ɾ����������λ
#define TWO_MENU_F2_E2_D3   72  //ɾ��������ʮλ
#define TWO_MENU_F2_E2_D4   73  //ɾ����������λ

//F2_E3�Ӳ˵�����
#define TWO_MENU_F2_E3_D1   74  //ɾ��������ǧλ
#define TWO_MENU_F2_E3_D2   75  //ɾ����������λ
#define TWO_MENU_F2_E3_D3   76  //ɾ��������ʮλ
#define TWO_MENU_F2_E3_D4   77  //ɾ����������λ

//F2_E4�Ӳ˵�����
#define TWO_MENU_F2_E4_D1   78  //ɾ��ȡ����ǧλ
#define TWO_MENU_F2_E4_D2   79  //ɾ��ȡ������λ
#define TWO_MENU_F2_E4_D3   80  //ɾ��ȡ����ʮλ
#define TWO_MENU_F2_E4_D4   81  //ɾ��ȡ������λ

//F3�Ӳ˵�����
#define TWO_MENU_F3_E1_SET  82 //F3_E1����ѡ��
#define TWO_MENU_F3_E2_SET  83 //F3_E1����ѡ��

//F6�Ӳ˵�����
#define TWO_MENU_F6_E1_SET  84 //F6_E1����ѡ��
#define TWO_MENU_F6_E2_SET  85 //F6_E2������������
#define TWO_MENU_F6_E3_SET  86 //F6_E3ѭ���Ƿ񱨶�
#define TWO_MENU_F6_E4_SET  87 //F6_E4������С����
#define TWO_MENU_F6_E5_SET  88 //F6_E5��ʾ�����ȵ���
#define TWO_MENU_F6_E6_SET  97 //F6_E6������������
#define TWO_MENU_F6_E7_SET  99 //F6_E6������������
#define TWO_MENU_F6_E8_SET  101 //F6_E6������������
//F7�Ӳ˵�����
#define TWO_MENU_F7_E1_SET  89 //F7_E1 Ĭ�Ϲ���999*9
#define TWO_MENU_F7_E2_SET  90 //F7_E2 ���ӹ���9999*9
#define TWO_MENU_F7_E3_SET  91 //F7_E3 ���ӹ���999*99
#define TWO_MENU_F7_E4_SET  92 //F7_E4 ���ӹ���9999*99

//F8�Ӳ˵�����
#define TWO_MENU_F8_E1_SET  93 //F8_E1 �����̻��Ƕ����
#define TWO_MENU_F8_E2_SET  94 //F8_E2 ��λ��������

//����˵�
#define DECODER_MENU        95 //������ʾ�˵�

#ifdef COPY_TWO_MACHINE
#define ONE_MENU_FE  102    //���ߴ���24C64 ���˵�
#define TWO_MENU_FE_E1  103 //���ߴ���24C64 ���Ͳ˵�
#define TWO_MENU_FE_E2  104 //���ߴ���24C64 ���ղ˵�
#define TWO_MENU_FE_E1_SET  105 //���ߴ���24C64 ���Ͳ˵������Ӳ˵�
#define TWO_MENU_FE_E2_SET  106 //���ߴ���24C64 ���ղ˵������Ӳ˵�
#endif
#define MENU_END                       107
#define MENU_SIZE   MENU_END
//����Ϊ��������
#define ZERO      1
#define ONE       2
#define TWO       3
#define THREE     4
#define FOUR      5
#define FIVE      6
#define SIX       7
#define SEVEN     8
#define EIGHT     9
#define NINE      10
#define TEN       11
#define ELEVEN    12
#define TWELVE    13
#define THIRTEEN  14
#define FOURTEEN  15
#define FIFTEEN   16
#define SIXTEEN   17
#define SEVENTEEN 18
#define EIGHTEEN  19
#define NINETEEN  20
#define TWENTY    21
#define THIRTY    22
#define FORTY     23
#define FIFTY     24
#define SIXTY     25
#define SEVENTY   26
#define EIGHTY    27
#define NINETTY   28
#define HUNDRED   29
#define AND       30
#define THOUSAND  31

#define ZIMU_A   33
#define ZIMU_B   34
#define ZIMU_C   35
#define ZIMU_D   36
#define ZIMU_E   37
#define ZIMU_F   38
#define ZIMU_G   39
#define ZIMU_H   40
#define ZIMU_I   41
#define ZIMU_J   42
#define ZIMU_K   43
#define ZIMU_L   44
#define ZIMU_M   45
#define ZIMU_N   46
#define ZIMU_O   47
#define ZIMU_P   48
#define ZIMU_Q   49
#define ZIMU_R   50
#define ZIMU_S   51
#define ZIMU_T   52
#define ZIMU_U   53
#define ZIMU_V   54
#define ZIMU_W   55
#define ZIMU_X   56
#define ZIMU_Y   57
#define ZIMU_Z   58

//#define BAOJING   59
#define YINGYUEYI 60
#define YINGYUEER 61
#define DINGDONG  62
#define QING      63
#define DAO       64
#define GUITAI    65
#define QUXIAO    66
#define HUJIAO    67
#define JIUSHUI   68
#define JIEZHANG  69
#define MAIDANG   70
#define DIANDANG  71
#define JIASHUI   72
#define TIANDIAN  73
#define SHIYING   74
#define FUWU      75
#define PIJIU     76
#define JIU       77
#define CUFANG    78
#define CUSHI     79
#define BATAI     80
#define BANGZHU   81
#define YISHENG   82
#define HUSHI     83
#define ANMO      84
#define ZHILIAO   85
#define QIANTAI   86
#define BAOJIE    87
#define BANGONG   88
#define BAZHEN    89
#define HUANYAO   90
#define SHUYE     91
#define JINJI     92
#define JIASHI    93
#define QINGJIE   94
#define GUANLI    95
#define WEIXIU    96
#define GUZHANG   97
#define ZHUXI     98
#define ZHUREN    99
#define LICAI     100
#define SHOUQUAN  101
#define ANQUAN    102
#define BAOAN     103
#define BAOJING   104
#define HAO       105
#define FANG      106
#define CHUANG    107
#define ZHUO      108
#define LOU       109
#define DENGHOU   110
#define CUNCHUSHEZHI   111
#define SHANGCHUSHEZHI 112  /*ɾ������*/
#define XIANSHISHEZHI  113
#define XIAOHAOSHEZHI  114
#define XUNHUANSHEZHI  115
#define YUYINSHEZHI    116
#define JIANPANSHEZHI 117
#define ANJIANSHEZHI   118
#define HUIFUSHEZHI    119
#define DI			   120
#define CHENGGONG      121
#define SHIBAI         122
#define CHUSHIHUA      123

#define QUXIAO_1  51
#define	BAOJING_1 52


#endif