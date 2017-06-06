/*
    $Id: Driv_der.h,v 1.1 2011/09/01 07:45:02 chnmhl Exp $
    $Log: Driv_der.h,v $
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
  께                              DRIV_DER.H                               께
  께                                                                       께
  께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�*/

//#include "driver.h"

#define URMET_COMPANY_DER       0             // for checksum

#define TIME_BASE_DER              10    // ms

#define MIN_FRAME_LEN_DER           3
#define MAX_FRAME_LEN_DER           7
#define TX_DER_QUEUE_DIMENSION             4

#define NUM_BIT_PER_BYTE_DER        8
#define NUM_STOP_BIT_DER            1

#define DER_TX_IN_PROGRESS              B0
#define DER_BUSDER_ERROR             B1
#define FLAG_ERR_DER_SEGNALATO       B2 



unsigned char InitializeDRV_der(void);
void GestProt_der(void);
//void ErroreProtocollo_der(unsigned char error);
//void DEC_TxRequestDRV_der(void);

unsigned char CheckLenght_der(void);
void Incr_index_queue_in_der(void);

__interrupt void isr_Pin_rx_der(void);
__interrupt void TimerDriver_der(void);

#define LEN_TX_BUFF_DER               10
#define LEN_RX_BUFF_DER               10

typedef struct
  {
  unsigned char n_byte;      // data field lenght
  unsigned char data[MAX_FRAME_LEN_DER - 1]; 
  } queue_tx_der;

#pragma DATA_SEG FAR_RAM
extern unsigned char buffer_tx_der[LEN_TX_BUFF_DER];
extern unsigned char buffer_rx_der[LEN_RX_BUFF_DER];
extern queue_tx_der der_queue_tx_appl[TX_DER_QUEUE_DIMENSION];
#pragma DATA_SEG DEFAULT

extern unsigned char driver_der_events;
extern unsigned char timer_driver_der;
#pragma DATA_SEG FAR_RAM
extern queue_tx_der *p_der_queue_tx_appl;
extern unsigned char queue_index_tx_in_der;     // index queue for add
extern unsigned char queue_index_tx_out_der;    // index queue for remove
#pragma DATA_SEG DEFAULT

/* eventi_drv */
#define DER_TX_POSITIVE_INDICATION   B0
#define DER_TX_NEGATIVE_INDICATION   B1
#define DER_RX_DATA_INDICATION       B2
#define DER_RX_ERROR_INDICATION      B3
#define DER_ERROR_TYPE               (B4|B5|B6|B7)

/* eventi_drv & ERROR_TYPE */
#define DER_OVERFLOW_BUFFER       1
#define DER_UART_ERROR            2
#define DER_CHKS_KO               3
// #define DER_BUFFER_OCCUPATO       4
// #define DER_TRAMA_TROPPO_CORTA    5
// #define DER_RILEVATO_CC           6
// #define DER_FINE_CC               7

extern unsigned char run_gestprot_der;
#define DER_INIT_GEST_PROT  B0
#define DER_TX_CNF_POS      B1
#define DER_TX_CNF_NEG      B2
#define DER_RX_READY        B3
#define DER_RX_ERROR        B4
#define DER_TX_REQUEST      B5


// offset field dati frame application
// #define DER_OFFS_ADDR_DEST_0      0
// #define DER_OFFS_ADDR_DEST_1      1
// #define DER_OFFS_ADDR_DEST_2      2
// #define DER_OFFS_ADDR_DEST_3      3
// #define DER_OFFS_DATI             4
// 
// #define DER_NO_PRIO               0
// #define DER_SI_PRIO               1

