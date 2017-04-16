#ifndef COMM_LUXTHREAD_FSS_2013_12_31
#define COMM_LUXTHREAD_FSS_2013_12_31

#include "../thread.h"

namespace space_logapi
{
    class CLuxThread : public CThread
    {
    public:
	    CLuxThread();
	    ~CLuxThread();

	    bool Initialize(PROCESS_FUNC pProcessFunc, void* pProcessParam, NOTIFY_END pNotifyEndFunc);
	    bool StartThread();
	    void WaitEnd();

    private:
	    static void* ThreadFunc(void*  pParam);

	    unsigned long  m_ulThreadId;
    };

    typedef CLuxThread CCurrPlatformThread;
}
#endif
