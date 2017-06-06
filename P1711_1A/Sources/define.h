
#include "string.h"
#define H_Modo_Ipervoice	0x01
#define	Hide_Button_Solution
//#define	High_Speed
#define PCB20150127
//--------------------------------------------------------------------------------
#define	Release
#ifdef	Release
	#define   FW_VERSION                              0x21//hardware modification
	#define   JD_VERSION                              0x14
	#define	Sub_Version				0x00
#else
	#define   FW_VERSION                              0x27//hardware modification
	#define   JD_VERSION                              0x14 
#endif
//#define   FW_VERSION                              0x01  // fixed 3780
/*
version from V1.0 to V1.1 fixed it is possible support any digital password(>0 ; < 6); fixed call switchboard from handset only for first time


*/

//--------------------------------------------------------------------------------

//#define   FRAME_KIND_MASK                             0x07  // mask to detect frame kind
//#define   OKI_SETTINGS                                0x07  // frame intended for OKI settings
//#define   OKI_SETUP_MASK                              0x80
//#define   AURINE                                      0x80
//#define   IPERVOICE                                   0x00





// #define PRG_7456      // for programmare on-board il MAX7456 with l'EVB (mette SPI in input)

// Prototipi of funzioni for tutti

void Audio  (unsigned char value);
void Video  (unsigned char value);
//void Led_PA_Mngr (unsigned char value);
//void Led_Mess_Mngr (unsigned char value);
//void Update_Signalling_LEDs(void);

void MCU_init(void); /* Device initialization function declaration */
void Init_var(void); /* Variable initialization */

void Init_time_base(void);
void Setup_tone(void);
void Courtesy_tone(void);
void Short_tone(void);
void Floor_call_tone(void);
void Tone_Management(void);
void States_Management(void);
void Timers_Management(void);
//void Audio_Management(void);

void Stop_tone(void);
void End_tone(void);

void Dissuasion_Tone(void);

//void Set_Wiper(unsigned char value, unsigned char pot);

void Set_Delay_for_Internal (void);
//void Potentiometer_Sequence(void);
//unsigned char CheckSum_The_Maxim_Memory(void);
//void Read_NVM_Char(void);


void Mic_State(unsigned char mic_state);




__interrupt void isrVsci1tx(void);
__interrupt void isrVsci1rx(void);
__interrupt void isrVsci1err(void);
__interrupt void isrVsci2tx(void);
__interrupt void isrVsci2rx(void);
__interrupt void isrVsci2err(void);


#define             ADD_GW_0                    0x80
#define             ADD_GW_1                    0x30
#define             ADD_GW_2                    0x30
#define             ADD_GW_3                    0x30

#define       MUTED             0x55
#define       ENABLED           0xAA


// for abilitare the visualizzazione delle coordinates of , il time of accensione and 
// le funzioni associate ai tasti
//#define DISPLAY_COORDINATES
#undef DISPLAY_COORDINATES







// define.h

#define PC                        0
#define DEBUG_PC                  0
#define MICRO                     1
#define DEBUG_DRV                 0
#define DEBUG                     0

#define OSC_EXT                   0
#define OSC_INT                   1
#define WHICH_OSC                 OSC_INT


#define KO      1
#define OK      0
#define ASSENTE 2
#define WARNING 3

#define YES     1
#define NO      0

#define ON      1
#define OFF     0

#define         BIT0                0x01
#define         BIT1                0x02
#define         BIT2                0x04
#define         BIT3                0x08
#define         BIT4                0x10
#define         BIT5                0x20
#define         BIT6                0x40
#define         BIT7                0x80

#define         BIT8                0x0100
#define         BIT9                0x0200
#define         BIT10           0x0400
#define         BIT11           0x0800
#define         BIT12           0x1000
#define         BIT13           0x2000
#define         BIT14           0x4000
#define         BIT15               0x8000


#define DENTIST_BUTTON_PRESSED             0x80 // For usare position del bit nelle trame
#define DENTIST_BUTTON_NOT_PRESSED         0x00

//#define POS_APRIPORTA                                  3 // number of of shift for analizzare il bit opendoor
//#define POS_DENTISTA                                  10 // number of of shift for analizzare il bit dentist

//#define DEBOUNCE_DENTISTA   4


#define ABORT   1 /*---segnala uscita programma tramite CTRL-BREAK */

#define B0      0x01
#define B1      0x02
#define B2      0x04
#define B3      0x08
#define B4      0x10
#define B5      0x20
#define B6      0x40
#define B7      0x80
#define B8      0x0100
#define B9      0x0200
#define B10     0x0400
#define B11     0x0800
#define B12     0x1000
#define B13     0x2000
#define B14     0x4000
#define B15     0x8000

#ifdef  PCB20150127
// definition pin
//Port A
//#define EN_VIDEO                  PTAD_PTAD0	// output new IO
#define	System_Video	    PTAD_PTAD0	//output
#define ADD1                     PTAD_PTAD1	// input
#define ADD2                     PTAD_PTAD2		// input for calling ID 
#define ADD3                     PTAD_PTAD3		// input
#define ADD4                     PTAD_PTAD4		// input
#define ADD5                     PTAD_PTAD5		// input
#define ADD6                     PTAD_PTAD6		// input
#define TC_PCM1_Shortcut_Detect	 PTAD_PTAD7		// input PCV PCM Video power control

//Port B
#define EN_Z                      PTBD_PTBD0	//output
#define REL2                     	PTBD_PTBD1  //output
#define HK							PTBD_PTBD2  //output
#define PCM1_Call_MODO_Button 			PTBD_PTBD3	// input PCM1 call the modo
#define	Gas_Detector_Button				PTBD_PTBD4	//input Gas detector
#define PANIC                    PTBD_PTBD5	//input

#define	REL6			PTBD_PTBD6 //output
#define Tel_DET			PTBD_PTBD7	//input for detect the telephone

//Port C
#define EX				PTCD_PTCD0		//input
#define Con485_R		PTCD_PTCD1  //output control as low
#define Con485_D		PTCD_PTCD2	//output control as transmit mode when is high
#define	RS485_TX		PTCD_PTCD3 // output
#define P_LCD_ON              PTCD_PTCD4      // input by button pressed breve --> comando of accensione monitor (se button lungo --> genera reset hw)
#define	RS485_RC		PTCD_PTCD5 // input
#define DTMF_SCL		PTCD_PTCD6	//output

//Port D
//#define HOOK                  PTDD_PTDD0    // 
#define	AudioPhone_HOOK				PTDD_PTDD0	// input hook of aduio phone
//#define RT			PTDD_PTDD1    // 	
#define TR_2		PTDD_PTDD1		//output  control the ring to audio phone
#define	IN4				PTDD_PTDD2	//output
#define	Audio_Call_Switchboard_Button	PTDD_PTDD3	//input AUDIO PHONE CALL TO SWITCH BOARD
//#define TC_PCM1_Shortcut_Detect		PTDD_PTDD4        //input PCV PCM Video power control
#define PTTIE             PTDD_PTDD4 //output
//#define PL_165						PTDD_PTDD5 	//output; chip select of 74HC165
#define PTTEI              PTDD_PTDD5	//output
//#define CLK_165						PTDD_PTDD6 	//output; clock of 74HC165
#define TC_PCM1				PTDD_PTDD6         //output PCV PCM Video power control
//#define DATA_165					PTDD_PTDD7 	//input; data of 74HC165
#define RPCH               PTDD_PTDD7	//ouput

//Port E
#define Disp_TX		PTED_PTED0	//output
#define Disp_RC		PTED_PTED1	// input
//#define RPCH               PTED_PTED2	//ouput
#define SS               PTED_PTED2	//ouput
#define RX_DER           PTED_PTED3	//inut
#define TX_DER           PTED_PTED4 //output
#define MISO			PTED_PTED5 //// input
//#define TC_PCM1		PTED_PTED5         //output PCV PCM Video power control
//#define PTTEI              PTED_PTED6	//output
#define MOSI              PTED_PTED6	//output
//#define PTTIE             PTED_PTED7 //output
#define SCL             PTED_PTED7  //output

//Port F
//#define HK_Check              PTFD_PTFD0  //input
#define CCTV_EN                PTFD_PTFD0	//output
#define EN_RELE_VIDEO	PTFD_PTFD1 //output  ?? ENA_VID
//#define CCTV_EN                PTFD_PTFD2	//output
#define HK_Check              PTFD_PTFD2  //input
//#define PCS                		PTFD_PTFD3	//
#define Audio_Switch        PTFD_PTFD3//Flag.bits.Audio_Connect_Flag  //PTGD_PTGD3	//1 connect to PCM, 0 connect to SYSTEM
#define SUON                	PTFD_PTFD4	//output
//#define Dial_EN                	PTFD_PTFD5	//output 
#define	DTMF_CE		PTFD_PTFD5	
#define LED_LOGO              PTFD_PTFD6    //out  Led of logo under monitor URMET
#define DTMF_SDA		PTFD_PTFD7	//output


//Port G
#define EN_AUDIO						PTGD_PTGD0 //output  ?Audio
#define U_D 								PTGD_PTGD1 //output
#define C_S 								PTGD_PTGD2//output
#define Audio_Phone_Lock_Button 		PTGD_PTGD3 //input DP_L?  for handset LOCK button
#define PCM1_Lock						PTGD_PTGD4//output
#define Pres_BT             					PTGD_PTGD5//input
#define RST_BT             					PTGD_PTGD6//output

#else
// definition pin
//Port A
//#define EN_VIDEO                  PTAD_PTAD0	// output new IO
#define	System_Video	    PTAD_PTAD0	//output
//#define CS_RX                     PTAD_PTAD1	// this pin unused
#define T_D0                     PTAD_PTAD2		// input for calling ID 
#define T_D1                     PTAD_PTAD3		// input
#define T_D2                     PTAD_PTAD4		// input
#define T_D3                     PTAD_PTAD5		// input
#define T_DV                     PTAD_PTAD6		// input
#define FSK                     	PTAD_PTAD7		// input

//Port B
#define EN_Z                      PTBD_PTBD0	//output
#define REL2                     	PTBD_PTBD1  //output
#define HK				PTBD_PTBD2  //output
#define 	PCM1_Call_MODO_Button 			PTBD_PTBD3	// input PCM1 call the modo
#define	Gas_Detector_Button				PTBD_PTBD4	//input Gas detector
#define PANIC                    PTBD_PTBD5	//input

#define	REL6			PTBD_PTBD6 //output
#define Tel_DET			PTBD_PTBD7	//input for detect the telephone

//Port C
//PTCD_PTCD0		//unused output
#define Con485_R		PTCD_PTCD1  //output control as low
#define Con485_D		PTCD_PTCD2	//output control as transmit mode when is high
#define	RS485_TX		PTCD_PTCD3 // output
#define P_LCD_ON              PTCD_PTCD4      // input by button pressed breve --> comando of accensione monitor (se button lungo --> genera reset hw)
#define	RS485_RC		PTCD_PTCD5 // input
#define DTMF_SCL		PTCD_PTCD6	//output

//Port D


//#define HOOK                  PTDD_PTDD0    // 
#define	AudioPhone_HOOK				PTDD_PTDD0	// input hook of aduio phone
//#define RT			PTDD_PTDD1    // 	
#define TR_2		PTDD_PTDD1		//output  control the ring to audio phone
#define	IN4				PTDD_PTDD2	//output
#define	Audio_Call_Switchboard_Button	PTDD_PTDD3	//input AUDIO PHONE CALL TO SWITCH BOARD
#define TC_PCM1_Shortcut_Detect		PTDD_PTDD4        //input PCV PCM Video power control
#define CLK_165						PTDD_PTDD6 	//output; clock of 74HC165
#define PL_165						PTDD_PTDD5 	//output; chip select of 74HC165
#define DATA_165					PTDD_PTDD7 	//input; data of 74HC165

//Port E
#define Disp_TX		PTED_PTED0	//output
#define Disp_RC		PTED_PTED1	// input
#define RPCH               PTED_PTED2	//ouput
#define RX_DER           PTED_PTED3	//inut
#define TX_DER           PTED_PTED4 //output
//#define PCV			PTED_PTED5 //
#define TC_PCM1		PTED_PTED5         //output PCV PCM Video power control
#define PTTEI              PTED_PTED6	//output
#define PTTIE              PTED_PTED7 //output

//Port F
#define HK_Check              PTFD_PTFD0  //input
#define EN_RELE_VIDEO	PTFD_PTFD1 //output  ?? ENA_VID
#define CCTV_EN                PTFD_PTFD2	//output
//#define PCS                		PTFD_PTFD3	//
#define Audio_Switch        PTFD_PTFD3//Flag.bits.Audio_Connect_Flag  //PTGD_PTGD3	//1 connect to PCM, 0 connect to SYSTEM
#define SUON                	PTFD_PTFD4	//output
//#define Dial_EN                	PTFD_PTFD5	//output 
#define	DTMF_CE		PTFD_PTFD5	
#define LED_LOGO              PTFD_PTFD6    //out  Led of logo under monitor URMET
#define DTMF_SDA		PTFD_PTFD7	//output


//Port G
#define EN_AUDIO						PTGD_PTGD0 //output  ?Audio
#define U_D 								PTGD_PTGD1 //output
#define C_S 								PTGD_PTGD2//output
#define Audio_Phone_Lock_Button 		PTGD_PTGD3 //input DP_L?  for handset LOCK button
#define PCM1_Lock						PTGD_PTGD4//output
#define Pres_BT             					PTGD_PTGD5//input
#define RST_BT             					PTGD_PTGD6//output


#endif

#define	F_Old_Pres_BT	SystemStatus.bits.Old_Pres_BT

//unknown PIN
//#define MIC_MUTE                  PTAD_PTAD0    //wait UD confirm

//#define SPK_MUTE                  PTGD_PTGD6        //wait UD confirm
//#define MIC_Mute_IO		PTGD_PTGD7

//#define MIC_MUTE                  New_Flag3.bits.MicMute		//wait UD confirm
#define PCM_Audio_Switch           Flag4.bits.Audio_Connect_Flag1 //PTCD_PTCD6	// 1 connect to audiophone, 0 connect to modo //auk2
//#define MUTE_OK         Flag.bits.MuteOK
//#define SPK_MUTE                  Flag3.bits.SpeakMute	//wait UD confirm

#define T1                        B0
#define T2                        B1
#define T3                        B2

#define TALK_ON                   1
#define TALK_OFF                  0


#define	To_HandSet	1
#define	To_Modo	0

#define	Pcm_To_HandSet	1
#define	Pcm_To_Modo	0

#define	V_To_System		0
#define	V_To_PCM		1
// time
#ifdef High_Speed
 	#define CPUCLK                    (18432000L * 2)
#else
	  #define CPUCLK                  (4443428L * 2)
#endif

#define BUSCLK                    (float)CPUCLK / 2L
#define COUNTER_2MS                 (unsigned int)((float)BUSCLK * 2L / 1000L)
#define COUNTER_500US               (unsigned int)((float)BUSCLK * 500L / 1000000L)    // not for 500 because overflow

#define BAUDRATE                  9600
#define BAUDRATE1                38400
#define TIME_BASE                2                        // 2 ms general time base 
#define TIME_1_SEC               100                      // time base 10 ms
#define TIME_100_MSEC            10                       // time base 10 ms
#define TIME_10_MSEC             5                        // time base 2 ms
#define TIME_1_Min               60                      // time base 1s
//#define TIME_SCAN_BUTTONS           20
#define TIME_SCAN_BUTTONS           2
//#define TIME_SCAN_BUTTONS           4
//#define SCAN_TIME_BASE           TIME_SCAN_BUTTONS          // time base button scan
#define SCAN_TIME_BASE           20          // time base button scan

#define MAX_RING                  5
#define MAX_TONES                 8
#define MAX_CALLS                 9

// #define FREQ_0                    600L
// #define TIME_F0_ON               50L                       // [ms]
// #define TIME_F0_OFF              00l                       // [ms]
//#define PERIODO_F0                (unsigned int)((float)BUSCLK / FREQ_0)
//#define NUMPER_F0_ON              (unsigned int)((TIME_F0_ON * FREQ_0) / 1000L)
//#define NUMPER_F0_OFF             (unsigned int)((TIME_F0_OFF * FREQ_0) / 1000L)

// #define FREQ_1                    1302L
// #define TIME_F1_ON               50L                       // [ms]
// #define TIME_F1_OFF              00L                       // [ms]
// #define PERIODO_F1                (unsigned int)((float)BUSCLK / FREQ_1
// #define NUMPER_F1_ON              (unsigned int)((TIME_F1_ON * FREQ_1) / 1000L)
// #define NUMPER_F1_OFF             (unsigned int)((TIME_F1_OFF * FREQ_1) / 1000L)

#define INIT_PROC B5
#define RUN_PROC  B6
#define AWAKE     B7

#define Signal(target,mask)     { DisableInterrupts; target |= mask; EnableInterrupts; }
#define Free(target,mask)       { DisableInterrupts; target &= ~mask; EnableInterrupts; }
#define GoTimer(timer,value)    { DisableInterrupts; timer = (value/TIME_BASE); EnableInterrupts; }
#define StopTimer(timer)        { DisableInterrupts; timer = 0; EnableInterrupts; }
#define SignalInt(target,mask)  { target |= mask; }
#define FreeInt(target,mask)       { target &= ~mask; }
#define GoTimerInt(timer,value)    { timer = (value/TIME_BASE); }
#define StopTimerInt(timer)        { timer = 0; }
#define SetBit(target,mask)     { target |= mask; }
#define ClrBit(target,mask)     { target &= ~mask; }

#define _Stop asm ( stop; )
  /*!< Macro to enter stop modes, STOPE bit in SOPT1 register must be set prior to executing this macro */

#define _Wait asm ( wait; )
  /*!< Macro to enter wait mode */


#define RESET_WATCHDOG           ;

// "tone_proc"
#define RING_REQUEST              B0
#define BI_TONE_END               B1
#define NOTE_PAUSE_END            B2
#define RING_NOW                  B7

//#define MAX_SUONERIE              5

// opcode trame bus of column
#define OPCC_DIAG                 1             // !!!! by rivedere


// values "state_puls"
/*
#define ATTESA_TASTO              0
#define DEBOUNCE_BUTTON_ON         1
#define TASTO_VALIDO              2
#define WAIT_RILASCIO             3
#define DEBOUNCE_BUTTON_OFF        4
*/

#define RELEASED                  0                           
#define PRE_PRESSED               1
#define PRESSED                   2
#define PRE_RELEASED              3

// #define ATTENDO_RITORNO_A_UNO     4      // Solo for detecting burst panic
// #define ATTENDO_RITORNO_A_ZERO    5      // Solo for detecting burst panic

//#define DEBOUNCE_BUTTON_FROM_UP_TO_DOWN    (unsigned int)(40 / SCAN_TIME_BASE)
//#define DEBOUNCE_BUTTON_FROM_DOWN_TO_UP    (unsigned int)(40 / SCAN_TIME_BASE)
#define DEBOUNCE_BUTTON_FROM_UP_TO_DOWN    (unsigned int)(60 / SCAN_TIME_BASE)
#define DEBOUNCE_BUTTON_FROM_DOWN_TO_UP    (unsigned int)(60 / SCAN_TIME_BASE)
//#define DEBOUNCE_BUTTON_FROM_UP_TO_DOWN    (unsigned int)(100 / SCAN_TIME_BASE)
//#define DEBOUNCE_BUTTON_FROM_DOWN_TO_UP    (unsigned int)(100 / SCAN_TIME_BASE)

#define DEBOUNCE_PANIC_FROM_UP_TO_DOWN      25 // 500ms  a base 20ms
#define DEBOUNCE_PANIC_FROM_DOWN_TO_UP       5 // 100ms  a base 20ms

//#define DURATA_MINIMA_IMPULSO_BASSO       4 // 80ms   a base 20ms 
//#define DURATA_MASSIMA_IMPULSO_BASSO      6 // 120ms  a base 20ms 

//#define DEBOUNCE_CONTEGGIO_IMPULSI        10 // value fittizio

#define PANIC_BATT_OK                BIT11   //
#define PANIC_BATT_LOW               BIT12   //
#define ONOFF_BUTTON                      BIT13   //
#define ONOFF_BUTTON_5S                   BIT14   //

//#define MAX_IMPULSI_PANIC                7 // number of of impilsi necessari for riconoscere the sequence of batteria scarica


//.....................
#define RELEASED                   0
#define PIN_LOW_PHASE              1
#define PIN_HIGH_PHASE             2

#define TOO_LONG                  90

#define MIN_SEQUENCE_TIME         72
#define MAX_SEQUENCE_TIME         78

#define BUTTON_DEBOUNCE                        3
#define RELEASE_WAIT                           4       
#define WAIT_RELEAS_FROM_LONG_PRESS            5       

#define PANIC_PRESS_TIME                      25

#define ONOFF_PRESS_TIME                         5      // --> 100ms
#define ONOFF_LONG_PRESS_TIME                  150      // --> 3s


#define MAX_VAL_CONEXANT_ATTENUATION         0xE8       // Sperimental value suggested by Villata 

//.....................


// "proc_scan"

#define BUTTON_READY              B0
#define BUTTON_READY_2S           B1
#define HOOK_VARIATION            B2




//#define AUTOMATIC_ONHOOK_TIMEOUT    5 // debug
#define AUTOMATIC_ONHOOK_TIMEOUT    60


//#define TIME_INSISTENZA          (unsigned int)(1000 / SCAN_TIME_BASE)
//#define DURATA_IMPULSO_MUTE       (160 / SCAN_TIME_BASE)

// "hook_state"
#define ON_HOOK                  0
#define PRE_OFF_HOOK             1
#define OFF_HOOK                 2
#define PRE_ON_HOOK              3


// #define DEBOUNCE_HOOK_FROM_UP_TO_DOWN     5  // debounce riaggancio
// #define DEBOUNCE_HOOK_DA_GIU_A_SU     5  // debounce offhook

// values for "call_from"
#define CALL_FROM_MAIN             0
#define CALL_FROM_SECONDARY        1
#define CALL_FROM_SWITCHBOARD      2
#define CALL_FROM_INTERCOM         3
#define CALL_FROM_FLOOR            4
#define CALL_FROM_COURTESY         5
#define CALL_FROM_DISSUASION       6
#define CALL_SHORT                 7
#define CALL_Long                 8

#define VIDEO_ON                            1
#define VIDEO_OFF                         0

#define FONIA_ON                            1
#define FONIA_OFF                           0

#define OKI_ON                            1
#define OKI_OFF                           0

#define FONIA_ON1                           1
#define FONIA_ON2                           2
#define FONIA_ALL_OFF                     3

#define ALARM_ON                  1
#define ALARM_OFF                 0

#define PA_LED_ON                 1        // percentual (base 10) of phase ON
//#define SVC_LED_ON               5        // percentual (base 10) of phase ON
//#define DENT_LED_ON              10        // percentual (base 10) of phase ON
#define STEADY_ON                10        // percentual (base 10) of phase ON
#define SWITCH_OFF                  0

#define OPEN_DOOR_LED_ABSENCE            1       // percentual (base 10) of phase ON
#define OPEN_DOOR_LED_MAIN              10       // percentual (base 10) of phase ON
#define OPEN_DOOR_LED_SECONDARY         10       // percentual (base 10) of phase ON 

#define LED_MESSAGES_PRESENT             1        // percentual (base 10) of phase ON



typedef enum PI_stati                           // state
{
    IDLE = 0,
    AUDIO_ONLY_OFF_HOOK_WAIT,
    VIDEO_OFF_HOOK_WAIT,
    SELFINSERTION_OFF_HOOK_WAIT,
    ANSWERINGMACHINE_OFF_HOOK_WAIT,       // temporary state to allow a correct frame composition of offhook SVC
    INTERCOM_OFF_HOOK_WAIT,
    IN_COMMUNICATION_AUDIO_ONLY,
    IN_COMMUNICATION_VIDEO,
    IN_COMMUNICATION_SELFINSERTION,
    IN_COMMUNICATION_ANSWERINGMACHINE,
} PI_stati_tipo;

typedef enum PI_stati1   						// state
{
	EX_IDLE = 0,
	PCM1CALLING,
	IN_COMMUNICATION_PCM1, //CONVERSATION WITH PCM1
	Mornitor_PCM1,
	Mornitor_CCTV,
	PSTN_Talking,
	PSTN_Calling,
	PSTN_Talking_Video,
	PSTN_Calling_Video,
	PSTN_Dial_Number,
	Alarm_Handle_Waiting,
} PI_stati_tipo1;

typedef enum AU_stati                           // state
{
    AUDIO_IDLE = 0,
  POW_0,
  POW_1,
  POW_2,
  POW_3,
  POW_4,
  POW_5,
  POW_6,
  OFF_HOOK_0,
  OFF_HOOK_1,
  ON_HOOK_0,
  ON_HOOK_1,
  MIC_IN_MUTE_0,
  MIC_IN_MUTE_1,
  MIC_OUT_OF_MUTE_0,
  MIC_OUT_OF_MUTE_1
} AU_stati_tipo;

//values for User_TS_State



#define      PRODUCTION_TEST_ACTIVATION       0
#define      TS_CALIBRATION                   1
#define      MIC_DISABLE                      5
#define      CKS_MAXIM_OK                     6


#define     TIME_OF_RING        30
//#define       DELAY_          15              // time base 100ms
#define     DELAY_1         5              // time base 100ms
#define     DELAY_2         DELAY_1 + TIME_OF_RING
#define     DELAY_3         DELAY_2 + TIME_OF_RING
#define     DELAY_4         DELAY_3 + TIME_OF_RING

#define     MASTER_APARTMENT_STATION            0


#define   OSD_OFFSET_X_INTERNAL_VIDEO     16
#define   OSD_OFFSET_Y_INTERNAL_VIDEO     8
#define   OSD_OFFSET_X_EXTERNAL_VIDEO     12
#define   OSD_OFFSET_Y_EXTERNAL_VIDEO     8


#define   NUM_BYTES_DA_COPIARE   5   // Numero of bytes necessari for creare a imagine dei dati in flash
typedef struct
  {    
  unsigned int write_flash_after_tx          :1;
//  unsigned int r_presence_absence         :1;    
  unsigned int r_messages_present            :1;    
  unsigned int r_door_state                  :1;    
  unsigned int bipbip_audioonly_call         :1;    
  unsigned int switched_on_off               :1;    
  unsigned int Send_On_Uart1                 :1;    
  unsigned int Room_Monitor_In_Progress      :1;    
  }BitV;

//#define R_presence_absence                                BitVars.r_presence_absence
#define R_messages_presents               BitVars.r_messages_present
#define Door_State                        BitVars.r_door_state              
#define BIPBIP_audioonly_call             BitVars.bipbip_audioonly_call
#define SwitchedOnOff                     BitVars.switched_on_off
#define Send_On_Uart_1                    BitVars.Send_On_Uart1
#define PRG_FLASH_AFTER_TX                BitVars.write_flash_after_tx
#define ROOM_MONITOR_IN_PROGRESS          BitVars.Room_Monitor_In_Progress

typedef struct
  {    
  unsigned int state_led_mute           :1;
  unsigned int Mic_Muted                :1;    
  unsigned int main_gui_locked          :1;    
  unsigned int call_gui_locked          :1;    
  unsigned int daudio_in_progress       :1;    
  unsigned int free6           :1;    
  unsigned int free7           :1;    
  unsigned int free8           :1;    
  }BitV1;

#define R_Led_mute                      BitVars1.state_led_mute
#define MIC_Muted                           BitVars1.Mic_Muted
#define Main_GUI_locked             BitVars1.main_gui_locked
#define Call_GUI_locked             BitVars1.call_gui_locked
//#define DAaudio_In_Progress     BitVars1.daudio_in_progress
#define Free_6                              BitVars1.free6
#define Free_7                              BitVars1.free7
#define Free_8                              BitVars1.free8

 /*                                                            
#define     TIME_IMP_AUDIO1         3  // time base 100ms      pulse EN_AUDIO
#define     TIME_IMP_AUDIO2         3  // time base 100ms      delay by EN_AUDIO a MIC_MUTE
#define     TIME_IMP_AUDIO3         1  // time base 100ms      pulse MIC_MUTE
#define     TIME_IMP_AUDIO4         2  // time base 100ms      delay by pulse MIC_MUTE a pulse SPK_MUTE
#define     TIME_IMP_AUDIO5         1  // time base 100ms      pulse SPK_MUTE
#define     TIME_IMP_AUDIO6         2  // time base 100ms      delay by pulse SPK_MUTE idle
#define     TIME_IMP_AUDIO7         3  // time base 100ms      delay by activation EN_AUDIO a pulse MIC_MUTE after offhook
#define     TIME_IMP_AUDIO8         2  // time base 100ms      pulse MIC_MUTE on close
*/
// Definition of the function linked to menu buttons
/*
typedef enum funz_tasti
  {
  F_NULL = 0,   // no function
  F_CYC,        // function cameras scan
  F_LIGHTS,     // function activation special
  F_LIGHTS_COM, // function activation while is in comm
  F_OD_2,       // function open door 2
  F_AUDIO,      // function activation audio
  F_MUTE_MIC,   // function mute microphone
  F_MUTE_SPKR,  // function mute microphone
  F_CONFIG,     // function settings
  F_CONFIG_IN_COM, // function settings while communication
  F_OD_1,       // function open door 1
  F_SI_CYC,     // function self_insertion
  F_MSG,        // function answering machine messagges
  F_CALL,       // function call
  F_CENTR,      // function call to switchboard
  F_OD_AUTO,    // function automatic opendoor 
  F_ABSENCE,    // function absence
  F_OFF,        // function off monitor
  F_VOLUME,     // function volume
  F_VOL_01,     // function level volume
  F_VOL_02,     // function level volume
  F_VOL_03,     // function level volume
  F_VOL_04,     // function level volume
  F_VOL_05,     // function level volume
  F_VOL_06,     // function level volume 
  F_VOL_07,     // function level volume 
  F_VOL_08,     // function level volume 
  F_VOL_09,     // function level volume 
  F_LUM_01,     // function level brightness
  F_LUM_02,     // function level brightness 
  F_LUM_03,     // function level brightness
  F_LUM_04,     // function level brightness 
  F_LUM_05,     // function level brightness 
  F_LUM_06,     // function level brightness 
  F_LUM_07,     // function level brightness 
  F_LUM_08,     // function level brightness 
  F_LUM_09,     // function level brightness 
  F_COL_01,     // function level color 
  F_COL_02,     // function level color
  F_COL_03,     // function level color
  F_COL_04,     // function level color
  F_COL_05,     // function level color
  F_COL_06,     // function level color
  F_COL_07,     // function level color
  F_COL_08,     // function level color
  F_COL_09,     // function level color
  F_EXT_CALL,   // function external call
  F_E_C_01,     // function ringtone selection for external call
  F_E_C_02,     // function ringtone selection for external call
  F_E_C_03,     // function ringtone selection for external call
  F_E_C_04,     // function ringtone selection for external call
  F_E_C_05,     // function ringtone selection for external call
  F_FLOOR_CALL, // function call to floor
  F_F_C_01,     // function ringtone selection for call to floor
  F_F_C_02,     // function ringtone selection for call to floor
  F_F_C_03,     // function ringtone selection for call to floor
  F_F_C_04,     // function ringtone selection for call to floor
  F_F_C_05,     // function ringtone selection for call to floor
  F_RINGTONE_VOLUME,   // function ringtone volume
  F_R_V_01,     // function level ringtone volume
  F_R_V_02,     // function level ringtone volume
  F_R_V_03,     // function level ringtone volume
  F_R_V_04,     // function level ringtone volume
  F_R_V_05,     // function level ringtone volume
  F_R_V_06,     // function level ringtone volume
  F_R_V_07,     // function level ringtone volume
  F_R_V_08,     // function level ringtone volume
  F_R_V_09,     // function level ringtone volume
  F_R_V_10,     // function level ringtone volume
  F_R_V_11,     // function level ringtone volume
  F_R_V_12,     // function level ringtone volume
  F_R_V_13,     // function level ringtone volume
  F_R_V_14,     // function level ringtone volume
  F_EXIT,       // function back to main screen
  F_EXIT_REG,   // function back to previous screen
  F_EXIT_SVC,   // function back to previous screen
  F_DELETE,     // function delete message
  F_NEXT,       // function next message
  F_CALL_INT,   // function call in apartment
  F_CALL_EXT,   // function call to another apartment
  F_CALL_SPEC,  // function call another column
  F_ATT_SPEC,   // function activation special services
  F_NUM_1,      // function to enter number ...
  F_NUM_2,      // function to enter number ...
  F_NUM_3,      // function to enter number ...
  F_NUM_4,      // function to enter number ...
  F_NUM_5,      // function to enter number ...
  F_NUM_6,      // function to enter number ...
  F_NUM_7,      // function to enter number ...
  F_NUM_8,      // function to enter number ...
  F_NUM_9,      // function to enter number ...
  F_NUM_0,      // function to enter number ...
  F_NUM_X,      // function to delete
  F_NUM_K,      // function to send a call
  F_VIDEO_TRANSFER,       // for video transfer function
  F_NTSC,       // Video mode selection
  F_PAL         // Video mode selection
  } funz_menu;
*/
//=======================================================

//#define       NEW_HW            // HW of Testate il 30/10/09
#undef        NEW_HW            // HW before to 30/10/09

#ifdef NEW_HW
#define       MIC_MUTE_STBY       0       
#define       MIC_MUTE_ACTIVE     1       
#define       SPK_MUTE_STBY       0
#define       SPK_MUTE_ACTIVE     1
#else
#define       MIC_MUTE_STBY       1       
#define       MIC_MUTE_ACTIVE     0       
#define       SPK_MUTE_STBY       1
#define       SPK_MUTE_ACTIVE     0
#endif

// ---- Management parametri speciali for icone -----------
#define       TRASP       0
//#define       NO_TRASP    64        // to be tested
#define       NO_TRASP    0x40        // to be tested
//??#define       NO_TRASP    0x80        // by data sheet  

//#define       BLINK       32        // to be tested
#define       BLINK       0x20        // to be tested
//??#define       BLINK       0x40        // by data sheet  
#define       NO_BLINK    0 

//#define       INV         16        // to be tested
#define       INV         0x10        // to be tested
//??#define       INV         0x20        // by data sheet  
#define       NO_INV      0

#define       NO_EFFECTS        TRASP + NO_BLINK + INV
#define       REVERSE_ICONS     TRASP + BLINK + NO_INV     // reverse effect  
//#define       REVERSE_ICONS     TRASP + BLINK + INV   // not reverse....     


#define       BLINK_NORMAL         0x40  // Values found with tries with Pont
#define       BLINK_REVERSE        0x60  // Values found with tries with Pont

#define       TIME_LOCK_GUI            10   // Time for gui (buttons) locking 


//----------- From data sheet ----------------------------------------
// 8-Bit Operation Mode (DMM[6] = 1)
// If DMAH[1] = 0, the content is to be interpreted as a Character Address byte, where
// Bits 70 = Character Address bits, CA[7:0] (Figure 12).
// If DMAH[1] = 1, the content is to be interpreted as a Character Attribute byte where
// Bit 7 = Local Background Control bit, LBC (Figure 24 and Table 4)
// Bit 6 = Blink bit, BLK
// Bit 5 = Invert bit, INV (see Figure 24)
// Bit 40 = 0
// (The LBC, BLK, and INV bits are described in the Display Memory Mode register.)





//-------------------------------------------------------
#define       ABSENCE_ON                  0x40    
#define       ABSENCE_OFF                 0x00    

#define       RELE_VIDEO_ON               1
#define       RELE_VIDEO_OFF              0

#define       RPCH_ON                     0
#define       RPCH_OFF                    1

//#define       MAX_CHAR_FOR_CALLED_UT      3    // max number of characters (4) for enter the user to call
//#define       CANCELLA_TUTTO              0xF0 // Carattere non stampabile for passare il comando of cancellazione
//#define       CONFERMA                    0xF1 // Carattere non stampabile for passare il comando of conferma immissione
//#define       ROW_INPUT_FOR_CALL          1
//#define       COLUMN_INPUT_FOR_CALL       10

#define       CALL_TO_INTERNAL          1
#define       CALL_TO_EXTERNAL          2
#define       CALL_TO_SWITCHBOARD       3
#define       CALL_TO_SPECIAL_SERVICES   4

//#define       NO_MORE_DIGIT              10   // trick used to lock the digit entering (not for all the GUI)
                                                                                                 
#define       WAIT_OF_CALL_TO_SWITCHBOARD_IS_STORED         10  // Time to wait for a call to switchboard

//#define       TIME_FOR_CHAR_SELECTION     80     // 800ms  (base 10 ms) for number/letter selection for the call 
//#define       A_SECOND_AND_HALF          15     //        (base 10 ms) 
//#define       VIDEO_NTSC                  0x80   // Bit set if the current video standard is NTSC

                                                 
//#define       OFFSET_ROW_NTSC            2

//#define       ROW_PAL_BUTTON_HOOK       12
//#define       ROW_NTSC_BUTTON_HOOK      12-OFFSET_ROW_NTSC

//#define       TIMEOUT_ICONS_VISIBLE_DURING_COMM           5

//#define       VALUE_TIMER_PROG_OKI        3      //DB attesa di 300ms 

//-------------------------
#ifdef DISPLAY_COORDINATES
#define       TIME_AUTOPOWEROFF        200     // in seconds
#else
#define       TIME_AUTOPOWEROFF        120     // in seconds
#endif 


#define       AUDIO_ONLY_CALL               1      // used to select tone type
#define       FLOOR_CALL                    2      // used to select tone type
#define       PSTN_CALL                    3      // used to select tone type

#define       LED_ON                      1
#define       LED_OFF                     0

#define       LOGO_BLINK                  5      // frequency blink led logo. base 100ms
//--------------------------

#define	RS485_TX_Ready	1
#define	RS485_TX_Idle		0

//--------------------------------------------------------------------


/*
//#define HCS08_FLASH_START_ADDR  0xEA00 
#define HCS08_FLASH_START_ADDR  0x8000  //COMMENT DUE TO aw32 debug
#define EE_Page1    HCS08_FLASH_START_ADDR
#define EE_Page2    EE_Page1+0x200
#define EE_Page3    EE_Page2+0x200
#define EE_Page4    EE_Page3+0x200 
#define EE_Page5    EE_Page4+0x200
#define EE_Page6    EE_Page5+0x200
#define EE_Page7    EE_Page6+0x200
#define EE_Page8    EE_Page7+0x200
#define EE_Page9    EE_Page8+0x200
//#define EE_Page10    EE_Page9+0x200
#define EE_PageLED    EE_Page9+0x200
#define EE_PageTouch  EE_PageLED+0x200
*/
#define PageOfRecord        10

//page address
#define HCS08_FLASH_START_ADDR  0xD000 
#define EE_Page1    HCS08_FLASH_START_ADDR
#define EE_Page2    EE_Page1+0x200 //PHONE BOOK PAGE1
#define EE_Page3    EE_Page2+0x200
#define EE_Page4    EE_Page3+0x200 
//#define EE_Page5    EE_Page4+0x200 //PHONE BOOK PAGE4
//#define EE_PageLED  EE_Page5+0x200
#define EE_PageCopy1  EE_Page4+0x200
//#define EE_PageCopy2  EE_PageCopy1+0x200
#define EE_HomeAutomation1_1    EE_PageCopy1 + 0x200

#define EE_HomeAutomation2_1    EE_HomeAutomation1_1+0x200//Room 2 A

#define EE_HomeAutomation3_1    EE_HomeAutomation2_1+0x200//Room 3 A

#define EE_HomeAutomation4_1    EE_HomeAutomation3_1+0x200//Room 4 A

#define EE_HomeAutomation5_1    EE_HomeAutomation4_1+0x200//Room 5 A

#define EE_HomeAutomation6_1    EE_HomeAutomation5_1+0x200//Room 6 A

#define EE_HomeAutomation7_1    EE_HomeAutomation6_1+0x200//Room 7 A

#define EE_HomeAutomation8_1    EE_HomeAutomation7_1+0x200//Room 8 A

#define EE_HomeAutomation9_1    EE_HomeAutomation8_1+0x200//Room 9 A

#define EE_HomeAutomation10_1    EE_HomeAutomation9_1+0x200//Room 10 A

#define EE_HomeAutomation11_1    EE_HomeAutomation10_1+0x200//Room 11 A

#define EE_HomeAutomation12_1    EE_HomeAutomation11_1+0x200//Room 12 A

#define EE_Input_Memory1    EE_HomeAutomation12_1+0x200


#define EE_Page_Tel_Momory    EE_Input_Memory1+0x200
//#define EE_Page_Tel_Momory1    EE_Page_Tel_Momory+0x200
#define EE_Page_AlarmSystem  EE_Page_Tel_Momory+0x200
#define EE_Scenario_CCTV	EE_Page_AlarmSystem+0x200
//#define EE_Scenario_CCTV1	EE_Scenario_CCTV+0x200
#define EE_CCTV_Room	EE_Scenario_CCTV+0x200
#define EE_Time_Event	EE_CCTV_Room + 0x200 // for save the timing event


//target page number
#define TargetPage1 1
#define TargetPage2 2
#define TargetPage3 3
#define TargetPage4 4
//#define TargetPage5 5


//#define TargetPage18      18
//#define TargetPageLed   TargetPage5+1
#define TargetPageCopy1 TargetPage4+1
//#define TargetPageCopy2 TargetPageCopy1+1
//#define TargetPageBook  TargetPageCopy2+1
//#define TargetPageTouch TargetPageBook+1

#define TargerNumPosition   241//130 //should be change to 24
#define TargerFlagPosition  242//131
//----------------------------------------------------------------------------------

//#define TX_ENABLE   PTFD_PTFD1        //·¢ËÍÊ¹ÄÜ
//#define Reset_Key   PTCD_PTCD4


//------------------------------------------------------------------------------------
//#define   RXBUF_LEN                   16
//#define   TXBUF_LEN                   16


//#define coefficient 20
//#define coefficient1 20
//------------------------------------------------------------------------------------
//key value
// main menu
/*
typedef enum Key_Type               
{
    No_key=0,
    K_MenuButton,   
    K_Exit,
    K_OK,
    K_All_Exit,
    K_LOK,
    K_I_Call,
    K_E_Call,
    K_Call_SW,
    K_Secial_Services,
    K_Name_Box,//10
    K_Code_Box, 
    K_AutoSwitch,           
    K_AnsweringService, 
    K_PhoneBook,
    K_MuteButton,
    K_CallSwitchBoard,
    K_LightTurnOn,
    K_Absence_Presence,
    K_Auto_Door_Open,
    K_Turn_Off,
    K_Door_Open_1,  
    K_Door_Open_2,//20
    K_Setting,  
    K_Phone_Book,
    K_UP,
    K_Down,
    K_Audio_Setting,
    K_Video_Setting,
    K_PhoneBook_Edit,
    K_CallVolume_Inc,
    K_CallVolume_Dec,
    K_AudioVolume_Inc,// 30
    K_AudioVolume_Dec,
    K_Beep_ON,
    K_Beep_OFF,
    K_Bright_Inc,
    K_Bright_Dec,
    K_Contrast_Inc,
    K_Contrast_Dec,
    K_Color_Inc,
    K_Color_Dec,
    K_PhoneBook_Add,// 40
    K_PhoneBook_Delete,
    K_PhoneBook_Modify,
    K_CallRinger1,  
    K_CallRinger2,
    K_CallRinger3,
    K_CallRinger4,
    K_CallRinger5,
    K_FloorCallRinger1,
    K_FloorCallRinger2,
    K_FloorCallRinger3,//50
    K_FloorCallRinger4,
    K_FloorCallRinger5,
    K_NotHide,
    K_Hide,
    K_MicroPhone_Mute,
    K_AnswerCall,
    //K_43To169_rationswitch,
    
    K_Reset,
} Key_V_Type;
*/
typedef enum Key_Type				
{
	No_key=0,
	K_MenuButton,	
	K_Exit,
	K_OK,
	K_All_Exit,
	K_LOK,
	K_I_Call,
    	K_E_Call,
    	K_Call_SW,
    	K_Secial_Services,
	K_Name_Box1,//10
	K_TimeSet,	
	K_AutoSwitch,			
	K_AnsweringService, 
	K_BurglarAlarm,
	K_Telephone_MENU,
	K_PhoneBook,
	K_LightTurnOn,
	K_Absence_Presence,
	K_Auto_Door_Open,
	K_Home_Auto,//20
	K_Door_Open_1,	
	K_Door_Open_2,
	K_Setting,	
	K_Phone_Book,
	K_UP,
	K_Down,
	K_Audio_Setting,
	K_Video_Setting,
	K_PhoneBook_Edit,
	K_CallVolume_Inc,// 30
	K_CallVolume_Dec,
	K_AudioVolume_Inc,
	K_AudioVolume_Dec,
	K_Beep_ON,
	K_Beep_OFF,
	K_Bright_Inc,
	K_Bright_Dec,
	K_Contrast_Inc,
	K_Contrast_Dec,
	K_Color_Inc,// 40
	K_Color_Dec,
	K_PhoneBook_Add,
	K_PhoneBook_Delete,
	K_PhoneBook_Modify,
	K_CallRinger1,	
	K_CallRinger2,
	K_CallRinger3,
	K_CallRinger4,
	K_CallRinger5,
	K_FloorCallRinger1,//50
	K_FloorCallRinger2,
	K_FloorCallRinger3,
	K_FloorCallRinger4,
	K_FloorCallRinger5,
	K_NotHide,
	K_Hide,
	K_MicroPhone_Mute,
	K_AnswerCall,
	K_43To169_rationswitch,
	K_Reset,//60
	K_CallSwitchBoard,
	K_Mornitor_PCM,
	K_Lift_UP,
	K_Lift_Down,
	K_About,
	K_PSTN_Answer_HandSet,
	K_Name_Box,
	K_Code_Box,
	ALARM_BLINK = 144,//no used
} Key_V_Type;

typedef enum KeyBoard_Type              
{
    K_Blank = 0x20,
    K_K_Symbol_Upcomma = 39,
    K_K_Symbol_comma = ',', 
    K_K_Symbol_ML = '-',
    K_K_Symbol_Dot = '.',
    K_K_Symbol_bias = '/',
    K_Num0 = '0',
    K_Num1,
    K_Num2,
    K_Num3,
    K_Num4,
    K_Num5,
    K_Num6,
    K_Num7,
    K_Num8,
    K_Num9,
    K_K_T0,
    K_K_Symbol_semicolon = ';',
    K_K_A = 'A',
    K_K_B,
    K_K_C,
    K_K_D,
    K_K_E,
    K_K_F,
    K_K_G,
    K_K_H,
    K_K_I,
    K_K_J,
    K_K_K,
    K_K_L,
    K_K_M,
    K_K_N,
    K_K_O,
    K_K_P,
    K_K_Q,
    K_K_R,
    K_K_S,
    K_K_T,
    K_K_U,
    K_K_V,
    K_K_W,
    K_K_X,
    K_K_Y,
    K_K_Z,
    K_K_Square_Brackets_Left = '[',
    K_K_Square_Brackets_Right = ']',
    K_K_Symbol_UL = '_',
    K_K_Delete = 127,
}KeyBoard_V_Type;

enum 
{
	KID_0,
	KID_1,
	KID_2,
	KID_3,
	KID_4,
	KID_5,
	KID_6,
	KID_7,
	KID_8,
	KID_9,
	KID_10,
	KID_11,
	KID_12,
	KID_13,
	KID_14,
	KID_15,
	KID_16,
	KID_17,
	KID_18,
	KID_19,
	KID_20,
	KID_21,
	KID_22,
	KID_23,
	KID_24,
	KID_25,
	KID_26,
	KID_27,
	KID_28,
	KID_29,
	KID_30,
	KID_31,
	KID_32,
	KID_33,
	KID_34,
	KID_35,
	KID_36,
	KID_37,
	KID_38,
	KID_39,
	KID_40,
	KID_41,
	KID_42,
	KID_43,
	KID_44	
};

#define K_Theme1    K_CallRinger1
#define K_Theme2    K_CallRinger2
#define K_Theme3    K_CallRinger3
#define K_Theme4    K_CallRinger4
#define K_Theme5    K_CallRinger5
/*
#define K_Num0  K_CallRinger1
#define K_Num1  K_CallRinger2
#define K_Num2  K_CallRinger3
#define K_Num3  K_CallRinger4
#define K_Num4  K_CallRinger5
#define K_Num5  K_FloorCallRinger1
#define K_Num6  K_FloorCallRinger2
#define K_Num7  K_FloorCallRinger3
#define K_Num8  K_FloorCallRinger4
#define K_Num9  K_FloorCallRinger5
*/
/*
#define No_key              0x00
#define K_AutoSwitch            0x01
#define K_AnsweringService  0x02  
#define K_PhoneBook         0x03  
#define K_MuteButton            0x04
#define K_CallSwitchBoard   0x05
#define K_LightTurnOn       0x06
#define K_Absence_Presence  0x07
#define K_Auto_Door_Open    0x08
#define K_Turn_Off          0x09
#define K_Door_Open_1       0x0a
#define K_Door_Open_2       0x0b
#define K_Setting               0x0c
#define K_Exit              0x0d
#define K_OK                    0x0e
*/

//--------------------------------------------------------------------
/*
#define M_PhoneBook 1
#define M_AutoOpen 2
#define M_Absent 3
#define M_REC_AutoOn 4
//#define M_UserModule 5
#define M_Setting 5


//setting page

#define S_Audio  1
#define S_Video  2
#define S_PhoneBook  3


//setting page 2
#define S_Scenario  1
#define S_Temperature  2
#define S_automatic  3
#define S_Lock1  4
#define S_Lock2  5
*/
//--------------------------------------------------------------------------------------
//PAGE REG
typedef enum Menu_States   						// key_state
{
	MessageManagment = 3,
	TimeManagment,
	WaitPage = 6,
	WaitWithShortCut,
	Panic_Cancle,
	MainMenu_1,
	MainMenu_2,
	MainMenu_3,
	MainMenu_4,
	AutoInsert_Selection, //
	PhoneBookEditList, //
	CallNumInput,//
	CallNumInput_KeyBoard,
	ThemeSelect,
	OClockSetting,
	ResetSelect,
	PSTN_Setting = 20,
	SettingSelect,//AudioAdjust,
	VideoAdjust,	
	PasswordInput,
	new_password,
	AlarmSystem_Setting,
	HomeAutomation_Setting,	
	PhoneBookEdit_Name_Code,
	PCM_Mornitoring, // not used
	PhoneBookDelSelect = 30,
	PhoneBookSelect,
	PhoneBookEditSelect,	
	Timer_Event_Edit_Parameter,
	Timer_Event_Edit_Event,
	CallNameCodeInput,
	Talk_To_CallModule_NotHide=40,
	Talk_To_SwtichBoard_NotHide,
	VideoMessagePlaying,
	Menu_CCTV,
	Menu_Telephone_Video,//44
	PCM_Auto_Insert,
	PSTN_Telephone = 48,
	PSTN_Telephone_Edit,
	Home_Automation = 50,
	Home_80_menu,
	AlarmStatus_Menu,//alarm status
	AlarmSystem_Manage,
	User_Scenario_Edit,
	Urmet_HA,
	Urmet_HA2,
	Tel_Contact_List,
	Tel_Call_Log,
	Alarm_handle,
	HomeAutomationDevice_List_S1,
	HomeAutomationDevice_List_S2,
	HomeAutomationDevice_List_S3,
	HomeAutomationDevice_List_S4,
	HomeAutomationDevice_List_S5,
	HomeAutomationDevice_List_S6,
	HomeAutomationDevice_List_S7,
	HomeAutomationDevice_List1 = 67,//60	
	Dimmer_Select = 84,
	RGB_Select,
	Shutter_Select,
	Shutter_R_L_Select,
	Shutter_Blind_Select,
	AC_Select,
	HVAC_Select,
} Menu_state_type;
/*
typedef enum Menu_States                        // key_state
{
    WaitPage = 6,
    MainMenu,
    CallNumInput,
    PhoneBookSelect,
    PhoneBookEditSelect,
    PhoneBookAddCall,
    CallNameCodeInput,
    PhoneBookEditList,
    PhoneBookDELList,
    ResetSelect,
    SettingSelect,
    AudioAdjust,
    VideoAdjust,    
    Audio_VideoAdjust,
    OnlyAudio_Setting,
    VideoMessagePlaying,
    Talk_To_CallModule_Hide,
    Talk_To_CallModule_NotHide,
    Talk_To_SwtichBoard_Hide,
    Talk_To_SwtichBoard_NotHide,
} Menu_state_type;
*/
#define MaxRecordNum  30
//#define CharactersNum 39                          //old value is 38, change to 39 is for add ' ' to the list
#define NameLen   16
#define CodeLen   4

#define RecordNumIn_one_page 5



#define uchar       unsigned char


typedef struct
{
	uchar calltype;
	unsigned char namelenth;
	unsigned char codelenth;	
	unsigned char name[NameLen];		
	unsigned char CallCode[CodeLen+1];	
}Intercom_Record;



//#define debug
#define   Nop    asm(nop;)

typedef struct{
	uchar  RCallMelody;
	uchar RFloorCallMelody;
	uchar CallVolume;
	//uchar BeepVolume;
	uchar AudioVolume;
	uchar Theme;

	union{
		uchar all;
		struct {
				unsigned Messages_Present:1;				//
				unsigned Mute:1;				//
				unsigned PasswordRequest:1;			//display OK button on the bar or not
				unsigned BeepVolume:1;				//
				unsigned Presence_Absence:1;				//
				unsigned Door_Auto_Open:1;			//
				unsigned HA_Passwordneed:1;				//
				unsigned PSTN_with_Video:1;				//
			}bits;				
		
	}S_Flag;
	//uchar RecordCnt;
	uchar TOCC;
	uchar MyColumn;
	uchar PSTN_Call_Melody;
} Setting;

#define     F_kind_of_ringtone_audioonly        SettingPara.RCallMelody
#define     F_kind_of_ringtone_floor        SettingPara.RFloorCallMelody
#define     F_kind_of_ringtone_Pstn        SettingPara.PSTN_Call_Melody
//#define F_presence_absence      SettingPara.S_Flag.bits.Presence_Absence
#define F_messages_present      SettingPara.S_Flag.bits.Messages_Present
#define F_Audio_Volume          SettingPara.AudioVolume
#define F_Volume_Call           SettingPara.CallVolume
#define F_Led_mute          SettingPara.S_Flag.bits.Mute
#define F_AutomaticDoorOpener	SettingPara.S_Flag.bits.Door_Auto_Open
#define F_HA_Passwordneed		SettingPara.S_Flag.bits.HA_Passwordneed
#define F_Presence_Absence		SettingPara.S_Flag.bits.Presence_Absence
#define F_PSTN_With_Video		SettingPara.S_Flag.bits.PSTN_with_Video
//#define   F_Button_for_dentist        SettingPara.AutoMatic

#define ParaStartAddr 0x00
#define RecordStartAddr 0x01    //change from 0x18 due to we have new page save it

#define     P_callmelody        2
//#define     P_floorcallmelody   3
//#define     P_callvolume        4
//#define     P_beepvolume    5
//#define     P_audiovolume   6
//#define     P_brightness    7
//#define     P_constrast     8
//#define     P_color         9
//#define     P_theme         7
//#define     P_backlightcolor  11
//#define     P_recordcnt       0//change from 12 to 0 due to this used a new page to save it
//#define     P_tocc            9
//#define     P_Column          10


//#define     P_ForMark       14


#define OnlyUpdateData      0
#define UpdateData_Show     1

//#define MenuHide            0
//#define MenuNotHide     1   

#define MAX_RINGTONES              5

#define Max_Volume_Levels       5
#define Min_Volume_Levels       1

#define Old_ENA_LCD Flag.bits.E_LCD_F



//for HA and Alarm System

#define MaxVideoStarQueue 22 //Homeautomation name and icon init need at least 26 frame
#define MaxKNX_TXQueue		4


//#define HCS08_FLASH_START_ADDR  0xEA00 
//#define HCS08_FLASH_START_ADDR1  0x078000	//COMMENT DUE TO aw32 debug

//----------------------------------------------------------------------------------
#define	SpaceByte		2
//#define TX_ENABLE	  PTFD_PTFD1		//·¢ËÍÊ¹ÄÜ
//#define Reset_Key	  PTCD_PTCD4

#define	MaxObject_In_OneMenu	8//one menu

/*
*******************************************
for JD
*/
#define LUNGH_BUFF_RX_JD  24
typedef struct
 {
  //unsigned char Start_JD;         // start byte
  //unsigned int	  Board_ID;         //  
  unsigned int	  Function_ID;         //  
  unsigned int	  Length;         // should be 00 06 
  uchar data[18]; //title 32bytes and message body 70bytes
 }JD_Data_TX;


typedef struct
 {
  unsigned char Start_JD;         // start byte
  unsigned int	  Board_ID;         //  
  unsigned int	  Function_ID;         //  
  unsigned int	  Length;         // should be 00 06 
  //uchar		BelongMenu;
  //uchar		Button_ID;
  uchar		data[9];
 }JD_Data_RC;

typedef struct
{
  uchar Year1;
  uchar Year2;
  uchar Year3;
  uchar Year4;
  uchar Month1;	
  uchar Month2;	
  uchar Day1;	
  uchar Day2;	
  uchar Week;
}JD_Date;

typedef struct
{
 
 uchar Hour1;
 uchar Hour2;
 uchar Min1;
 uchar Min2;
 uchar Sec1;
 uchar Sec2;
}JD_Clock;

//*********************************
#define	Switch_Menu_JD		8
#define	Set_Button_JD		9
#define	JD_Year_D			0x0001
#define	JD_Oclok			0x0002
#define	ChangeButton_Icon	0x0015
#define	ChangePhoto_Icon	0x0019
#define	SetButtonState		0x0009
#define	SetJD_oclock		0x0011
#define	SetJD_Date			0x0012
#define	Control_State		0x001D
#define	SET_DOC_CONTENT		0x001A


//-------------------------------------------------------------
typedef struct
{
	unsigned int DeviceID;
	uchar type;
}OutputButton;
/*
#define	Relay_Button	1
#define	Dimmer_Button	2
#define	RGB_Button	3
#define	Shutter_Button	4
*/
#define Filter_Relay_button	0x0002
#define Filter_Dimmer_button	0x0004
#define Filter_RGB_button	0x0008
#define Filter_Shutter_button	0x0010
#define Filter_Shutter_L_R_button	0x0020
#define Filter_Shutter_Blind_button	0x0040
#define Filter_HVAC_button	0x0080
#define Filter_AC_button	0x0100
#define Filter_Valve_button	0x0200
#define Filter_Valve1_button	0x0400
//#define Filter_AC_button	0x0100
#define	T_Relay	1
#define	T_Dimmer	2
#define	T_Shutter	3
#define	T_AC		4
#define	T_All_Device		8

typedef enum Button_States   						// key_state
{
	Hide_Icon = 0,
	Relay_Button,
	Dimmer_Button,
	RGB_Button,
	Shutter_Button,
	Shutter_L_R_Button,
	Shutter_Blind_Button,
	HVAC_Button,
	AC_Button,
	Valve_Button, //with icon 103 and 104
	Valve_Button1,//with icon 101 and 102
	Relay_Button_PC,
	Relay_Button_PO,
	Valve_Button_PC,
	Valve_Button_PO,
	Valve_Button1_PC,
	Valve_Button1_PO,
}Type_Button_Icon;
//homedevice type
#define Device_Type_882_80  1



#define	LightLevels	11  //0 is off then from 10% to 100% with step 10%
//-------------------------------------------------------------------------
/*-------------------------------------------
new protocol and interface for home automation feature
we will with up to 16 output and 10 input
---------------------------------------------*/



typedef struct
{
	unsigned int group_address;
	//uchar 	Command_Times[2];
	uchar	Button_Type;//01 normal relay, 02 dimmer , 03 RGB, 04 shouter  05 shouter_left and right
	uchar	Button_Name[8];
	uchar	Button_Name_Len;
	unsigned int	Button_Status;
	uchar	Protocol_Type;//0 HRB, 1 KNX
	unsigned int group_address1;
	unsigned int group_address2;
	unsigned int group_address3;
	
}OutPutButtons; 
#define	HomeAutomation_MaxButtons		16

#define	Protocol_HRB	0
#define	Protocol_KNX	1

#define	Alarm_Instants		0
#define	Alarm_Delay		1
#define	Alarm_24H_OnDuty		2



#define	Alarm_Tpye_Disable		0
#define	Alarm_Tpye_Burglar		1
#define	Alarm_Tpye_Panic		2
#define	Alarm_Tpye_Fire		3
#define	Alarm_Tpye_Gas			4
#define	Alarm_Tpye_Flooding	5
#define	Alarm_Tpye_Intrusion	6
#define	Alarm_Tpye_Coercion	7
#define	Alarm_Tpye_Tamper		8
#define	Alarm_Tpye_DoorForced		9


typedef struct
{
	unsigned int group_address;
	//uchar 	Command_Times[2];
	uchar	Alarm_Type;// the alarm type to switchboard
	uchar	Input_Name[8];
	uchar	Input_Name_Len;
	unsigned int	Input_Status; //NC OR NO un_used
	uchar 	property; // 0 instants , 1 delay 2 24H(can not be disable
	uchar	Delay_Time;
	uchar	Zone_Belong;//from zone1 to zone6 //contain 6 inputs
	unsigned int Alarm_Event_GroupAddress; //when get the alarm information we can driver some other output event
	uchar	Alarm_Event_Value;
	
}InPutAlarm;
#define	HomeAutomation_MaxInput		10
typedef struct
{
	uchar AlarmMode;
	uchar Away_Zone_Belong; //
	uchar Home_Zone_Belong;
	uchar AlarmZoneEnable;//6bits mapped 6 zones
	unsigned int Enable_group_address;
	uchar E_ID;
	unsigned int Disable_group_address;
	uchar D_ID;
	uchar En_Alarm_DelayTime;
}AlarmSystem;

typedef struct
{
	unsigned int group_address;
	uchar S_ID;	
	
	uchar name[8];
	uchar lenth;
	uchar Icon_Protocol_Type;//low 4bit 0 HRB, 1 KNX, 2 both  high 4bits for icon ID 
}Scenario_Setting;

typedef struct
{
	unsigned int group_address;
	uchar S_ID;	
	//uchar Icon_Protocol_Type;//low 4bit 0 HRB, 1 KNX,  high 4bits for icon ID //comment because for this case we have to used HRB with 882-80 
	uchar name[8];
	uchar lenth;
}CCTV_Setting;

typedef struct
{
	uchar R;
	uchar G;
	uchar B;
	
}RGB_Para;


typedef union{
 struct {
  unsigned C_H:2;//0: atuo 1: cold 2: heat;3: dry
  unsigned mode:2;				//0:comfort 1:night 2:save // for HVAC 3 is standby mode
  unsigned speed:2;			//display OK button on the bar or not
  unsigned Swing:2;				//
  				//
	}bits;				
	uchar all;
}A_CStatus;

/*
typedef struct
{
	A_CStatus A_Cstatus;
	
	
}ACStatus; //for air condition status
*/

typedef struct
{	
	uchar Icon_ID;	
	uchar name[8];
	uchar lenth;
}Room_Setting;

typedef struct
{
	uchar name[8];
	uchar lenth;
}ZoneNameS;
#define	ZoneNum	6

typedef struct
{
	uchar Installer[6];
	uchar User[6];
	
}Password_Alarmsystem;

typedef enum Role_AlarmSystem_Type   						// key_state
{
	No_regedit = 0,
	Installer,
	A_User,
} Role_AlarmSystem;


typedef struct
{
	uchar name[8];
	uchar lenth;
}Tel_Memory_S;

typedef struct
{
	uchar D_key[6];
	uchar lenth;
}DTMF_Key;
#define	Tel_Memory_Num	4

typedef struct { 
  uchar    Head1;    // Always is 0x55=STX1. 
  uchar    Head2;    // Always is 0xAA=STX2. 
  unsigned int      MachineID;  // terminal¡¯s ID. 
  unsigned int      Reserved;  // Reserved 
  unsigned int    Command; // Command ID 
  //long unsigned int  Length;  // Input Parameter 1 
  uchar	config;
  uchar	length;
  unsigned int  Dest_Address;
  unsigned int  InParam;  // Input Parameter 2 
  unsigned int    ChkSum;  // Checksum 
} CMDPKT;                                // Size of command packet is 16 Bytes. 

typedef struct { 
  uchar    Head1;    // Always is 0x55=STX1. 
  uchar    Head2;    // Always is 0xAA=STX2. 
  unsigned int      MachineID;  // terminal¡¯s ID. 
  unsigned int      SRC_ADDR;  // 
  unsigned int      Dest_ADDR;  // 
  uchar    nlength; // 
  unsigned int R_Flag;
  uchar Data[2];  
  unsigned int    ChkSum;  // Checksum 
} CMDPKT_Status;   

typedef enum DTMF_States   						// key_state
{
	DTMF_Idle = 0,
	DTMF_Ready,
	DTMF_CallCode,
	DTMF_StopCode,
	Tel_Talk,
} DTMF_state_type;

typedef enum Tel_Det_States   						// key_state
{
	Tel_Idle = 0,
	Tel_First_Low,
	Tel_First_High,
	Tel_Second_Low,
	Tel_Second_High,
	
} Tel_Det_type;

typedef struct
{
	uchar Tel_Num_Len;
	uchar Tel_Number[16];
	//uchar Inf_address;
}Tel_Memory_Record;

typedef struct
{
	uchar Hour; // hour from 0 to 23
	uchar Minite;//minite form 0 to 59
	uchar mode; // , bit0 is for enable or disable, then bit1 to bit7 is for  Sunday to Saturday
	//uchar event;//low 4 bit for scenario ID, high 4bits for event mode, ringtone or scenario or both
	//uchar event_mode;//scenario or ringtone or both
	uchar Protocol_Select[5]; // low 4 bit is for protocol, HRB or KNX or Both,bit NO 4 is for the length of KNX data; bit No 5 is for scenario,bit No 6 is for RGB,bit No 7 for HVAC
	unsigned int  Inf_address[5];
	uchar Inf_ID[5];
}Timer_Event;




#define	PCM_Call_Button_Detect_Time	10


typedef enum AC_Address   						// state
{
	Temperatire_Setting_Update = 0,
	Power_ON_Off,
	Heat_Cool,
	Auto_Dry,
	High_Midle_Speed,
	Low_auto_Speed,
	LR_UD_Swing,
	Auto_Stop_Swing,
	Comfort_Mode,
	Night_Mode,
	Save_Mode,
	Heat_16_17,
	Heat_18_19,
	Heat_20_21,
	Heat_22_23,
	Heat_24_25,
	Heat_26_27,
	Heat_28_29,
	Heat_30_31,
	Cool_16_17,
	Cool_18_19,
	Cool_20_21,
	Cool_22_23,
	Cool_24_25,
	Cool_26_27,
	Cool_28_29,
	Cool_30_31,
} AC_Address_Map;

typedef enum HVAC_Address   						// state
{
	HVAC_Temperatire_Setting_Update = 0,
	HVAC_Power_ON_Off,
	HVAC_Heat_Cool,	// synchronization with other devices
	HVAC_HeatWind_Speed,
	HVAC_CoolWind_Speed,
	HVAC_Comfort_Mode,
	HVAC_Night_Mode,
	HVAC_Save_Mode,
	HVAC_Heat_Command,
	HVAC_Cool_Command,
	HVAC_Temperature_Sensor, //when get a new value, the modo should decide open or stop the valve of the heat or cool
	
} HVAC_Address_Map;

#define	P_UserPassword 8
#define Address_Inf_StartAddr 0x18
#define	Alarm_Zone_Addr	15 //NO 18 address position
#define	Alarm_Flag_Addr	17 //NO 18 address position

#define LUNGH_BUFF_TX  16
#define LUNGH_BUFF_RX  16
typedef struct
 {
  unsigned char tx_buf[LUNGH_BUFF_TX];         // character row position 
  unsigned char BccMsgLen;    // function linked to field
 }Hrb_tx_buf;

typedef struct
 {
  unsigned int Group_Address;         // character row position 
  unsigned int Value;
  uchar Length;
 }KNX_Queue;

#define	K_TelCall1	31
#define	K_TelCall2	32
#define	K_TelCall3	33
#define	K_TelCall4	34
#define	K_TelHookOn	11
#define	K_TelHookOff	10
#define	K_TelCallStar	22
#define	K_TelCallShart	21

#define	K_PSTN_Setting	8

#define	K_Mute		21

#define	K_PSTN_HRB_Switch 	6

#define	K_Zone1	11
#define	K_Zone2	12
#define	K_Zone3	13
#define	K_Zone4	14
#define	K_Zone5	15
#define	K_Zone6	16

#define	K_Clear		18
#define	K_DisArm	20
#define	K_Password_OK	26
#define	K_Home_Alarm	35
#define	K_Away_Alarm	36
#define	K_AlarmStatus	37


#define	K_Min1_Inc	47
#define	K_Min1_Dec	48
#define	K_Min2_Inc	50
#define	K_Min2_Dec	51

#define	K_Day1_Inc  21
#define	K_Day1_Dec 28
#define	K_Day2_Inc  17
#define	K_Day2_Dec 18

#define	K_Month_Inc  38
#define	K_Month_Dec 36

#define	K_Year_Inc  30
#define	K_Year_Dec  31

#define	K_Stop 	27


//extern button
#define OldGasDectorStatus	SystemStatus.bits.Oldgasstatus
#define GasAlarmF		New_Flag3.bits.GasAlarm
#define OldAudioPhone_Lock_button	SystemStatus.bits.AudioPhone_Lock_button
#define OldAudioPhone_Call_switchboard		SystemStatus.bits.AudioPhone_Call_switchboard
#define OldAudioPhone_Hook		SystemStatus.bits.AudioPhone_Hook
#define OldPCM1_Call_MODO_Button		SystemStatus.bits.PCM1_Call


#define	CLASS_DEC	0x08	// Destination class DEC
#define	CLASS_PALM	0x0E	// Destination class PALM



#define PALM_TO_DEC	16*CLASS_PALM+CLASS_DEC
#define DEC_TO_PALM	16*CLASS_DEC+CLASS_PALM

#define	RC_DADA_ACK		0xD3

#define	OPC_CCTV_Scenario_Wirte	0xD4
#define	OPC_CCTV_Scenario_Read	       0xD6
#define	OPC_CCTV_Scenario_ACK	       0xD7

#define	OPC_Alarm_System_Wirte	0xD8
#define	OPC_Alarm_System_Read	       0xD9
#define	OPC_Alarm_System_ACK	       0xDA

#define	OPC_HA_System_Wirte	0xDB
#define	OPC_HA_System_Read	       0xDC
#define	OPC_HA_System_ACK	       0xDD
#define	OPC_DADA_ACK		0xDA


#define	F_RingRepeatT	5
#define	F_RingRepeatTs	2 //old is 2, changed due to need default repeat ringtone is 1
