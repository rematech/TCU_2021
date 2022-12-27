#ifndef __DEFINES_H__
#define __DEFINES_H__

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

#define UNIT_MHZ            1000000U
#define UNIT_KHZ            1000U

#ifndef FALSE
#define FALSE	0
#define TRUE	1
#endif

#ifndef ON
#define	ON	    1
#define	OFF	    0
#endif

#ifndef HI
#define	HI	    1
#define	LO	    0
#endif


#ifndef _BIT
#define _BIT(x)			    (1 << (x))
#endif

#ifndef _SBI // Set Bit 
#define _SBI(PORTX , BitX)   (PORTX |=  (BitX))
#endif

#ifndef _CBI // Clear Bit 
#define _CBI(PORTX , BitX)   (PORTX &= ~(BitX))
#endif

#ifndef _RBI // Read Bit
#define _RBI(PORTX , BitX)   (((PORTX & BitX) == BitX) ? 1:0)
#endif

///////////////////////////////////////////////////////////////////////
// ASCII
#define	BEL		    0x07
#define	BS		    0x08
#define	LF		    0x0a
#define	CR		    0x0d
#define	ESC		    0x1b
///////////////////////////////////////////////////////////////////////



/******************************************************************************/
/* public variables                                                           */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __DEFINES_H__


