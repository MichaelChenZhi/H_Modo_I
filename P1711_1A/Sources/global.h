
// global.h

#ifndef MAIN
  #define EXT   extern
#else
  #define EXT
#endif

// Seriale verso il conexant
//#define LUNGH_BUFF_TX  10
//#define LUNGH_BUFF_RX  5

EXT unsigned char TEST_AA;
EXT unsigned char Slave_Spi_EN_Dly;
EXT uchar Rc_Tel_num[22];
EXT uchar PCM_OnOff_Interval;     //steven ask add (PCM off and DELAY 3S ,the PCM can be on) 

EXT void Rc_Tel_Num(void);

EXT unsigned int timer_sec;                     // for conteggio seconds
EXT unsigned char b1;

// parametri for impostazione tone bitonal
#pragma DATA_SEG FAR_RAM
EXT unsigned char tone_proc;
EXT unsigned int period_f0;                      //  total tone period
EXT unsigned int period_f0_on;                   //  ON period
EXT unsigned int numper_f0_on;
EXT unsigned int numper_f0_off;
EXT unsigned int period_f1;                      // total tone period
EXT unsigned int period_f1_on;                   // ON period
EXT unsigned int numper_f1_on;
EXT unsigned int numper_f1_off;
EXT unsigned char num_door_opencket_biton;               // counter packet of bitonal spaced by pause
EXT unsigned char num_sequence_biton;             // counter sequence packet of bitonal+pause spaced by pause long
// variables for set kind ofnota by to be emitted
EXT unsigned char tone_num_door_openk_biton;             // number of packet of bitonal spaced by pause
EXT unsigned char tone_num_seq_biton;             // number of sequence packet of bitonal+pause spaced by pause long
EXT unsigned char kind_of_ring;                   // kind of ring to be emitted
EXT unsigned char kind_of_ring_save;              // saving kind of ring by to be emitted (for sequence)
EXT unsigned char kind_of_ring_bck;               // saving kind of ring used to restore error condition where Setup_tone with kind_of_ring=0
EXT unsigned char call_from;                      // source of call (used toi define tone parameters (pause, sequence, etc.))
EXT unsigned char save_call_from;                 // saving source call for delayed ring in internal 2 and 3
EXT unsigned int tone_door_openuse_bitonal;              // time pause between packet bitonal
EXT unsigned int tone_door_openuse_sequence;             // time pause long between sequence packet bitonal+pause

#pragma DATA_SEG DEFAULT
EXT unsigned int numper;
EXT unsigned int numper_on;
EXT unsigned int numper_off;
EXT unsigned int timer_repeat_tone;               // timer iterations sequence tone by to be emitted
EXT unsigned int timer_door_openuse_tone;                // timer for management of pause between packet note
EXT unsigned char phase_tone;
EXT unsigned char freq_tone;
EXT unsigned int tone_t_rep;

#pragma DATA_SEG FAR_RAM
EXT unsigned char   proc_scan;
EXT unsigned char   timer_proc_scan;                // timer button scan
EXT unsigned char   hook_state;
EXT unsigned char   hook_state_toggled;
EXT unsigned char   hook_timer;                     // for debounce HOOK
EXT unsigned char   dip_sw;                         // state dip-switch

EXT unsigned char   function;                       // function linked to button pressed
EXT unsigned int    button;                          // code button pressed
EXT unsigned int    button_2s;
//EXT unsigned char   state_puls;                          // 
//EXT unsigned char   cnt_debounce_puls;
//EXT unsigned char   timer_persistence;
//EXT unsigned int    cnt_tasti;                      // counter tasti premuti (!!!!!debug)
                    
EXT unsigned char   state_door_opennic;

EXT unsigned int	  timer_wait_offhook_audio;

//EXT unsigned char   timer_pre_occupato_D; 		// timer wait D_PRESENCE for altri

EXT unsigned char	  Internal;
EXT unsigned char	  User;
EXT unsigned char	  MODO_State;
EXT uchar	Sub_MODO_State; //use for PSTN CCTV and so on
EXT unsigned char	  timer_blinking_of_door_open_led;
EXT unsigned char	  phase_led_on_door_open;

EXT unsigned char	  timer_blinking_of_messages_led;
EXT unsigned char	  phase_led_on_mess;

EXT unsigned char	  timer_ring_delay;
EXT unsigned char	  timer_delayed_event;
EXT unsigned char	  Frame_copy[8];

EXT unsigned char	  kind_of_my_communication;   	// 

EXT unsigned char	  number_SVC;					// Number of video clip in progress for D_SVC_DELETE

EXT unsigned char	  TX_ST_completed;
EXT unsigned char	  Messages_States;

EXT unsigned char	  Timer_Retry_D_Panic;
EXT unsigned char	  Num_Retry_D_Panic;

EXT unsigned char	  Timer_Retry_D_Alarm;
EXT unsigned char	  Num_Retry_D_Alarm;

EXT unsigned char	  Timer_Retry_D_PresenceAbsence;
EXT unsigned char	  Num_Retry_D_PresenceAbsence;

//EXT unsigned char 	timer_second;
//EXT unsigned char 	Secondo_trascorso;					// bit

EXT unsigned char	  Re_TX_Buffer[20];					// Buffer for frame to be re-transmitted when ack wait is elapsed
EXT unsigned char   timer_100ms;
EXT unsigned char   timer_10ms;
EXT unsigned char   timer_1min; 
EXT unsigned char 	TEST_Phase_Active;							// phase of test active (test procedure)
EXT unsigned char 	Old_Value;											//variable of supporto (test procedure)

EXT unsigned char 	Actual_Expansion_Value;					//
EXT unsigned char 	Old_Expansion_Value;						//
EXT unsigned char 	debounce_dentist;						  //
//EXT unsigned char 	Button_for_dentist;              // bit imagine dello state button dentist

EXT unsigned char 	Battery_State;						      // Bit Stato batteria

EXT unsigned char		* ST_p_ret_val;
EXT unsigned char   * ptr_fl;
EXT BitV            BitVars;
EXT BitV1           BitVars1;


EXT unsigned char 	R_kind_of_ringtone_cito;
EXT unsigned char 	R_kind_of_ringtone_floor;
EXT unsigned char 	R_kind_of_ringtone_PSTN;

EXT unsigned char 	R_Audio_Volume;
EXT unsigned char 	R_Volume_Call;
EXT unsigned char   R_presence_absence;
            


EXT unsigned char 	edges_counter;              
EXT unsigned char 	burst_lenght;
EXT unsigned char 	open_door_button;
EXT unsigned char 	button_lasting;
EXT unsigned char 	switch_on_button;            // state button P_LCD_ON
EXT unsigned char 	button_lasting_lcd;          // for debounce

EXT unsigned char   timer_delay_offhook;

EXT unsigned char   hook_timer_idle;     // timer of riaggancio automatic nel caso che A IDLE l'utente abbia dimenticato sganciato

EXT unsigned char   dummy;

EXT unsigned int     WD_line_der;  // WDLINE

//EXT unsigned char   state_old_mute_ok;       // for detect the transizione 1-->0 of MUTE_OK
//EXT unsigned char   tim_pulse;               // for generate SPK_MUTE, MIC_MUTE pulse 

//EXT unsigned char   timer_audio_poweron;     // for audio to power on sequence 
//EXT unsigned char	  ST_Audio;

// Variables for the management of codes to be called 







                                          
EXT unsigned char   OLD_MODO_State;            // Previous state used to detect the transitions
EXT uchar OLD_Sub_Modo_State;

EXT unsigned char   Timer_Inactivity;          // Timer auto-power off 
//EXT unsigned char   Timer_ExitPSTN;          // Timer auto-power off 
                                                                          
//EXT unsigned char   k, w;                    // variables for touchscreen

EXT unsigned char   temp;                    // variables 
//EXT unsigned char   i;                       // variables 

//EXT unsigned char   Volume_Level;             // Livello Volume ottenuto come % del duty for the tone by rapprenestare
//                                              // :2 vuol dire volume max. Values standard /4 /8 /16

                                              
//EXT unsigned char   buffer_tx[LUNGH_BUFF_TX]; // buffer transmission to conexant
//EXT unsigned char   buffer_rx[LUNGH_BUFF_RX]; // buffer reception from conexant
//EXT unsigned char   index_buffer_tx;          // transmission to conexant
//EXT unsigned char   index_buffer_rx;          // reception by conexant
//EXT unsigned char   n_byte_to_tx;             // 
//EXT unsigned char   n_byte_rx;                // 

//EXT unsigned char   Conexant_Link_Status; 

//EXT unsigned char   Vol_to_set;               // Max. Vol = 0 (+6dB), Min. Vol = 0xB6 (-68dB)
EXT unsigned char   Volume;
//EXT unsigned char   Volume_quantized;
//EXT uchar	HideState;
EXT unsigned char   bck_function;
EXT unsigned char   Button_for_dentist;       // bit imagine of dentist button
//EXT unsigned char   timer_logo;               // timer for blink logo led  (presence/absence) 

EXT unsigned char   int_k;                    // variable used inside serial interrupt



//EXT unsigned char   code;                         // panic button 

//EXT unsigned char   Temp[4];                        // For temporary buffer

EXT unsigned char   Timer_GUI_locking;              // For lock the GUI 
//EXT unsigned char   Timer_wait_call_from_CEN;       // 

EXT unsigned char   Timer_Wait_Tone;           // Timer used to delay soud emission until conexant has applied volume values.



//EXT unsigned char   Timer_4SwitchB_call                // Timer for read debounce during TS calibration

EXT uchar	MenuSleepTime;


EXT unsigned char   Timer_SHSTC;                       // Timer for evaluate if Switchboard Has Stored The Call




//----------------------------------------------------------
// add By Michael
EXT unsigned char PageStatus;		// page state
EXT unsigned char OldPageStatus;
EXT unsigned char KeyValue;
//EXT unsigned char LastKeyValue;

//EXT unsigned char SubItemSel;		//
EXT unsigned char ItemIndex;
EXT unsigned char PhoneRIndex;
//EXT unsigned char TypeIndex;
EXT unsigned char PhoneBookPage;
//EXT unsigned char name[NameLen];
//EXT unsigned char namelenth;
//EXT unsigned char CallCode[CodeLen+1];
//EXT unsigned char codelenth;

EXT Intercom_Record Temp_Record;
/*
EXT unsigned char SavePhoneRIndex;
EXT unsigned char SaveTypeIndex;
EXT unsigned char Savename[NameLen];
EXT unsigned char Savenamelenth;
EXT unsigned char SaveCallCode[CodeLen+1];
EXT unsigned char Savecodelenth;
*/
//EXT unsigned char inputindex;
EXT unsigned char inputfocus;		//input focus position
EXT unsigned char temp1;
EXT unsigned char temp2;
EXT unsigned char temp3;
EXT unsigned char temp4;
EXT unsigned char temp5;
EXT unsigned char temp6;
EXT unsigned char temp7;
EXT unsigned char temp8;
EXT unsigned char temp9;
//EXT uchar RingVolume;
EXT unsigned char ringtone_type; 

//EXT unsigned char AddressNO[RecordNumIn_one_page];
EXT unsigned char 	BackMenu;
//EXT unsigned char Rationswitch;
EXT uchar VolumeMenu;




//for JD Module
//************************************
EXT unsigned char index_buffer_tx_JD;
EXT uchar	tx_len_JD;
EXT uchar	tx_buf_Program[28];

EXT uchar	Old_index_buffer_rx_JD;
EXT uchar	index_buffer_rx_JD;
EXT uchar	rx_buf_Program[LUNGH_BUFF_RX_JD];

EXT uchar	JD_RxDone;			//JD Message received for me
EXT JD_Data_TX JD_TX_Queue[MaxVideoStarQueue];

EXT	uchar	JD_TX_Frame_Index;
EXT	uchar	JD_Save_Frame_Index;
EXT	uchar	JD_TX_Frame_Time;
//EXT TX_VideoStar VideoStarQueue[MaxVideoStarQueue];

EXT uchar	JD_Buf_Clear_Time;
EXT union{
 struct {
  unsigned editPhoneBook:1;				//
  unsigned KeyNhandle:1;			//
  unsigned Set_AB_Server:1;			//
  unsigned SavePBRQ:1;				//
  unsigned DelPhoneBook:1;				//
  unsigned :1;				//
  unsigned SaveAP:1;				//
  unsigned E_LCD_F:1;				//
	}bits;				//
	uchar all;
}Flag;
//--------------------
/*
EXT union{
 struct {
  unsigned :1;			//Present  900ms 
  unsigned :1;   //send Ack Present frame
  unsigned :1;				//
  unsigned :1;			//
  unsigned :1;			//
  unsigned :1;				//
  unsigned :1;			//
  unsigned :1;				//
	}bits;				
	uchar all;
}Flag3;
*/
EXT union{
 struct {
  unsigned :1;			//Present  900ms 
  unsigned MicMute:1;   //send Ack Present frame
  unsigned GasAlarm:1;				//
  unsigned :1;			//
  unsigned :1;			//
  unsigned :1;				//
  unsigned :1;			// no used
  unsigned :1;				//
	}bits;				
	uchar all;
}New_Flag3;

EXT union{
 struct {
  unsigned :1;			//
  unsigned :1;				//
  unsigned :1;				//
  unsigned :1;			//
  unsigned DefaultS:1;				//
  unsigned :1;				// use to count time without any button press
  unsigned :1;			//check whether have enough time to videostar 
  unsigned Audio_Connect_Flag1:1;				//
	}bits;				
	uchar all;
}Flag4;

EXT union{
 struct {
  unsigned AudioPhone_Lock_button:1;			//
  unsigned AudioPhone_Call_switchboard:1;				//
  unsigned AudioPhone_Hook:1;				//
  unsigned Oldgasstatus:1;			//display OK button on the bar or not
  unsigned UpdatePB:1;				//
  unsigned PCM1_Call:1;				//
  unsigned Old_Pres_BT:1;			//
  unsigned CallSwitchBoard_Icon:1;				//
	}bits;				
	uchar all;
}SystemStatus;

EXT union{
 struct {
  unsigned Setting_F:1;			//
  unsigned HA_F:1;				//
  unsigned SecuritySetting:1;				//
  unsigned :1;			//display OK button on the bar or not
  unsigned :1;				//
  unsigned :1;				//
  unsigned :1;			//
  unsigned REL2_Delay:1;				//
	}bits;				
	uchar all;
}PasswordRequest;


EXT union{
 struct {
  unsigned :1;			//
  unsigned :1;				//
  unsigned :1;				//
  unsigned RD_Tel_Num_flag:1;			//display OK button on the bar or not
  unsigned EN_Capture:1;				//
  unsigned blind_audio:1;				//
  unsigned Dly_EN_Capture_Flag:1;			//
  unsigned :1;				//
	}bits;				
	uchar all;
}Common_falg;


EXT Setting SettingPara;
EXT void (*SaveInf)(void);
EXT uchar SaveData[336];    //use to copy a page data
EXT uchar ICON;
EXT unsigned int AddressResult;

//
EXT JD_Clock	R_Clock;//real clock
EXT JD_Date	R_Date;//real date
EXT JD_Clock	R_Clock_Temp;//real clock
EXT JD_Date	R_Date_Temp;//real date
EXT	uchar	JD_start_wait;

//EXT uchar ScenarioCallBack;
EXT uchar Tel_num[16];
EXT uchar Tel_index;
//EXT uchar Tel_det_High;
EXT unsigned int Tel_det_Low;


EXT uchar BackMenu;
//EXT D882_80_Inf *pp;
EXT uchar Edit_Index;
EXT uchar Tel_Det_State;
EXT uchar RoomIndex;
EXT uchar RoomSelect;
EXT uchar Button_Index;
EXT uchar KeySelect;

EXT uchar Alarm_Enable_Time_Delay_CNT;

EXT uchar CheckDTMF_Delay;
EXT uchar Object_Button_Same_Cnt;
EXT uchar Object_ID;
EXT uchar Filter_ID;
EXT uchar Buttonlist_Menu_ID;

EXT uchar Filter_Button_Select;
EXT uchar Filter_KeySelect;

EXT uchar KNX_RxBuf[LUNGH_BUFF_RX]; // buffer reception from conexant
EXT uchar KNX_R_len;
//EXT uchar Wait_times_KNX;
EXT	uchar	KNX_TX_Frame_Time;
EXT Tel_Memory_Record *Tel_num_Record;
EXT uchar LightStatus[24];

EXT uchar DlySendTimeEven;

EXT unsigned long int TPM2_ch1_pluse_val;
EXT unsigned long int pluse_width_base;
EXT unsigned int TPM2_ch1_pluse_begin_val;
EXT unsigned int TPM2_ch1_pluse_new_val;
EXT unsigned char TPM2_ch1_interrupt_times;
EXT unsigned char blind_audio_error_times;
EXT unsigned char blind_audio_period_limit;
EXT unsigned char blind_audio_period_times;
EXT unsigned char TPM2_time_overflow_times;
EXT unsigned char blind_audio_dec_times;
EXT unsigned char Dly_EN_Capture;
EXT unsigned char TPM2_capture_times;
EXT unsigned int capture_counter[3];
EXT unsigned char capture_overflow[3];
EXT	unsigned char	BlindAudio_NO;
EXT	unsigned char	OLD_Sub_MODO_State1;//ԭOLD_MODO_State1;
EXT	unsigned char	BlindAudioWaveWaitTime;
EXT	unsigned char	ClearTelDispFlag;
EXT unsigned char  TelRdFrameTime;
EXT unsigned char  SpiSendBuf[4];
EXT unsigned char  RdTelNumErrorTimes;

//EXT uchar SaveData11[512];
EXT union{
	
	uchar MsgBuf[85];	
	Tel_Memory_Record Tel_Log_Number[5];
	
}Memory_S;//for share the memory space
EXT uchar Tel_Log_Index;


EXT union{
 struct {
  unsigned ReflashTimerCount_F:1;			//
  unsigned StopToneNeed:1;				//
  unsigned PSTN_Video_State_Hope:1;				//
  unsigned PSTNOnRingTone:1;			//
  unsigned T5_F:1;				//
  unsigned T6_F:1;				//
  unsigned T7_F:1;			//
  unsigned T8_F:1;				//
	}bits;				
	uchar all;
}EventHandle_Flag;



EXT A_CStatus AC_Status[12];
EXT uchar A_CTemperature[12];//with 12 room's air condition
EXT uchar AC_Temperature_From_Sensor[12]; //get the data form knx device 

EXT Timer_Event Timer_Event_temp;
EXT uchar Timer_Event_Index;
EXT uchar Timer_Event_Inf_Index;
EXT uchar Timer_Event_Delay;
//EXT Timer_Event R_Timer_Event;
EXT union{
 struct {
  unsigned T1_F:1;			//
  unsigned T2_F:1;				//
  unsigned T3_F:1;				//
  unsigned T4_F:1;			//display OK button on the bar or not
  unsigned T5_F:1;				//
  unsigned T6_F:1;				//
  unsigned T7_F:1;			//
  unsigned T8_F:1;				//
	}bits;				
	uchar all;
}TimeEvent_Flag;

EXT union{
 struct {
  unsigned T1_F:1;			//
  unsigned T2_F:1;				//
  unsigned T3_F:1;				//
  unsigned T4_F:1;			//display OK button on the bar or not
  unsigned T5_F:1;				//
  unsigned T6_F:1;				//
  unsigned T7_F:1;			//
  unsigned T8_F:1;				//
	}bits;				
	uchar all;
}TimeEvent_Flag_Done;//this flag for which event has done and then check if it need change to disable state due to single event.
EXT uchar Event_Step;
EXT	unsigned int Timer_Handle_TimerEvent[8]; // for count the time when we need handle the new timer event
EXT	unsigned int Timer_Reflash_Count; // for count time when we need reflash the count

//EXT uchar		MsgBuf[80];

EXT uchar RoleAlarmSystem;

EXT AlarmSystem *AlarmP;
EXT Password_Alarmsystem *A_password;

EXT uchar Alarm_DelayTime_Cnt[HomeAutomation_MaxInput];
EXT uchar Alarm_Delay_Tone_Cnt;

EXT void (*JDFunctionCallBack)(void);
EXT void (*JDFunctionCallBack1)(void);
EXT void (*JDFunctionCallBack2)(void);
EXT void (*ExitMenu)(void);//use to exit save timer event edit menu
EXT uchar SetTimeDelay;

//EXT uchar	RingRepeatTime; //ring repeat times
EXT uchar PSTN_Call_TimeOut_Cnt;

EXT uchar DTMF_State;
EXT uchar DTMF_wait;

EXT uchar	MainMenu;
EXT uchar Max_RC_Index;
EXT uchar Max_TX_Index;

EXT uchar KeyLimitDly;

EXT uchar KNX_Buf[16]; //for received handle
EXT uchar KNX_Buf_TX[16];
EXT uchar KNX_Buf_TX_Length;
EXT uchar KNX_Buf_RC[16];
EXT uchar	KNX_R_TX_P; //for count the tx position
EXT uchar	KNX_R_RC_P; //for count the rc position
EXT uchar	KNX_R_RC_P_old;
EXT uchar	KNX_R_RC_Verify;
EXT uchar	FrameV_T_CNT;
EXT KNX_Queue KNX_Q_Buffer[MaxKNX_TXQueue];
//EXT Hrb_tx_buf HRB_TXBUF[MaxCan_TXQueue];
EXT uchar	KNX_TX_Index;
EXT uchar	KNX_Save_Index;
EXT uchar	HRB_TX_Time_CNT;// every frame time count we have to wait 30ms and then transmit next frame
EXT uchar	VolumeBar_DisplayTime;
EXT uchar	PanicCancleTime;

EXT uchar RGB_Light;
//EXT uchar RGB_Time_Cnt;
EXT unsigned int Group_Address[2];
#ifdef Hide_Button_Solution	
EXT uchar Need_Show_buttons;
#endif

EXT unsigned int AlarmKnowsFlag;
EXT unsigned int AlarmNeed_Reset_Flag;
EXT uchar AlarmDisplay_Channel;//current display channel
EXT unsigned int Alarm_Type_To_Switchboard;

EXT uchar WarningToneTime;
EXT uchar WarningToneNumber;

EXT uchar BT_Check_Time;

#pragma DATA_SEG DEFAULT
EXT uchar	Gas_detector_button_time;
EXT uchar	Lock_button_time;
EXT uchar	call_switchboard_button_time;
EXT uchar	Hook_button_time;
EXT uchar	PCM1_Call_MODO_Button_time;
//EXT Tel_Memory_Record Tel_Log_Number[5];
//EXT uchar Tel_Log_Index;
//EXT uchar	GasAlarmToneTime;
//EXT uchar	CallSwitchBoardTime;
EXT uchar	CallSwitchBoard_From;
EXT uchar	NoKeyTime;

EXT uchar PCM_Button_Detect_Wait;
EXT	unsigned int	Pcm_TalkTime; //count 
EXT uchar Pin_IO_Set_Delay;
EXT uchar	PCMcallingTime;
//EXT uchar	RingRepeatTimeCnt; // count the time when need repeat the ring again, every 4s should repeat the ring

EXT uchar VolumeCNT;
EXT uchar	PCM_LOCK_TIME;
EXT uchar Read74HC165_Time;//every 500ms read one time 74HC165 for get the dip-switch

EXT uchar Simulate_Hook_On_Time; //

EXT uchar Delay_Hook_Press_Time;
//EXT unsigned char   Timer_wait_call_from_CEN;       // 