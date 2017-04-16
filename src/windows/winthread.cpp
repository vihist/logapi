#include <process.h>
#include <Windows.h>

#include "winthread.h"

namespace space_logapi
{
    bool CWinThread::Initialize(PROCESS_FUNC pProcessFunc, void* pProcessParam, NOTIFY_END pNotifyEndFunc)
    {
        m_pProcessFunc	 = pProcessFunc;
        m_pNotifyEndFunc = pNotifyEndFunc;
        m_pProcessParam  = pProcessParam;

        return true;
    }

    bool CWinThread::StartThread()
    {
        HANDLE hThread  = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc, this, 0, NULL);
        if (hThread == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        m_hThread = (unsigned int)hThread;

        return true;
    }


    void CWinThread::WaitEnd()
    {
        (void)WaitForSingleObject((HANDLE)m_hThread, INFINITE);
    }

    unsigned int __stdcall CWinThread::ThreadFunc(void*  lpParam)
    {
        CWinThread *pSelf = static_cast<CWinThread *>(lpParam); 
        if( pSelf == NULL || pSelf->m_pProcessFunc == NULL) 
        { 
            return 1; 
        }

        pSelf->m_pProcessFunc(pSelf->m_pProcessParam);

        return 0;
    }
}