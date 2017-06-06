
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */ 
#include <stdio.h>  /* for memcpy() */ 
#include "define.h"
//#include "FlashWrite.h"
//#include "global.h"
#pragma MESSAGE DISABLE C4001   // condition always FALSE 

//#define ERASEFLASHFN_SIZE 80
    
#define PAGESIZE 512

#define ERASEERROR  1               //erase error
#define PROGERROR   2		            //program error

//Flash Commands
#define BlankCheck      0x05
#define ByteProg        0x20
#define ByteProgBurst   0x25
#define PageErase       0x40
#define MassErase       0x41

//#define DIVISOR_FL      (BUSCLK / 200000L)
/** 
** Refer to the PRM file of your project and define HCS08_FLASH_START_ADDR 
** accordingly. Also the PRM file should be modified to specify the new ROM 
** address. 2 pages are used for EEPROM emulatio, thus ROM = ROM + 0x200. 
**/ 

/** 
** Page size of HCS08 is 0x200 
**/ 
#define HCS08_FLASH_PAGE_SIZE   0x200 
#define UseingByte 15

/** 
** CPU bus frequncy, modify it according to your system configuration. 
** Unit is kHz. For example, 4 MHz is defined as 4000 (kHz) 
**/ 
#ifdef High_Speed
#define HCS08_BUS_FREQUENCY     18432	//2221	//   1110
#else
#define HCS08_BUS_FREQUENCY     4443	//2221	//   1110
#endif
/** 
** FLASH block protection. Default is off.  
** It is stongly to turn it on to protect the code space of CPU
*/
#define HCS08_FLASH_PROTECTION  0xFF 
/** 
**  Possible return values of operation result. 
**/ 
#define HCS08_EE_ERROR_OK           0x00   
#define HCS08_EE_ERROR_UNKOWN       0xFF 
#define HCS08_EE_ERROR_OUT_OF_RANGE 0x01
/** 
**  Below macros are defined for API use only.  
**  Do NOT modify it.  
**/ 
 
#define FLASH_CMD_BYTEPROG  0x20 
#define FLASH_CMD_BURST_BYTEPROG  0x25
#define FLASH_CMD_PAGEERASE 0x40     
 
#define EEPROM_START_ADDR HCS08_FLASH_START_ADDR 
#define EEPROM_PAGE_SIZE  HCS08_FLASH_PAGE_SIZE  
extern uchar SaveData[336];
static void flash_byte_prog(unsigned int _addr, unsigned char _data); 
static void flash_page_erase(unsigned int _addr); 


static unsigned char flash_byte_read(unsigned int _addr); 
#pragma DATA_SEG FAR_RAM

#pragma DATA_SEG DEFAULT

const uchar FlagPage1 @(EE_PageCopy1+TargerFlagPosition)  = 0xff;
const uchar FlagPage1Target @(EE_PageCopy1+TargerNumPosition) = 0xff;
const uchar F_defaultsettingpara[sizeof(Setting) + 2]@(EE_Page1)={
	  0x55,
	  0xaa,
	  0x01,//call melody
	  0x02,//floor call melody
	  0x03,//call volume
	  //0x01,//beep on or off
	  0x03,//audio volume
	  0x01,//theme
	  0x08,//S_Flag only beep is on as default	  
	  0x00,//TOCC
	  0x00,//colume address
	  0x03//PSTN moledy
	 //
	  //0x08
	};
//const uchar FlagPage2 @(EE_PageCopy2+TargerFlagPosition) = 0xff;
//const uchar FlagPage2Target @(EE_PageCopy2+TargerNumPosition) = 0xff;
//const uchar Recordlenth @(EE_PageBook+P_recordcnt) = 0;
//const uchar LEDFlag @(EE_PageLED) = 0;

//const unsigned char __NVPROT @ 0xFFBD = HCS08_FLASH_PROTECTION;
unsigned char _pgm1;

const uchar F_882_80_B1[336]	@(EE_HomeAutomation1_1) = { 
	9,1,Relay_Button,'L','i','g','h','t',' ','1',' ',7,11,1,Protocol_KNX,0,0,0,0,0,0,
	9,2,Relay_Button,'L','i','g','h','t',' ','2',' ',7,11,2,Protocol_KNX,0,0,0,0,0,0,
	9,3,Relay_Button,'L','i','g','h','t',' ','3',' ',7,11,3,Protocol_KNX,0,0,0,0,0,0,
	9,4,Relay_Button,'L','i','g','h','t',' ','4',' ',7,11,4,Protocol_KNX,0,0,0,0,0,0,
	9,5,Dimmer_Button,'D','i','m','m','e','r',' ','1',8,11,5,Protocol_KNX,9,22,9,23,0,0,
	9,6,Dimmer_Button,'D','i','m','m','e','r',' ','2',8,11,6,Protocol_KNX,9,24,9,25,0,0,
	9,7,Dimmer_Button,'D','i','m','m','e','r',' ','3',8,11,7,Protocol_KNX,9,26,9,27,0,0,
	9,8,Dimmer_Button,'D','i','m','m','e','r',' ','4',8,11,8,Protocol_KNX,9,28,9,29,0,0,

	9,31,3,'R','G','B','L','i','g','h','t',8,11,9,Protocol_KNX,9,32,9,33,0,0,
	10,1,Shutter_Blind_Button,'B','l','i','n','d',' ','1',' ',7,11,11,Protocol_KNX,10,2,11,1,11,2,

	9,71,Shutter_L_R_Button,'S','h','u','t','t','e','r','1',8,11,10,Protocol_KNX,9,72,0,0,0,0,
	9,73,4,'S','h','u','t','t','e','r','2',8,11,11,Protocol_KNX,9,74,0,0,0,0,										        		
	9,13,1,'S','i','r','e','n',' ',' ',' ',5,11,13,Protocol_KNX,0,0,0,0,0,0,
	9,14,1,'A','i','r',' ','C','o','n',' ',7,11,14,Protocol_KNX,0,0,0,0,0,0,
	9,15,Valve_Button1,'B','o','i','l','e','r',' ',' ',6,11,15,Protocol_KNX,0,0,0,0,0,0,
	9,16,Valve_Button,'G','a','s',' ',' ',' ',' ',' ',3,11,16,Protocol_KNX,0,0,0,0,0,0};



										  		

const uchar F_882_80_B2[336]	@(EE_HomeAutomation2_1) = { 
	9,1,1,'L','i','g','h','t',' ','1',' ',7,11,1,Protocol_KNX,0,0,0,0,0,0,
	9,2,1,'L','i','g','h','t',' ','2',' ',7,11,2,Protocol_KNX,0,0,0,0,0,0,
	9,3,1,'L','i','g','h','t',' ','3',' ',7,11,3,Protocol_KNX,0,0,0,0,0,0,
	9,4,1,'L','i','g','h','t',' ','4',' ',7,11,4,Protocol_KNX,0,0,0,0,0,0,
	9,5,2,'D','i','m','m','e','r',' ','1',8,11,5,Protocol_KNX,9,22,9,23,0,0,
	9,6,2,'D','i','m','m','e','r',' ','2',8,11,6,Protocol_KNX,9,24,9,25,0,0,
	9,7,2,'D','i','m','m','e','r',' ','3',8,11,7,Protocol_KNX,9,26,9,27,0,0,
	9,8,2,'D','i','m','m','e','r',' ','4',8,11,8,Protocol_KNX,9,28,9,29,0,0,

	9,31,3,'R','G','B','L','i','g','h','t',8,11,9,Protocol_KNX,9,32,9,33,0,0,
	9,200,HVAC_Button,'H','V','A','C',' ',' ',' ',' ',4,11,9,Protocol_KNX,9,32,9,33,0,0,
	10,1,Shutter_Blind_Button,'B','l','i','n','d',' ','1',' ',7,11,11,Protocol_KNX,10,2,11,1,11,2,
	10,3,Shutter_Blind_Button,'B','l','i','n','d',' ','2',' ',7,11,11,Protocol_KNX,10,4,11,3,11,4,										        											        		
	9,13,1,'S','i','r','e','n',' ',' ',' ',5,11,13,0,0,0,0,0,0,0,
	9,14,1,'A','i','r',' ','C','o','n',' ',7,11,14,0,0,0,0,0,0,0,
	9,15,Valve_Button1,'B','o','i','l','e','r',' ',' ',6,11,15,0,0,0,0,0,0,0,
	9,16,Valve_Button,'G','a','s',' ',' ',' ',' ',' ',3,11,16,0,0,0,0,0,0,0};
  


const uchar F_882_80_B3[336]	@(EE_HomeAutomation3_1) = { 
	9,1,1,'L','i','g','h','t',' ','1',' ',7,11,1,Protocol_KNX,0,0,0,0,0,0,
	9,2,1,'L','i','g','h','t',' ','2',' ',7,11,2,Protocol_KNX,0,0,0,0,0,0,
	9,3,1,'L','i','g','h','t',' ','3',' ',7,11,3,Protocol_KNX,0,0,0,0,0,0,
	9,4,1,'L','i','g','h','t',' ','4',' ',7,11,4,Protocol_KNX,0,0,0,0,0,0,
	9,51,2,'D','i','m','m','e','r',' ','1',8,11,5,Protocol_KNX,9,37,0,0,0,0,
	9,52,2,'D','i','m','m','e','r',' ','2',8,11,6,Protocol_KNX,9,38,0,0,0,0,
	9,53,2,'D','i','m','m','e','r',' ','3',8,11,7,Protocol_KNX,9,39,0,0,0,0,
	9,54,2,'D','i','m','m','e','r',' ','4',8,11,8,Protocol_KNX,9,40,0,0,0,0,

	9,31,3,'R','G','B','L','i','g','h','t',8,11,9,Protocol_KNX,9,32,9,33,0,0,
	10,200,AC_Button,'A','C','B','L','i','g','h','t',2,11,9,Protocol_KNX,9,32,9,33,0,0,
	/*10,1,4,'S','h','u','t','t','e','r','1',8,11,11,0,10,2,0,0,0,0,
	10,1,Shutter_Blind_Button,'S','h','u','t','t','e','r','2',8,11,11,0,10,2,11,1,11,2,*/
	9,71,4,'S','h','u','t','t','e','r','1',8,11,10,Protocol_KNX,9,72,0,0,0,0,
	9,73,4,'S','h','u','t','t','e','r','2',8,11,11,Protocol_KNX,9,74,0,0,0,0,

	9,13,1,'S','i','r','e','n',' ',' ',' ',5,11,13,Protocol_KNX,0,0,0,0,0,0,
	9,14,1,'A','i','r',' ','C','o','n',' ',7,11,14,Protocol_KNX,0,0,0,0,0,0,
	9,15,Valve_Button1,'B','o','i','l','e','r',' ',' ',6,11,15,Protocol_KNX,0,0,0,0,0,0,
	9,16,Valve_Button,'G','a','s',' ',' ',' ',' ',' ',3,11,16,Protocol_KNX,0,0,0,0,0,0};

const uchar F_882_80_B4[336]	@(EE_HomeAutomation4_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};

const uchar F_882_80_B5[336]	@(EE_HomeAutomation5_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};
	
const uchar F_882_80_B6[336]	@(EE_HomeAutomation6_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};

const uchar F_882_80_B7[336]	@(EE_HomeAutomation7_1) = {
	0x51,10,1,'B','B','T','T','O','N','0','1',8,0x53,10,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};
	
const uchar F_882_80_B8[336]	@(EE_HomeAutomation8_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};


const uchar F_882_80_B9[336]	@(EE_HomeAutomation9_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};
const uchar F_882_80_B10[336]	@(EE_HomeAutomation10_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};

const uchar F_882_80_B11[336]	@(EE_HomeAutomation11_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};

const uchar F_882_80_B12[336]	@(EE_HomeAutomation12_1) = {
	9,1,1,'B','B','T','T','O','N','0','1',8,11,1,0,0,0,0,0,0,0,
	9,2,1,'B','U','T','T','O','N','0','2',8,11,2,0,0,0,0,0,0,0,
	9,3,1,'B','U','T','T','O','N','0','3',8,11,3,0,0,0,0,0,0,0,
	9,4,1,'B','U','T','T','O','N','0','4',8,11,4,0,0,0,0,0,0,0,
	9,5,2,'B','U','T','T','O','N','0','5',8,11,5,0,0,0,0,0,0,0,
	9,6,2,'B','U','T','T','O','N','0','6',8,11,6,0,0,0,0,0,0,0,
	9,7,2,'B','U','T','T','O','N','0','7',8,11,7,0,0,0,0,0,0,0,
	9,8,3,'B','U','T','T','O','N','0','8',8,11,8,0,0,0,0,0,0,0,
	9,9,1,'B','U','T','T','O','N','0','9',8,11,9,0,0,0,0,0,0,0,
	9,10,1,'B','U','T','T','O','N','1','0',8,11,10,0,0,0,0,0,0,0,
	9,11,1,'B','U','T','T','O','N','1','1',8,11,11,0,0,0,0,0,0,0,
	9,12,1,'B','U','T','T','O','N','1','2',8,11,12,0,0,0,0,0,0,0,
	9,13,1,'B','U','T','T','O','N','1','3',8,11,13,0,0,0,0,0,0,0,
	9,14,1,'B','U','T','T','O','N','1','4',8,11,14,0,0,0,0,0,0,0,
	9,15,1,'B','U','T','T','O','N','1','5',8,11,15,0,0,0,0,0,0,0,
	9,16,1,'B','U','T','T','O','N','1','6',8,11,16,0,0,0,0,0,0,0};	
//input information
const uchar F_IutPut1[200]	@(EE_Input_Memory1) = {10,1,1,'I','N','P','U','T','0','0','1',8,11,1,1,10,0x0f,0x11,1,0,
										           10,2,2,'I','N','P','U','T','0','0','2',8,11,2,0,10,0x0f,0x11,1,0,
										           10,3,3,'I','N','P','U','T','0','0','3',8,11,3,2,10,0x0f,0x11,1,0,
										           10,4,4,'I','N','P','U','T','0','0','4',8,11,4,1,10,0x0f,0x11,1,0,
										           10,5,5,'I','N','P','U','T','0','0','5',8,11,5,0,0,0,0,0,0,
										           10,6,6,'I','N','P','U','T','0','0','6',8,11,6,0,0,0,0,0,0,
										           10,7,7,'I','N','P','U','T','0','0','7',8,11,7,0,0,0,0,0,0,
										           10,8,8,'I','N','P','U','T','0','0','8',8,11,8,0,0,0,0,0,0,
										           10,9,1,'I','N','P','U','T','0','0','9',8,11,9,0,0,0,0,0,0,
										           10,10,1,'I','N','P','U','T','0','1','0',8,11,10,1,30,0x3f,0x11,1,0};
																				           
const uchar F_Tel_num[200]	@(EE_Page_Tel_Momory) = {0,0x38,0x30,0x30,0x34,0,0,0,0,0,0,0,0,0,0,0,0,
												 0,0x38,0x30,0x30,0x35,0,0,0,0,0,0,0,0,0,0,0,0,
												 0,'9','1','8','6','8','2','1','8','9','7','6','7',0,0,0,0};

//const uchar F_Alarm_Enable_GroupAddress[3] @(EE_Page10 + 2) = {0,0,0,0,0,0}; // this can link with scenario address, it means one button enable alarm system
//const uchar F_Alarm_Flag @(EE_Page10 + 1) = 0x00;
const Password_Alarmsystem F_Password_AlarmSystem @(EE_Page_AlarmSystem+SpaceByte) = {
																'1','2','3','4','5','6',
																'6','5','4','3','2','1'	
																};
const AlarmSystem	 F_Alarm_System @(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem)) ={0,7,0,0x00,0x0915,21,0x0915,22,30};
const ZoneNameS F_ZoneName[6] @(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem)) ={'Z','o','n','e','0','1',' ',' ',6,
																							 'Z','o','n','e','0','2',' ',' ',6,
																							 'Z','o','n','e','0','3',' ',' ',6,
																							 'Z','o','n','e','0','4',' ',' ',6,
																							 'Z','o','n','e','0','5',' ',' ',6,
																							 'Z','o','n','e','0','6',' ',' ',6};
const Tel_Memory_S F_Tel_MemoryName[4] @(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) +3 + ZoneNum*sizeof(ZoneNameS)) ={
																							'A','l','l','e','n',' ',' ',' ',0,
																							 'M','e','m','o','r','y','2',' ',0,
																							 'N','i','c','o',' ',' ',' ',' ',0,
																							 'M','e','m','o','r','y','4',' ',0};
																							
const DTMF_Key T_DTMF_Key @(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) +3 + ZoneNum*sizeof(ZoneNameS) + 4*sizeof(Tel_Memory_S)) = {1,1,12,0,0,0,3};

const uchar F_Scenarioshortcut @(EE_Scenario_CCTV + 18*sizeof(Scenario_Setting) + 1) = 0x01;																						//+ 3 is for zone alarm enable or not

const uchar F_PCM_Number @(EE_Scenario_CCTV + 18*sizeof(Scenario_Setting) + 2) = 0x01;
const uchar F_PCM_Door_Open_Time @(EE_Scenario_CCTV + 18*sizeof(Scenario_Setting) + 3) = 0x05;

const Scenario_Setting F_ScenarioPara[18] @(EE_Scenario_CCTV) = {0x0915,0x01,'G','o','o','d',' ','B','y','e',8,0x32,
															0x0915,0x02,'W','e','l','c','o','m','e',' ',7,0x82,
															0x0915,0x03,'R','e','l','a','x',' ',' ',' ',5,0x42,
															0x0915,0x04,'R','e','l','a','x',' ',' ',' ',5,0x12,
															0x0915,0x05,'H','o','l','i','d','a','y',' ',7,0x52,
															0x0915,0x06,'N','i','g','h','t',' ',' ',' ',5,0x62,
															0x0915,0x07,'D','a','y',' ',' ',' ',' ',' ',3,0x72,
															0x0915,0x08,'A','l','l',' ','U','P',' ',' ',6,0x22,
															0x0915,0x09,'A','l','l',' ','D','o','w','n',8,0x92,															
	
															0x0915,0x01,'G','o','o','d',' ','B','y','e',8,0x30,
															0x0915,0x02,'W','e','l','c','o','m','e',' ',7,0x80,
															0x0915,0x03,'R','e','l','a','x',' ',' ',' ',5,0x70,
															0x0915,0x04,'R','e','l','a','x',' ',' ',' ',5,0x60,
															0x0915,0x05,'R','e','l','a','x',' ',' ',' ',5,0x50,
															0x0915,0x06,'R','e','l','a','x',' ',' ',' ',5,0x40,
															0x0915,0x07,'R','e','l','a','x',' ',' ',' ',5,0x30,
															0x0915,0x08,'R','e','l','a','x',' ',' ',' ',5,0x20,
															0x0915,0x09,'R','e','l','a','x',' ',' ',' ',5,0x11
															};


const CCTV_Setting  F_CCTV_Para[4] @(EE_CCTV_Room) = {0x0915,0x0a,'C','C','T','V','1',' ',' ',' ',5,	
													0x0915,0x0b,'C','C','T','V','2',' ',' ',' ',5,	
													0x0915,0x0c,'C','C','T','V','3',' ',' ',' ',5,	
													0x0915,0x0d,'C','C','T','V','4',' ',' ',' ',5};


const Room_Setting F_RoomType[12] @(EE_CCTV_Room + sizeof(F_CCTV_Para)) ={1,'K','i','t','c','h','e','n',' ',7,
																		2,'H','R','B',' ',' ',' ',' ',' ',3,
																		3,'K','N','X',' ',' ',' ',' ',' ',3,
																		4,'L','a','u','n','d','y',' ',' ',6,
																		5,'D','i','n','i','n','g',' ',' ',6,
																		6,'G','a','r','d','e','n',' ',' ',6,
																		7,'R','e','l','a','x',' ',' ',' ',5,
																		8,'S','t','a','i','r',' ',' ',' ',5,
																		9,'R','e','l','a','x',' ',' ',' ',5,
																		10,'G','y','m',' ',' ',' ',' ',' ',3,
																		11,'D','r','e','s','s','i','n','g',8,
																		12,'C','a','r','p','o','r','t',' ',7
																		};

const Timer_Event F_Timer_Event[8] @(EE_Time_Event) = {0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,													
													0,0,0,0,0,0,0,0,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0
};
/* 
** =================================================================== 
**     Method      :  HCS08_EE_Init  
** 
**     Description : 
**         fFCLK initialization code for FLASH programming and erasing. 
**         fFCLK must be within the range from 150 kHz to 200 kHz.  
**         After this initialization methdo, fFCLK is always arround  
**         175 kHz, which is within the range of specification. 
**     Patameters: 
**          no. 
**     Return Values: 
**          no 
** =================================================================== 
*/ 


void HCS08_EE_Init(void) 
{ 
/**  
** fFCLK setting 
** FCDIV: 7 6 5 4 3 2 1 0 
**        | | |          
**        | | +DIV[5:0]: fFCLK = fBUS / (8exp(PRDIV8) * DIV +1) 
**        | +PRDIV8: Prescale by 8, 1: input to the FLASH clock divider is **                    
the bus rate clock divided by 8. 
**        +DIVLD: divisor load status, 1: has been written since reset. 
*/ 
if(!FCDIV_DIVLD) 
   { 
      if(HCS08_BUS_FREQUENCY >= 12000) 
      { 
          FCDIV = (((HCS08_BUS_FREQUENCY / (8*175))) | 0x40) - 1; 
      } 
      else 
      { 
          FCDIV = (HCS08_BUS_FREQUENCY / 175) - 1; 
      } 
   } 
} 

/* ===================================================================*/ 
// Array of opcode instructions of the Erase/Program function in  
// the HCS08 family  
// size = 40 bytes  
const unsigned char ROM_PGM[] = 
{ 
  0x9E, 0xFE, 0x03, // LDHX 3,SP   ; get the target address '_addr' 
/* PC is pushed when calling, 2 bytes used (SP is decreased by 2) 
** SP is now pointing to the next available unit 
** thus '_addr' is at [SP+3] 
*/ 
  0xF7,               // STA ,X      ; store 'data' to 'addr'  
  0xA6, 0x20,           // LDA #$20    ; load FCMD, CMD should be changed before calling 
  0xC7, 0x18, 0x26,     // STA $1826   ; FCMD 
  0x45, 0x18, 0x25,     // LDHX #$1825 ; address of FSTAT 
  0xF6,                   // LDA ,X       ; load FSTAT to A 
  0xAA, 0x80,            // ORA #$80     ; set FCBEF to lunch the command 
  0xF7,                   // STA ,X       ; store back to FSTAT 
  0x9D,                   // NOP           ; wait for 4 cycles 
  0x9D,                   // NOP 
  0x9D,                   // NOP 
  0x9D,                   // NOP 
  0x45, 0x18, 0x25,     // LDHX #$1825 ; check FACCERR and FPVIOL 
  0xF6,                   // LDA ,X 
  0xA5, 0x30,            // BIT #$30 
  0x27, 0x04,            // BEQ *+6     ; branch to label_1 
  0xA6, 0xFF,            // LDA #$FF    ; return 0xFF if flag error 
  0x20, 0x07,            // BRA *+9     ; branch to label_2  
//label_1:   
  0xC6, 0x18, 0x25,   // LDA $1825   ; wait for command to be completed 
  0xA5, 0x40,            // BIT #$40 
  0x27, 0xF9,            // BEQ *-5     ; branch to label_1 
//label_2:   
  0x81                    // RTS          ; return 
}; 
/* 
** =================================================================== 
**     Method      :  flash_byte_prog  
** 
**     Description : 
**         flash_byte_prog programs one byte of data to the specified  
**         address of FLASH. this is an internal method only used by API. 
**     Patameters: 
**          unsigned int,       -- target address to be programmed 
**          unsigned char.      -- data to be programmed 
**     Return Values: 
**          no 
** =================================================================== 
*/ 

static 
void flash_byte_prog(unsigned int _addr, unsigned char _data) 
{ 
  unsigned char _pgm[sizeof(ROM_PGM) + 2]; 
 
  // save '_addr' 
  _pgm[0] = (unsigned char)((_addr & 0xff00) >> 8); 
  _pgm[1] = (unsigned char)(_addr & 0x00ff); 
 
  // copy the code to stack (local variable _pgm[])  
  if(!_pgm1)
  {
  	(void)memcpy(_pgm + 2, ROM_PGM, sizeof(ROM_PGM)); 
	//_pgm[34] = 0x81;
  }
  _pgm1 = 1;
  _pgm[7] = FLASH_CMD_BURST_BYTEPROG; // change FCMD value 
  //_pgm[7] = FLASH_CMD_BYTEPROG; // change FCMD value 
 //FCMD = FLASH_CMD_BURST_BYTEPROG;
  //DisableInterrupts; 
 
  if (FSTAT&0x10){               // Check to see if FACCERR is set 
      FSTAT = FSTAT | 0x10;     // write a 1 to FACCERR to clear 
  } 
  __asm 
  { 
    LDA _data     // pass '_data' thru A 
    TSX           // transfer SP to HX 
    JSR 2,X       // first 2 bytes of _pgm[] are '_addr' 
  } 
 
  //EnableInterrupts; 
} 
/* 
** =================================================================== 
**     Method      :  flash_page_erase  
** 
**     Description : 
**         flash_page_erase erases one page that contains the specified  
**         address of FLASH. this is an internal method only used by API. 
**     Patameters: 
**          unsigned int.    -- target address to be erased 
**     Return Values: 
**          no 
** =================================================================== 
*/ 
/*
unsigned char ProgFlash(unsigned int _address,unsigned int x)
{
	//unsigned char *far ptr;
	unsigned char *ptr;
	unsigned iii;
unsigned char result;
	ptr = (unsigned char *far) _address;
	//ptr = (unsigned char *)_address;
	//ptr = (unsigned char *far)&LEDFlag;
        FSTAT = FSTAT_FACCERR_MASK | FSTAT_FPVIOL_MASK; //reset error flags

//write to Flash 
	for(iii = 0; iii<x;iii++)
	{
		//HCS08_EE_Init();
		while(!FSTAT_FCBEF);
		        *ptr = SaveData[iii];         //data does not matter
		     ptr++;
		//write erase command to FMCD
		        FCMD =  ByteProgBurst;

		//write 1 to FCBEF to launch command and to clear FCBEF
		        FSTAT_FCBEF = 1;
		       
		  asm
		  {
		  nop
		                nop
		                nop
		                nop
		        };              //wait at least 4 clock cycles

		  //If FPVIOL or FACCERR in FSTAT are set, then quit
		  if ((FSTAT_FPVIOL == 1)||(FSTAT_FACCERR == 1))
		    {
		    result = !OK;
			return(result);
		    }
		 
		//  else
		//    {
		//else loop till FCCP is set
		               
		//        };
		//for (;FSTAT_FCCF == 0;);
        }

  	 for (;FSTAT_FCCF == 0;);
                result = OK;             //signal OK
        return(result);
};*/
static 
void flash_page_erase(unsigned int _addr) 
{ 
  unsigned char _pgm[sizeof(ROM_PGM) + 2]; 
 
  // save '_addr' 
  _pgm[0] = (unsigned char)((_addr & 0xff00) >> 8); 
  _pgm[1] = (unsigned char)(_addr & 0x00ff); 
 
  // copy codes to stack (local variable _pgm[]) 
  (void)memcpy(_pgm + 2, ROM_PGM, sizeof(ROM_PGM)); 
 
  _pgm[7] = FLASH_CMD_PAGEERASE;// change FCMD value 
 
  DisableInterrupts;
  if (FSTAT&0x10){              // Check to see if FACCERR is set 
      FSTAT = FSTAT | 0x10;     // write a 1 to FACCERR to clear 
  } 
  __asm 
  { 
    TSX           // transfer SP to HX 
    JSR 2,X       // first 2 bytes of _pgm[] are '_addr' 
  } 
  EnableInterrupts; 
} 

/* 
** =================================================================== 
**     Method      :  flash_byte_read  
** 
**     Description : 
**         flash_byte_read reads one byte data from the specified  
**         address of FLASH. this is an internal method only used by API. 
**     Patameters: 
**          unsigned int.         -- target address to be erased 
**     Return Values: 
**          unsigned char.        -- the data 
** =================================================================== 
*/ 
static 
unsigned char flash_byte_read(unsigned int _addr) 
{ 
    return *(unsigned char *)_addr; 
} 
/*-------------------------------------------------------------
 function:    get ini information
 
---------------------------------------------------------------*/
void  Read_IniData(unsigned char *buf) {
      unsigned int addre=EE_Page1;
      uchar i=0;
      for(i=0;i < sizeof(Setting) + 2;i++) {
         *buf++=flash_byte_read(addre++);
      }  
}
/*
void  Read_LEDFlag(unsigned char *buf)
{
  unsigned int addre=EE_PageLED;
  uchar i=0;
  for(i=0;i<1;i++) {
    *buf++=flash_byte_read(addre++);
  }
}*/
//----------------------------------------------------------------
void CopyPage(unsigned int _DstPage) {
	unsigned int i;
	unsigned int j=243;
	unsigned int addre= _DstPage;
	if(_DstPage == EE_Page1)
	{
		j=sizeof(Setting) + 2;
	}
	//else if(_DstPage > EE_PageCopy2)
	else if(_DstPage > EE_PageCopy1)
	{// for home automation we need 336 bytes for a page
		j = 336;

	}

	for(i=0;i<j;i++) {
		SaveData[i]= flash_byte_read(addre++);

	}


}
//----------------------------------------------------------------
void InserData(unsigned char *_buf,unsigned int _index,unsigned int _len) {
	 unsigned int i;
	 for(i=0;i<_len;i++)
	 {
		SaveData[_index+i]=*_buf++;
	 }
}
//----------------------------------------------------------------
void WritePage( unsigned int _DstPage) {
      unsigned int i;
      unsigned int j=243;
       __RESET_WATCHDOG();
       //DisableInterrupts;
       if(_DstPage < EE_Page1) // for save 
	   	return;
       flash_page_erase(_DstPage);
       //__RESET_WATCHDOG();
      //rewrite
        
	if(_DstPage ==EE_Page1) {
		j = sizeof(Setting) + 2;
	}
	else if(_DstPage == EE_Time_Event)
	{
		j = sizeof(Timer_Event)*8;
	}
	//else if(_DstPage > EE_PageCopy2)
	else if(_DstPage > EE_PageCopy1)
	{
		j = 336;

	}  
	   
	   
      DisableInterrupts; 
	 //(void)memcpy(_pgm + 2, ROM_PGM, sizeof(ROM_PGM)); 
	 _pgm1 = 0;
      for(i=0;i<j;i++){
	  	//LED_MSG = ~LED_MSG;
       flash_byte_prog(_DstPage+i,SaveData[i]);
	   	//LED_MSG = ~LED_MSG;
       __RESET_WATCHDOG();
      }
      //Nop; 
      EnableInterrupts;
      
}
/*------------------------------------------------------
erase the target page date
*/
void ErasePage( unsigned int _DstPage)
{
	flash_page_erase(_DstPage);
}


/*---------------------------------------------------------
 write some byte
---------------------------------------------------------*/
void WriteData(unsigned int _DstPage,unsigned char *_buf,unsigned int _index,unsigned int _len) {
  //uchar data[25];  //512
	CopyPage(_DstPage);
	//flash_page_erase(_DstPage);
	InserData(_buf,_index,_len);
	//write data to the dest page
	//WritePage(_DstPage);
	if(_DstPage == EE_Page1)
	{
		//SaveData[TargerNumPosition] = 1; //target page is page1
		WritePage(_DstPage);
		return;
	}
	else if(_DstPage == EE_Page2)
	{
		SaveData[TargerNumPosition] = TargetPage2; //target page is page2	
	}
	else if(_DstPage == EE_Page3)
	{
		SaveData[TargerNumPosition] = TargetPage3; //target page is page3	
	}
	else if(_DstPage == EE_Page4)
	{
		SaveData[TargerNumPosition] = TargetPage4; //target page is page4	
	}
//else if(_DstPage == EE_Page5)
//	{
//		SaveData[TargerNumPosition] = TargetPage5; //target page is page5	
//	}
	else
	{
		WritePage(_DstPage);
		return;
	}
	SaveData[TargerFlagPosition] = 0x51;
	WritePage(EE_PageCopy1);
  
}
/*------------------------------------------------------------------
ReadRecord information
--------------------------------------------------------------------*/
/*
void ReadRecordData(unsigned int _DstPage,unsigned char *_buf,unsigned int _index,unsigned char _len) {
         unsigned int i;
         unsigned int addre=_DstPage+_index;
         //CopyPage(_DstPage);
         for(i=0;i<_len;i++) {
            *_buf++=flash_byte_read(addre++);
         }
    
}
*/