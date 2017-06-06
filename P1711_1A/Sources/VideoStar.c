#include <hidef.h>                                /* for EnableInterrupts macro */
#include "derivative.h"                           /* include peripheral declarations */
#include <string.h>
#include  "define.h"
#include "Global.h"
#include "spi.h"
#include "Eeprom.h"
#include "Trame.h"
#include "VideoStar.h"
#include "HomeAutoMation.h"
#define BEEPON 1


extern uchar MaxValue5(uchar level);
void BeepOK(void);

//extern void ReadRecordData(unsigned int _DstPage,unsigned char *_buf,unsigned int _index,unsigned char _len);
extern void SaveInfQueue(void (*finProc)());
//extern void Intercom_Function(unsigned char _type,unsigned char _address);
//extern void HexToAsg(uchar *p,uchar q);
//extern unsigned char AddressNO[4];

/*
*********************************************
receive frame from JD module and check whether a new key press
if yes, set new key 
*********************************************
*/
extern uchar IfBelongType(uchar type, uchar Filter_Type);
//extern const Room_Setting F_RoomType[12];
void JD_TX_Managment(void)
{
	unsigned int CheckSum = 0;
	uchar i;
	if(JD_Save_Frame_Index != JD_TX_Frame_Index && !SCI1C2_TIE && !JD_TX_Frame_Time && !JD_start_wait)
	{
		(void)memcpy(&tx_buf_Program[3], &JD_TX_Queue[JD_TX_Frame_Index].Function_ID,JD_TX_Queue[JD_TX_Frame_Index].Length+4);
		//fill checksum and end byte
		tx_buf_Program[0] = 0x7e;
		//Board ID
		tx_buf_Program[1] = 0;
		tx_buf_Program[2] = 0;
		//fill checksum and end byte
		for(i = 0; i<JD_TX_Queue[JD_TX_Frame_Index].Length+7; i++)
		{
			CheckSum += tx_buf_Program[i];
		}
		CheckSum &=0x007e;
		tx_buf_Program[i++] = CheckSum&0x00ff;
		tx_buf_Program[i++] = 0x55;
		tx_buf_Program[i++] = 0xaa;
		tx_len_JD = JD_TX_Queue[JD_TX_Frame_Index].Length+10;
		index_buffer_tx_JD = 0;
		SCI1C2_TIE = 1; //enable tx and start transmit data

		KeyLimitDly=2;
						
	}
	
}
void InPutQuere(void)
{
	JD_Save_Frame_Index++;
	if(JD_Save_Frame_Index == MaxVideoStarQueue)
	{
		JD_Save_Frame_Index = 0;
	}
}
void LoadJD_Menu(uchar subitemsel)
{
	//unsigned char i;
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	if(SystemStatus.bits.CallSwitchBoard_Icon)
	{
		SetButton_State(0x01, MainMenu, K_CallSwitchBoard, OFF);
        	SystemStatus.bits.CallSwitchBoard_Icon = 0;
	}
	
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = Switch_Menu_JD;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 1;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = subitemsel;//MainMenu;
	
	InPutQuere();
	/*
	if (FlagAlFlashMenu) {
		//SetControl_State(subitemsel, ALARM_BLINK, ON);
		SetIconBlink(Set_Button_JD, PageStatus, 1, ALARM_BLINK, 1);		
		SetControl_State(FlagAlFlashMenu, ALARM_BLINK, OFF);	
		FlagAlFlashMenu = subitemsel;
	}	
	*/
	if(subitemsel == WaitWithShortCut)
	{
		ScenarioShortcut_Init();
	}
}

void LoadJD_SleepMenu(uchar subitemsel)
{
	if(subitemsel == OFF)
	{
		if(PageStatus > WaitWithShortCut)
			return; //ignor the command due to we are not waitpage
	}
	if(subitemsel == ON)//set the flag when we need the LCD wake up
		SwitchedOnOff = 1;
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = 0x0013;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 1;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = subitemsel;//MainMenu;
	
	InPutQuere();
	
}

void SetSwitchIcon(uchar subitemsel,uchar data1, uchar data2,uchar data3,uchar data4,uchar data5)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = subitemsel;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 5;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = data1;//MainMenu;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = data2;
	JD_TX_Queue[JD_Save_Frame_Index].data[2] = data3;
	JD_TX_Queue[JD_Save_Frame_Index].data[3] = data4;
	JD_TX_Queue[JD_Save_Frame_Index].data[4] = data5;
	InPutQuere();
}

/*void sendJDdata(uchar JDcmd,uchar JDpage,uchar JDlen, uchar *p)
{
	uchar i;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = JDcmd;
	JD_TX_Queue[JD_Save_Frame_Index].Length = JDlen;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = JDpage;//MainMenu;
	for(i=0;i<JDlen;i++)
		{
			;//JD_TX_Queue[JD_Save_Frame_Index].data[1+i]=*(p+i);

		}
	InPutQuere();
}*/

void SetButton_State( uchar data1, uchar data2,uchar data3,uchar data4)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = SetButtonState;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 4;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = data1;//MainMenu;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = data2;
	JD_TX_Queue[JD_Save_Frame_Index].data[2] = data3;
	JD_TX_Queue[JD_Save_Frame_Index].data[3] = data4;
	InPutQuere();
}
//--------------------------------------------------------------------------------------------------
void SetControl_State( uchar data1, uchar data2,uchar data3)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = Control_State;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 3;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = data1;//MainMenu;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = data2;
	JD_TX_Queue[JD_Save_Frame_Index].data[2] = data3;
	InPutQuere();
}
//----------------------------------------------------------------------------------------------


void SetButton_Name(uchar Menu_ID, uchar Control_ID,uchar Num_Len, uchar *T_num)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = 0x001E;
	JD_TX_Queue[JD_Save_Frame_Index].Length = Num_Len + 2;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = Menu_ID;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = Control_ID;
	if(Num_Len == 0 || Num_Len > 17)
	{
		JD_TX_Queue[JD_Save_Frame_Index].Length = 3;
		JD_TX_Queue[JD_Save_Frame_Index].data[2] = 0x20;
	}
	else
	{
		(void)memcpy(&JD_TX_Queue[JD_Save_Frame_Index].data[2],T_num,Num_Len);
	}
		
	InPutQuere();
}

void SetTel_Num(uchar Menu_ID, uchar Control_ID,uchar Num_Len, uchar *T_num)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = 0x001A;
	JD_TX_Queue[JD_Save_Frame_Index].Length = Num_Len + 2;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = Menu_ID;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = Control_ID;
	if(Num_Len == 0 || Num_Len > 17)
	{
		JD_TX_Queue[JD_Save_Frame_Index].Length = 3;
		JD_TX_Queue[JD_Save_Frame_Index].data[2] = 0x20;
	}
	else
	{
		(void)memcpy(&JD_TX_Queue[JD_Save_Frame_Index].data[2],T_num,Num_Len);
	}
	
	InPutQuere();
}
//---------------------------------------------------------------------

//-------------------------------------------------------------------------
void CheckCommand(unsigned int p)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = p;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 0;
	//JD_TX_Queue[JD_Save_Frame_Index].data[0] = 0;//MainMenu;
	
	InPutQuere();
}
//------------------------------------------------------------------------------

void Set_Oclock( uchar data1, uchar data2,uchar data3,uchar data4, uchar data5, uchar data6)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = SetJD_oclock;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 6;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = data1;//MainMenu;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = data2;
	JD_TX_Queue[JD_Save_Frame_Index].data[2] = data3;
	JD_TX_Queue[JD_Save_Frame_Index].data[3] = data4;
	JD_TX_Queue[JD_Save_Frame_Index].data[4] = data5;
	JD_TX_Queue[JD_Save_Frame_Index].data[5] = data6;
	InPutQuere();
}


void Set_Date( uchar data1, uchar data2,uchar data3,uchar data4, uchar data5, uchar data6, uchar data7, uchar data8)
{
	//JD_TX_Queue[JD_Save_Frame_Index].Start_JD = 0x7e;
	//JD_TX_Queue[JD_Save_Frame_Index].Board_ID = 0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = SetJD_Date;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 8;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] = data1;//MainMenu;
	JD_TX_Queue[JD_Save_Frame_Index].data[1] = data2;
	JD_TX_Queue[JD_Save_Frame_Index].data[2] = data3;
	JD_TX_Queue[JD_Save_Frame_Index].data[3] = data4;
	JD_TX_Queue[JD_Save_Frame_Index].data[4] = data5;
	JD_TX_Queue[JD_Save_Frame_Index].data[5] = data6;
	JD_TX_Queue[JD_Save_Frame_Index].data[6] = data7;
	JD_TX_Queue[JD_Save_Frame_Index].data[7] = data8;
	InPutQuere();
}



//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------
display the telephone number on the area
----------------------------------------------------------------------------------*/
void ShowTelNum(void)
{
	uchar i;
	for(i = 0; i < 16;i++)
	{
		if(Tel_num[i] == 0)
		{
			SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+i, 0, 0x00, 26);
		}
		else if(Tel_num[i] == 0x0c)
		{
			SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+i, 0, 0x00, 25);
		}
		else if(Tel_num[i] == 0x0b)
		{
			SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+i, 0, 0x00, 22);
		}
		else
		{
			SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+i, 0, 0x00, 10 + Tel_num[i]);
		}
	}
}
//--------------------------------------------------------------------------------
void AlarmStatus_Name_Init(uchar menuid)
{
	InPutAlarm *p;
	uchar i;
	for(i = 0; i < HomeAutomation_MaxInput; i++)
	{
		p = (InPutAlarm *)(EE_Input_Memory1 + (i*sizeof(InPutAlarm)) );
		SetTel_Num(menuid,i+9,p->Input_Name_Len,p->Input_Name);
	}
	// for the first record
	//p = (InPutAlarm *)(EE_Input_Memory1);
	//SetTel_Num(menuid,9,p->Input_Name_Len,p->Input_Name);
}

void AlarmSystemZoneName_Init(void)
{
	ZoneNameS *p;
	uchar i;
	p = (ZoneNameS *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem));
	for(i = 0; i < ZoneNum;i++)
	{
		SetButton_Name(AlarmSystem_Manage,11+i,p->lenth,p->name);
		/*
		if(!i)
		{
			SetButton_Name(AlarmSystem_Manage,11+i,p->lenth,p->name);
		}
		*/
		p++;
	}
	Tel_index = 0;
}


void Tel_MemoryName_Init(void)
{
	
	Tel_Memory_S *p;
	uchar i;
	p = (Tel_Memory_S *)(EE_Page_AlarmSystem+SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem)  + 3 + (ZoneNum*sizeof(ZoneNameS)));
	for(i = 0; i < Tel_Memory_Num;i++)
	{
		SetButton_Name(PSTN_Telephone,31+i,p->lenth,p->name);
		/*
		if(!i)
		{
			SetButton_Name(PSTN_Telephone,31+i,p->lenth,p->name);
		}
		*/
		p++;
	}
	
}
// RoomType Name Initialize


// scenario
//--------------------------------------------------------------------------------
void HA_Button_Name_Init(uchar menuid,uchar ch, uchar room)
{
	OutPutButtons *p;
	if(menuid)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200) +168 + ch*sizeof(OutPutButtons) );
	}
	else
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ ch*sizeof(OutPutButtons) );
	if(p->Button_Type)//with type
		SetTel_Num(HomeAutomationDevice_List1,ch+100,p->Button_Name_Len,p->Button_Name);
	else
		SetTel_Num(HomeAutomationDevice_List1,ch+100,0,p->Button_Name);
}

void HA_Button_Name_Init_Filter(uchar menuid,uchar ch, uchar room, uchar filter)
{
	OutPutButtons *p;
	if(menuid)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200) + 168 + ch*sizeof(OutPutButtons) );
	}
	else
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ ch*sizeof(OutPutButtons) );
	if(IfBelongType(p->Button_Type,filter))//with type
	{
		SetTel_Num(Buttonlist_Menu_ID,Object_ID+100,p->Button_Name_Len,p->Button_Name);
		Object_ID++;
	}
	
}

void HA_Button_Name(void)
{
	//ch is the room selected
	uchar i;
	for(i = 0; i<8 ; i++)
	{
		HA_Button_Name_Init(Button_Index,i,RoomSelect);
	}
	//HA_Button_Name_Init(Button_Index,0,RoomSelect);
	HomeHA_RoomTitle_Init(RoomSelect);
}

void HA_Button_Name_Filter(void)
{
	/*
	//RoomSelect,Button_Index,Filter_ID
	uchar i;
	Object_ID = 0;
	for(i = 0; i<8 ; i++)
	{
		HA_Button_Name_Init_Filter(Button_Index,i,RoomSelect,filter);
	}
	*/
	OutPutButtons *p;
	uchar i,j,k,m,z = 0;

	m = Button_Index*8;
	for(k = 0; k < 2;k++)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(RoomSelect*0x200)+ k*168);
		j = RoomSelect*HomeAutomation_MaxButtons + k*MaxObject_In_OneMenu;
		
		for(i = 0; i < 8; i++)
		{
			if(IfBelongType(p->Button_Type, Filter_ID) &&  z<8)
			{
				//SetSwitchIcon(ChangeButton_Icon,HomeAutomationDevice_List1,10+i,0,0, 241);//big icon
				//SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, 20+i , 0, 0x00, 242);
				if(!m)
				{
															
					SetTel_Num(Buttonlist_Menu_ID,z+100,p->Button_Name_Len,p->Button_Name);
					if(!z)
						SetTel_Num(Buttonlist_Menu_ID,z+100,p->Button_Name_Len,p->Button_Name);
					z++;
					if(z >= 8)
						return;
				}
				
				if(m)
					m--;
			}
			
			p++;
		}
	}
}
//----------------------------------------------


void RoomType_Init(uchar ch,uchar w_i)
{
	uchar i;
	Room_Setting *pp;
	if(ch)
		pp = (Room_Setting *)(EE_CCTV_Room + 4*sizeof(CCTV_Setting) + 6*sizeof(Room_Setting));
	else
		pp = (Room_Setting *)(EE_CCTV_Room + 4*sizeof(CCTV_Setting));
	for(i = 0; i<6; i++)
	{
		SetButton_Name(Home_Automation,22+i,pp->lenth,pp->name);
		//SetSwitchIcon(ChangeButton_Icon,Home_Automation,22+i,0,1,0x4A+pp->Icon_ID);		
		pp++;
	}
	if(w_i)
	{
		pp -= 6;
		for(i = 0; i<6; i++)
		{
			//SetButton_Name(Home_Automation,22+i,pp->lenth,pp->name);
			SetSwitchIcon(ChangeButton_Icon,Home_Automation,22+i,0,1,0x4A+pp->Icon_ID);		
			pp++;
		}
	}
}

void RoomType_Init_Back(void)
{
	RoomType_Init(RoomIndex,1); 
}

void RoomType_Init_Back0(void)
{
	RoomType_Init(RoomIndex,0); 
}
void ScenarioMenu_Init(uchar ch, uchar w_i)
{
	Scenario_Setting *pp;
	//unsigned int X;
	if(ch)
		pp = (Scenario_Setting *) ((EE_Scenario_CCTV) + 9*sizeof(Scenario_Setting));
	else
		pp = (Scenario_Setting *) (EE_Scenario_CCTV);
	
	for(temp1 = 0; temp1 < 9; temp1++)
	{
		if(pp->lenth < 9)
			SetButton_Name(Home_Automation,5+temp1,pp->lenth,pp->name);
		if(w_i)
		{
			SetSwitchIcon(ChangeButton_Icon,Home_Automation,5+temp1,0,1,0x2e+((pp->Icon_Protocol_Type&0xf0)>>4)-1);
		}
		
		pp++;
	}
	temp1 = 0;
	/*
	pp-=9;
	if(pp->lenth < 9)
		SetButton_Name(Home_Automation,5+temp1,pp->lenth,pp->name);
		*/
	//SetSwitchIcon(ChangeButton_Icon,Home_Automation,5+temp1,0,1,0x2e+((pp->Icon_Protocol_Type&0xf0)>>4)-1);
}



void ScenarioShortcut_Init(void)
{
	Scenario_Setting *pp;
	//unsigned int X;
	
	pp = (Scenario_Setting *) (EE_Scenario_CCTV);
	
	for(temp1 = 0; temp1 < 2; temp1++)
	{
		
		SetSwitchIcon(ChangeButton_Icon,WaitWithShortCut,5+temp1,0,1,0x2e+((pp->Icon_Protocol_Type&0xf0)>>4)-1);
		if(pp->lenth < 9)
			SetButton_Name(WaitWithShortCut,5+temp1,pp->lenth,pp->name);
		pp++;
	}
	/*
	temp1 = 0;
	pp = (Scenario_Setting *) (EE_Scenario_CCTV);
	SetSwitchIcon(ChangeButton_Icon,WaitWithShortCut,5+temp1,0,1,0x2e+((pp->Icon_Protocol_Type&0xf0)>>4)-1);
	*/
}


uchar IfBelongType(uchar type,uchar Filter_Type)
{
	switch(Filter_Type)
	{
		case T_Relay:
			if(type == Relay_Button || type == Valve_Button || type == Valve_Button1 || type == Relay_Button_PC || type == Relay_Button_PO || type == Valve_Button_PC || type == Valve_Button_PO || type == Valve_Button1_PC || type == Valve_Button1_PO)
				return 1;
		break;	
		case T_Dimmer:
			if(type == Dimmer_Button || type == RGB_Button)
				return 1;
		break;	
		case T_Shutter:
			if(type == Shutter_Button || type == Shutter_L_R_Button || type == Shutter_Blind_Button)
				return 1;
		break;		
		case T_AC:
			if(type == HVAC_Button || type == AC_Button)
				return 1;
		break;	
		case T_All_Device:
			if(type != Hide_Icon)
				return 1;
		break;
	}
	return 0;
}
uchar HomeHA_RoomButtonWithFilter_Check(uchar room, uchar Filter)
{
	OutPutButtons *p;
	uchar i,j,samecnt;
	//uchar BackGround1;
	//uchar BackGround2;
	
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200));
	j = room*HomeAutomation_MaxButtons;
	samecnt = 0;
	for(i = 0; i < 16; i++)
	{
		if(IfBelongType(p->Button_Type, Filter))
		{
			samecnt++;
		}	
		p++;
	}
	/*
	p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+168);
	for(i = 0; i < 8; i++)
	{
		if(IfBelongType(p->Button_Type, Filter))
		{
			samecnt++;
		}	
		p++;
	}
	*/
	return samecnt;
}

void HomeHA_RoomButtonWithFilter_Init(uchar room, uchar index, uchar Filter)
{
/*
	OutPutButtons *p;
	uchar i,j,k,m,z = 0;
	m = index*8;
	for(k = 0; k < 2;k++)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ k*168);
		j = room*2 + k;
		
		for(i = 0; i < 8; i++)
		{
			if(IfBelongType(p->Button_Type, Filter) &&  z<8)
			{
				if(!m)
				{
					if(p->Button_Type >= Shutter_Button && p->Button_Type <= Shutter_Blind_Button)
					{
						SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 161 + p->Button_Type);//big icon
						SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 103  + p->Button_Type);
					}
					else if(p->Button_Type == HVAC_Button)
					{
						SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 162);
						SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 120);
					}
					
					else if(p->Button_Type == AC_Button)
					{
						SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 162);
						SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 110);
					}
					
					else
					{
						if(p->Button_Type > 0 && p->Button_Type <RGB_Button+1)
							SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 161 + p->Button_Type);
						else 
							SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 162);

						if(LightStatus[j]&(0x01<<i))
						{
							if(p->Button_Type == Valve_Button || p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button_PO)
							{
								SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 104);
							}
							else if(p->Button_Type == Valve_Button1 || p->Button_Type == Valve_Button1_PC|| p->Button_Type == Valve_Button1_PO)
							{
								SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 102);
							}
							else
								SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 105);
						}
						else
						{
							if(p->Button_Type == Valve_Button || p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button_PO)
							{
								SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 103);
							}
							else if(p->Button_Type == Valve_Button1 || p->Button_Type == Valve_Button1_PC || p->Button_Type == Valve_Button1_PO)
							{
								SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 101);
							}
							else
								SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 106);
						}
					}
					z++;
					if(z >= 8)
						return;
				}
				
				if(m)
					m--;
			}
			
			p++;
		}
	}

*/



	OutPutButtons *p;
	uchar i,j,k,m,z = 0;
	uchar tempA1,tempA2;
	uchar tempB1,tempB2;
	m = index*8;
	for(k = 0; k < 2;k++)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ k*168);
		j = room*2 + k;
		
		for(i = 0; i < 8; i++)
		{
			if(IfBelongType(p->Button_Type, Filter) &&  z<8)
			{
				if(!m)
				{
					if(p->Button_Type >= Shutter_Button && p->Button_Type <= Shutter_Blind_Button)
					{
						tempA1=0;tempA2=161 + p->Button_Type;
						//SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 161 + p->Button_Type);//big icon
						tempB1=0;tempB2=103 + p->Button_Type;
						//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 103  + p->Button_Type);
					}
					else if(p->Button_Type == HVAC_Button)
					{
						tempA1=0;tempA2=162;
						//SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 162);
						tempB1=0;tempB2=120;
						//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 120);
					}
					
					else if(p->Button_Type == AC_Button)
					{
						tempA1=0;tempA2=162;
						//SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 162);
						tempB1=0;tempB2=110;
						//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 110);
					}
					
					else
					{
						if(p->Button_Type > 0 && (p->Button_Type <RGB_Button+1))
						{	
							tempA1=0;tempA2=161 + p->Button_Type;
							//SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 161 + p->Button_Type);
						}
						else 
						{
							tempA1=0;tempA2=162;
							//SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,10+z,0,0, 162);
						}
						if(LightStatus[j]&(0x01<<i))
						{
							if(p->Button_Type == Valve_Button || p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button_PO)
							{
								tempB1=0;tempB2=104;
								//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 104);
							}
							else if(p->Button_Type == Valve_Button1 || p->Button_Type == Valve_Button1_PC|| p->Button_Type == Valve_Button1_PO)
							{
								tempB1=0;tempB2=102;
								//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 102);
							}
							else
							{
								tempB1=0;tempB2=105;
								//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 105);
							}
						}
						else
						{
							if(p->Button_Type == Valve_Button || p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button_PO)
							{
								tempB1=0;tempB2=103;
								//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 103);
							}
							else if(p->Button_Type == Valve_Button1 || p->Button_Type == Valve_Button1_PC || p->Button_Type == Valve_Button1_PO)
							{
								tempB1=0;tempB2=101;
								//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 101);
							}
							else
							{
								tempB1=0;tempB2=106;
							
								//SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 106);
							}
						}
					}
					
					JD_TX_Queue[JD_Save_Frame_Index].Function_ID = 0x0020;
					if(p->Button_Name_Len>10)
						p->Button_Name_Len=10;
					JD_TX_Queue[JD_Save_Frame_Index].Length = 6+p->Button_Name_Len;
					JD_TX_Queue[JD_Save_Frame_Index].data[0] = Buttonlist_Menu_ID;
					JD_TX_Queue[JD_Save_Frame_Index].data[1] = 10+z;
					JD_TX_Queue[JD_Save_Frame_Index].data[2] = tempA1;
					JD_TX_Queue[JD_Save_Frame_Index].data[3] = tempA2;
					JD_TX_Queue[JD_Save_Frame_Index].data[4] = tempB1;
					JD_TX_Queue[JD_Save_Frame_Index].data[5] = tempB2;
					(void)memcpy(&JD_TX_Queue[JD_Save_Frame_Index].data[6],p->Button_Name,p->Button_Name_Len);
					//SetTel_Num(Buttonlist_Menu_ID,z+100,p->Button_Name_Len,p->Button_Name);
					InPutQuere();
					z++;
					if(z >= 8)
						return;
				}
				
				if(m)
					m--;
			}
			
			p++;
		}
	}

}


void UpdataStatusHomeHA_RoomButtonWithFilter_Init(uchar room, uchar index, uchar Filter, uchar T_Room, uchar T_index)
{
	OutPutButtons *p;
	uchar i,j,k,m,z = 0;
	m = index*8;
	for(k = 0; k < 2;k++)
	{
		p = (OutPutButtons *)(EE_HomeAutomation1_1+(room*0x200)+ k*168);
		j = room*2 + k;
		
		for(i = 0; i < 8; i++)
		{
			if(IfBelongType(p->Button_Type, Filter) &&  z<8)
			{
				//SetSwitchIcon(ChangeButton_Icon,HomeAutomationDevice_List1,10+i,0,0, 241);//big icon
				//SetSwitchIcon(ChangeButton_Icon, HomeAutomationDevice_List1, 20+i , 0, 0x00, 242);
				if(!m)
				{
					if(p->Button_Type >= Shutter_Button && p->Button_Type <= Shutter_Blind_Button)
					{
						
					}
					else if(p->Button_Type == HVAC_Button || p->Button_Type == AC_Button)
					{
						
					}
					/*
					else if(p->Button_Type == AC_Button)
					{
						
					}
					*/
					else
					{
						if(k == T_Room && i == T_index)
						{

							if(LightStatus[j]&(0x01<<i))
							{
								if(p->Button_Type == Valve_Button || p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button_PO)
								{
									SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 104);
								}
								else if(p->Button_Type == Valve_Button1 || p->Button_Type == Valve_Button1_PC || p->Button_Type == Valve_Button1_PO)
								{
									SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 102);
								}
								else
									SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 105);
							}
							else
							{
								if(p->Button_Type == Valve_Button || p->Button_Type == Valve_Button_PC || p->Button_Type == Valve_Button_PO)
								{
									SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 103);
								}
								else if(p->Button_Type == Valve_Button1 || p->Button_Type == Valve_Button1_PC || p->Button_Type == Valve_Button1_PO)
								{
									SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 101);
								}
								else
									SetSwitchIcon(ChangeButton_Icon, Buttonlist_Menu_ID, 20+z , 0, 0x00, 106);
							}
							return;
						}
					}
					z++;
					if(z >= 8)
						return;
				}
				
				if(m)
					m--;
			}
			
			p++;
		}
	}
	
}
void HomeHA_RoomTitle_Init(uchar room)
{
	
	Room_Setting *pp;
	
	pp = (Room_Setting *)(EE_CCTV_Room + 4*sizeof(CCTV_Setting) + room*sizeof(Room_Setting));
	
	
	SetSwitchIcon(ChangeButton_Icon,Buttonlist_Menu_ID,144,0,1,0xBE+pp->Icon_ID);
	SetButton_Name(Buttonlist_Menu_ID,144,pp->lenth,pp->name);
		
}
#ifdef Hide_Button_Solution	
void Object_Button_Visible(void)
{
	uchar i;
	uchar temp[7];
	for(i = 0; i < 7; i++)
	{
		if(i+1<Need_Show_buttons)
		{
			temp[i]=1;
			//SetControl_State(Buttonlist_Menu_ID, 11 + i, 1);
			//SetControl_State(Buttonlist_Menu_ID, 21 + i, 1);
		}
		else
		{
			temp[i]=0;
			//SetControl_State(Buttonlist_Menu_ID, 11 + i, 0);
			//SetControl_State(Buttonlist_Menu_ID, 21 + i, 0);
		}
	}
	//sendJDdata(21,Buttonlist_Menu_ID,7,temp);
	//uchar i;
	i=0;
	JD_TX_Queue[JD_Save_Frame_Index].Function_ID = 0x0021;//JDcmd;
	JD_TX_Queue[JD_Save_Frame_Index].Length = 8;//JDlen;
	JD_TX_Queue[JD_Save_Frame_Index].data[0] =Buttonlist_Menu_ID;// JDpage;//MainMenu;
	for(i=0;i<7;i++)
		{
			JD_TX_Queue[JD_Save_Frame_Index].data[1+i]=temp[i];//*(p+i);

		}
	InPutQuere();
}

#endif

void Init_HVAC_CH(uchar menu_id)
{
	if(AC_Status[RoomSelect].bits.C_H == 1)
	{	//
		SetSwitchIcon(ChangeButton_Icon,menu_id,29,0,1,0x1f);
		SetSwitchIcon(ChangeButton_Icon,menu_id,30,0,1,0x02);	
	}	
	else
	{
		AC_Status[RoomSelect].bits.C_H = 2;
		SetSwitchIcon(ChangeButton_Icon,menu_id,29,0,1,0x01);
		SetSwitchIcon(ChangeButton_Icon,menu_id,30,0,1,0x20);		
	}
}

void Init_AC_CH(uchar menu_id)
{
	if(AC_Status[RoomSelect].bits.C_H == 0)
	{//auto
		SetSwitchIcon(ChangeButton_Icon,menu_id,29,0,1,0x25);
		SetSwitchIcon(ChangeButton_Icon,menu_id,30,0,1,0x01);
		SetSwitchIcon(ChangeButton_Icon,menu_id,31,0,1,0x02);
		SetSwitchIcon(ChangeButton_Icon,menu_id,32,0,1,0x14);
	}
	else if(AC_Status[RoomSelect].bits.C_H == 1)
	{	//
		SetSwitchIcon(ChangeButton_Icon,menu_id,29,0,1,0x13);
		SetSwitchIcon(ChangeButton_Icon,menu_id,30,0,1,0x1f);
		SetSwitchIcon(ChangeButton_Icon,menu_id,31,0,1,0x02);
		SetSwitchIcon(ChangeButton_Icon,menu_id,32,0,1,0x14);
	}
	else if(AC_Status[RoomSelect].bits.C_H == 2)
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,29,0,1,0x13);
		SetSwitchIcon(ChangeButton_Icon,menu_id,30,0,1,0x01);
		SetSwitchIcon(ChangeButton_Icon,menu_id,31,0,1,0x20);
		SetSwitchIcon(ChangeButton_Icon,menu_id,32,0,1,0x14);
	}
	else
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,29,0,1,0x13);
		SetSwitchIcon(ChangeButton_Icon,menu_id,30,0,1,0x01);
		SetSwitchIcon(ChangeButton_Icon,menu_id,31,0,1,0x02);
		SetSwitchIcon(ChangeButton_Icon,menu_id,32,0,1,0x26);
	}
}


void Init_AC_mode(uchar menu_id)
{
	if(AC_Status[RoomSelect].bits.mode == 1)
	{	//night
		SetSwitchIcon(ChangeButton_Icon,menu_id,33,0,0,251);
		SetSwitchIcon(ChangeButton_Icon,menu_id,34,0,1,0x1a);
		SetSwitchIcon(ChangeButton_Icon,menu_id,35,0,0,253);
	}
	else if(AC_Status[RoomSelect].bits.mode == 2)
	{	//save
		SetSwitchIcon(ChangeButton_Icon,menu_id,33,0,0,251);
		SetSwitchIcon(ChangeButton_Icon,menu_id,34,0,0,252);
		SetSwitchIcon(ChangeButton_Icon,menu_id,35,0,1,0x1b);
	}
	else if(AC_Status[RoomSelect].bits.mode == 3)
	{	//standby
		SetSwitchIcon(ChangeButton_Icon,menu_id,33,0,0,251);
		SetSwitchIcon(ChangeButton_Icon,menu_id,34,0,0,252);
		SetSwitchIcon(ChangeButton_Icon,menu_id,35,0,0,253);
	}
	else
	{	//comfort
		SetSwitchIcon(ChangeButton_Icon,menu_id,33,0,1,0x19);
		SetSwitchIcon(ChangeButton_Icon,menu_id,34,0,0,252);
		SetSwitchIcon(ChangeButton_Icon,menu_id,35,0,0,253);
	}
}



void Init_AC_Speed(uchar menu_id)
{
	if(AC_Status[RoomSelect].bits.speed == 0)
	{//auto
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,1,0x21);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,0,254);
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,0,255);
		SetSwitchIcon(ChangeButton_Icon,menu_id,39,0,1,0x00);
	}
	else if(AC_Status[RoomSelect].bits.speed == 1)
	{	//
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,1,0x05);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,1,0x1c);
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,0,255);
		SetSwitchIcon(ChangeButton_Icon,menu_id,39,0,1,0x00);
	}
	else if(AC_Status[RoomSelect].bits.speed == 2)
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,1,0x05);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,0,254);
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,1,0x1d);
		SetSwitchIcon(ChangeButton_Icon,menu_id,39,0,1,0x00);
	}
	else
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,1,0x05);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,0,254);
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,0,255);
		SetSwitchIcon(ChangeButton_Icon,menu_id,39,0,1,0x1e);
	}
}

void Init_HVAC_Speed(uchar menu_id)
{
	if(AC_Status[RoomSelect].bits.speed == 0)
	{//auto
		AC_Status[RoomSelect].bits.speed = 1;
	}
	if(AC_Status[RoomSelect].bits.speed == 1)
	{	//
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,1,0x1c);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,0,255);
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,1,0);
	
		
	}
	else if(AC_Status[RoomSelect].bits.speed == 2)
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,0,254);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,1,0x1d);
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,1,0x00);
		//SetSwitchIcon(ChangeButton_Icon,menu_id,39,0,1,0x00);
		
	}
	else
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,36,0,0,254);
		SetSwitchIcon(ChangeButton_Icon,menu_id,37,0,0,255);		
		SetSwitchIcon(ChangeButton_Icon,menu_id,38,0,1,0x1e);
	}
}

void Init_AC_Swing(uchar menu_id)
{
	if(AC_Status[RoomSelect].bits.speed == 0)
	{//auto
		SetSwitchIcon(ChangeButton_Icon,menu_id,40,0,1,0x21);
		SetSwitchIcon(ChangeButton_Icon,menu_id,41,0,1,0x06);
		SetSwitchIcon(ChangeButton_Icon,menu_id,42,0,1,0x07);
		SetSwitchIcon(ChangeButton_Icon,menu_id,43,0,1,0x08);
	}
	else if(AC_Status[RoomSelect].bits.speed == 1)
	{	//
		SetSwitchIcon(ChangeButton_Icon,menu_id,40,0,1,0x05);
		SetSwitchIcon(ChangeButton_Icon,menu_id,41,0,1,0x22);
		SetSwitchIcon(ChangeButton_Icon,menu_id,42,0,1,0x07);
		SetSwitchIcon(ChangeButton_Icon,menu_id,43,0,1,0x08);
	}
	else if(AC_Status[RoomSelect].bits.speed == 2)
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,40,0,1,0x05);
		SetSwitchIcon(ChangeButton_Icon,menu_id,41,0,1,0x06);
		SetSwitchIcon(ChangeButton_Icon,menu_id,42,0,1,0x23);
		SetSwitchIcon(ChangeButton_Icon,menu_id,43,0,1,0x08);
	}
	else
	{
		SetSwitchIcon(ChangeButton_Icon,menu_id,40,0,1,0x05);
		SetSwitchIcon(ChangeButton_Icon,menu_id,41,0,1,0x06);
		SetSwitchIcon(ChangeButton_Icon,menu_id,42,0,1,0x07);
		SetSwitchIcon(ChangeButton_Icon,menu_id,43,0,1,0x24);
	}
}

void Init_AC_Temperature(uchar p)
{
	uchar x,y;
	x = p/10;
	y = p%10;

	SetSwitchIcon(ChangeButton_Icon, PageStatus, 144, 0, 0x03, 0xde + x);
	SetSwitchIcon(ChangeButton_Icon, PageStatus, 145, 0, 0x03, 0xde + y);
}



void AC_Menu_Init(uchar AC_menu)
{
	if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect))
	{
			
		//set icon to off
		SetSwitchIcon(ChangeButton_Icon,AC_menu,28,0,1,0x0e);
	}
	else
	{
	
		SetSwitchIcon(ChangeButton_Icon,AC_menu,28,0,1,0x0d);
	}
	if(AC_menu == HVAC_Select)
	{
		Init_HVAC_CH(HVAC_Select);
		Init_HVAC_Speed(AC_menu);
	}
	else
	{
		Init_AC_CH(AC_menu);
		Init_AC_Speed(AC_menu);
	}
	Init_AC_mode(AC_menu);
	
	if(AC_menu == AC_Select)
		Init_AC_Swing(AC_menu);
	Init_AC_Temperature(A_CTemperature[RoomSelect]);
}

/*

*/
void TimerEventSetting_Init(uchar item)
{
	Timer_Event *p;
	uchar i;
	p = (Timer_Event *) (EE_Time_Event);
	p += item;
	if(p->mode & 0x01)
	{
		//enable
		SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,33,0,0,131);
	}
	else
	{
		SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,33,0,0,132);
	}
	for(i = 1;i < 8;i++)
	{
		if(p->mode&(0x01<<i))
		{
			SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,60+i,0,0,131);
		}
		else
			SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,60+i,0,0,132);
	}
	//show the time 
	SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 22, 0, 0x03, 0xde + (p->Hour/10));
	SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 23, 0, 0x03, 0xde +  (p->Hour%10));
	SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 24, 0, 0x03, 0xde +  (p->Minite/10));
	SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 25, 0, 0x03, 0xde + (p->Minite%10));
	(void)memcpy(&Timer_Event_temp,p,sizeof(Timer_Event_temp));
	//set event variable flag
	for(i = 0; i < 5; i++)
	{
		if(p->Inf_address[i] > 0 && p->Inf_address[i] != 0xffff)
		{
			SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,28+i,0,1,0xA7 + i);
		}
		else
			SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,28+i,0,1,0x9b + i);
	}
}

/*
Init the time event flag on the main menu
*/

void TimerEventFlag_Init(void)
{
	Timer_Event *p;
	uchar i;
	p = (Timer_Event *) (EE_Time_Event);
	for(i = 0; i < 8; i++)
	{
		if(p->mode & 0x01)
		{
			//enable
			SetControl_State(MainMenu, 4, ON);
			return;
		}
		p++;
	}
	SetControl_State(MainMenu, 4, OFF);
	
}



/*
tempeature format translate
*/
unsigned int FloatToInt(unsigned long data)
{
	uchar i=0,flag=0;
	unsigned int tem=0;
	if(data&0x80000000)
	{
		data=data-1;
		data=~data;
		flag=1;
	}
	while(data>2048)
	{
		data=data/2;
		i++;
	}
	tem=(unsigned int)data;
	if(flag)
	{
		tem=(~tem)+1;
		tem&=0x07ff;
		tem |= 0x8000;
	}
	tem |= ((unsigned long)i)<<11;
	return tem;
}

uchar IntToFloat(unsigned int data)
{
	uchar i,flag=0;
	unsigned long tem;
	i=(uchar)(data>>11);
	i &= 0x0f;
	data=data&0x87ff;
	if(data&0x8000)
	{//<0
		data=data&0x07ff;
		data |= 0x0800;
		data=data-1;
		data=(~data)&0x0fff;
		flag=1;
	}
	else
		data=data&0x07ff;
	tem=(unsigned long)data;
	while(i)
	{
		tem = tem*2;
		i--;
	}
	if(flag)
	{
		return 0;
	}
	return tem / 100;
}


void BeepOK(void)
{
    
    if(SettingPara.S_Flag.bits.BeepVolume) {
      
		  Short_tone();
    }   
}


/*
*********************************************
receive frame from JD module and check whether a new key press
if yes, set new key 
*********************************************
*/
void JD_Frame_RC(void)
{
	
	JD_Data_RC Key_JD;
	
	if(JD_RxDone)
	{
		JD_RxDone = 0;
		(void)memcpy(&Key_JD,&rx_buf_Program,sizeof(Key_JD));
		if(Key_JD.Function_ID == 0x100)
		{
			//the function ID is for new button press
			//check the page ID is correct
			if (Key_JD.data[1] == 144)
				return;
			KeyValue = Key_JD.data[1];//button id

		//if(JD_Save_Frame_Index != JD_TX_Frame_Index)
		//{
		//	Dissuasion_Tone();
		//	
		//}
		//else	
		//{
			//Flag.bits.KeyNhandle = 1;

			if((KeyLimitDly)||(JD_Save_Frame_Index != JD_TX_Frame_Index))
			//if((KeyLimitDly)||(JD_Save_Frame_Index != JD_TX_Frame_Index) ||(0!= If_JD_Call_Empty()))
			{
				Dissuasion_Tone();
			}
			else
			{
				Flag.bits.KeyNhandle = 1;
				BeepOK();
				KeyLimitDly=2;
			}
				
		//}
			#ifdef CE_Version
				Timer_Inactivity = 10;
			#else
				Timer_Inactivity = TIME_AUTOPOWEROFF;
			#endif
			//menu match test, if they are not match, we do it
			if((Key_JD.data[0] == WaitPage && PageStatus != WaitPage) || (Key_JD.data[0] == WaitWithShortCut && PageStatus != WaitWithShortCut))
			{
				LoadJD_Menu(PageStatus);
			}
			
		}
		else if(Key_JD.Function_ID == JD_TX_Queue[JD_TX_Frame_Index].Function_ID)
		{ //received ack
			
			if(Key_JD.Function_ID == JD_Oclok)
			{
				(void)memcpy(&R_Clock,&Key_JD.data[0],6);
				//POP_JD_Command();
				if(PageStatus == OClockSetting)
				{
					//set the clock
					
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 22, 0, 0x03, 0xde + R_Clock.Hour1);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 23, 0, 0x03, 0xde + R_Clock.Hour2);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 24, 0, 0x03, 0xde + R_Clock.Min1);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 25, 0, 0x03, 0xde + R_Clock.Min2);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 22, 0, 0x03, 0xde + R_Clock.Hour1);// it always missing so far without reason 
					//temp4 = R_Clock.Hour1*10 + R_Clock.Hour2;
					//temp1 = R_Clock.Min1;
					//temp2 = R_Clock.Min2;
					R_Clock_Temp = R_Clock;
				}
			}
			else if(Key_JD.Function_ID == JD_Year_D)
			{
				(void)memcpy(&R_Date,&Key_JD.data[0],9);
				//POP_JD_Command();
				if(PageStatus == OClockSetting)
				{
					//set the clock
					
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 41, 0, 0x03, 0xde + R_Date.Year3);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 42, 0, 0x03, 0xde + R_Date.Year4);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 43, 0, 0x03, 0xde + R_Date.Month1);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 44, 0, 0x03, 0xde + R_Date.Month2);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 27, 0, 0x03, 0xde + R_Date.Day1);
					SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 16, 0, 0x03, 0xde + R_Date.Day2);
					R_Date_Temp = R_Date;
				}
				else
				{
					JDFunctionCallBack2 = &TimeToReflashTimeEventCount;
				}
			}
			
			POP_JD_Command();
			/*
			else //if(Key_JD.data[0] == 0x80 || Key_JD.data[0] == 0x10)
			{//transmit succeed
			
				POP_JD_Command();
			}
			*/
		}
		else if((Key_JD.Function_ID >> 8) == JD_TX_Queue[JD_TX_Frame_Index].Function_ID)
		{	//some time the JD board return bigendtem problem
			POP_JD_Command();
		}
	}
}


void POP_JD_Command(void)
{
	if(JD_TX_Frame_Index != JD_Save_Frame_Index)
	{
		JD_TX_Frame_Time = 0;
		if(JD_TX_Queue[JD_TX_Frame_Index].Function_ID == Switch_Menu_JD)
		{
			JD_TX_Frame_Time = 25; //make sure the menu has been show
		}
		else if(JD_TX_Queue[JD_TX_Frame_Index].Function_ID == 0x0013)
		{
			//sleep on it need take more time
			JD_TX_Frame_Time = 90;
		}
		JD_TX_Frame_Index++;//we can send next frame
		
		if(JD_TX_Frame_Index == MaxVideoStarQueue)
		{
			JD_TX_Frame_Index = 0;
			
		}		
	}	
}


//---------------------------------------------------------------------------------------------
void JD_CallBack(void)
{
	if(JDFunctionCallBack != NULL && JD_Save_Frame_Index == JD_TX_Frame_Index && !SetTimeDelay)
	{
		(*JDFunctionCallBack)();
		JDFunctionCallBack = NULL;
	}
	else if(JDFunctionCallBack1 != NULL && JD_Save_Frame_Index == JD_TX_Frame_Index && !SetTimeDelay)
	{
		(*JDFunctionCallBack1)();
		JDFunctionCallBack1 = NULL;
	}
	else if(JDFunctionCallBack2 != NULL && JD_Save_Frame_Index == JD_TX_Frame_Index && !SetTimeDelay)
	{
		(*JDFunctionCallBack2)();
		JDFunctionCallBack2 = NULL;
	}
}

//----------------------------------------------------------------------------------------
void SetMainMenuState(unsigned char ID,unsigned char St)
{
	SetControl_State(MainMenu_1, ID, St);
	SetControl_State(MainMenu_2, ID, St);
	SetControl_State(MainMenu_3, ID, St);
	SetControl_State(MainMenu_4, ID, St);
}

//------------------------------------------------------------------------------------------
void Setting_Menu(void)
{
	temp1 = MaxValue5(SettingPara.CallVolume);
	temp2 = MaxValue5(SettingPara.AudioVolume);
	temp3 = MaxValue5(SettingPara.RCallMelody);
	temp4 = MaxValue5(SettingPara.RFloorCallMelody);
	temp5 = SettingPara.S_Flag.bits.BeepVolume;
	temp6 = SettingPara.S_Flag.bits.Mute;
	temp7 = SettingPara.S_Flag.bits.Presence_Absence;
	temp8 = SettingPara.S_Flag.bits.PasswordRequest;
	
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 13, 0, 0x01, 0x6d + temp1);
	SetSwitchIcon(ChangePhoto_Icon, SettingSelect, 14, 0, 0x01, 0x72 + temp2);
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 12, 0, 0x01, 0x68 + temp3);
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 19, 0, 0x01, 0x81 + temp4);
	
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 15, 0, 0x01, 0x78 + temp5);
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 16, 0, 0x01, 0x7a + SettingPara.S_Flag.bits.Mute);
	
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 21, 0, 0x01, 0x87 + SettingPara.S_Flag.bits.Presence_Absence);	
	SetSwitchIcon(ChangeButton_Icon, SettingSelect, 5, 0, 0x01, 0x80 + SettingPara.S_Flag.bits.PasswordRequest);
	
	
	PageStatus = SettingSelect;
	LoadJD_Menu(SettingSelect);
}


//-----------------------------------------------------------------
void SetDate(void)
{
	Set_Date(R_Date_Temp.Year1,R_Date_Temp.Year2,R_Date_Temp.Year3,R_Date_Temp.Year4,R_Date_Temp.Month1,R_Date_Temp.Month2,R_Date_Temp.Day1,R_Date_Temp.Day2);
	SetTimeDelay = 6;
	//JDFunctionCallBack = &HA_Button_Init_CallBack;
}

//-----------------------------------------------------------------------------------
void CallcodeInputMenu_Init(uchar index)
{
	switch(index)
	{
		case 1:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_I_Call, 0, 0x00, 143);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Call_SW, 0, 0x00, 135);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Secial_Services, 0, 0x00, 136);
			if(PageStatus == PhoneBookEdit_Name_Code)
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_E_Call, 0, 0x00, 134);
		break;
		case 2:
			//it is new menu with fixed icon		
			if(PageStatus == PhoneBookEdit_Name_Code)
			{
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_E_Call, 0, 0x00, 144);
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_I_Call, 0, 0x00, 133);
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Call_SW, 0, 0x00, 135);
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Secial_Services, 0, 0x00, 136);
			}
		break;
		case 3:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_I_Call, 0, 0x00, 133);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Call_SW, 0, 0x00, 145);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Secial_Services, 0, 0x00, 136);
			if(PageStatus == PhoneBookEdit_Name_Code)
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_E_Call, 0, 0x00, 134);
		break;
		case 4:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_I_Call, 0, 0x00, 133);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Call_SW, 0, 0x00, 135);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Secial_Services, 0, 0x00, 146);
			if(PageStatus == PhoneBookEdit_Name_Code)
				SetSwitchIcon(ChangeButton_Icon, PageStatus, K_E_Call, 0, 0x00, 134);
		break;
	}
	
}

//----------------------------------------------------------------------------
/*
show a phone book to screen
*/
void ShowInterCom_PhoneBook(uchar index)
{
	Intercom_Record *p;
	uchar i;
	p = (Intercom_Record *) (EE_Page2 + 512*index);
	for(i = 0; i < PageOfRecord; i++)
	{
		if(p->calltype <5 && p->calltype > 0)
			SetTel_Num(PageStatus,40+i,p->namelenth,p->name);
		else
			SetTel_Num(PageStatus,40+i,0,p->name);
		p++;
	}
	SetSwitchIcon(ChangePhoto_Icon, PageStatus, 24, 0, 0x01, 0x9b + index);
}

//------------------------------------------------------------------------------
void CallType_SetFocus(uchar index)
{
	switch(index)
	{
		case 1:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 6, 0, 0, 143);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 7, 0, 0, 134);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 8, 0, 0, 135);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 9, 0, 0, 136);
		break;
		case 2:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 6, 0, 0, 133);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 7, 0, 0, 144);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 8, 0, 0, 135);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 9, 0, 0, 136);
		break;
		case 3:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 6, 0, 0, 133);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 7, 0, 0, 134);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 8, 0, 0, 145);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 9, 0, 0, 136);
		break;
		case 4:
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 6, 0, 0, 133);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 7, 0, 0, 134);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 8, 0, 0, 135);
			SetSwitchIcon(ChangeButton_Icon, PageStatus, 9, 0, 0, 146);
		break;
	}
}
//------------------------------------------------------------------------------
void FillEdit_Name_Code_Type(uchar PageIndex,uchar Index)
{
	Intercom_Record *p;
	p = (Intercom_Record *) (EE_Page2 + 512*PageIndex);
	p += Index;
	if(p->calltype <5 && p->calltype > 0)
	{
		SetTel_Num(PageStatus,15,p->namelenth,p->name);
		SetTel_Num(PageStatus,5,p->codelenth,p->CallCode);
		CallType_SetFocus(p->calltype);
		//fill data to buffer
		/*
		(void)memcpy(CallCode,p->CallCode,p->codelenth);
		(void)memcpy(Temp_Record.name,p->name,p->namelenth);
		Temp_Record.calltype = p->calltype;
		*/
		//(void)memcpy(&Temp_Record.calltype,p->calltype,sizeof(Intercom_Record));
		Temp_Record = *p;
	}
	else
	{
		CallType_SetFocus(1);
		Temp_Record.calltype = 1;
		Temp_Record.namelenth = 0;
		Temp_Record.codelenth = 0;
	}
}


void CheckDel_Name_Code_Type(uchar PageIndex,uchar Index)
{
	Intercom_Record *p;
	p = (Intercom_Record *) (EE_Page2 + 512*PageIndex);
	p += Index;
	
	Temp_Record = *p;
	
	
}
//------------------------------------------------------------------------------

void ShowPhoneBook_E_D(uchar p)
{
	PageStatus = p;
	LoadJD_Menu(p);
	ShowInterCom_PhoneBook(PhoneBookPage - 1);
}
//--------------------------------------------------------------------------------
void Init_VolumeBar(uchar menu_id)
{
	//ini the volume bar
		if(SettingPara.AudioVolume < 6)
		{
			SetSwitchIcon(ChangePhoto_Icon, menu_id, 5, 0, 0x00, 0xbe + SettingPara.AudioVolume);
		}
		
		SetControl_State(menu_id, 5, ON);
		//after 3s we let the volume bar switch off
		VolumeBar_DisplayTime = 3;
		VolumeMenu = menu_id;
}

void R_SetAudio(void)
{
	ClearVolume();
	SetVolumeChange(1,SettingPara.AudioVolume);
	SaveInfQueue(&SaveAudioPara);
}
void SetAudio(void)
{
	JDFunctionCallBack2 = &R_SetAudio;
	SetTimeDelay = 3;
	
}


void If_need_Wakeup_LCD(void)
{
	if(PageStatus == WaitPage || PageStatus == WaitWithShortCut)
        {
        	LoadJD_SleepMenu(ON);
        }
}

void Wakeup_LCD_CallBack(void)
{
	
       LoadJD_SleepMenu(ON);
        
}

void If_need_Wakeup_LCD_CallBack(void)
{
	if(PageStatus == WaitPage || PageStatus == WaitWithShortCut)
        {
        	JDFunctionCallBack2 = &Wakeup_LCD_CallBack;
        }
}
void CCTV_Init(void)
{
	uchar i;
	CCTV_Setting *pp;
	
	pp = (CCTV_Setting *)(EE_CCTV_Room);
	for(i = 0; i<4; i++)
	{
		SetButton_Name(Menu_CCTV,11+i,pp->lenth,pp->name);
		pp++;
	}
	/*
	pp = (CCTV_Setting *)(EE_CCTV_Room);
	SetButton_Name(Menu_CCTV,11,pp->lenth,pp->name);
	*/
}