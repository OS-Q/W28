#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];					/*远端IP地址*/
extern uint16 	remote_port;					/*远端端口号*/
extern uint16		local_port;						/*定义本地端口*/
extern uint8  	use_dhcp;							/*是否使用DHCP获取IP*/
extern uint8  	use_eeprom;						/*是否使用EEPROM中的IP配置信息*/


//#define STM32_SPI1
#define  STM32_SPI2	

/*W5500接STM32的 SPI1硬件接线定义*/
//	W5500_SCS    --->		STM32_GPIOA4								 /*W5500的片选管脚*/
//	W5500_SCLK	 --->	  STM32_GPIOA5								 /*W5500的时钟管脚*/
//  W5500_MISO	 --->		STM32_GPIOA6								 /*W5500的MISO管脚*/
//	W5500_MOSI	 --->		STM32_GPIOA7								 /*W5500的MOSI管脚*/
//	W5500_RESET	 --->   STM32_GPIOC0								 /*W5500的RESET管脚*/
//	W5500_INT		 --->		STM32_GPIOC1								 /*W5500的INT管脚*/

/*W5500接STM32的 SPI2硬件接线定义*/
//	W5500_SCS 	--->	 STM32_GPIOB12								 /*W5500的片选管脚*/
//	W5500_SCLK	--->	 STM32_GPIOB13								 /*W5500的时钟管脚*/
//  W5500_MISO	--->	 STM32_GPIOB14								 /*W5500的MISO管脚*/
//	W5500_MOSI	--->	 STM32_GPIOB15								 /*W5500的MOSI管脚*/
//	W5500_RESET	--->   STM32_GPIOB9								   /*W5500的RESET管脚*/
//	W5500_INT		--->   STM32_GPIOC6								   /*W5500的INT管脚*/

#define NORMAL_STATE            0
#define NEW_APP_IN_BACK         1           //there is new app in back address
#define ApplicationAddress 	    0x08008000  // 0x08000000-0x08007FFF Boot size is 32K
#define AppBackupAddress        0x08018000  // 0x08008000-0x08017FFF App back start address and size is 64k
#define ConfigAddr		          0x08028000  // 0x08018000-0x08027FFF APP backup and configaddr start address is 64k 
#define VerAddr                 0x08029000  // 版本信息存储   

#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460											  /*定义每个数据包的大小*/
#define KEEP_ALIVE_TIME	     		30	                        // 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0                          /*使用初始定义的IP信息*/
#define IP_FROM_DHCP	          1                          /*使用DHCP获取IP信息*/
#define IP_FROM_EEPROM	        2							             /*使用EEPROM定义的IP信息*/
extern uint8	ip_from;											               /*选择IP信息配置源*/
#pragma pack(1)
/*此结构体定义了W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG											
{
  uint8 op[4];
	uint8 mac[6];																						 /*MAC地址*/
  uint8 sw_ver[2];																				 /*软件版本号*/
	uint8 lip[4];																						 /*local IP本地IP地址*/
  uint8 sub[4];																						 /*子网掩码*/
  uint8 gw[4];																						 /*网关*/	
  uint8 dns[4];																						 /*DNS服务器地址*/
  //uint8 rip[4];																						 /*remote IP远程IP地址*/

  uint8 dhcp;
  uint8 debug;
  uint16 fw_len;
  uint8 state;
}CONFIG_MSG;



#pragma pack()

#pragma pack(1)
/*此结构体定义了eeprom写入的几个变量，可按需修改*/
typedef struct _EEPROM_MSG	                    
{
	uint8 mac[6];																							/*MAC地址*/
  uint8 lip[4];																							/*local IP本地IP地址*/
  uint8 sub[4];																							/*子网掩码*/
  uint8 gw[4];																							/*网关*/
}EEPROM_MSG_STR;
#pragma pack()

extern EEPROM_MSG_STR EEPROM_MSG;
extern CONFIG_MSG  	ConfigMsg,RecvMsg;
extern uint8 dhcp_ok;																				/*DHCP获取成功*/
extern uint32	dhcp_time;																		/*DHCP运行计数*/
extern vu8	ntptimer;																				/*NPT秒计数*/

/*MCU配置相关函数*/
void gpio_for_w5500_config(void);														/*SPI接口reset 及中断引脚*/
void timer2_init(void);																			/*STM32定时器2初始化*/
void timer2_isr(void);																			/*定时器中断执行的函数*/
void reboot(void);																					/*STM32软复位*/
void write_config_to_eeprom(void);													/*写配置信息到EEPROM中*/
void read_config_from_eeprom(void);													/*从EEPROM中读出信息*/

/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*写入一个8位数据到W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*从W5500读出一个8位数据*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void reset_w5500(void);																			/*硬复位W5500*/
void set_w5500_mac(void);																		/*配置W5500的MAC地址*/
void set_w5500_ip(void);																		/*配置W5500的IP地址*/

/*需要用定时的的应用函数*/
void dhcp_timer_init(void);																	/*dhcp用到的定时器初始化*/
void ntp_timer_init(void);																	/*npt用到的定时器初始化*/


void set_default(void);
void set_network(void);
uint32 FLASH_PagesMask(vu32 Size);
void write_config_to_flash(void);
void reboot_app(void);
void copy_app(uint32 fw_len);
void get_config(void);
void get_vernn(void);
void write_ver_to_flash(void);

#endif
