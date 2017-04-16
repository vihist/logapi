#ifndef COMM_CRITICAL_SECTION_FSS_2013_12_11
#define COMM_CRITICAL_SECTION_FSS_2013_12_11

#ifdef _LINUX_
#include "./linux/luxcritical.h"
#else
#include <Windows.h>
#endif

namespace space_logapi
{
#define AUTO_CRITICAL_SECTION(lock_cs) CAutoCriticalSection lock(lock_cs)

class CLock_CS
{
public:
    CLock_CS(void)
    {
        InitializeCriticalSection(&m_CriticalSection);
    }

    virtual~CLock_CS(void)
    {
        DeleteCriticalSection(&m_CriticalSection);
    }

    void Lock(void)
    {
        EnterCriticalSection(&m_CriticalSection);
    }

    void Unlock(void)
    {
        LeaveCriticalSection(&m_CriticalSection);
    }

private:
    CRITICAL_SECTION m_CriticalSection;
};

// 自动锁
class CAutoCriticalSection
{
public:
	CAutoCriticalSection(CLock_CS & lock):m_lock(lock)
	{
		m_lock.Lock();
	}

	virtual ~CAutoCriticalSection()
	{
		//lint -e{1551}
		m_lock.Unlock();
	}

private:
    CAutoCriticalSection();
	CAutoCriticalSection& operator = (const CAutoCriticalSection& ); //不支持赋值
	CLock_CS & m_lock;
};
}

#endif

