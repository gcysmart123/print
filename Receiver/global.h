#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#undef  WDT_FUNC
#define  REPLAY_PATCH  
#define  COPY_TWO_MACHINE
#define  PRINTER
#define  TRANSMIT_KEY
#define  AWAIT_VOICE_PATCH
#define  CHANGE_DISPLAY_FUNTION
#define  VOICE_DISPLAY_SYNCHRONOUS_PATCH
#define  CHANGE_TM1629_DISPLAY_POSITION
#define  DECODER_DOUBLE_CHECK_PATCH

/*FE 的E2是接收，接收的先设置进入E2*/
/*另外一台机器设置进入FE 的E1，是发送，发送的先设置进入E1*/
#endif