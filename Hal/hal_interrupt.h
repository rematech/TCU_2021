#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

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


// 001 : 2
// 010 : 4
// 100 : 8
// 101 : A
// 110 : C
// 111 : E
// Priority Level : MSB 3bit

#define HAL_INT_PRIORITY_LVL0       0x20 // HIGH
#define HAL_INT_PRIORITY_LVL1       0x40
#define HAL_INT_PRIORITY_LVL2       0x80
#define HAL_INT_PRIORITY_LVL3       0xA0
#define HAL_INT_PRIORITY_LVL4       0xC0
#define HAL_INT_PRIORITY_LVL5       0xE0 // LOW


//*****************************************************************************
//
// The number of SysTick interrupts to generate per second.
//
//*****************************************************************************



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __HAL_INTERRUPT_H__

