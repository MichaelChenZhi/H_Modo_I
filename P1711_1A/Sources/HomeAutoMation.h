extern uchar If_JD_Call_Empty(void);
extern void CheckRecount_Timer(void);
extern void TimeEventCaculate(uchar item);
extern void TimeEvent(void);
extern void TimeToReflashTimeEventCount(void);
extern void ScenarioOutput(uchar ch, uchar p);
extern void HA_Menu_Ini(void);
extern uchar CheckDate(void);
extern void KNX_TX_Managment(void);
extern void KNX_RC_Managment(void);
extern void KNX_Device_Control_Queue(unsigned int dest_addr, unsigned int value, uchar length);
extern void CheckMenu(uchar index);
extern void HA_Button_Init_CallBack_Filter(void);
extern void HomeHA_RoomButton_TypePress_Filter(uchar room ,uchar index,uchar key);
extern void HomeHA_RoomButton_Press_Filter(uchar room ,uchar index,uchar key);
extern void HomeHA_RoomDimmerButton_Press(uchar room ,uchar index,uchar key, uchar key1);
extern void HomeHA_RoomRGBButton_Press(uchar room ,uchar index,uchar key, uchar key1);
extern void HomeHA_ShutterPositionButton_Press(uchar room ,uchar index,uchar key, uchar Value);
extern void HomeHA_ShutterPositionButton_Press_Right(uchar room ,uchar index,uchar key, uchar Value);
extern void HomeHA_ShutterButton_Press(uchar room ,uchar index,uchar key, uchar key1);
extern void HomeHA_ShutterAdjustButton_Press(uchar room ,uchar index,uchar key, uchar Value);
extern void UpdataTemperatureTOBus(void);
extern void HomeHA_ACButton_Press(uchar room ,uchar index,uchar key, uchar key1, uchar Value);
extern void OpenHeatChannel(void);
extern void CloseHeatChannel(void);
extern void OpenCoolChannel(void);
extern void CloseCoolChannel(void);
extern void HVAC_Power_ON_OffCallBack(void);
extern void HVAC_ComfortModoCallBack(void);
extern void HVAC_NightModeCallBack(void);
extern void CCTVOutput(uchar ch);
extern void Tel_HookOFF_Do_Video(uchar state_H);
extern void Tel_HookOFF_Do(uchar state_H);
extern void DTMF_Key_Out_Put(void);
extern void FillListData(void);
extern void TimerEvent_Edit_Init(void);
extern void TimerEvent_Edit_Init_Back(void);