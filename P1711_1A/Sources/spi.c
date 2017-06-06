/*
    $Id: spi.c,v 1.1 2011/09/01 07:45:02 chnmhl Exp $
    $Log: spi.c,v $
    Revision 1.1  2011/09/01 07:45:02  chnmhl
    First release

    Revision 1.1  2011/06/13 14:31:10  vrzrrt
    First commit. This module is only for translation purposes and will be never updated.

    Revision 1.2  2009/06/12 06:53:17  vrzrrt
    Nuova version of Paggio. Base for start sviluppo application.


*/
/**************************************************************************************
 *
 * spi.c
 *
 * Contains the source code for communicating
 * with the MAX7456 using a bit-banged SPI port
**************************************************************************************/

// INCLUDES ***********************************************


#include "MC9S08AW60.h"                /* I/O map for MC9S08AW60CFU */
#include "define.h"


void S_Delay(void)
{
    Nop;
    Nop;
    Nop;
    Nop;
}
/*

uchar VolumeTable(uchar p)
{
	
}
*/
void SetVolumeChange(uchar t,uchar p)
{
//set speaker volume ,t==0 decrease mode,t==1 increase mode
  uchar i;
	uchar volume_p;
//EN_AUDIO=1;//give power
//Nop;
//Nop;
  C_S=1;
  S_Delay();
  if(t==0) {                                      //decrease`

    U_D=0;                                        //set low to make decrease
    S_Delay();
    C_S=0;                                        //active

  }                                               //increase
  else {

    U_D=1;                                        //set high to make increase
     S_Delay();
     S_Delay();
    C_S=0;                                        //active
  }
  	switch(p)
  	{
  		case 0:
			volume_p = 14;//14; //keep same as level 1 due to it is not possible reach to 0 level
		break;
		case 1:
			volume_p = 16;//15; //old value is 17
		break;
		case 2:
			volume_p = 17;//16;
		break;
		case 3:
			volume_p = 19;//18;
		break;
		case 4:
			volume_p = 22;
		break;
		case 5:
			volume_p = 25;
		break;
  	}
  
  //for(i=0;i<p*3+17;i++) {
  	for(i=0;i<volume_p;i++) {
	    U_D=~U_D;
	    S_Delay();
	    U_D=~U_D;
	    S_Delay();
  	}
  C_S=1;
  U_D=0;
  C_S=0;

//

}


void ClearVolume(void)
{
//make the volume to zero
  uchar i;

  C_S=1;
   S_Delay();
  U_D=0;                                          //set low to make decrease
   S_Delay();
  C_S=0;                                          //active
  for(i=0;i<32;i++) {
    U_D=1;
   S_Delay();
    U_D=0;
     S_Delay();
  }
//C_S=1;

}

/*----------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------*/
uchar Read_HC165(void) 
{   
	
	uchar y = 0; 

#ifdef  PCB20150127
	//y=PTAD;
	//y=(y>>1);
	if(!PTAD_PTAD1)
		y |= 0x01<<5;
	if(!PTAD_PTAD2)
		y |= 0x01<<4;
	if(!PTAD_PTAD3)
		y |= 0x01<<3;
	if(!PTAD_PTAD4)
		y |= 0x01<<2;
	if(!PTAD_PTAD5)
		y |= 0x01<<1;
	if(!PTAD_PTAD6)
		y |= 0x01<<0;
	
#else	
	uchar i;    //
	//DisableInterrupts; 
	PL_165 = 0;   //loading data
	Nop;
	Nop;
	Nop;
	Nop;
	PL_165 = 1;   //
	for(i = 0;i<8;i++)   //read 8 times
	{    
		Nop;
		Nop;
		if(!DATA_165)  //
		{       
			y |= 0x01<<i;     
		}
		Nop;
		Nop;
		CLK_165 = 0; //next bit
		Nop;
		Nop;
		CLK_165 = 1; 
	}

#endif
	//DisableInterrupts; 
	//Hc165In_pin.all=temp;
	
	return y;
}


/*==========================================================================*/
void HW_Protocol_Init(void)
/*==========================================================================*/
{
  DisableInterrupts;
/* ### Init_SCI init code */
/* SCI1C2: TIE=0,TCIE=0,RIE=0,ILIE=0,TE=0,RE=0,RWU=0,SBK=0 */
  SCI2C2 = 0x00;                                  /* Disable the SCI1 module */
  (void)(SCI2S1 = 0);                             /* Dummy read of the SCI1S1 registr to clear flags */
  (void)(SCI2D = 0);                              /* Dummy read of the SCI1D registr to clear flags */
  if(Pres_BT)
  {
  	#ifdef High_Speed
  	SCI2BD = (BUSCLK / 16 / BAUDRATE1)-1;
	#else
	SCI2BD = (BUSCLK / 16 / BAUDRATE1);
	#endif
	Con485_R = 0;
  }
  else
  {
  	SCI2BD = BUSCLK / 16 / BAUDRATE;
	Con485_R = 1;
  }
/* SCI1C1: LOOPS=0,SCISWAI=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
  SCI2C1 = 0x00;
/* SCI1C2: TIE=0,TCIE=0,RIE=1,ILIE=0,TE=1,RE=1,RWU=0,SBK=0 */
  SCI2C2 = 0x2C;                                  // abilito TX/RX; interrupt su RX
/* SCI1C3: R8=0,T8=0,TXDIR=1,TXINV=0,ORIE=1,NEIE=1,FEIE=1,PEIE=1 */
  SCI2C3 = 0x20;                                  // pin TX invertito; abilito interrupt anche su errori rx
/* SCI1S2: RXINV=0,BRK13=0,RAF=0 */
  SCI2S2 = 0x00;                                  // pin RX non invertito

  SCI1C2 = 0x00;                                  /* Disable the SCI2 module */
  (void)(SCI1S1 = 0);                             /* Dummy read of the SCI1S1 registr to clear flags */
  (void)(SCI1D = 0);                              /* Dummy read of the SCI1D registr to clear flags */

#ifdef High_Speed
  SCI1BD = BUSCLK / 16 / BAUDRATE-1;
#else
  SCI1BD = BUSCLK / 16 / BAUDRATE;
#endif

/* SCI1C1: LOOPS=0,SCISWAI=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
  SCI1C1 = 0x00;
/* SCI1C2: TIE=0,TCIE=0,RIE=1,ILIE=0,TE=1,RE=1,RWU=0,SBK=0 */
  SCI1C2 = 0x2C;                                  // abilito TX/RX; interrupt su RX
/* SCI1C3: R8=0,T8=0,TXDIR=1,TXINV=0,ORIE=1,NEIE=1,FEIE=1,PEIE=1 */
  SCI1C3 = 0x20;                                  // pin TX invertito; abilito interrupt anche su errori rx
/* SCI1S2: RXINV=0,BRK13=0,RAF=0 */
  SCI1S2 = 0x00;  

  EnableInterrupts;
}



