/*
*	File for the management dei pin of output del micro che gestisce 
*   il device 1707/95	- Staffa IMAGO
*
*
 *  $Id: Output.c,v 1.2 2011/09/01 08:18:59 vrzrrt Exp $
 *  $Log: Output.c,v $
 *  Revision 1.2  2011/09/01 08:18:59  vrzrrt
 *  Added OKI (Echo Canceller) support.
 *  Now audio is managed during call.
 *  Still to be added volume adjustment, mute functions, volume settings and related stuffs.
 *
 *  Revision 1.1  2011/09/01 07:45:02  chnmhl
 *  First release
 *
 *  Revision 1.1  2011/06/13 14:31:10  vrzrrt
 *  First commit. This module is only for translation purposes and will be never updated.
 *
 *  Revision 1.12  2010/02/25 08:10:13  vrzrrt
 *  Corretto baco on presence/absence
 *
 *  Revision 1.11  2010/02/01 18:01:19  vrzrrt
 *  Bug fixing
 *
 *  Revision 1.10  2009/12/04 11:41:34  vrzrrt
 *  Led management OK - verified
 *
 *  Revision 1.9  2009/12/03 15:56:08  vrzrrt
 *  WIP new led configuration
 *
 *  Revision 1.8  2009/12/01 10:08:52  vrzrrt
 *  TEST bench setting up
 *
 *  Revision 1.7  2009/10/21 15:21:27  vrzrrt
 *  Wip volumi conexant ok
 *
 *  Revision 1.6  2009/10/05 15:16:30  vrzrrt
 *  wip on blink tasti mute and audio
 *
 *  Revision 1.5  2009/09/30 15:08:59  vrzrrt
 *  WIP on nuova management touch, leds and flash
 *
 *  Revision 1.4  2009/09/29 17:01:25  vrzrrt
 *  WIP on bar aurine and flash
 *
 *  Revision 1.3  2009/09/11 15:15:17  vrzrrt
 *  WIP on variazione suonerie
 *
 *  Revision 1.2  2009/07/22 15:37:49  vrzrrt
 *  WIP - Management tasti
 *
 *  Revision 1.1  2009/05/18 10:04:44  vrzrrt
 *  Primo committ. Versione debug Villata
 *
 *  Revision 1.3  2008/12/16 09:51:28  vrzrrt
 *  Perfezionata management tasti and led
 *
 *  Revision 1.2  2008/11/14 16:31:19  vrzrrt
 *  Updatementi: no aggiuntivo tasti, management led of audio
 *
 *  Revision 1.1  2008/10/24 09:36:16  vrzrrt
 *  Creazione modulo. Derivato by staffa ipervoice WIP
 *

 *
*/

#include "define.h"
#include "global.h"
#include "MC9S08AW60.h"                /* I/O map for MC9S08AW60CFU */
#include "Flashwrite.h"             

#pragma DATA_SEG DEFAULT
// extern const unsigned char F_messages_present;
// extern const unsigned char F_presence_absence;
	 


// Activation function / deactivation video
void Video (unsigned char value){
	
	if (value == VIDEO_ON){
		//EN_VIDEO = VIDEO_ON;
    LED_LOGO = LED_ON;
	}	
	else{
		//EN_VIDEO = VIDEO_OFF;
    LED_LOGO = LED_OFF;
	}
}


	
// Activation function / deactivation audio
void Audio (unsigned char value){
	
  	if (value == FONIA_ON){
  	  
		EN_AUDIO  = FONIA_OFF;// H_Modo's Hardware used this
   		EN_Z      = FONIA_ON;
      		VolumeCNT = 2;// after 200ms set the correct volume 
        //Status_AUDIO = ON; // DB 

  	}
  	else{
  	  
		EN_AUDIO  = FONIA_ON;
		EN_Z      = FONIA_OFF;
  			
        //Status_AUDIO = OFF; // DB 
  	}
}
//-----------------------------------------------------------------------------	
// Activation function / deactivation 
/*     //comment by Michael due to no HW
void Led_PA_Mngr (unsigned char value){
	if(value == STEADY_ON){
		timer_blinking_of_door_open_led = STEADY_ON;
		phase_led_on_door_open = STEADY_ON;
		LED_OPEN_DOOR = LED_ON;
	}
	else{	
		if(value == SWITCH_OFF){
			timer_blinking_of_door_open_led = SWITCH_OFF;
			LED_OPEN_DOOR = LED_OFF;
		}
		else{
			phase_led_on_door_open = value;                               
			timer_blinking_of_door_open_led = 1; // for start the procedure
		}
	}
}
*/
//-----------------------------------------------------------------------------	
                                                        
//-----------------------------------------------------------------------------	
// Activation function / deactivation 
/*
void Led_Mess_Mngr (unsigned char value){
	if(value == STEADY_ON){
		timer_blinking_of_messages_led = STEADY_ON;
		phase_led_on_mess = STEADY_ON;
		LED_MESSAGGES = LED_ON;
	}
	else{	
		if(value == SWITCH_OFF){
			timer_blinking_of_messages_led = SWITCH_OFF;
			LED_MESSAGGES = LED_OFF;
		}
		else{
			phase_led_on_mess = value;                               
			timer_blinking_of_messages_led = 1; // for start the procedure
		}
	}
}
*/
//-----------------------------------------------------------------------------	



//-----------------------------------------------------------------------------	
// Update led accordingly to the priority of the active conditions
/*
void Update_Signalling_LEDs(void){
                                                
  //--- MANAGING FOR LED PA --------------------------
	Led_PA_Mngr(SWITCH_OFF);  // Switch off

//VA.0  if(R_presence_absence == ABSENCE_ON){
  if(F_presence_absence == ABSENCE_ON){
  	Led_PA_Mngr(OPEN_DOOR_LED_ABSENCE);  // Switch on
  }  
	if(Door_State == ON){
		Led_PA_Mngr(OPEN_DOOR_LED_MAIN);  // Switch on
	}

  //--- MANAGING FOR LED MUTE --------------------------
  if(R_Led_mute == ON)
		LED_MUTE = LED_ON; 
	else
		LED_MUTE = LED_OFF; 

  //--- MANAGING FOR AUTOMATIC OPENDOOR LED --------------
  if(Button_for_dentist == DENTIST_BUTTON_PRESSED)
		LED_AUTOMATIC_OPEN_DOOR = LED_ON; 
	else
		LED_AUTOMATIC_OPEN_DOOR = LED_OFF; 

  //--- MANAGING FOR MESSAGES PRESENT LED --------------
	Led_Mess_Mngr(SWITCH_OFF);  // Switch off

  if(R_messages_presents == ON)
		Led_Mess_Mngr(LED_MESSAGES_PRESENT); 
                     
                      
}	
	
*/	
	
void BlueToothScan(void)
{
	if(F_Old_Pres_BT != Pres_BT)
	{
		if(!BT_Check_Time)
			BT_Check_Time = 5;
	}
	else
		BT_Check_Time = 0;
}
	
