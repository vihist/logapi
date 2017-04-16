#ifndef COMM_LUXEVENT_FSS_2013_12_31
#define COMM_LUXEVENT_FSS_2013_12_31

#include <time.h>

namespace space_logapi
{
    struct  THANDLE;
    typedef THANDLE* HANDLE;

    #define INFINITE 0xFFFFFFFF
    #define WAIT_TIMEOUT 0x00000102L
    #define WAIT_OBJECT_0 0


    HANDLE CreateEvent(void *pSec,bool bManualReset,bool bInitialState,char *pStr);

    void   CloseHandle(HANDLE handle);

    void   SetEvent(HANDLE handle);

    void   ResetEvent(HANDLE handle);

    int    WaitForSingleObject(HANDLE handle,unsigned int timeout);

    void CastMilliSecondToTimespec(unsigned long ulMillisecond, timespec& TimeSpec);
}

#endif
