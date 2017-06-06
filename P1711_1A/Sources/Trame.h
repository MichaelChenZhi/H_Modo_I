/*
    $Id: Trame.h,v 1.2 2011/09/01 08:18:59 vrzrrt Exp $
    $Log: Trame.h,v $
    Revision 1.2  2011/09/01 08:18:59  vrzrrt
    Added OKI (Echo Canceller) support.
    Now audio is managed during call.
    Still to be added volume adjustment, mute functions, volume settings and related stuffs.

    Revision 1.1  2011/09/01 07:45:02  chnmhl
    First release

    Revision 1.1  2011/06/13 14:31:10  vrzrrt
    First commit. This module is only for translation purposes and will be never updated.

    Revision 1.6  2009/12/22 11:50:46  vrzrrt
    Implemented NTSC video standard management

    Revision 1.5  2009/12/01 10:08:52  vrzrrt
    TEST bench setting up

    Revision 1.4  2009/11/23 15:55:21  vrzrrt
    WIP test procedures

    Revision 1.3  2009/10/13 15:01:04  vrzrrt
    wip timeraneo

    Revision 1.2  2009/07/22 15:37:49  vrzrrt
    WIP - Management tasti

    Revision 1.1  2009/05/18 10:04:44  vrzrrt
    Primo committ. Versione debug Villata

    Revision 1.1  2008/10/24 09:36:16  vrzrrt
    Creazione modulo. Derivato by staffa ipervoice WIP


*/
//-------------------------------- DEFINES FOR ALL DEVICES ---------------------------
#define			DELAY_EVENT								10 
//#define			TIMER_PRE_OCCUPATO				15 // Base tempi 100ms
//#define			TIMER_ATTESA_PRESENZA_D		50
//#define			TIMER_ATTESA_PRESENZA_C		50
#define			COMMUNICATION_TIMER				615
//#define			TIMER_ATTESA_SGANCIO_FITTIZIO		30 // for stati wait offhook timeranei
//#define			TIMERINO							10 // for time comunicaizone garantito circa nullo

#define			ENDLESS							0xFFFF	// for endless timers

#define			ON									1
#define			OFF									0

//#define			DEFAULT_TIME_ATTESA_SGANCIO_T1		150	// 15"
//#define			DEFAULT_TIME_COMUNICAZIONE_GARANTITO_T2		150	// 15"
//#define			DEFAULT_CANALE_2_PRESENTE			1	// present


// #define			NUMERO_CHIAMATA_ROMBO			7		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_CHIAMATA_QUADRATO	6		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_CHIAMATA_1					1		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_CHIAMATA_2					2		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_CHIAMATA_3					3		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_CHIAMATA_4					4		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_CHIAMATA_5					5		// codici usati nella D_CALL_BUTTON
// 
// #define			NUMERO_SPECIALE_QUADRATO	6		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_SPECIALE_1					1		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_SPECIALE_2					2		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_SPECIALE_3					3		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_SPECIALE_4					4		// codici usati nella D_CALL_BUTTON
// #define			NUMERO_SPECIALE_5					5		// codici usati nella D_CALL_BUTTON

#define			SOURCE_MASK				      0x0F  //
#define			A2_BIT_USER_MASK				0x60  // utente netto
#define			A3_BIT_USER_MASK				0xE0  // utente completo
#define			A5_BIT_USER_MASK				0x1F  // internal completo
#define			A4_BIT_INTERNAL_MASK		0x0F  // internal netto
#define			NTSC_PAL_MASK           0x80  //
#define     DEC_VILLA_CODE          0x81  //
#define			INTERNAL_BROADCAST			0x1F  //
#define			USER_BROADCAST					0xE0  //
#define			DENTIST					    		0x80  //
#define			DOOR_STATE							0x80  //

#define			VIDEO_OR_AUDIO_COMM_TYPE		0x10
//#define			TIPO_CANALE			0x20
//  source: BIT3,2,1,0
#define			FROM_MAIN_CM				     0	// 0000 --> from main
#define			FROM_SECONDARY_CM				 1	// 0001	--> from secondary
#define			FROM_SWITCHBOARD				 2	// 0010 --> from switchboard
#define			FROM_EXTERNAL_AS				 3	// 0011 --> from AS external
#define			FROM_INTERNAL_AS				 4	// 0100 --> from AS internal (I/F intercom)
#define			FROM_IP_PHONE				  	 5	// 0101 --> by TEL IP
#define			FROM_THE_INQUIRED_ANSWERING_MACHINE		 6	// 0110 --> following a interrogazione SVC
#define			FROM_S_I_CM_IP_AUDIO_MONODIR	         7	// 0111 --> following a self_insertion on PE IP with audio monodirectional
#define			FROM_S_I_CM_IP_AUDIO_BIDIR          	 8	// 1000 --> following a self_insertion on PE IP with audio bidirectional
#define			FROM_S_I_CM_VILLA_AUDIO_MONODIR	       9	// 1001 --> following a self_insertion on PE Villa with audio monodirectional
#define			FROM_S_I_CM_VILLA_AUDIO_BIDIR          10	// 1010 --> following a self_insertion on PE Villa with audio bidirectional
#define			FROM_SWITCHBOARD_FOR_ROOM_MONITOR	  	 11	// 1011 --> by switchboard for room monitor
#define			FROM_VILLA_CM		   	              		 12	// 1100 --> by PE villa
#define			FROM_S_I_IP_CM_NO_AUDIO	 			         13	// 1101 --> following a self_insertion on PE IP without audio
#define			FROM_S_I_VILLA_CM_NO_AUDIO	 		       14	// 111  --> following a self_insertion on PE Villa without audio

#define			D_OPC 				0
#define			D_ADDRESS			1
#define			D_DATA_1 			2
#define			D_DATA_2 			3
#define			D_DATA_3 			4 
#define			D_DATA_4 			5 
#define			D_DATA_5 			6


#define			C_OPC 				0
#define			C_SOURCE_1			1
#define			C_SOURCE_2			2
#define			C_SOURCE_3 			3
#define			C_SOURCE_4			4
#define			C_DEST_1 			5
#define			C_DEST_2 			6
#define			C_DEST_3 			7 
#define			C_DEST_4 			8 
#define			C_DATA_1 			9
#define			C_DATA_2 			10
#define			C_DATA_3 			11
#define			C_DATA_4 			12
#define			C_DATA_5 			13
#define			C_DATA_6 			14
#define			C_DATA_7 			15

// #define			IND_GW_1			0x80
// #define			IND_GW_2			0x30
// #define			IND_GW_3			0x30
// #define			IND_GW_4			0x30


#define			ACK_WAITING_TIMEOUT  5		// = 5 seconds
//#define			ACK_WAITING_TIMEOUT  3		// = 3 seconds 
#define			RETRANSMISSION_NUMBER	3

#define			TIME_BETWEEN_POLLING	1200

#define			NO_TONE      		0x00         // stop sound
#define			TIPO_TONE_1			0x01         // courtesy
#define			TIPO_TONE_2			0x02         // dissuasion


// #define			TIME_OCCUPATO_FOLIO		20	   // Time in seconds
// #define			MENU_RESPONSE_KO			0x00
// #define			MENU_RESPONSE_OK			0x80



// #define			IF_ALLARMI					0x10	// address interfaccia allarmi

// #define			RESET_INCONDIZIONATO		0x00
// #define			PRE_RESET_SENZA_TIMEOUT		0xFF


// ---------------------------------- OPCODES TRAME DERIVAZIONE --------------------------
#define	D_CALL					0x33	//	AS          			data1	        	                       	DEC
#define	D_PRESENCE					0x34	//	AS						data1                                    	AS
#define	D_HOOK_OFF	 				0x35	//	AS				        data1  										AS
#define	D_FREE_LINE				0x36	//	AS          			data1										AS
#define	D_FREE_LINE				0x36	//	AS          			data1										DEC
#define D_DOOR_OPENER_1				0x38	//	AS						     			       						AS
#define D_DOOR_OPENER_2				0x39	//	AS						     			       						AS
#define D_DOOR_STATE              	0x9C	// 	AS						data1										DEC
#define D_POLLING					0xC3	//	AS 																	DEC
#define D_ACK_POLLING				0xC4	//	AS						data1 										AS
#define D_SPECIAL_FUNCTION        	0x9A	// 	AS						data1										AS
#define D_PRESENCE_ABSENCE_BUTTON	0x65	//	AS						data1										AS
#define D_ACK_PRESENCE_ABSENCE_BUTTON	0x66//	AS						data1  										DEC
#define D_ALARM                  	0xAA	//  ALL						data1 data2 data3							ALL
#define D_ACK_ALARM              	0xAB	//  ALL                                                                 DEC
#define D_PANIC                   	0xA6	//  AS			            data1                                       AS
#define D_ACK_PANIC               	0xA7	//  AS																	DEC
#define D_TONE						0x53	//	AS                      data1										DEC
#define D_BOUNCE					0x3A	//	AS          			     	        	                    	AS
#define D_CYCLE						0x55	//	AS          			     	        	                    	AS
#define D_CYCLE_VILLA				0x56	//	AS          			     	        	                    	AS
#define D_SVC_NEXT					0x59	//	AS          			     	        	                    	AS
#define D_SVC_DELETE				0x5A	//	AS          			     	        	                    	AS
#define D_SVC_NUMBER				0x5B	//	AS          			data1  	        	                          		DEC
#define D_MESSAGES_PRESENT			0xCA	//	AS          			     	        	                    	DEC
#define D_MESSAGES_ABSENT			0xCB	//	AS          			     	        	                    	DEC
#define D_CALL_BUTTON        	0x93	// 	AS						data1										AS
#define D_OUT_CALL_FUNCTION   0x95	// 	AS						data1 data2 data3 data4						AS (Folio)
#define D_IN_CALL_FUNCTION  0x96	// 	AS						data1 										AS (Folio)
#define D_DUMMY						0xC9	// 	src																	src
#define D_MENU						0x5C	// 	AS																	DEC
#define D_MENU_RESPONSE				0x5D	// 	DEC						data1										AS
#define	D_AUDIO						0x3C	//	gw          dec			data1 data2 
#define	D_MENU					    0x5C	//	AS          				              	                          		AS
#define	D_MENU_RESPONSE			    0x5D    //	AS          			data1              	                          		DEC
#define D_TEST_PHASE_ACTIVATION             	0xFB    //  0xFF A5 5A				    0xFF A5 5A       
//0x6C
#define D_FLOOR_CALL        	0xA5	//  
#define D_FW_REQ                     0xBB    //  AS 
#define D_FW_RESPONSE                0xDD    //  AS          data1
//0xC5
//0xC6
//#define D_OKI_WR                    0xE0    // DB OKI addr    OKI data  
//#define D_OKI_RD_REQ                0xE1    // DB OKI addr      
//#define D_OKI_RD                    0xE2    // DB OKI addr    OKI data  




// ---------------------------------- OPCODES TRAME COLONNA --------------------------
// #define	C_CHIAMATA					0x33	//	gw			dec       	data1 data2
// #define	C_CHIAMATA					0x33	//	dec			dec       	data1 data2
// #define	C_PRESENZA					0x34	//	dec			gw			data1
// #define	C_PRESENZA					0x34	//	dec			dec			data1
// #define	C_SGANCIO	 				0X35	//	dec			gw 			data1
// #define	C_SGANCIO	 				0X35	//	dec			dec			data1
// #define	C_LINEA_LIBERA				0X36	//	dec			gw 			data1
// #define	C_LINEA_LIBERA				0X36	//	dec			dec			data1
// #define	C_LINEA_LIBERA				0X36	//	gw			dec			data1
// #define C_APRIPORTA_1				0X38	//	dec			gw          data1
// #define C_APRIPORTA_2				0X39	//	dec			gw          data1
// #define C_STATO_PORTA              	0x9C	// 	gw			dec			data1
// #define C_POLLING					0xC3	//	gw			dec         data1
// #define C_ACK_POLLING				0xC4	//	dec			gw          data1 data2
// #define C_REGISTER					0xC5	//	dec			gw
// #define C_FUNZIONE_SPECIALE        	0x9A	// 	dec			gw			data1
// #define C_BUTTON_PRESENZA_ABSENCE	0x65	//	dec			gw			data1
// #define C_ACK_BUTTON_PRESENZA_ABSENCE	0x66//	gw			dec         data1
// #define C_ALARM                  	0xAA	//  dec			gw			data1 data2 data3
// #define C_ACK_ALARM              	0xAB	//  gw          dec
// #define C_INT_ASCENSORE          	0xCC	//  gw          asc			data1 data2 data3
// #define C_PANIC                   	0xA6	//  dec			gw          data1
// #define C_ACK_PANIC               	0xA7	//  gw          dec
// #define	C_CONFIGURAZIONI    		0x6A	//	gw  		dec      	data1 data2
// #define	C_RESET_ALARM				0xA3	//	dec         gw 			data1
// #define	C_RESET_ALARM				0xA3	//	gw          dec			data1
// #define	C_ACK_RESET_ALARM			0xA4	//	dec         gw
// #define	C_ACK_RESET_ALARM			0xA4	//	gw          dec
// #define C_CYCLE						0x55	//	dec         gw			data1 data2
// #define C_SVC_NEXT					0x59	//	dec         gw			data1
// #define C_SVC_DELETE				0x5A	//	dec         gw			data1
// #define C_SVC_NUMBER				0x5B	//	dec         gw			data1 data2
// #define C_MESSAGGI_PRESENTI			0xCA	//  dec         gw
// #define C_MESSAGGI_ASSENTI			0xCB	//  dec         gw
// #define	C_RICHIESTA_CENTRALINO_TEL	0x69	//  dec 		gw          data1 data2
// #define C_CHIAMATA_AL_PIANO        	0xA5	// 
// #define C_DUMMY						0xC9	// 	src			null
// #define	C_FONIA						0x3C	//	gw          dec			data1 data2 data3 data4 dato5 dato6
// #define	C_FONIA						0x3C	//	dec         dec			data1 data2 data3 data4 dato5 dato6
// #define	C_MENU					    0x5C	//	dec			gw




