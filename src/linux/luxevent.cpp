#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "luxevent.h"

namespace space_logapi
{

    struct THANDLE
    {
	    pthread_mutex_t mtx;
	    pthread_cond_t cond;
	    bool manual_reset;
	    bool signaled;
    };


    void CastMilliSecondToTimespec(unsigned long ulMillisecond, timespec& TimeSpec)
    {
	    TimeSpec.tv_sec  = ulMillisecond/1000;
	    TimeSpec.tv_nsec = (ulMillisecond%1000) * 1000*1000;
    }

    timespec operator+(const timespec& right, const timespec& left)
    {
	    unsigned long ulNanoSecond = right.tv_nsec + left.tv_nsec;

	    timespec result;
	    result.tv_sec  = right.tv_sec + left.tv_sec + ulNanoSecond /(1000*1000*1000);
	    result.tv_nsec = ulNanoSecond % (1000*1000*1000);

	    return result;
    }

    HANDLE CreateEvent(void *pSec,bool bManualReset,bool bInitialState,char *pStr)
    {
	    HANDLE handle = (HANDLE)malloc(sizeof(THANDLE));
	    pthread_cond_init(&handle->cond, 0);
	    pthread_mutex_init(&handle->mtx,NULL);
	    handle->manual_reset = bManualReset;
	    handle->signaled = bInitialState;
	    return handle;
    }

    void CloseHandle(HANDLE handle)
    {
	    if(NULL == handle)
	    {
		    return;
	    }

	    int i = pthread_mutex_destroy(&handle->mtx);
	    pthread_cond_destroy(&handle->cond);
	    free(handle);
    }

    void SetEvent(HANDLE handle)
    {
	    if(NULL == handle)
	    {
		    return;
	    }

	    pthread_mutex_lock(&handle->mtx);

	    if(handle->manual_reset)
	    {
		    pthread_cond_broadcast(&handle->cond);
	    }

	    else
	    {
		    pthread_cond_signal(&handle->cond);
	    }

	    handle->signaled = true;

	    pthread_mutex_unlock(&handle->mtx);
    }

    void ResetEvent(HANDLE handle)
    {
	    if(NULL == handle)
	    {
		    return;
	    }

	    pthread_mutex_lock(&handle->mtx);

	    handle->signaled = false;

	    pthread_mutex_unlock(&handle->mtx);
    }

    int WaitForSingleObject(HANDLE handle,unsigned int timeout)
    {
	    if(NULL == handle)
	    {
		    return 0;
	    }

	    pthread_mutex_lock(&handle->mtx);

	    int ret = 0;
	    if(!handle->signaled)
	    {
		    if(timeout == 0) 
		    {
			    ret = 1;
			    //WAIT_TIMEOUT;
		    }
		    else if (timeout == INFINITE)
		    {
			    while (!ret && !handle->signaled)
			    {
				    ret = pthread_cond_wait(&handle->cond, &handle->mtx);
			    }	
		    }
		    else
		    {
			    timespec tlast;
			    CastMilliSecondToTimespec(timeout, tlast);

			    timespec tnow;
			    clock_gettime(CLOCK_REALTIME, &tnow);

			    timespec tend = tnow+tlast;
			    while(!ret && !handle->signaled)
			    {
				    ret = pthread_cond_timedwait(&handle->cond, &handle->mtx, &tend);
			    }
		    }
	    }


	    int iResult = WAIT_TIMEOUT;
	    if (ret == 0)
	    {
		    if (!handle->manual_reset)
		    {
			    handle->signaled = false;
		    }

		    iResult = WAIT_OBJECT_0;
	    }

	    pthread_mutex_unlock(&handle->mtx);

	    return iResult;
    }
}