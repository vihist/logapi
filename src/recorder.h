#ifndef RECORDER_FSS_2013_02_23
#define RECORDER_FSS_2013_02_23

#include "buildintype.h" //lint -e537

#include "singleton.h"

#ifdef _UTTEST
#include "ut_stub.h"
#endif

namespace space_logapi
{
    class CLogInfo;
    class CLogQueue;
    class COstreamManager;
    class CThread;

    class CLogRecorder
    {
    public:
        CLogRecorder();
        ~CLogRecorder();

        void Initialize(CLogQueue* pLogQueue, COstreamManager* pOstreamManager);

        void StartThread();
        void StopThread();

        static void ThreadFunc(void* pParam);

    private:
        void Record(ulong ulCacheTime);

    private:
        CThread* m_pThread;
        bool m_bExist;

        CLogQueue* m_pLogQueue;
        COstreamManager *m_pOstreamManager;

        ulong m_ulLogBuffLen;
        char  *m_pLogBuff;
    };

    typedef CSingleton<CLogRecorder> CLogRecorderS;
}
#endif
