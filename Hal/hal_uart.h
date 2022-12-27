
#ifndef __HAL_UART_H__
#define __HAL_UART_H__

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

/******************************************************************************/
/* define                                                                     */
/******************************************************************************/

#define LOG_MSG_NO_MAX      8
#define UART_RX_BUF_SIZE   128

typedef enum
{
    HAL_UART_CH_0 = 0,
    HAL_UART_CH_1,
    HAL_UART_CH_2,
    HAL_UART_CH_3,
    HAL_UART_CH_4,
    HAL_UART_CH_5,
    HAL_UART_CH_6,
    HAL_UART_CH_7,
    HAL_UART_CH_MAX,
}HAL_UART_LIST;

typedef enum 
{
    PARITY_NONE,
    PARITY_ODD,
    PARITY_EVEN
}UART_PARITY_L;

/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

void Hal_Uart0_Int_Handler(void);
void Hal_Uart1_Int_Handler(void);
void Hal_Uart2_Int_Handler(void);
void Hal_Uart3_Int_Handler(void);
void Hal_Uart4_Int_Handler(void);
void Hal_Uart5_Int_Handler(void);
void Hal_Uart6_Int_Handler(void);
void Hal_Uart7_Int_Handler(void);


uint8_t Hal_Uart_Rcv_Len(uint8_t ch);
uint8_t Hal_Uart_Get_Char(uint8_t ch);
uint8_t Hal_Uart_Data_Fill(uint8_t ch);

void Hal_Uart_Send(uint8_t ch,const uint8_t *pui8Buffer, uint32_t ui32Count);
void Hal_Uart_Init(uint8_t ch,uint8_t priority,uint32_t baud,uint8_t parity);



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __HAL_UART_H__
