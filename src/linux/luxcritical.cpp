#include "luxcritical.h"

namespace space_logapi
{
    void InitializeCriticalSection(pthread_mutex_t*  pmutex)
    {
	    pthread_mutexattr_t mutexattr;
	    pthread_mutexattr_init(&mutexattr);
	
	    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
	
	    pthread_mutex_init(pmutex, &mutexattr);
    };

    void DeleteCriticalSection(pthread_mutex_t*  pmutex)
    {
	    pthread_mutex_destroy(pmutex);
    }

    void EnterCriticalSection(pthread_mutex_t*  pmutex)
    {
	    pthread_mutex_lock(pmutex);
    };

    void LeaveCriticalSection(pthread_mutex_t*  pmutex)
    {
	    pthread_mutex_unlock(pmutex);
    }
}