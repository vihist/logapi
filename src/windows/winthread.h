#ifndef COMM_CWINTHREAD_FSS_2013_12_29
#define COMM_CWINTHREAD_FSS_2013_12_29

#include "../thread.h"

namespace space_logapi
{
    class CWinThread : public CThread
    {
    public:
        bool Initialize(PROCESS_FUNC pProcessFunc, void* pProcessParam, NOTIFY_END pNotifyEndFunc);
        bool StartThread();
        void WaitEnd();

    private:
        static unsigned int __stdcall ThreadFunc(void*  pParam);

        unsigned int m_hThread;
    };

    typedef CWinThread CCurrPlatformThread;
}


#endif
