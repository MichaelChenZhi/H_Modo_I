
#include <hidef.h>                                /* for EnableInterrupts macro */
#include "derivative.h"                           /* include peripheral declarations */
#include <string.h>
#include  "define.h"
#include "Global.h"

#include "Trame.h"
#include "VideoStar.h"
#include "AlarmSystem.h"

extern void ReadTime_From_JDBoard(void);
void KNX_Device_Control(unsigned int command, unsigned int dest_addr, unsigned int value, uchar length);
void KNX_Device_Control_Queue(unsigned int dest_addr, unsigned int value, uchar length);
void HomeHA_ACButton_Press(uchar room ,uchar index,uchar key, uchar key1, uchar Value);
//void outputrelays(unsigned int g_address, uchar st);
//void outputRGB(unsigned int g_address, RGB_Para st);
void HomeHA_RoomRGBButton_Press(uchar room ,uchar index,uchar key, uchar key1);
//void bcc_AvvioTx(void);
void IsMyUpdataStatus(unsigned int g_address, uchar value);
void IsMyACStatus(unsigned int g_address, uchar value, uchar value1);
const uchar Dimmer_Value[32] = {4,7,11,14,14,17,21,24,28,31,38,35,42,35,45,49,52,56,63,59,63,66,70,73,77,81,84,88,88,91,96,100};
const RGB_Para RGB_Value[49] = {254,208,0,// 1
							254,148,0,
							251,29,3,
							255,0,0,
							250,3,34,// 5
							255,0,99,
							255,0,162,
							255,0,228,
							255,252,0,
							255,215,0,// 10
							255,164,0,
							255,50,0,
							255,0,50,
							255,0,173,
							255,0,242,// 15
							254,0,255,
							218,255,0,
							252,245,7,
							235,0,255,
							212,0,255,// 20
							159,254,4,
							162,255,0,
							166,1,251,
							144,6,239,
							68,255,4,// 25
							44,255,0,
							43,1,255,
							67,1,247,
							0,255,0,
							4,252,16,// 30
							1,15,246,
							0,0,255,
							2,254,19,
							4,254,43,
							7,251,104,// 35
							13,249,203,
							13,218,249,
							8,119,155,
							0,56,255,
							6,18,228, // 40
							0,255,36,
							1,254,109,
							0,254,168,
							4,252,228,
							11,237,249,// 45
							5,175,253,
							0,116,255,
							2,69,248, // 48
							0,0,0 // close
							};


uchar If_JD_Call_Empty(void)
{
	if(JDFunctionCallBack == NULL && JDFunctionCallBack1 == NULL && JDFunctionCallBack2 == NULL)
		return 1;
	return 0;
}


void OpenHeatChannel(void)
{
	KNX_Device_Control_Queue(Group_Address[0]+ HVAC_Heat_Command,ON,0);
	SetTimeDelay = 1;
}

void CloseHeatChannel(void)
{
	KNX_Device_Control_Queue(Group_Address[0]+ HVAC_Heat_Command,OFF,0);
	SetTimeDelay = 1;
}

void OpenCoolChannel(void)
{
	KNX_Device_Control_Queue(Group_Address[0]+ HVAC_Cool_Command,ON,0);
	SetTimeDelay = 1;
}

void CloseCoolChannel(void)
{
	KNX_Device_Control_Queue(Group_Address[0]+ HVAC_Cool_Command,OFF,0);
	SetTimeDelay = 1;
}


void HVAC_Power_ON_OffCallBack(void)
{
	HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Save_Mode,OFF);
	SetTimeDelay = 1;
}

void HVAC_ComfortModoCallBack(void)
{
	HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Comfort_Mode,OFF);
	SetTimeDelay = 1;
}

void HVAC_NightModeCallBack(void)
{
	HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Night_Mode,OFF);
	SetTimeDelay = 1;
}
void UpdataTemperatureTOBus(void)
{
	unsigned int TT;
	TT = FloatToInt(A_CTemperature[RoomSelect] * 100);
	KNX_Device_Control_Queue(Group_Address[0], TT, 2);
}
uchar CheckDate(void)
{
	uchar day,mon,year;
	year=10*R_Date_Temp.Year3+R_Date_Temp.Year4;
	day = R_Date_Temp.Day1 * 10 + R_Date_Temp.Day2;
	mon = R_Date_Temp.Month1 * 10 + R_Date_Temp.Month2;
	if(mon > 12 || mon == 0)
		return 0;
	/*
	if((year%4==0)&&(mon==2)&&(day>29))
		return 0;
	if((year%4!=0)&&(mon==2)&&(day>28))
		return 0;
	if(day > 31 || day == 0)
		return 0;*/

	if(day){
		if(mon==2){
			if(year%4==0){
				if(day>29)
					return 0;	
			}
			else{
				if(day>28)
					return 0;	
			}
		}
		else if((mon==4)||(mon==6)||(mon==9)||(mon==11)){
			if(day>30)
				return 0;	
		}
		else{
			if(day>31)
				return 0;	
		}

	}
	else
		return 0;	
	
	return 1;
}
void FillListData(void)
{
	for(temp1 = 0; temp1 < 10; temp1++)
	{
		Tel_num_Record = ((Tel_Memory_Record *)(EE_Page_Tel_Momory + (temp1*sizeof(Tel_Memory_Record))));
		if(Tel_num_Record->Tel_Num_Len>0 && Tel_num_Record->Tel_Num_Len<17)
		{
			SetTel_Num(Tel_Contact_List,40+temp1,Tel_num_Record->Tel_Num_Len,Tel_num_Record->Tel_Number);
		}
	}

}
/*
time to recount the new timer event
 */
void CheckRecount_Timer(void)
 {
 	if(EventHandle_Flag.bits.ReflashTimerCount_F && If_JD_Call_Empty())
 	{
 		EventHandle_Flag.bits.ReflashTimerCount_F = 0;
		JDFunctionCallBack1 = &ReadTime_From_JDBoard;
		//JDFunctionCallBack2 = &TimeToReflashTimeEventCount;
		
 	}
 }


/*
check the event time if it is enable , after that we set the time to count
this function will be call at the beginning we need count the time, or every 0 o'clock, we need to check the time if the date match
itme: means the event id, notice that we have 8 event can be enable or disable
*/
void TimeEventCaculate(uchar item)
{
	uchar T_Hour,T_Min;
	Timer_Event *p;
	T_Hour = R_Clock.Hour1*10 + R_Clock.Hour2;
	T_Min = R_Clock.Min1*10 +  R_Clock.Min2;
	if(item < 8)
	{
		
		 p = (Timer_Event *) (EE_Time_Event);
		 p += item;
		 Timer_Handle_TimerEvent[item] = 0;
		 if(p->mode&0x01)
		 {
		 	//the time event is enable
		 	//check if date match
		 	if( R_Date.Week < 7)
		 	{	//the date is valid
			 	if(p->mode & (1<<(R_Date.Week + 1)))
			 	{
			 		//date match
			 		if(T_Hour < p->Hour)
				 	{
				 		if(T_Min < p->Minite)
				 		{
				 			Timer_Handle_TimerEvent[item] = (p->Hour - T_Hour) * 60 + (p->Minite - T_Min);
				 		}
						else
						{
							Timer_Handle_TimerEvent[item] = (p->Hour - T_Hour -1) * 60 +  (60 -(T_Min  - p->Minite));
						}
				 	}
					else if(T_Hour == p->Hour)
					{
						if(T_Min < p->Minite)
				 		{
				 			Timer_Handle_TimerEvent[item] =  p->Minite - T_Min;
				 		}
						else if(T_Min == p->Minite)
						{
							Timer_Handle_TimerEvent[item] = 1;
						}
					}
			 	}
		 	}
		 	
		 	
		 }
	}
}
/*
Time Event handle
read time from JD board and then compare the time in flash, if match and it is enable,then transimt the command
*/
void TimeEvent(void)
{
	 uchar i;
	 uchar X;
	 
	 Timer_Event *p;
	 p = (Timer_Event *) (EE_Time_Event);
	 if(KNX_Save_Index != KNX_TX_Index || Timer_Event_Delay || !If_JD_Call_Empty())
	 {//make sure the HRB queue is OK, due HRB only with 4 queue but here we with
		return;
	 }
	 if(TimeEvent_Flag.all)
	 {
		 for(i = 0; i < 8; i++)
		 {
		 	if(TimeEvent_Flag.all & (0x01<<i))
		 	{
		 		//with time event need to handle
		 		if(Event_Step < 5)
		 		{
		 			X = p->Protocol_Select[Event_Step] & 0x0f;
					if(p->Inf_address[Event_Step])// && p->Inf_ID[Event_Step])//Fixed according Michael 2015-1-8
					{
		 		
			 				if(p->Protocol_Select[Event_Step] & 0x80)
			 				{
			 					//For AC touch
			 					if(p->Protocol_Select[Event_Step] & 0x40)
			 					{
			 						//set the temperature
			 						A_CTemperature[RoomSelect] = p->Inf_ID[Event_Step];
			 					}
								
			 					HomeHA_ACButton_Press((p->Inf_address[Event_Step]>>12),(p->Inf_address[Event_Step]>>11)&0x01,(p->Inf_address[Event_Step]>>8)&0x07,(p->Inf_address[Event_Step]&0x00ff),p->Inf_ID[Event_Step]);
			 				}							
			 				else if(p->Protocol_Select[Event_Step] & 0x10)
			 				{

								if(p->Protocol_Select[Event_Step] & 0x40)
								{
									//only for RGB
									HomeHA_RoomRGBButton_Press((p->Inf_address[Event_Step]>>12),(p->Inf_address[Event_Step]>>8)&0x0f,(p->Inf_address[Event_Step]&0x00ff),p->Inf_ID[Event_Step]);
								}
			 					else if(p->Protocol_Select[Event_Step] & 0x20)
			 						KNX_Device_Control_Queue( p->Inf_address[Event_Step], ((p->Inf_ID[Event_Step] -1)<<8), 1);
								else
									KNX_Device_Control_Queue(p->Inf_address[Event_Step], ((p->Inf_ID[Event_Step])<<8), 1);
			 				}
							else
							{
								//only 1 bit data, it should be switch on or off
								KNX_Device_Control_Queue(p->Inf_address[Event_Step],((p->Inf_ID[Event_Step])<<8),0);
							}


						Timer_Event_Delay = 2;//for 20ms delay
					}
		 			Event_Step++;
		 		}
				else
				{
					Event_Step = 0;
					//clear the event flag
					TimeEvent_Flag.all &= ~(0x01<<i);
					//TimeEvent_Flag_Done.all |= (0x01<<i);//event has been handle
				}
				return;
		 	}
			p++;
		 }
	 }
}


void TimeToReflashTimeEventCount(void)
{
	uchar i;
	for(i = 0;i < 8;i++)
	{
		TimeEventCaculate(i);
	}
	//set the time when need count again1
	Timer_Reflash_Count = (24 - (R_Clock.Hour1*10+R_Clock.Hour2))*60 + (60 -(R_Clock.Min1*10 + R_Clock.Min2));
}


void HomeHA_RoomButton_TypePress_Filter(uchar room ,uchar index,uchar key)
{
	
	OutPutButtons *p;
	uchar i,j,y;
	uchar x,z,k = 0;
	if(index)
		z = key+MaxObject_In_OneMenu+1;
	else
		z = key+1;
	
	for(j = 0; j < 2; j++)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ j*168);
		for(x = 0; x<MaxObject_In_OneMenu;x++)
		{
			
			
			if(IfBelongType(p->Button_Type, Filter_ID))
				k++;
			if(k == z)
			{
				break;
			}
			p++;
		}
		if(k == z)
		{
			break;
		}
	}
	//p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x400)+ index*0x200 + key*sizeof(OutPutButtons));
	i = room*2+j;
	k = x;
	Filter_Button_Select = j;
	Filter_KeySelect = x;
	if(p->Button_Type == Dimmer_Button)
	{
		//go to Dimmer menu
		PageStatus = Dimmer_Select;
		LoadJD_Menu(Dimmer_Select);
	}
	else if(p->Button_Type == RGB_Button)
	{
		//go to RGB menu
		PageStatus = RGB_Select;
		LoadJD_Menu(RGB_Select);
	}
	else if(p->Button_Type == Shutter_Button)
	{
		PageStatus = Shutter_Select;
		LoadJD_Menu(Shutter_Select);
	}
	else if(p->Button_Type == Shutter_L_R_Button)
	{
		PageStatus = Shutter_R_L_Select;
		LoadJD_Menu(Shutter_R_L_Select);
	}
	else if(p->Button_Type == Shutter_Blind_Button)
	{
		PageStatus = Shutter_Blind_Select;
		LoadJD_Menu(Shutter_Blind_Select);
	}
	else if(p->Button_Type == AC_Button)
	{
		AC_Menu_Init(AC_Select);
		Group_Address[0] = p->group_address;
		PageStatus = AC_Select;
		LoadJD_Menu(AC_Select);
	}
	else if(p->Button_Type == HVAC_Button)
	{
		//HVAC_Select
		AC_Menu_Init(HVAC_Select);
		Group_Address[0] = p->group_address;
		PageStatus = HVAC_Select;
		LoadJD_Menu(HVAC_Select);
	}
	else
	{
		//reley button or valve button
	
		if(LightStatus[i] & (0x01<<k) )
		{
			//outputrelays(OutputButton_List[key].DeviceID,OFF);
			y = OFF;
			
			if(p->Button_Type == Valve_Button ||p->Button_Type == Valve_Button_PC)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue+10, 0, 0x00, 103);
			}
			else if(p->Button_Type == Valve_Button1 ||p->Button_Type == Valve_Button1_PC)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue+10, 0, 0x00, 101);
			}
			else
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue+10, 0, 0x00, 106);
			}
			
			LightStatus[i] &= ~(0x01<<k);//clear
		}
		
		else
		{
			y =ON;
			
			if(p->Button_Type == Valve_Button ||p->Button_Type == Valve_Button_PC)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue+10, 0, 0x00, 104);
			}
			else if(p->Button_Type == Valve_Button1 ||p->Button_Type == Valve_Button1_PC)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue+10, 0, 0x00, 102);
			}
			else
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue+10 , 0, 0x00, 105);
			}
			
			//temp1 = (0x01<<key);
			LightStatus[i] |= (0x01<<k); //set
			
		}
		//update icon

		if(p->Button_Type == Relay_Button_PC ||p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button1_PC)
			y = 0;
		if(p->Button_Type == Relay_Button_PO ||p->Button_Type == Valve_Button_PO|| p->Button_Type == Valve_Button1_PO)
			y = 1;

			
			KNX_Device_Control_Queue(p->group_address,y<<8,0);

		
		if(ExitMenu != NULL)
		{
			//save event 
			Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
			Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= (p->Protocol_Type & 0x0f);
			
			Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x8f; //clear data length bit and scnario bit and RGB bit
			Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address;
			Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = y;
		}
		
	}
	
}


void HomeHA_RoomButton_Press_Filter(uchar room ,uchar index,uchar key)
{
	//index 
	OutPutButtons *p;
	
	uchar i,j,y;
	uchar x,z,k = 0;
	if(index)
		z = key+MaxObject_In_OneMenu+1;
	else
		z = key+1;
	
	for(j = 0; j < 2; j++)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ j*168);
		for(x = 0; x<MaxObject_In_OneMenu;x++)
		{
			
			
			if(IfBelongType(p->Button_Type, Filter_ID))
				k++;
			if(k == z)
			{
				break;
			}
			p++;
		}
		if(k == z)
		{
			break;
		}
	}

	
	//p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x400)+ index*0x200 + key*sizeof(OutPutButtons));
	i = room*2+j;
	Filter_Button_Select = j;
	Filter_KeySelect = x;
	if(p->Button_Type == Shutter_Button)
	{
		PageStatus = Shutter_Select;
		LoadJD_Menu(Shutter_Select);
		return;
	}
	if(p->Button_Type == Shutter_L_R_Button)
	{
		PageStatus = Shutter_R_L_Select;
		LoadJD_Menu(Shutter_R_L_Select);
		return;
	}
	if(p->Button_Type == Shutter_Blind_Button)
	{
		PageStatus = Shutter_Blind_Select;
		LoadJD_Menu(Shutter_Blind_Select);
		return;
	}
	else if(p->Button_Type == AC_Button)
	{
		
		//Air condition menu init
		AC_Menu_Init(AC_Select);
		PageStatus = AC_Select;
		Group_Address[0] = p->group_address; //for save time we don't need get the adderss every time
		LoadJD_Menu(AC_Select);
		return;
	}
	else if(p->Button_Type == HVAC_Button)
	{
		AC_Menu_Init(HVAC_Select);
		PageStatus = HVAC_Select;
		Group_Address[0] = p->group_address; //for save time we don't need get the adderss every time
		LoadJD_Menu(HVAC_Select);
		return;
	}
	if(LightStatus[i] & (0x01<<x) )
	{
			//outputrelays(OutputButton_List[index*8+key].DeviceID,OFF);	
			y = 0;
			if(p->Button_Type == Valve_Button ||p->Button_Type == Valve_Button_PC ||p->Button_Type == Valve_Button_PO)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue, 0, 0x00, 103);
			}
			else if(p->Button_Type == Valve_Button1 ||p->Button_Type == Valve_Button1_PC ||p->Button_Type == Valve_Button1_PO)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue, 0, 0x00, 101);
			}
			
			else
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue, 0, 0x00, 106);
			LightStatus[i] &= ~(0x01<<x);//clear
	}
		
	else
	{
			//outputrelays(OutputButton_List[index*8 + key].DeviceID,y);
			if(p->Button_Type == Valve_Button ||p->Button_Type == Valve_Button_PC ||p->Button_Type == Valve_Button_PO)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue, 0, 0x00, 104);
			}	
			else if(p->Button_Type == Valve_Button1 ||p->Button_Type == Valve_Button1_PC ||p->Button_Type == Valve_Button1_PO)
			{
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue, 0, 0x00, 102);
			}
			else
				SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, KeyValue , 0, 0x00, 105);
			//temp1 = (0x01<<key);
			LightStatus[i] |= (0x01<<x); //set
			
			//comment old due to we with new address for dimmer on or off
			if(p->Button_Type == RGB_Button)
			{
				y = 254;
				
			}
			else
			{
				//reley button
				y = 1;
	
			}
	}
	
		if(p->Button_Type == Relay_Button || p->Button_Type == Valve_Button || p->Button_Type == Valve_Button1 ||  p->Button_Type == Relay_Button_PC || p->Button_Type == Relay_Button_PO ||p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button1_PC ||p->Button_Type == Valve_Button_PO|| p->Button_Type == Valve_Button1_PO|| p->Button_Type == Dimmer_Button)
		{
			if(p->Button_Type == Relay_Button_PC ||p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button1_PC)
				y = 0;
			if(p->Button_Type == Relay_Button_PO ||p->Button_Type == Valve_Button_PO|| p->Button_Type == Valve_Button1_PO)
				y = 1;
			KNX_Device_Control_Queue(p->group_address,y<<8,0);
			if(ExitMenu != NULL)
			{
				//save event 		
				Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
				Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= (p->Protocol_Type & 0x0f) ; 
				Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x8f; //clear data length bit and scnario bit
				Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address;
				Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = y;				
			}
		}
		else
		{	
			
			KNX_Device_Control_Queue(p->group_address,y<<8,1);
			if(p->Button_Type == RGB_Button)
			{
				//Group_Address[0] = p->group_address1;
				KNX_Device_Control_Queue(p->group_address1,y<<8,1);
				//Group_Address[1] = p->group_address2;
				KNX_Device_Control_Queue(p->group_address2,y<<8,1);
				//RGB_Time_Cnt = 10;
				if(y>0)
					RGB_Light = 1;//for open the RGB light, default is 1
				else
					RGB_Light = 48;//for close the RGB light
				//
				if(ExitMenu != NULL)
				{
					//save event 	
					Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
					Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((p->Protocol_Type & 0x0f) + 0x50); //set data length
					Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0xdf; //set data length bit and scnario bit and clear RGB bit
					Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = (((room<<4) + j)<<8) + x;					
					Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = RGB_Light;				
				}
				

			}
			
		}

	
	
}


void abc(uchar a,unsigned int b,uchar c) 
{
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((a & 0x0f) + 0x10); //set data length
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x9f; //set scnario bit and clear RGB bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = b;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = c;	

}
	
void HomeHA_RoomDimmerButton_Press(uchar room ,uchar index,uchar key, uchar key1)
{
	//for dimmer group address is the switch address and the address1 is the dimmer group address
	
	OutPutButtons *p;
	uchar i,y;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));
	i = room*2+index;
	
	//outputrelays(OutputButton_List[index*8 + key].DeviceID,y);
	SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, key+20 , 0, 0x00, 105);
			//temp1 = (0x01<<key);
	LightStatus[i] |= (0x01<<key); //set
	
		y = Dimmer_Value[key1];

		if(p->Protocol_Type==0)
			KNX_Device_Control_Queue(p->group_address1,y<<8,1);
		else if(p->Protocol_Type==1)
			KNX_Device_Control_Queue(p->group_address1,(y*255/100)<<8,1);

	if(ExitMenu != NULL)
	{
		//save event 		
		/*Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((p->Protocol_Type & 0x0f) + 0x10); //set data length
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x9f; //set scnario bit and clear RGB bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = y;	
		*/
		abc(p->Protocol_Type,p->group_address,y);
	}
}

void HomeHA_RoomRGBButton_Press(uchar room ,uchar index,uchar key, uchar key1)
{
	
	OutPutButtons *p;
	//uchar i;
	//RGB_Para y;
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));
	
	
		RGB_Light = key1;

		KNX_Device_Control_Queue(p->group_address,RGB_Value[RGB_Light].R<<8,1);
		KNX_Device_Control_Queue(p->group_address1,RGB_Value[RGB_Light].G<<8,1);
		KNX_Device_Control_Queue(p->group_address2,RGB_Value[RGB_Light].B<<8,1);

	if(ExitMenu != NULL)
	{
		//save event 		
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((p->Protocol_Type & 0x0f) + 0x50); //set RGB bit
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0xdf; //set data length bit and scnario bit 
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = (((room<<4) + index)<<8) + key ;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = RGB_Light;				
	}
}

//---------------------------------------------------------------------------------
void HomeHA_ShutterButton_Press(uchar room ,uchar index,uchar key, uchar key1)
{
	
	OutPutButtons *p;
	uchar y;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));

	if(key1 == K_UP)
		y = 0;
	else if(key1 == K_Down)
		y = 1;
	else 
		y = 2;
							

		if(y == 2)
		{
			y = 1;
			KNX_Device_Control_Queue(p->group_address1,y<<8,0);//stop address with value 1
		}
		else
			KNX_Device_Control_Queue(p->group_address,y<<8,0);//up / down address with value 0:up and 1: down

	if(ExitMenu != NULL)
	{
		//save event 	
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= (p->Protocol_Type & 0x0f) ; 
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x8f; //clear scnario bit and clear RGB bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = y;				
	}
}


void HomeHA_ShutterAdjustButton_Press(uchar room ,uchar index,uchar key, uchar Value)
{
	
	OutPutButtons *p;
	//uchar y;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));
	

		
		KNX_Device_Control_Queue(p->group_address3,Value<<8,1);

	if(ExitMenu != NULL)
	{
		//save event 	
		/*Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((p->Protocol_Type & 0x0f) + 0x10); 
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x9f; //set scnario bit and clear RGB bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address3;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = Value;	
		*/
		abc(p->Protocol_Type,p->group_address3,Value);
	}
	
}

void HomeHA_ShutterPositionButton_Press(uchar room ,uchar index,uchar key, uchar Value)
{
	
	OutPutButtons *p;
	//uchar y;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));
	

		
	KNX_Device_Control_Queue(p->group_address2,Value<<8,1);   //light addr

	if(ExitMenu != NULL)
	{
		//save event 	
		/*Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((p->Protocol_Type & 0x0f) + 0x10); 
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x9f; //set scnario bit and clear RGB bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address2;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = Value;	
		*/
		abc(p->Protocol_Type,p->group_address2,Value);
	}
}

void HomeHA_ShutterPositionButton_Press_Right(uchar room ,uchar index,uchar key, uchar Value)
{//this function is for 
	
	OutPutButtons *p;
	//uchar y;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));
	

		KNX_Device_Control_Queue(p->group_address3,Value<<8,1);  //right addr

	if(ExitMenu != NULL)
	{
		//save event 	
		/*Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= ((p->Protocol_Type & 0x0f) + 0x10); 
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &= 0x9f; //set scnario bit and clear RGB bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = p->group_address3;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = Value;	
		*/
		abc(p->Protocol_Type,p->group_address3,Value);
	}
}



void HomeHA_ACButton_Press(uchar room ,uchar index,uchar key, uchar key1, uchar Value)
{
	
	OutPutButtons *p;
	//uchar y;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ index*168 + key*sizeof(OutPutButtons));
	
	//no need test protocol, we only with KNX A/C interface
	if(p->Button_Type == HVAC_Button && (key1 == HVAC_CoolWind_Speed ||key1 == HVAC_HeatWind_Speed))
	{
		KNX_Device_Control_Queue(p->group_address+key1,Value,1);
	}
	else if(p->Button_Type == HVAC_Button && key1 == HVAC_Temperatire_Setting_Update)
	{
		KNX_Device_Control_Queue(p->group_address+key1,FloatToInt(Value * 100),2);
		A_CTemperature[room] = Value;
	}
	else if(p->group_address>0 && (p->group_address&0x00ff) < 230)
	{
		KNX_Device_Control_Queue(p->group_address+key1,Value<<8,0);
	}
	if(p->Button_Type == HVAC_Button)
	{
		if(key1 == HVAC_Heat_Cool)
		{
			Group_Address[0] = p->group_address;//load the address for  call back funtion
			if(Value == ON)
			{
				//for heat, we need open the heat channel and close the cool channel
				JDFunctionCallBack = &OpenHeatChannel;
				JDFunctionCallBack1 = &CloseCoolChannel;
			}
			else
			{
				//open the cool channel and close the heat channel
				JDFunctionCallBack = &CloseHeatChannel;
				JDFunctionCallBack1 = &OpenCoolChannel;
			}
		}
	}
	if(ExitMenu != NULL)
	{
		//save event 		
		//Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = (p->Protocol_Type & 0x0f) + 0x80; //set as AC bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = (room<<12) + (index<<11) + (key<<8) + key1;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = Value;				
	}
	
}
/*

*/

void HA_Button_Init_CallBack(void)
{
	//SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, K_OK, 0, 0x00, 42);
	LoadJD_Menu(HomeAutomationDevice_List1);
	HA_Button_Name();
	HomeHA_RoomTitle_Init(RoomSelect);
}



void HA_Button_Init_CallBack_Filter(void)
{
	//SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, K_OK, 0, 0x00, 42);
	LoadJD_Menu(Buttonlist_Menu_ID);
	//HA_Button_Name_Filter();
	HomeHA_RoomTitle_Init(RoomSelect);
	//LoadJD_Menu(Buttonlist_Menu_ID);
}

void TimerEvent_Edit_Init(void)
{
	TimerEventSetting_Init(Timer_Event_Index);
	PageStatus = Timer_Event_Edit_Parameter;
	LoadJD_Menu(Timer_Event_Edit_Parameter);
}

void TimerEvent_Edit_Init_Back(void)
{
	//TimerEventSetting_Init(Timer_Event_Index);
	PageStatus = Timer_Event_Edit_Parameter;
	LoadJD_Menu(Timer_Event_Edit_Parameter);
}

//------------------------------------------------------------
void KNX_Device_Control(unsigned int command, unsigned int dest_addr, unsigned int value, uchar length)
{
	CMDPKT *p;
	uchar i;
	unsigned int sum;
	//KNX_Buf[0] = 0x85;
	//Max3107Write(0x0b, 1, KNX_Buf);
	//if(dest_addr)
	//{
		p = (CMDPKT *)&KNX_Buf_TX;
		p->Head1 = 0x55;
		p->Head2 = 0xaa;
		p->MachineID = 0; //default
		p->Reserved = 0;
		p->Command = command;
		p->config = 0x03;
		p->length = 1+length;
		p->Dest_Address = dest_addr;
		p->InParam = value;
		sum = 0;
		for(i = 0; i<14;i++)
		{
			sum += KNX_Buf_TX[i];
		}
		p->ChkSum = ((sum&0x00ff)<<8) + ((sum&0xff00)>>8);
		
		//Max3107Write(0, 16, KNX_Buf);
		//Max3107State = buff_check;
	//}
}

void KNX_Device_Control_Queue(unsigned int dest_addr, unsigned int value, uchar length)
{
	KNX_Q_Buffer[KNX_Save_Index].Group_Address = dest_addr;
	KNX_Q_Buffer[KNX_Save_Index].Value = value;
	KNX_Q_Buffer[KNX_Save_Index].Length = length;
	KNX_Save_Index++;
	if(KNX_Save_Index >= MaxKNX_TXQueue)
	{
		KNX_Save_Index = 0;
	}
}

void Rx_KNX_Verify(void)
{
	CMDPKT_Status *p;
	//unsigned int sum;
	//uchar i;
	unsigned int B_temp;
	if(KNX_R_len == 14)// || KNX_R_len == 15) //comment only for switch status
	{
		p = (CMDPKT_Status *)&KNX_RxBuf;		
			//verify OK
		if(p->Head1 == 0xA5 && p->Head2 == 0x5A && p->nlength == 1)
		{
			//update the status
			if(IsEn_Dis_Alarm(p->Dest_ADDR,p->Data[0]))
			{
				return;
			}
			temp7 = IsMydvice_ID(p->Dest_ADDR);
			B_temp = temp7;
			if(temp7)
			{
				
				if (p->Data[0])//update status
				{	
					//SetControl_State(AlarmStatus_Menu, temp7+30, ON);
					SetSwitchIcon(ChangePhoto_Icon, AlarmStatus_Menu, 30+temp7, 0, 0x00, 219);
					AlarmKnowsFlag &= ~(0x0001<<(B_temp - 1));//clear
					AlarmNeed_Reset_Flag &= ~(1<<(B_temp - 1));//clear
					if(!AlarmKnowsFlag)
						SetControl_State(MainMenu, 7, OFF);// hide the icon
					
				}
				else 
				{
					//SetControl_State(AlarmStatus_Menu, temp7+27, OFF);
					SetSwitchIcon(ChangePhoto_Icon, AlarmStatus_Menu, 30+temp7, 0, 0x00, 218);//for status page show
					//scan the alarm event
					ScanAlarmEvent(temp7); 
					AlarmKnowsFlag |= (0x0001<<(B_temp-1));//set
					SetControl_State(MainMenu, 7, ON);// show the icon
					
				}
				
					
			}
			else
			{
				IsMyUpdataStatus(p->Dest_ADDR,p->Data[0]);		
			}
		}
		//}
	}
	else if(KNX_R_len == 15)
	{
		//posible AC temperature value
		p = (CMDPKT_Status *)&KNX_RxBuf;
		if(p->Head1 == 0xA5 && p->Head2 == 0x5A && p->nlength == 3)
		{
			//update the status
			
			IsMyACStatus(p->Dest_ADDR,p->Data[0],p->Data[1]);					
		}
	}
}

void IsMyUpdataStatus(unsigned int g_address, uchar value)
{
	// it need check it is HRB protocol
	OutPutButtons *p;
	uchar i,j,k;
	//unsigned int T_address;
	//T_address = g_address;
	for(k = 0; k < 12; k++)
	{
		for(j = 0; j < 2; j++)
		{
			p = (OutPutButtons *)(EE_HomeAutomation1_1+(k*0x200) + j*168);
			
			for(i = 0; i < MaxObject_In_OneMenu; i++)
			{
				if(p->Button_Type == HVAC_Button && p->group_address + HVAC_Heat_Cool == g_address)
				{
					//HVAC Switch COOL and HEAT
					if(value)
						AC_Status[k].bits.C_H = 2; //heat mode
					else
						AC_Status[k].bits.C_H = 1; //cool mode
				}
				else if(p->Button_Type == HVAC_Button && p->group_address + HVAC_Power_ON_Off == g_address)
				{
					if(value > 0)
					 {
					 	LightStatus[k*2 + j] |= (0x01<<i); //set
					 }
					 else
					 {
					 	LightStatus[k*2 + j] &= ~(0x01<<i);//clear
					 }
				}
				else if(p->Button_Status == g_address && (p->Button_Type < Shutter_Button || p->Button_Type > AC_Button))
				{
					 if(value > 0)
					 {
					 	LightStatus[k*2 + j] |= (0x01<<i); //set
					 }
					 else
					 {
					 	LightStatus[k*2 + j] &= ~(0x01<<i);//clear
					 }

					//update icon if the menu is right match
					if( PageStatus == HomeAutomationDevice_List1 && RoomSelect == k && Button_Index == j)
					{
						UpdataStatusHomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID,j,i);
					}
					
				}
				
				p++;
			}
		}
	}
	//return 0;
}

void IsMyACStatus(unsigned int g_address, uchar value, uchar value1)
{
	// it need check it is HRB protocol
	OutPutButtons *p;
	uchar i,j,k;
	uchar temp_AC;
	//unsigned int T_address;
	//T_address = g_address;
	for(k = 0; k < 12; k++)
	{
		for(j = 0; j < 2; j++)
		{
			p = (OutPutButtons *)(EE_HomeAutomation1_1+(k*0x200) + j*168);
			
			for(i = 0; i < MaxObject_In_OneMenu; i++)
			{
				if(p->Button_Type == HVAC_Button || p->Button_Type == AC_Button)
				{
					 if(p->group_address == g_address)
					 {
					 	//set the value
					 	temp_AC = IntToFloat( (value<<8) + value1);
						if(temp_AC < 41 && temp_AC > 4)
					 		A_CTemperature[k] = temp_AC;
						//return;
					 }
					 else if(p->Button_Type == HVAC_Button && p->group_address + HVAC_Temperature_Sensor == g_address)
					 {
					 	AC_Temperature_From_Sensor[k] = IntToFloat( (value<<8) + value1);
						//check if need call back funtion to handle the temperature changed
						if(LightStatus[k*2 + j] & (0x01<<i))
						{
							//it is power on
							if(AC_Status[k].bits.C_H == 1)
							{
								//setting is cool mode
								if(A_CTemperature[k] > AC_Temperature_From_Sensor[k])
								{
									//need open cool valve
									KNX_Device_Control_Queue(p->group_address + HVAC_Cool_Command,ON<<8,0);
								}
								else
								{
									//need close cool valve
									KNX_Device_Control_Queue(p->group_address + HVAC_Cool_Command,OFF,0);
								}
							}
							else
							{ // for heat mode
								if(A_CTemperature[k] > AC_Temperature_From_Sensor[k])
								{
									//need close heat valve
									KNX_Device_Control_Queue(p->group_address + HVAC_Heat_Command,OFF,0);
								}
								else
								{
									KNX_Device_Control_Queue(p->group_address + HVAC_Heat_Command,ON<<8,0);
								}
							}
						}
					 }
				}
				p++;
			}
		}
	}
}


//-----------------------------------------------------------------------------	
// Update led accordingly to the priority of the active conditions

	
	
void ScenarioOutput(uchar ch, uchar p)
{
	Scenario_Setting *pp;
	//uchar X;
	if(p)
		pp = (Scenario_Setting *) (EE_Scenario_CCTV+(ch  + 9)*sizeof(Scenario_Setting));
	else
		pp = (Scenario_Setting *) (EE_Scenario_CCTV+ch * sizeof(Scenario_Setting));
	//X = pp->Icon_Protocol_Type & 0x0f;
	//if(X == 1)
	//{
		//knx
		if(pp->group_address)
		{
			//X = ;
			KNX_Device_Control_Queue(pp->group_address, (pp->S_ID << 8), 1);
			
		}
	//}
	/*
	else if(X == 0)
	{
		//HRB
		outputrelays(pp->group_address,pp->S_ID);
	}
	
	else
	{
		//both
		//knx
		if(pp->S_ID && pp->group_address)
		{
			//X = ;
			KNX_Device_Control(0x501, pp->group_address, ((pp->S_ID -1)<<8), 1);
			
		}

		//HRB
		//outputrelays(pp->group_address,pp->S_ID);
	}
	*/
	if(ExitMenu != NULL)
	{
		//save event 
		
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = (pp->Icon_Protocol_Type & 0x0f);
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] &=0xbf;//clear RGB bit
		Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] |= 0x30; //set data length bit and scnario bit
		Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = pp->group_address;
		Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = pp->S_ID;
	}
}

void CheckMenu(uchar index)
{
	#ifdef Hide_Button_Solution	
	Buttonlist_Menu_ID = HomeAutomationDevice_List1;
	if(index)
	{
		Need_Show_buttons = Object_Button_Same_Cnt -8; 
	}
	else
	{
		if(Object_Button_Same_Cnt > 8)
			Need_Show_buttons = 8;
		else
			Need_Show_buttons = Object_Button_Same_Cnt;
	}
	//JDFunctionCallBack1 = &Object_Button_Visible;
	Object_Button_Visible();
	#else
	if(index)
	{
		Buttonlist_Menu_ID = HomeAutomationDevice_List_S1 + Object_Button_Same_Cnt -9; 
	}
	else
	{
		if(Object_Button_Same_Cnt > 8)
			Buttonlist_Menu_ID = HomeAutomationDevice_List_S1 + Object_Button_Same_Cnt -9; 
		else
			Buttonlist_Menu_ID = HomeAutomationDevice_List_S1 + Object_Button_Same_Cnt -1; 
	}
	#endif
}


void HA_Menu_Ini(void)
{
	Edit_Index = 0;	
	RoomIndex = 0;					
	// scenario button name init
	//room button name init
	JDFunctionCallBack = &RoomType_Init_Back;
	PageStatus = Home_Automation;///demo
	LoadJD_Menu(Home_Automation);
	ScenarioMenu_Init(Edit_Index,1);
}
//--------------------------------------------------------------------------------------
void CCTVOutput(uchar ch)
{
	CCTV_Setting *pp;	
	//pp = (CCTV_Setting *) (EE_Scenario_CCTV+9*sizeof(Scenario_Setting)+ ch*sizeof(CCTV_Setting));
	pp = (CCTV_Setting *)(EE_CCTV_Room + ch*sizeof(CCTV_Setting));

		
	if(pp->group_address)
	{				
		KNX_Device_Control_Queue(pp->group_address, (pp->S_ID << 8), 1);
	}						
}

//-------------------------------------------------
/*
RS485 TX managment
*/
void KNX_TX_Managment(void)
{
	unsigned int CheckSum = 0;
	//uchar i;
	if(SCI2S1_TC && !SCI2C2_TIE)
	{
		//if(Pres_BT)
		//{
			Con485_D = RS485_TX_Idle;
		//	Con485_R = 0;
		//}
		//else
		//{
			//Con485_D = RS485_TX_Ready;
		//	Con485_R = 1;
		//}
	}
	if(Pres_BT)
	{
		if(KNX_Save_Index != KNX_TX_Index && !SCI2C2_TIE && !KNX_TX_Frame_Time)
		{
			if(KNX_Q_Buffer[KNX_TX_Index].Group_Address && KNX_Q_Buffer[KNX_TX_Index].Group_Address != 0xffff)
			{
				//address is valid
				Con485_D = RS485_TX_Ready;
				KNX_Device_Control(0x501, KNX_Q_Buffer[KNX_TX_Index].Group_Address, KNX_Q_Buffer[KNX_TX_Index].Value, KNX_Q_Buffer[KNX_TX_Index].Length);
				KNX_R_TX_P = 0; //send data from first buffer
				
				KNX_Buf_TX_Length = 16;
				//Flag4.bits.Bus_Useing = 1;
				SCI2C2_TIE = 1; //enable tx and start transmit data
			}
			KNX_TX_Index++;
			if(KNX_TX_Index >= MaxKNX_TXQueue)
			{
				KNX_TX_Index = 0;
			}
							
		}
	}
}

void KNX_RC_Managment(void)
{
	
	if(Pres_BT)
	{
		if((KNX_R_RC_P == 14 ||KNX_R_RC_P == 15) && KNX_R_RC_Verify > 1)// && KNX_R_RC_Verify > 2// && KNX_Buf_RC[KNX_R_RC_P - 1] == 0x5A)
		{
			//possible need check the frame	
			KNX_R_len = KNX_R_RC_P;
			(void)memcpy(&KNX_RxBuf,&KNX_Buf_RC,KNX_R_len);
			//clear rc buffer
			DisableInterrupts;
			KNX_R_RC_P = 0;
			(void)memset(&KNX_Buf_RC,0,sizeof(KNX_Buf_RC));
			EnableInterrupts;
			Rx_KNX_Verify();
			(void)memset(&KNX_RxBuf,0,sizeof(KNX_RxBuf));
			KNX_R_len = 0;
			KNX_R_RC_Verify = 0;
		}
		else if(KNX_R_RC_P>0)
		{
			//count we have data on the buffer
			DisableInterrupts;
			KNX_R_RC_Verify++;
			
			if(KNX_R_RC_Verify > 3)
			{
				KNX_R_RC_Verify = 0;
				//receive time out
				//DisableInterrupts;
				KNX_R_RC_P = 0;
				//EnableInterrupts;
			}
			EnableInterrupts;
		}
		
	}
	else
	{
		//progrom mode
		
	}
}


void StopPlayMelody(void)
{
	if(!SettingPara.S_Flag.bits.BeepVolume)
	{
		Stop_tone();		
	}
}



uchar Number_T(uchar p)
{
	if(p == 10)
		return 0x30;
	else if(p == 0x0b)
		return '*';
	else if(p == 0x0c)
		return '#';
	else
		return (p+0x30);
}
void Telephone_Number_Push(void)
{
	//(void)memcpy(&KNX_Buf,&Tel_num,Tel_index);
	uchar i;
	if(++Tel_Log_Index>4)
	{
		Tel_Log_Index = 0;
	}
	for(i = 0; i< Tel_index; i++)
	{
		Memory_S.Tel_Log_Number[Tel_Log_Index].Tel_Number[i] = Number_T(Tel_num[i]);
	}
	//(void)memcpy(&Tel_Log_Number[Tel_Log_Index].Tel_Number[0],&Tel_num,Tel_index);
	Memory_S.Tel_Log_Number[Tel_Log_Index].Tel_Num_Len = Tel_index;
	
}


//------------------------------------------------------------------
void Tel_HookOFF_Do_Video(uchar state_H)
{
	if(Sub_MODO_State == PSTN_Calling_Video)
	{
		Sub_MODO_State = PSTN_Talking_Video;
		OLD_Sub_Modo_State = PSTN_Talking_Video;
		//change icon for hook off
		//SetButton_State(0x01, PSTN_Talking_Video, K_AnswerCall, ON); 
		DTMF_State = Tel_Talk;	
		StopPlayMelody();
		PCM_Audio_Switch = state_H;
		if(state_H == To_Modo)
		{
			SetSwitchIcon(ChangeButton_Icon, Menu_Telephone_Video, K_OK, 0, 0x00, 45);
		}
		
		Tel_index=16;
		(void)memset(&Tel_num,0,sizeof(Tel_num));
		ShowTelNum();
		Tel_index = 0;
	}
	else if(DTMF_State == Tel_Talk)
	{
		if(state_H == To_Modo && PCM_Audio_Switch == To_HandSet)
		{
			PCM_Audio_Switch = To_Modo;
			SetSwitchIcon(ChangeButton_Icon, Menu_Telephone_Video, K_OK, 0, 0x00, 45);
		}
		else
		{
			DTMF_State = DTMF_Idle;
			Sub_MODO_State = EX_IDLE;
			OLD_Sub_Modo_State = EX_IDLE;
			PageStatus = MainMenu;
			LoadJD_Menu(MainMenu);
		}
		
	}
}

void Tel_HookOFF_Do(uchar state_H)
{
	if(Sub_MODO_State == PSTN_Calling)
	{
		Sub_MODO_State = PSTN_Talking;
		//ClearTelDispFlag=1;
		DTMF_State = Tel_Talk;
		CheckDTMF_Delay = 3;
		//switch icon
		//SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
		StopPlayMelody();
		PCM_Audio_Switch = state_H;
		if(state_H == To_Modo)
		{
			SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
		}

		Tel_index=16;
		(void)memset(&Tel_num,0,sizeof(Tel_num));
		ShowTelNum();
		Tel_index = 0;
		
	}
	else if(DTMF_State == Tel_Talk)
	{
		if(state_H == To_Modo && PCM_Audio_Switch == To_HandSet)
		{
			PCM_Audio_Switch = To_Modo;
			SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
		}
		else
		{
			DTMF_State = DTMF_Idle;
			Sub_MODO_State = EX_IDLE;
			if(PCM_Audio_Switch == To_Modo)
				OLD_Sub_Modo_State = EX_IDLE;
			
			SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB0);
			SetButton_State(0x01, PSTN_Telephone, 16, OFF);
			if(Tel_index>0)
			{
				(void)memset(&Tel_num,0,sizeof(Tel_num));
				ShowTelNum();
			}
			Tel_index = 0;
		}
	}
	else if(Tel_index>0)
	{
		(void)memset(&KNX_Buf,0xff,sizeof(KNX_Buf));
		(void)memcpy(&KNX_Buf,&Tel_num,Tel_index);
		CheckDTMF_Delay = 3;
		DTMF_State = DTMF_Ready;
		Telephone_Number_Push();//save it as log
		SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
	}
}


//-----------------------------------------------------------------
void DTMF_Key_Out_Put(void)
{
	DTMF_Key *p;
	
	p = (DTMF_Key *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) +3 + ZoneNum*sizeof(ZoneNameS) + 4*sizeof(Tel_Memory_S));
	if(p->lenth)
	{
		//KNX_Buf[0]
		(void)memset(&KNX_Buf,0xff,sizeof(KNX_Buf));
		(void)memcpy(&KNX_Buf,&p->D_key[0],p->lenth);
		DTMF_State = DTMF_Ready;
	}
}

