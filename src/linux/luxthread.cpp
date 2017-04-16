#include <pthread.h>
#include <unistd.h>

#include "luxthread.h"

namespace space_logapi
{
    CLuxThread::CLuxThread() : CThread(),m_ulThreadId(~0)
    {

    }

    CLuxThread::~CLuxThread()
    {
	    WaitEnd();
    }

    bool CLuxThread::Initialize(PROCESS_FUNC pProcessFunc, void* pProcessParam, NOTIFY_END pNotifyEndFunc)
    {
	    m_pProcessFunc	 = pProcessFunc;
	    m_pNotifyEndFunc = pNotifyEndFunc;
	    m_pProcessParam  = pProcessParam;
	    return true;
    }

    bool CLuxThread::StartThread()
    {
	    pthread_t ThreadId;
	    if(pthread_create(&ThreadId, NULL, ThreadFunc, this) != 0)
	    {
		    return false;
	    }

	    m_ulThreadId = ThreadId;

	    return true;
    }


    void CLuxThread::WaitEnd()
    {
	    if (m_ulThreadId == ~0)
	    {
		    return;
	    }
	    pthread_join(m_ulThreadId, NULL);
    }

    void *CLuxThread::ThreadFunc(void*  lpParam)
    {
	    CLuxThread *pSelf = static_cast<CLuxThread *>(lpParam); 
	    if( pSelf == NULL || pSelf->m_pProcessFunc == NULL) 
	    { 
		    return NULL; 
	    }

	    pSelf->m_pProcessFunc(pSelf->m_pProcessParam);

	    return NULL;
    }
}
