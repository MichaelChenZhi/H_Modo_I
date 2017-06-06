

/*께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�
  께                                                                       께
  께                               Ges_der.c                              께
  께                                                                       께
  께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�*/

#include <hidef.h> /* for EnableInterrupts macro */
#include "MC9S08AW60.h"                /* I/O map for MC9S08AW16CFU */
#include "define.h"
#include "global.h"
#include "ges_der.h"
#include "driv_der.h"
#include "FlashWrite.h"
#include "Trame.h"

//extern void FormatTramaRx_col(void);
extern void FormatFrameRx_der(void);
extern void	GestDer_ST_TXCompleted(void);
extern void Frame_AS_ManagementDer(void);
extern void	Gest_ST_TXCompleted(void);
//extern void Write_Data_In_Flash (void);


#pragma DATA_SEG DAFAULT
//unsigned char state_impianto_der;

#pragma DATA_SEG FAR_RAM
unsigned char tx_application_der_data_appl[20];
unsigned char rx_application_der_data_appl[25];
#pragma DATA_SEG DAFAULT

unsigned char der_appl_state;


//void FormatFrame_der(unsigned char frame_der);
void Frame_AS_Management(void);
//void Gest_UnPallino(void);
//void T_Richiesta_SVC(void);
//void Gest_QuattroPallini(void);
//void Gest_DuePallini(void);
// void Gest_Apriporta(void);
void Gest_Panic(void);
void Presence_Absence_Management(void);
//void Gest_Espansione(void);
//void Gest_TrePallini(void);

unsigned char Abort_frame_der(unsigned char opc);

void Courtesy_tone(void);
//void Dissuasion_Tone(void);

#define INIT_DRIVER_DER                   0
#define READY_DER                         1

/*============================================================== 17/03/98 ==*/
void Derivation_Frame_Management(void)
/*================================================================== A.T. ==*/
  {
  
  
   /*----------------------- Eventi line ---------------------------*/
  if (driver_der_events & DER_TX_POSITIVE_INDICATION)
    {
    Free(driver_der_events, DER_TX_POSITIVE_INDICATION);    // DER_TX_COMPLETED
      
    if (PRG_FLASH_AFTER_TX != OFF)
      { 
      PRG_FLASH_AFTER_TX = OFF;
      
      End_tone();
      
      //Write_Data_In_Flash();
      if(SaveInf!=NULL)
	      {
	        (*SaveInf)();
	        SaveInf = NULL;
	      }

      //Update_Signalling_LEDs();
		} 
  	
	
	  Gest_ST_TXCompleted();
      
    }
  /*-----------------------------------------*/
  if (driver_der_events & DER_TX_NEGATIVE_INDICATION)
    {
    Free(driver_der_events, DER_TX_NEGATIVE_INDICATION);
    }
  /*-----------------------------------------*/
  if (driver_der_events & DER_RX_ERROR_INDICATION)
    {
    Free(driver_der_events, DER_RX_ERROR_INDICATION);
#ifdef BROKEN_FRAME
    Dissuasion_Tone();         
#endif    
    }
  /*-----------------------------------------*/


/*- Application -------------------------------------------------*/

  switch (der_appl_state)
    {
    case INIT_DRIVER_DER:
      InitializeDRV_der();
      der_appl_state = READY_DER;
      break;

    case READY_DER:
      if (driver_der_events & DER_RX_DATA_INDICATION)
        {
        Free(driver_der_events, DER_RX_DATA_INDICATION);
				FormatFrameRx_der();
        Frame_AS_Management();

//        Courtesy_tone();         // !!!! debug
        }
      break;

    default:
       break;
    }
}

/*============================================================== 18/03/98 ==*/
void Free_Line(void)
{
	p_der_queue_tx_appl = &der_queue_tx_appl[queue_index_tx_in_der];
	p_der_queue_tx_appl->n_byte                     = 3;
	p_der_queue_tx_appl->data[0]                    = D_FREE_LINE;
	p_der_queue_tx_appl->data[1]                    = User + Internal;
	p_der_queue_tx_appl->data[2]                    = kind_of_my_communication; 
	Incr_index_queue_in_der();                      // update index queue
}
#include "Gest_trame_DER_staffa.c"


