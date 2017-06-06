
/*
** ###################################################################
**     This code is generated by the Device Initialization Tool.
**     It is overwritten during code generation.
**     USER MODIFICATION ARE PRESERVED ONLY INSIDE INTERRUPT SERVICE ROUTINES
** ###################################################################
*/

#include "MC9S08AW32.h"                /* I/O map for MC9S08AW16CFU */
#include "define.h"
#include "global.h"
#include "driv_der.h"
unsigned long int pluse_width_val(unsigned int begin_counter,unsigned int end_counter,unsigned char begin_over_times,unsigned char end_over_times);
//#define NEW_HW
// volatile KBISCSTR _KBISC @0x0000001E;
// volatile KBIPESTR _KBIPE @0x0000001F;

/*
** ===================================================================
**     Method      :  MCU_init (bean MC9S08AW60_64)
**
**     Description :
**         Device initialization code for selected peripherals.
** ===================================================================
*/
void MCU_init(void)
{
	/* ### MC9S08AW16_64 "Cpu" init code ... */
	/*  PE initialization code after reset */
	/* Common initialization of the write once registers */
	/* SOPT: COPE=0,COPT=1,STOPE=0 */
	#ifdef High_Speed
		SOPT = 0x53;                                      
		/* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
		SPMSC1 = 0x1C;                                      
		/* SPMSC2: LVWF=0,LVWACK=0,LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 */
		SPMSC2 = 0x00;                                      
		/* SMCLK: MPE=0,MCSEL=0 */
		SMCLK &= (unsigned char)~0x17;                     
		/*  System clock initialization */
		/* ICGC1: HGO=0,RANGE=1,REFS=0,CLKS1=0,CLKS0=1,OSCSTEN=1,LOCD=0 */
		ICGC1 = 0x4C;                                      
		/* ICGC2: LOLRE=0,MFD2=1,MFD1=1,MFD0=0,LOCRE=0,RFD2=0,RFD1=0,RFD0=0 */
		ICGC2 = 0x60;                                      
		if (*(unsigned char*far)0xFFBE != 0xFF) { /* Test if the device trim value is stored on the specified address */
			ICGTRM = *(unsigned char*far)0xFFBE; /* Initialize ICGTRM register from a non volatile memory */
		}
		while(!ICGS1_LOCK) {                 /* Wait */
		}
	#else
		
		 SOPT = 0xD3;             // Abilito WD
		  /* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
		  SPMSC1 = 0x1C;
		  /* SPMSC2: LVWF=0,LVWACK=0,LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 */
		  SPMSC2 = 0x00;
		  /* SMCLK: MPE=0,MCSEL=0 */
		  SMCLK &= (unsigned char)~0x17;

		   ICGC1 = 0x0C;
	      /* ICGFLT: FILT11=0,FILT10=0,FILT9=0,FILT8=0,FILT7=1,FILT6=1,FILT5=0,FILT4=0,FILT3=0,FILT2=0,FILT1=0,FILT0=0 */
	      ICGFLT = 0xC0;                       /* Initialization of the filter value */
	      /* ICGC2: LOLRE=0,MFD2=0,MFD1=0,MFD0=0,LOCRE=0,RFD2=0,RFD1=1,RFD0=0 */
	      ICGC2 = 0x00;             // N = 4; R = 1  --> Fbus = 4.44 MHz
	      ICGTRM = *(unsigned char*far)0xFFBE; /* Initialize ICGTRM register from a non volatile memory */
 	#endif

#ifdef  PCB20150127
	/* Common initialization of the CPU registers */
	// 0=input; 1=output
	// PORT A
	PTASE = 0x00;     // 1= slew rate control
	PTADS = 0x00;     // 1= high drive
	PTAPE = 0xFE;     // 1= pullup  1111 1110
	PTAD  = 0x00;
	PTADD = 0x01;     // 1= output
	// PORT B
	PTBSE = 0x00;     // 1= slew rate control
	PTBDS = 0x00;     // 1= high drive
	PTBPE = 0x38;     // 1= pullup     
	PTBD  = 0x00;

	PTBDD = 0x47;     // 1= output  (CS_OSD = input) 

	// PORT C
	PTCSE = 0x00;     // 1= slew rate control
	PTCDS = 0x00;     // 1= high drive
	PTCPE = 0x30;     // 1= pullup  0011 0001
	PTCD  = 0x00;
	PTCDD = 0xCE;     // 1= output  1100 1110
	// PORT D
	PTDSE = 0x00;     // 1= slew rate control
	PTDDS = 0x00;     // 1= high drive
	PTDPE = 0x09;     // 1= pullup 00001001
	PTDD  = 0x82;     //           00000010
	PTDDD = 0xf6;     // 1= output  1111 0110
	// PORT E
	PTESE = 0x00;     // 1= slew rate control
	PTEDS = 0x00;     // 1= high drive
	PTEPE = 0xC4;     // 1= pullup   1110 0100
	PTED  = 0x18; 
	PTEDD = 0xD5;     // 1= output   1101 0101

	// PORT F
	PTFSE = 0x01;     // 1= slew rate control
	PTFDS = 0x00;     // 1= high drive
	PTFPE = 0x04;     // 1= pullup
	PTFD  = 0x20;
	PTFDD = 0xFB;     // 1= output
	
	// PORT G
	PTGSE = 0x00;     // 1= slew rate control
	PTGDS = 0x00;     // 1= high drive
	PTGPE = 0x28;     // 1= pullup    0010 1000
	//  PTGD  = 0x40;
	PTGD  = 0x40;
	PTGDD = 0xD7;     // 1= output   OLD 0X67 CHANGE FOR TC LINE   1101 0111

#else  
	/* Common initialization of the CPU registers */
	// 0=input; 1=output
	// PORT A
	PTASE = 0x00;     // 1= slew rate control
	PTADS = 0x00;     // 1= high drive
	PTAPE = 0x00;     // 1= pullup
	PTAD  = 0x00;
	PTADD = 0x01;     // 1= output
	// PORT B
	PTBSE = 0x00;     // 1= slew rate control
	PTBDS = 0x00;     // 1= high drive
	PTBPE = 0x3c;     // 1= pullup     
	PTBD  = 0x00;

	PTBDD = 0x47;     // 1= output  (CS_OSD = input) 

	// PORT C
	PTCSE = 0x00;     // 1= slew rate control
	PTCDS = 0x00;     // 1= high drive
	PTCPE = 0x30;     // 1= pullup
	PTCD  = 0x00;
	PTCDD = 0xCF;     // 1= output 
	// PORT D
	PTDSE = 0x00;     // 1= slew rate control
	PTDDS = 0x00;     // 1= high drive
	PTDPE = 0x19;     // 1= pullup
	PTDD  = 0x02;
	PTDDD = 0x66;     // 1= output
	// PORT E
	PTESE = 0x00;     // 1= slew rate control
	PTEDS = 0x00;     // 1= high drive
	PTEPE = 0x00;     // 1= pullup
	PTED  = 0x18;     //                           0001 1000
	PTEDD = 0xF5;     // 1= output  (SPI = input)  1111 0101

	// PORT F
	PTFSE = 0x00;     // 1= slew rate control
	PTFDS = 0x00;     // 1= high drive
	PTFPE = 0x00;     // 1= pullup
	PTFD  = 0x20;
	PTFDD = 0xFB;     // 1= output
	
	// PORT G
	PTGSE = 0x00;     // 1= slew rate control
	PTGDS = 0x00;     // 1= high drive
	PTGPE = 0x28;     // 1= pullup
	//  PTGD  = 0x40;
	PTGD  = 0x40;
	PTGDD = 0xD7;     // 1= output   OLD 0X67 CHANGE FOR TC LINE

#endif
	
  

  
//   /* SCI1C1: LOOPS=0,SCISWAI=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
//   SCI1C1 = 0x00;
//   /* SCI1C2: TIE=0,TCIE=0,RIE=1,ILIE=0,TE=1,RE=1,RWU=0,SBK=0 */
// //ccccc  SCI1C2 = 0x2C;
//   SCI1C2 = 0x8;
//   /* SCI1C3: R8=0,T8=0,TXDIR=1,TXINV=0,ORIE=1,NEIE=1,FEIE=1,PEIE=1 */
//   SCI1C3 = 0x2F;
//   /* SCI1S2: BRK13=0,RAF=0 */
//   SCI1S2 = 0x00;

  /* ### Init_COP init code */
  SRS = 0xFF;                          /* Clear WatchDog counter */
  /* ### Init_IRQ init code */
  /* ** Initialize pin IRQ ** */
   IRQSC &= (unsigned char)~0x02;       /* Disable IRQ Interrupt */
  /* IRQSC: IRQPE=1 */
  IRQSC |= (unsigned char)0x10;                               
  /* IRQSC: IRQACK=1 */
  IRQSC |= (unsigned char)0x04;                               
  /* IRQSC: IRQIE=1 */
  //IRQSC |= (unsigned char)0x02;        /* Enable IRQ Interrupt */
  /* ### Init_TPM init code */
  /* TPM1SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
  TPM1SC = 0x00;                       /* Stop and reset counter */
  TPM1MOD = 0x00;                      /* Period value setting */
  TPM1C0V = COUNTER_2MS;                    /* Compare 0 value setting */
  (void)(TPM1C0SC = 0);               /* Channel 0 int. flag clearing (first part) */
  /* TPM1C0SC: CH0F=0,CH0IE=1,MS0B=0,MS0A=1,ELS0B=0,ELS0A=0 */
  TPM1C0SC = 0x50;                     /* Int. flag clearing (2nd part) and channel 0 contr. register setting */
  (void)(TPM1SC = 0);                 /* Overflow int. flag clearing (first part) */

	TPM1C4V = 0;
	TPM1C4SC = 0x44;//TPM1C1SC_CH1IE=1;TPM1C1SC_ELS1A=1;//TPM1C1SC = 0x50;


  /* TPM1SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=1,PS2=0,PS1=0,PS0=0 */
  TPM1SC = 0x08;                       /* Int. flag clearing (2nd part) and timer control register setting */
	//TPM1SC = 0x48;
  /* ### Init_GPIO init code */

  TPM2SC = TPM2SC_CLKSA_MASK;       // BUSCLK clock source
/*
  #ifndef PRG_7456
  // init SPI interface
  // MAX7556: mode 00 (SCK low idle); 10MHz max.
  // AD7843 : mode 00 (SCK low idle);  2MHz max.
  // MCP4231: mode 00 / 11 (SCK low/high idle); 10MHz max.
  SPI1C1 = SPI1C1_SPE_MASK | SPI1C1_MSTR_MASK;         // master mode 00 (CPHA/CPOL); SS = GPIO; msb first
  SPI1C2 = 0;                                          // separate pin MISO/MOSI
  #if (CPUCLK <= (1200000L*2))
    SPI1BR = 0;                    // BUSCLK : 2 (1x2)
  #elif (CPUCLK <= (2300000L*2))
    SPI1BR = 0;                    // BUSCLK : 2 (1x2)
  #else // if  (CPUCLK <= (4500000L*2))
    SPI1BR = SPI1BR_SPPR0_MASK;    // BUSCLK : 4 (2x2)
  #endif


//  SPI1BR = 0xFF;                    // pi� piano che posso 
//  SPI1BR = 0x00;                    // velocit� smodata
  SPI1BR = 0x33;                    // velocit� 70KHz

  b1 = SPI1S;         // dummy read (status register)
  #endif
  */
  /* ### */
  asm CLI;                             /* Enable interrupts */
} /*MCU_init*/


/*
** ===================================================================
**     Interrupt handler : Timer_2milli
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void Timer_2milli(void)
{
  TPM1C0V += COUNTER_2MS;    // retrigger timer
  // PIN_TEST = ~PIN_TEST;           //!!!!! debug

  // Timer_colonna();

  if (timer_driver_der)
 	--timer_driver_der;

  if(timer_repeat_tone)       // for management tone
    timer_repeat_tone--;

  if(timer_door_openuse_tone)       // for management tone
    {
    if (--timer_door_openuse_tone == 0)
      SignalInt(tone_proc, NOTE_PAUSE_END);
    }

	if (timer_10ms)
    --timer_10ms;

	if(TEST_AA)
		TEST_AA--;
	if(Slave_Spi_EN_Dly)
	{
		Slave_Spi_EN_Dly--;
		if(!Slave_Spi_EN_Dly)
			SS= ON;
	}

  TPM1C0SC_CH0F = 0;

}
/* end of Timer_2milli */

/*
** ===================================================================
**     Interrupt handler : capture_ISR
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void capture_ISR(void)
{
	unsigned long int temp1;
	if(TPM1C4SC_CH4F)
	{
		TPM1C4SC_CH4F=0;
		if((Common_falg.bits.EN_Capture==1)&&(BlindAudioWaveWaitTime==0))
		{
			capture_counter[TPM2_capture_times]=TPM1C4V;
			capture_overflow[TPM2_capture_times]=TPM2_time_overflow_times;
			
			TPM2_ch1_interrupt_times++;
			
			TPM2_capture_times++;
			if(TPM2_capture_times>1)
			{
				TPM2_capture_times=1;
				temp1=pluse_width_val(capture_counter[0],capture_counter[1],capture_overflow[0],capture_overflow[1]);
				capture_counter[0]=capture_counter[1];
				capture_overflow[0]=capture_overflow[1];
				if(TPM2_ch1_interrupt_times==1)
					blind_audio_dec_times=0;
				if(TPM2_ch1_interrupt_times==2)
				{
					#ifdef High_Speed
						if((temp1>39420)&&(temp1<59125))   //æ���źŵ�Ƶ��Ϊ300Hz~450Hz���е�һ��
					#else
						if((temp1>9873)&&(temp1<14810))
					#endif
					{
						blind_audio_dec_times=0;
						pluse_width_base=temp1;
					}
				}
				else if(TPM2_ch1_interrupt_times>2)
				{
					//if((temp1>(pluse_width_base-3000))&&(temp1<(pluse_width_base+3000)))
					if((temp1>(pluse_width_base-300))&&(temp1<(pluse_width_base+300)))
					{
						blind_audio_dec_times++;
						blind_audio_error_times=0;
						if(blind_audio_dec_times>80)
						{
							//blind_audio_dec_times=0;
							
							//blind_audio_flag=1;
							blind_audio_period_limit=15;
							
							blind_audio_period_times++;
							
							if((blind_audio_period_times>1)&&(BlindAudio_NO>20))
								blind_audio_period_times=0;
							BlindAudio_NO=0;
													
							BlindAudioWaveWaitTime=15;
							if(blind_audio_period_times>5)
							{
								blind_audio_period_times=0;
								//if(BlindAudioDecTime)
								//{
									Common_falg.bits.blind_audio=1;								
								//}
								//else
								//	BlindAudioDecTime=600;
							}
							
							TPM2_ch1_interrupt_times=0;
							TPM2_time_overflow_times=0;
							TPM2_capture_times=0;
							blind_audio_dec_times=0;
							pluse_width_base=0;
						}
						
					}
					else
					{
						blind_audio_error_times++;
						if(blind_audio_error_times>1)
						{		
							blind_audio_error_times=0;
							TPM2_ch1_interrupt_times=0;
							TPM2_time_overflow_times=0;
							TPM2_capture_times=0;
							blind_audio_dec_times=0;
							pluse_width_base=0;
						}
					}
				}
				else
				{
					TPM2_ch1_interrupt_times=0;
					TPM2_time_overflow_times=0;
					TPM2_capture_times=0;
					blind_audio_dec_times=0;
					pluse_width_base=0;
				}
			}
		}
	}

}

__interrupt void Vtpm1ovf_ISR(void)
{
	
	if(TPM1SC_TOF)
	{
		TPM1SC_TOF=0;

		if(Common_falg.bits.EN_Capture==1)
		{
		TPM2_time_overflow_times++;
		
		if(TPM2_time_overflow_times>100)
		{
			TPM2_time_overflow_times=0;
			TPM2_capture_times=0;
			//TPM2_STATUS|=(uint32_t)TPM_STATUS_CH1F_MASK;
			
		}
		}
	}

}


/*__interrupt void SPI_ISR(void)
{
	uchar temp;
	if(SPI1S_SPTEF)
	{
		temp=SPI1D;	SPI1C1_SPE=0;//test

	}
	if(SPI1S_SPRF)
		temp=SPI1D;
	if(SPI1S_MODF)
		temp=SPI1D;
}*/

/*
** ===================================================================
**     Interrupt handler : isr_tone_chiam
**
**     Description :
**         Management tone call
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isr_tone_chiam(void)
{
  if (--numper == 0)
    {  // scaduto time emissione Fx (phase ON o phase OFF)
    if ((phase_tone != 0) && (numper_off != 0))
      {  // end phase ON with phase OFF non nulla --> disactivate pin for start phase OFF
      TPM2C0SC &= ~(TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0A_MASK);   // disactivate pin on TPM2
      numper = numper_off;                // for the durata richiesta
      }
    else
      {  // end phase OFF o end phase ON with phase OFF nulla --> passo ad altra frequency
      TPM2C0SC = TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0A_MASK;   // activate pin on TPM2
      if (freq_tone == 0)
        {  // set the parametri F1
        numper_on = numper = numper_f1_on;
        numper_off = numper_f1_off;
        TPM2MOD = period_f1;
        TPM2C0V = period_f1_on;
        }
      else
        {  // prima of ricominciare with F0, check se loop iterations sequence terminato
        if (timer_repeat_tone == 0)
          { // termine sequence tone bitonal
          TPM2C0SC &= ~(TPM2C0SC_MS0B_MASK | TPM2C0SC_ELS0A_MASK);   // disactivate pin on TPM2
          TPM2SC_TOIE = 0;    // disable interrupt
          // signal to application
          SignalInt(tone_proc, BI_TONE_END);
          }
        else
          { // set the parametri F0
          numper_on = numper = numper_f0_on;
          numper_off = numper_f0_off;
          TPM2MOD = period_f0;
          TPM2C0V = period_f0_on;
          }  // endif
        }  // endif

      freq_tone = ~freq_tone;
      }  // endif
    phase_tone = ~phase_tone;
    }  // endif
  TPM2SC_TOF = 0;    // reset flag
}
/* end of isr_tone_chiam */

/*
** ===================================================================
**     Interrupt handler : isrVicg
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVicg(void)
{
  /* Write your interrupt code here ... */

}
/* end of isrVicg */


/*
** ===================================================================
**     Interrupt handler : isrVirq
**
**     Description :
**         User interrupt service routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isrVirq(void)
{
  /* Write your interrupt code here ... */
   if(IRQSC_IRQF) {
       IRQSC_IRQEDG=~IRQSC_IRQEDG;
       IRQSC_IRQACK=1;
	   //P_LCD_ON = ~P_LCD_ON;
    }
}
/* end of isrVirq */



/* Initialization of the CPU registers in FLASH */

/* NVPROT: FPS7=1,FPS6=1,FPS5=1,FPS4=1,FPS3=1,FPS2=1,FPS1=1,FPDIS=1 */
const unsigned char NVPROT_INIT @0x0000FFBD = 0xFF;

/* NVOPT: KEYEN=0,FNORED=1,SEC01=1,SEC00=0 */
const unsigned char NVOPT_INIT @0x0000FFBF = 0x7E;



extern void _Startup(void);

/* Interrupt vector table */
#define UNASSIGNED_ISR 0xFFFF          /* unassigned interrupt service routine */

void (* const _vect[])() @0xFFCC = {   /* Interrupt vector table                     */
         UNASSIGNED_ISR,               /* Int.no. 25 Vrti (at FFCC)                  */
         UNASSIGNED_ISR,               /* Int.no. 24 Viic1 (at FFCE)                 */
         UNASSIGNED_ISR,               /* Int.no. 23 Vadc1 (at FFD0)                 */
         UNASSIGNED_ISR,               /* Int.no. 22 Vkeyboard1 (at FFD2)            */
         isrVsci2tx,               /* Int.no. 21 Vsci2tx (at FFD4)               */
         isrVsci2rx,               /* Int.no. 20 Vsci2rx (at FFD6)               */
         isrVsci2err,               /* Int.no. 19 Vsci2err (at FFD8)              */
         isrVsci1tx,                   /* Int.no. 18 Vsci1tx (at FFDA)               */
         isrVsci1rx,                   /* Int.no. 17 Vsci1rx (at FFDC)               */
         isrVsci1err,                  /* Int.no. 16 Vsci1err (at FFDE)              */
//          UNASSIGNED_ISR,               /* Int.no. 18 Vsci1tx (at FFDA)               */
//          UNASSIGNED_ISR,               /* Int.no. 17 Vsci1rx (at FFDC)               */
//          UNASSIGNED_ISR,               /* Int.no. 16 Vsci1err (at FFDE)              */
         UNASSIGNED_ISR,               /* Int.no. 15 Vspi1 (at FFE0)                 */
         isr_tone_chiam,               /* Int.no. 14 Vtpm2ovf (at FFE2)              */
         UNASSIGNED_ISR,               /* Int.no. 13 Vtpm2ch1 (at FFE4)              */
         UNASSIGNED_ISR,               /* Int.no. 12 Vtpm2ch0 (at FFE6)              */
         Vtpm1ovf_ISR,               /* Int.no. 11 Vtpm1ovf (at FFE8)              */
         UNASSIGNED_ISR,               /* Int.no. 10 Vtpm1ch5 (at FFEA)              */
         capture_ISR,               /* Int.no.  9 Vtpm1ch4 (at FFEC)              */
         UNASSIGNED_ISR,               /* Int.no.  8 Vtpm1ch3 (at FFEE)              */
         TimerDriver_der,              /* Int.no.  7 Vtpm1ch2 (at FFF0)              */
         isr_Pin_rx_der,               /* Int.no.  6 Vtpm1ch1 (at FFF2)              */
         Timer_2milli,                 /* Int.no.  5 Vtpm1ch0 (at FFF4)              */
         isrVicg,                      /* Int.no.  4 Vicg (at FFF6)                  */
         UNASSIGNED_ISR,               /* Int.no.  3 Vlvd (at FFF8)                  */
         isrVirq,                      /* Int.no.  2 Virq (at FFFA)                  */
         UNASSIGNED_ISR,               /* Int.no.  1 Vswi (at FFFC)                  */
         _Startup                      /* Int.no.  0 Vreset (at FFFE)   Reset vector */
 };


unsigned long int pluse_width_val(unsigned int begin_counter,unsigned int end_counter,unsigned char begin_over_times,unsigned char end_over_times)
{
	if((end_over_times-begin_over_times)>0)
	{
		TPM2_ch1_pluse_val=0xffff*(end_over_times-begin_over_times-1)+(0xffff-begin_counter)+end_counter;
	}
	else
		TPM2_ch1_pluse_val=end_counter-begin_counter;
	
	return TPM2_ch1_pluse_val;
}

/* END */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.98 [03.80]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/

