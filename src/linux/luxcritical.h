#ifndef COMM_LUXCRITICAL_FSS_2013_12_31
#define COMM_LUXCRITICAL_FSS_2013_12_31

#include <pthread.h>

namespace space_logapi
{
    typedef pthread_mutex_t CRITICAL_SECTION;

    void InitializeCriticalSection(pthread_mutex_t*  pmutex);

    void DeleteCriticalSection(pthread_mutex_t*  pmutex);

    void EnterCriticalSection(pthread_mutex_t*  pmutex);

    void LeaveCriticalSection(pthread_mutex_t*  pmutex);
}

#endif

