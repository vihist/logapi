#ifndef COMM_LUXIOMANAGER_FSS_2013_12_31
#define COMM_LUXIOMANAGER_FSS_2013_12_31

#include <semaphore.h>
#include <pthread.h>
#include <map>

#include "iomanager.h"

using namespace std;

namespace space_logapi
{
    class CLuxIOManager :public CIOManager
    {
    public:
	    static CIOManager* CreateInstance();

	    bool Initialize();
	    void AssocIOPort(int Socket, ULONG ulAppend);
	    void RequestRead(int Socket, void* pBuff, ULONG ulBuffLen, UCHAR ucCheck);
	    void PostQuitEvent();

    private:
	    CLuxIOManager();
	    ~CLuxIOManager();

	    void WaitIOEventProcess();

	    struct WSBUFF
	    {
		    WSBUFF() : ulLen(0), pBuff(NULL)
		    {
		    }

		    ULONG ulLen;
		    UCHAR *pBuff;
	    };

	    enum ENU_STATE
	    {
		    IDLE,
		    OCCUPY,
		    WORKING,
	    };

	    struct RECV_SEM
	    {
		    int Socket;

		    ULONG ulAppendKey;

		    WSBUFF wsBuff;

		    int  iState;
		    char check;

		    void Initialize();
		    void destroy();
		    void clean();

		    void Post();
		    void Wait();

		    void SetRecvBuff(UCHAR *pBuff, ULONG ulBuffLen);
	    };


	    sem_t Semt;
	    pthread_mutex_t mutex;

	    RECV_SEM m_RecvSemArray[MAX_CONN_NUM];

	    map<int, USHORT> m_mpSockToSemIndex;

	    bool m_bIsExit;
    };

    typedef CLuxIOManager CCurrIOManager;
}

#endif
