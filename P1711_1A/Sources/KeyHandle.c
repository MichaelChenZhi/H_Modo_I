#include "derivative.h" /* include peripheral declarations */
#include	"define.h"

#include "Global.h"
#include "Trame.h"
#include "VideoStar.h"
#include "Driv_der.h"
#include "HomeAutoMation.h"
#include "AlarmSystem.h"
#include "Eeprom.h"
#include "Ges_der.h"
//extern Setting SettingPara;
//extern TouchPanelSetting TouchPanelPara;
//extern const uchar LEDFlag;
//#pragma CODE_SEG PPAGE_2
extern const uchar F_PCM_Number;
extern const uchar  F_PCM_Door_Open_Time;

extern void F_Back_Menu(void);
extern void Dissuasion_Tone(void);
extern void SavePhoneBookRecord(uchar phonerindex,uchar phonePageindex,Intercom_Record *p);
//extern void DeletePhoneBook(uchar p);
extern void SaveAudioPara(void);
//extern void SaveVideoPara(void);
extern void DefaultSetting(uchar p1,uchar p2);
//extern void SaveA_Vsetting(void);
extern void Gest_Panic(void);
extern void Long_Tone(void);
extern void TestTone(uchar p); 
extern void Switch_Off(void);

extern void Button_Selfinsertion_Cyclic(void);
//--------------------------------------------
//extern void AutoSwitchButtonPress(void);
extern void S_In_Call_Frame(uchar p);
extern void S_Special_Frame(uchar p);
//extern void S_Memu_Frame(uchar p);

extern void Presence_Absence_Management(void);
//------------------------------------------------
//extern void SaveAutoAddress(void);
//extern void SaveOpenDoor1Address(void);
//extern void SaveOpenDoor2Address(void);
extern void SettoWaitPage(void);
extern void Play_New_Melody(unsigned char kind);
extern void SwitchTalkMenu(void);
extern void Theme_Ini(void);

//void Mute_Toggle(void);
void StairLight(void);
//--------------------------------------------------

void SaveInfQueue(void (*finProc)());


void Show_Tel_List(void)
{
	PageStatus = Tel_Contact_List;
	LoadJD_Menu(Tel_Contact_List);
	//fill list data
	FillListData();
}

uchar MaxValue5(uchar level)
{
	if(level < 5)
		return level;
	return 5;
}

void Add_Circle(uchar *p,uchar data_limit)
{
	if(*p<data_limit)
	{
		*p=(*p)+1;
	}
	else
	{
		*p = 0;
	}
}
void Dec_Circle(uchar *p,uchar data_limit)
{
	if(*p>0)
	{
		*p=(*p)-1;
	}
	else
		*p = data_limit;
}
//-----------------------------------------------------------------------
void TestTone(uchar p) {

  R_Volume_Call = p;
  //Calculate_Volume_and_update_Conexant(R_Volume_Call);
  Play_New_Melody(AUDIO_ONLY_CALL);
}


void Opendoor_Management(uchar p)
{
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
  			p_der_queue_tx_appl->n_byte                     = 2;
        p_der_queue_tx_appl->data[D_OPC] = D_DOOR_OPENER_1 + p -1;
        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
  			Incr_index_queue_in_der(); 
}
//---------------------------------------------------
void S_OUT_CALL_FUNCTION(uchar p1,uchar p2, uchar p3,uchar p4)
{
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
        p_der_queue_tx_appl->n_byte                     = 6;  
        p_der_queue_tx_appl->data[D_OPC] = D_OUT_CALL_FUNCTION;
        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
        p_der_queue_tx_appl->data[D_DATA_1] = p1; //Code gw         0x80 30 30 30
// v1.1       p_der_queue_tx_appl->data[D_DATA_2] = 0x43; 
        p_der_queue_tx_appl->data[D_DATA_2] = p2; // v1.1 
        p_der_queue_tx_appl->data[D_DATA_3] = p3; 
        p_der_queue_tx_appl->data[D_DATA_4] = p4;      // Switchboard preferred
        Incr_index_queue_in_der();                      // update index queue
     
        Main_GUI_locked = 1;
        Timer_GUI_locking = TIME_LOCK_GUI;
}
//------------------------------------------------------
uchar JudgeValue(uchar tpye,uchar *cod,uchar codlen) {
    unsigned int tvalue=0;

    uchar i;
    switch(tpye) {
      case 1: //intercom code from 0 to 15
		if(codlen < 3)
		{
			if(codlen == 1)
			{	//the value should <=9
				if(cod[0] <= K_Num9)
					return 1;
			}
			else
			{
				//the valute should <=15
				tvalue = (cod[0] - K_Num0)*10 + (cod[1] - K_Num0);
				if(tvalue < 16)
					return 1;
			}
		}
      
      break;
      case 2://external intercom 
      if(codlen==4) {
        return 1;
      }
      
      break;
	case 3:  //new spt add the switchboard code from 1 to 249
      case 4:  //special call from 1 to 249
      //if(codlen==3) {
          //maybe >250
          for(i=0;i<codlen;i++) {
             tvalue=tvalue*10;
             tvalue+=*cod++-K_Num0;
          }
          if(tvalue>0 && tvalue<250) {
                return 1;
          }
          
      //}
      break;
    }
    
    return 0;
}
//-----------------------------------------------------------------------
void UpKeyPress(void)
{
	if(JD_Save_Frame_Index != JD_TX_Frame_Index)
	{
		Dissuasion_Tone();
		return;
	}
	// new idea for change one page for every up button press
	if(PhoneBookPage > 1)
	{
		PhoneBookPage--;		
			
	}
	else
	{
		//Dissuasion_Tone();
		PhoneBookPage = 3;
	}
	ShowInterCom_PhoneBook(PhoneBookPage - 1);		
}
//-----------------------------------------------------------------------
void DownKeyPress(void)
{
	if(JD_Save_Frame_Index != JD_TX_Frame_Index)
	{
		Dissuasion_Tone();
		return;
	}
	if(PhoneBookPage < 3)
	{
		PhoneBookPage++;		
				
	}
	else
	{
		//Dissuasion_Tone();
		PhoneBookPage = 1;
	}
	ShowInterCom_PhoneBook(PhoneBookPage - 1);	
}
//------------------------------------------------------------------------------
uchar NumKeyPress(void)
{
	temp1 = (PhoneBookPage-1)*RecordNumIn_one_page+ (KeyValue - K_Num0);
	//if(temp1 <= SettingPara.RecordCnt)
	//{
		PhoneRIndex = temp1;
		ItemIndex = KeyValue - K_Num0; //this used for shut call
		//LoadPhoneBook(F_PhoneBook,KeyValue - K_Num0);
	//}
	//else
	//{
	//	Flag4.bits.Num_P_OK = 1;
	//	Dissuasion_Tone();
	//	return 0;
	//}
	return 1;
}
//-----------------------------------------------------------------------
void MicroPhoneMutePress(uchar menu_id, uchar ConTrol_ID)
{
	if(	(MODO_State == IN_COMMUNICATION_VIDEO) ||
					(MODO_State == IN_COMMUNICATION_ANSWERINGMACHINE)   ||
					(MODO_State == IN_COMMUNICATION_SELFINSERTION)  ||
					(MODO_State == IN_COMMUNICATION_AUDIO_ONLY) ||
					(Sub_MODO_State == IN_COMMUNICATION_PCM1) ||
					(Sub_MODO_State == PSTN_Talking) ||
					(Sub_MODO_State == PSTN_Talking_Video)){
				          // Icon must blink to signal the mute condition 
				          if(MIC_Muted == OFF){
//				            ST_Audio = MIC_IN_MUTE_0;                 // switch off  the microphone
//				            timer_audio_poweron = TIME_IMP_AUDIO8;
                    
				            //MIC_Muted = ON;
						Mic_State(ON);
				            // blink the icon
						SetButton_State(0x01, menu_id, ConTrol_ID, ON);
				          }
				          else{
//				            ST_Audio = MIC_OUT_OF_MUTE_0;                 // switch on the microphone
//				            timer_audio_poweron = TIME_IMP_AUDIO8;
				            //MIC_Muted = OFF;
						Mic_State(OFF);
				            // remove blink for the icon
				            SetButton_State(0x01, menu_id, ConTrol_ID, OFF);
				          }
	}
}
//----------------------------------------------------------------
void AudioButtonPress(void)
{
	if(hook_state_toggled == OFF_HOOK)
          {
	          if(MODO_State != IN_COMMUNICATION_SELFINSERTION){
	            hook_state_toggled = ON_HOOK;
  
	          }
          }
        else  
          {
	          hook_state_toggled = OFF_HOOK;

          }
        Signal(proc_scan, HOOK_VARIATION);     // simulate offhook
}

//---------------------------------------------------------------------------------
void ReadTime_From_JDBoard(void)
{
	CheckCommand(JD_Oclok);
	CheckCommand(JD_Year_D);
	//SetTimeDelay = 2; //set delay 200ms for we can read the date correctly from JD module
}
//---------------------------------------------------------------------------------
void DefaultSave(void)
{

 	if(F_Presence_Absence)
 	{
 		Flag.bits.Set_AB_Server = 1;
 	}
	DefaultSetting(0x55,0xaa);                      //load default setting in the function
	
	Theme_Ini();
	PageStatus=MainMenu;
	LoadJD_Menu(MainMenu);
}


//------------------------------------------------
void SavePhoneBookRecordQueue(void)
{                                                 //save a phone book record ,this is  for the call back function
  SavePhoneBookRecord(PhoneRIndex,PhoneBookPage-1,&Temp_Record);
}


//----------------------------------------------


//--------------------------------------------------------------------------


//------------------------------------------------------------------------

void ShutCall(uchar p) {
	Intercom_Record *Temp_R;
	Temp_R = (Intercom_Record *) (EE_Page2 + 512*(PhoneBookPage -1));
	Temp_R += p;				    
	if( Temp_R->calltype == 1)
	{
				         //send internal intercom call  frame
		temp2=0;
		for(temp1=0;temp1<Temp_R->codelenth;temp1++) 
		{
			temp2*=10;
			temp2+=Temp_R->CallCode[temp1]-0x30;
		} 
		 
		S_In_Call_Frame(temp2);
	} 
	else if(Temp_R->calltype == 2) 
	{
		
               	S_OUT_CALL_FUNCTION(Temp_R->CallCode[0],Temp_R->CallCode[1],Temp_R->CallCode[2],Temp_R->CallCode[3]); 
				          
	} 
	else if(Temp_R->calltype == 3)
	{
		
		temp2=0;
		for(temp1=0;temp1<Temp_R->codelenth;temp1++) 
		{
			temp2*=10;
			temp2+=Temp_R->CallCode[temp1]-0x30;
		}
		S_OUT_CALL_FUNCTION(0x80,0x30,0x30,temp2);
				          
		
	}
	else if(Temp_R->calltype == 4)
	{
		//special call
		temp2=0;
		for(temp1=0;temp1<Temp_R->codelenth;temp1++)
		{
			temp2*=10;
			temp2+=Temp_R->CallCode[temp1]-0x30;
		}
					  	 
		S_Special_Frame(temp2);
		 
	}
	else
		Dissuasion_Tone();
					    
	
}

//------------------------------------
/*
*/
void ExitToWaitPage(void) {
    hook_state_toggled = ON_HOOK;
	Switch_Off();
	SettoWaitPage();
	LoadJD_Menu(PageStatus);
}
//----------------------------------------------------------------------------------------
void SaveInfQueue(void (*finProc)())
{
	SaveInf=finProc;
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte = 2;
  	p_der_queue_tx_appl->data[D_OPC] = D_DUMMY;
	p_der_queue_tx_appl->data[D_ADDRESS] = 0;
 	Incr_index_queue_in_der();                       
	PRG_FLASH_AFTER_TX = ON;
}
//----------------------------------------------------------------------------------------

void KeyHandle(void)
{
	//handle the key
	if(Flag.bits.KeyNhandle)
	{
		Flag.bits.KeyNhandle = 0;
		if(NoKeyTime)
			return;
		switch(PageStatus)
		{
			case WaitPage: 
			case WaitWithShortCut:	
			//STANDARD MODE
			switch(KeyValue)
			{
				
				case K_MenuButton:
					if(JD_start_wait)
					{

						JD_start_wait = 1;
					}
					Video(VIDEO_ON);
				        System_Video = RELE_VIDEO_OFF;				    			        
				        SwitchedOnOff = 1;										
					PageStatus = MainMenu;
					LoadJD_Menu(MainMenu);	 
				break;
				case 5:
				case 6:
					ScenarioOutput(KeyValue-5, 0);
				break;
				case K_BurglarAlarm:
					//go to menu which can be cancle the panic button
					if(JD_start_wait)
					{
						JD_start_wait = 0;
					}
					Video(VIDEO_ON);
				        System_Video = RELE_VIDEO_OFF;        
				        SwitchedOnOff = 1;
					SetSwitchIcon(ChangeButton_Icon, Panic_Cancle, K_Exit, 0, 0x00, 231);
					PageStatus = Panic_Cancle;
					LoadJD_Menu(Panic_Cancle);
					
					PanicCancleTime = 7; //after 5s without cancel the panic frame will be sent
					break;
				default:
				return;
			}
			break; 
			case Panic_Cancle:
				switch(KeyValue)
				{
					case K_Exit://for auto insertion
					case K_OK:
						//begin monitor a camera
						//Button_Selfinsertion_Cyclic();	
						PanicCancleTime = 0;
						SettoWaitPage();
						Switch_Off();
						LoadJD_Menu(PageStatus);
							//Autoinsert(0);
					
					break;
				}
			break;
			case MainMenu_1:
			case MainMenu_2:
			case MainMenu_3:
			case MainMenu_4:
			//Main Menu
			switch(KeyValue)
			{
				case K_AutoSwitch:
					//begin monitor a camera
					//should goto selection menu for CCTV or PCM or 
					PageStatus = AutoInsert_Selection;
					LoadJD_Menu(AutoInsert_Selection);
					break;
				case K_AnsweringService:
					if(!Main_GUI_locked){
						 p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
					        p_der_queue_tx_appl->n_byte = 2;
					        p_der_queue_tx_appl->data[D_OPC] = D_SVC_NEXT;
					        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
					        Incr_index_queue_in_der();                      // update index queue
					        Main_GUI_locked = 1;
					        Timer_GUI_locking = TIME_LOCK_GUI;
					}
					else
						Dissuasion_Tone();
					break;

				case K_Telephone_MENU://used to PSTN module
				
					Tel_index = 0;
					(void)memset(&Tel_num,0,sizeof(Tel_num));
					ShowTelNum();
					SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB0);
					SetButton_State(0x01, PSTN_Telephone, 16, OFF); //default should be not blink
					TelRdFrameTime=100;
					PageStatus = PSTN_Telephone;
					LoadJD_Menu(PSTN_Telephone);
					JDFunctionCallBack = &Tel_MemoryName_Init;
					break;
				case K_PhoneBook:							
					 PageStatus = CallNumInput;
					 LoadJD_Menu(CallNumInput);
					 Temp_Record.calltype= 1;	//default item for call type			  
					 CallcodeInputMenu_Init(Temp_Record.calltype);
					Temp_Record.codelenth = 0;
					(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));
					
					//LoadCallingCodeInput(F_CallingCodeInput, CallCode, 4, TypeIndex);
					break;	
				case K_CallSwitchBoard:
					
				   	//emit the call switchboard frame	, the value is fixed	
				   	if(!Main_GUI_locked){
						S_OUT_CALL_FUNCTION(0x80,0x30,0x30,250);
						//LoadMainMenu(F_MainMenu,1); //set the icon blink
						SetButton_State(0x01, MainMenu, K_CallSwitchBoard, ON);
						SystemStatus.bits.CallSwitchBoard_Icon = 1;
						PCM_Audio_Switch = To_Modo;
				   	}
					else
						Dissuasion_Tone();
					break;
				case K_LightTurnOn:
					//transmit the light on frame
					StairLight();
					break;

				case K_Auto_Door_Open:
					if(Internal == MASTER_APARTMENT_STATION){ 
		    	   
					  	if(F_AutomaticDoorOpener)
						{	
							F_AutomaticDoorOpener = 0;						
							Button_for_dentist = DENTIST_BUTTON_NOT_PRESSED;
						}
						else
						{							 
							 F_AutomaticDoorOpener = 1;					 
							 Button_for_dentist = DENTIST_BUTTON_PRESSED;		
						}						
						//set icon display accordingly
						SetSwitchIcon(ChangeButton_Icon, PageStatus, K_Auto_Door_Open, 0, 0, 31 + F_AutomaticDoorOpener);
						//save parameter
						Flag.bits.SaveAP = 1;
		  			}
					else
						Dissuasion_Tone();
				break;

				case K_Door_Open_1:
					Opendoor_Management(1);
				break;
				case K_Door_Open_2:
					Opendoor_Management(2);
				break;
				
				case K_Home_Auto:
					
					if(F_HA_Passwordneed)
					{
						PasswordRequest.all = 0;
						PasswordRequest.bits.HA_F = 1;
						Tel_index = 0;
						(void)memset(&Tel_num,0,sizeof(Tel_num));
						PageStatus = PasswordInput;
						LoadJD_Menu(PasswordInput);
					}
					else
					{
						HA_Menu_Ini();
					}
					
				break;

				case K_BurglarAlarm:
					//these should be new code 
				//	SetControl_State(Home_80_suo, ALARM_BLINK, OFF);
					Tel_index = 0;
					RoleAlarmSystem = No_regedit;
					PageStatus = AlarmSystem_Manage;
					LoadJD_Menu(AlarmSystem_Manage);
					AlarmSystemZoneName_Init();
					break;
				case K_Setting:
					if( SettingPara.S_Flag.bits.PasswordRequest)
					{
						PasswordRequest.all = 0;
						PasswordRequest.bits.Setting_F = 1;
						Tel_index = 0;
						(void)memset(&Tel_num,0,sizeof(Tel_num));
						PageStatus = PasswordInput;
						LoadJD_Menu(PasswordInput);
					}
					else
					{
						Setting_Menu();
					}
				break;
				
				
			}
			break;
			case AutoInsert_Selection:
				switch(KeyValue)
				{
					case K_Exit:
						PageStatus = MainMenu;
						LoadJD_Menu(MainMenu);
					break;
					case 4://CCTV
						BackMenu = PageStatus;
						Sub_MODO_State = Mornitor_CCTV;
						PageStatus = Menu_CCTV;
						LoadJD_Menu(Menu_CCTV);
						CCTV_Init();
					break;
					case 5:
					
						if(F_PCM_Number)
						{
							Sub_MODO_State = Mornitor_PCM1;
							
							//need hide and show icons
							if(F_PCM_Number > 1)
							{
								SetControl_State(PCM_Auto_Insert - 1, 7, ON); //for next
							}
							else
							{
								SetControl_State(PCM_Auto_Insert - 1, 7, OFF);
							}
							SetControl_State(PCM_Auto_Insert - 1, 6, OFF);
							
						}
						else
							Dissuasion_Tone();
					break;
					case 6:
						if(!Main_GUI_locked){
							Button_Selfinsertion_Cyclic();
							//if(hook_state_toggled == OFF_HOOK)
								hook_state_toggled = ON_HOOK;
						}
						else
							Dissuasion_Tone();
					break;
				}
			break;
			case CallNumInput:
			case CallNumInput_KeyBoard:
				//input the calling code 
				switch(KeyValue)
				{
					case K_Exit:
						PageStatus = MainMenu;
						LoadJD_Menu(MainMenu);
						
					break;
					case K_OK:
						//send the code which is input
						if(Temp_Record.codelenth > 0)
						{
							if(Temp_Record.calltype == 1)
							{	//internal intercom call from 0 to 15
								temp2=0;
					         		for(temp1=0;temp1<Temp_Record.codelenth;temp1++) {
					           			temp2*=10;
							           	temp2+=Temp_Record.CallCode[temp1]-0x30;
							       }
								if(temp2 < 16)
								{
									S_In_Call_Frame(temp2);
								}
								else
								{								
									Temp_Record.codelenth = 0;
									(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));				   					 
									SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
									Dissuasion_Tone();
								}
							}
							else if(Temp_Record.calltype == 2)
							{
								//external intercom call 4 codes
								if(Temp_Record.codelenth != 4)
								{
									Temp_Record.codelenth = 0;
									(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));				   					 
									SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
									Dissuasion_Tone();
								}
								else
								{
									S_OUT_CALL_FUNCTION(Temp_Record.CallCode[0],Temp_Record.CallCode[1],Temp_Record.CallCode[2],Temp_Record.CallCode[3]); 
								}
							}
							else if(Temp_Record.calltype == 3)
							{
								//call switchboard frame
								AddressResult=0;
					         		for(temp1=0;temp1<Temp_Record.codelenth;temp1++) {
					           			AddressResult*=10;
							           	AddressResult+=Temp_Record.CallCode[temp1]-0x30;
							       }
								if(AddressResult == 0 || AddressResult > 249)
								{
									Temp_Record.codelenth = 0;
									(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));				   					 
									SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
									Dissuasion_Tone();
								}
								else
									S_OUT_CALL_FUNCTION(0x80,0x30,0x30,AddressResult&0x00ff);
							}
							else if(Temp_Record.calltype == 4)
							{
								//special call from 1 to 249
								AddressResult=0;
					         		for(temp1=0;temp1<Temp_Record.codelenth;temp1++) {
					           			AddressResult*=10;
							           	AddressResult+=Temp_Record.CallCode[temp1]-0x30;
							       }
								if(AddressResult == 0 || AddressResult > 249)
								{
									Temp_Record.codelenth = 0;
									(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));				   					 
									SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
									Dissuasion_Tone();
								}
								else
								{
									S_Special_Frame(AddressResult&0x00ff);
								}
							}
						}
						
						else
						{
							Dissuasion_Tone();
						}
						
					break;
						
					case K_PhoneBook:
						
						PageStatus = PhoneBookSelect;
						LoadJD_Menu(PhoneBookSelect);
						//ItemIndex = 1;				  
						PhoneBookPage = 1;		//init the phonebook to Page 1	
						//SystemStatus.bits.OKbutton = 1; //OK button should be display on the status bar
						ShowInterCom_PhoneBook(PhoneBookPage-1);
						
					break;
					case K_I_Call:
					
					case K_Call_SW:
					case K_Secial_Services:
						if(PageStatus != CallNumInput)
						{
							PageStatus = CallNumInput;
							LoadJD_Menu(CallNumInput);
						}
						//change the call type
						if(Temp_Record.calltype != KeyValue - K_I_Call + 1)
						{
							Temp_Record.codelenth = 0;
							(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));
							SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
						}
						Temp_Record.calltype = KeyValue - K_I_Call + 1;
						CallcodeInputMenu_Init(Temp_Record.calltype);
					break;
					case K_E_Call:
						if(PageStatus != CallNumInput_KeyBoard)
						{
							PageStatus = CallNumInput_KeyBoard;
							LoadJD_Menu(CallNumInput_KeyBoard);
							Temp_Record.calltype = KeyValue - K_I_Call + 1;
							Temp_Record.codelenth = 0;
						}
					break;
					case 148:
					case 149:
					case 150:
					case 151:
					case 152:
					case 153:
					case 154:
					case 155:
					case 156:
					case 157:	
					
						KeyValue -= 100;
					case K_Num0:
					case K_Num1:
					case K_Num2:
					case K_Num3:
					case K_Num4:
					case K_Num5:
					case K_Num6:
					case K_Num7:
					case K_Num8:
					case K_Num9:
						if(Temp_Record.calltype == 1)
						{
							if(Temp_Record.codelenth < 2)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
								SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
							}
							else
								Dissuasion_Tone();
						}
						else if(Temp_Record.calltype == 2)
						{
							if(Temp_Record.codelenth < 4)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
								SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
							}
							else
								Dissuasion_Tone();
						}
						else
						{	//switchboard and special code with 3 numbers
							if(Temp_Record.codelenth < 3)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
								SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
							}
							else
								Dissuasion_Tone();
						}
						break;
					case K_K_Delete:
					case 128:	
							if(Temp_Record.codelenth > 0){
								Temp_Record.CallCode[--Temp_Record.codelenth] = 0x20;
								SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
							}
					break;
					default:
						if(Temp_Record.calltype == 2)
						{
							//
							if(Temp_Record.codelenth < 4 && KeyValue>= K_K_A && KeyValue<= K_K_Z)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
								SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
							}
							else
								Dissuasion_Tone();
						}
						else
							Dissuasion_Tone();
					return;
				}
			break;
			case PhoneBookDelSelect:
			//select the phonebook
			switch(KeyValue)
			{
				case K_Exit:
					Setting_Menu();
				break;

				case K_UP:
					UpKeyPress();
					
					
				break;
				
				case K_Down:
					DownKeyPress();
				
				break;
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:				
				case 19:
				case 20:
				case 21:
				case 22:	
					PhoneRIndex = KeyValue - 13; // fixed delete issue 2015-1-16
					//check if the record is valid
					CheckDel_Name_Code_Type(PhoneBookPage - 1,PhoneRIndex);
					if(Temp_Record.calltype < 5)
					{				
						PhoneRIndex = KeyValue - 13;
						//delete the record
						Temp_Record.calltype = 0xff;
						//Temp_Record.namelenth = 0;
						//Temp_Record.codelenth = 0;
						Flag.bits.SavePBRQ = 1;
						//it need show the phonebook after the new record have been save
						Flag.bits.DelPhoneBook = 1;
					}
					else
						Dissuasion_Tone();
				break;
				case 23: // switch to Delete list menu

					ShowPhoneBook_E_D(PhoneBookEditSelect);
				break;
				default:
				return;
			}
			break;
			case PhoneBookSelect:
			//select the phonebook
			switch(KeyValue)
			{
				case K_Exit:
					PageStatus = CallNumInput;
					(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));
					Temp_Record.calltype = 1; //default tpye
					Temp_Record.codelenth = 0;
					Temp_Record.namelenth = 0;
					//SetTel_Num(PageStatus,5,codelenth,CallCode);
					LoadJD_Menu(CallNumInput);
					CallcodeInputMenu_Init(Temp_Record.calltype);					
				break;

				case K_UP:
					UpKeyPress();
					
					
				break;
				
				case K_Down:
					DownKeyPress();
				
				break;
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:				
				case 19:
				case 20:
				case 21:
				case 22:	
						
					//if(NumKeyPress())
					//{//select is valid
					ShutCall(KeyValue - 13);
					//}
				break;
				default:
				return;
			}
			break;
			
			case PhoneBookEditSelect:
			//selete to edit
			switch(KeyValue)
			{
				case K_Exit:
				
					Setting_Menu();
				break;
				
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:				
				case 19:
				case 20:
				case 21:
				case 22:
					//switch to Edit menu
					PhoneRIndex = KeyValue -13;
					PageStatus = PhoneBookEdit_Name_Code;
					LoadJD_Menu(PhoneBookEdit_Name_Code);					
					FillEdit_Name_Code_Type(PhoneBookPage - 1,PhoneRIndex);					
					//set Name is the focus
					inputfocus = 1; //set name as the focus
					SetSwitchIcon(ChangeButton_Icon, PageStatus, 10, 0, 0, 147);
					SetSwitchIcon(ChangeButton_Icon, PageStatus, 11, 0, 0, 138);
				break;
				case 23: // switch to Delete list menu

					ShowPhoneBook_E_D(PhoneBookDelSelect);
				break;
				case 25:
					UpKeyPress();
				break;
				case 26:
					DownKeyPress();
				break;
				
				
				default:
				return;
			}
			break;
			
			case PhoneBookEdit_Name_Code:
				switch(KeyValue)
				{
					case K_Exit:
						ShowPhoneBook_E_D(PhoneBookEditSelect);
					break;
					
					case K_OK:
						if(Temp_Record.namelenth>0)
						{
							 if(Temp_Record.codelenth>0 && JudgeValue(Temp_Record.calltype,Temp_Record.CallCode,Temp_Record.codelenth) == 1) //new spt changed if(TypeIndex == 3 ||(codelenth>0 && JudgeValue(TypeIndex,CallCode,codelenth) == 1))
							 {
								//save information
								//codelenth = 0;
								Flag.bits.SavePBRQ = 1;
								//it need show the phonebook after the new record have been save
								Flag.bits.editPhoneBook = 1;
							    	//ShowInterCom_PhoneBook(PhoneBookPage - 1);	
							    	
							}
							else
							{
							    	Temp_Record.codelenth = 0;
						  		inputfocus = 2; //set the focus on code box when input error
						  		(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));
								SetTel_Num(PhoneBookEdit_Name_Code,5,Temp_Record.codelenth,Temp_Record.CallCode);
								SetSwitchIcon(ChangeButton_Icon, PageStatus, 10, 0, 0, 137);
								SetSwitchIcon(ChangeButton_Icon, PageStatus, 11, 0, 0, 148);
								
							    	Dissuasion_Tone();
							}
				      
						}
						else{
							//we have to input the name
							inputfocus = 1;
							//FillDataToVideoStarBuf(inputfocus, UpdateData_Show, namelenth, name);
						   	Dissuasion_Tone();
						}
					
					break;
					case K_I_Call:
					case K_E_Call:
					case K_Call_SW:
					case K_Secial_Services:
						
						if(Temp_Record.calltype != KeyValue - K_I_Call + 1)
						{
							Temp_Record.codelenth = 0;
							(void)memset(Temp_Record.CallCode,0x20,sizeof(Temp_Record.CallCode));
							SetTel_Num(PageStatus,5,Temp_Record.codelenth,Temp_Record.CallCode);
						}
						Temp_Record.calltype = KeyValue - K_I_Call + 1;
						CallcodeInputMenu_Init(Temp_Record.calltype);
					break;
					
					case 10:
						if(inputfocus != 1)
						{
							inputfocus = 1; //set name as the focus
							SetSwitchIcon(ChangeButton_Icon, PageStatus, 11, 0, 0, 138);
							SetSwitchIcon(ChangeButton_Icon, PageStatus, 10, 0, 0, 147);						
						}
					break;
					case 11:
						if(inputfocus != 2)
						{
							inputfocus = 2; //set name as the focus
							SetSwitchIcon(ChangeButton_Icon, PageStatus, 10, 0, 0, 137);
							SetSwitchIcon(ChangeButton_Icon, PageStatus, 11, 0, 0, 148);
						}
					break;
					case K_K_Delete:
					case 128:	
					if(inputfocus == 1)
					{	//input name
						if(Temp_Record.namelenth > 0)
						{
							Temp_Record.name[--Temp_Record.namelenth] = 0x20;
							SetTel_Num(PhoneBookEdit_Name_Code,15,Temp_Record.namelenth,Temp_Record.name);
						}
					}
					else
					{
						//input code
						if(Temp_Record.codelenth > 0)
						{
							Temp_Record.CallCode[--Temp_Record.codelenth] = 0x20;
							SetTel_Num(PhoneBookEdit_Name_Code,5,Temp_Record.codelenth,Temp_Record.CallCode);
						}
					}
				break;
				default:
					
					if(inputfocus == 1)
					{//input name
						if(KeyValue==K_Blank || KeyValue == K_K_Symbol_comma ||KeyValue == K_K_Symbol_Dot || KeyValue == K_K_Symbol_UL || KeyValue == K_K_Symbol_semicolon || KeyValue == K_K_Symbol_ML || (KeyValue>=K_Num0 && KeyValue<=K_Num9) || (KeyValue >= K_K_A && KeyValue <= K_K_Z))
						{
							if(Temp_Record.namelenth < NameLen)
							{
								Temp_Record.name[Temp_Record.namelenth++] = KeyValue;
								SetTel_Num(PhoneBookEdit_Name_Code,15,Temp_Record.namelenth,Temp_Record.name);
							}
							else
								Dissuasion_Tone();
						}
					}
					else
					{
						//input code
						if(Temp_Record.calltype == 1)
						{
							if(KeyValue>=K_Num0 && KeyValue<=K_Num9 && Temp_Record.codelenth < 2)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
							}
							else
							{
								Dissuasion_Tone();
							}
						}
						else if(Temp_Record.calltype == 2)
						{
							if(((KeyValue>=K_Num0 && KeyValue<=K_Num9) || (KeyValue >= K_K_A && KeyValue <= K_K_Z)) && Temp_Record.codelenth < 4)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
							}
							else
							{
								Dissuasion_Tone();
							}
						}
						else if(Temp_Record.calltype == 3 ||Temp_Record.calltype == 4)
						{ //special code only 3numbers should be accept //new spt add sw code input limit 3 numbers
							if(KeyValue>=K_Num0 && KeyValue<=K_Num9 && Temp_Record.codelenth < 3)
							{
								Temp_Record.CallCode[Temp_Record.codelenth++] = KeyValue;
							}
							else
							{
								Dissuasion_Tone();
							}
						}
						else
						{
							Dissuasion_Tone();
						}
						SetTel_Num(PhoneBookEdit_Name_Code,5,Temp_Record.codelenth,Temp_Record.CallCode);
					}
				break;
					
				}
			break;
			
			case Timer_Event_Edit_Parameter:
				switch(KeyValue)
				{
					case K_Exit:
						//StairLight();
						PageStatus = Timer_Event_Edit_Event;
						LoadJD_Menu(Timer_Event_Edit_Event);
					break;
					case K_OK:
						//save and leave the menu
						SaveInfQueue(&SaveTimerEvent);//SaveTimerEvent();
						DlySendTimeEven=3;
						//TimerEventFlag_Init();
						PageStatus = Timer_Event_Edit_Event;
						LoadJD_Menu(Timer_Event_Edit_Event);
						EventHandle_Flag.bits.ReflashTimerCount_F = 1; //set let recount the time
						
					break;
					case 28:
					case 29:
					case 30:
					case 31:
					case 32:
						
						//event select
						Timer_Event_Inf_Index = KeyValue - 28;
						Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = 0;
						Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = 0;
						ExitMenu = &TimerEvent_Edit_Init_Back;
						HA_Menu_Ini();
						
					break;
					case 33://enable or disable
						if(Timer_Event_temp.mode& 0x01)
						{
							Timer_Event_temp.mode &= 0xfe;
							SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,33,0,0,132);
						}
						else
						{
							Timer_Event_temp.mode |= 0x01;
							SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,33,0,0,131);
						}
					break;
					case 35://preview
						
						if(TimeEvent_Flag.all)
							Dissuasion_Tone();
						else
							TimeEvent_Flag.all |= (1<<Timer_Event_Index);
							
							
					break;
					case 45:
						if(Timer_Event_temp.Hour < 23)
						{
							Timer_Event_temp.Hour++;
							
						}
						else
							Timer_Event_temp.Hour = 0;
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 22, 0, 0x03, 0xde + (Timer_Event_temp.Hour/10));
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 23, 0, 0x03, 0xde +  (Timer_Event_temp.Hour%10));
					break;
					case 46:
						if(Timer_Event_temp.Hour > 0)
						{
							Timer_Event_temp.Hour--;
							
						}
						else
							Timer_Event_temp.Hour = 23;
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 22, 0, 0x03, 0xde + (Timer_Event_temp.Hour/10));
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 23, 0, 0x03, 0xde +  (Timer_Event_temp.Hour%10));
					break;
					
					case 47:
						if(Timer_Event_temp.Minite < 50)
						{
							Timer_Event_temp.Minite += 10;							
						}
						else
							Timer_Event_temp.Minite %= 10;
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 24, 0, 0x03, 0xde + (Timer_Event_temp.Minite/10));
						
					break;
					case 48:
						if(Timer_Event_temp.Minite > 9)
						{
							Timer_Event_temp.Minite -= 10;							
						}
						else					
							Timer_Event_temp.Minite += 50;
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 24, 0, 0x03, 0xde + (Timer_Event_temp.Minite/10));
						
					break;
					case 50:
						if(Timer_Event_temp.Minite%10 < 9)
						{
							Timer_Event_temp.Minite += 1;							
						}
						else
						{
							
							Timer_Event_temp.Minite -= 9;
						}
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 25, 0, 0x03, 0xde + (Timer_Event_temp.Minite%10));
						
					break;
					case 51:
						if(Timer_Event_temp.Minite%10 > 0)
						{
							Timer_Event_temp.Minite -= 1;							
						}
						else					
							Timer_Event_temp.Minite += 9;
						SetSwitchIcon(ChangePhoto_Icon, Timer_Event_Edit_Parameter, 25, 0, 0x03, 0xde + (Timer_Event_temp.Minite%10));
						
					break;
					case 61:
					case 62:
					case 63:
					case 64:
					case 65:
					case 66:
					case 67:
						if(Timer_Event_temp.mode&(0x01<<(KeyValue-60)))
						{
							Timer_Event_temp.mode &= ~(0x01<<(KeyValue-60));
							SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,KeyValue,0,0,132);
						}
						else
						{
							Timer_Event_temp.mode |= (0x01<<(KeyValue-60));
							SetSwitchIcon(ChangeButton_Icon,Timer_Event_Edit_Parameter,KeyValue,0,0,131);
						}
					break;
					
				}
			break;
			case Timer_Event_Edit_Event:
				switch(KeyValue)
				{
					case K_Exit:
						//StairLight();
						//PageStatus = AudioAdjust;
						//LoadJD_Menu(AudioAdjust);
						Setting_Menu();
					break;
					case 49:
					case 50:
					case 51:
					case 52:
					case 53:
					case 54:
					case 55:
					case 56:
						Timer_Event_Index = KeyValue - 49;
						//init Timer event menu
						/*
						TimerEventSetting_Init(RoomSelect);
						PageStatus = Timer_Event_Edit_Parameter;
						LoadJD_Menu(Timer_Event_Edit_Parameter);
						*/
						TimerEvent_Edit_Init();
					break;
				}
			break;
			/*
			case CallNameCodeInput:
			switch(KeyValue)
			{
				case K_Exit:
					
				break;
				case K_OK:
					
				break;
					
				
				
			  	
				
				
			}
			break;
			*/
			case ThemeSelect:
				switch(KeyValue)
				{
					case K_Exit:
						
						Setting_Menu();
					break;
					case K_OK:
						//save the information and set the correct background
						SettingPara.Theme = temp9;						
						MainMenu = MainMenu_1 + temp9 - 1;							
						SaveInfQueue(&SaveAudioPara);
						PageStatus = MainMenu;												
						LoadJD_Menu(MainMenu);
						JD_start_wait = 1;//reflash the message icon and so on						
					break;
					
					case KID_10:
						if (++temp9 > 4)
							temp9 = 1;
						SetSwitchIcon(ChangePhoto_Icon, ThemeSelect, 8, 0, 0x02, 0x0e + temp9);
						break;
										
							
					case KID_11:
						if (--temp9 == 0)
							temp9 = 4;		
						SetSwitchIcon(ChangePhoto_Icon, ThemeSelect, 8, 0, 0x02, 0x0e + temp9);
						break;				
				
				}
			break;
			case OClockSetting:
				switch(KeyValue)
				{
					case K_Exit:
						Setting_Menu();
					break;
					case K_OK:
						//save the information and set the correct background
						//save oclock to JD board
						if(CheckDate())
						{
						
							Set_Oclock(R_Clock_Temp.Hour1, R_Clock_Temp.Hour2, R_Clock_Temp.Min1, R_Clock_Temp.Min2, 0, 1);
							Setting_Menu();
							SetTimeDelay = 10;
							JDFunctionCallBack = &SetDate;
							EventHandle_Flag.bits.ReflashTimerCount_F = 1;
						}
						else
							Dissuasion_Tone();
						//Set_Date(R_Date_Temp.Year1,R_Date_Temp.Year2,R_Date_Temp.Year3,R_Date_Temp.Year4,R_Date_Temp.Month1,R_Date_Temp.Month2,R_Date_Temp.Day1,R_Date_Temp.Day2);
					break;
					case K_CallRinger1:
						if(R_Clock_Temp.Hour1 < 2)
						{
							if(R_Clock_Temp.Hour2<9) 
							{
								R_Clock_Temp.Hour2++;
							}
							else
							{
								R_Clock_Temp.Hour2 = 0;
								R_Clock_Temp.Hour1++;
							}
						}
						else
						{
							if(R_Clock_Temp.Hour2<3)
							{
								R_Clock_Temp.Hour2++;
							}
							else
							{
								R_Clock_Temp.Hour2 = 0;
								R_Clock_Temp.Hour1 = 0;
							}
						}
				
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 22, 0, 0x03, 0xde + R_Clock_Temp.Hour1);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 23, 0, 0x03, 0xde + R_Clock_Temp.Hour2);
						break;
					case K_CallRinger2:
						if(R_Clock_Temp.Hour1 > 0)
						{
							if(R_Clock_Temp.Hour2>0)
							{
								R_Clock_Temp.Hour2--;
							}
							else
							{
							
								R_Clock_Temp.Hour2 = 9;
								R_Clock_Temp.Hour1--;
								
							}
						}
						else
						{
							if(R_Clock_Temp.Hour2>0)
							{
								R_Clock_Temp.Hour2--;
							}
							else
							{
								R_Clock_Temp.Hour2 = 3;
								R_Clock_Temp.Hour1 = 2;
							}
						}
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 22, 0, 0x03, 0xde + R_Clock_Temp.Hour1);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 23, 0, 0x03, 0xde + R_Clock_Temp.Hour2);
						break;
					case K_Min1_Inc:
						/*if(R_Clock_Temp.Min1<5)
						{
							R_Clock_Temp.Min1++;
						}
						else
							R_Clock_Temp.Min1 = 0;
						*/
						Add_Circle(&R_Clock_Temp.Min1,5);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 24, 0, 0x03, 0xde + R_Clock_Temp.Min1);
						break;
					case K_Min1_Dec:
						/*if(R_Clock_Temp.Min1>0)
						{
							R_Clock_Temp.Min1--;
						}
						else
							R_Clock_Temp.Min1 = 5;
						*/
						Dec_Circle(&R_Clock_Temp.Min1,5);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 24, 0, 0x03, 0xde + R_Clock_Temp.Min1);
						
					break;
					case K_Min2_Inc:
						/*if(R_Clock_Temp.Min2<9)
						{
							R_Clock_Temp.Min2++;
						}
						else
							R_Clock_Temp.Min2 = 0;
						*/
						Add_Circle(&R_Clock_Temp.Min2,9);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 25, 0, 0x03, 0xde + R_Clock_Temp.Min2);
						break;
					case K_Min2_Dec:
						/*if(R_Clock_Temp.Min2>0)
						{
							R_Clock_Temp.Min2--;
						}
						else
							R_Clock_Temp.Min2 = 9;
						*/
						Dec_Circle(&R_Clock_Temp.Min2,9);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 25, 0, 0x03, 0xde + R_Clock_Temp.Min2);
						break;
					case K_Day2_Inc:
						/*if(R_Date_Temp.Day2<9)
						{
							R_Date_Temp.Day2++;
							
						}
						else
						{
							
							R_Date_Temp.Day2 = 0;
						}*/
						Add_Circle(&R_Date_Temp.Day2,9);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 16, 0, 0x03, 0xde + R_Date_Temp.Day2);
						break;
					case K_Day2_Dec:
						/*if(R_Date_Temp.Day2>0)
						{
							R_Date_Temp.Day2--;
							
						}
						else
							R_Date_Temp.Day2 = 9;
						*/
						//if((R_Date_Temp.Month1==0)&&(R_Date_Temp.Month2==2)&&((10*R_Date_Temp.Year3+R_Date_Temp.Year4)%4==0))
							Dec_Circle(&R_Date_Temp.Day2,9);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 16, 0, 0x03, 0xde + R_Date_Temp.Day2);
						break;
					case K_Day1_Inc:
						/*if(R_Date_Temp.Day1<3)
						{
							R_Date_Temp.Day1++;
						}
						else
						{						
							R_Date_Temp.Day1 = 0;
							
						}*/
						//if((R_Date_Temp.Month1==0)&&(R_Date_Temp.Month2==2))
						//	Add_Circle(&R_Date_Temp.Day1,2);
						//else
							Add_Circle(&R_Date_Temp.Day1,3);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 27, 0, 0x03, 0xde + R_Date_Temp.Day1);
						break;
					case K_Day1_Dec:
						/*if(R_Date_Temp.Day1>0)
						{
							R_Date_Temp.Day1--;
						}
						else
						{
							R_Date_Temp.Day1 = 3;
							
						}
						*/
						//if((R_Date_Temp.Month1==0)&&(R_Date_Temp.Month2==2))
						//	Dec_Circle(&R_Date_Temp.Day1,2);
						//else
							Dec_Circle(&R_Date_Temp.Day1,3);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 27, 0, 0x03, 0xde + R_Date_Temp.Day1);
						break;	
					case K_Month_Inc:
						if(R_Date_Temp.Month1)
						{
							if(R_Date_Temp.Month2<2)
							{
								R_Date_Temp.Month2++;	
							}
							else
							{
								R_Date_Temp.Month2 = 1;
								R_Date_Temp.Month1 = 0;
							}
						}
						else
						{
							if(R_Date_Temp.Month2<9)
							{
								R_Date_Temp.Month2++;	
							}
							else
							{
								R_Date_Temp.Month2 = 0;
								R_Date_Temp.Month1 = 1;
							}
						}

						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 43, 0, 0x03, 0xde + R_Date_Temp.Month1);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 44, 0, 0x03, 0xde + R_Date_Temp.Month2);
						break;
					case K_Month_Dec:
						if(R_Date_Temp.Month1)
						{
							if(R_Date_Temp.Month2>0)
							{
								R_Date_Temp.Month2--;	
							}
							else
							{
								R_Date_Temp.Month2 = 9;
								R_Date_Temp.Month1 = 0;
							}
						}
						else
						{
							if(R_Date_Temp.Month2>0)
							{
								R_Date_Temp.Month2--;	
							}
							else
							{
								R_Date_Temp.Month2 = 2;
								R_Date_Temp.Month1 = 1;
							}
						}

						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 43, 0, 0x03, 0xde + R_Date_Temp.Month1);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 44, 0, 0x03, 0xde + R_Date_Temp.Month2);
						break;
					case K_Year_Inc:
						if(R_Date_Temp.Year4<9)
						{
							R_Date_Temp.Year4++;
						}
						else
						{
							R_Date_Temp.Year4 = 0;
							if(R_Date_Temp.Year3 < 4)
							{
								R_Date_Temp.Year3++;
							}
							else
							{
								R_Date_Temp.Year3 = 0;
							}
						}
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 41, 0, 0x03, 0xde + R_Date_Temp.Year3);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 42, 0, 0x03, 0xde + R_Date_Temp.Year4);
						break;
					case K_Year_Dec:
						if(R_Date_Temp.Year4>0)
						{
							R_Date_Temp.Year4--;
						}
						else
						{
							R_Date_Temp.Year4 = 9;
							if(R_Date_Temp.Year3 > 0)
							{
								R_Date_Temp.Year3--;
							}
							else
							{
								R_Date_Temp.Year3 = 4;
							}
						}
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 41, 0, 0x03, 0xde + R_Date_Temp.Year3);
						SetSwitchIcon(ChangePhoto_Icon, OClockSetting, 42, 0, 0x03, 0xde + R_Date_Temp.Year4);
						break;
					
				}
			break;
			case ResetSelect:
				switch(KeyValue)
				{
					case K_Exit:
					case 5:	
						// NO button press
						PageStatus = SettingSelect;
						LoadJD_Menu(SettingSelect);
					break;
					case K_OK:
						
						Flag4.bits.DefaultS = 1;
						
					break;
				}
			break;
			case PSTN_Setting:
				switch(KeyValue)
				{
					case K_Exit:
						
						Setting_Menu();
						//Read_LEDFlag(&Flag3.all);
					break;
					case K_OK:
						//save 
						//SaveInfQueue(&SaveAudioPara);//save enable video or disable video
						SettingPara.PSTN_Call_Melody = temp3;
						F_PSTN_With_Video = temp5;			
						R_kind_of_ringtone_PSTN = F_kind_of_ringtone_Pstn;
						SaveInfQueue(&SaveAudioPara);
						Setting_Menu();
					break;
					
					case 8:
						R_kind_of_ringtone_cito = temp3;    
	        				Play_New_Melody(AUDIO_ONLY_CALL);
					break;
					case K_CallRinger1:
					
					
						if(temp3 > 1)
						{
							temp3--;
							
							//else SetSwitchIcon(ChangePhoto_Icon, AudioAdjust, 6, 0, 0x00, 0x86 + temp3);
						}
						else
							temp3 = 5;
						R_kind_of_ringtone_cito = temp3;    
						R_Volume_Call = temp1;
	        				Play_New_Melody(AUDIO_ONLY_CALL);
							
						SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 8, 0, 0x01, 0x68 + temp3);
					
					break;
					case K_CallRinger2:
						if(temp3 < 5)
						{
							temp3++;
							
						}
						else
							temp3 = 1;
						R_kind_of_ringtone_cito = temp3;    
						R_Volume_Call = temp1;
	        				Play_New_Melody(AUDIO_ONLY_CALL);							
						SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 8, 0, 0x01, 0x68 + temp3);
							
					break;
					case 9:
					case K_CallRinger3:
					case K_CallRinger4:
						if(temp5)
						{
							temp5 = OFF;
							//SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 9, 0, 0x01, 0x7e);
						}
						else
						{
							temp5 = ON;
							//SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 9, 0, 0x01, 0x7f);
						}
						SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 9, 0, 0x01, 0x7e + temp5);
					break;
					
				}
			break;	
			case SettingSelect: //default for audio setting
			switch(KeyValue)
			{
				case K_Exit:
					
				 	R_Volume_Call = SettingPara.CallVolume;
					R_kind_of_ringtone_cito = SettingPara.RCallMelody;
					PageStatus = MainMenu;
					LoadJD_Menu(MainMenu);
					
					//Flag3.all = LEDFlag;//data[0];
				break;
				case K_OK:
				    	//save the audio parameter
					R_Volume_Call = temp1;
					SettingPara.CallVolume = MaxValue5(temp1);
					SettingPara.AudioVolume = MaxValue5(temp2);	
					SettingPara.RCallMelody = MaxValue5(temp3);
					SettingPara.RFloorCallMelody = MaxValue5(temp4);
					SettingPara.S_Flag.bits.BeepVolume = temp5;
					R_Volume_Call = SettingPara.CallVolume;
					R_kind_of_ringtone_cito = SettingPara.RCallMelody;
					SettingPara.S_Flag.bits.Mute = temp6;
					R_Led_mute = F_Led_mute;
					if(F_Presence_Absence != temp7)
					{
						if(Internal == MASTER_APARTMENT_STATION){
							F_Presence_Absence = temp7;
          						Presence_Absence_Management();    
  						}
					}
					
					SettingPara.S_Flag.bits.PasswordRequest = temp8;
					//SaveAudioPara();
					Flag.bits.SaveAP = 1;
					SetMainMenuState(5, OFF + SettingPara.S_Flag.bits.Mute);
					PageStatus = MainMenu;
					LoadJD_Menu(MainMenu);
					//SavePresencewithMessage(); 
					if(F_Led_mute)
					{						
						SetControl_State(MainMenu, 5, ON);
										    //send iic command set mute light on						 
					}
					else
						SetControl_State(MainMenu, 5, OFF);
					
					if(F_Presence_Absence)
					{						
						SetControl_State(MainMenu, 9, ON);
										    //send iic command set mute light on						 
					}
					else
						SetControl_State(MainMenu, 9, OFF);
				break;
				case 4:
				case 5:
				case 6://for active or deactive the password
					if(temp8)
					{
						temp8 = 0;					
					}
					else
					{
						temp8 = 1;						
					}
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 5, 0, 0x01, 0x80 + temp8);
				break;
				case 9://used for theme setting 
					temp9 = SettingPara.Theme;
					PageStatus = ThemeSelect;
					SetSwitchIcon(ChangePhoto_Icon, ThemeSelect, 8, 0, 0x02, 0x0e + temp9);					
					LoadJD_Menu(ThemeSelect);
				break;				
				case 10:// home automation setting
				// set the correct icon first
				
					SetSwitchIcon(ChangeButton_Icon, HomeAutomation_Setting, 5, 0, 0x01, 0x80 + F_HA_Passwordneed);
					PageStatus = HomeAutomation_Setting;
					LoadJD_Menu(HomeAutomation_Setting);
				break;
				case K_TimeSet:
					//read the time first					
					PageStatus = OClockSetting;					
					ReadTime_From_JDBoard();
					LoadJD_Menu(OClockSetting);					
				break;
				case 12://play current melody
					R_kind_of_ringtone_cito = temp3;    
        				Play_New_Melody(AUDIO_ONLY_CALL);
				break;
				case 13: // play current volume
					TestTone(temp1);
				break;
				case 18:
					if(temp4 > 1)
					{
						temp4--;
					}
					else
					{
						temp4 = 5;
					}
					R_kind_of_ringtone_floor = temp4;  
					Play_New_Melody(FLOOR_CALL);
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 19, 0, 0x01, 0x81 + temp4);
				break;
				
				case 20:
					if(temp4 < 5)
					{
						temp4++;
					}
					else
					{
						temp4 = 1;
					}
					R_kind_of_ringtone_floor = temp4;
					Play_New_Melody(FLOOR_CALL);
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 19, 0, 0x01, 0x81+ temp4);
				break;
				case 19://play current melody for floor call
					R_kind_of_ringtone_floor = temp4;    
        				Play_New_Melody(FLOOR_CALL);
				break;
				case 7:
				case 21:
				case 22:
					if(temp7)
					{
						temp7 = 0;							
					}   
					else
					{
						temp7 = 1;						
					}
					
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 21, 0, 0x01, 0x87 + temp7);	
				break;
				case 27://phone book edit
					PhoneBookPage = 1;										
					ShowPhoneBook_E_D(PhoneBookEditSelect);
										
				break;
				/*
				case 28:
					PageStatus = VideoAdjust;
				break;
				*/
				//case K_Mute:
				case 16:
				case 38:
				case 39:
					if(temp6)
					{
						temp6 = 0;				 						
					}   
					else
					{
						temp6 = 1;				 					
					}
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 16, 0, 0x01, 0x7a + temp6);
				break;
				case 17:// security inco pressing
					//new password
					//SecuritySetting
					if(SettingPara.S_Flag.bits.PasswordRequest)
					{
						Tel_index = 0;
						(void)memset(&Tel_num,0,sizeof(Tel_num));
						PageStatus = new_password;
						LoadJD_Menu(new_password);
					}
					else
					{
						//new input the old password first
						PasswordRequest.all = 0;
						PasswordRequest.bits.SecuritySetting = 1;
						Tel_index = 0;
						(void)memset(&Tel_num,0,sizeof(Tel_num));
						PageStatus = PasswordInput;
						LoadJD_Menu(PasswordInput);
					}
					
				break;
				
					
				case K_CallVolume_Inc:
					if(temp1 < Max_Volume_Levels)
					{
						temp1++;	
					}
					else
					{
						temp1 = 1;
					}
					R_kind_of_ringtone_cito = temp3; 
					TestTone(temp1);
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 13, 0, 0x01, 0x6d + temp1);
					
					
				break;
				
				
				case K_CallVolume_Dec:
					if(temp1 > Min_Volume_Levels)
					{
						temp1--;					
					}
					else
					{
						temp1 = Max_Volume_Levels;
					}
					R_kind_of_ringtone_cito = temp3; 
					TestTone(temp1);
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 13, 0, 0x01, 0x6d + temp1);
						
					
				break;
		
				case K_AudioVolume_Inc:
					if(temp2 < Max_Volume_Levels)
					{
						temp2++;	
					}
					else
					{
						temp2 = 1;
					}
					SetSwitchIcon(ChangePhoto_Icon, SettingSelect, 14, 0, 0x01, 0x72 + temp2);
					
					//LoadAudioAdjust(temp1,temp2,temp3,temp4,temp5);
				break;
				
				case K_AudioVolume_Dec:
					if(temp2 > Min_Volume_Levels)
					{
						temp2--;
						
					}
					else
					{
						temp2 = Max_Volume_Levels;
					}
					SetSwitchIcon(ChangePhoto_Icon, SettingSelect, 14, 0, 0x01, 0x72 + temp2);
				break;
				
				case K_CallRinger1:
					if(temp3 > 1)
					{
						temp3--;
					}
					else
					{
						temp3 = 5;
					}
					R_kind_of_ringtone_cito = temp3;  
					TestTone(temp1);
        				//Play_New_Melody(AUDIO_ONLY_CALL);
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 12, 0, 0x01, 0x68 + temp3);
				break;
				
				case K_CallRinger2:
					if(temp3 < 5)
					{
						temp3++;
					}
					else
					{
						temp3 = 1;
					}
					R_kind_of_ringtone_cito = temp3;
					TestTone(temp1);
        				//Play_New_Melody(AUDIO_ONLY_CALL);
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 12, 0, 0x01, 0x68 + temp3);
				break;
				
					
				case 15:
				case K_Beep_ON:
				case K_Beep_OFF:
					if(temp5 == ON)
					{
						temp5 = OFF;
					}
					else
					{
						temp5 = ON;
						
					}
					SetSwitchIcon(ChangeButton_Icon, SettingSelect, 15, 0, 0x01, 0x78 + temp5);
				break;
		
				case K_Video_Setting:
					PageStatus = VideoAdjust;
					Sub_MODO_State = Mornitor_PCM1;
				break;
				case K_Reset:
					PageStatus = ResetSelect;
					//ResetMenuSelect(1);;
					LoadJD_Menu(ResetSelect);
					KNX_Buf[0] = (FW_VERSION>>4) + 0x30;
					KNX_Buf[1] = '.';
					KNX_Buf[2] = (FW_VERSION&0x0F) + 0x30;
					#ifdef Release
					KNX_Buf[3]  = 'A';//(Sub_Version&0x0F) + 0x30;//Sub_Version; //sub fixed version
					SetTel_Num(ResetSelect,6,4,KNX_Buf);
					//SetTel_Num(ResetSelect,10,4,KNX_Buf);
					
					#else
					KNX_Buf[3]  = 'T';
					SetTel_Num(ResetSelect,6,4,KNX_Buf);
					//SetTel_Num(ResetSelect,10,4,KNX_Buf);
					#endif
					
				break;
				case K_PSTN_Setting://
				
					temp3 = SettingPara.PSTN_Call_Melody;
					temp5 = F_PSTN_With_Video;
					
					SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 8, 0, 0x01, 0x68 + temp3 );
					SetSwitchIcon(ChangeButton_Icon, PSTN_Setting, 9, 0, 0x01, 0x7e + F_PSTN_With_Video);					
					PageStatus = PSTN_Setting;
					LoadJD_Menu(PSTN_Setting);
					//SettingPara.RFloorCallMelody = temp3;
				break;
				default:
					return;
			}
			break;
			case VideoAdjust:
				switch(KeyValue)
				{
					case K_Exit:
						
						Sub_MODO_State = EX_IDLE;
						//OLD_MODO_State = IDLE;
						PageStatus = SettingSelect;
						LoadJD_Menu(SettingSelect);
						
					break;
					
				
				}
			break;
			case PasswordInput:

			switch(KeyValue)
			{
				case K_Exit:
					//StairLight();
					PageStatus = MainMenu;
					LoadJD_Menu(MainMenu);
					//Read_LEDFlag(&Flag3.all);
				break;
				case K_OK:
					//test if passwork is correct
					
						if(CheckPasswordForUser())
						{
							if(PasswordRequest.bits.Setting_F)
							{
								PasswordRequest.bits.Setting_F = 0;
								Setting_Menu();
								
							}
							else if(PasswordRequest.bits.HA_F)
							{
								PasswordRequest.bits.HA_F = 0;
								HA_Menu_Ini();
								//Setting_Menu();
								
							}
							else if(PasswordRequest.bits.SecuritySetting)
							{
								PasswordRequest.bits.SecuritySetting = 0;
								Tel_index = 0;
								(void)memset(&Tel_num,0,sizeof(Tel_num));
								PageStatus = new_password;
								LoadJD_Menu(new_password);
								
							}
								
						}
						else 
						{
							//password error
							Tel_index = 0;
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							SetTel_Num(PasswordInput,7,Tel_index,KNX_Buf);
							Dissuasion_Tone();
						}
					
				break;
				case 4:
					if(Tel_index > 0)
					{
						Tel_index--;
						SetTel_Num(PasswordInput,7,Tel_index,KNX_Buf);
					}
					break;
				case 8:
					if(Tel_index > 0)
					{
						Tel_index = 0;
						SetTel_Num(PasswordInput,7,Tel_index,KNX_Buf);
					}
				break;
				case 42:
				case 43:
				case K_Num0:
				case K_Num1:
				case K_Num2:
				case K_Num3:
				case K_Num4:
				case K_Num5:
				case K_Num6:
				case K_Num7:
				case K_Num8:
				case K_Num9:
					if(Tel_index<6)
					{	
						//display the numbers	
						
						Tel_num[Tel_index] = KeyValue;
						KNX_Buf[Tel_index] = '*';
						Tel_index++;
						SetTel_Num(PasswordInput,7,Tel_index,KNX_Buf);

						
						
					}
				break;
			}
			break;
			case new_password:
				switch(KeyValue)
				{
					case K_Exit:
						//StairLight();
						PageStatus = SettingSelect;
						LoadJD_Menu(SettingSelect);
					break;
					case K_OK:
						//test if passwork is correct
						
							if(Tel_index>0)
							{
								//save the new password
								SaveNewPassword();
								PageStatus = SettingSelect;
								LoadJD_Menu(SettingSelect);
							}
							else
								Dissuasion_Tone();
					break;
					case 4:
						if(Tel_index > 0)
						{
							Tel_index--;
							SetTel_Num(new_password,7,Tel_index,KNX_Buf);
						}
						break;
					case 8:
						if(Tel_index > 0)
						{
							Tel_index = 0;
							SetTel_Num(new_password,7,Tel_index,KNX_Buf);
						}
					break;
					case 42:
					case 43:
					case K_Num0:
					case K_Num1:
					case K_Num2:
					case K_Num3:
					case K_Num4:
					case K_Num5:
					case K_Num6:
					case K_Num7:
					case K_Num8:
					case K_Num9:
						if(Tel_index<6)
						{	
							//display the numbers							
							Tel_num[Tel_index] = KeyValue;
							KNX_Buf[Tel_index] = '*';
							Tel_index++;
							SetTel_Num(new_password,7,Tel_index,KNX_Buf);
						}
						else
							Dissuasion_Tone();
					break;
				}
			break;
				
			
			case HomeAutomation_Setting:
				switch(KeyValue)
				{
					case K_Exit:
						//StairLight();
						//PageStatus = AudioAdjust;
						//LoadJD_Menu(AudioAdjust);
						Setting_Menu();
					break;
					case 4:
					case 5:
					case 6:					
						if(F_HA_Passwordneed)
						{
							F_HA_Passwordneed = 0;
							//SavePresencewithMessage();  
							//SetSwitchIcon(ChangeButton_Icon, HomeAutomation_Setting, 5, 0, 0x01, 0x80);
						}
						else
						{
							F_HA_Passwordneed = 1;
							//SavePresencewithMessage();  
							//SetSwitchIcon(ChangeButton_Icon, HomeAutomation_Setting, 5, 0, 0x01, 0x81);
						}
						//SavePresencewithMessage();  
						Flag.bits.SaveAP = 1;
						SetSwitchIcon(ChangeButton_Icon, HomeAutomation_Setting, 5, 0, 0x01, 0x80 + F_HA_Passwordneed);
					break;
					case 7:
						//Timer Event Select
						
						PageStatus = Timer_Event_Edit_Event;
						LoadJD_Menu(Timer_Event_Edit_Event);
						
					break;
				}
			break;
			case VideoMessagePlaying:   //anwsering machine
				switch(KeyValue)
				{
					case K_Exit:
					//case K_All_Exit:	
						//unchange

					  	Free_Line();
						//modo should exit to main menu
					  	//ExitToWaitPage();
					  
					break;
				  	//long press ok button, then send the delete frame
					case K_OK:
						//skip
						//Answering_Machine_Function();	
						//S_SVC_NEXT(); 
						p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
					        p_der_queue_tx_appl->n_byte = 2;
					        p_der_queue_tx_appl->data[D_OPC] = D_SVC_NEXT;
					        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
					        Incr_index_queue_in_der();                      // update index queue
					        //Main_GUI_locked = 1;
					        //Timer_GUI_locking = TIME_LOCK_GUI;
					break;
				  	
					case 15:
				  		//delete one message
						//if(SVC_number){    // to avoid wrong deleting operations
							p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
			    				p_der_queue_tx_appl->n_byte = 3;
			    				p_der_queue_tx_appl->data[D_OPC] = D_SVC_DELETE;
			    				p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal;
			    				p_der_queue_tx_appl->data[D_DATA_1] = number_SVC; 
			    				Incr_index_queue_in_der();                       
						//}
				  	break;
					default:
						
					break;
				}
			break;
			
			case Talk_To_CallModule_NotHide:
				switch(KeyValue)
				{
					case K_Exit:
						LoadJD_Menu(Talk_To_CallModule_NotHide);
					break;
					case K_AutoSwitch:
						Button_Selfinsertion_Cyclic();	
					break;
					case K_LightTurnOn:
						StairLight();
					break;
					
					case K_Door_Open_1:
						Opendoor_Management(1);	
					break;
					case K_Door_Open_2:
						Opendoor_Management(2);	
					break;
					case K_Setting:
						
					break;
					case K_AudioVolume_Inc:
						if(SettingPara.AudioVolume < Max_Volume_Levels)
						{
							SettingPara.AudioVolume++;						
							SetAudio();	
						}
						Init_VolumeBar(Talk_To_CallModule_NotHide);
					break;
					case K_AudioVolume_Dec:
						if(SettingPara.AudioVolume > Min_Volume_Levels)
						{
							SettingPara.AudioVolume--;						
							SetAudio();	
						}
						Init_VolumeBar(Talk_To_CallModule_NotHide);
					break;
					case K_MicroPhone_Mute:
						MicroPhoneMutePress(Talk_To_CallModule_NotHide,K_MicroPhone_Mute);					
					break;
					case K_AnswerCall:
						if(!Delay_Hook_Press_Time)
						{
							if(MODO_State == AUDIO_ONLY_OFF_HOOK_WAIT || MODO_State == VIDEO_OFF_HOOK_WAIT || MODO_State == INTERCOM_OFF_HOOK_WAIT)
							{
								PCM_Audio_Switch = To_Modo;
								AudioButtonPress();
							}
							else if(PCM_Audio_Switch == To_HandSet)
							{
								PCM_Audio_Switch = To_Modo;
							}
							else
							{
								AudioButtonPress();
							}
						}
						else
							Dissuasion_Tone();
						//LoadTalkToCallModuleMenu(F_TalkToCallModuleMenu_NotHide,MenuNotHide, Rationswitch);
					break;
					case K_AnsweringService: //form handset
						if(MODO_State == AUDIO_ONLY_OFF_HOOK_WAIT || MODO_State == VIDEO_OFF_HOOK_WAIT || MODO_State == INTERCOM_OFF_HOOK_WAIT)
						{
							PCM_Audio_Switch = To_HandSet;
							AudioButtonPress();
						}
						else if(PCM_Audio_Switch == To_HandSet)
						{
							AudioButtonPress();
						}
							
					break;
					
					
				}
			break;
			case Menu_CCTV:
				switch(KeyValue)
				{
					case K_Exit:											
						LoadJD_Menu(Menu_CCTV);
						CCTV_Init();
						
						if(SetTimeDelay)
							JDFunctionCallBack2 = &CCTV_Init;
						else
							CCTV_Init();	
						
					break;
					case K_OK:
						//StairLight();
						Sub_MODO_State = IDLE;	
						PageStatus = BackMenu;
						F_Back_Menu();
						
						LoadJD_Menu(PageStatus);
						OLD_MODO_State = MODO_State;
					
					break;
					case 11:
					case 12:	
					case 13:
					case 14:
						//CCTV output active
						CCTVOutput(KeyValue-11);
					break;
					default:
						SetTimeDelay = 30;
					return;
				}
				break;
 			case Menu_Telephone_Video:
				switch(KeyValue)
				{
					case K_Exit:
						//exit form video setting
						LoadJD_Menu(Menu_Telephone_Video);
						
					break;
					case K_PSTN_Answer_HandSet:
						Tel_HookOFF_Do_Video(To_HandSet);
					break;
					case K_OK://hook off
						Tel_HookOFF_Do_Video(To_Modo);
					break;
					
					//case K_All_Exit://hook on
					/*
						if(DTMF_State == Tel_Talk)
						{
							DTMF_State = DTMF_Idle;
							MODO_State = IDLE;
							OLD_MODO_State = IDLE;
							PageStatus = MainMenu;
							LoadJD_Menu(MainMenu);
							
						}
						*/
					//break;
					case 6:
						
						if(Sub_MODO_State == PSTN_Calling_Video)
						{
							Sub_MODO_State = PSTN_Calling;
							SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB0);
						}
						else if(Sub_MODO_State == PSTN_Talking_Video)
						{
							Sub_MODO_State = PSTN_Talking;
							SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
						}
						else
							break;
						TelRdFrameTime=100;
						CheckDTMF_Delay = 0;

					break;
					case 7:
						if(++temp9 > 3)
						{
							temp = 0;
						}									
						CCTVOutput(temp9);
					break;
					case 11:
					case K_Door_Open_2:
						if(Sub_MODO_State == PSTN_Talking_Video)
						{
							EventHandle_Flag.bits.PSTN_Video_State_Hope = 1;
							DTMF_Key_Out_Put();
						}
					break;
										
					case K_AudioVolume_Inc:
					if(SettingPara.AudioVolume < Max_Volume_Levels)
					{
						SettingPara.AudioVolume++;
						//set audio volume
						//LoadAudio_VideoSetting(temp4,temp1,temp2,temp3);
						//wait UD code
						//Calculate_Volume_and_update_Conexant(temp4*28);
						/*
						ClearVolume();
						SetVolumeChange(1,SettingPara.AudioVolume);
						SaveA_Vsetting();
						*/
						SetAudio();
						//display the icon of volume
						//SetControl_State(Talk_To_CallModule_NotHide, SettingPara.AudioVolume, ON);
						
						
						
					}
					Init_VolumeBar(Menu_Telephone_Video);
					break;
					case K_AudioVolume_Dec:
						if(SettingPara.AudioVolume > Min_Volume_Levels)
						{
							//SetControl_State(Talk_To_CallModule_NotHide, SettingPara.AudioVolume, OFF);
							SettingPara.AudioVolume--;
							
							//LoadAudio_VideoSetting(temp4,temp1,temp2,temp3);
							//wait UD code
							//Calculate_Volume_and_update_Conexant(temp4*28);
							/*
							ClearVolume();
							SetVolumeChange(1,SettingPara.AudioVolume);
							SaveA_Vsetting();
							*/
							SetAudio();
							
						}
						Init_VolumeBar(Menu_Telephone_Video);
					break;
					case K_MicroPhone_Mute:
						MicroPhoneMutePress(Menu_Telephone_Video,K_MicroPhone_Mute);						
					break;
				}
				break;
			case PCM_Auto_Insert:
				switch(KeyValue)
				{
					case K_Exit:
						LoadJD_Menu(PCM_Auto_Insert - 1);
					break;
					case K_OK:// as the key hook on or off
						if(Sub_MODO_State == PCM1CALLING || Sub_MODO_State == Mornitor_PCM1)
						{
							Sub_MODO_State = IN_COMMUNICATION_PCM1;
							PCM_Audio_Switch = To_Modo;
							//set icon blink
							SetButton_State(0x01, Menu_Telephone_Video, 3, ON);//icon blink
						}
						else if(Sub_MODO_State == IN_COMMUNICATION_PCM1 && PCM_Audio_Switch == To_HandSet)
						{
							PCM_Audio_Switch = To_Modo;
							SetButton_State(0x01, Menu_Telephone_Video, 3, ON);// blink
						}
						else
						{
							Sub_MODO_State = EX_IDLE;
							OLD_MODO_State = 0xff;
							//SetButton_State(0x01, Menu_Telephone_Video, 3, OFF);//not blink
						}
							
					break;
					case 7://posible to next cam
						if(Sub_MODO_State == Mornitor_PCM1 && F_PCM_Number > 1)
						{
							//next channel when connect with 34
							Pin_IO_Set_Delay = 50;
							TC_PCM1 = OFF;
						}
						else
							Dissuasion_Tone();
					break;
					case 11:
						if(!PCM1_Lock)
						{
							PCM1_Lock = ON;	
							PCM_LOCK_TIME = F_PCM_Door_Open_Time;
						}
					break;
					case K_AudioVolume_Inc:
						if(SettingPara.AudioVolume < Max_Volume_Levels)
						{
							SettingPara.AudioVolume++;						
							SetAudio();																		
						}
						Init_VolumeBar(PCM_Auto_Insert -1);
					break;
					case K_AudioVolume_Dec:
						if(SettingPara.AudioVolume > Min_Volume_Levels)
						{
							SettingPara.AudioVolume--;							
							
							SetAudio();
							
						}
						Init_VolumeBar(PCM_Auto_Insert -1);
					break;
					case K_MicroPhone_Mute:
						MicroPhoneMutePress(PCM_Auto_Insert -1,K_MicroPhone_Mute);						
					break;
					
					case K_PSTN_Answer_HandSet:
						if(Sub_MODO_State == PCM1CALLING)
						{
							PCM_Audio_Switch = To_HandSet;
							Sub_MODO_State = IN_COMMUNICATION_PCM1;
						}
						else if(Sub_MODO_State == IN_COMMUNICATION_PCM1 && PCM_Audio_Switch == To_HandSet)
						{
							Sub_MODO_State = EX_IDLE;
							OLD_MODO_State = 0xff;
						}
					break;
					
				}
			break;
			case PSTN_Telephone:
				switch(KeyValue)
				{
					case K_MenuButton://redial
						if((DTMF_State == DTMF_Idle)&&(!PSTN_Call_TimeOut_Cnt))
						{
							if(Memory_S.Tel_Log_Number[Tel_Log_Index].Tel_Num_Len)
							{
								Tel_index = Memory_S.Tel_Log_Number[Tel_Log_Index].Tel_Num_Len;
									(void)memcpy(KNX_Buf,Memory_S.Tel_Log_Number[Tel_Log_Index].Tel_Number,Tel_index);
									for(temp1 = 0; temp1 < Tel_index; temp1++)
									{
										if(KNX_Buf[temp1] == 0x30)
										{
											KNX_Buf[temp1] = 0x0a;
										}
										else if(KNX_Buf[temp1] == '*')
										{
											KNX_Buf[temp1] = 0x0b;
										}
										else if(KNX_Buf[temp1] == '#')
										{
											KNX_Buf[temp1] = 0x0c;
										}
										else
										{
											KNX_Buf[temp1]-=0x30;
										}
									}
									(void)memset(&Tel_num,0,sizeof(Tel_num));
									(void)memcpy(&Tel_num,&KNX_Buf,Tel_index);
									ShowTelNum();
									//call directly
									KeyValue = K_TelHookOff;
									Flag.bits.KeyNhandle = 1;
									
							}
						}
						break;
					case K_Exit:
					case K_OK:
						//StairLight();

						if(Tel_index>0)
						{
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							ShowTelNum();
						}
						Tel_index = 0;
						
						DTMF_State = DTMF_Idle;
						Sub_MODO_State = IDLE;
						OLD_Sub_Modo_State = IDLE;
						Tel_index = 0;
						PageStatus = MainMenu;
						LoadJD_Menu(MainMenu);
					break;
					case 4:// DTMF Key
					//case K_Door_Open_2:
						if(Sub_MODO_State == PSTN_Talking)
						{
							//DTMF key
							
							DTMF_Key_Out_Put();
						}
					break;
					
					case 6://contact list
						if(Sub_MODO_State == IDLE)
						{
							// show the telephone number
							Show_Tel_List();
						}
						else
							Dissuasion_Tone();
					break;
					case 7:
						//call log
						if(Sub_MODO_State == IDLE)
						{
							PageStatus = Tel_Call_Log;
							LoadJD_Menu(Tel_Call_Log);
							temp2 = Tel_Log_Index;
							for(temp1 = 0; temp1 < 5; temp1++)
							{
								
								Tel_num_Record = &Memory_S.Tel_Log_Number[temp2];
								if(Tel_num_Record->Tel_Num_Len>0 && Tel_num_Record->Tel_Num_Len<17)
								{
									SetTel_Num(Tel_Call_Log,13+temp1,Tel_num_Record->Tel_Num_Len,Tel_num_Record->Tel_Number);
								}
								if(temp2>0)
								{
									temp2--;
								}
								else
									temp2 = 4; //due to we have 5 reacords
							}
							temp2 = Tel_Log_Index;
							for(temp1 = 0; temp1 < 5; temp1++)
							{
								Tel_num_Record = &Memory_S.Tel_Log_Number[temp2];
								if(Tel_num_Record->Tel_Num_Len>0 && Tel_num_Record->Tel_Num_Len<17)
								{
									SetTel_Num(Tel_Call_Log,13+temp1,Tel_num_Record->Tel_Num_Len,Tel_num_Record->Tel_Number);
									return;
								}
								if(temp2>0)
								{
									temp2--;
								}
								else
									temp2 = 4; //due to we have 5 reacords
							}
						}
						else
							Dissuasion_Tone();
					break;
					case 8://delete
						if(Tel_index)
						{
							Tel_index--;
							SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+Tel_index, 0, 0x00, 26);
							
						}
					break;
					//case K_AnsweringService:
					
					case K_PSTN_Answer_HandSet:
						Tel_HookOFF_Do(To_HandSet);
					break;
					case K_TelHookOff:
						Tel_HookOFF_Do(To_Modo);
						break;
					case K_TelHookOn:
						
						if(Tel_index>0)
						{
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							ShowTelNum();
						}
						Tel_index = 0;
					break;
					
					case 12://switch video
					
						if(Sub_MODO_State == PSTN_Calling || Sub_MODO_State == PSTN_Talking)
						{
							if(F_PSTN_With_Video)
							{
								//Sub_MODO_State = PSTN_Calling_Video;
								Sub_MODO_State += 2;
							}
							else
								Dissuasion_Tone();
						}
						else
							Dissuasion_Tone();
						/*
						else if(Sub_MODO_State == PSTN_Talking)
						{
							if(F_PSTN_With_Video)
							{
								Sub_MODO_State = PSTN_Talking_Video;
							}
							else
								Dissuasion_Tone();
						}
						*/
					break;
					case 13:
						if(SettingPara.AudioVolume < Max_Volume_Levels)
						{
							SettingPara.AudioVolume++;
							SetAudio();
						}
						Init_VolumeBar(PSTN_Telephone);
					break;
					case 14:
						if(SettingPara.AudioVolume > Min_Volume_Levels)
						{						
							SettingPara.AudioVolume--;							
							SetAudio();	
						}
						Init_VolumeBar(PSTN_Telephone);
					break;
					case 16:
						MicroPhoneMutePress(PSTN_Telephone,16);						
					break;	
					case K_TelCallStar:
						if(DTMF_State == Tel_Talk)
						{
							KNX_Buf[0] = 0x0b; // *
							KNX_Buf[1] = 0xff; // stop
							//KNX_Buf[2] = 0xff;//end
							DTMF_State = DTMF_Ready;
						}
						
						/*if(ClearTelDispFlag==1)
						{
							ClearTelDispFlag=0;
							Tel_index=16;
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							ShowTelNum();
							Tel_index = 0;
						}*/
						
						if(Tel_index<16)
						{
							SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+Tel_index, 0, 0x00, 22);
							//Tel_index++;
							Tel_num[Tel_index++] = 0x0b;
						}
						break;
					case K_TelCallShart:
						if(DTMF_State == Tel_Talk)
						{
							KNX_Buf[0] = 0x0c; // #
							KNX_Buf[1] = 0xff; // stop
							//KNX_Buf[2] = 0xff;//end
							DTMF_State = DTMF_Ready;
						}
						/*if(ClearTelDispFlag==1)
						{
							ClearTelDispFlag=0;
							Tel_index=16;
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							ShowTelNum();
							Tel_index = 0;
						}*/
						
						if(Tel_index<16)
						{
							SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+Tel_index, 0, 0x00, 25);
							Tel_num[Tel_index++] = 0x0c;
						}
						break;	
					case K_TelCall1:
					case K_TelCall2:	
					case K_TelCall3:
					case K_TelCall4:
						Tel_num_Record = ((Tel_Memory_Record *)(EE_Page_Tel_Momory + ((KeyValue- K_TelCall1)*sizeof(Tel_Memory_Record))));
						if(Tel_num_Record->Tel_Num_Len>0 && Tel_num_Record->Tel_Num_Len<16)
						{
							//SetTel_Num(Tel_Contact_List,40+temp1,Tel_num_Record->Tel_Num_Len,Tel_num_Record->Tel_Number);
							Tel_index = Tel_num_Record->Tel_Num_Len;
							(void)memcpy(KNX_Buf,Tel_num_Record->Tel_Number,Tel_index);
							for(temp1 = 0; temp1 < Tel_index; temp1++)
							{
								if(KNX_Buf[temp1] == 0x30)
								{
									KNX_Buf[temp1] = 0x0a;
								}
								else if(KNX_Buf[temp1] == '*')
								{
									KNX_Buf[temp1] = 0x0b;
								}
								else if(KNX_Buf[temp1] == '#')
								{
									KNX_Buf[temp1] = 0x0c;
								}
								else
								{
									KNX_Buf[temp1]-=0x30;
								}
							}
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							(void)memcpy(&Tel_num,&KNX_Buf,Tel_index);
							ShowTelNum();
						}
						else
							Dissuasion_Tone();
						
					break;
						
					case K_Num1://fa
					case K_Num2:
					case K_Num3:
					case K_Num4:
					case K_Num5:
					case K_Num6:
					case K_Num7:
					case K_Num8:
					case K_Num9:
					case K_K_T0:// key 0
						if(DTMF_State == Tel_Talk)
						{
							KNX_Buf[0] = KeyValue - K_Num0; // 4
							KNX_Buf[1] = 0xff; // stop
							
							DTMF_State = DTMF_Ready;
						}

						/*if(ClearTelDispFlag==1)
						{
							ClearTelDispFlag=0;
							Tel_index=16;
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							ShowTelNum();
							Tel_index = 0;
						}*/
						
						if(Tel_index<16)
						{	
							SetSwitchIcon(ChangePhoto_Icon, PSTN_Telephone, 60+Tel_index, 0, 0x00, 10 + KeyValue - K_Num0);
							Tel_num[Tel_index++] = KeyValue - K_Num0;
						}
						else
						{
							if(DTMF_State != Tel_Talk)
								Dissuasion_Tone();
						}
					break;
					
					
				}
				break;
			case PSTN_Telephone_Edit:
				switch(KeyValue)
				{
					case K_OK:
						//save the data
						SaveInfQueue(&SaveTelephoneNumber);  //SaveTelephoneNumber();
						SetTimeDelay = 6;
					case K_Exit:
					
						//StairLight();
						/*
						PageStatus = Tel_Contact_List;
						LoadJD_Menu(Tel_Contact_List);
						FillListData();
						*/
						JDFunctionCallBack2 = &Show_Tel_List;
					break;
					case 4: //delete
						if(Tel_index>0)
						{	
							//display the numbers							
							Tel_index--;
							SetTel_Num(PSTN_Telephone_Edit,7,Tel_index,Tel_num);
						}
					break;
					case 8:
						//delete all
						if(Tel_index>0)
						{	
							//display the numbers							
							Tel_index = 0;
							SetTel_Num(PSTN_Telephone_Edit,7,Tel_index,Tel_num);
						}
						break;
					break;
					case 35://#
					case 42://*
					case K_Num0:
					case K_Num1://fa
					case K_Num2:
					case K_Num3:
					case K_Num4:
					case K_Num5:
					case K_Num6:
					case K_Num7:
					case K_Num8:
					case K_Num9:
					
						if(Tel_index<16)
						{	
							//display the numbers							
							Tel_num[Tel_index++] = KeyValue;
							SetTel_Num(PSTN_Telephone_Edit,7,Tel_index,Tel_num);
						}
						else
							Dissuasion_Tone();
					break;

					
				}
				break;
			case Home_Automation:
				switch(KeyValue)
				{
					case K_Exit:

					case K_OK:
						if(JD_Save_Frame_Index == JD_TX_Frame_Index)
						{
							if(ExitMenu == NULL)
							{
								PageStatus = MainMenu;
								LoadJD_Menu(MainMenu);
							}
							else
							{
								(*ExitMenu)();
								ExitMenu = NULL;	
								//save event
							}
						}
						else
							Dissuasion_Tone();
					break;
					case 5: //
					case 6: 
					case 7: 
					case 8: 
					case 9: 
					case 10: //
					case 11: 
					case 12: 
					case 13: 
						//scenario output
						ScenarioOutput(KeyValue-5, Edit_Index);
						
						//
						
					break;						
					
					case 14:
					case 15:
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(RoomIndex)
							{
								RoomIndex = 0;
								//SetSwitchIcon(ChangeButton_Icon,Home_Automation,14,0,0,42);
							}
							else
							{
								RoomIndex = 1;
								//SetSwitchIcon(ChangeButton_Icon,Home_Automation,14,0,0,40);
							}
							//update icon for room list
							RoomType_Init(RoomIndex,1); 
						}
						break;
					case 20:
					case 21://next scenario page menu
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(Edit_Index)
							{
								Edit_Index = 0;
								//SetSwitchIcon(ChangeButton_Icon,Home_Automation,21,0,0,42);
							}
							else
							{
								Edit_Index = 1;
								//SetSwitchIcon(ChangeButton_Icon,Home_Automation,21,0,0,40);
							}
							ScenarioMenu_Init(Edit_Index,1);
						}						
						break;
					case 22://room select
					case 23:
					case 24:
					case 25:
					case 26:
					case 27:
						Filter_ID = T_All_Device;
						RoomSelect = KeyValue - 22 + RoomIndex*6;
						temp2 = HomeHA_RoomButtonWithFilter_Check(RoomSelect,Filter_ID);
						if(temp2)
						{
							Object_Button_Same_Cnt = temp2;
							Button_Index = 0;

							CheckMenu(Button_Index);
							PageStatus = HomeAutomationDevice_List1;
							HA_Button_Init_CallBack_Filter();
							HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
							//PageStatus = HomeAutomationDevice_List1;
							//JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
						}
						else 
							Dissuasion_Tone();
						break;
					
				}
				break;
			
			case AlarmStatus_Menu:
				switch(KeyValue)
				{
					//case 2:
					case 2:
						//StairLight();
						PageStatus = MainMenu;///demo
						LoadJD_Menu(MainMenu);					
						
					break;

					case 3:			
						PageStatus = AlarmSystem_Manage;///demo
						LoadJD_Menu(AlarmSystem_Manage);	
						AlarmSystemZoneName_Init();
						
					break;
				
				}
			break;

			case AlarmSystem_Manage:
				switch(KeyValue)
				{
					case K_Exit:
						//exit the role
						
					case K_OK:
						//StairLight();
						if(!Alarm_Delay_Tone_Cnt)
						{
							PageStatus = MainMenu;
							LoadJD_Menu(MainMenu);
							RoleAlarmSystem = No_regedit;
						}
					break;
					case K_Zone1:
					case K_Zone2:
					case K_Zone3:
					case K_Zone4:
					case K_Zone5:
					case K_Zone6:	
						//test role first
						
						if(RoleAlarmSystem == A_User)
						{
							//check the state
							temp1 = AlarmP->AlarmZoneEnable;
							if(temp1 & (0x01<<(KeyValue-K_Zone1)))
							{
								SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, KeyValue, 0, 0x01, 0x93);
								//set disable flag
								FreeInt(temp1, (0x01<<(KeyValue-K_Zone1)));
							}
							else
							{
								SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, KeyValue, 0, 0x01, 0xA6);
								SignalInt(temp1, (0x01<<(KeyValue-K_Zone1)));
							}
							//update the alarm enable flag
							SaveAlarmEnableFlag(temp1);
							Alarm_Logo();
						}
						else
							Dissuasion_Tone();
					break;
					case K_Clear:
						if(Tel_index>0)
						{	
							//display the numbers							
							Tel_index = 0;
							SetTel_Num(AlarmSystem_Manage,7,Tel_index,KNX_Buf);
						}
						break;
					case K_DisArm:
						if(RoleAlarmSystem == A_User)
						{
							
							if(AlarmP->AlarmZoneEnable)
							{
								SaveAlarmEnableFlag(0);
								for(temp1 = 0; temp1 < ZoneNum; temp1++)
								{	//disable all zone								
									SetSwitchIcon(ChangeButton_Icon, AlarmSystem_Manage, K_Zone1+temp1, 0, 0x01, 0x93);
								}
							}

							Alarm_Logo();
						}
						else
							Dissuasion_Tone();
					break;
					case 21: //delete
						if(Tel_index>0)
						{	
							//display the numbers							
							Tel_index--;
							SetTel_Num(AlarmSystem_Manage,7,Tel_index,KNX_Buf);
						}
					break;
					case K_Password_OK:
						if(Tel_index > 0)//support possible any digit password //we always with 6byte password
						{
							//we have 6 byte for password
							//check User password first because the user can set new password by themselves
							if(CheckPasswordForUser())
							{
								RoleAlarmSystem = A_User;
								Long_Tone();
								
								CancleAllDelayAlarm();
							}
							else if(CheckPasswordForInstaller())
							{
								RoleAlarmSystem = Installer;
								Long_Tone();
							}
							else
							{	
								//clear the password
								Dissuasion_Tone();
							}
							Tel_index = 0;
							SetTel_Num(AlarmSystem_Manage,7,Tel_index,KNX_Buf);
						}
						else
						{							
							Dissuasion_Tone();
						}
					break;
					case K_Home_Alarm:
						if(RoleAlarmSystem == A_User)
						{
							
							Home_Alarm();
						}
						else
							Dissuasion_Tone();
					break;
					case K_Away_Alarm:
						if(RoleAlarmSystem == A_User)
						{
						
							Away_Alarm();
						}
						else
							Dissuasion_Tone();
					break;
					case K_AlarmStatus:

						if(RoleAlarmSystem == A_User || RoleAlarmSystem == Installer)
						{
							PageStatus = AlarmStatus_Menu;
							LoadJD_Menu(AlarmStatus_Menu);
							AlarmStatus_Name_Init(AlarmStatus_Menu);
						}
						else
							Dissuasion_Tone();
					break;
					
					case K_Num0:
					case K_Num1://fa
					case K_Num2:
					case K_Num3:
					case K_Num4:
					case K_Num5:
					case K_Num6:
					case K_Num7:
					case K_Num8:
					case K_Num9:
					
						if(Tel_index<6)
						{	
							//display the numbers							
							Tel_num[Tel_index] = KeyValue;
							KNX_Buf[Tel_index] = '*';
							Tel_index++;
							SetTel_Num(AlarmSystem_Manage,7,Tel_index,KNX_Buf);
						}
						else
							Dissuasion_Tone();
					break;
				
				}
			break;
			case User_Scenario_Edit:
				switch(KeyValue)
				{
					case K_Exit:
					case K_OK:
						//StairLight();
						PageStatus = Home_Automation;
						LoadJD_Menu(Home_Automation);
					break;
					
					
				}
			break;
			
			case Tel_Contact_List:
				switch(KeyValue)
				{
					case K_Exit:
					case K_OK:
						//StairLight();
						TelRdFrameTime=100;
						PageStatus = PSTN_Telephone;
						LoadJD_Menu(PSTN_Telephone);
						Tel_MemoryName_Init();
					break;
					case 13:
					case 14:
					case 15:
					case 16:
					case 17:
					case 18:
					case 19:
					case 20:
					case 21:
					case 22:
						Tel_num_Record = ((Tel_Memory_Record *)(EE_Page_Tel_Momory + ((KeyValue- 13)*sizeof(Tel_Memory_Record))));
							if(Tel_num_Record->Tel_Num_Len>0 && Tel_num_Record->Tel_Num_Len<17)
							{
								//SetTel_Num(Tel_Contact_List,40+temp1,Tel_num_Record->Tel_Num_Len,Tel_num_Record->Tel_Number);
								Tel_index = Tel_num_Record->Tel_Num_Len;
								(void)memcpy(KNX_Buf,Tel_num_Record->Tel_Number,Tel_index);
								for(temp1 = 0; temp1 < Tel_index; temp1++)
								{
									if(KNX_Buf[temp1] == 0x30)
									{
										KNX_Buf[temp1] = 0x0a;
									}
									else if(KNX_Buf[temp1] == '*')
									{
										KNX_Buf[temp1] = 0x0b;
									}
									else if(KNX_Buf[temp1] == '#')
									{
										KNX_Buf[temp1] = 0x0c;
									}
									else
									{
										KNX_Buf[temp1]-=0x30;
									}
								}
								(void)memset(&Tel_num,0,sizeof(Tel_num));
								(void)memcpy(&Tel_num,&KNX_Buf,Tel_index);
								ShowTelNum();
								SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
								TelRdFrameTime=100;
								PageStatus = PSTN_Telephone;
								LoadJD_Menu(PSTN_Telephone);
								JDFunctionCallBack = &Tel_MemoryName_Init;
								KeyValue = 10;
								Flag.bits.KeyNhandle = 1;
							}
							else
								Dissuasion_Tone();
						break;
						
					case 23:
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
					case 30:
					case 31:
					case 32:
					//load the data and display
						Edit_Index = KeyValue - 23;
						PageStatus = PSTN_Telephone_Edit;
						LoadJD_Menu(PSTN_Telephone_Edit);
						
						Tel_num_Record = ((Tel_Memory_Record *)(EE_Page_Tel_Momory + (Edit_Index*sizeof(Tel_Memory_Record))));
						
						if(Tel_num_Record->Tel_Num_Len >0 && Tel_num_Record->Tel_Num_Len <17)
						{
							Tel_index = Tel_num_Record->Tel_Num_Len;
							(void)memcpy(Tel_num,Tel_num_Record->Tel_Number,Tel_index);
							SetTel_Num(PSTN_Telephone_Edit,7,Tel_index,Tel_num);
							SetTel_Num(PSTN_Telephone_Edit,7,Tel_index,Tel_num);//JD always miss the first frame
						}
						else
						{
							Tel_index = 0;
						}
					break;
				}
			break;
			case Tel_Call_Log:
				switch(KeyValue)
				{
					case K_Exit:
					case K_OK:
						//StairLight();
						TelRdFrameTime=100;
						PageStatus = PSTN_Telephone;
						LoadJD_Menu(PSTN_Telephone);
						JDFunctionCallBack = &Tel_MemoryName_Init;
					break;
					case 8:
					case 9:
					case 10:
					case 11:
					case 12:
						//get the right value of log
						temp2 = Tel_Log_Index;
						for(temp1 = 0; temp1 < (KeyValue-8); temp1++)
						{
							if(temp2>0)
							{
								temp2--;
							}
							else
								temp2 = 4; //due to we have 5 reacords
						}
						if(Memory_S.Tel_Log_Number[temp2].Tel_Num_Len)
						{
							Tel_index = Memory_S.Tel_Log_Number[temp2].Tel_Num_Len;
							(void)memcpy(KNX_Buf,Memory_S.Tel_Log_Number[temp2].Tel_Number,Tel_index);
							for(temp1 = 0; temp1 < Tel_index; temp1++)
							{
								if(KNX_Buf[temp1] == 0x30)
								{
									KNX_Buf[temp1] = 0x0a;
								}
								else if(KNX_Buf[temp1] == '*')
								{
									KNX_Buf[temp1] = 0x0b;
								}
								else if(KNX_Buf[temp1] == '#')
								{
									KNX_Buf[temp1] = 0x0c;
								}
								else
								{
									KNX_Buf[temp1]-=0x30;
								}
							}
							(void)memset(&Tel_num,0,sizeof(Tel_num));
							(void)memcpy(&Tel_num,&KNX_Buf,Tel_index);
							ShowTelNum();
							SetSwitchIcon(ChangeButton_Icon, PSTN_Telephone, K_TelHookOff, 0, 0x01, 0xB1);
							TelRdFrameTime=100;
							PageStatus = PSTN_Telephone;
							LoadJD_Menu(PSTN_Telephone);
							JDFunctionCallBack = &Tel_MemoryName_Init;
							KeyValue = K_TelHookOff;
							Flag.bits.KeyNhandle = 1;
						}
					break;
				}
			break;
			case Alarm_handle:
				switch(KeyValue)
				{
					case K_Exit:
					case K_OK:
						//StairLight();
						WarningToneNumber = 0;  //cancle the warning tone

						//clear the flag which need reset
						AlarmNeed_Reset_Flag &= ~(1<<(AlarmDisplay_Channel -1));
						AlarmDisplay_Channel = 0;
						//show next alarm which is save in flash
						ShowNextAlarm();
						if(!AlarmDisplay_Channel)
						{
							Sub_MODO_State = EX_IDLE;
							PageStatus = MainMenu;
							LoadJD_Menu(MainMenu);
							
						}
						
					break;
					
				}
			break;
			
			case HomeAutomationDevice_List1:
				switch(KeyValue)
				{
					case K_Exit:					
						//StairLight();															
						PageStatus = Home_Automation;///demo
						LoadJD_Menu(Home_Automation);
						ScenarioMenu_Init(Edit_Index,0);
						JDFunctionCallBack = &RoomType_Init_Back0;
					break;
					case K_OK://for up or down arrow
					case K_All_Exit:
													
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							KeyLimitDly=15;
							
							if(Button_Index)
							{
								Button_Index = 0;
							}
							else
							{
								if(Object_Button_Same_Cnt > MaxObject_In_OneMenu)
								{
									Button_Index = 1;
								}
								else
								{
									Dissuasion_Tone();
									return;
								}
							}
							CheckMenu(Button_Index);
							HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
							JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
						}
					break;
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
					case 15:
					case 16:
					case 17:
					case 18:
					case 19:
						KeySelect = KeyValue - 10;
						HomeHA_RoomButton_TypePress_Filter(RoomSelect,Button_Index,KeySelect);
						break;
					case 20:
					case 21:
					case 22:
					case 23:
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
						KeySelect = KeyValue - 20;
						HomeHA_RoomButton_Press_Filter(RoomSelect,Button_Index,KeySelect);
						break;
					
					case 30:
						//Dimmer
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(Filter_ID != T_Dimmer)
							{
								Filter_ID = T_Dimmer;  //Dimmer And RGB
								temp2 = HomeHA_RoomButtonWithFilter_Check(RoomSelect,Filter_ID);
								if(temp2)
								{
									Object_Button_Same_Cnt = temp2;
									Button_Index = 0;
									CheckMenu(Button_Index);
									HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
									//HA_Button_Name_Filter();
									JDFunctionCallBack=HA_Button_Name_Filter;
									//HomeHA_RoomTitle_Init(RoomSelect);
									//JDFunctionCallBack2 = &HA_Button_Init_CallBack_Filter;
								}
								else
									Dissuasion_Tone();
							}
						}
						break;
					case 31:						
						//shutte
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(Filter_ID != T_Shutter)
							{
								Filter_ID = T_Shutter; 
								
								temp2 = HomeHA_RoomButtonWithFilter_Check(RoomSelect,Filter_ID);
								if(temp2)
								{
									Object_Button_Same_Cnt = temp2;
									Button_Index = 0;
									CheckMenu(Button_Index);
									HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
									//HA_Button_Name_Filter();
									JDFunctionCallBack=HA_Button_Name_Filter;
									//HomeHA_RoomTitle_Init(RoomSelect);
									//JDFunctionCallBack2 = &HA_Button_Init_CallBack_Filter;
								}
								else
									Dissuasion_Tone();
							}
						}
						break;	
					case 32:
						//AC
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(Filter_ID != T_AC)
							{
								Filter_ID = T_AC;  //AV and HVAC
								
								temp2 = HomeHA_RoomButtonWithFilter_Check(RoomSelect,Filter_ID);
								if(temp2)
								{
									Object_Button_Same_Cnt = temp2;
									Button_Index = 0;
									CheckMenu(Button_Index);
									HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
									//HA_Button_Name_Filter();
									JDFunctionCallBack=HA_Button_Name_Filter;
									//HomeHA_RoomTitle_Init(RoomSelect);
									//JDFunctionCallBack2 = &HA_Button_Init_CallBack_Filter;
								}
								else
									Dissuasion_Tone();
							}
						}
						break;
					
					case 33:
						//relay
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(Filter_ID != T_Relay)
							{
								Filter_ID = T_Relay; 
								
								temp2 = HomeHA_RoomButtonWithFilter_Check(RoomSelect,Filter_ID);
								if(temp2)
								{
									Object_Button_Same_Cnt = temp2;
									Button_Index = 0;
									CheckMenu(Button_Index);
									HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
									//HA_Button_Name_Filter();
									JDFunctionCallBack=HA_Button_Name_Filter;
									//HomeHA_RoomTitle_Init(RoomSelect);
									//JDFunctionCallBack2 = &HA_Button_Init_CallBack_Filter;
								}
								else
									Dissuasion_Tone();
							}
						}
						break;
					case 34:
						//all device
						if(JD_Save_Frame_Index == JD_TX_Frame_Index && If_JD_Call_Empty())
						{
							if(Filter_ID != T_All_Device)
							{
								Filter_ID = T_All_Device; 
								
								temp2 = HomeHA_RoomButtonWithFilter_Check(RoomSelect,Filter_ID);
								if(temp2)
								{
									Object_Button_Same_Cnt = temp2;
									Button_Index = 0;
									CheckMenu(Button_Index);
									HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
									//HA_Button_Name_Filter();
									JDFunctionCallBack=HA_Button_Name_Filter;
									//HomeHA_RoomTitle_Init(RoomSelect);
									//JDFunctionCallBack2 = &HA_Button_Init_CallBack_Filter;
								}
								else
									Dissuasion_Tone();
							}
						}
						break;	
						
				}
			break;
			case Dimmer_Select:
				switch(KeyValue)
				{
					case K_Exit:					
						//StairLight();		
						
						PageStatus = HomeAutomationDevice_List1;
						HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
						JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
						//LoadJD_Menu(HomeAutomationDevice_List1);
						//HomeHA_RoomButton_Init(RoomSelect,Button_Index);
						//HA_Button_Name();
					break;
					default:
						HomeHA_RoomDimmerButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,KeyValue - 10);
					break;
				}
			break;
			case RGB_Select:
				switch(KeyValue)
				{
					case K_Exit:					
						
						PageStatus = HomeAutomationDevice_List1;
						//LoadJD_Menu(HomeAutomationDevice_List1);
						
						//HA_Button_Name();
						HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
						JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
					break;
					default:
						HomeHA_RoomRGBButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,KeyValue - 10);
					break;
				}
			break;
			case Shutter_Select:
			case Shutter_R_L_Select:	
				switch(KeyValue)
				{
					case K_Exit:					
						//StairLight();			
						PageStatus = HomeAutomationDevice_List1;
						//LoadJD_Menu(HomeAutomationDevice_List1);
						
						//HA_Button_Name();
						HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
						JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
					break;
					
					case 30:
					case 31:
					case 32:
					case 33:
					case 34:
					case 35:
					case 36:
					case 37:
					case 38:
						HomeHA_ShutterPositionButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,(KeyValue - 30) * 10);
					break;
					case 39:
						HomeHA_ShutterPositionButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,100);
					break;
					case 40:
					case 41:
					case 42:
					case 43:
					case 44:
					case 45:
					case 46:
					case 47:
					case 48:
						HomeHA_ShutterPositionButton_Press_Right(RoomSelect,Filter_Button_Select,Filter_KeySelect,(KeyValue - 40) * 10);
					break;
					case 49:
						HomeHA_ShutterPositionButton_Press_Right(RoomSelect,Filter_Button_Select,Filter_KeySelect,100);
					case K_UP:
						
						//break;
					case K_Down:
						
						//break;	
					case K_Stop:
						HomeHA_ShutterButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,KeyValue);
						break;
					default:
						
					break;
				}
				break;
			case Shutter_Blind_Select:
				switch(KeyValue)
				{
					case K_Exit:					
						//StairLight();			
						PageStatus = HomeAutomationDevice_List1;
						//LoadJD_Menu(HomeAutomationDevice_List1);
						
						//HA_Button_Name();
						HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
						JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
					break;
					case 14:
						HomeHA_ShutterAdjustButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,15);
						break;
					case 15:
						HomeHA_ShutterAdjustButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,25);
						break;
						
					case 16:
						HomeHA_ShutterAdjustButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,50);
						break;
					case 17:
						HomeHA_ShutterAdjustButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,75);
						break;
					case 18: // adjust
						HomeHA_ShutterAdjustButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,85);
						break;
					case 30:
					case 31:
					case 32:
					case 33:
					case 34:
					case 35:
					case 36:
					case 37:
					case 38:
						HomeHA_ShutterPositionButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,(KeyValue - 30) * 10);
					break;
					case 39:
						HomeHA_ShutterPositionButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,100);
					break;
					case K_UP:
						
						//break;
					case K_Down:
						
						//break;	
					case K_Stop:
						HomeHA_ShutterButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,KeyValue);
						break;
					default:
						
					break;
				}
				break;
			case AC_Select:
				switch(KeyValue)
				{
					case K_Exit:					
								
						PageStatus = HomeAutomationDevice_List1;						
						HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
						JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
					break;
					case 26:
						//temperature inc
						//video star says only cool and heat can transmit the temperature group address link
						if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect))// && (AC_Status[RoomSelect].bits.C_H == 1 || AC_Status[RoomSelect].bits.C_H == 2))
						{
							if(A_CTemperature[RoomSelect] < 31)
							{
								A_CTemperature[RoomSelect]++;
								Init_AC_Temperature(A_CTemperature[RoomSelect]);
								//update the temperature information to bus
								JDFunctionCallBack = &UpdataTemperatureTOBus;
							}
							
							if(AC_Status[RoomSelect].bits.C_H == 2)
							{
								//heat
								HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Heat_16_17 + ((A_CTemperature[RoomSelect] -16)/2) ,((A_CTemperature[RoomSelect] -15)%2));
							}
							//if(AC_Status[RoomSelect].bits.C_H == 1)
							//{
							else//cool							
								HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Cool_16_17 + ((A_CTemperature[RoomSelect] -16)/2) ,((A_CTemperature[RoomSelect] -15)%2));
							//}	
						}
						else
							Dissuasion_Tone();
					break;
					case 27:
						//temperature DEC
						if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect))// && (AC_Status[RoomSelect].bits.C_H == 1 || AC_Status[RoomSelect].bits.C_H == 2))
						{
							if(A_CTemperature[RoomSelect] > 16)
							{
								A_CTemperature[RoomSelect]--;
								
							}
							else
								A_CTemperature[RoomSelect] = 16;
							JDFunctionCallBack = &UpdataTemperatureTOBus;
							Init_AC_Temperature(A_CTemperature[RoomSelect]);
							if(AC_Status[RoomSelect].bits.C_H == 1)
							{
								//cool
								HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Cool_16_17 + ((A_CTemperature[RoomSelect] -16)/2) ,(A_CTemperature[RoomSelect] -15)%2);
							}
							else if(AC_Status[RoomSelect].bits.C_H == 2)
							{
								//heat
								HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Heat_16_17 + ((A_CTemperature[RoomSelect] -16)/2) ,(A_CTemperature[RoomSelect] -15)%2);
							}
						}
						else
							Dissuasion_Tone();
					break;
					case 28:// power on or off
						if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect) )
						{
							LightStatus[RoomSelect*2+Filter_Button_Select] &= ~(0x01<<Filter_KeySelect);
							HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Power_ON_Off,OFF);
							//set icon to off
							SetSwitchIcon(ChangeButton_Icon,AC_Select,28,0,1,0x0d);
						}
						else
						{
							LightStatus[RoomSelect*2+Filter_Button_Select] |= (0x01<<Filter_KeySelect);
							HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Power_ON_Off,ON);
							//set icon to on
							SetSwitchIcon(ChangeButton_Icon,AC_Select,28,0,1,0x0e);
							if(A_CTemperature[RoomSelect] < 16)
							{
								A_CTemperature[RoomSelect] = 16;
								Init_AC_Temperature(A_CTemperature[RoomSelect]);
							}
						}
							
					break;
					case 29:	
						AC_Status[RoomSelect].bits.C_H = 0;
						Init_AC_CH(PageStatus);
						//send KNX command						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Auto_Dry,ON);
					break;
					case 30:	
						AC_Status[RoomSelect].bits.C_H = 1;
						Init_AC_CH(PageStatus);
						//send KNX command						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Heat_Cool,OFF);
					break;
					case 31:	
						AC_Status[RoomSelect].bits.C_H = 2;
						Init_AC_CH(PageStatus);
						//send KNX command						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Heat_Cool,ON);
					break;
					case 32:
						AC_Status[RoomSelect].bits.C_H =3;
						Init_AC_CH(PageStatus);
						//send KNX command						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Auto_Dry,OFF);
						
					break;
					case 33://comfort mode						
					case 34:						
					case 35:
						AC_Status[RoomSelect].bits.mode = KeyValue - 33;
						Init_AC_mode(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,KeyValue -33 + Comfort_Mode,ON);
					break;
					case 36:
						AC_Status[RoomSelect].bits.speed = 0;
						Init_AC_Speed(PageStatus);
						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Low_auto_Speed,OFF);
					break;
					case 37:
						AC_Status[RoomSelect].bits.speed = 1;
						Init_AC_Speed(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Low_auto_Speed,ON);
					break;
					case 38:
						AC_Status[RoomSelect].bits.speed = 2;
						Init_AC_Speed(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,High_Midle_Speed,OFF);
					break;
					case 39:
						AC_Status[RoomSelect].bits.speed = 3;
						Init_AC_Speed(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,High_Midle_Speed,ON);
					break;
					case 40:
						AC_Status[RoomSelect].bits.speed = 0;
						Init_AC_Swing(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Auto_Stop_Swing,ON);
					break;
					case 41:
						AC_Status[RoomSelect].bits.speed = 1;
						Init_AC_Swing(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,LR_UD_Swing,OFF);
					break;
					case 42:
						AC_Status[RoomSelect].bits.speed = 2;
						Init_AC_Swing(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,LR_UD_Swing,ON);
					break;
					case 43:
						AC_Status[RoomSelect].bits.speed = 3;
						Init_AC_Swing(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,Auto_Stop_Swing,OFF);
						
					break;	
				}
			break;
			case HVAC_Select:
				if(!(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect))) 
				{ //the power is off and the key is not for power on
					if(KeyValue != 28)
					{
						if(KeyValue != K_Exit)
						{
							Dissuasion_Tone();
							return;
						}
					}
				}
				
				switch(KeyValue)
				{
					case K_Exit:					
								
						PageStatus = HomeAutomationDevice_List1;						
						HomeHA_RoomButtonWithFilter_Init(RoomSelect,Button_Index,Filter_ID);
						JDFunctionCallBack = &HA_Button_Init_CallBack_Filter;
					break;
					case 26:
						//temperature inc
						//video star says only cool and heat can transmit the temperature group address link
						if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect))// && (AC_Status[RoomSelect].bits.C_H == 1 || AC_Status[RoomSelect].bits.C_H == 2))
						{
							if(A_CTemperature[RoomSelect] < 40)
							{
								A_CTemperature[RoomSelect]++;
								Init_AC_Temperature(A_CTemperature[RoomSelect]);
								//update the temperature information to bus
								JDFunctionCallBack = &UpdataTemperatureTOBus;
								//check if need open heat wind channel
								//if heat mode we need open the heat channel
								if(AC_Status[RoomSelect].bits.C_H == 2)
								{
									if(A_CTemperature[RoomSelect] -  AC_Temperature_From_Sensor[RoomSelect] == 1)
									{
										
										JDFunctionCallBack1 = &OpenHeatChannel;
									}	
								}
								else if(A_CTemperature[RoomSelect] -  AC_Temperature_From_Sensor[RoomSelect] == 1)
								{
									//
									JDFunctionCallBack1 = &CloseCoolChannel;
								}
								//check if need save in Timer Event
								if(ExitMenu != NULL)
								{
									//save event 		
									//Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
									Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] =  0xC1; //set as AC bit
									Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = (RoomSelect<<12) +(Filter_Button_Select<<11) + (Filter_KeySelect<<8) + HVAC_Temperatire_Setting_Update;
									Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = A_CTemperature[RoomSelect];				
								}
							}
							else
								Dissuasion_Tone();
						}
						else
							Dissuasion_Tone();
					break;
					case 27:
						//temperature DEC
						if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect))// && (AC_Status[RoomSelect].bits.C_H == 1 || AC_Status[RoomSelect].bits.C_H == 2))
						{
							if(A_CTemperature[RoomSelect] > 5)
							{
								A_CTemperature[RoomSelect]--;								
							}
							else
								A_CTemperature[RoomSelect] = 5;
							JDFunctionCallBack = &UpdataTemperatureTOBus;
							Init_AC_Temperature(A_CTemperature[RoomSelect]);	

							if(AC_Status[RoomSelect].bits.C_H == 2)
							{
								if(AC_Temperature_From_Sensor[RoomSelect] - A_CTemperature[RoomSelect] == 1)
								{
									
									JDFunctionCallBack1 = &CloseHeatChannel;
								}	
							}
							else if(AC_Temperature_From_Sensor[RoomSelect] - A_CTemperature[RoomSelect] == 1)
							{
								//
								JDFunctionCallBack1 = &OpenCoolChannel;
							}
							if(ExitMenu != NULL)
							{
								//save event 		
								//Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] = 0;
								Timer_Event_temp.Protocol_Select[Timer_Event_Inf_Index] =  0xC1; //set as AC bit
								Timer_Event_temp.Inf_address[Timer_Event_Inf_Index] = (RoomSelect<<12) +(Filter_Button_Select<<11) + (Filter_KeySelect<<8) + HVAC_Temperatire_Setting_Update;
								Timer_Event_temp.Inf_ID[Timer_Event_Inf_Index] = A_CTemperature[RoomSelect];			
							}
						}
						else
							Dissuasion_Tone();
					break;
					
					case 28:// power on or off
					//key is Filter_KeySelect
				
						if(LightStatus[RoomSelect*2+Filter_Button_Select] & (0x01<<Filter_KeySelect) )
						{
							LightStatus[RoomSelect*2+Filter_Button_Select] &= ~(0x01<<Filter_KeySelect);
							//HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Power_ON_Off,OFF);
							//set icon to off
							SetSwitchIcon(ChangeButton_Icon,HVAC_Select,28,0,1,0x0d);
						}
						else
						{
							LightStatus[RoomSelect*2+Filter_Button_Select] |= (0x01<<Filter_KeySelect);
							//HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Power_ON_Off,ON);
							//set icon to on
							SetSwitchIcon(ChangeButton_Icon,HVAC_Select,28,0,1,0x0e);
							if(A_CTemperature[RoomSelect] < 5)
							{
								A_CTemperature[RoomSelect] = 5;
								Init_AC_Temperature(A_CTemperature[RoomSelect]);
							}
						}
							
					break;
					
					case 29:	
						AC_Status[RoomSelect].bits.C_H = 1;//for cool
						Init_HVAC_CH(PageStatus);
						//send KNX command						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Heat_Cool,OFF);
						//
						
					break;
					case 30:	
						AC_Status[RoomSelect].bits.C_H = 2;//for heat
						Init_HVAC_CH(PageStatus);
						//send KNX command						
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Heat_Cool,ON);
					break;


					case 31://standby mode
						AC_Status[RoomSelect].bits.mode = 3;
						Init_AC_mode(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Save_Mode,OFF);
						JDFunctionCallBack1 = &HVAC_ComfortModoCallBack;
						JDFunctionCallBack2 = &HVAC_NightModeCallBack;
					break;
					case 33://comfort mode	
						AC_Status[RoomSelect].bits.mode = 0;
						Init_AC_mode(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Comfort_Mode,ON);
						JDFunctionCallBack1 = &HVAC_Power_ON_OffCallBack;
						JDFunctionCallBack2 = &HVAC_NightModeCallBack;
					break;
					case 34: //night mode
						AC_Status[RoomSelect].bits.mode = 1;
						Init_AC_mode(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Night_Mode,ON);
						JDFunctionCallBack1 = &HVAC_Power_ON_OffCallBack;
						JDFunctionCallBack2 = &HVAC_ComfortModoCallBack;
					break;
					case 35: // save mode
						AC_Status[RoomSelect].bits.mode = 2;
						Init_AC_mode(PageStatus);
						HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_Save_Mode,ON);
						JDFunctionCallBack1 = &HVAC_ComfortModoCallBack;
						JDFunctionCallBack2 = &HVAC_NightModeCallBack;
					break;
					
					
					
					case 36://wind speed
					case 37:
					case 38:
						AC_Status[RoomSelect].bits.speed = KeyValue - 35;
						Init_HVAC_Speed(PageStatus);
						if(AC_Status[RoomSelect].bits.C_H == 1)
						{
							HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_CoolWind_Speed,(AC_Status[RoomSelect].bits.speed + 1)*50);
						}
						else
						{
							HomeHA_ACButton_Press(RoomSelect,Filter_Button_Select,Filter_KeySelect,HVAC_HeatWind_Speed,(AC_Status[RoomSelect].bits.speed + 1)*50);
						}
					break;

				}
			break;
			default:
			return;
		}
	}

}


//------------

void StairLight(void)
{
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
  			p_der_queue_tx_appl->n_byte                     = 3;
        p_der_queue_tx_appl->data[D_OPC] = D_SPECIAL_FUNCTION;
        p_der_queue_tx_appl->data[D_ADDRESS] = User + Internal; 
        p_der_queue_tx_appl->data[D_DATA_1] = 250;      // stairs light preferred (come il CEN)
  			Incr_index_queue_in_der(); 
}
//====================================================================


