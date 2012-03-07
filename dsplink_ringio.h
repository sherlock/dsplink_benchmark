/*
 * Title: DSPLink RingIO
 * Created By: Shougang SHI
 * Creation Date: 11/01/2011

 * Description: RingIO between ARM and DSP
 *
 * Revision: v0.00 - 11/01/2011 - Initial release
 */

/*  ------------------------ DSP/BIOS Link ----------------------------------*/
#if !defined (DSPLink_RingIO_H)
#define  DSPLink_RingIO_H

/*  ----------------------------------- DSP/BIOS Link                 */
#include <dsplink.h>
/*  ------------------------DSP/BIOS LINK API -------------------------------*/
#include <proc.h>
#include <mpcs.h>
#include <pool.h>
#include <ringio.h>
#include <stdio.h>
#include <string.h>
#include <dsplink_ringio.h>
#include <ring_io_os.h>

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

NORMAL_API DSP_STATUS RING_IO_Create (IN Char8 * dspExecutable, IN Uint8 processorId);
NORMAL_API Void RING_IO_Delete(IN Uint8 processorId);

NORMAL_API DSP_STATUS RING_IO_WriteClient1_Initial();
NORMAL_API Uint32 RING_IO_WriteClient1_Task(const char *buf_in,
		const int len_in, char *buf_out, int len_out);
NORMAL_API DSP_STATUS RING_IO_WriteClient1_Close();

NORMAL_API DSP_STATUS RING_IO_WriteClient2_Initial();
NORMAL_API Uint32 RING_IO_WriteClient2_Task(const char *buf_in,
		const int len_in, char *buf_out, int len_out);
NORMAL_API DSP_STATUS RING_IO_WriteClient2_Close();

/** ============================================================================
 *  @func   RING_IO_0Print
 *
 *  @desc   Print a message without any arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\ring_io_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void
RING_IO_0Print (Char8 * str);

/** ============================================================================
 *  @func   RING_IO_1Print
 *
 *  @desc   Print a message with one arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\message_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *  @arg    arg
 *              Argument to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void
RING_IO_1Print (Char8 * str, Uint32 arg);

/** ============================================================================
 *  @func   RING_IO_Sleep
 *
 *  @desc   Sleeps for the specified number of microseconds.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\ring_io_os.c
 *
 *  @arg    uSec
 *              Microseconds to sleep.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API Void
RING_IO_Sleep (Uint32 uSec);

//void set_dspfilename(char * dspfilename){dspExecutable = dspfilename;}
/*
 void set_RING_IO_BufferSize_Tx1(Uint32 buffersize){
 RING_IO_BufferSize_Tx1 = buffersize;
 RING_IO_BufferSize_Tx1 = DSPLINK_ALIGN (RING_IO_BufferSize_Tx1,
 DSPLINK_BUF_ALIGN);
 }

 void set_RING_IO_BufferSize_Rx1(Uint32 buffersize){
 RING_IO_BufferSize_Rx1 = buffersize;
 RING_IO_BufferSize_Rx1 = DSPLINK_ALIGN (RING_IO_BufferSize_Rx1,
 DSPLINK_BUF_ALIGN);
 }

 void set_RING_IO_BufferSize_Tx2(Uint32 buffersize){
 RING_IO_BufferSize_Tx2 = buffersize;
 RING_IO_BufferSize_Tx2 = DSPLINK_ALIGN (RING_IO_BufferSize_Tx2,
 DSPLINK_BUF_ALIGN);
 }

 void set_RING_IO_BufferSize_Rx2(Uint32 buffersize){
 RING_IO_BufferSize_Rx2 = buffersize;
 RING_IO_BufferSize_Rx2 = DSPLINK_ALIGN (RING_IO_BufferSize_Rx2,
 DSPLINK_BUF_ALIGN);
 }
 */

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif
