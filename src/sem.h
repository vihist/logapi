#ifdef _LINUX_
#include <semaphore.h>
typedef sem_t* SEM_HANDLE;
#else
#include <Windows.h>
typedef HANDLE SEM_HANDLE;
#endif

namespace space_logapi
{
    class CSemaphore
    {
    public:
        CSemaphore();
        ~CSemaphore();

        void Post();
        bool Wait(unsigned long ulTime);

    private:
        SEM_HANDLE m_hPutLogEvent;
    };
}