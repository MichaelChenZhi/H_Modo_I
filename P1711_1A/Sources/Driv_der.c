/*
    $Id: Driv_der.c,v 1.1 2011/09/01 07:45:02 chnmhl Exp $
    $Log: Driv_der.c,v $
    Revision 1.1  2011/09/01 07:45:02  chnmhl
    First release

    Revision 1.1  2011/06/13 14:31:10  vrzrrt
    First commit. This module is only for translation purposes and will be never updated.

    Revision 1.4  2009/10/05 15:16:30  vrzrrt
    wip on blink tasti mute and audio

    Revision 1.3  2009/09/11 15:15:17  vrzrrt
    WIP on variazione suonerie

    Revision 1.2  2009/05/20 10:21:08  vrzrrt
    Commit of allinemento commenti in testa ai files

    Revision 1.1  2009/05/18 10:04:44  vrzrrt
    Primo committ. Versione debug Villata

*/


/*²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
  ²²                                                                       ²²
  ²²                                                                       ²²
  ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²*/

#include <hidef.h> /* for EnableInterrupts macro */
#include "MC9S08AW60.h"                
#include "define.h"
#include "global.h"
#include "driv_der.h"
#include "trame.h"


/*-- Parameters ---------------------------------------------------*/
// line at 200 baud --> 50ms = Tbyte
#define TIME_BASE_DER              10    // ms

#define TIME_BETWEEN_TWO_CHARS_IN_DER    70 / TIME_BASE_DER  

#define TIME_INTERFRAME_DER        40 / TIME_BASE_DER  

#define TIME_WAIT_DATAREADY_TX_DER   80 / TIME_BASE_DER 

#define TIME_MAX_WAIT_CNF        3000 / TIME_BASE_DER  

#define MAX_DER_TX_RETRY            10

/*--------------------------------------------------------------------------*/

/*-- Interface -----------------------------------------------------------*/
unsigned char driver_der_events;


#pragma DATA_SEG FAR_RAM
queue_tx_der  der_queue_tx_appl[TX_DER_QUEUE_DIMENSION];
queue_tx_der  *p_der_queue_tx_appl = &der_queue_tx_appl[0]; 

unsigned char data_der_rx_appl[LEN_RX_BUFF_DER];
unsigned char *pointer_data_der_rx_appl = &data_der_rx_appl[0];
#pragma DATA_SEG DEFAULT


#pragma DATA_SEG FAR_RAM
unsigned char queue_index_tx_in_der;     // index queue for add
unsigned char queue_index_tx_out_der;    // index queue for get
#pragma DATA_SEG DEFAULT

/*--------------------------------------------------------------------------*/
unsigned char timer_driver_der;
unsigned char der_protocol_manager_state;
#pragma DATA_SEG FAR_RAM
unsigned char der_protocol_flags = 0;
unsigned char der_idle_detector;
unsigned char der_collision_detected;
#pragma DATA_SEG DEFAULT

#define DER_PROTOCOL_MANAGER_IDLE    0
#define DER_PROTOCOL_WAIT_CONFIRM   1

unsigned char run_gestprot_der = 0;

unsigned char mac_der_state = 0;
#define DER_MAC_IDLE      0
#define DER_MAC_FIRST_SAMP  1
#define DER_MAC_SECOND_SAMP 2
#define DER_MAC_THIRD_SAMP  3
#define DER_MAC_END_BIT     4
#define DER_MAC_BUS_ERROR   5       

unsigned char counter_der_line_state = 0;

#pragma DATA_SEG FAR_RAM
unsigned char buffer_tx_der[LEN_TX_BUFF_DER];
unsigned char buffer_rx_der[LEN_RX_BUFF_DER];

unsigned char cnt_bit_rx_der;
unsigned char sample_bit_der;
unsigned char tx_der_buffer_index = 0;
unsigned char rx_der_buffer_index = 0;
unsigned char n_byte_to_tx_der;
unsigned char n_byte_rx_der;
unsigned int  char_tx_der;
unsigned int  bck_char_tx_der;
unsigned int  char_rx_der;
unsigned char dataready_tx_der_wait_timer = 0;      
unsigned char timer_interchar_der;
unsigned char timer_interframe_der;
unsigned char tx_der_retry;
unsigned char der_collision_counter;
unsigned char der_wrong_frame_counter;
unsigned char der_rx_error;
unsigned char counter_rnd_der;        
unsigned int  wait_conf_guard_timer;

extern unsigned char der_appl_state; 

void GestProt_der(void);
void Mac_der(void);
void FormatFrameTx_der(void);
void FormatFrameRx_der(void);
void InitProtocol_der(void);
void RxInterrupt_der(void);
void TimerProtocol_der(void);
unsigned char CheckLenght_der(void);
void InitHWProtocol_der(void);
void StartTx_der(void);
void TxBuffer_der(void);
void InitTx_der(void);
unsigned char EndRx_der(void);
void InitRx_der(void);
void Incr_index_queue_in_der(void);
void Incr_index_queue_out_der(void);
void EnableStartRx_der(void);
void DisableStartRx_der(void);
void Enable_int_rx_der(void);

void SignalPunt(unsigned char *target, unsigned char mask);

// monitor void Signal(unsigned char *target, unsigned char mask);
// monitor void Free(unsigned char *target, unsigned char mask);
// monitor void GoTimer(unsigned char *timer, unsigned char value);
// monitor void StopTimer(unsigned char *timer);
// --------------------------------------------------*/

void FineRxKO(unsigned char errorcode);
#define DER_RXKO_OVERFLOW_BUFFER     1
#define DER_RXKO_UART_ERROR          2
#define DER_RXKO_CHKS_KO             3
#define DER_RXKO_BUSY_BUFFER         4
#define DER_RXKO_FRAME_LENGTH        5
#define DER_RXKO_SC_DETECTED         6
#define DER_RXKO_SC_END              7

/*================================================================*/
unsigned char InitializeDRV_der(void)
/*================================================================*/
{
  unsigned char result = OK;

  queue_index_tx_in_der = 0;
  queue_index_tx_out_der = 0;
  der_protocol_manager_state = DER_PROTOCOL_MANAGER_IDLE;
  der_idle_detector = NO;
  der_collision_detected = 0;

  InitHWProtocol_der();
  Signal(run_gestprot_der, DER_INIT_GEST_PROT);

  return (result);
}

/*================================================================*/
void TxRequestDRV_der(void)
/*================================================================*/
{
 ;// DO NOT CALL  Signal(run_gestprot_der, DER_TX_REQUEST);
}

/*================================================================*/
void GestProt_der(void)
/*================================================================*/
{
  if (run_gestprot_der & DER_INIT_GEST_PROT)
    {
    Free(run_gestprot_der, DER_INIT_GEST_PROT);
    InitProtocol_der();
    }
  else if (run_gestprot_der & DER_RX_READY)
    {
    Free(run_gestprot_der, DER_RX_READY);
    if (EndRx_der() == OK)   
      {
      Signal(driver_der_events, DER_RX_DATA_INDICATION);  
      der_protocol_flags &= ~DER_BUSDER_ERROR;   
      }
    }
  else if (run_gestprot_der & DER_RX_ERROR)
    {
    Free(run_gestprot_der, DER_RX_ERROR);
    // Signals an unsuccesful RX to the application
    Signal(driver_der_events, DER_RX_ERROR_INDICATION);
    }

  if (timer_driver_der == 0)
    {
    TimerProtocol_der();
    GoTimer(timer_driver_der, TIME_BASE_DER);      // retrigger timer

    }

  // Check on the transmission queue
  if ((mac_der_state == DER_MAC_IDLE) && (der_protocol_manager_state == DER_PROTOCOL_MANAGER_IDLE) && (queue_index_tx_out_der != queue_index_tx_in_der ))
    {  // se c'è qualcosa in queue by trasmettere and sono idle
    Signal(run_gestprot_der, DER_TX_REQUEST);
    }  // endif

  counter_rnd_der = TPM1CNTH;
  counter_rnd_der = TPM1CNTL;
  counter_rnd_der &= 0x0f;

  if (der_idle_detector != NO)
    {
    if (RX_DER == 0)
      {  // line still busy
      timer_interframe_der = TIME_INTERFRAME_DER + counter_rnd_der;  // retrigger timer in modo che to rilascio bus il timer abbia il value total
      }
    else
      {  // bus released
      der_idle_detector = NO;
      }
    }
    
  //  WDLINE ----------------------------
  if(WD_line_der >= 1000/TIME_BASE_DER) {
    WD_line_der = 0;
    der_appl_state = 0;
  }
  //  WDLINE ----------------------------

  switch (der_protocol_manager_state)
    {
    /*------------------*/
    case DER_PROTOCOL_MANAGER_IDLE:
    /*------------------*/
      if (run_gestprot_der & DER_TX_REQUEST)
        {  // application tx request
        if ((mac_der_state == DER_MAC_IDLE) && (timer_interframe_der == 0))
          { 
          FormatFrameTx_der();      
          StartTx_der();             // start with a transmission (beware: could be started in the same moment a reception from a remote device)
          der_protocol_manager_state = DER_PROTOCOL_WAIT_CONFIRM;
          Free(run_gestprot_der, (DER_TX_REQUEST + DER_TX_CNF_POS + DER_TX_CNF_NEG));
          }  // endif
        }
      break;

    /*------------------*/
    case DER_PROTOCOL_WAIT_CONFIRM:
    /*------------------*/
      if (wait_conf_guard_timer == 0)
        wait_conf_guard_timer = TIME_MAX_WAIT_CNF;
      if (run_gestprot_der & DER_TX_CNF_POS)
        { 
        Free(run_gestprot_der, DER_TX_CNF_POS);
        tx_der_retry = 0;
        timer_interframe_der = TIME_INTERFRAME_DER;       
        EnableStartRx_der();

        Incr_index_queue_out_der();                      

        // Signal a succesfull tx to the application
        Signal(driver_der_events, DER_TX_POSITIVE_INDICATION);
        der_protocol_manager_state = DER_PROTOCOL_MANAGER_IDLE;
        der_protocol_flags &= ~DER_BUSDER_ERROR;      // switch off  eventuale segnalazione of error
        wait_conf_guard_timer = 0;
        }
      else if (run_gestprot_der & DER_TX_CNF_NEG)
        {  // tx failed (for bus ko or collision)
        Free(run_gestprot_der, DER_TX_CNF_NEG);

        timer_interframe_der = TIME_INTERFRAME_DER + counter_rnd_der; //DB 15gen08 
        EnableStartRx_der();
        if (++tx_der_retry >= MAX_DER_TX_RETRY)
          {  
          tx_der_retry = 0;
          // Signal an unsuccesfull tx to the application
          Signal(driver_der_events, DER_TX_NEGATIVE_INDICATION);
          Incr_index_queue_out_der();                  
          der_protocol_flags |= DER_BUSDER_ERROR;      
          }

        der_protocol_manager_state = DER_PROTOCOL_MANAGER_IDLE;
        wait_conf_guard_timer = 0;
        }
      break;

    default:
      break;
    }
}

/*================================================================*/
void InitProtocol_der(void)
/*================================================================*/
{
  der_protocol_manager_state = DER_PROTOCOL_MANAGER_IDLE;
  mac_der_state = DER_MAC_IDLE;
  n_byte_to_tx_der = 0;
  tx_der_buffer_index = 0;
  timer_interchar_der = 0;
  char_tx_der = 0;
  char_rx_der = 0;
  dataready_tx_der_wait_timer = 0;
  tx_der_retry = 0;
  der_idle_detector = NO;
  counter_der_line_state = 0;
  der_collision_detected = NO;
  EnableStartRx_der();
  GoTimer(timer_driver_der, TIME_BASE_DER);      // retrigger timer
}

/*================================================================*/
void StartTx_der(void)
/*================================================================*/
{
  EnableStartRx_der();     // enable also the recption to check the collision   

  InitTx_der();           
  der_protocol_flags |= DER_TX_IN_PROGRESS;     
  TxBuffer_der();
}

unsigned char *pt;
/*================================================================*/
void FormatFrameTx_der(void)
/*================================================================*/
{
unsigned char i;
unsigned char chksum;               // checksum

  n_byte_to_tx_der = der_queue_tx_appl[queue_index_tx_out_der].n_byte;       // number of byte by tx (checksum escluso)
  pt = (unsigned char *) der_queue_tx_appl[queue_index_tx_out_der].data;
  chksum = 0;
  for (i=0; i<n_byte_to_tx_der; i++)
    {
    buffer_tx_der[i] = *pt++;
    chksum ^= buffer_tx_der[i];
    }  // endfor

  chksum = (chksum ^ 0xff) + URMET_COMPANY_DER;          // lo nego and sommo il code costruttore
  buffer_tx_der[n_byte_to_tx_der] = chksum;
  n_byte_to_tx_der++;     // + cks

}

/*================================================================*/
void FormatFrameRx_der(void)
/*================================================================*/
{
unsigned char i;

  for (i = 0; i < n_byte_rx_der; i++)
    {
    pointer_data_der_rx_appl[i] = buffer_rx_der[i];
    }
}

/*================================================================*/
void FineRxKO_der(unsigned char errorcode_der)
/*================================================================*/
{
  ++der_wrong_frame_counter;
  Signal(run_gestprot_der, DER_RX_ERROR);
  der_rx_error = (errorcode_der << 4);

}

/*================================================================*/
unsigned char Check_the_Checksum_der(void)
/*================================================================*/
{
unsigned char i, result = KO;
unsigned char chksum;

  chksum = 0;
  for (i = 0; i < (rx_der_buffer_index - 1); i++)
    {
    chksum ^= buffer_rx_der[i];
    }
  chksum = (chksum ^0xff) + URMET_COMPANY_DER;       
  if (chksum == buffer_rx_der[rx_der_buffer_index - 1])
    {
    result = OK;
    }

  return (result);
}

/*================================================================*/
unsigned char EndRx_der(void)
/*================================================================*/
{
unsigned char result = KO;

  n_byte_rx_der = rx_der_buffer_index;

  if (CheckLenght_der() == OK)
    {
    if (Check_the_Checksum_der() == OK)
      {
      result = OK;
      }
    else
      {
      /* Cks KO */
      FineRxKO_der(DER_RXKO_CHKS_KO);
      }
    }
  else
    {
    // Frame too short or too long
    FineRxKO_der(DER_RXKO_FRAME_LENGTH);
    }

  if ( der_collision_detected != NO)
    {
    der_collision_detected = NO;
    timer_interframe_der = TIME_INTERFRAME_DER + counter_rnd_der; //DB 15gen08 
    }
  else  
  timer_interframe_der = TIME_INTERFRAME_DER;

  if (RX_DER == 0)
    {  
    der_idle_detector = YES;    
    }
  
  mac_der_state = DER_MAC_IDLE;
  EnableStartRx_der();
  return (result);
}

/*================================================================*/
unsigned char CheckLenght_der(void)
/*================================================================*/
{
unsigned char result = KO;

  if ((n_byte_rx_der >= MIN_FRAME_LEN_DER) && (n_byte_rx_der <= MAX_FRAME_LEN_DER))
    {
    result = OK;
    }
  return (result);
}

/*================================================================*/
void InitTx_der(void)
/*================================================================*/
{
  tx_der_buffer_index = 0;
  rx_der_buffer_index = 0;
}

/*================================================================*/
void TxBuffer_der(void)
/*================================================================*/
{
  char_tx_der = buffer_tx_der[tx_der_buffer_index++];

  char_tx_der <<=1;        
  char_tx_der &= ~0x0001;    // start bit = 0
  char_tx_der |= 0x0200;     // stop bit = 1
  bck_char_tx_der = char_tx_der;     

  if (RX_DER != 0)
    { // free line --> transmission
    dataready_tx_der_wait_timer = TIME_WAIT_DATAREADY_TX_DER;   
    TX_DER = 1;               // start transmitting the start bit
    timer_interchar_der = TIME_BETWEEN_TWO_CHARS_IN_DER;     
    }
  else
    { 
    TX_DER = 0;           // start transmission (with lo start bit)
    dataready_tx_der_wait_timer = 0;    
    if (mac_der_state != DER_MAC_IDLE)
      {  // someone is transmitting --> I've yet received the start bit (interrupt)
      }
    else
      { 
      timer_interframe_der = TIME_INTERFRAME_DER;  
      InitRx_der();                 // init the reception
      timer_interchar_der = 0;         // isn't a frame
      cnt_bit_rx_der = 0;
      sample_bit_der = 0;
      der_idle_detector = YES;     // start the bus monitoring to detect the end of pliers on bus
      }
    der_protocol_manager_state = DER_PROTOCOL_WAIT_CONFIRM;
    Signal(run_gestprot_der, DER_TX_CNF_NEG);   // to schedule again the failed transmission
    }
}

/*================================================================*/
void InitRx_der(void)
/*================================================================*/
{
  counter_der_line_state = 0;
  cnt_bit_rx_der = 0;
}

/*================================================================*/
void TimerProtocol_der(void)
//    gira a TIME_BASE_DER
/*================================================================*/
{
  if (timer_interframe_der)
    {
    DisableInterrupts;
    --timer_interframe_der;
    if (timer_interframe_der == 0)
      {
      }
    EnableInterrupts;
    }

  if (timer_interchar_der)
    {
    DisableInterrupts;
    --timer_interchar_der;
    if (timer_interchar_der == 0)
      {
      Signal(run_gestprot_der, DER_RX_READY);      // end Rx
      }
    EnableInterrupts;
    }

  if (dataready_tx_der_wait_timer)
    {
    DisableInterrupts;
    --dataready_tx_der_wait_timer;
    if (dataready_tx_der_wait_timer == 0)
      { 
      der_protocol_flags &= ~DER_TX_IN_PROGRESS;
      der_protocol_manager_state = DER_PROTOCOL_WAIT_CONFIRM;  
      Signal(run_gestprot_der, DER_TX_CNF_NEG);
      TX_DER = 0;           // back to idle
      }
    EnableInterrupts;
    }

  if (wait_conf_guard_timer)
{
    --wait_conf_guard_timer;
    if (wait_conf_guard_timer == 0)
   {
      der_protocol_manager_state = DER_PROTOCOL_MANAGER_IDLE;
   }
    EnableInterrupts;
    }

  // WDLINE ----------------------------
  if(TX_DER)
      WD_line_der++;
  else 
      WD_line_der = 0;
  // WDLINE ----------------------------

}

/*================================================================*/
void Incr_index_queue_in_der(void)
/*================================================================*/
{
  if (++queue_index_tx_in_der >= TX_DER_QUEUE_DIMENSION)
    queue_index_tx_in_der = 0;
}

/*================================================================*/
void Incr_index_queue_out_der(void)
/*================================================================*/
{
  if (++queue_index_tx_out_der >= TX_DER_QUEUE_DIMENSION)
    queue_index_tx_out_der = 0;
}

/*--------------------------------------------------------------------------*/
/*                              Physical Driver                             */
/*--------------------------------------------------------------------------*/

/*================================================================*/
void InitHWProtocol_der(void)
/*================================================================== A.T. ==*/
{
  DisableInterrupts;

  // set TPM1C1 as input capture to detect start bit
  TPM1C1SC = TPM1C1SC_ELS1B_MASK;      // input capture on falling edge;
  TPM1C1SC_CH1F = 0;                  // clear possible pending flags

  // set TPM1C2 as output compare to sample data line
  TPM1C2SC = TPM1C2SC_MS2A_MASK;           // output compare

  TX_DER = 0;// WDLINE ----------------------------
  EnableInterrupts;
}

/*================================================================*/
void EnableStartRx_der(void)
/*================================================================*/
{
  DisableInterrupts;    // to ensure that a rx interrupt can't happen
  if (mac_der_state == DER_MAC_IDLE)
    {   // for non riabilitare a reception appena partita che avrebbe disabilitato l'interrupt
    TPM1C1SC_CH1F = 0;                   // clear possible pending flags
    TPM1C1SC_CH1IE = 1;                  // input capture enable
    }
  rx_der_buffer_index = 0;
  EnableInterrupts;
}

/*================================================================*/
void DisableStartRx_der(void)
/*================================================================*/
{
  TPM1C1SC_CH1IE = 0;                  // input capture disable 
  TPM1C1SC_CH1F = 0;                   // clear possible pending flags
}

/*================================================================*/
void Enable_int_rx_der(void)
//     !!!! by call se all'internal of un interrupt
/*================================================================*/
{
  TPM1C1SC_CH1F = 0;                   // clear possible pending flags
  TPM1C1SC_CH1IE = 1;                  // input capture enable
}

/*
** ===================================================================
**     Interrupt handler : isr_Pin_rx_der
**
**     Description :
**         Interrupt start bit
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void isr_Pin_rx_der(void)
{
unsigned int time;
  DisableStartRx_der();        // interrupt only on start bit
  timer_interframe_der = 0;
  InitRx_der();                
  timer_interchar_der = TIME_BETWEEN_TWO_CHARS_IN_DER;        
  dataready_tx_der_wait_timer = 0;   
  mac_der_state = DER_MAC_FIRST_SAMP;
  cnt_bit_rx_der = 0;
  sample_bit_der = 0;
  time = TPM1C1V;                  // save the edge time marker 
  TPM1C2V = time + COUNTER_2MS;    // set the time for first sample
  TPM1C2SC_CH2F = 0;               // clear possible pending flags
  TPM1C2SC_CH2IE = 1;              // enable interrupt timer
}

/*
** ===================================================================
**     Interrupt handler : TimerDriver_der
**
**     Description :
**         Timer for management Tx/Rx line
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
__interrupt void TimerDriver_der(void)
{
  switch (mac_der_state)
    {
    case DER_MAC_IDLE:

      break;

    case DER_MAC_FIRST_SAMP:            // first sample
      if (RX_DER)                   
        sample_bit_der++;
      TPM1C2V += COUNTER_500US;         // set the time for second sample
      mac_der_state = DER_MAC_SECOND_SAMP;
      break;

    case DER_MAC_SECOND_SAMP:           // second sample
      if (RX_DER)
        sample_bit_der++;
      TPM1C2V += COUNTER_500US;         // set the time for third sample
      mac_der_state = DER_MAC_THIRD_SAMP;
      break;

    case DER_MAC_THIRD_SAMP:            // third sample
      if (RX_DER)
        sample_bit_der++;
      TPM1C2V += COUNTER_2MS;           // set the time for end bit
      mac_der_state = DER_MAC_END_BIT;
      // decisione a maggioranza
      char_rx_der >>= 1;
      if (sample_bit_der >= 2)
        {
        char_rx_der |= 0x0200;
        sample_bit_der = 0x01;           // save for check with tx
        }
      else
        {
        sample_bit_der = 0x00;           // save for check with tx
        }  // endif
      if (++cnt_bit_rx_der >= (NUM_BIT_PER_BYTE_DER + 1 + NUM_STOP_BIT_DER))
        {  // sull'last sample dello stop bit abilito nuovamente interrupt on pin rx for evitare of perdere
           // il next start bit se sono troppo lento
           // devo quindi fare tutte the azioni del end byte perchè potrei non andare nello state successivo
        Enable_int_rx_der();
        // bufferizza comunque the reception (anche se tx)
        buffer_rx_der[rx_der_buffer_index++] = (unsigned char) (char_rx_der >> 1);
        if (rx_der_buffer_index >= LEN_RX_BUFF_DER)
          rx_der_buffer_index--;
        
        if (der_protocol_flags & DER_TX_IN_PROGRESS)
          {  // transmission active --> check collisione
          if ((unsigned int)char_rx_der != (unsigned int)bck_char_tx_der)
            {  // segnalo transmission fallita
            TX_DER = 0;           // riporto idle
            der_protocol_flags &= ~DER_TX_IN_PROGRESS;      //  and interrompo tx
            der_collision_detected = YES;
            der_protocol_manager_state = DER_PROTOCOL_WAIT_CONFIRM;  // 20080924 safety clear, nel caso qualcuno l'avesse messo idle
            SignalInt(run_gestprot_der, DER_TX_CNF_NEG);
            }  // endif
          }
        // timer_interchar_der = TIME_BETWEEN_TWO_CHARS_IN_DER;      //!!!!! verify  // set the timeout for rilevamento end frame
        }
      break;

    case DER_MAC_END_BIT:
      if (cnt_bit_rx_der >= (NUM_BIT_PER_BYTE_DER + 1 + NUM_STOP_BIT_DER))
        {  // end tx/rx byte
        if (der_protocol_flags & DER_TX_IN_PROGRESS)
          {  // transmission active (the collisione è già stata controllata)
          if (tx_der_buffer_index != n_byte_to_tx_der)
            {
            TxBuffer_der();      // next byte
            }
          else
            {
            /* Fine Tx */
            der_protocol_flags &= ~DER_TX_IN_PROGRESS;
            Enable_int_rx_der();
            timer_interchar_der = 0;          // for non segnalare reception
            der_protocol_manager_state = DER_PROTOCOL_WAIT_CONFIRM;     // safety clear, nel caso qualcuno l'avesse messo idle 
            SignalInt(run_gestprot_der, DER_TX_CNF_POS);
            mac_der_state = DER_MAC_IDLE;
            TPM1C2SC_CH2IE = 0;          // disable interrupt timer
            }
          }
        else
          {  // reception active
          // timer_interchar_der = TIME_BETWEEN_TWO_CHARS_IN_DER;
          TPM1C2SC_CH2IE = 0;          // disable interrupt timer
          }  // endif
        }
      else
        {  // ancora bit by tx/rx
        TPM1C2V += COUNTER_2MS;           // set the istante for start sample next bit
        mac_der_state = DER_MAC_FIRST_SAMP;
        if (der_protocol_flags & DER_TX_IN_PROGRESS)
          {  // transmission active --> check collisione
          if ((sample_bit_der & 0x01) == ((unsigned char)char_tx_der & 0x01))
            {  // no collisione --> posso continuare a trasmettere
            char_tx_der >>= 1;
            if (char_tx_der & 0x0001)
              TX_DER = 0;
            else
              TX_DER = 1;
            }
          else
            {  // collisione rilevata --> blocco the transmission and segnalo for the schedulazione for un time successivo
            TX_DER = 0;           // riporto idle
            der_protocol_flags &= ~DER_TX_IN_PROGRESS;
            der_collision_detected = YES;
            der_protocol_manager_state = DER_PROTOCOL_WAIT_CONFIRM;  // 20080924 safety clear, nel caso qualcuno l'avesse messo idle
            SignalInt(run_gestprot_der, DER_TX_CNF_NEG);
            }  // endif
          }  // endif
        }  // endif
      sample_bit_der = 0;     // for next bit
      break;

    case DER_MAC_BUS_ERROR:

      break;
    }  // endswitch

   // TimerProtocol_der();
   //
   // GestProt_der();
   // if (der_idle_detector)
   //   {
   //   if (PIN_COLLISIONE == 1)
   //     {
   //     if (++counter_der_line_state >= 10) /* 20 ms */
   //       {
   //       der_protocol_flags |= DER_FLAG_ABIL_ERR_RX;
   //       FineRxKO_der(DER_RXKO_SC_END);
   //       counter_der_line_state = 0;
   //       der_idle_detector = NO;
   //       }
   //     }
   //   else
   //     {
   //     counter_der_line_state = 0;
   //     }
   //   }

  TPM1C2SC_CH2F = 0;                   // clear eventuali flag pendenti

}
/* end of TimerDriver */

/*================================================================*/
unsigned char Abort_frame_der(unsigned char opc)
// Elimina the transmission delle trame with opcode specificato
// L'eliminazione consiste nel sostituire l'opcode with a DUMMY and nel ridurre to minimo i byte by trasmettere.
// L'eliminazione avviene nella queue of tx and se è the frame in progress of tx viene corrotto il checksum
// for non farla riconoscere dagli altri dispositivi. (in caso of conferma negativa of tx, in queue ci sarà the frame with opcode sostituito.
// Input: opc = opcode frame/e by eliminare
// Output: number of trame eliminate
/*================================================================*/
{
unsigned char nr_del;               // counter trame eliminate
unsigned char save_index_out_der;   // for salvare index
  nr_del = 0;
  if (der_protocol_manager_state == DER_PROTOCOL_WAIT_CONFIRM)
    {  // se sono in transmission in progress
    if (buffer_tx_der[0] == opc)
      {  // se sto trasmettendo proprio the frame by eliminare
      buffer_tx_der[n_byte_to_tx_der-1] += 0x33;    // altero il checksum affinchè non venga riconosciuta valida dagli altri
      }  // endif
    }  // endif
  
  save_index_out_der = queue_index_tx_out_der;
  while (queue_index_tx_out_der != queue_index_tx_in_der)
    {  // se ci sono elementi in queue allora check se present frame by eliminare
    if (der_queue_tx_appl[queue_index_tx_out_der].data[0] == opc)
      {  // frame by eliminare
      der_queue_tx_appl[queue_index_tx_out_der].data[0] = D_DUMMY;    // sostituisco l'opcode with a che non fa nulla
      der_queue_tx_appl[queue_index_tx_out_der].n_byte = (MIN_FRAME_LEN_DER - 1);   // and riduco to minimo il number of of byte for non sovraccaricare il bus
      nr_del++;
      }  // endif
    Incr_index_queue_out_der();    // passo a elemento successivo
    }  // endwhile
  queue_index_tx_out_der = save_index_out_der;     // restore index
  
  return nr_del;
}


