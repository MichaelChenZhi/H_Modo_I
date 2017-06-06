

#include <hidef.h> /* for EnableInterrupts macro */
#include "MC9S08AW60.h"                /* I/O map for MC9S08AW16CFU */
#include "define.h"
#include "global.h"
#include "Ges_der.h"
#include "Driv_der.h"
#include "Trame.h"
#include "Ges_scan.h"
//#include "Touch.h"
#include "Spi.h"
#include "VideoStar.h"
#include "FlashWrite.h"

void Courtesy_tone(void);
void Short_tone(void);
void Floor_call_tone(void);
unsigned char Maxim_2_Ascii(unsigned char val);
void Remove_blink(void);
void Management_Button_for_LCD(void);


// extern void Gest_Apriporta(void);
//extern void Gest_Espansione(void);
extern void Gest_Panic(void);
extern void Presence_Absence_Management(void);
extern void Button_Selfinsertion_Cyclic(void);

extern void Set_Speaker_Volume(unsigned char vol);
extern unsigned char Quantize(unsigned char vol);
extern void SettoWaitPage(void);
extern void BeepOK(void);

void Reset_Input_Parameters(void); 
void Switch_Off(void);



void Play_New_Melody(unsigned char kind);

//void Calculate_Volume_and_update_Conexant(unsigned char Volume_to_buttons);



/*
// Group of letter linked to each button 
const unsigned char Char_Button[11][6]={
             0, 0,0,0,0,0,
             0, NUM_1,0,0,0,0,
             4, NUM_2,A_CAPITAL,B_CAPITAL,C_CAPITAL,0,
             4, NUM_3,D_CAPITAL,E_CAPITAL,F_CAPITAL,0,
             4, NUM_4,G_CAPITAL,H_CAPITAL,I_CAPITAL,0,
             4, NUM_5,J_CAPITAL,K_CAPITAL,L_CAPITAL,0,
             4, NUM_6,M_CAPITAL,N_CAPITAL,O_CAPITAL,0,
             5, NUM_7,P_CAPITAL,Q_CAPITAL,R_CAPITAL,S_CAPITAL,
             4, NUM_8,T_CAPITAL,U_CAPITAL,V_CAPITAL,0,
             5, NUM_9,W_CAPITAL,X_CAPITAL,Y_CAPITAL,Z_CAPITAL,
             0, NUM_0,0,0,0,0             
};
*/
//----------------------------------------------------------------------
// Update video and set conexant
//----------------------------------------------------------------------
/*
void Set_Call_Volume_and_Update_Video(void){

  Calculate_Volume_and_update_Conexant(R_Volume_Call);
  

}
*/
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Update video and set conexant
//----------------------------------------------------------------------
/*
void Set_Audio_Volume_and_Update_Video(void){

  Calculate_Volume_and_update_Conexant(R_Audio_Volume);
  

}*/
//----------------------------------------------------------------------

/*===========================================================================*/
 void IO_Scan_Management( void )
/*===========================================================================*/
{

//unsigned char count, digit;

if(proc_scan & INIT_PROC)
  {
  Free(proc_scan, INIT_PROC);
  timer_proc_scan = TIME_SCAN_BUTTONS;
  button = No_key;
  Signal(proc_scan, AWAKE);     
  }

if(proc_scan & AWAKE)
  {
  Free(proc_scan, AWAKE);
  timer_proc_scan = TIME_SCAN_BUTTONS;

  RESET_WATCHDOG;

  // If a room monitor isi in progress, all button are disabled
  if(!ROOM_MONITOR_IN_PROGRESS)  
  	Buttons_scan();
	/*
  if (tim_pulse)
    {
    if(--tim_pulse == 0)
      {
      MIC_MUTE = MIC_MUTE_STBY;      
      //SPK_MUTE = SPK_MUTE_STBY;     
      }  // endif
    }*/
  }
  /*--- end AWAKE */

if(proc_scan & BUTTON_READY)
  {  //   button pressed
  Free(proc_scan, BUTTON_READY);

  Timer_Inactivity = TIME_AUTOPOWEROFF;

  // -----------------------------------------------------------------------  
  if(button & ONOFF_BUTTON)
    {
      button &= ~ONOFF_BUTTON;            // Button served
      Short_tone();
	if(JD_TX_Frame_Index == JD_Save_Frame_Index && !JD_start_wait) 
	{
		
		if(Sub_MODO_State == Alarm_Handle_Waiting)
		{
			if(PageStatus!= Alarm_handle)
			{
				PageStatus = Alarm_handle;
				LoadJD_Menu(Alarm_handle);
			}
			LoadJD_SleepMenu(ON);					
		}
		else
		{
		      if(SwitchedOnOff == 1)
		        { 
		          // If needed I send a free line that switch off at tx completed 
				if(MODO_State != IDLE){
					Free_Line();
				}
				else if(Sub_MODO_State != EX_IDLE)
				{
					Sub_MODO_State = EX_IDLE;
					OLD_MODO_State = 0xff;
				}
				// otherwise normal switch off
				else{
					hook_state_toggled = ON_HOOK;
					SettoWaitPage();
					Switch_Off();
					LoadJD_Menu(PageStatus);
				}
		        }
		      else  
		        { 
			        Video(VIDEO_ON);
			        System_Video = RELE_VIDEO_OFF;
			    
			        
			        SwitchedOnOff = 1;
				
				PageStatus = MainMenu;
				LoadJD_SleepMenu(ON);
				LoadJD_Menu(MainMenu);	          				
				
		        }  // endif
		}
	}
	else
		Dissuasion_Tone();
    }  // endif
    
  // -----------------------------------------------------------------------  
  /* comment due to no long press button again
  if(button & ONOFF_BUTTON_5S)
    {
       button &= ~ONOFF_BUTTON_5S;            // Button served

    }  // endif
    
   */ 
    

  // -----------------------------------------------------------------------  
  if(button & PANIC_BATT_OK)
    {
      Short_tone();
      button &= ~PANIC_BATT_OK;      // Button served
      Battery_State = 0x00;          // 0 means full
			Gest_Panic();
    }  // endif

  if(button & PANIC_BATT_LOW)
    {
      Short_tone();
      button &= ~PANIC_BATT_LOW;      // Button served
      Battery_State = 0x80;           // 0x80 means flat
      Gest_Panic();
    }  // endif
  

  }

/*
if(proc_scan & BUTTON_READY_2S)
  {  //   button pressed for 2 seconds
  Free(proc_scan, BUTTON_READY_2S);

  }
*/
	// ---------- hook management  ------------------------
	if(proc_scan & HOOK_VARIATION){
		Free(proc_scan, HOOK_VARIATION);
    if(hook_state_toggled == OFF_HOOK){
  		if(MODO_State == IDLE)
        hook_timer_idle = AUTOMATIC_ONHOOK_TIMEOUT; 

  		if (((timer_ring_delay) || (kind_of_ring_save)) && (call_from == CALL_FROM_FLOOR )){
        p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
        p_der_queue_tx_appl->n_byte                     = 3;
				p_der_queue_tx_appl->data[0]                    = D_TONE;
        p_der_queue_tx_appl->data[1]                    = User + INTERNAL_BROADCAST;	
        p_der_queue_tx_appl->data[2]                    = NO_TONE;
				Incr_index_queue_in_der();                     // update index queue
			}

  		if ((timer_ring_delay) || (kind_of_ring_save))
        End_tone();
			
			if(	(MODO_State == VIDEO_OFF_HOOK_WAIT) ||
				(MODO_State == ANSWERINGMACHINE_OFF_HOOK_WAIT)   ||
				(MODO_State == INTERCOM_OFF_HOOK_WAIT)  ||
				(MODO_State == AUDIO_ONLY_OFF_HOOK_WAIT) ){
				
				p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte                     = 3;
				p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
				p_der_queue_tx_appl->data[1]                    = User + Internal;
				p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 
				Incr_index_queue_in_der();                      // update index queue
				
			}

			if(MODO_State == SELFINSERTION_OFF_HOOK_WAIT){
			
				p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte                     = 3;
				p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
				p_der_queue_tx_appl->data[1]                    = User + Internal;

				if(kind_of_my_communication == FROM_S_I_IP_CM_NO_AUDIO)
				  kind_of_my_communication = FROM_S_I_CM_IP_AUDIO_MONODIR;
				                           
				p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 
				Incr_index_queue_in_der();                      // update index queue
		
			}

			if(MODO_State == IN_COMMUNICATION_SELFINSERTION){  
			
				
        // If I'm already in bidirectional audio, close the comm
				if(kind_of_my_communication == FROM_S_I_CM_IP_AUDIO_BIDIR){

					Free_Line();
		    }

		    // If I'm in monodir, I go in bidir and emit another offhook
				if(kind_of_my_communication == FROM_S_I_CM_IP_AUDIO_MONODIR){
				  kind_of_my_communication = FROM_S_I_CM_IP_AUDIO_BIDIR;

          p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];                
  				p_der_queue_tx_appl->n_byte                     = 3;
  				p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
  				p_der_queue_tx_appl->data[1]                    = User + Internal;
  				p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 
  				Incr_index_queue_in_der();                      // update index queue
		    }
		    
			}
		}
		else{
      hook_timer_idle = 0; 

			if(((kind_of_my_communication) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
				if(	(MODO_State == IN_COMMUNICATION_VIDEO) ||
					(MODO_State == IN_COMMUNICATION_ANSWERINGMACHINE)   ||
					(MODO_State == IN_COMMUNICATION_SELFINSERTION)  ||
					(MODO_State == INTERCOM_OFF_HOOK_WAIT)  ||
					(MODO_State == IN_COMMUNICATION_AUDIO_ONLY) ){
				
					
					Free_Line();
					}
			}
		}

		if(MODO_State == IDLE)
      Short_tone();

	}

	
} /*---end IO_Scan_Management */

/*===========================================================================*/
//  000 0000 0000 bit0 
// bit0  ---- palla
// bit1  ---- rombo
// bit2  ---- quadrato
// bit3  ---- opendoor
//
// bit4  ---- T1
// bit5  ---- T2
// bit6  ---- T3
// bit7  ---- T4
//
// bit8  ---- T5
// bit9  ---- T7
// bit10 ---- dentist
// bit11 ---- PANIC with batt. carica (o button a mano) 

// bit12 ---- PANIC with batt. Scarica 

/*===========================================================================*/
void Buttons_scan( void )
{


	// ---------- read dip switches ------------- 
  // read dip-switch
  //READ FROM 74HC165
	if(!Read74HC165_Time)
	{
		dip_sw = Read_HC165(); //DB 16gen08
 		Read74HC165_Time = 5;
		//--------------------------------------------
		
		if (TEST_Phase_Active)
		{						//VRVR
			if(Old_Value != dip_sw)
			{
				Old_Value = dip_sw;
				// Emit a beep 
				Short_tone();
	     	// Repeat the signal on the S+ and S- contacts
	      			RPCH = RPCH_ON;
			}
		}
		
		// ------------------ Management SWITCHES without debounce ----------
		User = 0;
	  	User = (dip_sw & 0x03) << 5;  
	  	User &= 0x60;
		
		Internal = (dip_sw & 0x3c) >> 2;

	}
   
                                                       
    // This pin is shared with automatic opendoor signalling, so 
    // it is always in output and it is turned in input only when readed 
    //check the IO for panic button
    //PTGD |= 0x08;    
    //PTGDD = 0x67;     // 1= output
    //code = PTGD;
    //PTGDD = 0x6F;     // 1= output   
    
    // Restore real condition
   //PANIC_D = 0;
    
    // --------- Management of burst. Remote command battery state ...

		switch(state_door_opennic){
		
		  case RELEASED:
				if (!PANIC){             // pressed
				  edges_counter = 1;
					state_door_opennic = PIN_LOW_PHASE;
  				burst_lenght = 0;
				}
		  break;
		
		  case PIN_LOW_PHASE:
				burst_lenght++;
				if (PANIC){            
				  edges_counter++;
					state_door_opennic = PIN_HIGH_PHASE;
				}
				// Timeout 
				if(burst_lenght >= TOO_LONG){
				  state_door_opennic = RELEASED;   
  			}  				
  			
		  break;
		
		  case PIN_HIGH_PHASE:
				burst_lenght++;
				if (!PANIC){             
				  edges_counter++;
					state_door_opennic = PIN_LOW_PHASE;
				}
				
				if(edges_counter == 16){              
				  if((burst_lenght >= MIN_SEQUENCE_TIME) &&
				     (burst_lenght <= MAX_SEQUENCE_TIME)){
					  state_door_opennic = RELEASED;            // Right sequence (time and edge number)
				          button |= PANIC_BATT_LOW;      
				          Signal(proc_scan, BUTTON_READY);
					}
					else{
					  state_door_opennic = RELEASED;   
					}
				}

				if(burst_lenght >= TOO_LONG){
					  state_door_opennic = RELEASED;   
	  			}
				
		  break;
		
		  default:
		  break;
		
		}

		
		
		//--- Normal Management (full battery or HW button)
		
		switch(open_door_button){
		
		  case RELEASED:
				if (!PANIC){                 
					open_door_button = BUTTON_DEBOUNCE;
  				button_lasting = 0;
				}
		  break;
		
		  case BUTTON_DEBOUNCE:
				if (PANIC){              
					open_door_button = RELEASED;            
				}
				else{
  				if(++button_lasting >= PANIC_PRESS_TIME){
					  open_door_button = RELEASE_WAIT;       
	          button |= PANIC_BATT_OK;      
	          Signal(proc_scan, BUTTON_READY);
					}
				}
		  break;
		
		  case RELEASE_WAIT:
				if (PANIC){             
					open_door_button = RELEASED;
				}
				
		  break;
		
		  default:
		  break;
		
		}

	    // Used also during calibration
	    Management_Button_for_LCD();		

	//PANIC_D = 1;

   
    
}/*--- end Buttons_scan() */


//----------------------------------------------------------------------
//  Clears input junk data

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//  Switch off the monitor 
//----------------------------------------------------------------------
void Switch_Off(void){
	Video(VIDEO_OFF);
	System_Video = RELE_VIDEO_OFF;

	MODO_State = IDLE;
	SwitchedOnOff = 0;

	button = 0;
	//Timer_wait_call_from_CEN = 0;
	Main_GUI_locked = 0;
	Call_GUI_locked = 0;
	/*
	if(SystemStatus.bits.CallSwitchBoard_Icon)
	{
		SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
	}
	*/
	Timer_GUI_locking = 0;
	Timer_SHSTC = 0;
  
}
//----------------------------------------------------------------------



//----------------------------------------------------------------------

//----------------------------------------------------------------------
//  Used to roll on next character of that button 

//----------------------------------------------------------------------

//----------------------------------------------------------------------
//   special management for last character of the buffer 
//----------------------------------------------------------------------

//----------------------------------------------------------------------


//----------------------------------------------------------------------


//----------------------------------------------------------------------



//----------------------------------------------------------------------
// Update melodies bars 
//----------------------------------------------------------------------

//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Update screen and play of selected melody
//----------------------------------------------------------------------
void Play_New_Melody(unsigned char kind){
 
	// Faccio il play      
	if(kind == AUDIO_ONLY_CALL)
		kind_of_ring = R_kind_of_ringtone_cito;
	else if(kind == PSTN_CALL)
		kind_of_ring = R_kind_of_ringtone_PSTN;
	else 
		kind_of_ring = R_kind_of_ringtone_floor;
  
	call_from = CALL_FROM_MAIN;     
	Signal(tone_proc, RING_REQUEST);
}
//----------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  Management of LCD button
//-----------------------------------------------------------------------------
void Management_Button_for_LCD(void){

	//--- Management of button P_LCD_ON
	switch(switch_on_button){
	
	  case RELEASED:
			if (P_LCD_ON != 0){                // pressed
				switch_on_button = BUTTON_DEBOUNCE;
				button_lasting_lcd = 0;
			}
	  break;
	
	  case BUTTON_DEBOUNCE:
			if (P_LCD_ON == 0){               
				switch_on_button = RELEASED;       // spike
			}
			else
			{
				if(++button_lasting_lcd >= (ONOFF_PRESS_TIME - 1))
				{
					switch_on_button = RELEASE_WAIT;       
				          Signal(proc_scan, BUTTON_READY);
				          button |= ONOFF_BUTTON;      
				}
			}
	  break;
	
	  case RELEASE_WAIT:
			if (P_LCD_ON == 0){             
				switch_on_button = RELEASED;
			}
 	  break;
 	  
 	  case WAIT_RELEAS_FROM_LONG_PRESS:
			if (P_LCD_ON == 0){             
				switch_on_button = RELEASED;
      			}
 	  break;
	
	  default:
	  break;
	
	}
}
//-----------------------------------------------------------------------------





