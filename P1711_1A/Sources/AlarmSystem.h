extern uchar IsEn_Dis_Alarm(unsigned int T_address, uchar value);
extern uchar IsMydvice_ID(unsigned int T_address);
extern void ScanAlarmEvent(uchar channel);
extern void ShowNextAlarm(void);
extern void ReScanAlarmEvent(uchar channel);
extern void CancleAllDelayAlarm(void);
extern void ShowRecount(uchar num, uchar MenuID);
extern uchar CheckPasswordForInstaller(void);
extern uchar CheckPasswordForUser(void);
extern void Home_Alarm(void);
extern void Away_Alarm(void);
extern void SendDelayAlarm(uchar channel);
extern void AlarmFrame_Format(void);
extern void SwitchToAlarmMenu(void);
extern void Alarm_Logo(void);