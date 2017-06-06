/*
    $Id: Ges_der.h,v 1.1 2011/09/01 07:45:02 chnmhl Exp $
    $Log: Ges_der.h,v $
    Revision 1.1  2011/09/01 07:45:02  chnmhl
    First release

    Revision 1.1  2011/06/13 14:31:10  vrzrrt
    First commit. This module is only for translation purposes and will be never updated.

    Revision 1.2  2009/05/20 10:21:08  vrzrrt
    Commit of allinemento commenti in testa ai files

    Revision 1.1  2009/05/18 10:04:44  vrzrrt
    Primo committ. Versione debug Villata

*/

/*께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�
  께                                                                       께
  께                               Ges_der.h                              께
  께                                                                       께
  께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�*/

#define HIGH_PRIORITY   1
#define LOW_PRIORITY    0

void Derivation_Frame_Management(void);
void Frame_AS_Management(void);
extern void Free_Line(void);
//void FormatFrame_der(unsigned char frame_der);

#pragma DATA_SEG DAFAULT
#pragma DATA_SEG FAR_RAM
extern unsigned char tx_application_der_data_appl[20];
extern unsigned char rx_application_der_data_appl[25];
#pragma DATA_SEG DAFAULT

extern unsigned char der_appl_state;
