/*
    $Id: FlashWrite.h,v 1.2 2012/04/24 03:52:19 chnmhl Exp $
    $Log: FlashWrite.h,v $
    Revision 1.2  2012/04/24 03:52:19  chnmhl
    fixed 3509,3500,3501

    Revision 1.1  2011/09/01 07:45:02  chnmhl
    First release

    Revision 1.3  2011/02/26 05:08:54  chnmhl
    add management on deep switch IO pullup and set as output for get lower power consumption

    Revision 1.4  2010/09/03 06:06:11  chnmhl
    Ipervoice folio Menu management update

    Revision 1.8  2009/11/04 16:21:30  vrzrrt
    WIP translation in progress

    Revision 1.7  2009/10/30 16:28:53  vrzrrt
    WIP translations

    Revision 1.6  2009/03/25 14:26:57  vrzrrt
    Aggiornata gestione gemelli

    Revision 1.5  2009/02/05 16:00:04  vrzrrt
    Aggiunte correzioni dopo summit con Fauro e Pont

    Revision 1.4  2008/12/17 17:20:04  vrzrrt
    Aggiornata gestione tasti

    Revision 1.3  2008/11/10 15:59:48  vrzrrt
    WIP

    Revision 1.2  2008/11/04 15:50:49  vrzrrt
    WIP - iniziate gestioni su tx completed

    Revision 1.1  2008/10/28 08:55:28  vrzrrt
    Primo commit - Versione Fauro

    Revision 0.0  2008/09/30 15:11:34  fravtr
    first release


*/


/*께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�
  께                                                                       께
  께                             WriteFlash                                께
  께                                                                       께
  께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�*/

extern void HCS08_EE_Init(void);
extern void  Read_IniData(unsigned char *buf);
extern void CopyPage(unsigned int _DstPage);
extern void InserData(unsigned char *_buf,unsigned int _index,unsigned int _len);
extern void WritePage( unsigned int _DstPage);
extern void WriteData(unsigned int _DstPage,unsigned char *_buf,unsigned int _index,unsigned int _len) ;
//extern void ReadRecordData(unsigned int _DstPage,unsigned char *_buf,unsigned int _index,unsigned char _len);