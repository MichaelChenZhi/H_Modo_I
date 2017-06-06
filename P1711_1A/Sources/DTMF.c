#include "MC9S08AW60.h"                /* I/O map for MC9S08AW16CFU */
#include "define.h"
#include "global.h"
#include	"VideoStar.h"
#include "Trame.h"

#define	Call_low_range	6 //for 75HZ
#define	Call_high_range	31 // for 16HZ

extern void Play_New_Melody(unsigned char kind);
void Delay(uchar p)
{
	uchar i;   
	for(i = 0; i<p; i++)
	{
		Nop;
	}
}
/*-------------------------------------------------------------
this file is for DTMF managment

----------------------------------------------------------------*/

uchar Tel_Call(void)
{
	uchar i;
	uchar code_pstn;
	if(Max_TX_Index<16 && KNX_Buf[Max_TX_Index]!= 0xFF)
	{
		
		code_pstn = KNX_Buf[Max_TX_Index];
		Max_TX_Index++;
		if(code_pstn == 0x0c && MODO_State == PSTN_Dial_Number)
			return 2;// for pause function
		//Delay(20);
		for(i = 0; i < 5; i++)
		{

			DTMF_SCL = ON;
			
			Nop;
			if(code_pstn&0x01) {
			      DTMF_SDA = ON;
			}
			else
				DTMF_SDA = OFF;
			code_pstn >>= 1;
			Delay(3);
			
			DTMF_SCL = OFF;		
			Delay(2);
		}
		DTMF_SCL = ON;
		return 0;
	}
	return 1;
}

void Tel_Call_StopCode(void)
{
	uchar i;
	uchar code_pstn;
	code_pstn =0x1f;
		for(i = 0; i < 5; i++)
		{

			DTMF_SCL = ON;
			
			Nop;
			if(code_pstn&0x01) {
			      DTMF_SDA = ON;
			}
			else
				DTMF_SDA = OFF;
			code_pstn >>= 1;
			Delay(3);
			
			DTMF_SCL = OFF;		
			Delay(2);
		}
		DTMF_SCL = ON;
	
}
//---------------------------------------
void DTMF_Management(void)
{
	switch(DTMF_State)
	{
		case DTMF_Idle:
			//Tel_Con  = Tel_HOOK_ON;
			DTMF_CE = 1;
			break;
		case DTMF_Ready:
			if(Sub_MODO_State == IDLE)
			{
				Sub_MODO_State = PSTN_Dial_Number;
			}
			else
			{
				if(Sub_MODO_State == PSTN_Talking || Sub_MODO_State == PSTN_Talking_Video)
				{
					CheckDTMF_Delay = 3;
				}
				else
					Sub_MODO_State = PSTN_Talking;
			}
			//Tel_Con  = Tel_HOOK_OFF;
			if(DTMF_CE == 1)
				DTMF_wait = 100;//try if the time problem cause the start and sharp not correct when power on first
			else
				DTMF_wait = 10;
			DTMF_State = DTMF_CallCode;
			DTMF_CE = 0;
			Max_TX_Index = 0;
			EN_Z = ON;
			EN_AUDIO  = FONIA_ON;
			REL2 = OFF;
			REL6 = ON;
			HK = OFF;
			
			break;
		case DTMF_CallCode:
			if(!DTMF_wait)
			{
				/*
				if(Tel_Call())
				{	
					DTMF_State = Tel_Talk;
				}
				else
				{
					DTMF_State = DTMF_StopCode;
					DTMF_wait = 10;
				}
				*/
				switch(Tel_Call())
				{
					case 0:
						DTMF_State = DTMF_StopCode;
						DTMF_wait = 10;
					break;
					case 1:
						DTMF_State = Tel_Talk;
						if(EventHandle_Flag.bits.PSTN_Video_State_Hope)
						{
							Sub_MODO_State = PSTN_Talking_Video;
							EventHandle_Flag.bits.PSTN_Video_State_Hope = 0;
						}
						else
							Sub_MODO_State = PSTN_Talking;
					break;
					case 2:
						//DTMF_State = DTMF_StopCode;
						DTMF_wait = 100;//just wait 1s then go on dial
					break;
				}
			}
			break;
		case DTMF_StopCode:
			if(!DTMF_wait)
			{
				Tel_Call_StopCode();
				DTMF_State = DTMF_CallCode;
				DTMF_wait = 10;
			}
			break;
		case Tel_Talk:
			//DTMF_CE = 1;
			//Tel_Con  = Tel_HOOK_OFF;
			DTMF_CE = 0;
			break;
	}
}


//-------------------------------------------
void Telephone_Call_Detect(void)
{
	if(MODO_State != IDLE)//for ignore maybe it cause the system slowly issue
		return;
	if(Sub_MODO_State < PSTN_Talking)
	{
		Common_falg.bits.Dly_EN_Capture_Flag=0;
		Common_falg.bits.EN_Capture=0;
		//check if there is a call comming
		switch(Tel_Det_State)
		{
			case Tel_Idle:
				if(!Tel_DET)
				{
					Tel_det_Low++;
					Tel_Det_State = Tel_First_Low;
				}
				else
					Tel_det_Low = 0;
			break;
			case Tel_First_Low:
				if(!Tel_DET)
				{
					Tel_det_Low++;
				}
				else
				{
					Tel_Det_State = Tel_First_High;
				}
			break;
			case Tel_First_High:
				if(Tel_DET)
				{
					Tel_det_Low++;
				}
				else
				{
					if(Tel_det_Low > Call_low_range && Tel_det_Low < Call_high_range)
					{
						//pstn calling
						if(MODO_State == IDLE && PageStatus != Alarm_handle && !PanicCancleTime && Sub_MODO_State == EX_IDLE)
						{
							if(F_PSTN_With_Video)
							{
								Sub_MODO_State = PSTN_Calling_Video;
								SetSwitchIcon(ChangeButton_Icon, Menu_Telephone_Video, K_OK, 0, 0x00, 44);

								//Tel_index=16;
								//(void)memset(&Tel_num,0,sizeof(Tel_num));
								//ShowTelNum();
								//Tel_index = 0;
							}
							else
							{
								Sub_MODO_State = PSTN_Calling;
								SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB0);

								if(PageStatus!=PSTN_Telephone)
								{
									TelRdFrameTime=200;
									Tel_index=16;
									(void)memset(&Tel_num,0,sizeof(Tel_num));
									ShowTelNum();
									Tel_index = 0;
									
								}
							}
							//Tel_det_Low = 6;
							Tel_Det_State = Tel_Idle;
							PSTN_Call_TimeOut_Cnt = 6;Timer_Inactivity=20;

							//Common_falg.bits.RD_Tel_Num_flag=1;
						}
						
						else
						{
							//else we make the switch button icon blank
							//SetButton_State(0x01,Talk_To_CallModule_NotHide, K_PSTN_HRB_Switch, ON);
							Courtesy_tone();
							Tel_det_Low = 0;
							Tel_Det_State = Tel_Idle;
						}
						
					}
					else
					{
						Tel_det_Low = 0;
						Tel_Det_State = Tel_Idle;
					}
				}
			break;
		}
		
		
	}
	else if(Sub_MODO_State == PSTN_Calling || Sub_MODO_State == PSTN_Calling_Video)
	{
		Common_falg.bits.Dly_EN_Capture_Flag=0;
		Common_falg.bits.EN_Capture=0;
		
		//check if call signal dismiss
		/*
		if(!Tel_DET)
			Tel_det_Low_new = 6;
		else
		{
			if(!Tel_det_Low_new)
			{
				MODO_State = IDLE;
				PSTN_Call_TimeOut_Cnt = 0;
			}
		}
		*/
		if(CheckDTMF_Delay)
		{
			Tel_Det_State = Tel_Idle;
			Tel_det_Low = 0;
			return;
		}
		switch(Tel_Det_State)
		{
			case Tel_Idle:
				if(!Tel_DET)
				{
					Tel_det_Low++;
					Tel_Det_State = Tel_First_Low;
				}
				else
					Tel_det_Low = 0;
			break;
			case Tel_First_Low:
				if(!Tel_DET)
				{
					Tel_det_Low++;
					
				}
				else
				{
					Tel_Det_State = Tel_First_High;
				}
			break;
			case Tel_First_High:
				if(Tel_DET)
				{
					Tel_det_Low++;
				}
				else
				{
					if(Tel_det_Low > Call_low_range && Tel_det_Low < Call_high_range)
					{
						
						Tel_Det_State = Tel_Idle;
						PSTN_Call_TimeOut_Cnt = 6;Timer_Inactivity=20;
						Tel_det_Low = 0;	
						//RingRepeatTime = 12;
						//EventHandle_Flag.bits.RingToneNeed = 1;
						if(!EventHandle_Flag.bits.PSTNOnRingTone)
						{
							Play_New_Melody(PSTN_CALL);
							TR_2 = OFF;// ring tone also for handset
							EventHandle_Flag.bits.PSTNOnRingTone = 1;
							EventHandle_Flag.bits.StopToneNeed = 1;	
						}
					}
					else
					{
						//check if the stop tone need
						if(EventHandle_Flag.bits.StopToneNeed && PSTN_Call_TimeOut_Cnt<5)
						{
							Stop_tone();
							//EventHandle_Flag.bits.RingToneNeed = 0;
							
						}
						Tel_det_Low = 0;
						Tel_Det_State = Tel_Idle;
					}
				}
			break;
		}
	}
	else if(Sub_MODO_State == PSTN_Talking || Sub_MODO_State == PSTN_Talking_Video)
	{
		//Common_falg.bits.RD_Tel_Num_flag=1;
	
		//check the call is end		
		if(CheckDTMF_Delay)
		{
			Tel_Det_State = Tel_Idle;
			Tel_det_Low = 0;
			EventHandle_Flag.bits.PSTNOnRingTone = 0;
			EventHandle_Flag.bits.StopToneNeed = 0;	
			return;
		}
		//Common_falg.bits.EN_Capture=1;
		if(Common_falg.bits.blind_audio==1)
		{
			Common_falg.bits.blind_audio=0;
			Common_falg.bits.EN_Capture=0;
			
			if(Sub_MODO_State == PSTN_Talking){
				SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB0);
				SetButton_State(0x01, PSTN_Telephone, 16, OFF);
			}
			else if(Sub_MODO_State == PSTN_Talking_Video){
				SetSwitchIcon(ChangeButton_Icon, Menu_Telephone_Video, K_OK, 0, 0x00, 44);
				SetButton_State(0x01, Menu_Telephone_Video, 57, OFF);
			}
			
			if(Tel_index>0)
			{
				(void)memset(&Tel_num,0,sizeof(Tel_num));
				ShowTelNum();
			}
			Tel_index = 0;

			PSTN_Call_TimeOut_Cnt = 0;														
			DTMF_State = DTMF_Idle;
			Sub_MODO_State = IDLE;
			
			Timer_Inactivity = 20;//TIME_AUTOPOWEROFF;
			if(PageStatus != Menu_Telephone_Video)
			{
				OLD_Sub_Modo_State = EX_IDLE;
			}

			//Timer_ExitPSTN=30;
			
		}
					
/*		switch(Tel_Det_State)
		{
			case Tel_Idle:
				if(!HK_Check)
				{
					Tel_det_Low++;
					Tel_Det_State = Tel_First_Low;
				}
				else
					Tel_det_Low = 0;
			break;
			case Tel_First_Low:
				if(!HK_Check)
				{
					Tel_det_Low++;
				}
				else
				{
					if(Tel_det_Low>115 && Tel_det_Low<280) //old value is 145 due to normal telephone is about 250ms high and 250ms low, check we found GSM signal is about 380ms high and 320ms low ; Tel_det_Low<200 to 250 due to new HW new PABX to 500ms
					{
						Tel_Det_State = Tel_First_High;
						Tel_det_Low = 0;
					}
					else
						Tel_Det_State = Tel_Idle;
				}
			break;
			case Tel_First_High:
				if(HK_Check)
				{
					Tel_det_Low++;
					
				}
				else
				{
					if(Tel_det_Low > 115 && Tel_det_Low < 280)
					{
						//should close the call

							PSTN_Call_TimeOut_Cnt = 0;														
							DTMF_State = DTMF_Idle;
							Sub_MODO_State = IDLE;
							
							SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB0);
							
							if(Tel_index>0)
							{
								(void)memset(&Tel_num,0,sizeof(Tel_num));
								ShowTelNum();
							}
							Tel_index = 0;
							Timer_Inactivity = TIME_AUTOPOWEROFF;
							if(PageStatus != Menu_Telephone_Video)
							{
								OLD_Sub_Modo_State = EX_IDLE;
							}
						//}
					}
					else
						Tel_Det_State = Tel_Idle;
				}
			break;
		}
*/		
	}
}





