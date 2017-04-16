#include <stdio.h>

#ifdef _LINUX_
#include <time.h>
#include <errno.h>
#include "./linux/luxevent.h"

#endif

#include "sem.h"
#include "logexception.h"

namespace space_logapi
{
    CSemaphore::CSemaphore() : m_hPutLogEvent(NULL)
    {
#ifdef _LINUX_
        m_hPutLogEvent = new sem_t;

        if(sem_init(m_hPutLogEvent, 0, 0) != 0)
        {
            throw CLogException("create put event faild");
        }

#else
        m_hPutLogEvent = CreateSemaphore(NULL, 0, 0xffff, NULL);
        if (m_hPutLogEvent == NULL)
        {
            throw CLogException("create put event faild");
        }
#endif
    }

    CSemaphore::~CSemaphore()
    {
#ifdef _LINUX_
        sem_destroy(m_hPutLogEvent);

        delete m_hPutLogEvent;
        m_hPutLogEvent = NULL;
#else
        CloseHandle(m_hPutLogEvent);
#endif

    }

    void CSemaphore::Post()
    {
#ifdef _LINUX_
        if(sem_post(m_hPutLogEvent) != 0)
        {
            throw CLogException("Set put event faild");
        }
#else
        if(!ReleaseSemaphore(m_hPutLogEvent, 1, NULL))
        {
            throw CLogException("Set put event faild");
        }
#endif
    }

    bool CSemaphore::Wait(unsigned long ulTime)
    {
#ifdef _LINUX_
        
        timespec CurTime;
        clock_gettime(CLOCK_REALTIME, &CurTime);

        timespec WaitTime;
        CastMilliSecondToTimespec(ulTime, WaitTime);

        timespec OutTime;
        OutTime.tv_nsec = CurTime.tv_nsec + WaitTime.tv_nsec;
        OutTime.tv_sec  = CurTime.tv_sec  + WaitTime.tv_sec;

        if (sem_timedwait(m_hPutLogEvent, &OutTime) != 0)
        {
            return false;
        }

        return true;

#else
        unsigned long ulRst = WaitForSingleObject(m_hPutLogEvent, ulTime);

        switch(ulRst)
        {
        case WAIT_TIMEOUT:
            return false;
        case WAIT_OBJECT_0:
            return true;
        default:
            throw CLogException("Wait put event faild");
        }
#endif

    }
}
