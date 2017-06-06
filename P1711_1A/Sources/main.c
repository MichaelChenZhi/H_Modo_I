

#include <hidef.h> /* for EnableInterrupts macro */
#include "MC9S08AW60.h"                /* I/O map for MC9S08AW16CFU */
#include "define.h"
#include "Flashwrite.h"

#define MAIN
#include "global.h"
#include "Ges_der.h"
#include "Driv_der.h"
#include "Trame.h"
#include "Ges_scan.h"
//#include "Touch.h"
#include "VideoStar.h"
#include "HomeAutomation.h"
#include "Spi.h"
#include "AlarmSystem.h"
#include "Eeprom.h"
void PCM_Audio_Set(void);
void ReceiveDataAck(void);
void Frame_HRB_Verify(void);
extern void Gest_Alarm(void);
extern void InitFlash( void );
extern void Switch_Off(void);
extern void BlueToothScan(void);
//extern void Calculate_Volume_and_update_Conexant(unsigned char Volume_to_Buttons);
void SwitchTalkToCallModule(void);
void SwitchTalkToSwitchBoard(void);

//#pragma DATA_SEG DEFAULT
extern const uchar FlagPage1;
extern const uchar F_Scenarioshortcut;
extern const uchar F_PCM_Number;
extern const uchar  F_PCM_Door_Open_Time;
//extern const uchar LEDFlag;
//extern const uchar Recordlenth;
extern void Play_New_Melody(unsigned char kind);
extern void S_OUT_CALL_FUNCTION(uchar p1,uchar p2, uchar p3,uchar p4);
//extern void  Read_LEDFlag(unsigned char *buf);
extern void DefaultSetting(unsigned char p1,unsigned char p2);
extern void Presence_Absence_Management(void);
extern void BeepOK(void);
extern void KeyHandle(void);
//extern void SaveA_Vsetting(void);
extern void SaveInfQueue(void (*finProc)());
extern void SavePhoneBookRecordQueue(void);
//extern void DeletePhoneBookQueue(void);
extern void SaveAudioPara(void);
//extern void SaveVideoPara(void);
extern void DefaultSave(void);
extern void FlashEvent(void);
extern void DTMF_Management(void);
extern void Telephone_Call_Detect(void);
//void Refresh_Unused_Pin_State(void);
extern void ExitToWaitPage(void);
void SaveEvent(void);
extern void JD_Frame_RC(void);
//void Gest_Conexant_Serial_Link(void);
//unsigned char Hex_2_Ascii (unsigned char val);
//void Update_OKI_Init_Values_File(void);
extern void Gest_Panic(void);
void Gest_tone(void);
void DataInit(void);
#ifdef PCB20150127
void SPI_init(void);
#endif
void External_Button_Scan(void);
uchar Ex_Module_StateManagement(void);

void SettoWaitPage(void);
void F_Back_Menu(void);

void SwitchTalkMenu(void);
void InitSlaveCpu(void);
extern void Telephone_Number_Push(void);
uchar Rc_Tel_Verify(void);
uchar StartEndFlag;


typedef struct
  {
  unsigned int  period_f0;      // tone F0
  unsigned int  numper_f0_on;   // number of periods tone on
  unsigned int  numper_f0_off;  // number of periods tone off
  unsigned int  period_f1;      // tone F1
  unsigned int  numper_f1_on;   // number of periods tone on
  unsigned int  numper_f1_off;  // number of periods tone off
  } par_tone;

typedef struct
  {
  unsigned int  t_rep;          // tone F0                                [ms]
  unsigned char num_door_openk_biton;  // number of packet bitonal
  unsigned char num_seq_biton;  // number of seqauenze packet bitonal
  unsigned int  pause_biton;    // pause between packet bitonal           [ms]
  unsigned int  pause_sequence; // pause between sequence packet bitonal  [ms]
  } par_chiam_da;

  
// tone A
#define A_FREQ_0                    520L                      // [Hz]
#define A_TIME_F0_ON               40L                       // [ms]
#define A_TIME_F0_OFF              0L                        // [ms]
#define A_FREQ_1                    600L                      // [Hz]
#define A_TIME_F1_ON               40L                       // [ms]
#define A_TIME_F1_OFF              0L                        // [ms]

// tone B
#define B_FREQ_0                    1563L                     // [Hz]
#define B_TIME_F0_ON               50L                       // [ms]
#define B_TIME_F0_OFF              0L                        // [ms]
#define B_FREQ_1                    1953L                     // [Hz]
#define B_TIME_F1_ON               50L                       // [ms]
#define B_TIME_F1_OFF              0L                        // [ms]

// tone C
#define C_FREQ_0                    1953L                     // [Hz]
#define C_TIME_F0_ON               50L                       // [ms]
#define C_TIME_F0_OFF              40L                       // [ms]
#define C_FREQ_1                    1563L                     // [Hz]
#define C_TIME_F1_ON               50L                       // [ms]
#define C_TIME_F1_OFF              80L                       // [ms]

// tone D
#define D_FREQ_0                    710L                      // [Hz]
#define D_TIME_F0_ON               100L                      // [ms]
#define D_TIME_F0_OFF              0L                        // [ms]
#define D_FREQ_1                    868L                      // [Hz]
#define D_TIME_F1_ON               100L                      // [ms]
#define D_TIME_F1_OFF              0L                        // [ms]

// tone E
#define E_FREQ_0                    600L                      // [Hz]
#define E_TIME_F0_ON               50L                       // [ms]
#define E_TIME_F0_OFF              0L                        // [ms]
#define E_FREQ_1                    1302L                     // [Hz]
#define E_TIME_F1_ON               50L                       // [ms]
#define E_TIME_F1_OFF              0L                        // [ms]

// tone F  (tone Courtesy)
#define F_FREQ_0                    2000L                     // [Hz]
#define F_TIME_F0_ON               50L                       // [ms]
#define F_TIME_F0_OFF              0L                        // [ms]
#define F_FREQ_1                    2000L                     // [Hz]
#define F_TIME_F1_ON               50L                       // [ms]
#define F_TIME_F1_OFF              0L                        // [ms]

// tone G  (tone Dissuasion)
#define G_FREQ_0                    2000L                     // [Hz]
#define G_TIME_F0_ON               50L                       // [ms]
#define G_TIME_F0_OFF              0L                        // [ms]
#define G_FREQ_1                    2000L                     // [Hz]
#define G_TIME_F1_ON               50L                       // [ms]
#define G_TIME_F1_OFF              0L                        // [ms]

//tone H (tone Long)
#define H_FREQ_0                    2000L         // [Hz]
#define H_TIME_F0_ON               100L            // [ms]
#define H_TIME_F0_OFF              0L             // [ms]
#define H_FREQ_1                    2000L         // [Hz]
#define H_TIME_F1_ON               100L            // [ms]
#define H_TIME_F1_OFF              0L             // [ms]

const par_tone tone_parameters[MAX_TONES] = {
// tone A
  BUSCLK / A_FREQ_0,                          // tone F0 (period)
  (A_TIME_F0_ON * A_FREQ_0) / 1000L,          // number of periods tone on
  (A_TIME_F0_OFF * A_FREQ_0) / 1000L,         // number of periods tone off
  BUSCLK / A_FREQ_1,                          // tone F1 (period)
  (A_TIME_F1_ON * A_FREQ_1) / 1000L,          // number of periods tone on
  (A_TIME_F1_OFF * A_FREQ_1) / 1000L,         // number of periods tone off

// tone B
  BUSCLK / B_FREQ_0,
  (B_TIME_F0_ON * B_FREQ_0) / 1000L,
  (B_TIME_F0_OFF * B_FREQ_0) / 1000L,
  BUSCLK / B_FREQ_1,
  (B_TIME_F1_ON * B_FREQ_1) / 1000L,
  (B_TIME_F1_OFF * B_FREQ_1) / 1000L,

// tone C
  BUSCLK / C_FREQ_0,
  (C_TIME_F0_ON * C_FREQ_0) / 1000L,
  (C_TIME_F0_OFF * C_FREQ_0) / 1000L,
  BUSCLK / C_FREQ_1,
  (C_TIME_F1_ON * C_FREQ_1) / 1000L,
  (C_TIME_F1_OFF * C_FREQ_1) / 1000L,

// tone D
  BUSCLK / D_FREQ_0,
  (D_TIME_F0_ON * D_FREQ_0) / 1000L,
  (D_TIME_F0_OFF * D_FREQ_0) / 1000L,
  BUSCLK / D_FREQ_1,
  (D_TIME_F1_ON * D_FREQ_1) / 1000L,
  (D_TIME_F1_OFF * D_FREQ_1) / 1000L,

// tone E
  BUSCLK / E_FREQ_0,
  (E_TIME_F0_ON * E_FREQ_0) / 1000L,
  (E_TIME_F0_OFF * E_FREQ_0) / 1000L,
  BUSCLK / E_FREQ_1,
  (E_TIME_F1_ON * E_FREQ_1) / 1000L,
  (E_TIME_F1_OFF * E_FREQ_1) / 1000L,

// tone F  (tone Courtesy)
  BUSCLK / F_FREQ_0,
  (F_TIME_F0_ON * F_FREQ_0) / 1000L,
  (F_TIME_F0_OFF * F_FREQ_0) / 1000L,
  BUSCLK / F_FREQ_1,
  (F_TIME_F1_ON * F_FREQ_1) / 1000L,
  (F_TIME_F1_OFF * F_FREQ_1) / 1000L,

// tone G  (tone Dissuasion)
  BUSCLK / G_FREQ_0,
  (G_TIME_F0_ON * G_FREQ_0) / 1000L,
  (G_TIME_F0_OFF * G_FREQ_0) / 1000L,
  BUSCLK / G_FREQ_1,
  (G_TIME_F1_ON * G_FREQ_1) / 1000L,
  (G_TIME_F1_OFF * G_FREQ_1) / 1000L,

// tone H long beep
 BUSCLK / H_FREQ_0,
  (H_TIME_F0_ON * H_FREQ_0) / 1000L,
  (H_TIME_F0_OFF * H_FREQ_0) / 1000L,
  BUSCLK / H_FREQ_1,
  (H_TIME_F1_ON * H_FREQ_1) / 1000L,
  (H_TIME_F1_OFF * H_FREQ_1) / 1000L,
};

const par_chiam_da call_from_parameters[MAX_CALLS] = {
// 0 - main
  2296,                                        // tone_t_rep            
  1,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  0,                                           // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 1 - Secondary
  396,                                         // tone_t_rep            
  5,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  200,                                         // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 2 - Switchboard
  96,                                          // tone_t_rep             
  3,                                           // tone_num_door_openk_biton
  5,                                           // tone_num_seq_biton
  50,                                          // tone_door_openuse_bitonal
  200,                                         // tone_door_openuse_sequence

// 3 - Intercom
  496,                                         // tone_t_rep             
  3,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  500,                                         // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 4 - Floor call 
  2296,                                        // tone_t_rep             
  1,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  0,                                           // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 5 - Courtesy
  96,                                          // tone_t_rep             
  2,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  100,                                         // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 6 - Dissuasion
  96,                                          // tone_t_rep             
  4,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  100,                                         // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 7 - Short    
  96,                                          // tone_t_rep             
  1,                                           // tone_num_door_openk_biton
  0,                                           // tone_num_seq_biton
  0,                                           // tone_door_openuse_bitonal
  0,                                           // tone_door_openuse_sequence

// 8 - Soft
  1,                                              // note_t_rep            (MUST BE 2ms LESS THAN (F_TIME_F0_ON + F_TIME_F0_OFF + F_TIME_F1_ON + F_TIME_F1_OFF)
  1,                                              // pack_number_for_bi_tone_note
  0,                                              // seq_number_for_bi_tone_note
  2,                                              // note_pause_bi_tone
  0,                                              // note_pause_sequence

};
//---------------------------------------------------


//unsigned int time_debug;
//#pragma CODE_SEG ROM

void main(void) {
	MCU_init(); /* call Device Initialization */

	#ifdef PCB20150127
	SPI_init();
	#endif
	/* include your code here */
	DataInit();
	Init_var();

	Init_time_base();
	HCS08_EE_Init();	//InitFlash();
	HW_Protocol_Init();
	Signal(proc_scan, INIT_PROC); 
	TEST_Phase_Active = 0;	
  //------- led update upon state of flash --
	Door_State = OFF;  // Switch off lo state door
	//PageStatus = WaitPage;


	//TC_PCM1 = ON;

	//TC_PCM1 = OFF;
	
	//SS= OFF;
	//while( !SPI1S_SPTEF);SPI1D = 0xA5;
  	//while( !SPI1S_SPRF);Rc_Tel_num[0] = SPI1D;            // svuoto
	//SS= ON;

	Courtesy_tone();
	
	InitSlaveCpu();	

  for (;;)
    {
    	__RESET_WATCHDOG(); // by default, WD (COP) is disabled with device init. When enabling, also reset the watchdog.
    //!!!!!! Refresh_Unused_Pin_State();
	//Refresh_Unused_Pin_State();
    	Derivation_Frame_Management();      
    	GestProt_der();                  
    	Timers_Management();
	States_Management();             
    	Tone_Management();               
    	IO_Scan_Management();            

	SaveEvent();
	JD_Frame_RC();
    	KeyHandle();
	JD_TX_Managment();

	 FlashEvent();

	// If test mode is active
    //if(TEST_Phase_Active){
   	//Potentiometer_Sequence();
    //	} //TEST mode 
	DTMF_Management();
	Telephone_Call_Detect();
	JD_CallBack();
	//Timer enent hanld when time is reach according the setting
	TimeEvent();
	CheckRecount_Timer();
	KNX_TX_Managment();
	KNX_RC_Managment();
	BlueToothScan();// BT connect we need change the baudrate for connect 1039-56
	External_Button_Scan();
	Rc_Tel_Num();
    	_Wait;
    } /* loop forever */
  /* please make sure that you never leave main */
}


// ----------------------------------------------------------------------------
void Init_var(void)
{

  timer_sec = TIME_1_SEC;
  b1 = '0';

  kind_of_ring_save = 0;
  call_from = CALL_FROM_MAIN;
  //state_door_opennic = RELEASED;
  
//	state_puls = 0;                         
  //cnt_debounce_puls = 0;
  //timer_persistence = 0;
	TR_2 = ON;	
  open_door_button = 0;
  number_SVC = 0;
  timer_delay_offhook = 0;
  hook_state_toggled = ON_HOOK;
  
//  timer_audio_poweron = 0;
  MODO_State = IDLE;
  Sub_MODO_State = EX_IDLE;
  OLD_MODO_State = IDLE;
  OLD_Sub_Modo_State = EX_IDLE;
//  ST_Audio = POW_0;
  
  
  SwitchedOnOff = 0;  
  
  // Load current values from flash
  
	//R_presence_absence = F_presence_absence;
	//R_messages_presents = F_messages_present;
	
	//R_pot_LUM = F_pot_LUM;
	//R_pot_COL = F_pot_COL;
  R_Audio_Volume = F_Audio_Volume;  
  R_Volume_Call = F_Volume_Call; 
  R_kind_of_ringtone_cito = F_kind_of_ringtone_audioonly;
  R_kind_of_ringtone_floor = F_kind_of_ringtone_floor;
  R_Led_mute = F_Led_mute;
  //Button_for_dentist = F_Button_for_dentist;
   
  ROOM_MONITOR_IN_PROGRESS = 0;  
  
  //timer_logo = 0;
  
  MIC_Muted = OFF;
	switch_on_button = RELEASED;
  
  Main_GUI_locked = 0;
  Call_GUI_locked = 0;
  Timer_GUI_locking = 0;
  timer_proc_scan = TIME_SCAN_BUTTONS;
  Timer_Wait_Tone = 0;
     
  
  //Calculate_Volume_and_update_Conexant(R_Audio_Volume);
  


  Timer_SHSTC = 0;

  //DAaudio_In_Progress = 0;
	JD_start_wait = 20;
    StartEndFlag=1;
  
  
  //OKI_State = OKI_IDLE; 
  ICON = 0x01;
  OldAudioPhone_Hook = AudioPhone_HOOK;
  //OKI_Current_Profile = IPERVOICE;

}

void Theme_Ini(void)
{
	if (SettingPara.Theme == 1)
			MainMenu = MainMenu_1;
		else if (SettingPara.Theme == 2)
			MainMenu = MainMenu_2;
		else if (SettingPara.Theme == 3)
			MainMenu = MainMenu_3;
		else if (SettingPara.Theme == 4)
			MainMenu = MainMenu_4;	
		else MainMenu = MainMenu_1;
}
//---------------------------------------------------------------
#ifdef PCB20150127
void SPI_init(void)
{
	 SPI1BR=0x24;   //spi baud rate 384K
	 //SPI1BR=0x77;
	SPI1C1=SPI1C1_SPE_MASK | SPI1C1_MSTR_MASK;;
	SPI1C2=0x00;
}
#endif



//-------------------------------------------------------------------
void DataInit(void)
{
	//data init read data from flash
	
		uchar  data[24]={ 0
		};
		Read_IniData(data);
		
		if(data[0]==0x55 && data[1]==0xaa)
		{
			//have already set before
			(void)memcpy(&SettingPara,&data[2],sizeof(Setting));  	

		}
		else
		{		
			DefaultSetting(0x00,0x00);
			//Flag3.all=0;			
		}
	
		
		R_Volume_Call = SettingPara.CallVolume;
		R_kind_of_ringtone_cito = F_kind_of_ringtone_audioonly;
		R_kind_of_ringtone_floor = F_kind_of_ringtone_floor;
		R_kind_of_ringtone_PSTN = F_kind_of_ringtone_Pstn;
		  Flag.all=0;
		  //Flag3.all=0;
		  Flag4.all=0;
	  
	  
	  
		if(F_AutomaticDoorOpener){
				
			Button_for_dentist = DENTIST_BUTTON_PRESSED;
		}
		else
		{
			Button_for_dentist = DENTIST_BUTTON_NOT_PRESSED;
		}
		//if(SettingPara.Presence_Absence)
		//	{
		//		TouchPanelPara.T_LED_Absent=4;
		//	}
		R_presence_absence = ABSENCE_OFF;
		if(F_Presence_Absence)
		{		  			
			R_presence_absence = ABSENCE_ON;
		}
		/*
		F_messages_present = OFF;
		if(Flag3.bits.message)
		{
			//Flag3.bits.message=1;
			R_messages_presents = ON;
			F_messages_present = ON;
		}
		*/
		R_Led_mute = F_Led_mute;
		SystemStatus.all=0;

		Theme_Ini();	
		SettoWaitPage();
		F_Back_Menu();
		  Old_ENA_LCD = 0;
			//no metter what happen, the first power on we have to sync the parameter with videostar

		//LastKeyValue=No_key;
		JD_Save_Frame_Index = 0;
		JD_TX_Frame_Index = 0;
		JD_TX_Frame_Time = 0;
		SaveInf = NULL;
		F_Old_Pres_BT = Pres_BT;
		Tel_Det_State = Tel_Idle;
		A_password = (Password_Alarmsystem *)(EE_Page_AlarmSystem + SpaceByte);
		AlarmP = (AlarmSystem *)(EE_Page_AlarmSystem + SpaceByte + sizeof(Password_Alarmsystem));
		JDFunctionCallBack = NULL;
		JDFunctionCallBack1 = NULL;
		JDFunctionCallBack2 = NULL;
		//HideState = MenuNotHide;
	//ringtone_type = F_Call_Ringtone;                   // ringtone type

}
// ----------------------------------------------------------------------------
void Init_time_base(void)
{
	TPM1C0V = TPM1CNT + COUNTER_2MS;

}

// ----------------------------------------------------------------------------
void Setup_tone(void)
{
const par_tone *pn;
const par_chiam_da *pc;
unsigned int b;
  pn = &tone_parameters[kind_of_ring-1];
  period_f0    = pn->period_f0;
  numper_f0_on  = pn->numper_f0_on;
	if((TEST_Phase_Active) && (kind_of_ring == 7)){  // During test it is neede a longer "short tone"
	  numper_f0_on  *= 20;
	}
  numper_f0_off = pn->numper_f0_off;
  period_f1    = pn->period_f1;
  numper_f1_on  = pn->numper_f1_on;
  numper_f1_off = pn->numper_f1_off;

  pc = &call_from_parameters[call_from];

  tone_t_rep          = pc->t_rep;        
  tone_num_door_openk_biton  = pc->num_door_openk_biton; 
  tone_num_seq_biton  = pc->num_seq_biton;     
  tone_door_openuse_bitonal = pc->pause_biton;      
  tone_door_openuse_sequence = pc->pause_sequence;  

  TPM2C0SC = TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0A_MASK;       
  TPM2MOD = period_f0;
//  if (kind_of_ring <= MAX_SUONERIE)
//    {  // for le suonerie il volume è max. --> duty = 50%
//   period_f0_on = period_f0 / 2;
//   period_f1_on = period_f1 / 2;
 
// 
//     }
//    else
//      {  // for i toni of courtesy il volume è min. --> duty = 2%
//        period_f0_on = period_f0 / 2;
//        period_f1_on = period_f1 / 2;
// // 
// // //     b1 = (unsigned char)((period_f0 * 2) / 100);
// // //     period_f0_on = period_f0 - b1;
// // //     b1 = (unsigned char)((period_f1 * 2) / 100);
// // //     period_f1_on = period_f1 - b1;
// // 
//      }  // endif
	 if (kind_of_ring <= MAX_RINGTONES)
    {  // for the ringtone the max volume is max. --> duty = 50%
    /*
    F0_ON_period = F0_period / 2;
    F1_ON_period = F1_period / 2;
    */
    	
    switch(R_Volume_Call) {
	case 0:	//duty = 1%
		 b = (period_f0 ) / 100;
	        period_f0_on = period_f0 - b;
	        b = (period_f1 ) / 100;
	        period_f1_on = period_f1 - b;
        break;
        case 1:  //duty = 2%
            b = (period_f0 ) / 50;
            period_f0_on = period_f0 - b;
            b = (period_f1 ) / 50;
            period_f1_on = period_f1 - b;
        break;
        case 2: 
		/*	//duty = 4%
            b = (period_f0 ) / 25;
            period_f0_on = period_f0 - b;
            b = (period_f1 ) / 25;
            period_f1_on = period_f1 - b;
            */
            b = (period_f0 ) / 33;
            period_f0_on = period_f0 - b;
            b = (period_f1 ) / 33;
            period_f1_on = period_f1 - b;
        break;
        case 3: 
			/*//duty = 8%
            b = (period_f0 *2) / 25;
            period_f0_on = period_f0 - b;
            b = (period_f1*2 ) / 25;
            period_f1_on = period_f1 - b;
            */
            b = (period_f0 ) / 16;
            period_f0_on = period_f0 - b;
            b = (period_f1 ) / 16;
            period_f1_on = period_f1 - b;
        break;
        case 4: //duty = 10%
            b = (period_f0 ) / 10;
            period_f0_on = period_f0 - b;
            b = (period_f1 ) / 10;
            period_f1_on = period_f1 - b;
        break;
        default:  //duty = 12%
             b = (period_f0 ) / 8;
            period_f0_on = period_f0 - b;
            b = (period_f1 ) / 8;
            period_f1_on = period_f1 - b;
        break;
        
      } 
    }
 else if(kind_of_ring == 7) // UD request the
    {  // for short tone the volume is min. --> duty = 1%
    b1 = (unsigned char)((period_f0 ) / 100);
    period_f0_on = period_f0 - b1;
    b1 = (unsigned char)((period_f1 ) / 100);
    period_f1_on = period_f1 - b1;
    }  // endif	 
  else
    {  // for courtesy tone the volume is min. --> duty = 2%
    b1 = (unsigned char)((period_f0 * 2) / 100);
    period_f0_on = period_f0 - b1;
    b1 = (unsigned char)((period_f1 * 2) / 100);
    period_f1_on = period_f1 - b1;
    }  // endif
  TPM2C0V = period_f0_on;

  numper = numper_on = numper_f0_on;   
  numper_off = numper_f0_off;
  phase_tone = 0xff;        
  freq_tone = 0;            
  GoTimer(timer_repeat_tone, tone_t_rep);    
  if (kind_of_ring_save == 0)          
    {  
    num_door_opencket_biton = tone_num_door_openk_biton;
    num_sequence_biton = tone_num_seq_biton;
    }  

  kind_of_ring_save = kind_of_ring; 
  TPM2SC_TOF = 1;           // clear flag
  TPM2SC_TOIE = 1;     
  R_Volume_Call = SettingPara.CallVolume;
  Free(tone_proc, (BI_TONE_END | NOTE_PAUSE_END));
  
  if (kind_of_ring <= 5) 
  	RPCH = RPCH_ON;
   
}


// ----------------------------------------------------------------------------
void Stop_tone(void)     
{
  TPM2C0SC &= ~(TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0A_MASK);   // disable pin on TPM2
  TPM2SC_TOIE = 0;    // disable interrupt
  TPM2SC_TOF = 0;     // reset flag
  //CALL_PIN = 0;       
  kind_of_ring_save   = 0;
  num_door_opencket_biton = 0;
  num_sequence_biton  = 0;
  timer_ring_delay = 0;
  GoTimer(timer_door_openuse_tone, 0);
  Free(tone_proc, (RING_REQUEST | BI_TONE_END | NOTE_PAUSE_END | RING_NOW));
  RPCH = RPCH_OFF;

  EventHandle_Flag.bits.StopToneNeed = 0;
  EventHandle_Flag.bits.PSTNOnRingTone = 0;
  // Must be added also here because stop tone is a legal exit point for the end of a tone
  //Calculate_Volume_and_update_Conexant(R_Audio_Volume);
//not managed by OKI  SetOKI_Volume(R_Audio_Volume);
	TR_2 = ON;
	//PTTIE = 1; //for fixed the data noice when transmit data to JD board
	EventHandle_Flag.bits.StopToneNeed = 0;
	EventHandle_Flag.bits.PSTNOnRingTone = 0;
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void End_tone(void)        // stop a tone in progress
{
  TPM2C0SC &= ~(TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0A_MASK);  
  TPM2SC_TOIE = 0;    // disable interrupt
  TPM2SC_TOF = 0;     // reset flag
  //CALL_PIN = 0;       
  kind_of_ring_save   = 0;
  num_door_opencket_biton = 0;
  num_sequence_biton  = 0;
  timer_ring_delay = 0;
  GoTimer(timer_door_openuse_tone, 0);
  Free(tone_proc, (RING_REQUEST | BI_TONE_END | NOTE_PAUSE_END | RING_NOW));
  
  RPCH = RPCH_OFF;
	//TR_2 = ON;
	//PTTIE = 1; //for fixed the data noice when transmit data to JD board
	//EventHandle_Flag.bits.StopToneNeed = 0;
	//EventHandle_Flag.bits.PSTNOnRingTone = 0;
  
	//switch on video at the end of ringtone in order to reduce power consumption
	//comment due to we need always switch on the LCD
	/*
  if((kind_of_ring == F_kind_of_ringtone_audioonly) && 
     ((MODO_State == AUDIO_ONLY_OFF_HOOK_WAIT) ||
      (MODO_State == VIDEO_OFF_HOOK_WAIT) || 
      (MODO_State == IN_COMMUNICATION_VIDEO) ||          
      (MODO_State == IN_COMMUNICATION_AUDIO_ONLY)        
     )
     ){ 

    if (((Frame_copy[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) == Internal){         // Xmitted by me
      if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
        Video(VIDEO_ON);
        //EN_RELE_VIDEO = RELE_VIDEO_ON;
		//PageStatus = WaitPage;
	//OLD_MODO_State = IDLE;
      }
    }
    else{
			if(((Frame_copy[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       		// From a twin?
//  			if(((Frame_copy[D_DATA_2]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST){       		
          Video(VIDEO_ON);
		//PageStatus = WaitPage;
		//OLD_MODO_State = IDLE;
//        }
      }  
    }   
  }    
*/
  if(BIPBIP_audioonly_call){
	  Courtesy_tone();
	  BIPBIP_audioonly_call = OFF;
	} 

  //Calculate_Volume_and_update_Conexant(R_Audio_Volume);
//not managed by OKI  SetOKI_Volume(R_Audio_Volume);

}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
void Courtesy_tone(void)        
{
  kind_of_ring = 6;
  call_from = CALL_FROM_COURTESY;
  Signal(tone_proc, RING_REQUEST);
}

// ----------------------------------------------------------------------------
void Dissuasion_Tone(void)     
{
  kind_of_ring = 6;
  call_from = CALL_FROM_DISSUASION;
  Signal(tone_proc, RING_REQUEST);
}

// ----------------------------------------------------------------------------
void Short_tone(void)     
{
  kind_of_ring = 7;
  call_from = CALL_SHORT;
  Signal(tone_proc, RING_REQUEST);
}

//-----------------------------------------------------------
void Long_Tone(void)
{
   kind_of_ring = 8;
   call_from = CALL_Long;
   Signal(tone_proc, RING_REQUEST);


}
// ----------------------------------------------------------------------------
void Floor_call_tone(void)     //
{
  kind_of_ring = F_kind_of_ringtone_floor;
  call_from = CALL_FROM_FLOOR;
  //  Signal(tone_proc, RING_REQUEST);
}


// ----------------------------------------------------------------------------
void Tone_Management(void)
{
  
  if (tone_proc & RING_REQUEST)
    {
    Free(tone_proc, RING_REQUEST);
    // Need a delay to wait the conexant has set the right audio level 
    Timer_Wait_Tone = 10;  // 100 ms with a 10ms timebase
    //Signal(tone_proc, RING_NOW);
    //Calculate_Volume_and_update_Conexant(R_Volume_Call);
//not managed by OKI    SetOKI_Volume(R_Volume_Call);
    }


  if (tone_proc & RING_NOW){
    Free(tone_proc, RING_NOW);

    EN_AUDIO = FONIA_OFF;       // audio activation
	if (kind_of_ring <= MAX_RINGTONES)//fixed 3781
	{
		if(!R_Led_mute || PageStatus == SettingSelect|| PageStatus == PSTN_Setting)
		{//
			Setup_tone();
		}
	}
	else
    		Setup_tone();
    }  // endif



  if (tone_proc & BI_TONE_END)
    {
    Free(tone_proc, BI_TONE_END);
    
    if (num_door_opencket_biton)
      {
      if ((--num_door_opencket_biton) == 0)
        {  
        if (num_sequence_biton)
          {
          if (--num_sequence_biton == 0)
            { 
	          Stop_tone();
            //End_tone();
            }
          else
            { 
            GoTimer(timer_door_openuse_tone, tone_door_openuse_sequence); 
            num_door_opencket_biton = tone_num_door_openk_biton;          
            }  // endif
          }
        else
          {  
	        Stop_tone();
          //End_tone();
          }  // endif
        }
      else
        { 
        GoTimer(timer_door_openuse_tone, tone_door_openuse_bitonal);       
        }  // endif
      }
    else
      { 
      //End_tone();
	  Stop_tone();
      }  // endif
    }  // endif

  if (tone_proc & NOTE_PAUSE_END)
    {
    Free(tone_proc, NOTE_PAUSE_END);
    if (kind_of_ring_save == 0)
      { 
      kind_of_ring_save = kind_of_ring_bck;     
      }  // endif
    kind_of_ring = kind_of_ring_save;          // resume kind of ringtone
    Signal(tone_proc, RING_NOW);               

    }  // endif

}


void SettoWaitPage(void)
{
	 if(F_Scenarioshortcut & 0x01)
	  {
		PageStatus = WaitWithShortCut;
	  }
	  else
	  	PageStatus = WaitPage;
}

void F_Back_Menu(void)
{
	if(F_Scenarioshortcut & 0x01)
		BackMenu = WaitWithShortCut;
	else
		BackMenu = WaitPage;
}
void Sleep_Menu_init(void)
{
	if(PageStatus == WaitWithShortCut)
	{
		LoadJD_Menu(PageStatus);
		LoadJD_SleepMenu(OFF);
	}
	else if(PageStatus == WaitPage)
		LoadJD_SleepMenu(OFF);
}

//-----------------------------------------------------------------------------------------
void IfAlarmSystme_Priority(void)
{
	if(Sub_MODO_State == Alarm_Handle_Waiting)
	{
		if(MODO_State != IDLE)
		{
			Free_Line();
			MODO_State = IDLE;
			OLD_MODO_State = 0xff;
		}
	}
}
// ------------------------------------------------------------------------------------------------------

void States_Management(void){
	
	// Analisys of States
	IfAlarmSystme_Priority();
	switch(MODO_State){

		case IDLE:   
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State;
				if(PageStatus != Alarm_handle)
				{
					if(PageStatus != WaitPage && PageStatus != WaitWithShortCut)
			  	  	{
			  	  		//PageStatus = WaitPage;
			    			ExitToWaitPage();
			  	  	}	
					Video(VIDEO_OFF);                     //V1.1
					Sub_MODO_State = EX_IDLE; 
				}
				else
				{
					Sub_MODO_State = Alarm_Handle_Waiting;//should display the alarm menu
				}
				Audio(FONIA_OFF);		          
				REL2 = OFF;	
				PCM_Audio_Switch = To_Modo;
				ICON = 0x01;
				ROOM_MONITOR_IN_PROGRESS = 0;  
	  		}
	      		EN_RELE_VIDEO = RELE_VIDEO_ON;       //V1.1
	      		// Always restore IPERVOICE as default 
	      		//check if need sub modo state managment
	      		if(Ex_Module_StateManagement())
	      		{
	      			//
	      			if(JD_Save_Frame_Index == JD_TX_Frame_Index)//for ignor the blue screen show when close the talk with call module
					System_Video = VIDEO_OFF;
				timer_wait_offhook_audio = 0;
				number_SVC = 0;
				timer_delay_offhook = 0;       
				MIC_Muted = OFF;
				Audio_Switch = V_To_System;
				IN4 = 0;						
				PTTIE = 0;
	      		}
		break;

		case AUDIO_ONLY_OFF_HOOK_WAIT:
      // Detect transition
		      if(OLD_MODO_State != MODO_State){
		        OLD_MODO_State = MODO_State; 
				/*
		        if(SystemStatus.bits.CallSwitchBoard_Icon)
			{
				SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
		        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
			}
			*/
		        SwitchedOnOff = 1;
			Audio_Switch = V_To_System;
		        SwitchTalkMenu();
		        Main_GUI_locked = 0;
		        Call_GUI_locked = 0;
			
			Timer_GUI_locking = 0; // safety clear	
		        if(R_Led_mute){ // If I'm in mute, I can switch on immediately the video
		          if (((Frame_copy[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) == Internal){         // Xmitted by me
		            if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
		              Video(VIDEO_ON);
		            }
		          }
		          else{
		      			if(((Frame_copy[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       		// From a twin?
		        			if(((Frame_copy[D_DATA_2]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST){       		
		                Video(VIDEO_ON);
		              }
		            }  
		          }
		        }
				
		      }
      			System_Video = RELE_VIDEO_OFF;
			
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case VIDEO_OFF_HOOK_WAIT:
		      // Detect transition
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State; 		        
				SwitchedOnOff = 1;
				CCTV_EN = 0;
				Audio_Switch = V_To_System;
				if(R_Led_mute)
				{ 
		          		if (((Frame_copy[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) == Internal)
					{         // Xmitted by me
		            			if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR)
						{
							Video(VIDEO_ON);
							System_Video = RELE_VIDEO_ON; 
		            			}
				
		          		}
		          		else
					{
		      				if(((Frame_copy[D_ADDRESS]) & A3_BIT_USER_MASK) == User)
						{       		// From a twin?
		        				if(((Frame_copy[D_DATA_2]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST)
							{       		
		                				Video(VIDEO_ON);
				
		              			}
		            			}  
		          		}
				}
				/*
				if(SystemStatus.bits.CallSwitchBoard_Icon)
				{
					SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
			        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
				}
				*/
				SwitchTalkMenu();
		        // Unlock GUI 
			        Main_GUI_locked = 0;
			        Call_GUI_locked = 0;
				
			        Timer_GUI_locking = 0; // safety clear
		      }
					// Audio(FONIA_OFF);
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case SELFINSERTION_OFF_HOOK_WAIT:
      // Detect transition
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State; 
				//PageStatus = WaitPage; //clear old state then we could send new data to videostar
				SwitchedOnOff = 1;
				CCTV_EN = 0;
				SwitchTalkMenu();	 
				Main_GUI_locked = 0;
				Call_GUI_locked = 0;
				//if(Timer_GUI_locking)
					//SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
				Timer_GUI_locking = 0; // safety clear
				PCM_Audio_Switch = To_Modo;
				//   			Audio(FONIA_ON);
			}
			Video(VIDEO_ON);
			System_Video = RELE_VIDEO_ON;
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case ANSWERINGMACHINE_OFF_HOOK_WAIT:
      // Detect transition
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State; 
				SwitchedOnOff = 1;				
				Main_GUI_locked = 0;
				Call_GUI_locked = 0;
				CCTV_EN = 0;
				//if(Timer_GUI_locking)
					//SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
				Timer_GUI_locking = 0; // safety clear
				PCM_Audio_Switch = To_Modo;	
			}			
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case INTERCOM_OFF_HOOK_WAIT:
      			// Detect transition
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State; 			       
				SwitchedOnOff = 1;				
				Main_GUI_locked = 0;
				Call_GUI_locked = 0;
				Audio_Switch = V_To_System;
				/*
				if(SystemStatus.bits.CallSwitchBoard_Icon)
				{
					SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
			        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
				}
				*/
				Timer_GUI_locking = 0; // safety clear
				SwitchTalkMenu();	
		      }
			
			System_Video = RELE_VIDEO_OFF;
				
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case IN_COMMUNICATION_AUDIO_ONLY:
			// Detect transition
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State; 
				// Unlock GUI 
				Main_GUI_locked = 0;
				Call_GUI_locked = 0;
				/*
				if(SystemStatus.bits.CallSwitchBoard_Icon)
				{
					SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
			        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
				}
				*/
				Timer_GUI_locking = 0; // safety clear
				Audio(FONIA_ON);
				//PageStatus = WaitPage;
				if(PageStatus != Talk_To_CallModule_NotHide)
				{
					SwitchTalkMenu();
				}
				SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AnswerCall, ON);// blink	
			}
			//			Video(VIDEO_OFF);
			System_Video = RELE_VIDEO_OFF;
			PCM_Audio_Set();
			REL2 = ON;	
			//SwitchTalkMenu();
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case IN_COMMUNICATION_VIDEO:
			// Detect transition
			if(OLD_MODO_State != MODO_State)
			{
				OLD_MODO_State = MODO_State; 
				Video(VIDEO_ON);
				System_Video = RELE_VIDEO_ON;
				Main_GUI_locked = 0;
				Call_GUI_locked = 0;
				CCTV_EN = 0;
				/*
				if(SystemStatus.bits.CallSwitchBoard_Icon)
				{
					SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
			        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
				}
				*/
				Timer_GUI_locking = 0; // safety clear
				Audio(FONIA_ON);
				if(PageStatus != Talk_To_CallModule_NotHide)
				{
					SwitchTalkMenu();
				}
				SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AnswerCall, ON);// blink
			}
			PCM_Audio_Set();
			REL2 = ON;	
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case IN_COMMUNICATION_SELFINSERTION:
      			// Detect transition
	      		if(OLD_MODO_State != MODO_State)
		  	{
	        		OLD_MODO_State = MODO_State; 
				//PageStatus = WaitPage; //clear old state
	  			Video(VIDEO_ON);
				System_Video = RELE_VIDEO_ON;
				CCTV_EN = 0;
	        		Main_GUI_locked = 0;
	        		Call_GUI_locked = 0;
					/*
				if(SystemStatus.bits.CallSwitchBoard_Icon)
				{
					SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
			        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
				}
				*/
	        		Timer_GUI_locking = 0; // safety clear
	  			Audio(FONIA_ON);	
				SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AnswerCall, ON);// blink
	      		}
			Video(VIDEO_ON);
			System_Video = RELE_VIDEO_ON;
			//SwitchTalkMenu();
			PCM_Audio_Set();
			IN4 = 0;						
			PTTIE = 0;
			REL2 = ON;	
	  		hook_timer_idle = 0; // Clear automatic onhook timer
		break;

		case IN_COMMUNICATION_ANSWERINGMACHINE:
		      // Detect transition
			if(OLD_MODO_State != MODO_State)
			{
		        	OLD_MODO_State = MODO_State; 
		  		Video(VIDEO_ON);
				Main_GUI_locked = 0;
				Call_GUI_locked = 0;
				CCTV_EN = 0;
				/*
				if(SystemStatus.bits.CallSwitchBoard_Icon)
				{
					SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
			        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
				}
				*/
				Timer_GUI_locking = 0; // safety clear
				PageStatus = VideoMessagePlaying;
				LoadJD_Menu(VideoMessagePlaying);
				Audio(FONIA_ON);			
			}
			Video(VIDEO_ON);
			//if(V_QueueLen == 0 && SystemStatus.bits.IICCheckLast == 0)
		      	System_Video = RELE_VIDEO_ON;
	  		//SwitchTalkToCallModule();
	  		IN4 = 0;						
			PTTIE = 0;
			REL2 = ON;	
			hook_timer_idle = 0; // Clear automatic onhook timer
		break;
		
		default:
		break;
	}
	
	  
	
}

// ------------------------------------------------------------------------------------------------------

void Timers_Management(void){

	unsigned char i_T;
	
	// ------------- Timer a  10ms ----------------------------------
	if (!timer_10ms)
	{
		timer_10ms = TIME_10_MSEC;


	if(BlindAudioWaveWaitTime)
		BlindAudioWaveWaitTime--;
		
		if (timer_sec)
			--timer_sec;
	  
	  	if (timer_100ms)
			--timer_100ms;

		if(Pin_IO_Set_Delay)
			Pin_IO_Set_Delay--;
		
		 if(JD_TX_Frame_Time)
			JD_TX_Frame_Time--;

		 if(TelRdFrameTime)
			TelRdFrameTime--;
    //------------------------------------------
		if(!Pres_BT)
		{ //check as HRB protocol
		    if(KNX_R_RC_P)
			{
				if(KNX_R_RC_P != KNX_R_RC_P_old)
				{
					KNX_R_RC_P_old = KNX_R_RC_P;
					FrameV_T_CNT = 0;
					
				}
				else
				{
					if(FrameV_T_CNT++>1)
					{
						//time to verify the rx frame
						FrameV_T_CNT = 0; //count time again
						Frame_HRB_Verify();
					}
				}
			}
			
		}
    //------------------------------------------                

		if(Hook_button_time)
		{
			Hook_button_time--;
			if(!Hook_button_time)
			{
				OldAudioPhone_Hook = AudioPhone_HOOK;
				//return the key handle according modo state
				if(MODO_State == AUDIO_ONLY_OFF_HOOK_WAIT || MODO_State == VIDEO_OFF_HOOK_WAIT || MODO_State == INTERCOM_OFF_HOOK_WAIT)
				{
					if(!AudioPhone_HOOK)
					{
						KeyValue = K_AnsweringService;//only for handset answer call  or close 
						Flag.bits.KeyNhandle=1;
						TR_2 = OFF;
						BeepOK();
					}
				}
				else if(MODO_State == IN_COMMUNICATION_AUDIO_ONLY ||MODO_State == IN_COMMUNICATION_VIDEO)
				{
					if(AudioPhone_HOOK && PCM_Audio_Switch == To_HandSet)
					{
						KeyValue = K_AnsweringService;//only for handset answer call  or close 
						Flag.bits.KeyNhandle=1;
						TR_2 = OFF;
						BeepOK();
					}
				}
				else if(Sub_MODO_State == PCM1CALLING ||Sub_MODO_State == PSTN_Calling || Sub_MODO_State == PSTN_Calling_Video)
				{
					if(!AudioPhone_HOOK)
					{
						KeyValue = K_PSTN_Answer_HandSet;
						Flag.bits.KeyNhandle=1;
						TR_2 = OFF;
						BeepOK();
					}
				}
				else if(Sub_MODO_State == IN_COMMUNICATION_PCM1 || Sub_MODO_State == PSTN_Talking || Sub_MODO_State == PSTN_Talking_Video)
				{
					if(PCM_Audio_Switch == To_HandSet)
					{
						KeyValue = K_PSTN_Answer_HandSet;
						Flag.bits.KeyNhandle=1;
					}
				}
				
			}



		}
	//------------------------------------------------------
		if(Timer_Event_Delay)
			Timer_Event_Delay--;

		if(DTMF_wait)
			DTMF_wait--;

		if(KNX_TX_Frame_Time)
			KNX_TX_Frame_Time--;

	 	
		//------------------------------------------				
		if (timer_proc_scan)
		{
			timer_proc_scan--;
			if (!timer_proc_scan)
			{
				SignalInt(proc_scan, AWAKE);
			}  
		}  
		//------------------------------------------				

	  	
		//------------------------------------------				
    
		//------------------------------------------				
		
		
		//------------------------------------------	
		
		if (Timer_Wait_Tone)
		{
			Timer_Wait_Tone--;
			if (!Timer_Wait_Tone)
			{
				Signal(tone_proc, RING_NOW);
			}  
		}
    //------------------------------------------				
    
    
		//------------------------------------------
		if(Old_index_buffer_rx_JD != index_buffer_rx_JD)
		{
			Old_index_buffer_rx_JD = index_buffer_rx_JD;
			JD_Buf_Clear_Time = 8;
		}
		else if(index_buffer_rx_JD)
		{
			JD_Buf_Clear_Time--;
			if(!JD_Buf_Clear_Time)
			{
				index_buffer_rx_JD = 0;
				Old_index_buffer_rx_JD = 0;
			}
		}
		else
			JD_Buf_Clear_Time = 8;
    //------------------------------------------				
	  	if(PCM1_Call_MODO_Button_time && !PCM_Button_Detect_Wait)
		{
			PCM1_Call_MODO_Button_time--;
			if(!PCM1_Call_MODO_Button_time)
			{
				if(PageStatus != Alarm_handle && MODO_State == IDLE && (Sub_MODO_State == EX_IDLE || Sub_MODO_State == PCM1CALLING))
				{
					if(PCMcallingTime < 20)
					{
						Sub_MODO_State = PCM1CALLING;
						R_Volume_Call = SettingPara.CallVolume;
						if(!R_Led_mute)	//not in mute
						{
							Play_New_Melody(AUDIO_ONLY_CALL);
							//RingRepeatTime = F_RingRepeatTs;
							//RingRepeatTimeCnt = F_RingRepeatT;
						}
						PCMcallingTime = 25;
						//TR_1 = OFF;
						TR_2 = OFF;
					}
				}
				else
				{//system is busy
					//TR_1 = OFF;
					if(Sub_MODO_State != Mornitor_PCM1)
					{
						TR_2 = OFF;
						Courtesy_tone();
					}
				}
			}
		}
  
	// ---------------------------------------------------------------
		if(Lock_button_time)
		{
			Lock_button_time--;
			if(!Lock_button_time)
			{
				if(Sub_MODO_State == PSTN_Talking)
				{
					if(PCM_Audio_Switch == To_HandSet)
						KeyValue = 4;
					else
						KeyValue = 144;
						
				}
				else if(Sub_MODO_State== IN_COMMUNICATION_PCM1 || Sub_MODO_State == PCM1CALLING)
				{
					KeyValue = 11;
				}
				else
				{
					KeyValue = K_Door_Open_1;
				}
				Flag.bits.KeyNhandle = 1;
				TR_2 = OFF;
				//TR_1 = ON;	
		             	BeepOK();
			}
		}
	}	
  // ------------- Timer a  100ms ----------------------------------
 if (timer_100ms == 0)
 {
	timer_100ms = TIME_100_MSEC;
	
    //Rc_Tel_Num();
	if(KeyLimitDly)
		KeyLimitDly--;

	if(PCM_OnOff_Interval)
		PCM_OnOff_Interval--;
	
	if((Common_falg.bits.EN_Capture==1)&&(blind_audio_period_times>0))
		BlindAudio_NO++;
			
		if(Dly_EN_Capture)
		{
			Dly_EN_Capture--;
			if(!Dly_EN_Capture)
				{
				;
				/*Common_falg.bits.EN_Capture=1;
				TPM2_time_overflow_times=0;
				TPM2_ch1_interrupt_times=0;
				TPM2_capture_times=0;
				blind_audio_period_limit=0;
				blind_audio_dec_times=0;
				blind_audio_period_times=0;*/
				}
		}



	if(SetTimeDelay)
   		SetTimeDelay--;
	
	if(Read74HC165_Time) //for scan the dipswitch
		Read74HC165_Time--;

	if(Delay_Hook_Press_Time)
		Delay_Hook_Press_Time--;
	if(BT_Check_Time)
	{
		BT_Check_Time--;
		if(!BT_Check_Time)
		{
			if(F_Old_Pres_BT != Pres_BT)
			{
				F_Old_Pres_BT = Pres_BT;
				HW_Protocol_Init();
			}
		}
	}
// 		//------------------------------------------				
// 		if(timer_pre_occupato_D){
// 			timer_pre_occupato_D--;	
// 			if(!timer_pre_occupato_D){
// 			}	
// 		}
	if(VolumeCNT)
	{
		VolumeCNT--;
		if(!VolumeCNT && EN_Z == ON)
		{
			ClearVolume();
			SetVolumeChange(1,SettingPara.AudioVolume);
		}	
	}
	//-------------------------------------------------------------------------------------
	if(PCM_Button_Detect_Wait && Sub_MODO_State != PCM1CALLING && Sub_MODO_State != IN_COMMUNICATION_PCM1)
		  	PCM_Button_Detect_Wait--;
		//------------------------------------------				
	/*
	if(timer_logo){
	  timer_logo--;
	  if(timer_logo == 1){
	    timer_logo = LOGO_BLINK;
	    LED_LOGO = ~LED_LOGO;
	  }
	}
	*/	
	
	
		//------------------------------------------				
	if(timer_delay_offhook){
		timer_delay_offhook--;
		if(!timer_delay_offhook)
		{
			if(hook_state_toggled == OFF_HOOK)
			{
				p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte                     = 3;
				p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
				p_der_queue_tx_appl->data[1]                    = User + Internal;
				p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 	  
				Incr_index_queue_in_der();                      // update index queue
			}
		}
	}
		//------------------------------------------
		
	if(call_switchboard_button_time)
	{
		call_switchboard_button_time--;
		if(!call_switchboard_button_time)
		{
			if(MODO_State == IDLE)
			{
				//KeyValue = K_CallSwitchBoard;
				//Flag.bits.KeyNhandle=1;
				//if(!CallSwitchBoardTime)
				//{
					
					if(!Main_GUI_locked){
						S_OUT_CALL_FUNCTION(0x80,0x30,0x30,250);
						//LoadMainMenu(F_MainMenu,1);
						//CallSwitchBoard_From = 1;
						PCM_Audio_Switch = To_HandSet;
						BeepOK();
				   	}
					else
						Dissuasion_Tone();
					//CallSwitchBoardTime = 20; //after 10s without any handle form switchboard, transmit discusion tone
					//the call swichboard from handset
					
					//Audio_Switch = 0;
					TR_2 = OFF;
					//TR_1 = ON;	
	                 		
				//}
			}
			else
			{
				KeyValue = K_Door_Open_2;
				Flag.bits.KeyNhandle = 1;
				TR_2 = OFF;
				//TR_1 = ON;	
	                 	BeepOK();
			}
		}
	}    
		//------------------------------------------				
		if(timer_ring_delay){
			timer_ring_delay--;	
			if(!timer_ring_delay){
			        if(!R_Led_mute){
			  				
	  				if(call_from == CALL_FROM_FLOOR){  // Ring immediately
	    					Signal(tone_proc, RING_REQUEST);
						TR_2 = OFF;// ring tone also for handset
	  				}
	  				else{
	  					if(hook_state_toggled == ON_HOOK){	// If I'm ONHOOK 
	      						Signal(tone_proc, RING_REQUEST);
							TR_2 = OFF;// ring tone also for handset
	  					}
	  					else{
				    		      if((call_from == CALL_FROM_SECONDARY) ||
				    		        (call_from == CALL_FROM_MAIN)  ||
				    		        (call_from == CALL_FROM_SWITCHBOARD)  ||
				                	(call_from == CALL_FROM_INTERCOM)    ||  
				    		        (call_from == CALL_FROM_SECONDARY))
				  						  Short_tone();				
	  					}				
	  				}
			        }
			}	
		}
  	
					
  // timer management
  
    if(timer_delayed_event){
      timer_delayed_event--;
      if(!timer_delayed_event){
    
        switch(Frame_copy[D_OPC]){

          case D_PRESENCE:
            timer_wait_offhook_audio = COMMUNICATION_TIMER;
            
            if((Frame_copy[D_DATA_1]) & VIDEO_OR_AUDIO_COMM_TYPE){
              // --- go in future state
              switch((Frame_copy[D_DATA_1]) & SOURCE_MASK){

                case FROM_SECONDARY_CM:
                case FROM_MAIN_CM:
                case FROM_SWITCHBOARD:
                case FROM_IP_PHONE:
                case FROM_VILLA_CM:
					OLD_MODO_State = IDLE;
                  MODO_State = VIDEO_OFF_HOOK_WAIT;

                  if((((Frame_copy[D_DATA_1]) & SOURCE_MASK) == FROM_SWITCHBOARD) && (Timer_SHSTC)){
                    hook_state_toggled = OFF_HOOK;
//                    ST_Audio = OFF_HOOK_0;
//                    timer_audio_poweron = TIME_IMP_AUDIO7;
                    Timer_SHSTC = 0; // Stop the timer that will switch me off when elapsed
                    
                    End_tone();
                  }
                
                break;	

                case FROM_THE_INQUIRED_ANSWERING_MACHINE:
                  MODO_State = ANSWERINGMACHINE_OFF_HOOK_WAIT;
//                   // For andare immediately in audio, data che non ho il hook by testare
//                   // invio immediately the frame of offhook
                  hook_state_toggled = OFF_HOOK;

                  
                break;	

                case FROM_S_I_IP_CM_NO_AUDIO:
                case FROM_S_I_VILLA_CM_NO_AUDIO:
                  MODO_State = SELFINSERTION_OFF_HOOK_WAIT;
                break;	

                default:
                break;
              }
              if (((Frame_copy[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) == Internal){// Xmitted by me
//           			if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
                  
                  // Switch on monitor					
                  Video(VIDEO_ON); 
                  
                  SwitchedOnOff = 1;
                  
                  if((MODO_State == VIDEO_OFF_HOOK_WAIT) || 
                     (MODO_State == ANSWERINGMACHINE_OFF_HOOK_WAIT)   ||
                     (MODO_State == SELFINSERTION_OFF_HOOK_WAIT)
                     ){
                    // If the call is a video one, I switch on immediately the signal to avoid 
                    // to see before the OSD and than the video
                    		System_Video = RELE_VIDEO_ON;
                    }
				
//                }
              }
              
            }
            else{  // it is surely audioonly
//         			if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
                MODO_State = AUDIO_ONLY_OFF_HOOK_WAIT;

                if((((Frame_copy[D_DATA_1]) & SOURCE_MASK) == FROM_SWITCHBOARD) && (Timer_SHSTC)){    //V1.1
                  hook_state_toggled = OFF_HOOK;                                                             //V1.1
//                  ST_Audio = OFF_HOOK_0;                                                                         //V1.1
//                  timer_audio_poweron = TIME_IMP_AUDIO7;                                                       //V1.1
                  Timer_SHSTC = 0; // Stop the timer that will switch me off when elapsed                                   //V1.1
                  // Stop the already started ringtone                                              //V1.1
                  End_tone();                                                                                  //V1.1
                }                                                                                               //V1.1
                                                



                if (((Frame_copy[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) == Internal){// Xmitted by me
             			if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR){

                    // Switch on monitor					
                    Video(VIDEO_ON);
                    
                    SwitchedOnOff = 1;
             
                  }
                }
                
//              }
            }

            if((hook_state_toggled == OFF_HOOK) ||
              // This is a presence emitted following call caming from CEN and requested by me and received 
              // within 10 seconds from the request richiesta 
              ((((Frame_copy[D_DATA_1]) & SOURCE_MASK) == FROM_SWITCHBOARD) && (Timer_SHSTC)))
              {
              Timer_SHSTC = 0; // Stop the timer that will switch me off when elapsed

              // Stop the already started ringtone
              End_tone();
              
              // Blink the icon
      				
              // Emit the off-hook          
              timer_delay_offhook = Internal + 1;
            
            }                     
              
      			// { PMPM
      			if(((Frame_copy[D_DATA_1]) & SOURCE_MASK) == FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
      				// Emit the off-hook immediately          
              p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
              p_der_queue_tx_appl->n_byte                     = 3;
              p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
              p_der_queue_tx_appl->data[1]                    = User + Internal;
              p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 	  
              Incr_index_queue_in_der();                      // update index queue
            }
      			// } PMPM
          break;
      
          default:
          break;
        
        } 
      }
    }

  	}			
			
//-------------    Management one second events ----------------------------------------
    if (timer_sec == 0)
      {
		timer_sec = TIME_1_SEC;
		if(timer_1min)
			timer_1min--;
		if(NoKeyTime)
			NoKeyTime--;
      //------------------------------------------				
      		if(CheckDTMF_Delay)
			CheckDTMF_Delay--;
		if(Alarm_Enable_Time_Delay_CNT)
		{
			Alarm_Enable_Time_Delay_CNT--;
			if(PageStatus == AlarmSystem_Manage)
			{
				ShowRecount(Alarm_Enable_Time_Delay_CNT,AlarmSystem_Manage);
				//Short_tone();
			}
		}

		 if(PSTN_Call_TimeOut_Cnt)//try to fixed Gerber's problem 2013-11-26
	     	{
	     		PSTN_Call_TimeOut_Cnt--;
			if(!PSTN_Call_TimeOut_Cnt)
			{
				//ClearTelDispFlag=1;
				if(Sub_MODO_State == PSTN_Calling || Sub_MODO_State == PSTN_Calling_Video)
				{
					Sub_MODO_State = IDLE;
					//Timer_ExitPSTN=30;
					Stop_tone();
				}
			}
	     	}

		 /*if(Timer_ExitPSTN)
		 {
			Timer_ExitPSTN--;
			if(!Timer_ExitPSTN)
			{
				if((PageStatus==PSTN_Telephone)||(PageStatus==Menu_Telephone_Video))
				{
					SettoWaitPage();
				}
			}
		}*/
		 //-------------------------------------------------------------
		 /*
		 if(RingRepeatTimeCnt)
		{
			RingRepeatTimeCnt--;
			if(!RingRepeatTimeCnt)
			{				
				if(RingRepeatTime)
				{
					RingRepeatTime--;
					RingRepeatTimeCnt = F_RingRepeatT;
					if(Sub_MODO_State == PSTN_Calling || Sub_MODO_State == PSTN_Calling_Video)
					{
							Play_New_Melody(AUDIO_ONLY_CALL);
							// Modo and HandSet should ring together
							//TR_1 = OFF;
							TR_2 = OFF;
					}

					else
						RingRepeatTimeCnt = 0;
				}
			}
		}
		*/
      //------------------------------------------				
		if(VolumeBar_DisplayTime)
	      	{
	      		VolumeBar_DisplayTime--;
			if(!VolumeBar_DisplayTime)
			{
				// Hide the audio volumn bar
				SetControl_State(VolumeMenu, 5, OFF);
			}
	      	}

		//--------------------------------------------
		if(WarningToneNumber)
		{
			if(WarningToneTime)
				WarningToneTime--;
			else
			{
				WarningToneTime = 3;
				WarningToneNumber--;
				Dissuasion_Tone();
			}
		}

		if(Alarm_Delay_Tone_Cnt)
		{
			Alarm_Delay_Tone_Cnt--;
			if(PageStatus == AlarmSystem_Manage)
			{
				ShowRecount(Alarm_Delay_Tone_Cnt,AlarmSystem_Manage);
				Short_tone();
			}
		}
      //------------------------------------------		
		if(PCMcallingTime)
		{// if pcm calling time out we should return to idle state when the modo state have no changed from call
			PCMcallingTime--;
			if(!PCMcallingTime)
			{
				if(Sub_MODO_State == PCM1CALLING)
				{
					Sub_MODO_State = EX_IDLE;
					if(MODO_State == IDLE)
						OLD_MODO_State = 0xff;
				}
			}
		}

		if(Pcm_TalkTime)
		  {
		  	Pcm_TalkTime--;
			if(!Pcm_TalkTime)
			{
				if(Sub_MODO_State != EX_IDLE)
				{
					Sub_MODO_State = EX_IDLE;
					if(MODO_State == IDLE)
						OLD_MODO_State = 0xff;
				}
			}
		  }
	  //---------------------------------------------------------
		if(Timer_SHSTC)
		{
        		Timer_SHSTC--;
			/*
			if(!Timer_SHSTC)
			{
				if(MODO_State == IDLE)
		            // Switch_Off like by button on bottom side of monitor
		            ;//Switch_Off(); //comment by Michael due to UD request, it cause time out problems
		        }
		        */
      		}

		if(Simulate_Hook_On_Time)
		{
			Simulate_Hook_On_Time--;
			if(!Simulate_Hook_On_Time)
			{
				if(MODO_State == IDLE)
				{
					hook_state_toggled = ON_HOOK; 
				}
			}
		}
      //------------------------------------------				
      
      		if(Gas_detector_button_time)
		 {
		 	Gas_detector_button_time--;
			if(!Gas_detector_button_time)
			{
				GasAlarmF = 1;
				Alarm_Type_To_Switchboard = 0x0400; //0x1000; //for bit 11
				//GasAlarmToneTime = 1; //warning tone at once
				if(AlarmP->AlarmZoneEnable & 0x01) //(0x01<<5))
				{
					Gest_Alarm();
					SetSwitchIcon(ChangeButton_Icon, Alarm_handle, 2, 0, 0x00, 235);			
					SwitchToAlarmMenu();
				}
		 	}
		 }
      //------------------------------------------				
  		if(hook_timer_idle)
		{
  			hook_timer_idle--;	
  			if(!hook_timer_idle)
			{
  				if (MODO_State == IDLE)
				{
					hook_state_toggled = ON_HOOK;
  				}
  			}	
      		}
      //------------------------------------------				

  		//------------------------------------------				
  		if((timer_wait_offhook_audio) && (timer_wait_offhook_audio != ENDLESS))
		{
  			timer_wait_offhook_audio--;	
  			if(!timer_wait_offhook_audio)
			{
  				if (MODO_State != IDLE)
				{
					Free_Line();
  				}
  			}	
      		}
      //------------------------------------------				
		if(PanicCancleTime)
		{
			PanicCancleTime--;
			if(PanicCancleTime>1)
			{
				ShowRecount(PanicCancleTime-1,Panic_Cancle);
				Short_tone();
			}
			else
			{
				SetSwitchIcon(ChangeButton_Icon, Panic_Cancle, K_Exit, 0, 0x00, 240);
				ShowRecount(0,Panic_Cancle);
				Short_tone();
			}
			if(!PanicCancleTime)
			{
				Gest_Panic();
				SettoWaitPage();
				Switch_Off();
				LoadJD_Menu(PageStatus);
			}
		}
      //------------------------------------------				
		if(Timer_Retry_D_Panic)
		{
			Timer_Retry_D_Panic--;
			if(!Timer_Retry_D_Panic)
			{
				if(Num_Retry_D_Panic)
				{
					Timer_Retry_D_Panic = ACK_WAITING_TIMEOUT;
					Num_Retry_D_Panic--;
					// Ri-trasmetto the frame without ack			
					p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
					p_der_queue_tx_appl->n_byte                     = 3;
					p_der_queue_tx_appl->data[D_OPC] = D_PANIC;
					p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
					p_der_queue_tx_appl->data[D_DATA_1] = Battery_State;
					Incr_index_queue_in_der();                      // update index queue
	          		}
	        	}
	      }
		
		if(Timer_Retry_D_Alarm)
		{
			Timer_Retry_D_Alarm--;
			if(!Timer_Retry_D_Alarm)
			{
				if(Num_Retry_D_Alarm)
				{
					Timer_Retry_D_Alarm= ACK_WAITING_TIMEOUT;
					Num_Retry_D_Alarm--;
					// Ri-trasmetto the frame without ack	
					/*
					p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
					p_der_queue_tx_appl->n_byte                     = 6;
					p_der_queue_tx_appl->data[D_OPC] = D_ALARM;
					p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 					
					p_der_queue_tx_appl->data[D_DATA_1] = Alarm_Type_To_Switchboard>>8;//
					p_der_queue_tx_appl->data[D_DATA_2] = Alarm_Type_To_Switchboard&0x00ff;//alarm flag
					p_der_queue_tx_appl->data[D_DATA_3] = 0x00;
					p_der_queue_tx_appl->data[D_DATA_4] = H_Modo_Ipervoice; //only for Hmodo
					Incr_index_queue_in_der();                      // update index queue
					*/
					AlarmFrame_Format();
	          		}
	        	}
	      }
		
      //------------------------------------------				
		for(temp = 0;temp<HomeAutomation_MaxInput;temp++)
		{
			if(Alarm_DelayTime_Cnt[temp]) 
			{
				Alarm_DelayTime_Cnt[temp]--;
				if(!Alarm_DelayTime_Cnt[temp])
				{
					//send delay alarm event
					SendDelayAlarm(temp);
					temp = HomeAutomation_MaxInput;
					//break; //return due to afraid more 10 alarm reach then the data Queue out of the limit
				}
			}
		}

		
      //------------------------------------------				
		if(Timer_Retry_D_PresenceAbsence)
		{
			Timer_Retry_D_PresenceAbsence--;
			if(!Timer_Retry_D_PresenceAbsence)
			{
				if(Num_Retry_D_PresenceAbsence)
				{
					Timer_Retry_D_PresenceAbsence = ACK_WAITING_TIMEOUT;
					Num_Retry_D_PresenceAbsence--;
					p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
					p_der_queue_tx_appl->n_byte                     = 3;
					p_der_queue_tx_appl->data[D_OPC] = D_PRESENCE_ABSENCE_BUTTON;
					p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
					p_der_queue_tx_appl->data[D_DATA_1] = R_presence_absence; 
					Incr_index_queue_in_der();                      
				}
			}
		}
      //------------------------------------------				

      //------------------------------------------				
		if(Timer_Inactivity)
		{
			Timer_Inactivity--;
			
			//if(Timer_Inactivity==10)
			//{	
			//	if(((PageStatus==PSTN_Telephone)||(PageStatus==Menu_Telephone_Video))&&(Sub_MODO_State == EX_IDLE))
			//	{
			//		SettoWaitPage();
			//		LoadJD_Menu(PageStatus);
			//	}
			//}
			
			if(!Timer_Inactivity)
			{		

				if(((PageStatus==PSTN_Telephone)||(PageStatus==Menu_Telephone_Video))&&(Sub_MODO_State == EX_IDLE))
				{
					SettoWaitPage();
					LoadJD_Menu(PageStatus);
				}
			
				if(MODO_State == IDLE && Sub_MODO_State == EX_IDLE)
	            // Switch_Off come by tastino on bottom all'LCD
				{
					Switch_Off();
					SettoWaitPage();
					LoadJD_Menu(PageStatus);
				}
				
	        	}
		}
      //------------------------------------------				


      //------------------------------------------				
		if(Timer_GUI_locking)
		{
			Timer_GUI_locking--;
			if(!Timer_GUI_locking)
			{
				if(Main_GUI_locked)
				{
					Main_GUI_locked= 0;
            				// Could be alpsed after a cycle request so I haven't to go to main screen
            				if(SystemStatus.bits.CallSwitchBoard_Icon)
					{
						SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
				        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
					}
          			}
          
          			Call_GUI_locked = 0;
        		}
      		}
      //------------------------------------------				

		if(PCM_LOCK_TIME)
		{
			PCM_LOCK_TIME--;
			if(!PCM_LOCK_TIME)
			{
				PCM1_Lock = OFF;
			}
		}

    
      //------------------------------------------		
       if(DlySendTimeEven){
			DlySendTimeEven--;
			if(!DlySendTimeEven)
				TimerEventFlag_Init();
       	}
	   	
		if(JD_start_wait)
		{
	      		JD_start_wait--;
			if(!JD_start_wait)
			{								
				SetControl_State(Talk_To_CallModule_NotHide, 5, OFF);
				SetControl_State(Menu_Telephone_Video, 5, OFF);
				SetControl_State(PSTN_Telephone, 5, OFF);
				
				for(temp1 = 0;temp1 < 6; temp1++)
				{
					if(AlarmP->AlarmZoneEnable&(0x01<<temp1))
					{
						SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, K_Zone1+temp1, 0, 0x01, 0xA6);
					}
				}
				if(!AlarmP->AlarmZoneEnable)
				{
					SetControl_State(MainMenu, 8, OFF);
				}

				TimerEventFlag_Init();
				
				if(!F_Led_mute)
				{						
					SetControl_State(MainMenu, 5, OFF);
									    //send iic command set mute light on						 
				}
				
				if(!F_messages_present)
				{						
					SetControl_State(MainMenu, 6, OFF);
									    //send iic command set mute light on						 
				}	
				if(!F_Presence_Absence)
				{						
					SetControl_State(MainMenu, 9, OFF);
									    //send iic command set mute light on						 
				}
				//alarm icon should hide at the beginning
				SetControl_State(MainMenu, 7, OFF);// hide the icon
				//if(SetControl_State(MainMenu, 5, ON);// hide the icon
				MenuSleepTime = 0;
			
				
				EventHandle_Flag.bits.ReflashTimerCount_F = 1;
				//JDFunctionCallBack1 = &ReadTime_From_JDBoard;
				if(JDFunctionCallBack2 == NULL)
					JDFunctionCallBack2 = &Sleep_Menu_init;
				//AlarmSystemZoneName_Init();

				if(StartEndFlag){
				Long_Tone();
				StartEndFlag=0;
				}
			}			
	    }
	
		if(OldPageStatus != PageStatus)
		{
			OldPageStatus = PageStatus;
			if(OldPageStatus == WaitPage || OldPageStatus == WaitWithShortCut)
			{
				MenuSleepTime = 5;
			}
			else
				MenuSleepTime = 0;
		}
		if(MenuSleepTime && !JD_start_wait)
		{
			MenuSleepTime--;
			if(!MenuSleepTime)
				LoadJD_SleepMenu(OFF);
		}

		//-------------------
		
	//---------------------------------------------------------------
	}
	//--------------------------------
	if(!timer_1min)
	{
		timer_1min = TIME_1_Min;
		for(i_T = 0; i_T < 8;i_T++)
		{
			if(Timer_Handle_TimerEvent[i_T])
			{
				Timer_Handle_TimerEvent[i_T]--;
				if(!Timer_Handle_TimerEvent[i_T])
				{
					//Time reach we should set the handle flag
					TimeEvent_Flag.all |=(1<<i_T);
					//check if we need time to recount or if it is a single event, we need disable the event due to we have implement it.
					
				}
			}
		}
		if(Timer_Reflash_Count)
		{
			Timer_Reflash_Count--;
			if(!Timer_Reflash_Count)
			{
				//set flag to call function recount the time
				EventHandle_Flag.bits.ReflashTimerCount_F = 1;
			}
		}
	}
//-------------  END Management ----------------------------------------
  
}

// ------------------------------------------------------------------------------------------------------
/*
void Refresh_Unused_Pin_State(void){
  // 0=input; 1=output

  // !!!!!! by aggiornare !!!!!!
  // // PORT A
  PTADD = 0x03;
  // // PORT B
  PTBDD = 0x04;

  PTCDD = 0xFC;     // 1= output
  // // PORT D
  PTDDD = 0x0F;     // 1= output
  // PTDD_PTDD1 = 0;
  // PTDD_PTDD2 = 0;
  // PTDD_PTDD3 = 0;
  // PTDDD = 0x0E;   
  // // PORT E
  PTEDD = 0xD7;
  
  // PTED_PTED0 = 0;
// 	 PTED_PTED1 = 0;
  	PTED_PTED2 = 0;
  // PTEDD = 0xF3;    
  // // PORT F
  PTFDD = 0x31;
  // PTFD_PTFD0 = 0;
  // PTFD_PTFD1 = 0;
  // PTFDD = 0xFF;   
  // // PORT G
  PTGDD = 0x6F;
  // PTGD_PTGD5 = 0;
  // PTGD_PTGD6 = 0;
  // PTGDD = 0xF0;   

}
*/
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// 
// void Audio_Management(void){
// 	
// 	// Analisys of States
// 	switch(ST_Audio)
//     {
// 
// 		case POW_0:
//       SPK_MUTE = SPK_MUTE_STBY;     // idle
//       MIC_MUTE = MIC_MUTE_STBY;     // idle
//       EN_AUDIO = 0;     // idle
//       timer_audio_poweron = TIME_IMP_AUDIO1;
//       ST_Audio = POW_1;
// 		break;
// 
// 		case POW_1:
//       if(timer_audio_poweron == 0)
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         ST_Audio = POW_2;
//         timer_audio_poweron = TIME_IMP_AUDIO2;
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 0;     // idle
//         }  // endif
// 		break;
// 
// 		case POW_2:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK != 0)
//           {  // microphone not in mute
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_ACTIVE;     // OFF MIC
//           EN_AUDIO = 1;
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = POW_3;
//           }
//         else  
//           {  // microphone in mute --> no pulse
//           ST_Audio = POW_3;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case POW_3:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK != 0)
//           {  // microphone not in mute
//           timer_audio_poweron = TIME_IMP_AUDIO4;    // wait
//           ST_Audio = POW_2;
//           }
//         else  
//           {  // microphone in mute --> evolution
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_STBY;     // idle
//           EN_AUDIO = 1;
//           timer_audio_poweron = TIME_IMP_AUDIO4;
//           ST_Audio = POW_4;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_ACTIVE;     // OFF MIC
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case POW_4:
//       if(timer_audio_poweron == 0)
//         {
//         // SPK_MUTE = 0;     // OFF SPK    
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         timer_audio_poweron = TIME_IMP_AUDIO5;
//         ST_Audio = POW_5;
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case POW_5:
//       if(timer_audio_poweron == 0)
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         timer_audio_poweron = TIME_IMP_AUDIO6;
//         ST_Audio = POW_6;
//         }
//       else  
//         {
//         // SPK_MUTE = 0;     // OFF SPK    
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case POW_6:
//       if(timer_audio_poweron == 0)
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 0;     // idle
//         ST_Audio = AUDIO_IDLE;
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case OFF_HOOK_0:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK == 0)
//           {  // microphone in mute --> activate pulse
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//         EN_AUDIO = 1;
//         timer_audio_poweron = TIME_IMP_AUDIO3;
//         ST_Audio = OFF_HOOK_1;
//           }
//         else  
//           {  // microphone not in mute --> close
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_STBY;     // idle
//           EN_AUDIO = 1;
//           ST_Audio = AUDIO_IDLE;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case OFF_HOOK_1:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK == 0)
//           {  // microphone still in mute --> re-activate pulse
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = OFF_HOOK_0;
//           }
//         else  
//           {  // microphone not in mute --> close
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         EN_AUDIO = 1;
//         ST_Audio = AUDIO_IDLE;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case ON_HOOK_0:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK != 0)
//           {  // microphone not in mute --> activate pulse
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//           EN_AUDIO = 1;
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = ON_HOOK_1;
//           }
//         else  
//           {  // microphone in mute --> close
//           ST_Audio = ON_HOOK_1;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case ON_HOOK_1:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK != 0)
//           {  // microphone not in mute --> re-activate pulse
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = ON_HOOK_0;
//           }
//         else  
//           {  // microphone in mute --> close
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_STBY;     // idle
//           EN_AUDIO = 0;
//           ST_Audio = AUDIO_IDLE;
//           Audio(OFF);                //!!!! for muovere anche EN_Z
// V1.1          MODO_State = IDLE;         //!!!! solo for non andare temperaneamente in audio and riattivare EN_AUDIO
//   
// V1.1           // switch off video
// V1.1           Video(VIDEO_OFF);
// V1.1           EN_RELE_VIDEO = RELE_VIDEO_OFF;
// V1.1           Disable_osd();
// V1.1           Clear_dm_osd();
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;       // idle
//         MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case MIC_IN_MUTE_0:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK != 0)
//           {  // microphone not in mute --> activate pulse
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//           EN_AUDIO = 1;
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = MIC_IN_MUTE_1;
//           }
//         else  
//           {  // microphone in mute --> close
//           ST_Audio = MIC_IN_MUTE_1;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case MIC_IN_MUTE_1:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK != 0)
//           {  // microphone not in mute --> re-activate pulse
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = MIC_IN_MUTE_0;
//           }
//         else  
//           {  // microphone in mute --> close
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_STBY;     // idle
//           EN_AUDIO = 0;
//           ST_Audio = AUDIO_IDLE;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;       // idle
//         MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//         EN_AUDIO = 1;
//         }  // endif
// 		break;
// 
// 		case MIC_OUT_OF_MUTE_0:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK == 0)
//           {  // microphone in mute --> activate pulse
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = MIC_OUT_OF_MUTE_1;
//           }
//         else  
//           {  // microphone not in mute --> close
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_STBY;     // idle
//           ST_Audio = AUDIO_IDLE;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_STBY;     // idle
//         }  // endif
// 		break;
// 
// 		case MIC_OUT_OF_MUTE_1:
//       if(timer_audio_poweron == 0)
//         {
//         if(MUTE_OK == 0)
//           {  // microphone still in mute --> re-activate pulse
//           timer_audio_poweron = TIME_IMP_AUDIO3;
//           ST_Audio = MIC_OUT_OF_MUTE_0;
//           }
//         else  
//           {  // microphone not in mute --> close
//           SPK_MUTE = SPK_MUTE_STBY;     // idle
//           MIC_MUTE = MIC_MUTE_STBY;     // idle
//           ST_Audio = AUDIO_IDLE;
//           }  // endif
//         }
//       else  
//         {
//         SPK_MUTE = SPK_MUTE_STBY;     // idle
//         MIC_MUTE = MIC_MUTE_ACTIVE;     // activate MIC
//         }  // endif
// 		break;
// 
// 
// 		case AUDIO_IDLE:
// 		break;
// 
// 		
// 		default:
// 		break;
// 	}
// 		
// 	
// }

//-----------------------------------------------------------------------------
//  Update the video with entered string
//-----------------------------------------------------------------------------





/*
** ===================================================================
**     Interrupt handler : isrVsci1rx
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVsci1rx(void)
{
	int_k = SCI1S1;        // read status register for azzerare flag
	  if (index_buffer_rx_JD< LUNGH_BUFF_RX_JD){
		rx_buf_Program[index_buffer_rx_JD] = SCI1D;
		if(!index_buffer_rx_JD)
		{
			if(rx_buf_Program[index_buffer_rx_JD] != 0x7e)
			{
				//index_buffer_rx_JD = 0; //if the start code not correct, it should clear the buffer
				return;
			}
		}
		index_buffer_rx_JD++;
		if(rx_buf_Program[index_buffer_rx_JD-1] == 0x55 && rx_buf_Program[index_buffer_rx_JD-2] == 0xaa && index_buffer_rx_JD >3)
		{
			//frame need done
			JD_RxDone = 1;
			index_buffer_rx_JD = 0;
		}
		else if(rx_buf_Program[index_buffer_rx_JD-1] == 0xaa && rx_buf_Program[index_buffer_rx_JD-2] == 0x55 && index_buffer_rx_JD >3)
		{
			index_buffer_rx_JD = 0;
		}
	  }
	  else
	  {
	  	index_buffer_rx_JD= 0;
		rx_buf_Program[index_buffer_rx_JD] = SCI1D; //we have to read the data away
	  }
  
}
/* end of isrVsci1rx */

/*
** ===================================================================
**     Interrupt handler : isrVsci1tx
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVsci1tx(void)
{
	if(SCI1S1_TDRE)
	{
   		if (index_buffer_tx_JD< tx_len_JD){
	     		SCI1D = tx_buf_Program[index_buffer_tx_JD];
	     		index_buffer_tx_JD++;
		}
		else           
		{
		   	SCI1C2_TIE = 0;	//SCI1C2 = 0x2C;   //disable interrupt
		   	JD_TX_Frame_Time = 100;// after 500ms without ack we would resend again
		 }
	}
}
/* end of isrVsci1tx */


/*
** ===================================================================
**     Interrupt handler : isrVsci1err
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVsci1err(void)
{
  int_k = SCI1S1;        // read status register for azzerare flag
  int_k = SCI1D;
//   if((status_reg & (SCI1S1_OR_MASK | SCI1S1_NF_MASK | SCI1S1_FE_MASK | SCI1S1_PF_MASK)) != SCI1S1_NF_MASK)
//     {  // non segnalo error solo sul NOISY FLAG 
//     int_k |= status_reg;
//     }  // endif

}
/* end of isrVsci1err */
/*
** ===================================================================
**     Interrupt handler : isrVsci2rx
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVsci2rx(void)
{//read data from RS485
	int_k = SCI2S1;
	if(KNX_R_RC_P< 16)
	{
		KNX_Buf_RC[KNX_R_RC_P] = SCI2D;
		KNX_R_RC_P++;
		KNX_R_RC_Verify = 0;
		
	}
	else
	{
		KNX_R_RC_P = 0;
		KNX_Buf_RC[KNX_R_RC_P] = SCI2D;
	}
  	
}
/* end of isrVsci1rx */

/*
** ===================================================================
**     Interrupt handler : isrVsci2tx
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVsci2tx(void)
{
	if(SCI2S1_TDRE)
	{
   		if (KNX_R_TX_P < KNX_Buf_TX_Length){
	     		SCI2D = KNX_Buf_TX[KNX_R_TX_P];
	     		KNX_R_TX_P++;
		}
		else           
		{
		   	SCI2C2_TIE = 0;	//SCI1C2 = 0x2C;   //disable interrupt
		   	KNX_TX_Frame_Time = 2;// after 20ms next frame is possible
		 }
	}
}
/* end of isrVsci1tx */


/*
** ===================================================================
**     Interrupt handler : isrVsci2err
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVsci2err(void)
{
  int_k = SCI2S1;        // read status register for azzerare flag
  int_k = SCI2D;
//   if((status_reg & (SCI1S1_OR_MASK | SCI1S1_NF_MASK | SCI1S1_FE_MASK | SCI1S1_PF_MASK)) != SCI1S1_NF_MASK)
//     {  // non segnalo error solo sul NOISY FLAG 
//     int_k |= status_reg;
//     }  // endif

}
// 
// //----------------------------------------------------------------------
// // Management of conexant communications     
// // No reception and transmission queue
// //----------------------------------------------------------------------
// void Gest_Conexant_Serial_Link(void){
//   
//   switch(Conexant_Link_Status){
//   
//   	case CNX_IDLE:
//       // Check that previous string is used 
//   	  if(Send_On_Uart_1){
//   	    Conexant_Link_Status = CNX_START_TX;
//       }   
//     break;
//   
//    	case CNX_START_TX:
//      // Create the string. Example !0074=FD  -->  +3dB
//   	 buffer_tx[0] = '!';
//   	 buffer_tx[1] = '0';
//   	 buffer_tx[2] = '0';
//   	 buffer_tx[3] = '7';
//   	 buffer_tx[4] = '4';
//   	 buffer_tx[5] = '=';
//   	 buffer_tx[6] = Hex_2_Ascii((Vol_to_set & 0xF0) >> 4);
//   	 buffer_tx[7] = Hex_2_Ascii((Vol_to_set & 0x0F));
//   	 buffer_tx[8] = 0xD;  //cr
//      index_buffer_tx = 0;
//      n_byte_to_tx = 9;
//      // Start the transmission
//   	 Conexant_Link_Status = CNX_W4_XMITTER_EMPTY;
// 
// //      // Creo the stringa esempio !0074=FD  --> for settare +3dB
// //   	 buffer_tx[0] = 'I';
// //   	 buffer_tx[1] = 0xD;  //cr
// //      index_buffer_tx = 0;
// //      n_byte_to_tx = 2;
// //      // Start the transmission
// //   	 Conexant_Link_Status = CNX_W4_XMITTER_EMPTY;
//   	break;
// 
//    	case CNX_W4_XMITTER_EMPTY:
//   	  if(SCI1S1_TDRE){
//         if(n_byte_to_tx){
//           SCI1D = buffer_tx[index_buffer_tx];
//           index_buffer_tx++;
//           n_byte_to_tx--;
//           if(!n_byte_to_tx)
//             Conexant_Link_Status = CNX_CLOSE_TX;
//         }
//         else{
//           Conexant_Link_Status = CNX_CLOSE_TX;
//         }
//       }
//     break;
//   
//    	case CNX_CLOSE_TX:
//    	  Send_On_Uart_1 = 0;      // Clear the request
//       index_buffer_tx = 0;
//       n_byte_to_tx = 0;
//       Conexant_Link_Status = CNX_IDLE;
//       // no wait for answer
//   	break;
//   
// //   	case CNX_START_TX:
// //   	break;
//   
//   	default:
//   	break;
//   }
//   
// }           
// //----------------------------------------------------------------------

/*
//----------------------------------------------------------------------
// Conversion from hex to ascii 
//----------------------------------------------------------------------
unsigned char Hex_2_Ascii (unsigned char val){
  
  unsigned char Val_Ascii;
  
  switch(val){

  	case 0:
  	case 1:
  	case 2:
  	case 3:
  	case 4:
  	case 5:
  	case 6:
  	case 7:
  	case 8:
  	case 9:
      Val_Ascii = val + 0x30;
  	break;
  
  	case 0xa:
  	case 0xb:
  	case 0xc:
  	case 0xd:
  	case 0xe:
  	case 0xf:
      Val_Ascii = val + 0x37;
  	break;
  
  	default:
  	break;
  }
  
  return Val_Ascii;
}
*/
//----------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void SwitchTalkToCallModule(void)
{	
	if(MODO_State == VIDEO_OFF_HOOK_WAIT ||MODO_State == SELFINSERTION_OFF_HOOK_WAIT ||MODO_State == IN_COMMUNICATION_VIDEO ||MODO_State == IN_COMMUNICATION_SELFINSERTION)
	{
		SetControl_State(Talk_To_CallModule_NotHide, K_AutoSwitch, ON);	//new quest on switchboard call the eye icon should display		
	}
	else
	{
		SetControl_State(Talk_To_CallModule_NotHide, K_AutoSwitch, OFF);			
	}
	
	if(MODO_State == SELFINSERTION_OFF_HOOK_WAIT || MODO_State == IN_COMMUNICATION_SELFINSERTION)
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AutoSwitch, ON);//set blink
	else
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AutoSwitch, OFF);//not blink	
	if(MIC_Muted)		
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_MicroPhone_Mute, ON);//set blink
	else
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_MicroPhone_Mute, OFF);//not link
	//audio icon
	SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AnswerCall, OFF);//not blink
	//show stair light icon
	SetControl_State(Talk_To_CallModule_NotHide, K_LightTurnOn, ON);
	SetControl_State(Talk_To_CallModule_NotHide, K_Door_Open_1, ON);
	SetControl_State(Talk_To_CallModule_NotHide, K_Door_Open_2, ON);
	SetSwitchIcon(ChangeButton_Icon, Talk_To_CallModule_NotHide, K_AnswerCall, 0, 0x02, 0x28 + ICON);
	PageStatus = Talk_To_CallModule_NotHide;
	LoadJD_Menu(Talk_To_CallModule_NotHide);
}

//----------------------------------------------------------------------------------------
void SwitchTalkToSwitchBoard(void)
{
	
	if(MODO_State == SELFINSERTION_OFF_HOOK_WAIT || MODO_State == IN_COMMUNICATION_SELFINSERTION)
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AutoSwitch, ON);//set blink
	else
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AutoSwitch, OFF);//set blink
	if(MIC_Muted)		
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_MicroPhone_Mute, ON);//set blink
	else
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_MicroPhone_Mute, OFF);//set blink	
	//audio icon
	SetButton_State(0x01, Talk_To_CallModule_NotHide, K_AnswerCall, OFF);//not blink
	SetControl_State(Talk_To_CallModule_NotHide, K_LightTurnOn, OFF);
	SetControl_State(Talk_To_CallModule_NotHide, K_Door_Open_1, OFF);
	SetControl_State(Talk_To_CallModule_NotHide, K_Door_Open_2, OFF);
	SetSwitchIcon(ChangeButton_Icon, Talk_To_CallModule_NotHide, K_AnswerCall, 0, 0x02, 0x28 + ICON);
	if(MODO_State == VIDEO_OFF_HOOK_WAIT ||MODO_State == SELFINSERTION_OFF_HOOK_WAIT ||MODO_State == IN_COMMUNICATION_VIDEO ||MODO_State == IN_COMMUNICATION_SELFINSERTION)
	{
		SetControl_State(Talk_To_CallModule_NotHide, K_AutoSwitch, ON);	//new quest on switchboard call the eye icon should display		
	}
	else
	{
		SetControl_State(Talk_To_CallModule_NotHide, K_AutoSwitch, OFF);			
	}
	PageStatus = Talk_To_CallModule_NotHide;
	LoadJD_Menu(Talk_To_CallModule_NotHide);

		
}

//-----------------------------------------------------------------------
void SwitchTalkMenu(void)
{
	If_need_Wakeup_LCD_CallBack();
	if(MODO_State == SELFINSERTION_OFF_HOOK_WAIT || MODO_State == IN_COMMUNICATION_SELFINSERTION)
	{
		SwitchTalkToCallModule();
	}
	else
	{
		switch(kind_of_my_communication)
		{
			case FROM_MAIN_CM:
			case FROM_SECONDARY_CM:
			case FROM_SWITCHBOARD: // change due to 3780 and the SPT would be changed
			case FROM_IP_PHONE:
				
				SwitchTalkToCallModule();				
			break;
			//case FROM_SWITCHBOARD:
			default:
				SwitchTalkToSwitchBoard();
			return;
		}
	}
}

//-----------------------------------------------------------------------
void SaveEvent(void)
{
	if((tone_proc & RING_REQUEST) || Timer_Wait_Tone)
		return;
	
	if(Flag.bits.Set_AB_Server && !TPM2SC_TOIE)
	{
		Flag.bits.Set_AB_Server = 0;
		Presence_Absence_Management();
	}
	
	if(Flag.bits.SavePBRQ && !TPM2SC_TOIE)
	{
		Flag.bits.SavePBRQ = 0;
		SaveInfQueue(&SavePhoneBookRecordQueue);
	}
	
	if(Flag.bits.DelPhoneBook && !Flag.bits.SavePBRQ && SaveInf == NULL && FlagPage1 != 0x51)
	{
		Flag.bits.DelPhoneBook = 0;
		ShowPhoneBook_E_D(PhoneBookDelSelect);
	}
	
	if(Flag.bits.SaveAP && !TPM2SC_TOIE)
	{
		Flag.bits.SaveAP = 0;
		SaveInfQueue(&SaveAudioPara);
	}
	
	if(Flag.bits.editPhoneBook && !Flag.bits.SavePBRQ && SaveInf == NULL && FlagPage1 != 0x51)
	{
		Flag.bits.editPhoneBook = 0;
		ShowPhoneBook_E_D(PhoneBookEditSelect);
	}
	
	if(Flag4.bits.DefaultS && !TPM2SC_TOIE)
	{
		Flag4.bits.DefaultS = 0;
		SaveInfQueue(&DefaultSave);
		JD_start_wait = 2;
		
	}
}


void Mic_State(unsigned char mic_state)
{
	MIC_Muted = mic_state;
	//MIC_MUTE = MIC_Muted;
	if(MIC_Muted)		
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_MicroPhone_Mute, ON);//set blink
	else
		SetButton_State(0x01, Talk_To_CallModule_NotHide, K_MicroPhone_Mute, OFF);//not link
}
//******************************************************************************

void PutTC_PCN_ON(void)
{
	if(!Pin_IO_Set_Delay)
	{	
		if(TC_PCM1 == OFF && !TEST_AA)
		{
			TC_PCM1 = ON;  
			TEST_AA=5;
		}
	}
	
	if(TC_PCM1_Shortcut_Detect)//short cut detect
	{
		if(!TEST_AA)
		{
			TC_PCM1 = OFF;
			TEST_AA = 255;
		}
	}
	
}

void Mic_StateSet(void)
{
	if(MIC_Muted)
		PTTEI = 1;
	else
		PTTEI = 0;
}

void PCM_Audio_Set(void)
{
	if(PCM_Audio_Switch)
	{//handset
		
		IN4 = 1;						
		PTTIE = 1;
		PTTEI = 1;
	}
	else
	{//modo
		
		IN4 = 0;						
		PTTIE = 0;
		Mic_StateSet();
	}	
}


void PSTN_State(void)
{
	EN_AUDIO  = FONIA_OFF;
	TC_PCM1 = OFF;
	Audio_Switch = V_To_PCM;//DEFAULT

	PCM_Audio_Set();
	HK = OFF;
	EN_Z = ON;
	
	REL2 = OFF;
	REL6 = ON;
	
	System_Video = OFF;

}

void Tel_Video_Menu_Init(void)
{
	PageStatus = Menu_Telephone_Video;
	 LoadJD_Menu(Menu_Telephone_Video);
	 SetControl_State(Menu_Telephone_Video, 7, ON);
	 SetControl_State(Menu_Telephone_Video, 6, ON);
	 SetButton_State(0x01, PCM_Auto_Insert - 1, K_OK, OFF);//not blink
	 temp9 = 4;
}
//---------------------------------------------------------------------------

uchar Ex_Module_StateManagement(void)
{
	if(OLD_Sub_MODO_State1 != Sub_MODO_State)
	{
		OLD_Sub_MODO_State1 = Sub_MODO_State;
		if((OLD_Sub_MODO_State1==PSTN_Talking)||(OLD_Sub_MODO_State1==PSTN_Talking_Video))
		{
			TPM1SC = 0x48;//TPM2_Init();
			Common_falg.bits.EN_Capture=1;
			TPM2_time_overflow_times=0;
			TPM2_ch1_interrupt_times=0;
			TPM2_capture_times=0;
			blind_audio_period_limit=0;
			blind_audio_dec_times=0;
			blind_audio_period_times=0;
		}
		else
		{
			//SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
			TPM1SC = 0x08;//TPM2_SC = (TPM_SC_CMOD(0x00) | TPM_SC_PS(0x00)); // Stop the counter and clear the interrupt flag 
			Common_falg.bits.EN_Capture=0;
		}
	}


	switch(Sub_MODO_State)
	{
		case EX_IDLE:
			if(OLD_Sub_Modo_State != Sub_MODO_State){
				OLD_Sub_Modo_State = Sub_MODO_State;
				PSTN_Call_TimeOut_Cnt = 0;
				DTMF_State = DTMF_Idle;
				EventHandle_Flag.bits.PSTNOnRingTone = 0;
				EventHandle_Flag.bits.StopToneNeed = 0;
				TC_PCM1 = OFF;PCM_OnOff_Interval=30;
				PCMcallingTime = 0;
				PTTEI = 0;
				CCTV_EN = 0;				
			}
			HK = ON;
		return 1;
		
		case PCM1CALLING:
			if(OLD_Sub_Modo_State != Sub_MODO_State){
			        OLD_Sub_Modo_State = Sub_MODO_State; 			      
			        PCM_Audio_Switch = To_Modo;
				//Init_Talk_Menu();
				If_need_Wakeup_LCD_CallBack();
				SetButton_State(0x01, PCM_Auto_Insert - 1, 3, OFF);//not blink
				SetButton_State(0x01, PCM_Auto_Insert - 1, K_MicroPhone_Mute, OFF);//not link
				SetControl_State(Menu_Telephone_Video, 7, OFF);
				SetControl_State(Menu_Telephone_Video, 6, OFF);
				PageStatus = PCM_Auto_Insert;
				LoadJD_Menu(PCM_Auto_Insert -1);			
				 CCTV_EN = 0;	
			        //Pcm_TalkTime = 30;
			      }			      
			      PutTC_PCN_ON();				  
			      Audio_Switch = V_To_PCM;
				System_Video = OFF;
			      //Audio(FONIA_OFF);
				EN_AUDIO  = FONIA_ON;
   				EN_Z      = FONIA_ON; // 20130924 steven request
			      	Video(VIDEO_ON);                     //V1.1
			      	PCM_Button_Detect_Wait = PCM_Call_Button_Detect_Time;											
				
		break;
		case IN_COMMUNICATION_PCM1:
			if(OLD_Sub_Modo_State != Sub_MODO_State){
			        OLD_Sub_Modo_State = Sub_MODO_State; 
			        Pcm_TalkTime = 120;			        			        
			        Audio(FONIA_ON);			        
			        //SwitchTalkToCallModule();			        
			        System_Video = OFF;
			      }
			         
		      		Video(VIDEO_ON);                     //V1.1	          	
				EN_AUDIO  = FONIA_OFF;
				PutTC_PCN_ON();
				Audio_Switch = V_To_PCM;
				REL2 = OFF;
				REL6 = OFF;
				PCM_Audio_Set();
				PCM_Button_Detect_Wait = PCM_Call_Button_Detect_Time;
			
		break;
		case Mornitor_PCM1:
			//for video adjust
			if(OLD_Sub_Modo_State != Sub_MODO_State)
			{
				OLD_Sub_Modo_State = Sub_MODO_State; 
				CCTV_EN = 0;
				Pcm_TalkTime = 300;		
				SetButton_State(0x01, PCM_Auto_Insert - 1, 3, OFF);//not blink
				SetButton_State(0x01, PCM_Auto_Insert - 1, K_MicroPhone_Mute, OFF);//not link
   				SetSwitchIcon(ChangeButton_Icon, PCM_Auto_Insert - 1, 3, 0, 0x00, 44);	
				if(PageStatus != VideoAdjust)
				{
					Audio(FONIA_ON);
					PageStatus = PCM_Auto_Insert;
					LoadJD_Menu(PCM_Auto_Insert - 1);//share with same menu with telephone
				}
			 }
			
	      		Video(VIDEO_ON);                     //V1.1		          	
			EN_AUDIO  = FONIA_OFF;
			PutTC_PCN_ON();				
			Audio_Switch = V_To_PCM;
			REL2 = OFF;
			System_Video = OFF;
			REL6 = OFF;
			IN4 = 0;	
			//set the audio only can listen
			PTTIE = 0;						
			PTTEI = 1; //micro set to mute
		
		break;
		case Mornitor_CCTV:
			System_Video = OFF;
			CCTV_EN = 1;
			Audio_Switch = V_To_PCM;
		break;
		case PSTN_Talking:
			if(OLD_Sub_Modo_State != Sub_MODO_State)
			{
			       OLD_Sub_Modo_State = Sub_MODO_State; 
				if(PCM_Audio_Switch == To_Modo)
				{
				 	SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);	
				}
				
				PageStatus = PSTN_Telephone;
			       LoadJD_Menu(PSTN_Telephone);
				JDFunctionCallBack = &Tel_MemoryName_Init;	
				VolumeCNT = 2;
			}
			CCTV_EN	 = 0;
			if(DTMF_State != Tel_Talk)
			{
				PTTEI = 1; //set microphone as mute for DTMF work fine
			}
			else
				PSTN_State();
		break;
		case PSTN_Calling:
			if(OLD_Sub_Modo_State != Sub_MODO_State)
			{
			        OLD_Sub_Modo_State = Sub_MODO_State; 
				 Video(VIDEO_ON);                     //V1.1
				 If_need_Wakeup_LCD_CallBack();
				 SetButton_State(0x01, PSTN_Telephone, 16, OFF);
				 PageStatus = PSTN_Telephone;
				 LoadJD_Menu(PSTN_Telephone);
				 //LoadJD_SleepMenu(ON);
				 SetTimeDelay = 10;//delay for make the name display correctly
				 JDFunctionCallBack = &Tel_MemoryName_Init;
				 //RingRepeatTimeCnt = F_RingRepeatT;	

			}

			Common_falg.bits.RD_Tel_Num_flag=1;

			
		break;
		case PSTN_Talking_Video:
			if(OLD_Sub_Modo_State != Sub_MODO_State)
			{
			        OLD_Sub_Modo_State = Sub_MODO_State;
				 if(PCM_Audio_Switch == To_Modo)
				 {
				 	SetSwitchIcon(ChangeButton_Icon, Menu_Telephone_Video, K_OK, 0, 0x00, 45);
				 }
				 /*
				 PageStatus = Menu_Telephone_Video;
				 LoadJD_Menu(Menu_Telephone_Video);
				 SetControl_State(Menu_Telephone_Video, 7, ON);
				 SetControl_State(Menu_Telephone_Video, 6, ON);
				 SetButton_State(0x01, PCM_Auto_Insert - 1, K_OK, OFF);//not blink
				 temp9 = 4;
				 */
				 Tel_Video_Menu_Init();				 
			        VolumeCNT = 2;
				 
			}
			CCTV_EN	 = 1;
			if(DTMF_State != Tel_Talk)
			{
				PTTEI = 1; //set microphone as mute for DTMF work fine
			}
			else
				PSTN_State();
			//PSTN_State();
		break;
		case PSTN_Calling_Video:
			if(OLD_Sub_Modo_State != Sub_MODO_State)
			{
				OLD_Sub_Modo_State = Sub_MODO_State; 
				Video(VIDEO_ON);                     //V1.1
				If_need_Wakeup_LCD_CallBack();
				/*
				PageStatus = Menu_Telephone_Video;
				LoadJD_Menu(Menu_Telephone_Video);
				SetControl_State(Menu_Telephone_Video, 7, ON);
				SetControl_State(Menu_Telephone_Video, 6, ON);
				SetButton_State(0x01, PCM_Auto_Insert - 1, K_OK, OFF);//not blink
				temp9 = 4;
				*/
				Tel_Video_Menu_Init();
				SetSwitchIcon(ChangeButton_Icon, Menu_Telephone_Video, K_OK, 0, 0x00, 44);
				
				//LoadJD_SleepMenu(ON);
				//RingRepeatTimeCnt = F_RingRepeatT;

			}
			CCTV_EN = 1;
			Audio_Switch = V_To_PCM;

			Common_falg.bits.RD_Tel_Num_flag=1;
			
		break;
		case PSTN_Dial_Number:
			
		break;
	}
	return 0;
}



void External_Button_Scan(void)
{
	//uchar button_value;
	//button_value = PTAD;
	//audio phone lock key
	if(JD_start_wait)
		return;
	if(!Audio_Phone_Lock_Button && OldAudioPhone_Lock_button)
	{
		OldAudioPhone_Lock_button = 0;
		Lock_button_time = 5;
	}
	else if(Audio_Phone_Lock_Button)
	{
		OldAudioPhone_Lock_button = 1;
		Lock_button_time = 0;
	}
	//call to switchboard
	//changed due to new feature add
	//comment due to dipswitch remove
	//if(Flag3.bits.GasScan_Enable)
	//{
		//scan gas detector
	if(Gas_Detector_Button && !OldGasDectorStatus)
	{
		OldGasDectorStatus = 1;
		if(!Alarm_Enable_Time_Delay_CNT)
			Gas_detector_button_time = 2; //if alarm keep over 5s, the alarm information will sent to 
	}
	else if(!Gas_Detector_Button)
	{
		OldGasDectorStatus = 0;
		Gas_detector_button_time = 0;
		GasAlarmF = 0; //clear the Flag of gas alarm
		AlarmNeed_Reset_Flag &= 0xFBFF; //clear for bit 11
	}

	
	if(!Audio_Call_Switchboard_Button && OldAudioPhone_Call_switchboard)
	{
		OldAudioPhone_Call_switchboard = 0;
		call_switchboard_button_time = 5;
	}
	else if(Audio_Call_Switchboard_Button)
	{
		OldAudioPhone_Call_switchboard = 1;
		call_switchboard_button_time = 0;
	}
	
	//hook
	if(AudioPhone_HOOK!=OldAudioPhone_Hook)
	{		
		if(!Hook_button_time)
			Hook_button_time = 5;
	}
	else 
	{		
		Hook_button_time = 0;
	}
	//pcm1 call button
	
	if(!PCM1_Call_MODO_Button && OldPCM1_Call_MODO_Button &&!PCM_OnOff_Interval)
	{

		OldPCM1_Call_MODO_Button = 0;
		PCM1_Call_MODO_Button_time = 5;
	}
	else if(PCM1_Call_MODO_Button)
	{
		OldPCM1_Call_MODO_Button = 1;
		PCM1_Call_MODO_Button_time = 0;
	}
	
}


uchar  GetSum(uchar *p,uchar n)
{
	uchar  i,s;
	for (i =0, s =0x55; i < n; i++, p++) {
	    s ^= *p;
	}
	return s;
	
}
int  VerificaTramaRx(void)
{
	uchar  sum;
	if (KNX_R_len < 3)		return 1;	//verify KO
	sum = GetSum(KNX_Buf,KNX_R_len-1);
	if (sum != KNX_Buf[KNX_R_len-1])		return 2;	//verify KO
	//
	//	clear Rx buffer (vedi trame corte che implicano campo dati a "0"
	//
	KNX_Buf[KNX_R_len -1] = 0;
	return 0; //verify OK
}
void bcc_AvvioTx(void)
{
	KNX_R_TX_P = 0; //send data from first buffer
	KNX_Buf_TX[KNX_Buf_TX_Length++] = GetSum(KNX_Buf_TX,KNX_Buf_TX_Length);
	Con485_D = RS485_TX_Ready;	
	//Flag4.bits.Bus_Useing = 1; // it is not Bi_Diretion as same as HRB
	SCI2C2_TIE = 1; //enable tx and start transmit data
}
void Alarm_System_Data_ACK(void)
{

	uchar y,z;
	InPutAlarm *pp;
	ZoneNameS *ppp;
	Password_Alarmsystem *p1;
	DTMF_Key *pp1;
	z= KNX_Buf[2]&0x0f;
	y = (KNX_Buf[2]&0xf0)>>4;
	//x = (y-1)/4;
	//y = (y-1)%4;
	
	pp=(InPutAlarm *)(EE_Input_Memory1 + (y-1)*sizeof(InPutAlarm));
	KNX_Buf_TX[0] = DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
	KNX_Buf_TX[1] = OPC_DADA_ACK;		// 
	KNX_Buf_TX[2] = KNX_Buf[2];		//
	if(KNX_Buf[2] == 0)
	{
		KNX_Buf_TX[3] = FW_VERSION;
		KNX_Buf_TX[4] = JD_VERSION;//gui version
		KNX_Buf_TX_Length = 5;
		
	}
	else
	{
		switch(z)
		{
			case 0:
				//ack fw version
				(void)memcpy(&KNX_Buf_TX[3],&pp->group_address,12);
				KNX_Buf_TX_Length = 15;
			break;
			case 1:
				(void)memcpy(&KNX_Buf_TX[3],&pp->Input_Status,8);
				KNX_Buf_TX_Length = 11;
			break;
			case 2:
				//zone name
				ppp = (ZoneNameS *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) + (y-1)*sizeof(ZoneNameS));
				(void)memcpy(&KNX_Buf_TX[3],&ppp->name[0],9);
				KNX_Buf_TX_Length = 12;
			break;
			case 3:
				KNX_Buf_TX[3] = AlarmP->Away_Zone_Belong;
				KNX_Buf_TX[4] = AlarmP->Home_Zone_Belong;
				(void)memcpy(&KNX_Buf_TX[5],&AlarmP->Enable_group_address,7);
				KNX_Buf_TX_Length = 12;
			break;
			case 4:
			case 5:
			case 6:
			case 7:
				// tel memory name
				//EE_Page10+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) +3 + ZoneNum*sizeof(ZoneNameS)
				ppp = (ZoneNameS *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) + 3 + ZoneNum*sizeof(ZoneNameS) + (z-4)*sizeof(ZoneNameS)); 
				(void)memcpy(&KNX_Buf_TX[3],&ppp->name[0],9);
				KNX_Buf_TX_Length = 12;
			break;
			case 8:
				//DTMF key
				pp1 = (DTMF_Key *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) + 3 + ZoneNum*sizeof(ZoneNameS) + 4*sizeof(ZoneNameS)); 
				(void)memcpy(&KNX_Buf_TX[3],&pp1->D_key[0],7);
				KNX_Buf_TX_Length = 10;
			break;
			case 9:
				p1 = (Password_Alarmsystem *)(EE_Page_AlarmSystem+SpaceByte);
				(void)memcpy(&KNX_Buf_TX[3],&p1->Installer[0],12);
				KNX_Buf_TX_Length = 15;
			break;
		}
	}
	
	
	bcc_AvvioTx();
	
}
void Alarm_System_Data_Program(void)
{
	uchar y,z;
	InPutAlarm *pp;
	ZoneNameS *ppp;// same with telephone name
	Password_Alarmsystem *p1;
	DTMF_Key *pp1;
	z= KNX_Buf[2]&0x0f;
	y = (KNX_Buf[2]&0xf0)>>4;

	switch(z)
	{
		case 0:
			if(y>0)
			{
				pp=(InPutAlarm *)(&SaveData [(y-1)*sizeof(InPutAlarm)]);
				(void)memcpy(&pp->group_address,&KNX_Buf[3],12);
			}
		break;
		case 1:
			pp=(InPutAlarm *)(&SaveData [(y-1)*sizeof(InPutAlarm)]);
			(void)memcpy(&pp->Input_Status,&KNX_Buf[3],8);
			if(y==10)
			{
				//save data on flash x zone
				WritePage(EE_Input_Memory1);
			}
		break;
		case 2:
			ppp = (ZoneNameS *)(&Memory_S.MsgBuf[(y-1)*sizeof(ZoneNameS)+10]); //+3 we keep the buffer for away zone belong and home zone belong
			(void)memcpy(&ppp->name[0],&KNX_Buf[3],9);
		break;
		case 3:
			SaveZoonBelongInf(KNX_Buf[3],KNX_Buf[4],KNX_Buf[5],KNX_Buf[6],KNX_Buf[7],KNX_Buf[8],KNX_Buf[9],KNX_Buf[10],KNX_Buf[11]);
		break;
		case 4:
		case 5:
		case 6:
		case 7:	
			//save Telephone name for M1 to M4
			ppp = (ZoneNameS *)(&Memory_S.MsgBuf[(z-4)*sizeof(ZoneNameS)]); 
			(void)memcpy(&ppp->name[0],&KNX_Buf[3],9);
			
		break;
		case 8:
			//save DTMF key button
			pp1 = (DTMF_Key *)(&Memory_S.MsgBuf[4*sizeof(ZoneNameS)]); 
			(void)memcpy(&pp1->D_key[0],&KNX_Buf[3],7);
			SaveTelephoneM_name_DTMF_key();
			(void)memset(&Memory_S.MsgBuf[0],0,43);
		break;
		case 9:
			//save installer and user password
			p1 = (Password_Alarmsystem *)(&Memory_S.MsgBuf[0]);
			(void)memcpy(&p1->Installer[0],&KNX_Buf[3],12);
			SavePassword();
			break;
		
	}
	
	
}
//-------------------------------------------------------------

void HA_System_Data_Program(void)
{
	uchar y,z;
	OutPutButtons *pp;
	//ZoneNameS *ppp;
	z= KNX_Buf[2]&0x0f;
	y = (KNX_Buf[2]&0xf0)>>4;
	//x = (y-1)/4;
	//x = y%16;
	
	if(z == 2)
	{
		pp = (OutPutButtons *)(&SaveData [y*sizeof(OutPutButtons)]); //
		(void)memcpy(&pp->Button_Status,&KNX_Buf[3],9);
		if(y==0x0f)
		{
			//save data on flash x zone
			WritePage(EE_HomeAutomation1_1+(Edit_Index-1)*0x200);
			
		}
		/*
		else if(y==0x0f)
		{
			WritePage(EE_HomeAutomation1_1+(Edit_Index-1)*0x200 + 168);
			//ack the version
		}
		*/
	}
	else if(z == 1)
	{
		pp=(OutPutButtons *)(&SaveData [y*sizeof(OutPutButtons)]);
		(void)memcpy(&pp->group_address,&KNX_Buf[3],12);
		
	}
	else if(z == 0)
	{
		Edit_Index = KNX_Buf[4];
	}
}

void HA_System_Data_ACK(void)
{

	uchar y,z;
	OutPutButtons *pp;
	//ZoneNameS *ppp;
	z= KNX_Buf[2]&0x0f;
	y = (KNX_Buf[2]&0xf0)>>4;
	//x = (y-1)/4;
	//x = y%8;
	//if(y>7)
	//{
	pp=(OutPutButtons *)(EE_HomeAutomation1_1+(Edit_Index-1)*0x200+ y*sizeof(OutPutButtons));
	//}
	//else
	//{
	//	pp=(OutPutButtons *)(EE_HomeAutomation1_1+(Edit_Index-1)*0x200+ x*sizeof(OutPutButtons));
	//}
	KNX_Buf_TX[0] = DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
	KNX_Buf_TX[1] = OPC_HA_System_ACK;		// 
	KNX_Buf_TX[2] = KNX_Buf[2];		//

	if(KNX_Buf[2] == 0)
	{
		//ack fw version
		KNX_Buf_TX[3] = FW_VERSION;
		KNX_Buf_TX[4] = JD_VERSION;//gui version
		KNX_Buf_TX_Length = 5;
		Edit_Index = KNX_Buf[4];
	}
	
	else if(z == 2)
	{
		//zone name
		
		(void)memcpy(&KNX_Buf_TX[3],&pp->Button_Status,9);
		KNX_Buf_TX_Length = 12;
		
	}
	else if(z == 1)
	{
		(void)memcpy(&KNX_Buf_TX[3],&pp->group_address,12);
		KNX_Buf_TX_Length = 15;
	}

	bcc_AvvioTx();
	
}


//---------------------------------------------------------------
void HA_System_CCTV_Scenario_Data_Program(void)
{
	uchar y,z;
	Scenario_Setting *p;
	CCTV_Setting *pp;
	Room_Setting *ppp;

	z= KNX_Buf[2]&0x0f;
	y = (KNX_Buf[2]&0xf0)>>4; // from 1 to 9

	
	if(z == 2)
	{
		if(Edit_Index == 3)
		{ //we programing the CCTV and room type information
			ppp = (Room_Setting *)(&SaveData [4*sizeof(CCTV_Setting) + (y-1)*sizeof(Room_Setting)]); //
		
			(void)memcpy(&ppp->Icon_ID,&KNX_Buf[3],sizeof(Room_Setting));
			if(y==12)
			{
				//save data on flash x zone
				WritePage(EE_CCTV_Room);
				
			}
		}
		else
		{
			//for scenario
			p = (Scenario_Setting *)(&SaveData [((Edit_Index-1) * 9 + (y-1))*sizeof(Scenario_Setting)]); //
		
			(void)memcpy(&p->Icon_Protocol_Type,&KNX_Buf[3],1);
			if(y==9 && Edit_Index == 2)
			{
				//save data on flash
				WritePage(EE_Scenario_CCTV);
			}
		}
		
		
	}
	else if(z == 1)
	{
		if(Edit_Index == 3)
		{
			//for cctv setting parameter
			pp=(CCTV_Setting *)(&SaveData [(y-1)*sizeof(CCTV_Setting)]);
			(void)memcpy(&pp->group_address,&KNX_Buf[3],12);
		}
		else
		{
			//for scenario setting
			p = (Scenario_Setting *)(&SaveData [((Edit_Index-1) * 9 + (y-1))*sizeof(Scenario_Setting)]); //
		
			(void)memcpy(&p->group_address,&KNX_Buf[3],12);
		}
	}
	else if(z == 0)
	{
		Edit_Index = KNX_Buf[4];
		SaveData[18*sizeof(Scenario_Setting) + 1] =  KNX_Buf[5];
		SaveData[18*sizeof(Scenario_Setting) + 2] =  KNX_Buf[6];//PCM number
		SaveData[18*sizeof(Scenario_Setting) + 3] =  KNX_Buf[7];//PCM door open time
	}
}

void HA_System_CCTV_Scenario_Data_ACK(void)
{
	uchar y,z;
	Scenario_Setting *p;
	CCTV_Setting *pp;
	Room_Setting *ppp;

	z= KNX_Buf[2]&0x0f;
	y = (KNX_Buf[2]&0xf0)>>4; // from 1 to 9

	if(z == 2)
	{
		if(Edit_Index == 3)
		{ //we programing the CCTV and room type information
			
			ppp=(Room_Setting *)(EE_CCTV_Room+4*sizeof(CCTV_Setting) + (y-1)*sizeof(Room_Setting));
			KNX_Buf_TX[0]=DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
			KNX_Buf_TX[1]=OPC_CCTV_Scenario_ACK;		// 
			KNX_Buf_TX[2]=KNX_Buf[2];		//
			(void)memcpy(&KNX_Buf_TX[3],&ppp->Icon_ID,10);
			KNX_Buf_TX_Length = 13;
		}
		else
		{
			//for scenario
			p = (Scenario_Setting *)(EE_Scenario_CCTV + ((Edit_Index-1)*9 + (y-1))*sizeof(Scenario_Setting)); //
			KNX_Buf_TX[0]=DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
			KNX_Buf_TX[1]=OPC_CCTV_Scenario_ACK;		// 
			KNX_Buf_TX[2]=KNX_Buf[2];		//
			(void)memcpy(&KNX_Buf_TX[3],&p->Icon_Protocol_Type,1);
			KNX_Buf_TX_Length = 4;	
	
		}
		
		
	}
		
	else if(z == 1)
	{
		if(Edit_Index == 3)
		{ //we programing the CCTV and room type information
			
			pp=(CCTV_Setting *)(EE_CCTV_Room+(y-1)*sizeof(CCTV_Setting));
			KNX_Buf_TX[0]=DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
			KNX_Buf_TX[1]=OPC_CCTV_Scenario_ACK;		// 
			KNX_Buf_TX[2]=KNX_Buf[2];		//
			(void)memcpy(&KNX_Buf_TX[3],&pp ->group_address,12);
			KNX_Buf_TX_Length = 15;
		}
		else
		{
			//for scenario
			p = (Scenario_Setting *)(EE_Scenario_CCTV + ((Edit_Index-1)*9 + (y-1))*sizeof(Scenario_Setting)); //
			KNX_Buf_TX[0] = DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
			KNX_Buf_TX[1]=OPC_CCTV_Scenario_ACK;		// 
			KNX_Buf_TX[2]=KNX_Buf[2];		//
			(void)memcpy(&KNX_Buf_TX[3],&p->group_address,12);
			KNX_Buf_TX_Length = 15;	
	
		}
		
	}
	
	else if(KNX_Buf[2] == 0)
	{
		//ack fw version
		KNX_Buf_TX[0] = DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
		KNX_Buf_TX[1] = OPC_CCTV_Scenario_ACK;		// 
		KNX_Buf_TX[2] = KNX_Buf[2];		//
		KNX_Buf_TX[3] = KNX_Buf[4];
		KNX_Buf_TX[4] = FW_VERSION;//gui version
		KNX_Buf_TX[5] = F_Scenarioshortcut;
		KNX_Buf_TX[6] = F_PCM_Number;
		KNX_Buf_TX[7] = F_PCM_Door_Open_Time;
		KNX_Buf_TX_Length = 8;
		Edit_Index = KNX_Buf[4];
	}
	
	bcc_AvvioTx();
	
}


void Frame_HRB_Verify(void)
{
	(void)memcpy(&KNX_Buf,&KNX_Buf_RC,KNX_R_RC_P);
	KNX_R_len = KNX_R_RC_P;
	KNX_R_RC_P = 0; //clear len for next RX
	KNX_R_RC_P_old = 0;
	HRB_TX_Time_CNT = 3;
	if(!VerificaTramaRx())
	{
		//a frame rx OK
		
		
			//RxDone = 1;
			if(KNX_Buf[0] == PALM_TO_DEC && KNX_Buf[1] == OPC_Alarm_System_Wirte)
			{
					//if (!Pres_BT) 
					//{
					Alarm_System_Data_Program();
					ReceiveDataAck();
					NoKeyTime = 5;
					//}
					//return;
			}
			else if(KNX_Buf[0] == PALM_TO_DEC && KNX_Buf[1] == OPC_Alarm_System_Read)
			{
					//if (!Pres_BT) 
					//{
						Alarm_System_Data_ACK();
						
					//}
					//return;
			}

			else if(KNX_Buf[0] == PALM_TO_DEC && KNX_Buf[1] == OPC_HA_System_Wirte)
			{
					//if (!Pres_BT) 
					//{
					HA_System_Data_Program();
					ReceiveDataAck();
					NoKeyTime = 5;
					//}
					//return;
			}

			else if(KNX_Buf[0] == PALM_TO_DEC && KNX_Buf[1] == OPC_HA_System_Read)
			{
					//if (!Pres_BT) 
					//{
						HA_System_Data_ACK();
						
					//}
					//return;
			}

			else if(KNX_Buf[0] == PALM_TO_DEC && KNX_Buf[1] == OPC_CCTV_Scenario_Wirte)
			{
					//if (!Pres_BT) 
					//{
						HA_System_CCTV_Scenario_Data_Program();
						ReceiveDataAck();
						NoKeyTime = 5;
					//}
					//return;
			}
			else if(KNX_Buf[0] == PALM_TO_DEC && KNX_Buf[1] == OPC_CCTV_Scenario_Read)
			{
					//if (!Pres_BT) 
					//{
						HA_System_CCTV_Scenario_Data_ACK();
						
					//}
					//return;
			}
		//
	}
	
}

void ReceiveDataAck(void)
{
	KNX_Buf_TX[0] = DEC_TO_PALM;	// SRC=DEC - DEST=BROADCAST
	KNX_Buf_TX[1] = RC_DADA_ACK;		// 
	KNX_Buf_TX[2] = 1;		//	
	//KNX_Buf_TX[3] = 0x09;		//
	KNX_Buf_TX_Length = 3;			
	bcc_AvvioTx();
}


void InitSlaveCpu(void)
{
	unsigned char i;
	SpiSendBuf[0]=0xA5;SpiSendBuf[1]=0xA6;SpiSendBuf[2]=0xA5;SpiSendBuf[3]=0xA6;
	
	DisableInterrupts;
	for(i = 0; i<4; i++)
	{
		Nop;Nop;Nop;Nop;Nop;
		SS= OFF;
		while( !SPI1S_SPTEF);SPI1D =SpiSendBuf[i];
		while( !SPI1S_SPRF);Rc_Tel_num[0] = SPI1D;
		SS= ON;
	}
	EnableInterrupts; 
}

void Rc_Tel_Num(void)      
{   
	unsigned char i;
	
	if(((PageStatus==PSTN_Telephone)||(PageStatus==Menu_Telephone_Video))&&(!EX)&&(!TelRdFrameTime))  	
	
	//if(Sub_MODO_State == PSTN_Talking)
	//  ;
	//else if(Sub_MODO_State == PSTN_Talking_Video)
	//  ;
	//else if((!EX)&&(!TelRdFrameTime))  
	{		
		DisableInterrupts;
		SS= OFF;
		while( !SPI1S_SPTEF);SPI1D = 0xA5;
  		while( !SPI1S_SPRF);Rc_Tel_num[0] = SPI1D;            // svuoto
			SS= ON;
		Nop;Nop;Nop;Nop;Nop;

		SS= OFF;
  		while( !SPI1S_SPTEF);SPI1D = 0xA6;
  		while( !SPI1S_SPRF);Rc_Tel_num[1] = SPI1D;            // svuotoSS= ON;
		SS= ON;
		
		for(i = 0; i<20; i++)
		{
			Nop;Nop;Nop;Nop;Nop;
			SS= OFF;
			while( !SPI1S_SPTEF);SPI1D = 0x00;
			while( !SPI1S_SPRF);Rc_Tel_num[i] = SPI1D;
			SS= ON;
		}
		EnableInterrupts; 

		TelRdFrameTime=100;
			
		//if((Rc_Tel_num[2]==0x55)&&(Rc_Tel_num[3]==0xaa))
		if(Rc_Tel_Verify())
		{
			Common_falg.bits.RD_Tel_Num_flag=0;
			RdTelNumErrorTimes=0;

		if(Sub_MODO_State == PSTN_Talking)
		  ;
		else if(Sub_MODO_State == PSTN_Talking_Video)
		  ;
		else
		{
			if(!memcmp(Tel_num,Rc_Tel_num+3,sizeof(Tel_num)))
				;
			else
			{			
				(void)memset(&Tel_num,0,sizeof(Tel_num));
		
				(void)memcpy(&Tel_num,Rc_Tel_num+3,Rc_Tel_num[2]);
			
				(void)memset(&Rc_Tel_num,0,sizeof(Rc_Tel_num));
				Tel_index=16;
				ShowTelNum();
			}

			for(i=0;i<16;i++)
			{
				if(Tel_num[i]==0)
				{
					break;		
				}
				else
					Tel_index=i+1;
			}
			Telephone_Number_Push();//save it as log

		}
			
			SpiSendBuf[0]=0xAA;SpiSendBuf[1]=0x55;SpiSendBuf[2]=0xAA;SpiSendBuf[3]=0x55;
			DisableInterrupts;
			for(i = 0; i<4; i++)
			{
				Nop;Nop;Nop;Nop;Nop;
				SS= OFF;
				while( !SPI1S_SPTEF);SPI1D =SpiSendBuf[i];
				while( !SPI1S_SPRF);Rc_Tel_num[0] = SPI1D;
				SS= ON;
			}
			EnableInterrupts; 
			
			TelRdFrameTime=1;
		}
		else
		{
			if(RdTelNumErrorTimes>10)
				TelRdFrameTime=250;	
			else
				RdTelNumErrorTimes++;
		}	
		
	}

}

uchar Rc_Tel_Verify(void)
{

	unsigned char temp;
	unsigned char i;
	temp=0;
	for(i=0;i<(Rc_Tel_num[2]+1);i++)
	{
		temp+=Rc_Tel_num[2+i];
	}
	if(Rc_Tel_num[2+i]==temp)
		return 1;
	else
		return 0;

}



