
#ifndef __APP_HOST_IF_H__
#define __APP_HOST_IF_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"
/******************************************************************************/
/* define                                                                     */
/******************************************************************************/


typedef enum
{
    HIL_MCU = 0,

    HIL_MAX
} HOST_IF_LIST;

typedef enum
{
    HIL_DEV_ID_EXT = 0,  // RS232
    HIL_DEV_ID_TCU,      // RS232
    HIL_DEV_ID_ACU,      // RS232
    HIL_DEV_ID_LCD,      // RS232
    HIL_DEV_ID_CST,      // RS485
    HIL_DEV_ID_N2B,      // RS485
    HIL_DEV_ID_HDB,      // RS232

    HIL_DEV_ID_MAX
}HOST_IF_ID_LIST;

typedef enum
{
    HIS_PACKET_ID_L = 0,
    HIS_PACKET_ID_H,
    HIS_PACKET_HEADER,
    HIS_PACKET_PAYLOAD,
    HIS_PROCESS_MESSAGE,
    
    HIS_INVALID
} HOST_IF_STATE_LIST;

typedef enum
{
    IBS_WAITING = 0,
    IBS_RECEIVED,
    IBS_ERROR,

    IBS_INVALID
} IN_BUFFER_STATE;

typedef struct 
{
    uint16_t packet_id;
    uint8_t seq;
    uint8_t unit_id;
    uint8_t size;
    uint8_t cs;
}__attribute__((packed)) PACKET_HEADER;

typedef struct
{   
    uint8_t   opcode;
    uint8_t   data[100];

}__attribute__((packed)) PACKET_PAYLOAD;

#define PACKET_HEADER_LEN   (sizeof(PACKET_HEADER))
#define PACKET_OPCODE_LEN   1

typedef struct
{   
    PACKET_HEADER   header;
    PACKET_PAYLOAD  payload;

}__attribute__((packed)) HOST_IF_PACKET;

#define HI_MSG_MAX  (sizeof(HOST_IF_PACKET))

#define HI_MSG_TIMEOUT_T    TICK_MS(200)


// ---------------------------------------------------------------------------
// Host Interface packet identifier
// ---------------------------------------------------------------------------
#define HI_PACKET_ID            0xFEA5

typedef enum
{
    RET_CODE_SUC = 0,
    RET_CODE_ERR_CS,
    RET_CODE_ERR_TYPE,
    RET_CODE_ERR_OPCODE,
    RET_CODE_ERR_PARAM,
    RET_CODE_ERR_UNKNOWN,

} HI_RESULT_CODE;


// ---------------------------------------------------------------------------
// Host Interface OPCODE : MCU
// ---------------------------------------------------------------------------
typedef enum
{
    HIOP_TCU_REP_BOOT = 0,
    HIOP_TCU_RES_BOOT,
    
    HIOP_TCU_REQ_DATA,
    HIOP_TCU_RES_DATA,
    
    HIOP_TCU_REQ_SET_SP,
    HIOP_TCU_RES_SET_SP,

    HIOP_TCU_REQ_SET_ALARM,
    HIOP_TCU_RES_SET_ALARM,
    
    HIOP_TCU_REP_EVENT,
    HIOP_TCU_RES_EVENT,

    HIOP_TCU_INVALID
} HI_OPCODE_TCU;



/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                  */
/******************************************************************************/
void App_HI_Data_Echo(uint8_t en);
void App_HI_Data_Update(uint8_t *data,uint32_t len);
void App_Host_IF_Send_Msg(uint8_t opcode, uint8_t *data, uint32_t len);
void App_Host_IF_Proc(void);
void App_Host_IF_Init(uint8_t hi_ch,uint8_t uart_ch,uint8_t priority,uint32_t baudrate,uint8_t parity);
uint32_t App_Host_IF_build_Packet(uint8_t *data, uint32_t len);

void App_Host_IF_Send_Boot_Msg(void);

void App_Host_Comm_Seq_Set(uint8_t seq);
uint8_t App_Host_Comm_Seq_Get(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __APP_HOST_IF_H__

