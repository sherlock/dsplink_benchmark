/*   	
 * Title: DSPLink RingIO
 * Created By: Shougang SHI
 * Creation Date: 11/01/2011

 * Description: RingIO between ARM and DSP
 * 	
 * Revision: v0.00 - 11/01/2011 - Initial release
 */
/*  ------------------------ DSP/BIOS Link ----------------------------------*/
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

	/*  ============================================================================
	 *  @const   NUM_ARGS
	 *
	 *  @desc   1: enalbe the debug info; 0: close the debug info
	 *  ============================================================================
	 */

#define DEBUG_ENABLED 1

	/*  ============================================================================
	 *  @const   NUM_ARGS
	 *
	 *  @desc   Number of arguments specified to the DSP application.
	 *  ============================================================================
	 */
#define NUM_ARGS              4u

	/** ============================================================================
	 *  @name   SAMPLE_POOL_ID
	 *
	 *  @desc   ID of the POOL used for the sample.
	 *  ============================================================================
	 */
#define SAMPLE_POOL_ID        0u

	/** ============================================================================
	 *  @name   NUM_BUF_SIZES
	 *
	 *  @desc   Number of buffer pools to be configured for the allocator.
	 *  ============================================================================
	 */
#define NUM_BUF_SIZES         7u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL0
	 *
	 *  @desc   Number of buffers in first buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL0         1u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL1
	 *
	 *  @desc   Number of buffers in second buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL1         1u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL2
	 *
	 *  @desc   Number of buffers in third buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL2         1u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL3
	 *
	 *  @desc   Number of buffers in fourth buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL3         1u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL4
	 *
	 *  @desc   Number of buffers in fifth buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL4           4u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL5
	 *
	 *  @desc   Number of buffers in fifth buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL5           4u

	/** ============================================================================
	 *  @const  NUM_BUF_POOL6
	 *
	 *  @desc   Number of buffers in fifth buffer pool.
	 *  ============================================================================
	 */
#define NUM_BUF_POOL6           4u

	/** ============================================================================
	 *  @name   RING_IO_ATTR_BUF_SIZE
	 *
	 *  @desc   Size of the RingIO Attribute buffer (in bytes).
	 *  ============================================================================
	 */
#define RING_IO_ATTR_BUF_SIZE   2048u

	/** ============================================================================
	 *  @const  RING_IO_VATTR_SIZE
	 *
	 *  @desc   size of the RingIO varibale attribute buffer.
	 *  ============================================================================
	 */
#define RING_IO_VATTR_SIZE      1u

	/*  ============================================================================
	 *  @const   RINGIO_DATA_START
	 *
	 *  @desc    Fixed attribute type indicates  start of the data in the RingIO
	 *  ============================================================================
	 */
#define RINGIO_DATA_START       1u

	/*  ============================================================================
	 *  @const   NOTIFY_DATA_START
	 *
	 *  @desc    Notification message  to  DSP.Indicates data transfer start
	 *  ============================================================================
	 */
#define NOTIFY_DATA_START       2u

	/*  ============================================================================
	 *  @const   RINGIO_DATA_END
	 *
	 *  @desc    Fixed attribute type indicates  start of the data in the RingIO
	 *  ============================================================================
	 */
#define RINGIO_DATA_END         3u

	/*  ============================================================================
	 *  @const   NOTIFY_DATA_START
	 *
	 *  @desc     Notification message  to  DSP.Indicates data transfer stop.
	 *  ============================================================================
	 */
#define NOTIFY_DATA_END         4u

	/*  ============================================================================
	 *  @const   RINGIO_DSP_END
	 *
	 *  @desc     Fixed attribute type indicates  end of the dsp
	 *  ============================================================================
	 */
#define RINGIO_DSP_END         5u

	/*  ============================================================================
	 *  @const   NOTIFY_DSP_END
	 *
	 *  @desc     Notification message  to  DSP.Indicates DSP end
	 *  ============================================================================
	 */
#define NOTIFY_DSP_END         6u

	/*  ============================================================================
	 *  @const   RING_IO_WRITER_BUF_SIZE
	 *
	 *  @desc    Writer task buffer acquire size.
	 *  ============================================================================
	 */
#define RING_IO_WRITER_BUF_SIZE    1024u
	/** ============================================================================
	 *  @const  fReaderStart
	 *
	 *  @desc   boolean variable to  indicate Gpp reader can start reading.
	 *  ============================================================================
	 */
	STATIC Uint32 fReaderStart1 = FALSE;
	STATIC Uint32 fReaderStart2 = FALSE;

	/** ============================================================================
	 *  @const  fReaderEnd
	 *
	 *  @desc   boolean variable to  indicate Gpp reader should stop reading.
	 *  ============================================================================
	 */
	STATIC Uint32 fReaderEnd1 = FALSE;
	STATIC Uint32 fReaderEnd2 = FALSE;

	/*  ============================================================================
	 *  @name   RING_IO_BufferSize
	 *
	 *  @desc   Size of the RingIO Data Buffer to be used for data transfer.
	 *  ============================================================================
	 */
	STATIC Uint32 RING_IO_BufferSize_Tx1;
	STATIC Uint32 RING_IO_BufferSize_Rx1;
	STATIC Uint32 RING_IO_BufferSize_Tx2;
	STATIC Uint32 RING_IO_BufferSize_Rx2;
	STATIC Uint32 RING_IO_BytesToTransfer1;
	STATIC Uint32 RING_IO_BytesToTransfer2;

	/** ============================================================================
	 *  @const  RingIOWriterName
	 *
	 *  @desc   Name of the RingIO used by the application in writer mode.
	 *  ============================================================================
	 */
	STATIC Char8 RingIOWriterName1[RINGIO_NAME_MAX_LEN] = "RINGIO1";
	STATIC Char8 RingIOWriterName2[RINGIO_NAME_MAX_LEN] = "RINGIO3";
//Char8 RingIOWriterName1[RINGIO_NAME_MAX_LEN];
//Char8 RingIOWriterName2[RINGIO_NAME_MAX_LEN];
	/** ============================================================================
	 *  @name   RingIOWriterHandle
	 *
	 *  @desc   Handle to the RingIO used by the application in writer mode.
	 *  ============================================================================
	 */
	STATIC RingIO_Handle RingIOWriterHandle1 = NULL;
	STATIC RingIO_Handle RingIOWriterHandle2 = NULL;
//RingIO_Handle RingIOWriterHandle1;
//RingIO_Handle RingIOWriterHandle2;

	/** ============================================================================
	 *  @const  RingIOReaderName
	 *
	 *  @desc   Name of the RingIO used by the application in reader mode.
	 *  ============================================================================
	 */
	STATIC Char8 RingIOReaderName1[RINGIO_NAME_MAX_LEN] = "RINGIO2";
	STATIC Char8 RingIOReaderName2[RINGIO_NAME_MAX_LEN] = "RINGIO4";
//Char8 RingIOReaderName1[RINGIO_NAME_MAX_LEN];
//Char8 RingIOReaderName2[RINGIO_NAME_MAX_LEN];

	/** ============================================================================
	 *  @name   RingIOReaderHandle
	 *
	 *  @desc   Handle to the RingIO used by the application in reader mode.
	 *  ============================================================================
	 */
	STATIC RingIO_Handle RingIOReaderHandle1 = NULL;
	STATIC RingIO_Handle RingIOReaderHandle2 = NULL;
//RingIO_Handle RingIOReaderHandle1;
//RingIO_Handle RingIOReaderHandle2;

	Pvoid semPtrWriter1 = NULL;
	Pvoid semPtrReader1 = NULL;
//Pvoid semPtrWriter1;
//Pvoid semPtrReader1;

	Pvoid semPtrWriter2 = NULL;
	Pvoid semPtrReader2 = NULL;
//Pvoid semPtrWriter2;
//Pvoid semPtrReader2;

//	char dspfilename[]="ringio_new.out";

	STATIC
	NORMAL_API
	Void RING_IO_Writer_Notify1(IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg);

	STATIC
	NORMAL_API
	Void RING_IO_Writer_Notify2(IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg);

	STATIC
	NORMAL_API
	Void RING_IO_Reader_Notify1 (IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg);

	STATIC
	NORMAL_API
	Void RING_IO_Reader_Notify2 (IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg);

	/** ============================================================================
	 *  @func   RING_IO_Create
	 *
	 *  @desc   This function allocates and initializes resources used by
	 *          this application.
	 *
	 *  @modif  RING_IO_InpBufs , RING_IO_OutBufs
	 *  ============================================================================
	 */

	NORMAL_API DSP_STATUS RING_IO_Create (IN Char8 * dspExecutable,IN Uint8 processorId)
	{
		DSP_STATUS status = DSP_SOK;
		Uint32 numArgs = NUM_ARGS;
		Uint32 numBufs [NUM_BUF_SIZES] = {NUM_BUF_POOL0,
			NUM_BUF_POOL1,
			NUM_BUF_POOL2,
			NUM_BUF_POOL3,
			NUM_BUF_POOL4,
			NUM_BUF_POOL5,
			NUM_BUF_POOL6
		};

		Uint32 size [NUM_BUF_SIZES];
		SMAPOOL_Attrs poolAttrs;
		Char8 * args [NUM_ARGS];
		Char8 tempCmdString [NUM_ARGS][11];
		RingIO_Attrs ringIoAttrs;

		RING_IO_BufferSize_Tx1 = 1024;
		RING_IO_BufferSize_Rx1 = 4096;
		RING_IO_BufferSize_Tx2 = 2048;
		RING_IO_BufferSize_Rx2 = 2048;

		RING_IO_BytesToTransfer1 = 0;
		RING_IO_BytesToTransfer2 = 0;

		RING_IO_BufferSize_Tx1 = DSPLINK_ALIGN (RING_IO_BufferSize_Tx1,
				DSPLINK_BUF_ALIGN);
		RING_IO_BufferSize_Rx1 = DSPLINK_ALIGN (RING_IO_BufferSize_Rx1,
				DSPLINK_BUF_ALIGN);
		RING_IO_BufferSize_Tx2 = DSPLINK_ALIGN (RING_IO_BufferSize_Tx2,
				DSPLINK_BUF_ALIGN);
		RING_IO_BufferSize_Rx2 = DSPLINK_ALIGN (RING_IO_BufferSize_Rx2,
				DSPLINK_BUF_ALIGN);
		RING_IO_BytesToTransfer1 = DSPLINK_ALIGN (RING_IO_BytesToTransfer1,
				DSPLINK_BUF_ALIGN);
		RING_IO_BytesToTransfer2 = DSPLINK_ALIGN (RING_IO_BytesToTransfer2,
				DSPLINK_BUF_ALIGN);

#if DEBUG_ENABLED
		RING_IO_0Print ("Entered RING_IO_Create ()\n");
#endif

		/*
		 *  Create and initialize the proc object.
		 */
		status = PROC_setup (NULL);

		/*
		 *  Attach the Dsp with which the transfers have to be done.
		 */
		if (DSP_SUCCEEDED (status)) {
			status = PROC_attach (processorId, NULL);
			if (DSP_FAILED (status)) {
				RING_IO_1Print ("PROC_attach () failed. Status = [0x%x]\n",
						status);
			}
		}
		else {
			RING_IO_1Print ("PROC_setup () failed. Status = [0x%x]\n", status);
		}

		/*
		 *  Open the pool.
		 */
		if (DSP_SUCCEEDED (status)) {

			size [0] = RING_IO_BufferSize_Tx1;
			size [1] = RING_IO_BufferSize_Rx1;
			size [2] = RING_IO_BufferSize_Tx2;
			size [3] = RING_IO_BufferSize_Rx2;

			size [4] = RING_IO_ATTR_BUF_SIZE;
			size [5] = sizeof (RingIO_ControlStruct);
			size [6] = sizeof (MPCS_ShObj);
			poolAttrs.bufSizes = (Uint32 *) &size;
			poolAttrs.numBuffers = (Uint32 *) &numBufs;
			poolAttrs.numBufPools = NUM_BUF_SIZES;
			poolAttrs.exactMatchReq = TRUE;
			status = POOL_open (POOL_makePoolId(processorId, SAMPLE_POOL_ID), &poolAttrs);
			if (DSP_FAILED (status)) {
				RING_IO_1Print ("POOL_open () failed. Status = [0x%x]\n",
						status);
			}
		}

		/*
		 *  Load the executable on the DSP.
		 */
		if (DSP_SUCCEEDED (status)) {

			/* RingIO send data buffer size*/
			RING_IO_IntToString (size [1], tempCmdString [0]);
			args [0] = tempCmdString [0];

			/* RingIO recv data buffer size*/
			RING_IO_IntToString (size [3], tempCmdString [1]);
			args [1] = tempCmdString [1];

			/* RingIO attr buffer size */
			RING_IO_IntToString (RING_IO_ATTR_BUF_SIZE, tempCmdString [2]);
			args [2] = tempCmdString [2];
			/* RingIO foot buffer size */
			RING_IO_IntToString (0, tempCmdString [3]);
			args [3] = tempCmdString [3];

			{
				status = PROC_load (processorId, dspExecutable, numArgs, args);
			}

			if (DSP_FAILED (status)) {
				RING_IO_1Print ("PROC_load () failed. Status = [0x%x]\n", status);
			}
		}

		//Create the write RingIO for sending
		if (DSP_SUCCEEDED (status)) {
			ringIoAttrs.transportType = RINGIO_TRANSPORT_GPP_DSP;
			ringIoAttrs.ctrlPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.dataPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.attrPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.lockPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.dataBufSize = size [0];
			ringIoAttrs.footBufSize = 0;
			ringIoAttrs.attrBufSize = RING_IO_ATTR_BUF_SIZE;
#if defined (DSPLINK_LEGACY_SUPPORT)
			status = RingIO_create (RingIOWriterName1, &ringIoAttrs);
#else
			status = RingIO_create (processorId, RingIOWriterName1, &ringIoAttrs);
#endif /* if defined (DSPLINK_LEGACY_SUPPORT) */
			if (DSP_FAILED (status)) {
				RING_IO_1Print ("RingIO_create () failed. Status = [0x%x]\n",
						status);
			}
		}

		//Create the write RingIO for receiving
		if (DSP_SUCCEEDED (status)) {
			ringIoAttrs.transportType = RINGIO_TRANSPORT_GPP_DSP;
			ringIoAttrs.ctrlPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.dataPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.attrPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.lockPoolId = POOL_makePoolId(processorId, SAMPLE_POOL_ID);
			ringIoAttrs.dataBufSize = size [2];
			ringIoAttrs.footBufSize = 0;
			ringIoAttrs.attrBufSize = RING_IO_ATTR_BUF_SIZE;
#if defined (DSPLINK_LEGACY_SUPPORT)
			status = RingIO_create (RingIOWriterName2, &ringIoAttrs);
#else
			status = RingIO_create (processorId, RingIOWriterName2, &ringIoAttrs);
#endif /* if defined (DSPLINK_LEGACY_SUPPORT) */
			if (DSP_FAILED (status)) {
				RING_IO_1Print ("RingIO_create () failed. Status = [0x%x]\n",
						status);
			}
		}

		/*
		 *  Start execution on DSP.
		 */
		if (DSP_SUCCEEDED (status)) {
			status = PROC_start (processorId);
			if (DSP_FAILED (status)) {
				RING_IO_1Print (" PROC_start () failed. Status = [0x%x]\n",
						status);
			}
		}
#if DEBUG_ENABLED
		RING_IO_0Print ("Leaving RING_IO_Create ()\n");
#endif

		return (status);

	}
	/** ============================================================================
	 *  @func   RING_IO_WriteClient1_Initial
	 *
	 *  @desc   This function implements the writer task  for this sample
	 *          application.
	 *          The  writer task has the following flow:
	 *          1.  This task (GPP RingIO writer) sets the notifier for the RINGIO1
	 *              writer with the specific  watermark  value of the buffer size
	 *              used for data transfer. The notifier function post the semaphore
	 *              passed to it, resulting in unblocking the application which
	 *              would be waiting on it.
	 *              */
	NORMAL_API DSP_STATUS RING_IO_WriteClient1_Initial() {

		DSP_STATUS status = DSP_SOK;
		//DSP_STATUS relStatus = DSP_SOK;
		//DSP_STATUS tmpStatus = DSP_SOK;
		//RingIO_BufPtr bufPtr = NULL;
		//Pvoid semPtrWriter = NULL;

		Uint16 type;
		//Uint32 acqSize;

		//Pvoid semPtrReader = NULL;
		//Uint32 param;
		//Uint32 vAttrSize = 0;
		//Uint32 rcvSize = RING_IO_BufferSize1;
		//Uint32 totalRcvbytes = 0;

		DSP_STATUS attrStatus = DSP_SOK;

		////////////////////////////////////////////////////////////////////////////////
		// initial the write task
		////////////////////////////////////////////////////////////////////////////////
#if DEBUG_ENABLED
		RING_IO_0Print("Entered RING_IO_WriterClient1 ()\n");
#endif
		/*
		 *  Open the RingIO to be used with GPP as the writer.
		 *
		 *  Value of the flags indicates:
		 *     No cache coherence for: Control structure
		 *                             Data buffer
		 *                             Attribute buffer
		 *     Exact size requirement.
		 */
		RingIOWriterHandle1 = RingIO_open(RingIOWriterName1, RINGIO_MODE_WRITE,
				(Uint32)(RINGIO_NEED_EXACT_SIZE));
		if (RingIOWriterHandle1 == NULL) {
			status = RINGIO_EFAILURE;
			RING_IO_1Print("RingIO_open1 () Writer failed. Status = [0x%x]\n",
					status);
		}

		if (DSP_SUCCEEDED(status)) {
			/* Create the semaphore to be used for notification */
			status = RING_IO_CreateSem(&semPtrWriter1);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RING_IO_CreateSem1 () Writer SEM failed "
						"Status = [0x%x]\n", status);
			}
		}

		if (DSP_SUCCEEDED(status)) {
			/*
			 *  Set the notification for Writer.
			 */
			do {
				/* Set the notifier for writer for RingIO created by the GPP. */
				status = RingIO_setNotifier(RingIOWriterHandle1,
						RINGIO_NOTIFICATION_ONCE, RING_IO_WRITER_BUF_SIZE,
						&RING_IO_Writer_Notify1,
						(RingIO_NotifyParam) semPtrWriter1);
				if (status != RINGIO_SUCCESS) {
					RING_IO_Sleep(10);
				}
			}while (DSP_FAILED(status));

		}
		////////////////////////////////////////////////////////////////////////////////
		//end  initial the write task
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		// initial the read  task
		////////////////////////////////////////////////////////////////////////////////
#if DEBUG_ENABLED
		RING_IO_0Print("Entered RING_IO_ReaderClient1 ()\n");
#endif

		/*
		 *  Open the RingIO to be used with GPP as the reader.
		 *  Value of the flags indicates:
		 *     No cache coherence for: Control structure
		 *                             Data buffer
		 *                             Attribute buffer
		 *     Exact size requirement false.
		 */
		do {
			RingIOReaderHandle1 = RingIO_open(RingIOReaderName1, RINGIO_MODE_READ,
					0);

			//	RING_IO_0Print (" RingIO_open (RingIOReaderName1()ing \n") ;

		}while (RingIOReaderHandle1 == NULL);

		/* Create the semaphore to be used for notification */
		status = RING_IO_CreateSem(&semPtrReader1);
		if (DSP_FAILED(status)) {
			RING_IO_1Print("RING_IO_CreateSem1 () Reader SEM failed "
					"Status = [0x%x]\n", status);
		}
		if (DSP_SUCCEEDED(status)) {
			do {

				/* Set the notifier for reader for RingIO created by the DSP. */
				/*
				 * Set water mark to zero. and try to acquire the full buffer
				 * and  read what ever is available.
				 */
				status = RingIO_setNotifier(RingIOReaderHandle1,
						RINGIO_NOTIFICATION_ONCE, 0, &RING_IO_Reader_Notify1,
						(RingIO_NotifyParam) semPtrReader1);
				if (DSP_FAILED(status)) {
					/*RingIO_setNotifier () Reader failed  */
					RING_IO_Sleep(10);
				}
			}while (DSP_FAILED(status));
		}

		//RING_IO_0Print (" RingIO_setNotifier1 Reader SEM  \n");
#if DEBUG_ENABLED
		RING_IO_0Print("End initial the read  task1 \n");
#endif

		return status;
		////////////////////////////////////////////////////////////////////////////////
		//end initial the read  task
		////////////////////////////////////////////////////////////////////////////////

	}

	NORMAL_API Uint32 RING_IO_WriteClient1_Task(const char *buf_in,
			const int len_in, char *buf_out, int len_out) {
		Uint16 type;
		DSP_STATUS status = DSP_SOK;
		DSP_STATUS relStatus = DSP_SOK;
		//DSP_STATUS tmpStatus = DSP_SOK;
		DSP_STATUS attrStatus = DSP_SOK;
		RingIO_BufPtr bufPtr = NULL;
		Uint32 bytesTransfered = 0;
		Uint32 attrs[RING_IO_VATTR_SIZE];
		Uint8 *ptr8;
		Uint16 i = 0;
		Uint8 exitFlag = FALSE;

		Uint32 acqSize;

		//Pvoid semPtrReader = NULL;
		Uint32 param;
		Uint32 vAttrSize = 0;
		Uint32 rcvSize = 0;
		Uint32 totalRcvbytes = 0;

		RING_IO_BytesToTransfer1 = len_in;
		rcvSize = len_out;

#if DEBUG_ENABLED
		RING_IO_0Print("start send \n");
#endif
		////////////////////////////////////////////////////////////////////////////////
		//the execute of write task
		////////////////////////////////////////////////////////////////////////////////

		if (DSP_SUCCEEDED(status)) {
			/* Send data transfer attribute (Fixed attribute) to DSP*/
			type = (Uint16) RINGIO_DATA_START;
			status = RingIO_setAttribute(RingIOWriterHandle1, 0, type, 0);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RingIO_setAttribute1 failed to set the  "
						"RINGIO_DATA_START. Status = [0x%x]\n", status);
			}
		}
#if DEBUG_ENABLED
		RING_IO_0Print("RingIO_setAttribute1 () Writer SEM   \n");
#endif

		if (DSP_SUCCEEDED(status)) {
			/* Send Notification  to  the reader (DSP)*/
#if DEBUG_ENABLED
			RING_IO_0Print("GPP-->DSP1:Sent Data Transfer Start "
					"Attribute\n");
#endif
			do {
				status = RingIO_sendNotify(RingIOWriterHandle1,
						(RingIO_NotifyMsg) NOTIFY_DATA_START);
				if (DSP_FAILED(status)) {
					/* RingIO_sendNotify failed to send notification */
					RING_IO_Sleep(10);
				} else {
#if DEBUG_ENABLED
					RING_IO_0Print("GPP-->DSP1:Sent Data Transfer Start "
							"Notification \n");
#endif
				}
			}while (status != RINGIO_SUCCESS);
		}

		if (DSP_SUCCEEDED(status)) {
#if DEBUG_ENABLED
			RING_IO_1Print("Bytes to transfer :%ld \n", RING_IO_BytesToTransfer1);
			RING_IO_1Print("Data buffer size  :%ld \n", RING_IO_BufferSize_Tx1);
#endif

			while ((RING_IO_BytesToTransfer1 == 0)
					|| (bytesTransfered < RING_IO_BytesToTransfer1)) {

				/* Update the attrs to send variable attribute to DSP*/
				attrs[0] = RING_IO_WRITER_BUF_SIZE;
				//attrs [0] = RING_IO_BytesToTransfer1;

				/* ----------------------------------------------------------------
				 * Send to DSP.
				 * ----------------------------------------------------------------
				 */
				/* Set the scaling factor variable attribute*/
				status = RingIO_setvAttribute(RingIOWriterHandle1, 0, /* at the beginning */
						0, /* No type */
						0, attrs, sizeof(attrs));
				if (DSP_FAILED(status)) {
					/* RingIO_setvAttribute failed */
					RING_IO_Sleep(10);
				} else {
					/* Acquire writer bufs and initialize and release them. */
					acqSize = RING_IO_WRITER_BUF_SIZE;
					//acqSize = RING_IO_BytesToTransfer1;
					status = RingIO_acquire(RingIOWriterHandle1, &bufPtr, &acqSize);

					/* If acquire success . Write to  ring bufer and then release
					 * the acquired.
					 */
					if ((DSP_SUCCEEDED(status)) && (acqSize > 0)) {
						//copy the data to RIngIO buf
						ptr8 = (Uint8 *)(bufPtr);
						for (i = 0;
								((i < acqSize)&& ((i + bytesTransfered) < RING_IO_BytesToTransfer1)); i++) {
							ptr8[i] = buf_in[(bytesTransfered + i)];
						}

						//RING_IO_InitBuffer (bufPtr, acqSize);

						//debug
#if DEBUG_ENABLED

						for (i = 0; i < 5; i++) {
							RING_IO_1Print("    Send [0x%x]  ", ptr8[i]);
						}
						RING_IO_0Print("\n");
#endif

						if ((RING_IO_BytesToTransfer1 != 0)
								&& ((bytesTransfered + acqSize)
										> RING_IO_BytesToTransfer1)) {

							/* we have acquired more buffer than the rest of data
							 * bytes to be transferred */
							if (bytesTransfered != RING_IO_BytesToTransfer1) {

								relStatus = RingIO_release(
										RingIOWriterHandle1,
										(RING_IO_BytesToTransfer1
												- bytesTransfered));
								if (DSP_FAILED(relStatus)) {
									RING_IO_1Print("RingIO_release1 () in Writer "
											"task failed relStatus = [0x%x]"
											"\n", relStatus);
								}
							}

							/* Cancel the  rest of the buffer */
							status = RingIO_cancel(RingIOWriterHandle1);
							if (DSP_FAILED(status)) {
								RING_IO_1Print("RingIO_cancel1 () in Writer"
										"task failed "
										"status = [0x%x]\n", status);
							}
							bytesTransfered = RING_IO_BytesToTransfer1;

						} else {

							relStatus = RingIO_release(RingIOWriterHandle1,
									acqSize);
							if (DSP_FAILED(relStatus)) {
								RING_IO_1Print("RingIO_release1 () in Writer task "
										"failed. relStatus = [0x%x]\n", relStatus);
							} else {
								bytesTransfered += acqSize;
							}
						}

						/*if ((bytesTransfered % (RING_IO_WRITER_BUF_SIZE * 8u)) == 0)
						 {
						 RING_IO_1Print ("GPP-->DSP1:Bytes Transferred: %lu \n",
						 bytesTransfered);
						 }*/
					} else {

						/* Acquired failed, Wait for empty buffer to become
						 * available.
						 */
						status = RING_IO_WaitSem(semPtrWriter1);
						if (DSP_FAILED(status)) {
							RING_IO_1Print("RING_IO_WaitSem1 () Writer SEM failed "
									"Status = [0x%x]\n", status);
						}
					}
				}
			}

#if DEBUG_ENABLED
			RING_IO_1Print("GPP-->DSP1:Total Bytes Transmitted  %ld \n",
					bytesTransfered);
#endif

			bytesTransfered = 0;

			/* Send  End of  data transfer attribute to DSP */
			type = (Uint16) RINGIO_DATA_END;

			do {
				status = RingIO_setAttribute(RingIOWriterHandle1, 0, type, 0);
#if DEBUG_ENABLED
				if (DSP_SUCCEEDED(status)) {
					RING_IO_1Print("RingIO_setAttribute1 succeeded to set the  "
							"RINGIO_DATA_END. Status = [0x%x]\n", status);
				}
#endif
				//else {
				//	RING_IO_YieldClient ();
				//}
			}while (status != RINGIO_SUCCESS);

#if DEBUG_ENABLED
			RING_IO_0Print("GPP-->DSP1:Sent Data Transfer End Attribute\n");
#endif

			if (DSP_SUCCEEDED(status)) {

				/* Send Notification  to  the reader (DSP)
				 * This allows DSP  application to come out from blocked state  if
				 * it is waiting for Data buffer and  GPP sent only data end
				 * attribute.
				 */
				status = RingIO_sendNotify(RingIOWriterHandle1,
						(RingIO_NotifyMsg) NOTIFY_DATA_END);
				if (DSP_FAILED(status)) {
					RING_IO_1Print("RingIO_sendNotify1 failed to send notification "
							"NOTIFY_DATA_END. Status = [0x%x]\n", status);
				} else {
#if DEBUG_ENABLED
					RING_IO_0Print("GPP-->DSP1:Sent Data Transfer End Notification"
							" \n");
#endif
					//	RING_IO_YieldClient ();
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		//end the execute of write task
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		// the execute of read task
		////////////////////////////////////////////////////////////////////////////////

		if (DSP_SUCCEEDED(status)) {

			/*
			 * Wait for notification from  DSP  about data
			 * transfer
			 */
			status = RING_IO_WaitSem(semPtrReader1);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RING_IO_WaitSem1 () Reader SEM failed "
						"Status = [0x%x]\n", status);
			}

#if DEBUG_ENABLED
			RING_IO_0Print(" RING_IO_WaitSem1 () Reader SEM  \n");
#endif

			if (fReaderStart1 == TRUE) {

				fReaderStart1 = FALSE;

				/* Got  data transfer start notification from DSP*/
				do {

					/* Get the start attribute from dsp */
					status = RingIO_getAttribute(RingIOReaderHandle1, &type,
							&param);
					if ((status == RINGIO_SUCCESS)
							|| (status == RINGIO_SPENDINGATTRIBUTE)) {

						if (type == (Uint16) RINGIO_DATA_START) {
#if DEBUG_ENABLED
							RING_IO_0Print("GPP<--DSP1:Received Data Transfer"
									"Start Attribute\n");
#endif
							break;
						} else {
							RING_IO_1Print("RingIO_getAttribute1 () Reader failed "
									"Unknown attribute received instead of "
									"RINGIO_DATA_START. Status = [0x%x]\n", status);
							RING_IO_Sleep(10);
						}
					} else {
						RING_IO_Sleep(10);
					}

				}while ((status != RINGIO_SUCCESS)
						&& (status != RINGIO_SPENDINGATTRIBUTE));
			}

			/* Now reader  can start reading data from the ringio created
			 * by Dsp as the writer
			 */
			acqSize = len_out;

			while (exitFlag == FALSE) {

				status = RingIO_acquire(RingIOReaderHandle1, &bufPtr, &acqSize);

				if ((status == RINGIO_SUCCESS) || (acqSize > 0)) {
					/* Got buffer from DSP.*/
					ptr8 = (Uint8 *)(bufPtr);
					for (i = 0; ((i < acqSize) && ((totalRcvbytes + i) < len_out));
							i++)
					buf_out[(totalRcvbytes + i)] = ptr8[i];

					rcvSize -= acqSize;
					totalRcvbytes += acqSize;

#if DEBUG_ENABLED
					for (i = 0; i < acqSize; i++)
					RING_IO_1Print("Reader1 Received [0x%x]  ", buf_out[i]);
					RING_IO_0Print("\n");
#endif

					/* Verify the received data */
					//if (DSP_SOK != RING_IO_Reader_VerifyData(bufPtr, acqSize)) {
					//	RING_IO_1Print(" Data1 verification failed after"
					//			"%ld bytes received from DSP \n", totalRcvbytes);
					//}
					/* Release the acquired buffer */
					relStatus = RingIO_release(RingIOReaderHandle1, acqSize);
					if (DSP_FAILED(relStatus)) {
						RING_IO_1Print("RingIO_release1 () in Writer task"
								"failed relStatus = [0x%x]\n", relStatus);
					}

					/* Set the acqSize for the next acquire */
					if (rcvSize == 0) {
						/* Reset  the rcvSize to  size of the full buffer  */
						rcvSize = len_out;
						acqSize = len_out;
					} else {
						/*Acquire the partial buffer  in next acquire */
						acqSize = rcvSize;
					}

					/*if ((totalRcvbytes % (8192u)) == 0u) {
					 RING_IO_1Print ("GPP<--DSP1:Bytes Received :%lu \n",
					 totalRcvbytes);

					 }*/

				} else if ((status == RINGIO_SPENDINGATTRIBUTE)
						&& (acqSize == 0u)) {

					/* Attribute is pending,Read it */
					attrStatus = RingIO_getAttribute(RingIOReaderHandle1, &type,
							&param);
					if ((attrStatus == RINGIO_SUCCESS)
							|| (attrStatus == RINGIO_SPENDINGATTRIBUTE)) {

						if (type == RINGIO_DATA_END) {
							/* End of data transfer from DSP */
#if DEBUG_ENABLED
							RING_IO_0Print("GPP<--DSP1:Received Data Transfer"
									"End Attribute \n");
#endif
							exitFlag = TRUE;/* Come Out of while loop */
						} else {
							RING_IO_1Print("RingIO_getAttribute () Reader "
									"error,Unknown attribute "
									" received Status = [0x%x]\n", attrStatus);
						}
					} else if (attrStatus == RINGIO_EVARIABLEATTRIBUTE) {

						vAttrSize = sizeof(attrs);
						attrStatus = RingIO_getvAttribute(RingIOReaderHandle1,
								&type, &param, attrs, &vAttrSize);

						if ((attrStatus == RINGIO_SUCCESS)
								|| (attrStatus == RINGIO_SPENDINGATTRIBUTE)) {

							/* Success in receiving  variable attribute*/
							rcvSize = attrs[0];
							/* Set the  acquire size equal to the
							 * rcvSize
							 */
							acqSize = rcvSize;
#if DEBUG_ENABLED
							RING_IO_1Print("!!#RingIO_getAttribute () Reader "
									" received size = [%d]\n", rcvSize);
#endif

						} else if (attrStatus == RINGIO_EVARIABLEATTRIBUTE) {

							RING_IO_1Print("Error: "
									"buffer is not sufficient to receive"
									"the  variable attribute "
									"Status = [0x%x]\n", attrStatus);
						} else if (attrStatus == DSP_EINVALIDARG) {

							RING_IO_1Print("Error: "
									"Invalid args to receive"
									"the  variable attribute "
									"Status = [0x%x]\n", attrStatus);

						} else if (attrStatus == RINGIO_EPENDINGDATA) {

							RING_IO_1Print("Error:RingIO_getvAttribute "
									"Status = [0x%x]\n", attrStatus);
						} else {
							/* Unable to get  variable attribute
							 * go back to read data.
							 * this may occur due to ringo flush by the DSP
							 * or may be due to  general failure
							 */
						}

					} else {
						RING_IO_1Print("RingIO_getAttribute () Reader error "
								"Status = [0x%x]\n", attrStatus);
					}
				} else if ((status == RINGIO_EFAILURE)
						|| (status == RINGIO_EBUFEMPTY)) {

					/* Failed to acquire buffer */
					status = RING_IO_WaitSem(semPtrReader1);
					if (DSP_FAILED(status)) {
						RING_IO_1Print("RING_IO_WaitSem () Reader SEM failed "
								"Status = [0x%x]\n", status);
					}
				} else {
					acqSize = len_out;

				}

				/* Reset the acquired size if it is changed to zero by the
				 * failed acquire call
				 */
				if (acqSize == 0) {
					acqSize = len_out;
				}

			}
		}
#if DEBUG_ENABLED
		RING_IO_1Print("GPP<--DSP1:Bytes Received %ld \n", totalRcvbytes);
#endif

		if (fReaderEnd1 != TRUE) {
			/* If data transfer end notification  not yet received
			 * from DSP ,wait for it.
			 */
			status = RING_IO_WaitSem(semPtrReader1);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RING_IO_WaitSem1 () Reader SEM failed "
						"Status = [0x%x]\n", status);
			}
		}
		//else {

		//}
//	totalRcvbytes = 0;
//	rcvSize = RING_IO_BufferSize1;
		fReaderEnd1 = FALSE;
		exitFlag = FALSE;
#if DEBUG_ENABLED
		RING_IO_0Print("End Reader Task1  () \n");
#endif

		return totalRcvbytes;

		////////////////////////////////////////////////////////////////////////////////
		//End the execute of read task
		////////////////////////////////////////////////////////////////////////////////

	}

/////////////////////////////////////////////////////////////////////////////////

	NORMAL_API DSP_STATUS RING_IO_WriteClient1_Close() {
		DSP_STATUS tmpStatus = DSP_SOK;
		DSP_STATUS status = DSP_SOK;
		do {
			tmpStatus = RingIO_sendNotify(RingIOWriterHandle1,
					(RingIO_NotifyMsg) NOTIFY_DSP_END);
			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_0Print("RingIO_sendNotify (RingIOWriterHandle1)\n");
				RING_IO_Sleep(10);
			} else {
				status = RINGIO_SUCCESS;
			}
		}while (DSP_FAILED(tmpStatus));

		////////////////////////////////////////////////////////////////////////////////
		//close  the write  task
		////////////////////////////////////////////////////////////////////////////////

		/* Delete the semaphore used for notification */
		if (semPtrWriter1 != NULL) {
			tmpStatus = RING_IO_DeleteSem(semPtrWriter1);
			if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("Error RING_IO_DeleteSem1 () Writer SEM failed "
						"Status = [0x%x]\n", status);
				return status;
			}
		}

		//RING_IO_0Print ("RING_IO_DeleteSem1 () Writer SEM  \n");

		/*
		 *  Close the RingIO to be used with GPP as the writer.
		 */
		if (RingIOWriterHandle1 != NULL) {
			while ((RingIO_getValidSize(RingIOWriterHandle1) != 0)
					|| (RingIO_getValidAttrSize(RingIOWriterHandle1) != 0)) {
				RING_IO_Sleep(10);
			}
			tmpStatus = RingIO_close(RingIOWriterHandle1);
			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("RingIO_close1 () Writer failed. Status = [0x%x]\n",
						status);
				return status;
			}
		}

#if DEBUG_ENABLED
		RING_IO_0Print("Leaving RING_IO_WriterClient1 () \n");
#endif
		////////////////////////////////////////////////////////////////////////////////
		//End close  the write  task
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		//close  the read  task
		////////////////////////////////////////////////////////////////////////////////

		if (semPtrReader1 != NULL) {
			tmpStatus = RING_IO_DeleteSem(semPtrReader1);
			if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("RING_IO_DeleteSem1 () Reader SEM failed "
						"Status = [0x%x]\n", status);
			}
		}
		//RING_IO_0Print (" RING_IO_DeleteSem1 () Reader SEM   \n");

		/*
		 *  Close the RingIO to be used with GPP as the reader.
		 */
		if (RingIOReaderHandle1 != NULL) {
			tmpStatus = RingIO_close(RingIOReaderHandle1);
			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("RingIO_close1 () Reader failed. Status = [0x%x]\n",
						status);
			}
		}
#if DEBUG_ENABLED
		RING_IO_0Print("Leaving RING_IO_ReaderClient1 () \n");
#endif
		////////////////////////////////////////////////////////////////////////////////
		//End close  the read  task
		////////////////////////////////////////////////////////////////////////////////

		/* Exit */
//	RING_IO_Exit_client(&writerClientInfo1);
		status = tmpStatus;
		return status;

	}

	NORMAL_API DSP_STATUS RING_IO_WriteClient2_Initial() {
		DSP_STATUS status = DSP_SOK;
		//DSP_STATUS relStatus = DSP_SOK;
		//DSP_STATUS tmpStatus = DSP_SOK;
		//RingIO_BufPtr bufPtr = NULL;
		//Pvoid semPtrWriter = NULL;

		Uint16 type;
		//Uint32 acqSize;

		//Pvoid semPtrReader = NULL;
		//Uint32 param;
		//Uint32 vAttrSize = 0;
		//Uint32 rcvSize = RING_IO_BufferSize1;
		//Uint32 totalRcvbytes = 0;

		DSP_STATUS attrStatus = DSP_SOK;

		////////////////////////////////////////////////////////////////////////////////
		// initial the write task
		////////////////////////////////////////////////////////////////////////////////
#if DEBUG_ENABLED
		RING_IO_0Print("Entered RING_IO_WriterClient2 ()\n");
#endif
		/*
		 *  Open the RingIO to be used with GPP as the writer.
		 *
		 *  Value of the flags indicates:
		 *     No cache coherence for: Control structure
		 *                             Data buffer
		 *                             Attribute buffer
		 *     Exact size requirement.
		 */
		RingIOWriterHandle2 = RingIO_open(RingIOWriterName2, RINGIO_MODE_WRITE,
				(Uint32)(RINGIO_NEED_EXACT_SIZE));
		if (RingIOWriterHandle2 == NULL) {
			status = RINGIO_EFAILURE;
			RING_IO_1Print("Error RingIO_open2 () Writer failed. Status = [0x%x]\n",
					status);
			return status;
		}

		if (DSP_SUCCEEDED(status)) {
			/* Create the semaphore to be used for notification */
			status = RING_IO_CreateSem(&semPtrWriter2);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("Error RING_IO_CreateSem2 () Writer SEM failed "
						"Status = [0x%x]\n", status);
				return status;
			}
		}

		if (DSP_SUCCEEDED(status)) {
			/*
			 *  Set the notification for Writer.
			 */
			do {
				/* Set the notifier for writer for RingIO created by the GPP. */
				status = RingIO_setNotifier(RingIOWriterHandle2,
						RINGIO_NOTIFICATION_ONCE, RING_IO_WRITER_BUF_SIZE,
						&RING_IO_Writer_Notify2,
						(RingIO_NotifyParam) semPtrWriter2);
				if (status != RINGIO_SUCCESS) {
					RING_IO_Sleep(10);
				}
			}while (DSP_FAILED(status));

		}
		////////////////////////////////////////////////////////////////////////////////
		//end  initial the write task
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		// initial the read  task
		////////////////////////////////////////////////////////////////////////////////
#if DEBUG_ENABLED
		RING_IO_0Print("Entered RING_IO_ReaderClient2 ()\n");
#endif

		/*
		 *  Open the RingIO to be used with GPP as the reader.
		 *  Value of the flags indicates:
		 *     No cache coherence for: Control structure
		 *                             Data buffer
		 *                             Attribute buffer
		 *     Exact size requirement false.
		 */
		do {
			RingIOReaderHandle2 = RingIO_open(RingIOReaderName2, RINGIO_MODE_READ,
					0);

			//	RING_IO_0Print (" RingIO_open (RingIOReaderName1()ing \n") ;

		}while (RingIOReaderHandle2 == NULL);

		/* Create the semaphore to be used for notification */
		status = RING_IO_CreateSem(&semPtrReader2);
		if (DSP_FAILED(status)) {

			RING_IO_1Print("Error RING_IO_CreateSem2 () Reader SEM failed "
					"Status = [0x%x]\n", status);
			return status;
		}
		if (DSP_SUCCEEDED(status)) {
			do {

				/* Set the notifier for reader for RingIO created by the DSP. */
				/*
				 * Set water mark to zero. and try to acquire the full buffer
				 * and  read what ever is available.
				 */
				status = RingIO_setNotifier(RingIOReaderHandle2,
						RINGIO_NOTIFICATION_ONCE, 0, &RING_IO_Reader_Notify2,
						(RingIO_NotifyParam) semPtrReader2);
				if (DSP_FAILED(status)) {
					/*RingIO_setNotifier () Reader failed  */
					RING_IO_Sleep(10);
				}
			}while (DSP_FAILED(status));
		}

		//RING_IO_0Print (" RingIO_setNotifier1 Reader SEM  \n");
#if DEBUG_ENABLED
		RING_IO_0Print("End initial the read  task2 \n");
#endif
		return status;
		////////////////////////////////////////////////////////////////////////////////
		//end initial the read  task
		////////////////////////////////////////////////////////////////////////////////

	}

	NORMAL_API Uint32 RING_IO_WriteClient2_Task(const char *buf_in,
			const int len_in, char *buf_out, int len_out)
	{
		Uint16 type;
		DSP_STATUS status = DSP_SOK;
		DSP_STATUS relStatus = DSP_SOK;
		//DSP_STATUS tmpStatus = DSP_SOK;
		DSP_STATUS attrStatus = DSP_SOK;
		RingIO_BufPtr bufPtr = NULL;
		Uint32 bytesTransfered = 0;
		Uint32 attrs[RING_IO_VATTR_SIZE];
		Uint16 i = 0;
		Uint8 exitFlag = FALSE;
		Uint32 acqSize;
		Uint8 *ptr8;

		//Pvoid semPtrReader = NULL;
		Uint32 param;
		Uint32 vAttrSize = 0;
		Uint32 rcvSize = 0;
		Uint32 totalRcvbytes = 0;

		RING_IO_BytesToTransfer2 = len_in;
		rcvSize = len_out;

#if DEBUG_ENABLED
		RING_IO_0Print("Task2 start send \n");
#endif
		////////////////////////////////////////////////////////////////////////////////
		//the execute of write task
		////////////////////////////////////////////////////////////////////////////////

		if (DSP_SUCCEEDED(status)) {
			/* Send data transfer attribute (Fixed attribute) to DSP*/
			type = (Uint16) RINGIO_DATA_START;
			status = RingIO_setAttribute(RingIOWriterHandle2, 0, type, 0);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RingIO_setAttribute1 failed to set the  "
						"RINGIO_DATA_START. Status = [0x%x]\n", status);
			}
		}
#if DEBUG_ENABLED
		RING_IO_0Print("RingIO_setAttribute2 () Writer SEM   \n");
#endif

		if (DSP_SUCCEEDED(status)) {
			/* Send Notification  to  the reader (DSP)*/
#if DEBUG_ENABLED
			RING_IO_0Print("GPP-->DSP2:Sent Data Transfer Start "
					"Attribute\n");
#endif
			do {
				status = RingIO_sendNotify(RingIOWriterHandle2,
						(RingIO_NotifyMsg) NOTIFY_DATA_START);
				if (DSP_FAILED(status)) {
					/* RingIO_sendNotify failed to send notification */
					RING_IO_Sleep(10);
				} else {
#if DEBUG_ENABLED
					RING_IO_0Print("GPP-->DSP2:Sent Data Transfer Start "
							"Notification \n");
#endif
				}
			}while (status != RINGIO_SUCCESS);
		}

		if (DSP_SUCCEEDED(status)) {
#if DEBUG_ENABLED
			RING_IO_1Print("Client2 Bytes to transfer :%ld \n", RING_IO_BytesToTransfer2);
			RING_IO_1Print("Client2 Data buffer size  :%ld \n", RING_IO_BufferSize_Tx2);
#endif

			while ((RING_IO_BytesToTransfer2 == 0)
					|| (bytesTransfered < RING_IO_BytesToTransfer2)) {

				/* Update the attrs to send variable attribute to DSP*/
				attrs[0] = RING_IO_WRITER_BUF_SIZE;
				//attrs [0] = RING_IO_BytesToTransfer1;

				/* ----------------------------------------------------------------
				 * Send to DSP.
				 * ----------------------------------------------------------------
				 */
				/* Set the scaling factor variable attribute*/
				status = RingIO_setvAttribute(RingIOWriterHandle2, 0, /* at the beginning */
						0, /* No type */
						0, attrs, sizeof(attrs));
				if (DSP_FAILED(status)) {
					/* RingIO_setvAttribute failed */
					RING_IO_Sleep(10);
				} else {
					/* Acquire writer bufs and initialize and release them. */
					acqSize = RING_IO_WRITER_BUF_SIZE;
					//acqSize = RING_IO_BytesToTransfer1;
					status = RingIO_acquire(RingIOWriterHandle2, &bufPtr, &acqSize);

					/* If acquire success . Write to  ring bufer and then release
					 * the acquired.
					 */
					if ((DSP_SUCCEEDED(status)) && (acqSize > 0)) {
						//copy the data to RIngIO buf
						ptr8 = (Uint8 *)(bufPtr);
						for (i = 0;
								((i < acqSize)
										&& ((i + bytesTransfered)
												< RING_IO_BytesToTransfer2)); i++) {
							ptr8[i] = buf_in[(bytesTransfered + i)];
						}

						//RING_IO_InitBuffer (bufPtr, acqSize);

						//debug
#if DEBUG_ENABLED

						for (i = 0; i < 5; i++) {
							RING_IO_1Print("Client2 Send [0x%x]  ", ptr8[i]);
						}
						RING_IO_0Print("\n");
#endif

						if ((RING_IO_BytesToTransfer2 != 0)
								&& ((bytesTransfered + acqSize)
										> RING_IO_BytesToTransfer2)) {

							/* we have acquired more buffer than the rest of data
							 * bytes to be transferred */
							if (bytesTransfered != RING_IO_BytesToTransfer2) {

								relStatus = RingIO_release(
										RingIOWriterHandle2,
										(RING_IO_BytesToTransfer2
												- bytesTransfered));
								if (DSP_FAILED(relStatus)) {
									RING_IO_1Print("RingIO_release2 () in Writer "
											"task failed relStatus = [0x%x]"
											"\n", relStatus);
								}
							}

							/* Cancel the  rest of the buffer */
							status = RingIO_cancel(RingIOWriterHandle2);
							if (DSP_FAILED(status)) {
								RING_IO_1Print("RingIO_cancel2 () in Writer"
										"task failed "
										"status = [0x%x]\n", status);
							}
							bytesTransfered = RING_IO_BytesToTransfer2;

						} else {

							relStatus = RingIO_release(RingIOWriterHandle2,
									acqSize);
							if (DSP_FAILED(relStatus)) {
								RING_IO_1Print("RingIO_release2 () in Writer task "
										"failed. relStatus = [0x%x]\n", relStatus);
							} else {
								bytesTransfered += acqSize;
							}
						}

						/*if ((bytesTransfered % (RING_IO_WRITER_BUF_SIZE * 8u)) == 0)
						 {
						 RING_IO_1Print ("GPP-->DSP1:Bytes Transferred: %lu \n",
						 bytesTransfered);
						 }*/
					} else {

						/* Acquired failed, Wait for empty buffer to become
						 * available.
						 */
						status = RING_IO_WaitSem(semPtrWriter2);
						if (DSP_FAILED(status)) {
							RING_IO_1Print("RING_IO_WaitSem2 () Writer SEM failed "
									"Status = [0x%x]\n", status);
						}
					}
				}
			}

#if DEBUG_ENABLED
			RING_IO_1Print("GPP-->DSP2:Total Bytes Transmitted  %ld \n",
					bytesTransfered);
#endif

			bytesTransfered = 0;

			/* Send  End of  data transfer attribute to DSP */
			type = (Uint16) RINGIO_DATA_END;

			do {
				status = RingIO_setAttribute(RingIOWriterHandle2, 0, type, 0);
#if DEBUG_ENABLED
				if (DSP_SUCCEEDED(status)) {
					RING_IO_1Print("RingIO_setAttribute2 succeeded to set the  "
							"RINGIO_DATA_END. Status = [0x%x]\n", status);
				}
#endif
				//else {
				//	RING_IO_YieldClient ();
				//}
			}while (status != RINGIO_SUCCESS);

#if DEBUG_ENABLED
			RING_IO_0Print("GPP-->DSP2:Sent Data Transfer End Attribute\n");
#endif

			if (DSP_SUCCEEDED(status)) {

				/* Send Notification  to  the reader (DSP)
				 * This allows DSP  application to come out from blocked state  if
				 * it is waiting for Data buffer and  GPP sent only data end
				 * attribute.
				 */
				status = RingIO_sendNotify(RingIOWriterHandle2,
						(RingIO_NotifyMsg) NOTIFY_DATA_END);
				if (DSP_FAILED(status)) {
					RING_IO_1Print("RingIO_sendNotify2 failed to send notification "
							"NOTIFY_DATA_END. Status = [0x%x]\n", status);
				} else {
#if DEBUG_ENABLED
					RING_IO_0Print("GPP-->DSP2:Sent Data Transfer End Notification"
							" \n");
#endif
					//	RING_IO_YieldClient ();
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		//end the execute of write task
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		// the execute of read task
		////////////////////////////////////////////////////////////////////////////////

		if (DSP_SUCCEEDED(status)) {

			/*
			 * Wait for notification from  DSP  about data
			 * transfer
			 */
			status = RING_IO_WaitSem(semPtrReader2);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RING_IO_WaitSem2 () Reader SEM failed "
						"Status = [0x%x]\n", status);
			}

#if DEBUG_ENABLED
			RING_IO_0Print(" RING_IO_WaitSem2 () Reader SEM  \n");
#endif

			if (fReaderStart1 == TRUE) {

				fReaderStart1 = FALSE;

				/* Got  data transfer start notification from DSP*/
				do {

					/* Get the start attribute from dsp */
					status = RingIO_getAttribute(RingIOReaderHandle2, &type,
							&param);
					if ((status == RINGIO_SUCCESS)
							|| (status == RINGIO_SPENDINGATTRIBUTE)) {

						if (type == (Uint16) RINGIO_DATA_START) {
#if DEBUG_ENABLED
							RING_IO_0Print("GPP<--DSP2:Received Data Transfer"
									"Start Attribute\n");
#endif
							break;
						} else {
							RING_IO_1Print("RingIO_getAttribute2 () Reader failed "
									"Unknown attribute received instead of "
									"RINGIO_DATA_START. Status = [0x%x]\n", status);
							RING_IO_Sleep(10);
						}
					} else {
						RING_IO_Sleep(10);
					}

				}while ((status != RINGIO_SUCCESS)
						&& (status != RINGIO_SPENDINGATTRIBUTE));
			}

			/* Now reader  can start reading data from the ringio created
			 * by Dsp as the writer
			 */
			acqSize = len_out;

			while (exitFlag == FALSE) {

				status = RingIO_acquire(RingIOReaderHandle2, &bufPtr, &acqSize);

				if ((status == RINGIO_SUCCESS) || (acqSize > 0)) {
					/* Got buffer from DSP.*/
					ptr8 = (Uint8 *)(bufPtr);
					for (i = 0; ((i < acqSize) && ((totalRcvbytes + i) < len_out));
							i++)
					buf_out[(totalRcvbytes + i)] = ptr8[i];

					rcvSize -= acqSize;
					totalRcvbytes += acqSize;

#if DEBUG_ENABLED
					for (i = 0; i < acqSize; i++)
					RING_IO_1Print("Reader2 Received [0x%x]  ", buf_out[i]);
					RING_IO_0Print("\n");
#endif

					/* Verify the received data */
					//if (DSP_SOK != RING_IO_Reader_VerifyData(bufPtr, acqSize)) {
					//	RING_IO_1Print(" Data1 verification failed after"
					//			"%ld bytes received from DSP \n", totalRcvbytes);
					//}
					/* Release the acquired buffer */
					relStatus = RingIO_release(RingIOReaderHandle2, acqSize);
					if (DSP_FAILED(relStatus)) {
						RING_IO_1Print("RingIO_release2 () in Writer task"
								"failed relStatus = [0x%x]\n", relStatus);
					}

					/* Set the acqSize for the next acquire */
					if (rcvSize == 0) {
						/* Reset  the rcvSize to  size of the full buffer  */
						rcvSize = len_out;
						acqSize = len_out;
					} else {
						/*Acquire the partial buffer  in next acquire */
						acqSize = rcvSize;
					}

					/*if ((totalRcvbytes % (8192u)) == 0u) {
					 RING_IO_1Print ("GPP<--DSP1:Bytes Received :%lu \n",
					 totalRcvbytes);

					 }*/

				} else if ((status == RINGIO_SPENDINGATTRIBUTE)
						&& (acqSize == 0u)) {

					/* Attribute is pending,Read it */
					attrStatus = RingIO_getAttribute(RingIOReaderHandle2, &type,
							&param);
					if ((attrStatus == RINGIO_SUCCESS)
							|| (attrStatus == RINGIO_SPENDINGATTRIBUTE)) {

						if (type == RINGIO_DATA_END) {
							/* End of data transfer from DSP */
#if DEBUG_ENABLED
							RING_IO_0Print("GPP<--DSP2: Received Data Transfer"
									"End Attribute \n");
#endif
							exitFlag = TRUE; /* Come Out of while loop */
						} else {
							RING_IO_1Print("RingIO_getAttribute () Reader "
									"error,Unknown attribute "
									" received Status = [0x%x]\n", attrStatus);
						}
					} else if (attrStatus == RINGIO_EVARIABLEATTRIBUTE) {

						vAttrSize = sizeof(attrs);
						attrStatus = RingIO_getvAttribute(RingIOReaderHandle2,
								&type, &param, attrs, &vAttrSize);

						if ((attrStatus == RINGIO_SUCCESS)
								|| (attrStatus == RINGIO_SPENDINGATTRIBUTE)) {

							/* Success in receiving  variable attribute*/
							rcvSize = attrs[0];
							/* Set the  acquire size equal to the
							 * rcvSize
							 */
							acqSize = rcvSize;
#if DEBUG_ENABLED
							RING_IO_1Print("!!#RingIO_getAttribute () Reader 2"
									" received size = [%d]\n", rcvSize);
#endif

						} else if (attrStatus == RINGIO_EVARIABLEATTRIBUTE) {

							RING_IO_1Print("Error2: "
									"buffer is not sufficient to receive"
									"the  variable attribute "
									"Status = [0x%x]\n", attrStatus);
						} else if (attrStatus == DSP_EINVALIDARG) {

							RING_IO_1Print("Error2: "
									"Invalid args to receive"
									"the  variable attribute "
									"Status = [0x%x]\n", attrStatus);

						} else if (attrStatus == RINGIO_EPENDINGDATA) {

							RING_IO_1Print("Error2:RingIO_getvAttribute "
									"Status = [0x%x]\n", attrStatus);
						} else {
							/* Unable to get  variable attribute
							 * go back to read data.
							 * this may occur due to ringo flush by the DSP
							 * or may be due to  general failure
							 */
						}

					} else {
						RING_IO_1Print("RingIO_getAttribute2 () Reader error "
								"Status = [0x%x]\n", attrStatus);
					}
				} else if ((status == RINGIO_EFAILURE)
						|| (status == RINGIO_EBUFEMPTY)) {

					/* Failed to acquire buffer */
					status = RING_IO_WaitSem(semPtrReader2);
					if (DSP_FAILED(status)) {
						RING_IO_1Print("RING_IO_WaitSem2 () Reader SEM failed "
								"Status = [0x%x]\n", status);
					}
				} else {
					acqSize = len_out;

				}

				/* Reset the acquired size if it is changed to zero by the
				 * failed acquire call
				 */
				if (acqSize == 0) {
					acqSize = len_out;
				}

			}
		}
#if DEBUG_ENABLED
		RING_IO_1Print("GPP<--DSP2: Bytes Received %ld \n", totalRcvbytes);
#endif

		if (fReaderEnd2 != TRUE) {
			/* If data transfer end notification  not yet received
			 * from DSP ,wait for it.
			 */
			status = RING_IO_WaitSem(semPtrReader2);
			if (DSP_FAILED(status)) {
				RING_IO_1Print("RING_IO_WaitSem2 () Reader SEM failed "
						"Status = [0x%x]\n", status);
			}
		}
		//else {

		//}
//	totalRcvbytes = 0;
//	rcvSize = RING_IO_BufferSize1;
		fReaderEnd2 = FALSE;
		exitFlag = FALSE;
#if DEBUG_ENABLED
		RING_IO_0Print("End Reader Task2  () \n");
#endif

		return totalRcvbytes;

		////////////////////////////////////////////////////////////////////////////////
		//End the execute of read task
		////////////////////////////////////////////////////////////////////////////////

	}

////////////////////////////////////////////////////////////////////////////////////

	NORMAL_API DSP_STATUS RING_IO_WriteClient2_Close()
	{
		DSP_STATUS tmpStatus = DSP_SOK;
		DSP_STATUS status = DSP_SOK;
		do {
			tmpStatus = RingIO_sendNotify(RingIOWriterHandle2,
					(RingIO_NotifyMsg) NOTIFY_DSP_END);
			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_0Print("RingIO_sendNotify (RingIOWriterHandle2)\n");
				RING_IO_Sleep(10);
			} else {
				status = RINGIO_SUCCESS;
			}
		}while (DSP_FAILED(tmpStatus));

		////////////////////////////////////////////////////////////////////////////////
		//close  the write  task
		////////////////////////////////////////////////////////////////////////////////

		/* Delete the semaphore used for notification */
		if (semPtrWriter2 != NULL) {
			tmpStatus = RING_IO_DeleteSem(semPtrWriter2);
			if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("Error RING_IO_DeleteSem2 () Writer SEM failed "
						"Status = [0x%x]\n", status);
				return status;
			}
		}

		//RING_IO_0Print ("RING_IO_DeleteSem1 () Writer SEM  \n");

		/*
		 *  Close the RingIO to be used with GPP as the writer.
		 */
		if (RingIOWriterHandle2 != NULL) {
			while ((RingIO_getValidSize(RingIOWriterHandle2) != 0)
					|| (RingIO_getValidAttrSize(RingIOWriterHandle2) != 0)) {
				RING_IO_Sleep(10);
			}
			tmpStatus = RingIO_close(RingIOWriterHandle2);
			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("RingIO_close2 () Writer failed. Status = [0x%x]\n",
						status);
				return status;
			}
		}
#if DEBUG_ENABLED
		RING_IO_0Print("Leaving RING_IO_WriterClient2 () \n");
#endif
		////////////////////////////////////////////////////////////////////////////////
		//End close  the write  task
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		//close  the read  task
		////////////////////////////////////////////////////////////////////////////////

		if (semPtrReader2 != NULL) {
			tmpStatus = RING_IO_DeleteSem(semPtrReader2);
			if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("RING_IO_DeleteSem2 () Reader SEM failed "
						"Status = [0x%x]\n", status);
			}
		}
		//RING_IO_0Print (" RING_IO_DeleteSem1 () Reader SEM   \n");

		/*
		 *  Close the RingIO to be used with GPP as the reader.
		 */
		if (RingIOReaderHandle2 != NULL) {
			tmpStatus = RingIO_close(RingIOReaderHandle2);
			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_1Print("RingIO_close2 () Reader failed. Status = [0x%x]\n",
						status);
			}
		}
#if DEBUG_ENABLED
		RING_IO_0Print("Leaving RING_IO_ReaderClient2 () \n");
#endif
		////////////////////////////////////////////////////////////////////////////////
		//End close  the read  task
		////////////////////////////////////////////////////////////////////////////////

		/* Exit */
//	RING_IO_Exit_client(&writerClientInfo1);
		status = tmpStatus;
		return status;

	}

	/** ----------------------------------------------------------------------------
	 *  @func   RING_IO_Delete
	 *
	 *  @desc   This function implements the deletion of the RingIO
	 *
	 *  @modif  None
	 *  ----------------------------------------------------------------------------
	 */

	NORMAL_API Void RING_IO_Delete(IN Uint8 processorId)
	{
		DSP_STATUS status = DSP_SOK;
		DSP_STATUS tmpStatus = DSP_SOK;

#if DEBUG_ENABLED
		RING_IO_0Print("Entered RING_IO_Delete ()\n");
#endif
		/*
		 *  Delete the sending RingIO to be used with GPP as the writer.
		 */
		do {
#if defined (DSPLINK_LEGACY_SUPPORT)
			tmpStatus = RingIO_delete (RingIOWriterName1);
#else
			tmpStatus = RingIO_delete(processorId, RingIOWriterName1);
#endif /* if defined (DSPLINK_LEGACY_SUPPORT) */

			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_0Print("RingIO_delete (RingIOWriterName1)\n");
				RING_IO_Sleep(10);
			} else {
				status = RINGIO_SUCCESS;
			}
		}while (DSP_FAILED(tmpStatus));

		/*
		 *  Delete the receiving RingIO to be used with GPP as the writer.
		 */
		do {
#if defined (DSPLINK_LEGACY_SUPPORT)
			tmpStatus = RingIO_delete (RingIOWriterName2);
#else
			tmpStatus = RingIO_delete(processorId, RingIOWriterName2);
#endif /* if defined (DSPLINK_LEGACY_SUPPORT) */

			if (DSP_FAILED(tmpStatus)) {
				status = tmpStatus;
				RING_IO_Sleep(10);
			} else {
				status = RINGIO_SUCCESS;
			}
		}while (DSP_FAILED(tmpStatus));

		/*
		 *  Stop execution on DSP.
		 */
		tmpStatus = PROC_stop(processorId);
		if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
			status = tmpStatus;
			RING_IO_1Print("PROC_stop () failed. Status = [0x%x]\n", status);
		}

		/*
		 *  Close the pool
		 */
		tmpStatus = POOL_close(POOL_makePoolId(processorId, SAMPLE_POOL_ID));
		if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
			status = tmpStatus;
			RING_IO_1Print("POOL_close () failed. Status = [0x%x]\n", status);
		}

		/*
		 *  Detach from the processor
		 */
		tmpStatus = PROC_detach(processorId);
		if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
			status = tmpStatus;
			RING_IO_1Print("PROC_detach () failed. Status = [0x%x]\n", status);
		}

		/*
		 *  Destroy the PROC object.
		 */
		tmpStatus = PROC_destroy();
		if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
			status = tmpStatus;
			RING_IO_1Print("PROC_destroy () failed. Status = [0x%x]\n", status);
		}

		/*
		 *  OS Finalization
		 */
		//tmpStatus = RING_IO_OS_exit();
		if (DSP_SUCCEEDED(status) && DSP_FAILED(tmpStatus)) {
			status = tmpStatus;
			RING_IO_1Print("RING_IO_OS_exit () failed. Status = [0x%x]\n", status);
		}

#if DEBUG_ENABLED
		RING_IO_0Print("Leaving RING_IO_Delete ()\n");
#endif

	}

	/** ----------------------------------------------------------------------------
	 *  @func   RING_IO_Writer_Notify
	 *
	 *  @desc   This function implements the notification callback for the RingIO
	 *          opened by the GPP in writer  mode.
	 *
	 *  @modif  None
	 *  ----------------------------------------------------------------------------
	 */
	STATIC
	NORMAL_API
	Void RING_IO_Writer_Notify1(IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg)
	{
		DSP_STATUS status = DSP_SOK;

		/* Post the semaphore. */
		status = RING_IO_PostSem ((Pvoid) param);
		if (DSP_FAILED (status)) {
			RING_IO_1Print ("RING_IO_PostSem () failed. Status = [0x%x]\n",
					status);
		}
#if DEBUG_ENABLED
		RING_IO_0Print (" RING_IO_Writer_Notify1  Scuccess \n");
#endif

	}

	STATIC
	NORMAL_API
	Void RING_IO_Writer_Notify2(IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg)
	{
		DSP_STATUS status = DSP_SOK;

		/* Post the semaphore. */
		status = RING_IO_PostSem ((Pvoid) param);
		if (DSP_FAILED (status)) {
			RING_IO_1Print ("RING_IO_PostSem () failed. Status = [0x%x]\n",
					status);
		}
#if DEBUG_ENABLED
		RING_IO_0Print (" RING_IO_Writer_Notify2  Scuccess \n");
#endif
	}

	/** ----------------------------------------------------------------------------
	 *  @func   RING_IO_Reader_Notify
	 *
	 *  @desc   This function implements the notification callback for the RingIO
	 *          opened by the GPP in  reader mode.
	 *
	 *  @modif  None
	 *  ----------------------------------------------------------------------------
	 */
	STATIC
	NORMAL_API
	Void RING_IO_Reader_Notify1 (IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg)
	{
		DSP_STATUS status = DSP_SOK;

#if DEBUG_ENABLED
		RING_IO_1Print ("###RING_IO_Reader_Notify1. (msg) = %d\n",
				msg);
#endif

		switch(msg) {
			case NOTIFY_DATA_START:
			fReaderStart1 = TRUE;
#if DEBUG_ENABLED
			RING_IO_0Print (" RING_IO_Reader_Notify1 Start Scuccess \n");
#endif
		/* Post the semaphore. */
		status = RING_IO_PostSem ((Pvoid) param);
			break;

			case NOTIFY_DATA_END:
			fReaderEnd1 = TRUE;
#if DEBUG_ENABLED
			RING_IO_0Print (" RING_IO_Reader_Notify1 End Scuccess \n");
#endif
		/* Post the semaphore. */
		status = RING_IO_PostSem ((Pvoid) param);
			break;

			default:
			break;
		}


		if (DSP_FAILED (status)) {
			RING_IO_1Print ("RING_IO_PostSem () failed. Status = [0x%x]\n",
					status);
		}
	}

	STATIC
	NORMAL_API
	Void RING_IO_Reader_Notify2 (IN RingIO_Handle handle,
			IN RingIO_NotifyParam param,
			IN RingIO_NotifyMsg msg)
	{
		DSP_STATUS status = DSP_SOK;
#if DEBUG_ENABLED
		RING_IO_1Print ("###RING_IO_Reader_Notify2. (msg) = %d\n",
				msg);
#endif

		switch(msg) {
			case NOTIFY_DATA_START:
			fReaderStart2 = TRUE;
#if DEBUG_ENABLED
			RING_IO_0Print (" RING_IO_Reader_Notify2 Start Scuccess \n");
#endif
			/* Post the semaphore. */
			status = RING_IO_PostSem ((Pvoid) param);
			break;

			case NOTIFY_DATA_END:
			fReaderEnd2 = TRUE;
#if DEBUG_ENABLED
			RING_IO_0Print (" RING_IO_Reader_Notify2 End Scuccess \n");
#endif
			/* Post the semaphore. */
			status = RING_IO_PostSem ((Pvoid) param);
			break;

			default:
			break;
		}

		if (DSP_FAILED (status)) {
			RING_IO_1Print ("RING_IO_PostSem () failed. Status = [0x%x]\n",
					status);
		}
	}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
