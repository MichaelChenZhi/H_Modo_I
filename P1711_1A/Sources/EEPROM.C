#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include <string.h>
#include	"define.h"
#include "Global.h"


//extern uchar RingVolume;
extern void HCS08_EE_Init(void);


extern uchar FlagPage1;
extern uchar FlagPage1Target;

//extern uchar FlagPage2;
//extern uchar FlagPage2Target;



extern void WriteData(unsigned int _DstPage,unsigned char *_buf,unsigned int _index,unsigned int _len) ;
extern void InserData(unsigned char *_buf,unsigned int _index,unsigned int _len);
extern void WritePage( unsigned int _DstPage);
extern void CopyPage(unsigned int _DstPage);
extern void ErasePage( unsigned int _DstPage);
extern void SaveInfQueue(void (*finProc)());
/*
uchar SearchFitAddr(void)
{
	uchar a;
	uchar b;
	for(a=1;a<MaxRecordNum;a++)
	{
		__RESET_WATCHDOG();
		for(b=0;b<SettingPara.RecordCnt;b++)
		{
			if(RecordAddr[b]==a)
			{
				break;
			}
		}
		if(b==SettingPara.RecordCnt)
		  {
				return a;
		  }
		
	}

	return MaxRecordNum;
}
*/
//-----------------------------------------------------------------



//--------------------------------------------------------




//-----------------------------------------------------------------
/*
function: update new phonebook information into flash when a new record added
*/
//-----------------------------------------------------------------
/*
void UpdatePhoneBook(void)
{
	
		
		(void)memcpy(&SaveData[RecordStartAddr],RecordAddr,MaxRecordNum);
		//num[0]=SettingPara.RecordCnt;
		//WriteData(EE_Page1,num,P_recordcnt+ParaStartAddr,1);
		//InserData(num,P_recordcnt+ParaStartAddr,1);
		SaveData[P_recordcnt] = SettingPara.RecordCnt;
		SaveData[TargerNumPosition] = TargetPageBook; //target page is pagebook
		SaveData[TargerFlagPosition] = 0x51;
		WritePage(EE_PageCopy2);//EE_PageBook
}
*/
//---------------------------------------------------------------------

//---------------------------------------------------------------------
void SavePhoneBookRecord(uchar phonerindex,uchar phonePageindex,Intercom_Record *p)
{			  
	  WriteData(EE_Page2+phonePageindex*512,&p->calltype,phonerindex*24,24);	  							
}


//----------------------------------------------------------------------
/*
void DeletePhoneBook(unsigned char index)
{
	uchar i;
	uchar num[1];
	//delete one record
	
	  

	//resave the RecordAddr and record lenth
	CopyPage(EE_PageBook);
	
	InserData(RecordAddr,RecordStartAddr,MaxRecordNum);
	num[0]=SettingPara.RecordCnt;
	InserData(num,P_recordcnt,1);
	SaveData[TargerNumPosition] = TargetPageBook; //target page is pagebook
	SaveData[TargerFlagPosition] = 0x51;
	WritePage(EE_PageCopy1);
}
*/
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------



/*
void SavePresencewithMessage(void)
{
	uchar data[1];
	
	
	
	data[0]=Flag3.all;
	
	
	  
	//if(Flag3.bits.absence)
	//	F_presence_absence = ABSENCE_ON;
	//else
	//	F_presence_absence = ABSENCE_OFF;

	if(Flag3.bits.message)
		F_messages_present = ON;
	else 
		F_messages_present = OFF;
	WriteData(EE_PageLED,data,ParaStartAddr,21);
}
*/
//-----------------------------------------------------------------------------------


/*
void SaveAutoAddress(void)
{
	
	uchar data[1];
	data[0]=SettingPara.AutoOnAddress;
	#ifdef Flash
	WriteData(EE_Page1,data,P_AutoAddress+ParaStartAddr,1);
	#else	
	
	#endif
}

void SaveOpenDoor1Address(void)
{
	
	uchar data[1];
	data[0]=SettingPara.OpenDoor1Address;
	#ifdef Flash
	WriteData(EE_Page1,data,P_OpenDoor1Address+ParaStartAddr,1);
	#else	
	
	#endif
}

void SaveOpenDoor2Address(void)
{
	
	uchar data[1];
	data[0]=SettingPara.OpenDoor2Address;
	#ifdef Flash
	WriteData(EE_Page1,data,P_OpenDoor2Address+ParaStartAddr,1);
	#else	
	
	#endif
}
*/
//---------------------------------------------------------------------------------
void SaveAudioPara(void)
{
	/*
	sec data[5];
	data[0] = SettingPara.RCallMelody;
	data[1] = SettingPara.RFloorCallMelody;
	data[2] = SettingPara.CallVolume;
	//data[3] = SettingPara.S_FlagBeepVolume;
	data[4] = SettingPara.AudioVolume;
	*/
	
	WriteData(EE_Page1,&SettingPara.RCallMelody,P_callmelody+ParaStartAddr,sizeof(SettingPara));
}
//---------------------------------------------------------------------------------
/*
void SaveVideoPara(void)
{
	uchar data[1];
	
	data[0]=SettingPara.Theme;
	
	WriteData(EE_Page1,data,P_theme+ParaStartAddr,1);
	
}
*/
//---------------------------------------------------------------------------------

void DefaultSetting(uchar p1,uchar p2)
{	  
	const Setting defaultsettingpara={
	  0x01,//call melody
	  0x02,//floor call melody
	  0x03,//call volume
	  //0x01,//beep on or off
	  0x03,//audio volume
	  0x01,//theme
	  0x08,//S_Flag only beep is on as default	  
	  0x00,//TOCC
	  0x00,//colume address
	  0x03//PSTN moledy
	 //
	  //0x08
	};
	HCS08_EE_Init();

	CopyPage(EE_Page1);
	SaveData[0]=p1;
	SaveData[1]=p2;		
	if(SaveData[0]==0x55 && SaveData[1]==0xaa)
	{
		//did not delete the phone book
		//WritePage(EE_Page1);
		//reset the value which is save by video star
		(void)memcpy(&SettingPara.RCallMelody,&defaultsettingpara.RCallMelody,6);
		SettingPara.PSTN_Call_Melody = 0x03;
		//SettingPara = defaultsettingpara;
		(void)memcpy(&SaveData[P_callmelody],&SettingPara.RCallMelody,sizeof(Setting));
		
	}
	else
	{
	  //delete the phone book
	  	//(void)memcpy(&SettingPara,defaultsettingpara,sizeof(SettingPara));
	  	SettingPara = defaultsettingpara;
		(void)memcpy(&SaveData[P_callmelody],&defaultsettingpara.RCallMelody,sizeof(Setting));
		SaveData[0]=0x55;
		SaveData[1]=0xaa;
	   
	}
	R_Volume_Call = SettingPara.CallVolume;
	R_Audio_Volume = F_Audio_Volume; 
	R_presence_absence = ABSENCE_OFF;
	R_Led_mute = F_Led_mute;
	if(F_Presence_Absence)
	{		  			
		R_presence_absence = ABSENCE_ON;
	}
	WritePage(EE_Page1);
			
}
//

/*
void SaveA_Vsetting(void)
{
	uchar data[1];
	data[0]=SettingPara.AudioVolume;	
	WriteData(EE_Page1,data,P_audiovolume+ParaStartAddr,1);
	
}
*/
void FlashCopyFuntion1(void)
{
	unsigned int targetaddress;
	if(FlagPage1Target > 0 && FlagPage1Target<TargetPageCopy1+1)
	{
		targetaddress = EE_Page1 + (FlagPage1Target - 1) * 512;
		CopyPage(EE_PageCopy1);
		WritePage(targetaddress);
		//ErasePage(EE_PageCopy1);	
	}
	/*
	else
	{
		ErasePage(EE_PageCopy1);
	}
	*/
	ErasePage(EE_PageCopy1);
}

/*
void FlashCopyFuntion2(void)
{
	//unsigned int targetaddress;
	if(FlagPage2Target > 0 && FlagPage2Target<TargetPageCopy2+1)
	{
		targetaddress = EE_Page1 + (FlagPage2Target - 1) * 512;
		CopyPage(EE_PageCopy2);
		WritePage(targetaddress);
		ErasePage(EE_PageCopy2);
			
	}
	else
	{
		ErasePage(EE_PageCopy2);
	}
	
}
*/
/*---------------------------------------------------
SCAN THE Flag which need do flash from copy page to target page
*/
void FlashEvent(void)
{
	if(FlagPage1 == 0x51 && SaveInf == NULL && PRG_FLASH_AFTER_TX == OFF)//we have flash event and the callback poiter is empty
	{
		
		SaveInfQueue(&FlashCopyFuntion1);
		
	}
	/*
	else if(FlagPage2 == 0x51 && SaveInf == NULL && PRG_FLASH_AFTER_TX == OFF)//we have flash event and the callback poiter is empty
	{
		
		SaveInfQueue(&FlashCopyFuntion2);
		
	}
	*/
	/*
	else if(SystemStatus.bits.UpdatePB && SaveInf == NULL && PRG_FLASH_AFTER_TX == OFF && FlagPage1 != 0x51 && FlagPage2 != 0x51)
	{ //update phonebook structure event request
		SystemStatus.bits.UpdatePB = 0;
		SaveInfQueue(&UpdatePhoneBook);
	}
	*/
}

void SaveNewPassword(void)
{
	uchar data[6];
	uchar i;
	for(i = 0; i< 6;i++)
	{
		data[i] = Tel_num[i];
	}
	//EE_Page10+SpaceByte
	WriteData(EE_Page_AlarmSystem,data,P_UserPassword,6);
}


void SaveAlarmEnableFlag(uchar A_F)
{
	uchar data[1];	
	data[0] = A_F;			
	WriteData(EE_Page_AlarmSystem,data,Alarm_Flag_Addr,1);
}

void SaveZoonBelongInf(uchar p1, uchar p2, uchar p3,uchar p4,uchar p5,uchar p6,uchar p7,uchar p8, uchar p9)
{
	//uchar data[2];
	
	Memory_S.MsgBuf[0] = p1;//away zone
	Memory_S.MsgBuf[1] = p2;//home zone
	Memory_S.MsgBuf[2] = AlarmP->AlarmZoneEnable;
	Memory_S.MsgBuf[3] = p3;
	Memory_S.MsgBuf[4] = p4;
	Memory_S.MsgBuf[5] = p5;
	Memory_S.MsgBuf[6] = p6;
	Memory_S.MsgBuf[7] = p7;
	Memory_S.MsgBuf[8] = p8;
	Memory_S.MsgBuf[9] = p9;
	WriteData(EE_Page_AlarmSystem,Memory_S.MsgBuf,Alarm_Zone_Addr,64);//3//10+6*9
	
}


void SaveTelephoneM_name_DTMF_key(void)
{
	WriteData(EE_Page_AlarmSystem,Memory_S.MsgBuf,SpaceByte+sizeof(Password_Alarmsystem) + sizeof(AlarmSystem) +3 + ZoneNum*sizeof(ZoneNameS),43);//3
}


void SavePassword(void)
{
	
	WriteData(EE_Page_AlarmSystem,Memory_S.MsgBuf,SpaceByte,12);//3	
}


void SaveTimerEvent(void)
{
	
	//WriteData(EE_Time_Event,&Timer_Event_temp.Hour,EE_Time_Event+Timer_Event_Index*sizeof(Timer_Event_temp),sizeof(Timer_Event_temp));//3
	WriteData(EE_Time_Event,&Timer_Event_temp.Hour,Timer_Event_Index*sizeof(Timer_Event_temp),sizeof(Timer_Event_temp));//3	
}


void SaveTelephoneNumber(void)
{
	uchar data[17];
	data[0] = Tel_index;
	(void)memcpy(&data[1],Tel_num,16);
	
	WriteData(EE_Page_Tel_Momory,data,Edit_Index*17,17);
	
}