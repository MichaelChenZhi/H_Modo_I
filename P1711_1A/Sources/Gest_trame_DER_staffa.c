

#include "FlashWrite.h"
//#include "Touch.h"


#pragma DATA_SEG FAR_RAM
extern void LoadJD_Menu(uchar subitemsel);
extern void AlarmFrame_Format(void);
extern void SetButton_State( uchar data1, uchar data2,uchar data3,uchar data4);
extern void SetSwitchIcon(uchar subitemsel,uchar data1, uchar data2,uchar data3,uchar data4,uchar data5);
extern void Switch_Off(void);
extern void SaveInfQueue(void (*finProc)());
extern void SaveAudioPara(void);
extern void SetControl_State( uchar data1, uchar data2,uchar data3);
extern unsigned char 	*pointer_data_der_rx_appl;
extern queue_tx_der 	*p_der_queue_tx_appl;
extern queue_tx_der 	der_queue_tx_appl[];


#pragma DATA_SEG DEFAULT


void Frame_AS_Management(void){

//  unsigned char temp;
  
	// Analisys of Frames
	switch(pointer_data_der_rx_appl[D_OPC]){

		case D_CALL:
// 			// NEW: in order to manage the 50 or 60 Hz video signal, has been added the info on BIT7 of the frame 
// 			// bit set means NTSC, bit equal 0 means PAL
//       F_Standard_Video = pointer_data_der_rx_appl[D_DATA_1] & NTSC_PAL_MASK; 
//       if(F_Standard_Video & VIDEO_NTSC)
//         ROW_of_Hook_button = ROW_NTSC_BUTTON_HOOK;

			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       		// For my apartment
				if((((pointer_data_der_rx_appl[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) == Internal) &&
            (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) != INTERNAL_BROADCAST)){   // For my internal (exactly me)
					
          // Unlock GUI 
          Main_GUI_locked = 0;
          Call_GUI_locked = 0;

					// Emit immediately a D_PRESENCE
          p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
          p_der_queue_tx_appl->n_byte                     = 4;
					p_der_queue_tx_appl->data[D_OPC] = D_PRESENCE;
					p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
					p_der_queue_tx_appl->data[D_DATA_1] = pointer_data_der_rx_appl[D_DATA_1]; 
					p_der_queue_tx_appl->data[D_DATA_2] = pointer_data_der_rx_appl[D_ADDRESS]; 
					if(Internal == MASTER_APARTMENT_STATION)        // If I'm master 
					  p_der_queue_tx_appl->data[D_DATA_1] |= Button_for_dentist + R_presence_absence; 
          
          Incr_index_queue_in_der();                      // update index queue
					
					timer_ring_delay = 0;

          // IF the call is AUDIO ONLY, set the BIPBIP at the end of the ringtone
					if(!((pointer_data_der_rx_appl[D_DATA_1]) & VIDEO_OR_AUDIO_COMM_TYPE)){     
					  if(((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK) != FROM_SWITCHBOARD_FOR_ROOM_MONITOR) // PMPM if Room monitor no sound must be performed
						  BIPBIP_audioonly_call = ON;	
          }

					// Check if I have to ring; if yes, ring 'immediately'
					switch ((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK){

						case FROM_SECONDARY_CM:
								if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
  		        call_from = CALL_FROM_SECONDARY;
	            kind_of_ring = F_kind_of_ringtone_audioonly;
							timer_ring_delay = DELAY_1;
									}
						break;

						case FROM_MAIN_CM:
							if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
  		        call_from = CALL_FROM_MAIN;
	            kind_of_ring = F_kind_of_ringtone_audioonly;
							timer_ring_delay = DELAY_1;  
								}
						break;

						case FROM_SWITCHBOARD:
              if(!Timer_SHSTC){                      // If it is a switchboard re-call
              	if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
    		        call_from = CALL_FROM_SWITCHBOARD;
  	            kind_of_ring = F_kind_of_ringtone_audioonly;
  							timer_ring_delay = DELAY_1;
              		}
              }
						break;

						case FROM_IP_PHONE:
							if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
  		        call_from = CALL_FROM_SWITCHBOARD;
	            kind_of_ring = F_kind_of_ringtone_audioonly;
							timer_ring_delay = DELAY_1;
								}
						break;

						case FROM_VILLA_CM:
							if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
  		        call_from = CALL_FROM_SECONDARY;
	            kind_of_ring = F_kind_of_ringtone_audioonly;
							timer_ring_delay = DELAY_1;
								}
						break;

						case FROM_SWITCHBOARD_FOR_ROOM_MONITOR:
							// non emetto alcun sound
						break;
			
						case FROM_THE_INQUIRED_ANSWERING_MACHINE:
						case FROM_S_I_IP_CM_NO_AUDIO:
						case FROM_S_I_VILLA_CM_NO_AUDIO:
						break;
			
						default:
						break;
		
					}
			
				}
				else{
					if(((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST){   // For all the apartment
						
						if(Internal == MASTER_APARTMENT_STATION){        // If I'm master I have to emit the presence frame
					
              // Unlock GUI che avevo bloccato in wait of risposta by CEN
              Main_GUI_locked = 0;
              Call_GUI_locked = 0;

              p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
              p_der_queue_tx_appl->n_byte                     = 4;
							p_der_queue_tx_appl->data[D_OPC] = D_PRESENCE;
							p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
							p_der_queue_tx_appl->data[D_DATA_1] = pointer_data_der_rx_appl[D_DATA_1] | Button_for_dentist | R_presence_absence;  
							p_der_queue_tx_appl->data[D_DATA_2] = pointer_data_der_rx_appl[D_ADDRESS]; 
              Incr_index_queue_in_der();                      // update index queue

						}
						else{	// If I'm slave I go in idle 
              //current_menu = NULL;
							MODO_State = IDLE;
						}
						
						if(hook_state_toggled == ON_HOOK){   // If I'm not in offhook 
							timer_ring_delay = 0;

              // IF the call is AUDIO ONLY, set the BIPBIP at the end of the ringtone
							if(!((pointer_data_der_rx_appl[D_DATA_1]) & VIDEO_OR_AUDIO_COMM_TYPE))     
							  BIPBIP_audioonly_call = ON;	

							// Check: if I have to ring
							switch ((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK){

								case FROM_SECONDARY_CM:
									if(!(tone_proc & RING_REQUEST))
									{
										End_tone(); 
							    		        call_from = CALL_FROM_SECONDARY;
									        kind_of_ring = F_kind_of_ringtone_audioonly;
									        Set_Delay_for_Internal();
									}
								break;
	
								case FROM_MAIN_CM:
									//SRD 20111123
									if(!(tone_proc & RING_REQUEST))
										{
										End_tone();  // here the Dissuasion_Tone is emiting, we have to end it.
	    		        call_from = CALL_FROM_MAIN;
			            kind_of_ring = F_kind_of_ringtone_audioonly;
			            Set_Delay_for_Internal();
										}
								break;
	
								case FROM_SWITCHBOARD:
                  						if(!Timer_SHSTC){   
				  					if(!(tone_proc & RING_REQUEST))
									{
										End_tone(); 
	      		       						 call_from = CALL_FROM_SWITCHBOARD;
			              					kind_of_ring = F_kind_of_ringtone_audioonly;
			             						 Set_Delay_for_Internal();
				  					}
								  }
                break;
	
								case FROM_EXTERNAL_AS:
									if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
	    		        call_from = CALL_FROM_INTERCOM;
			            kind_of_ring = F_kind_of_ringtone_audioonly;
			            Set_Delay_for_Internal();
										}
								break;
	
								case FROM_INTERNAL_AS:
									if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
	    		        call_from = CALL_FROM_INTERCOM;
			            kind_of_ring = F_kind_of_ringtone_audioonly;
			            Set_Delay_for_Internal();
										}
								break;
	
								case FROM_IP_PHONE:
									if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
	    		        call_from = CALL_FROM_SWITCHBOARD;
			            kind_of_ring = F_kind_of_ringtone_audioonly;
			            Set_Delay_for_Internal();
										}
								break;
	
								case FROM_VILLA_CM:
									if(!(tone_proc & RING_REQUEST))
										{
										End_tone(); 
	    		        call_from = CALL_FROM_SECONDARY;
			            kind_of_ring = F_kind_of_ringtone_audioonly;
			            Set_Delay_for_Internal();
										}
								break;
			
				
								case FROM_THE_INQUIRED_ANSWERING_MACHINE:
								case FROM_S_I_IP_CM_NO_AUDIO:
								case FROM_S_I_VILLA_CM_NO_AUDIO:
								case FROM_SWITCHBOARD_FOR_ROOM_MONITOR:
								break;
				
								default:
								break;
			
							}
						}
						else{
              // Emit only courtesy tone (when timer elapsed)
              timer_ring_delay = 1;
						}
					}
					else{	// It is for a twin of mine
            //current_menu = NULL;
						MODO_State = IDLE;
					}
			
				}
			
			}
			else{		// It is a call for another apartment
				if (MODO_State != IDLE){		
					if ((kind_of_my_communication == FROM_INTERNAL_AS) ||  // If I'm speaking inside my apartment
						(((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK) == FROM_INTERNAL_AS)){		// if the call is for another apartment
						// nothing
					}
					else{
            //current_menu = NULL;
						MODO_State = IDLE;
					}
				}
			}
			
		break;

		case D_PRESENCE:
			
//			timer_pre_occupato_D = 0;
			
			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       		// From a twin?
				
				MODO_State = IDLE;	

  			if(((pointer_data_der_rx_appl[D_DATA_2]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST){       		

					switch ((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK){
						case FROM_MAIN_CM:
						case FROM_SECONDARY_CM:
						case FROM_SWITCHBOARD:
						case FROM_EXTERNAL_AS:
						case FROM_IP_PHONE:
						case FROM_VILLA_CM:
							// Only in this case, receive a presence is relevant for me		
							timer_delayed_event = DELAY_EVENT;
							kind_of_my_communication = pointer_data_der_rx_appl[D_DATA_1] & SOURCE_MASK;
							ST_p_ret_val = memcpy(Frame_copy, pointer_data_der_rx_appl, sizeof(Frame_copy));
						break;
				
						default:
						break;
					}
				}
			}
			else{ // by another apartment: check if I have to leave 
				if (MODO_State != IDLE){		
					if ((kind_of_my_communication == FROM_INTERNAL_AS) || 
						(((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK) == FROM_INTERNAL_AS)){	
						// nothing
					}
					else{
            //current_menu = NULL;
						MODO_State = IDLE;
					}
				}
			}
				
		break;
		
		case D_HOOK_OFF:
      // I have to check if this frame is emitted by a decoder that is forwarding a column frame
      if(pointer_data_der_rx_appl[D_ADDRESS] == DEC_VILLA_CODE){
        if (MODO_State == INTERCOM_OFF_HOOK_WAIT){
          if(hook_state_toggled == OFF_HOOK){ 
            MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
            timer_wait_offhook_audio = COMMUNICATION_TIMER;
          }
        }      
      }
      // or from bracket
      else{

  			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       	
					if (((MODO_State == INTERCOM_OFF_HOOK_WAIT) || (MODO_State == IN_COMMUNICATION_AUDIO_ONLY))&& 
					    (kind_of_my_communication == (pointer_data_der_rx_appl[D_DATA_1] & SOURCE_MASK)) &&
  						(kind_of_my_communication == FROM_INTERNAL_AS)
							)
	        {
            if(hook_state_toggled == OFF_HOOK){ 
	            MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
	            timer_wait_offhook_audio = COMMUNICATION_TIMER;
					  }
          }
	        else{
            dummy = Abort_frame_der(D_HOOK_OFF);
            
            //current_menu = NULL;
	          MODO_State = IDLE;
	          timer_wait_offhook_audio = 0;
					}
  				if (((timer_ring_delay) || (kind_of_ring_save)) && (call_from != CALL_FROM_FLOOR ))	
            End_tone();
				}
        else{  																																								// from another apartment 
					if ((MODO_State == INTERCOM_OFF_HOOK_WAIT) && 
					  (kind_of_my_communication == (pointer_data_der_rx_appl[D_DATA_1] & SOURCE_MASK)) &&
						(kind_of_my_communication != FROM_INTERNAL_AS))
          {
            if(hook_state_toggled == OFF_HOOK){ // solo se il hook è on
              MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
              timer_wait_offhook_audio = COMMUNICATION_TIMER;
	          }
          }
// 	        else{
// 	          MODO_State = IDLE;
// 	          timer_wait_offhook_audio = 0;
// 					}
				}
      }

		break;
		
		case D_FREE_LINE:
			
			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       		// If from/for my apartment   			
				// This dissuasion is to say the user that its request has been refused 

        if(MODO_State != IDLE){   
                                 
//          ST_Audio = ON_HOOK_0;                 // switch off  the microphone
          Audio(FONIA_OFF);
//          timer_audio_poweron = TIME_IMP_AUDIO8;
          // switch off video                       //V1.1
          Video(VIDEO_OFF);                     //V1.1
         	System_Video = RELE_VIDEO_OFF;       //V1.1
  
  
          if((MODO_State == IN_COMMUNICATION_AUDIO_ONLY) && (!ROOM_MONITOR_IN_PROGRESS))
            Dissuasion_Tone();
          timer_delayed_event = 0;   // V2.0 - bug: if a freeline arrives before a second from the call
                                     //        the monitor goes in idle and then in wait offhook and then stays ON. 
          //current_menu = NULL;
          MODO_State = IDLE;
          // Reset hook state
          hook_state = ON_HOOK;
          hook_state_toggled = ON_HOOK;
  				if (((timer_ring_delay) || (kind_of_ring_save)) && (call_from != CALL_FROM_FLOOR ))	
            End_tone();
          if(ROOM_MONITOR_IN_PROGRESS)
            ROOM_MONITOR_IN_PROGRESS = 0;  
    
          Switch_Off();  
			  }
      }                                                                                                             			
			else{ // by another apartment                          
				if(((kind_of_my_communication != FROM_INTERNAL_AS)) &&		  
					(((pointer_data_der_rx_appl[D_DATA_1]) & SOURCE_MASK) != FROM_INTERNAL_AS)) { 	  

          if(MODO_State != IDLE){  
                                  

            //ST_Audio = ON_HOOK_0;                 // switch off  the microphone
            Audio(FONIA_OFF);
            
//            timer_audio_poweron = TIME_IMP_AUDIO8;
            // switch off video                       //V1.1
            Video(VIDEO_OFF);                     //V1.1
            System_Video = RELE_VIDEO_OFF;       //V1.1

  
            if((MODO_State == IN_COMMUNICATION_AUDIO_ONLY) && (!ROOM_MONITOR_IN_PROGRESS))
              Dissuasion_Tone();
            //current_menu = NULL;
  					MODO_State = IDLE;						
            // Reset hook position to offhook
            hook_state = ON_HOOK;
            hook_state_toggled = ON_HOOK;
    				if (((timer_ring_delay) || (kind_of_ring_save)) && (call_from != CALL_FROM_FLOOR ))	
              End_tone();
            Switch_Off(); 
					}																					  
				}													  									  
			}                                                         		                                                    			
				                                                                                                        			
		break;
		
		case D_DOOR_OPENER_1:
			// If from another utente
			//comment by Michael due to we have no LED in H-Modo
			/*
			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) != User){
				Door_State = OFF; // Switch off led
  		  Update_Signalling_LEDs();
			}
			*/
			// nothing
		break;
		
		case D_DOOR_OPENER_2:
			// nothing
		break;
		
		case D_DOOR_STATE:
			// If open
			/*
			if(pointer_data_der_rx_appl[D_DATA_1] & DOOR_STATE){
				// If for me
				if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
			   	   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal)){
          Door_State = ON;
				}
				else{
          Door_State = OFF;
				}
			}
			// If Closed
			else{
        Door_State = OFF;
			}
		  Update_Signalling_LEDs();
		  */
		break;
		
		case D_POLLING:
			// If for me
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
			   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal)){
        p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte      = 3;
				p_der_queue_tx_appl->data[D_OPC] = D_ACK_POLLING;
				p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
				if(Internal == MASTER_APARTMENT_STATION)  // If I'm master
					p_der_queue_tx_appl->data[D_DATA_1] = pointer_data_der_rx_appl[D_DATA_1] | Button_for_dentist | R_presence_absence;     
				else
					p_der_queue_tx_appl->data[D_DATA_1] = pointer_data_der_rx_appl[D_DATA_1]; 
				
				Incr_index_queue_in_der();                      // update index queue
			}
		break;
		
		case D_ACK_POLLING:
			// nothing
		break;
		
		case D_SPECIAL_FUNCTION:
			// nothing
		break;
		
		case D_PRESENCE_ABSENCE_BUTTON:
			// nothing
		break;
		
		case D_ACK_PRESENCE_ABSENCE_BUTTON:
			// If for me
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
			   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal)){

				if(Timer_Retry_D_PresenceAbsence){
					Timer_Retry_D_PresenceAbsence = 0;
					Num_Retry_D_PresenceAbsence = 0;

				}
			}
		break;
		
		case D_ALARM:
			// nothing
		break;
		
		case D_ACK_ALARM:
			// nothing //Michael for alarm system of Hmodo
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User)){
			
				Timer_Retry_D_Alarm = 0;
				Num_Retry_D_Alarm= 0;
			}
		break;
		
		case D_PANIC:
			// nothing
		break;
		
		case D_ACK_PANIC:
			// If for me
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User)){
			
				Timer_Retry_D_Panic = 0;
				Num_Retry_D_Panic = 0;
			}
		break;
		
		case D_TONE:
			// If for me
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
			   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal) ||
			   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST)
				 ){

         // Selection of frame kind: OKI configuration or standard D_TONE
        
  			   switch(pointer_data_der_rx_appl[D_DATA_1]){
             
		             case TIPO_TONE_1:
		               Short_tone();
		             break;
		           
		             case TIPO_TONE_2:
		               Dissuasion_Tone();   
					   if(Timer_GUI_locking)
						{
							Timer_GUI_locking = 0;
							if(Main_GUI_locked)
							{
								Main_GUI_locked= 0;
			            				// Could be alpsed after a cycle request so I haven't to go to main screen
			            				if(SystemStatus.bits.CallSwitchBoard_Icon)
								{
									SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
							        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
										/*
									if(PageStatus == MainMenu_2 || PageStatus == MainMenu_3 || PageStatus == MainMenu_4)
										LoadJD_Menu(PageStatus);
										*/
								}
	          				}
	          				Call_GUI_locked = 0;
	        			}
		            	 break;
		           
		             case NO_TONE:
		               End_tone();
		             break;
		           
		             default:
		             break;
		           
		           
		         }
			}
		break;
		
		case D_FLOOR_CALL:

			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){  // For my apartment
					
				Set_Delay_for_Internal();

				Floor_call_tone();
			}      
			else{		// It is a call for another apartment and sto squillando
				if (((timer_ring_delay) || (kind_of_ring_save)) && (call_from == CALL_FROM_FLOOR )){
				  End_tone();
       			 }
			}

		break;

		case D_BOUNCE:
			// if from my apartment, switch off the monitor
			if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){       		// If from/for my apartment   			
				System_Video = RELE_VIDEO_OFF;
			}
		break;
		
		case D_CYCLE:
		break;
		
		case D_CYCLE_VILLA:
		break;
		
		case D_SVC_NEXT:
		break;
		
		case D_SVC_DELETE:
			// nothing
		break;
		
		case D_SVC_NUMBER:
			// If for me 
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
			   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal)){   
				// If in right state   			
				if (MODO_State == IN_COMMUNICATION_ANSWERINGMACHINE){
					number_SVC = pointer_data_der_rx_appl[D_DATA_1];
				}
			}
		break;
		
		case D_MESSAGES_PRESENT:
			// If for me
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) && (Internal == MASTER_APARTMENT_STATION)){

				//R_messages_presents = ON;
				F_messages_present = ON;
				SaveInfQueue(&SaveAudioPara);				
				if(F_messages_present)
				{						
					SetControl_State(MainMenu, 6, ON);
						 //send iic command set mute light on						 
				}			
			}
		break;
		
	
		case D_MESSAGES_ABSENT:
			// If for me
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User)){
				if(F_messages_present != OFF){
					F_messages_present = OFF;
					SetControl_State(MainMenu, 6, OFF);
					//Flag3.bits.message = OFF;
					SaveInfQueue(&SaveAudioPara);
          
				}
			}
   	break;
		
		case D_CALL_BUTTON:
		break;
		
		case D_OUT_CALL_FUNCTION:
		break;
		
		case D_IN_CALL_FUNCTION:
		break;
		
		case D_MENU:
		break;
		
		case D_MENU_RESPONSE:
		break;
		
		case D_AUDIO:

// -------------------------------------- Break throgh case: I have to evaluate if leave or not ------------------------
      if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) != User) &&     
				(((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) != User))	{       

        if((kind_of_my_communication != FROM_INTERNAL_AS) &&
           (((pointer_data_der_rx_appl[D_DATA_2]) & SOURCE_MASK) != FROM_INTERNAL_AS)){
          timer_wait_offhook_audio = 0;
          //current_menu = NULL;
          MODO_State = IDLE;
        }
      }
      else if(((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
            (((pointer_data_der_rx_appl[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) != Internal) &&
            (((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) != INTERNAL_BROADCAST)) &&
            (((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) != User)){
            timer_wait_offhook_audio = 0;
            //current_menu = NULL;
            MODO_State = IDLE;
            }
        
      else if(((((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) == User) &&
            (((pointer_data_der_rx_appl[D_DATA_1]) & A4_BIT_INTERNAL_MASK) != Internal) &&
            (((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) != INTERNAL_BROADCAST)) &&
            (((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) != User)){
            timer_wait_offhook_audio = 0;
            //current_menu = NULL;
            MODO_State = IDLE;
            }

      else if(((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
            (((pointer_data_der_rx_appl[D_ADDRESS]) & A4_BIT_INTERNAL_MASK) != Internal) &&
            (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) != INTERNAL_BROADCAST)) &&
            ((((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) == User) &&
            (((pointer_data_der_rx_appl[D_DATA_1]) & A4_BIT_INTERNAL_MASK) != Internal) &&
            (((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) != INTERNAL_BROADCAST))){        
            timer_wait_offhook_audio = 0;
            //current_menu = NULL;
            MODO_State = IDLE;
            }
            
      
      // -------------------------------------- Normal case: I haven't to leave ------------------------
      // Analizzo DATO_1 cioè quello del chiamante (ad es. staffa of altra decodifica)
      if(((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) == User)  {      // My apartment
				if(((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) == Internal)	{  // My internal (exactly me)
					
					kind_of_my_communication = (pointer_data_der_rx_appl[D_DATA_2] & SOURCE_MASK);		

          // Switch on monitor					
          Video(VIDEO_ON);

          SwitchedOnOff = 1;

                                                                             
          if(kind_of_my_communication == FROM_INTERNAL_AS){
            hook_state_toggled = OFF_HOOK;
          }

          if(hook_state_toggled == ON_HOOK){	 
						timer_ring_delay = DELAY_1;
						
						MODO_State = INTERCOM_OFF_HOOK_WAIT;
    				timer_wait_offhook_audio = COMMUNICATION_TIMER;
						
            kind_of_ring = F_kind_of_ringtone_audioonly;
            call_from = CALL_FROM_INTERCOM;
					  BIPBIP_audioonly_call = ON;	

					}
          else{
            timer_wait_offhook_audio = COMMUNICATION_TIMER;
            MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
            // Icon blink to signal the user audio active
            

//            ST_Audio = OFF_HOOK_0;
//            timer_audio_poweron = TIME_IMP_AUDIO7;

            Timer_SHSTC = 0; 
          }
                                      

//           // Test for capire se l'ho ricevuta following a a mia dbutton 
//           // In questo caso vado già in sganciato and audio
//     		  if(((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) == User)  {     
// 				    if(((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) == Internal)	{  
// //              if(Timer_SHSTC){
//                 
//                 // Blink the icon to say the user that audio is active
//                 Blink_the_Icon(HOOK_ICON_BLINK);
//   
//                 timer_wait_offhook_audio = COMMUNICATION_TIMER;
//                 MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
//   
//                 // To enable audio
//                 hook_state_toggled = OFF_HOOK;
//                 ST_Audio = OFF_HOOK_0;
//                 timer_audio_poweron = TIME_IMP_AUDIO7;
//   
//                 Timer_SHSTC = 0; // Stop the timer that will switch me off when elapsed
// //              }
//             }
//           }
				}
				
				else{
					if(((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST)	{   // Tutto my apartment
						
					}
					else{									 // caller is a twin of mine
            //current_menu = NULL;
						MODO_State = IDLE;  
					}
				}
			}			
      
		  if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User)  {      		// My apartment
				
				if(((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal)	{       // My internal (exactly me)
					
					kind_of_my_communication = (pointer_data_der_rx_appl[D_DATA_2] & SOURCE_MASK);	

          if(hook_state_toggled == ON_HOOK){	// If I'm ONHOOK 
						timer_ring_delay = DELAY_1;
						
						MODO_State = INTERCOM_OFF_HOOK_WAIT;
    				timer_wait_offhook_audio = COMMUNICATION_TIMER;
						
            kind_of_ring = F_kind_of_ringtone_audioonly;
            call_from = CALL_FROM_INTERCOM;
					  BIPBIP_audioonly_call = ON;	
					}

          // Switch on monitor					
          Video(VIDEO_ON);
          SwitchedOnOff = 1;


          // Test to understand if it is received following a mine dbutton 
          // In this case I'll go directly in audio and off hook
     		  if(((pointer_data_der_rx_appl[D_DATA_1]) & A3_BIT_USER_MASK) == User)  {     
 				    if(((pointer_data_der_rx_appl[D_DATA_1]) & A5_BIT_USER_MASK) == Internal)	{  
//              if(Timer_SHSTC){
                // Blink the icon to say the user that audio is active
              
  
                timer_wait_offhook_audio = COMMUNICATION_TIMER;
                MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
  
                // To enable audio
                hook_state_toggled = OFF_HOOK;
//                ST_Audio = OFF_HOOK_0;
//                timer_audio_poweron = TIME_IMP_AUDIO7;
    
                Timer_SHSTC = 0; // Stop the timer that will switch me off when elapsed
//              }
            }
          }
				}
				else{
					if(((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == INTERNAL_BROADCAST)	{   // All the apartment
						
						MODO_State = INTERCOM_OFF_HOOK_WAIT;
            kind_of_ring = F_kind_of_ringtone_audioonly;
            call_from = CALL_FROM_INTERCOM;
						
						kind_of_my_communication = (pointer_data_der_rx_appl[D_DATA_2] & SOURCE_MASK);	
            
						if(hook_state_toggled == ON_HOOK){   // If I'm ONHOOK 
							timer_ring_delay = 0;
              kind_of_ring = F_kind_of_ringtone_audioonly;
              call_from = CALL_FROM_INTERCOM;

							Set_Delay_for_Internal();
						  BIPBIP_audioonly_call = ON;	
  					}
						else{  // If already offhook
						
              if(((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User){ 
//                 // Emit a offhook
//                 p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
//                 p_der_queue_tx_appl->n_byte                     = 3;
//                 p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
//                 p_der_queue_tx_appl->data[1]                    = User + Internal;
//                 p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 	   
//                 Incr_index_queue_in_der();                      // update index queue
//                 Incr_index_queue_in_der();

								// Emit later the offhook to avoid frame collision
								timer_delay_offhook = Internal + 1;
                timer_wait_offhook_audio = COMMUNICATION_TIMER;

                // Emit only courtesy tone (when timer elapsed)
                timer_ring_delay = 1;
              }
            }

            // Switch on monitor					
            Video(VIDEO_ON);
            SwitchedOnOff = 1;
            
  
					}
					else{	// It is for a twin of mine
						if((pointer_data_der_rx_appl[D_DATA_2] & SOURCE_MASK) != FROM_INTERNAL_AS){	
             // current_menu = NULL;
						  MODO_State = IDLE;
						//else
						  // Nothing
						}  
					}
				}
			}

    break;

    case D_TEST_PHASE_ACTIVATION:
      if((pointer_data_der_rx_appl[D_ADDRESS] == 0xA5) &&
        (pointer_data_der_rx_appl[D_DATA_1] == 0x5A) &&
        (pointer_data_der_rx_appl[D_DATA_2] == 0xA5)){

        // Set or confirm the flag
        TEST_Phase_Active = 1; //enable TEST mode 

        // Data analisys
        switch(pointer_data_der_rx_appl[D_DATA_3]){

        	case PRODUCTION_TEST_ACTIVATION:
		
            p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];   //1.1

            // I have to perform the maxim checksum and send the result to the test equipment  
            //if(CheckSum_The_Maxim_Memory()){
              // Set the positive answer 
  						p_der_queue_tx_appl->data[D_DATA_3] = CKS_MAXIM_OK; 
            //}
            //else{                                   
              // Set the negative answer 
  		//				p_der_queue_tx_appl->data[D_DATA_3] = CKS_MAXIM_KO; 
            //}

            //Send the frame   
//1.1            p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];  
            p_der_queue_tx_appl->n_byte                     = 5;
						p_der_queue_tx_appl->data[D_OPC] = D_TEST_PHASE_ACTIVATION;
						p_der_queue_tx_appl->data[D_ADDRESS] = 0xA5; 
						p_der_queue_tx_appl->data[D_DATA_1] = 0x5A; 
						p_der_queue_tx_appl->data[D_DATA_2] = 0xA5; 
            Incr_index_queue_in_der();       
           
        	break;
			       
        	case TS_CALIBRATION:
            // Switch on all the leds

            // To drive on the fourth led (shared with PANIC button port) I have to reprogram the port direction
            LED_LOGO = LED_ON;
 		LoadJD_Menu(0);

       	   
			
        	break;
        	/*
        	case TS_CALIBRATION_OK:
		case TS_CALIBRATION_KO:
			TEST_Phase_Active = 0;
		break;
        	case POTENTIOMETER_TEST_START:
           // Pot_Sequence_State = POT_STEP_1; //we used videostar module
          break;
        */
        	//case MIC_DISABLE:
//            ST_Audio = MIC_IN_MUTE_0;                 // switch off  the microphone
//            timer_audio_poweron = TIME_IMP_AUDIO8;
              //Mic_State(OFF);
        	//break;
        
        	default:
        	break;
        }
      
      }
    break;

    case D_FW_REQ:
			if((((pointer_data_der_rx_appl[D_ADDRESS]) & A3_BIT_USER_MASK) == User) &&
			   (((pointer_data_der_rx_appl[D_ADDRESS]) & A5_BIT_USER_MASK) == Internal)){
                                     
        p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte      = 3;
				p_der_queue_tx_appl->data[D_OPC] = D_FW_RESPONSE;
				p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
				p_der_queue_tx_appl->data[D_DATA_1] = FW_VERSION;
        Incr_index_queue_in_der();
      }
    break;
		

	
		default:
		break;
	}

}

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
void	Gest_ST_TXCompleted(void){
	
		switch(buffer_tx_der[D_OPC]){
	
			case D_PRESENCE:
				timer_delayed_event = DELAY_EVENT;
				kind_of_my_communication = buffer_tx_der[D_DATA_1] & SOURCE_MASK;
				ST_p_ret_val = memcpy(Frame_copy, buffer_tx_der, sizeof(Frame_copy));
			break;
		
 			case D_HOOK_OFF:
				timer_wait_offhook_audio = COMMUNICATION_TIMER;
				
				if(kind_of_my_communication == FROM_SWITCHBOARD_FOR_ROOM_MONITOR){
					timer_wait_offhook_audio = ENDLESS;
          ROOM_MONITOR_IN_PROGRESS = 1;  
		  //--Michael Add for panic button press from main menu, this is the new feature which is not same with 1717.1
		  PageStatus = IDLE;
		 Video(VIDEO_OFF);
  		System_Video = RELE_VIDEO_OFF;
		//------------------------------
				}
				
				switch(MODO_State){
			
					case AUDIO_ONLY_OFF_HOOK_WAIT:
					case INTERCOM_OFF_HOOK_WAIT:
						MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
						ICON = 1;
					break;
			
					case VIDEO_OFF_HOOK_WAIT:
						MODO_State = IN_COMMUNICATION_VIDEO;
						ICON = 1;
					break;
			
					case SELFINSERTION_OFF_HOOK_WAIT:
						MODO_State = IN_COMMUNICATION_SELFINSERTION;
						ICON = 2;
			      // Display arrow monodirectional   
            					SetSwitchIcon(ChangeButton_Icon, Talk_To_CallModule_NotHide, K_AnswerCall, 0, 0x02, 0x28 + ICON);	
					break;
			
					case ANSWERINGMACHINE_OFF_HOOK_WAIT:
						MODO_State = IN_COMMUNICATION_ANSWERINGMACHINE;
						ICON = 1;
					break;
						
					case IN_COMMUNICATION_SELFINSERTION:
						MODO_State = IN_COMMUNICATION_SELFINSERTION;
						OLD_MODO_State = IDLE;
						ICON = 3;
			      // Display arrow bidirectional
            					SetSwitchIcon(ChangeButton_Icon, Talk_To_CallModule_NotHide, K_AnswerCall, 0, 0x02, 0x28 + ICON);
					break;
			
					default:
					break;
				}

        // Update the icon an activate the microphone
        hook_state_toggled = OFF_HOOK;
//        ST_Audio = OFF_HOOK_0;
//        timer_audio_poweron = TIME_IMP_AUDIO7;


// 				if((kind_of_my_communication) & VIDEO_OR_AUDIO_COMM_TYPE){
// 					// Switch on monitor					
// 					// --- migro nello state futuro
// 					switch(kind_of_my_communication){
// 
// 						case FROM_SECONDARY_CM:
// 						case FROM_MAIN_CM:
// 						case FROM_SWITCHBOARD:
// 						case FROM_IP_PHONE:
// 						case FROM_VILLA_CM:
// 							MODO_State = IN_COMMUNICATION_VIDEO;
// 						break;	
// 
// 						case FROM_THE_INQUIRED_ANSWERING_MACHINE:
// 							MODO_State = IN_COMMUNICATION_ANSWERINGMACHINE;
// 						break;	
// 
// 						case FROM_S_I_CM_IP_AUDIO_MONODIR:
// 						case FROM_S_I_CM_IP_AUDIO_BIDIR:
// 						case FROM_S_I_CM_VILLA_AUDIO_MONODIR:
// 						case FROM_S_I_CM_VILLA_AUDIO_BIDIR:
// 							MODO_State = IN_COMMUNICATION_SELFINSERTION;
// 						break;	
// 
// 						default:
// 						break;
// 					}
// 				}
// 				else{  // E' sicuramente CITO
// 
// 					MODO_State = IN_COMMUNICATION_AUDIO_ONLY;
// 				}
			break;
		
			case D_FREE_LINE:
        // Reset hook position to offhook
        hook_state = ON_HOOK;
        hook_state_toggled = ON_HOOK;

				timer_wait_offhook_audio = 0;
//         if(MODO_State != IN_COMMUNICATION_ANSWERINGMACHINE)  
//           current_menu = NULL;

        Switch_Off();
          
			break;
		
			case D_PANIC:
				
			break;

			case D_PRESENCE_ABSENCE_BUTTON:
			break;

			case D_BOUNCE:
        
        
        			System_Video = RELE_VIDEO_ON;

			break;
		
			case D_MENU:
			break;
		
			case D_MENU_RESPONSE:
			break;
		
			case D_AUDIO:
			break;
		
  		case D_OUT_CALL_FUNCTION:
  		  // Setup the data to avoid ring when I'll receive the D_AUDIO frame
        // Check if a switchboard has been called
        if(
        (buffer_tx_der[D_DATA_1] == 0x80) &&               //Code switchboard 0x80 30 30 30
 //v1.1       (buffer_tx_der[D_DATA_2] == 0x43) && 
        (buffer_tx_der[D_DATA_2] == 0x30) && 
        (buffer_tx_der[D_DATA_3] == 0x30)) 
        {
          // Keep the screen for 10 seconds 
          // (when timer is elapsed, I'll go in idle and the swb call will be managed normally
          Timer_SHSTC = WAIT_OF_CALL_TO_SWITCHBOARD_IS_STORED;
        }
        else{
    		  // Setup the data to avoid ring when I'll receive the D_AUDIO frame
          hook_state_toggled = OFF_HOOK;
        }


      break;
  		
  		case D_IN_CALL_FUNCTION:
  		  // Setup the data to avoid ring when I'll receive the D_AUDIO frame
        hook_state_toggled = OFF_HOOK; 
		  Simulate_Hook_On_Time = 3;
  		break;
		
			default:
			break;
		}
}
// ------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void Button_Selfinsertion_Cyclic(void){
	
//  unsigned int res;


  switch(MODO_State){

    case IDLE:
      p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
			p_der_queue_tx_appl->n_byte = 2;
      p_der_queue_tx_appl->data[D_OPC] = D_CYCLE;
      p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
			Incr_index_queue_in_der();                      // update index queue

      // Hourglass to say the user that the request has been issued
      
    
      Main_GUI_locked = 1;
      Timer_GUI_locking = TIME_LOCK_GUI;
    break;

    case AUDIO_ONLY_OFF_HOOK_WAIT:
      Dissuasion_Tone();
    break;

    case VIDEO_OFF_HOOK_WAIT:
      
      if (System_Video == RELE_VIDEO_ON){       // if monitor already on   
        p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte                     = 2;
        p_der_queue_tx_appl->data[D_OPC] = D_CYCLE;
        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
				Incr_index_queue_in_der();                      // update index queue
      }
      else{   // if monitor OFF
        p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
				p_der_queue_tx_appl->n_byte                     = 2;
        p_der_queue_tx_appl->data[D_OPC] = D_BOUNCE;
        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
				Incr_index_queue_in_der();                      // update index queue
      }
    break;

    case SELFINSERTION_OFF_HOOK_WAIT:
      p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
      p_der_queue_tx_appl->n_byte                     = 2;
      p_der_queue_tx_appl->data[D_OPC] = D_CYCLE;
      p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
      Incr_index_queue_in_der();                      // update index queue
      Delay_Hook_Press_Time = 20;
    break;

    case ANSWERINGMACHINE_OFF_HOOK_WAIT:
    break;

    case INTERCOM_OFF_HOOK_WAIT:
      Dissuasion_Tone();
    break;

    case IN_COMMUNICATION_AUDIO_ONLY:
      Dissuasion_Tone();
    break;

    case IN_COMMUNICATION_VIDEO:
    case IN_COMMUNICATION_SELFINSERTION:
      p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
      p_der_queue_tx_appl->n_byte                     = 2;
      p_der_queue_tx_appl->data[D_OPC] = D_CYCLE;
      p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
      Incr_index_queue_in_der();                      // update index queue
    
    break;

//     case IN_COMMUNICATION_ANSWERINGMACHINE:
//       p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
//       p_der_queue_tx_appl->n_byte                     = 2;
//       p_der_queue_tx_appl->data[D_OPC] = D_SVC_NEXT;
//       p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
//       Incr_index_queue_in_der();                      // update index queue
//     break;
  
    default:
    break;
    }

}

//------------------------------------------------------------------------------




// void T_Richiesta_SVC(void){
// 
//   switch(MODO_State){
// 
//     case IDLE:
//         p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
//         p_der_queue_tx_appl->n_byte = 2;
//         p_der_queue_tx_appl->data[D_OPC] = D_SVC_NEXT;
//         p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
//         Incr_index_queue_in_der();                      // update index queue
// 
// 
// //       if(hook_state_toggled == ON_HOOK){			 
// // 	      p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
// // 				p_der_queue_tx_appl->n_byte = 2;
// // 	      p_der_queue_tx_appl->data[D_OPC] = D_CYCLE_VILLA;
// // 	      p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
// // 				Incr_index_queue_in_der();                      // update index queue
// //       }
// 		break;
// 
//     case AUDIO_ONLY_OFF_HOOK_WAIT:
//     break;
//     
//     case VIDEO_OFF_HOOK_WAIT:
//     case SELFINSERTION_OFF_HOOK_WAIT:
// 		break;
// 
//     case ANSWERINGMACHINE_OFF_HOOK_WAIT:
//     break;
// 
//     case INTERCOM_OFF_HOOK_WAIT:
//     break;
// 
//     case IN_COMMUNICATION_AUDIO_ONLY:
//     break;
// 
//     case IN_COMMUNICATION_SELFINSERTION:
// // 			if((kind_of_my_communication == FROM_S_I_CM_IP_AUDIO_MONODIR) ||       // For evitare of mandare CICLO quando sono già in audio bidir
// // 			   (kind_of_my_communication == FROM_S_I_CM_VILLA_AUDIO_MONODIR)){
// // 				p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
// // 				p_der_queue_tx_appl->n_byte                     = 3;
// // 				p_der_queue_tx_appl->data[0]                    = D_HOOK_OFF;
// // 				p_der_queue_tx_appl->data[1]                    = User + Internal;
// // 	
// // 				if(kind_of_my_communication == FROM_S_I_CM_IP_AUDIO_MONODIR)
// // 				  kind_of_my_communication = FROM_S_I_CM_IP_AUDIO_BIDIR;
// // 				  
// // 				if(kind_of_my_communication == FROM_S_I_CM_VILLA_AUDIO_MONODIR)
// // 				  kind_of_my_communication = FROM_S_I_CM_VILLA_AUDIO_BIDIR;
// // 				  
// // 				p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 
// // 				Incr_index_queue_in_der();                      // update index queue
// // 			}
//     break;
// 
//     case IN_COMMUNICATION_ANSWERINGMACHINE:
// //       if(number_SVC){    // safety clear (non richiedere cancellazioni "strane")
// // 				p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
// // 		    p_der_queue_tx_appl->n_byte                     = 3;
// // 		    p_der_queue_tx_appl->data[D_OPC] = D_SVC_DELETE;
// // 		    p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal;
// // 		    p_der_queue_tx_appl->data[D_DATA_1] = number_SVC; 
// // 		    Incr_index_queue_in_der();                      // update index queue
// // 			}
//     break;
//   
//     default:
//     break;
//     }
// 
// }



// ------------------------------------------------------------------------------
// void Gest_Apriporta(void){
//   p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
//   p_der_queue_tx_appl->n_byte                     = 2;
//   p_der_queue_tx_appl->data[D_OPC] = D_DOOR_OPENER_1;
//   p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
//   Incr_index_queue_in_der();                      // update index queue
// }
// ------------------------------------------------------------------------------



//------------------------------------------------------------------------------
void Gest_Panic(void){

	// Set timer for re-transmission
	Timer_Retry_D_Panic = ACK_WAITING_TIMEOUT;
	Num_Retry_D_Panic = RETRANSMISSION_NUMBER;
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 3;
	p_der_queue_tx_appl->data[D_OPC] = D_PANIC;
	p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
	p_der_queue_tx_appl->data[D_DATA_1] = Battery_State;
	Incr_index_queue_in_der();                      // update index queue
}


void Gest_Alarm(void)
{
	if(Internal == MASTER_APARTMENT_STATION) 
	{
		Timer_Retry_D_Alarm= ACK_WAITING_TIMEOUT;
		Num_Retry_D_Alarm= RETRANSMISSION_NUMBER;
		/*
		p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
		p_der_queue_tx_appl->n_byte                     = 6;
		p_der_queue_tx_appl->data[D_OPC] = D_ALARM;
		p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
		p_der_queue_tx_appl->data[D_DATA_1] = Alarm_Type_To_Switchboard>>8;//
		p_der_queue_tx_appl->data[D_DATA_2] = Alarm_Type_To_Switchboard&0x00ff;//alarm flag
		p_der_queue_tx_appl->data[D_DATA_3] = 0x00;
		p_der_queue_tx_appl->data[D_DATA_4] = H_Modo_Ipervoice; //only for Hmodo
		Incr_index_queue_in_der(); 
		*/
		AlarmFrame_Format();
	}
}
void Presence_Absence_Management(void){
  
	if(F_Presence_Absence == 0){
		R_presence_absence = ABSENCE_OFF;
		
	}  
	else{ 
		R_presence_absence = ABSENCE_ON;
		
	}
	Timer_Retry_D_PresenceAbsence = ACK_WAITING_TIMEOUT;
	Num_Retry_D_PresenceAbsence = 3;

	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 3;
	p_der_queue_tx_appl->data[D_OPC] = D_PRESENCE_ABSENCE_BUTTON;
	p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
//VA.0  p_der_queue_tx_appl->data[D_DATA_1] = F_presence_absence; 
	p_der_queue_tx_appl->data[D_DATA_1] = R_presence_absence; 
	Incr_index_queue_in_der();
   //&& !TPM2SC_TOIE
   	//Flag.bits.SavePM = 1;
	//SaveInfQueue(&SavePresencewithMessage);
  
	
}

//------------------------------------------------------------------------
void Set_Delay_for_Internal (void){

	switch (Internal){

		case 0:
		case 4:
		case 8:
		case 12:
			timer_ring_delay = DELAY_1;
		break;

		case 1:
		case 5:
		case 9:
		case 13:
			timer_ring_delay = DELAY_2;
		break;

		case 2:
		case 6:
		case 10:
		case 14:
			timer_ring_delay = DELAY_3;
		break;

		case 3:
		case 7:
		case 11:
		case 15:
			timer_ring_delay = DELAY_4;
		break;

		default:
		break;
	}
}
//------------------------------------------------------------------------
void S_In_Call_Frame(uchar p)
{
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 3;
	p_der_queue_tx_appl->data[D_OPC] = D_IN_CALL_FUNCTION;
	p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
	p_der_queue_tx_appl->data[D_DATA_1] = p; 
      
      	Incr_index_queue_in_der();                      
    
}

void S_Special_Frame(uchar p)
{
	
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 3;
	p_der_queue_tx_appl->data[D_OPC] = D_SPECIAL_FUNCTION;
	p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
	p_der_queue_tx_appl->data[D_DATA_1] = p; 
	Incr_index_queue_in_der();                       

}
/*
void S_OUT_CALL_FUNCTION(uchar p1,uchar p2, uchar p3,uchar p4)
{	    
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 6;
	p_der_queue_tx_appl->data[D_OPC] = D_OUT_CALL_FUNCTION;
	p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
	 p_der_queue_tx_appl->data[D_DATA_1] = p1; 
	p_der_queue_tx_appl->data[D_DATA_2] = p2;
	p_der_queue_tx_appl->data[D_DATA_3] = p3;
	p_der_queue_tx_appl->data[D_DATA_4] = p4;
	Incr_index_queue_in_der();     	  
}
*/
//------------------------------------------------------------------------
// Implements the squence requested by the SCL1717-001A.doc (prrduction test)

//------------------------------------------------------------------------




//------------------------------------------------------------------------
// Check the content of the maxim character memory. Performed in order
// to know if the factory programming has been succesful  
//------------------------------------------------------------------------

//------------------------------------------------------------------------


