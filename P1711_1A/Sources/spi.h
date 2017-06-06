/*
    $Id: spi.h,v 1.1 2011/09/01 07:45:02 chnmhl Exp $
    $Log: spi.h,v $
    Revision 1.1  2011/09/01 07:45:02  chnmhl
    First release

    Revision 1.1  2011/06/13 14:31:10  vrzrrt
    First commit. This module is only for translation purposes and will be never updated.

    Revision 1.2  2009/06/12 06:53:17  vrzrrt
    Nuova version of Paggio. Base for start sviluppo application.


*/
/*께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�
  께                                                                       께
  께                              SPI.H                                    께
  께                                                                       께
  께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�*/

// Prototipi
//void spiWriteReg(const unsigned char regAddr, const unsigned char regData);
//void spiWriteRegAutoIncr(const unsigned char regData);
extern void ClearVolume(void);
extern void SetVolumeChange(uchar t,uchar p);
extern void HW_Protocol_Init(void);
extern uchar Read_HC165(void) ;