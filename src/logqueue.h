#ifndef LOG_QUEUE_FSS_2013_02_13
#define LOG_QUEUE_FSS_2013_02_13

#include "buildintype.h" //lint -e537

#include "singleton.h"
#include "circlebuff.h"

#ifdef _LINUX_
#include "./linux/luxevent.h"
#endif

#ifdef _UTTEST
#include "ut_stub.h"
#endif


namespace space_logapi
{
    class CLogInfo;
    class CSemaphore;
    
    class CLogQueue
    {
        friend class CLogRecorder;

    public:
        CLogQueue();
        ~CLogQueue();

        void Initialize();

        ushort PutLog(const char* pLogBuff, ushort usLen);
        ushort GetLog(char* pLogBuff, ulong& ulLen, ulong ulCacheTime);
        
        bool IsEmpty();

    private:
        CCircleBuff m_CircleBuff;
        CSemaphore* m_pSemaphore;
    };

    typedef CSingleton<CLogQueue> CLogQueueS;

#ifdef _UTTEST 
    const ulong  MAX_QUEUE_BUFF_LEN = 2*1024+1;
#else
    const ulong  MAX_QUEUE_BUFF_LEN = 1024*1024;
#endif
}

#endif
