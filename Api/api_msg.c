

#include "api_msg.h"

#if 0

typedef enum
{
/* -------------- BEGIN CUSTOM AREA [060] utilExecutiveAPI.h -----------------
 * CONFIGURATION: Add message ques here */
  mqUSER_INPUT, // appUserinterface components Consumer only
/* ---------------------------- END CUSTOM AREA --------------------------- */
  mqINVALID                          // Used for enumerated type range
                                       // checking (DO NOT REMOVE)
} eMESSAGE_QUEUES;

typedef struct
{
    BYTE                cWriteIndex;   // index where last entry was written
                                       // to queue
    BYTE                cReadIndex;    // index where next entry will be read
                                       // from queue
    BYTE                cQueueEmpty;   // flag indicating that queue is empty
} MSG_QUEUE_INFO, *PMSG_QUEUE_INFO;


typedef struct
{
    BYTE                cQLength;      // number of messages in queue
    BYTE                cMsgSize;      // message size in bytes
    WORD                wOffset;       // start of queue in memory
} MSG_QUEUE_CONFIG;
typedef const MSG_QUEUE_CONFIG *PCMSG_QUEUE_CONFIG;

typedef struct
{
    eKEY                eKey;          // user input event
    BYTE                cCount;        // event repeat count
} KEY_EVENT, *PKEY_EVENT;

// --------------------------------CONFIGURATION------------------------------
// Size of messages for each message queue (bytes)
// --------------------------------CONFIGURATION------------------------------
#define USER_INPUT_MSG_SIZE 	sizeof(KEY_EVENT)		
// --------------------------------CONFIGURATION------------------------------
// Number of messages in each message queue
// --------------------------------CONFIGURATION------------------------------
#define USER_INPUT_QUEUE_MSGS 	100 // was 5, to avoid Queue Full by encoder rotation

typedef struct
{
/* --------------------------- BEGIN CUSTOM AREA  --------------------------
 * CONFIGURATION: Structure definition for memory used to store message queues.
 * Each queue should be defined as a separate byte array in the structure. */
  BYTE acUserInput[USER_INPUT_QUEUE_MSGS * USER_INPUT_MSG_SIZE];
/* ---------------------------- END CUSTOM AREA --------------------------- */
} MSG_QUEUES;

static BYTE m_acMsgQueues[sizeof(MSG_QUEUES)];

static MSG_QUEUE_INFO   m_asQInfo[mqINVALID];

void utilExecMsgSend(eMESSAGE_QUEUES eQueue,
                     *uint8_t           pacData)
{
  PMSG_QUEUE_INFO     psQInfo = &m_asQInfo[eQueue];
  PCMSG_QUEUE_CONFIG  psQCfg = &m_asQConfig[eQueue];

  // validate parameters in debug mode only
  ASSERT(eQueue < mqINVALID);
  ASSERT(pacData != NULL);

  if (psQInfo->cQueueEmpty)
  {
    psQInfo->cQueueEmpty = FALSE;
  }
  else
  {
    psQInfo->cWriteIndex =
        (psQInfo->cWriteIndex == psQCfg->cQLength - 1) ?
        0 : psQInfo->cWriteIndex + 1;
    ASSERT(psQInfo->cWriteIndex != psQInfo->cReadIndex);
  }

  memcpy(&m_acMsgQueues[psQCfg->wOffset + psQCfg->cMsgSize * psQInfo->cWriteIndex],
                 pacData,
                 psQCfg->cMsgSize);
  
}


eRESULT utilExecMsgRead(eMESSAGE_QUEUES eQueue,
                        *uint8_t           pacData)
{
  BYTE                cDepth;
  PMSG_QUEUE_INFO     psQInfo = &m_asQInfo[eQueue];
  PCMSG_QUEUE_CONFIG  psQCfg = &m_asQConfig[eQueue];

  // validate parameters in debug mode only
  ASSERT(eQueue < mqINVALID);
  ASSERT(pacData != NULL);

  if (psQInfo->cWriteIndex == psQInfo->cReadIndex)
  {
    cDepth = psQInfo->cQueueEmpty ? 0 : 1;
  }
  else
  {
    cDepth = (psQInfo->cWriteIndex > psQInfo->cReadIndex) ?
        psQInfo->cWriteIndex - psQInfo->cReadIndex :
        psQInfo->cWriteIndex - psQInfo->cReadIndex + psQCfg->cQLength + 1;
  }

  if (cDepth)
  {
    memcpy(pacData,
                   &m_acMsgQueues[psQCfg->wOffset + psQCfg->cMsgSize * psQInfo->cReadIndex],
                   psQCfg->cMsgSize);

    if (psQInfo->cReadIndex == psQInfo->cWriteIndex)
    {
      psQInfo->cQueueEmpty = TRUE;
    }
    else
    {
      psQInfo->cReadIndex = (psQInfo->cReadIndex == psQCfg->cQLength - 1) 
                                                         ? 0 : psQInfo->cReadIndex + 1;
    }
  }

  return cDepth ? rcSUCCESS : rcERROR;
}

/**
 *  @brief      This API clears all messages from the specified queue.
                    
 *  @param[in]  eQueue : Queue to clear
 *  @param[out] None
 *  @return     None
 *  @note       None
 *  @attention  None
 */
void utilExecMsgClear(eMESSAGE_QUEUES eQueue)
{
    PMSG_QUEUE_INFO     psQInfo = &m_asQInfo[eQueue];

    // validate parameters in debug mode only
    ASSERT(eQueue < mqINVALID);

    psQInfo->cReadIndex = 0;
    psQInfo->cWriteIndex = 0;
    psQInfo->cQueueEmpty = TRUE;
}

/**
 *  @brief      This API checks if a message is present in the specified queue.
                    
 *  @param[in]  eQueue : Queue to check for messages
 *  @param[out] None
 *  @return     TRUE if there is a message in the queue, FALSE otherwise
 *  @note       None
 *  @attention  None
 */
BOOL utilExecMsgPresent(eMESSAGE_QUEUES eQueue)
{
    // validate parameters in debug mode only
    ASSERT(eQueue < mqINVALID);

    return !m_asQInfo[eQueue].cQueueEmpty;
}

/**
 *  @brief      This API suspends execution of the caller until a message is received in
                    the specified queue.  The caller is suspended in a loop (busy wait).
                    
 *  @param[in]  eQueue  : Queue to send message to
                        wTicks  : Time to wait for message before returning                        
 *  @param[out] pacData : Pointer to message data
 *  @return     rcSUCCESS if a message was read, rcERROR if queue was empty
 *  @note       None
 *  @attention  None
 */
eRESULT utilExecMsgWait(eMESSAGE_QUEUES eQueue,
                        WORD            wTicks,
                        *uint8_t           pacData)
{
  eRESULT             eResult = rcERROR;
  PMSG_QUEUE_INFO     psQInfo = &m_asQInfo[eQueue];
  PCMSG_QUEUE_CONFIG  psQCfg = &m_asQConfig[eQueue];
  DWORD                dwStartTime, dwEndTime, dwTime;

  // validate parameters in debug mode only
  ASSERT(eQueue < mqINVALID);
  ASSERT(pacData != NULL);

  // compute HW timer value after delay
  dwStartTime = TICK_GET;
#if USE_DOWN_SYSTEM_TICK    
  dwEndTime = dwStartTime - (wTicks + 1);
#else
  if(wTicks)
    dwEndTime = dwStartTime + (wTicks - 1);
  else
    dwEndTime = dwStartTime  + 1;
#endif

  // loop until the que is not empty or the time expires.
  while (TRUE)
  {
    if (!(psQInfo->cQueueEmpty))
    {
        break;
    }
      
    dwTime = TICK_GET;
      
#if USE_DOWN_SYSTEM_TICK        
    if (dwEndTime < dwStartTime && 
        !(dwTime <= dwStartTime && 
          dwTime > dwEndTime))
    {
      break;
    }
    if (dwEndTime >= dwStartTime && 
        !(dwTime <= dwStartTime ||
           dwTime > dwEndTime))
    {
      break;
    }
#else
    if(dwEndTime < dwStartTime &&
        dwTime < dwStartTime && dwTime >= dwEndTime)
    {
      break;
    }

    if(dwEndTime >= dwStartTime &&
        dwTime > dwStartTime && dwTime >= dwEndTime)
    {
      break;
    }
#endif
  }
    
  if (!psQInfo->cQueueEmpty)
  {
    memcpy(pacData, &m_acMsgQueues[psQCfg->wOffset + psQCfg->cMsgSize * psQInfo->cReadIndex],
                   psQCfg->cMsgSize);
    eResult = rcSUCCESS;
  }

    return eResult;
}
#endif

