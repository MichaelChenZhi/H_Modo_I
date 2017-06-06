#include <hidef.h>                                /* for EnableInterrupts macro */
#include "derivative.h"                           /* include peripheral declarations */
#include <string.h>
#include  "define.h"
#include "Global.h"
#include "Driv_der.h"
#include "Trame.h"
#include "VideoStar.h"
#include "HomeAutoMation.h"
#include "Eeprom.h"
#include "Ges_der.h"
extern void Gest_Alarm(void);
void ReScanAlarmEvent(uchar channel);


void AlarmFrame_Format(void)
{
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 6;
	p_der_queue_tx_appl->data[D_OPC] = D_ALARM;
	p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 					
	p_der_queue_tx_appl->data[D_DATA_1] = Alarm_Type_To_Switchboard>>8;//
	p_der_queue_tx_appl->data[D_DATA_2] = Alarm_Type_To_Switchboard&0x00ff;//alarm flag
	p_der_queue_tx_appl->data[D_DATA_3] = 0x00;
	p_der_queue_tx_appl->data[D_DATA_4] = H_Modo_Ipervoice; //only for Hmodo
	Incr_index_queue_in_der(); 
}

void GetAlarm_Type_Format(uchar p)
{
	Alarm_Type_To_Switchboard = 0;
	if(p)
	{
		Alarm_Type_To_Switchboard = 0x8000 >> (p -1);
	}
	/*
	switch(p)
	{
		case Alarm_Tpye_Burglar:
			Alarm_Type_To_Switchboard = 0x8000;
		break;
		case Alarm_Tpye_Panic:
			Alarm_Type_To_Switchboard = 0x4000;
		break;
		case Alarm_Tpye_Fire:
			Alarm_Type_To_Switchboard = 0x2000;
		break;
		case Alarm_Tpye_Gas:
			Alarm_Type_To_Switchboard = 0x1000;
		break;
		case Alarm_Tpye_Flooding:
			Alarm_Type_To_Switchboard = 0x0800;
		break;
		case Alarm_Tpye_Intrusion:
			Alarm_Type_To_Switchboard = 0x0400;
		break;
		case Alarm_Tpye_Coercion:
			Alarm_Type_To_Switchboard = 0x0200;
		break;
		case Alarm_Tpye_Tamper:
			Alarm_Type_To_Switchboard = 0x0100;
		break;
		case Alarm_Tpye_DoorForced:
			Alarm_Type_To_Switchboard = 0x0080;
		break;
	}
	*/
}

void WarningTone(uchar p)
{
	WarningToneNumber = p;
	WarningToneTime = 3;
	Dissuasion_Tone();
}

void SwitchToAlarmMenu(void)
{
	if(PageStatus != AlarmStatus_Menu)
	{
		If_need_Wakeup_LCD();
		if(MODO_State != IDLE) //close the conversation when send the alarm to switchboard
		{
	  		Free_Line();
		}
		Sub_MODO_State = Alarm_Handle_Waiting;
		PageStatus = Alarm_handle;
		LoadJD_Menu(Alarm_handle);
		WarningTone(20);
	}
}


void SendDelayAlarm(uchar channel)
{
	InPutAlarm *pp;
	pp=(InPutAlarm *)(EE_Input_Memory1 + channel*sizeof(InPutAlarm));
	AlarmNeed_Reset_Flag |= (0x0001<<channel);//set
	GetAlarm_Type_Format(pp->Alarm_Type);
	Gest_Alarm();
	if(pp->Alarm_Event_GroupAddress)
	{
		KNX_Device_Control_Queue(pp->Alarm_Event_GroupAddress, (pp->Alarm_Event_Value <<8), 1);					
	}
	SetSwitchIcon(ChangeButton_Icon, Alarm_handle, 2, 0, 0x00, 230 + pp->Alarm_Type -1);
		
		//switch to the alarm menu
		/*
	if(PageStatus != AlarmStatus_Menu)
	{
		If_need_Wakeup_LCD();
		if(MODO_State != IDLE) //close the conversation when send the alarm to switchboard
		{
	  		Free_Line();
		}
		Sub_MODO_State = Alarm_Handle_Waiting;
		PageStatus = Alarm_handle;
		LoadJD_Menu(Alarm_handle);
		WarningTone(20);
	}
		*/
		SwitchToAlarmMenu();
	
}

uchar CheckAlarmEvent(uchar type, uchar zone)
{
	uchar i; 
	
	if(Alarm_Enable_Time_Delay_CNT)// enable delay, for guys leave
		return 0;
	if(type == Alarm_24H_OnDuty)
		return 1;
	else if(type == Alarm_Instants)
	{
		//check if zone enable
		if(!zone) //not belong any zone
			return 1;
		else
		{
			for(i=0;i<ZoneNum;i++)
			{
				if(zone & (0x01<<i))
				{
					if(AlarmP->AlarmZoneEnable & (0x01<<i))
						return 1; //belong zone is set to enable
				}
			}
		}
		
	}
	else if(type == Alarm_Delay)
	{
		for(i=0;i<ZoneNum;i++)
		{
			if(zone & (0x01<<i))
			{
				if(AlarmP->AlarmZoneEnable & (0x01<<i))
					return 2; //belong zone is set to enable
			}
		}	
	}
	return 0;
}

void ScanAlarmEvent(uchar channel)
{
	InPutAlarm *pp;
	uchar x;
	//unsigned int xxx = channel;
	//x = (channel-1)/4;
	//y = (channel-1)%4;
	pp=(InPutAlarm *)(EE_Input_Memory1+ (channel-1)*sizeof(InPutAlarm));
	
	temp8 = pp->Alarm_Type;
	if(temp8 != 0)//check if we have the alarm type
	{
		//send the alarm message to switchboard
		//temp8 = pp->Alarm_Type -1;
		//test if it is the instants or delay or 24H
		x = CheckAlarmEvent(pp->property,pp->Zone_Belong);
		if(x == 1)
		{
			//begin count the time
			Alarm_DelayTime_Cnt[channel-1] = 1;//pp->Delay_Time;
			if(!Alarm_Delay_Tone_Cnt)
			{ //put time and beginning transmit beep tone
				Alarm_Delay_Tone_Cnt = 1;//pp->Delay_Time; 
			}
			AlarmDisplay_Channel = channel;

			return;
			
		
				//only master can be send alarm,check the alarm type first and then send the frame
				GetAlarm_Type_Format(pp->Alarm_Type);
				Gest_Alarm();
				//SendAlarm(OPC_ALL1+ temp8-1); 
				AlarmNeed_Reset_Flag |= (0x0001<<(channel-1));//set
				//for alarm scenarios is default as both protocol
				
				//KNX scenario
				if(pp->Alarm_Event_GroupAddress)
				{
					KNX_Device_Control_Queue(pp->Alarm_Event_GroupAddress, (pp->Alarm_Event_Value <<8), 1);					
				}

				
				//set the correct icon to the alarm menu
				if(Alarm_Delay_Tone_Cnt)
					return;//delay with the priority
				SetSwitchIcon(ChangeButton_Icon, Alarm_handle, 2, 0, 0x00, 230 + temp8 -1);
				AlarmDisplay_Channel = channel;
				//switch to the alarm menu
				/*
				if(PageStatus != AlarmStatus_Menu)
				{
					
					If_need_Wakeup_LCD();
					if(MODO_State != IDLE)
					{
	  					Free_Line();
					}
					//callback function for enter alarm Handle menu
					PageStatus = Alarm_handle;
					Sub_MODO_State = Alarm_Handle_Waiting;
					LoadJD_Menu(Alarm_handle);
					WarningTone(20);
					
				}
				*/
				 SwitchToAlarmMenu();
			
		}
		else if(x == 2)
		{
			//delay alarm wait
			If_need_Wakeup_LCD();
			PageStatus = AlarmSystem_Manage;
			LoadJD_Menu(AlarmSystem_Manage);
			AlarmSystemZoneName_Init();
			//begin count the time
			Alarm_DelayTime_Cnt[channel-1] = pp->Delay_Time;
			if(!Alarm_Delay_Tone_Cnt)
			{ //put time and beginning transmit beep tone
				Alarm_Delay_Tone_Cnt = pp->Delay_Time; 
			}
			AlarmDisplay_Channel = channel;
		}
	}
}


void Alarm_Logo(void)
{
	if(AlarmP->AlarmZoneEnable)
	{
		SetControl_State(MainMenu, 8, ON);
		Alarm_Enable_Time_Delay_CNT = AlarmP->En_Alarm_DelayTime;
	}
	else
	{
		SetControl_State(MainMenu, 8, OFF);
		if(Alarm_Enable_Time_Delay_CNT)
			Alarm_Enable_Time_Delay_CNT = 1;
	}
		
}

void Away_Alarm(void)
{
	if(AlarmP->AlarmZoneEnable != AlarmP->Away_Zone_Belong)
	{
		SaveAlarmEnableFlag(AlarmP->Away_Zone_Belong);
		for(temp1 = 0; temp1 < ZoneNum; temp1++)
		{
			if(AlarmP->AlarmZoneEnable & (0x01<<temp1))
				SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, K_Zone1+temp1, 0, 0x01, 0xA6);
			else
				SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, K_Zone1+temp1, 0, 0x01, 0x93);
		}
		/*
		if(AlarmP->AlarmZoneEnable)
		{
			SetControl_State(MainMenu, 8, ON);
		}
		else
			SetControl_State(MainMenu, 8, OFF);
		Alarm_Enable_Time_Delay_CNT = AlarmP->En_Alarm_DelayTime;
		*/
		Alarm_Logo();
	}
}

void Home_Alarm(void)
{
	if(AlarmP->AlarmZoneEnable != AlarmP->Home_Zone_Belong)
	{
		SaveAlarmEnableFlag(AlarmP->Home_Zone_Belong);
		for(temp1 = 0; temp1 < ZoneNum; temp1++)
		{
			if(AlarmP->AlarmZoneEnable & (0x01<<temp1))
				SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, K_Zone1+temp1, 0, 0x01, 0xA6);
			else
				SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, K_Zone1+temp1, 0, 0x01, 0x93);
		}
		
	}
	//enable alarm delay
	Alarm_Logo();
}



uchar IsEn_Dis_Alarm(unsigned int T_address, uchar value)
{
	AlarmSystem *pp;
	
	pp=(AlarmSystem *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem));
	
	if(pp->Disable_group_address == T_address && pp->D_ID == value)
	{
		//Disable alarm
		Away_Alarm();
		return 1;
	}
	else if(pp->Enable_group_address== T_address && pp->E_ID == value)
	{
		//enable alarm
		Home_Alarm();
		return 1;
	}
	return 0;
}

uchar IsMydvice_ID(unsigned int T_address)
{
	uchar i;
	InPutAlarm *pp;
	//uchar x,y;
	for(i = 0; i < 10; i++)
	{
		//x = i/4;
		//y = i%4;
		pp=(InPutAlarm *)(EE_Input_Memory1 + i*sizeof(InPutAlarm));
		if(pp->group_address == T_address)
		{
			temp8 = pp->Alarm_Type;
			return i+1;
		}
	}
			
	return 0;
}


void ShowNextAlarm(void)
{
	uchar i;
	if(AlarmNeed_Reset_Flag)
	{
		//we with alarm need to reset
		for(i = 0; i < 10; i++)
		{
			if(AlarmNeed_Reset_Flag&(1<<i))
			{
				AlarmDisplay_Channel = i+1;
				ReScanAlarmEvent(AlarmDisplay_Channel);
			}
			if(AlarmDisplay_Channel)
			{
				return;
			}
		}
	}
}

void ReScanAlarmEvent(uchar channel)
{
	InPutAlarm *pp;
	uchar x;
	
	//x = (channel-1)/4;
	//y = (channel-1)%4;
	pp=(InPutAlarm *)(EE_Input_Memory1+ (channel-1)*sizeof(InPutAlarm));
	
	temp8 = pp->Alarm_Type;
	if(temp8 != 0)//check if we have the alarm type
	{
		//send the alarm message to switchboard
		//temp8 = pp->Alarm_Type -1;
		//test if it is the instants or delay or 24H
		x = CheckAlarmEvent(pp->property,pp->Zone_Belong);
		if(x > 0)
		{
			
			SetSwitchIcon(ChangeButton_Icon, Alarm_handle, 2, 0, 0x00, 230 + temp8 -1);
			//AlarmDisplay_Channel = channel;
		
			//if(PageStatus != AlarmStatus_Menu)
			//{
				
			//	PageStatus = Alarm_handle;
			//	LoadJD_Menu(Alarm_handle);
			//	
				
			//}
		}
		else
			AlarmDisplay_Channel = 0;
		
	}
}


void ShowRecount(uchar num, uchar MenuID)
{
	uchar data[3];
	if(num)
	{
		data[0] = num/100 +0x30;
		data[1] =( (num/10)%10)+0x30;
		data[2] = (num%10)+0x30;
		SetTel_Num(MenuID,4,3,data);
	}
	else
		SetTel_Num(MenuID,4,0,data);
}

void CancleAllDelayAlarm(void)
{
	if(Alarm_Delay_Tone_Cnt)
	{
		Alarm_Delay_Tone_Cnt = 0;
		ShowRecount(Alarm_Delay_Tone_Cnt,AlarmSystem_Manage);
		//clear all delay alarm
		for(temp1 = 0 ; temp1< HomeAutomation_MaxInput; temp1++)
		{
			Alarm_DelayTime_Cnt[temp1] = 0;
		}
	}
}

//--------------------------------------------------------------------------
uchar CheckPasswordForInstaller(void)
{
	uchar i;
	for(i = 0;i<6;i++)
	{
		if(Tel_num[i] != A_password->Installer[i])
		{
			return 0;
		}
								
	}
	return 1;
}


uchar CheckPasswordForUser(void)
{
	uchar i;
	for(i = 0;i<6;i++)
	{
		if(Tel_num[i] != A_password->User[i])
		{
			return 0;
		}
								
	}
	return 1;
}
//------------------------------------------------------------------------