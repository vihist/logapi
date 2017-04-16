#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>

#include "luxiomanager.h"

namespace space_logapi
{

    CLuxIOManager::CLuxIOManager():m_bIsExit(false)
    {
	    //对对端已经close的socket进行写操作，linux会抛出SIGPIPE异常导致进程退出，此处屏蔽
	    signal(SIGPIPE, SIG_IGN);
    }

    CLuxIOManager::~CLuxIOManager()
    {
	    for (int i=0; i<MAX_CONN_NUM; i++)
	    {
		    m_RecvSemArray[i].clean();
	    }

	    sem_destroy(&Semt);
    }

    CIOManager* CLuxIOManager::CreateInstance()
    {
	    if (m_pSingleInstance == NULL)
	    {
		    m_pSingleInstance = new CLuxIOManager;
	    }

	    return m_pSingleInstance;
    }


    bool CLuxIOManager::Initialize()
    {
	    for (int i=0; i<MAX_CONN_NUM; i++)
	    {
		    m_RecvSemArray[i].Initialize();
	    }
	
	    sem_init(&Semt, 0, 0);
	    pthread_mutex_init(&mutex, 0);
	
	    return true;
	
    }

    void CLuxIOManager::AssocIOPort(int Socket, ULONG ulAppend)
    {
	    pthread_mutex_lock(&mutex);
	
	    for (int i=0; i<MAX_CONN_NUM; i++)
	    {
		    if(m_RecvSemArray[i].iState == IDLE)
		    {
			    m_RecvSemArray[i].iState = OCCUPY;
			    m_RecvSemArray[i].Socket = Socket;
			    m_RecvSemArray[i].ulAppendKey = ulAppend;
			    m_RecvSemArray[i].check =-1;
			    m_mpSockToSemIndex.insert(make_pair(Socket, i));
			    break;
		    }
	    }
	
	    pthread_mutex_unlock(&mutex);
    }

    void CLuxIOManager::RequestRead(int Socket, void* pBuff, ULONG ulBuffLen, UCHAR ucCheck)
    {
	    //查找到当前未触发的信号量，若查找不到，直接返回失败
	    //查找成功，设置对应数据信息，触发信号量。
	    map<int, USHORT>::iterator itermp = m_mpSockToSemIndex.find(Socket);
	    if (itermp  == m_mpSockToSemIndex.end())
	    {
		    return;
	    }

	    USHORT usIndex = itermp->second;
	    if (usIndex > MAX_CONN_NUM)
	    {
		    return;
	    }
	    pthread_mutex_lock(&mutex);
	
	    m_RecvSemArray[usIndex].SetRecvBuff((UCHAR *)pBuff, ulBuffLen);
	    m_RecvSemArray[usIndex].check = ucCheck;
	    pthread_mutex_unlock(&mutex);
	
	    sem_post(&Semt);
    }

    void CLuxIOManager::PostQuitEvent()
    {
	    m_bIsExit = true;
	    sem_post(&Semt);

	    for (int i=0; i<MAX_CONN_NUM; i++)
	    {
		    m_RecvSemArray[i].clean();
	    }
    }

    void CLuxIOManager::WaitIOEventProcess()
    {
	    //等待信号量，获取对应的信息，执行。
	    //此处如果错误码是EINTR，则说明是系统中断引起（一般gdb调试会触发），并不是信号量有效了。则需要继续等待，
	    while(sem_wait(&Semt) != 0 && errno == EINTR)
	    {
	    };

	    if (m_bIsExit == true)
	    {
		    return;
	    }

	    pthread_mutex_lock(&mutex);

	    int i=0;
	    for (; i<MAX_CONN_NUM; i++)
	    {
		    if(m_RecvSemArray[i].iState == OCCUPY)
		    {
			    m_RecvSemArray[i].iState = WORKING;
			    break;
		    }
	    }
	    pthread_mutex_unlock(&mutex);
	
	    if(i == MAX_CONN_NUM)
	    {
		    return;
	    }
	
	    USHORT usCurrIndex = i;
	

	    fd_set FdRead;

	    while (true) 	
	    {
		    FD_ZERO(&FdRead);
		    FD_SET(m_RecvSemArray[usCurrIndex].Socket, &FdRead);

		    if(select(m_RecvSemArray[usCurrIndex].Socket+1, &FdRead, NULL, NULL, NULL) == -1)
		    {
			    if (m_bIsExit)
			    {
				    return;
			    }

			    break;
		    }
		
		    int iTrans = recv(m_RecvSemArray[usCurrIndex].Socket, m_RecvSemArray[usCurrIndex].wsBuff.pBuff,  m_RecvSemArray[usCurrIndex].wsBuff.ulLen, 0);

		    UCHAR ucEventType = IO_EVENT_READ;
		    if (iTrans == -1)
		    {
			    if (errno == ECONNRESET)
			    {
				    ucEventType = IO_EVENT_PEER_CLOSE;
			    }
			    else
			    {
				    ucEventType = IO_EVENT_ERROR;
			    }
		    }

		    //根据IOEVENT类型执行对应注册的函数
		    MAP_EVENT_TO_FUNC::iterator itermp = m_mpEventToProcess.find(ucEventType);
		    if (itermp != m_mpEventToProcess.end())
		    {
			    EVENT_PROCESS EventProcess = itermp->second;
			    if (EventProcess != NULL)
			    {
				    EventProcess((void *)m_RecvSemArray[usCurrIndex].ulAppendKey, (USHORT)iTrans, m_RecvSemArray[usCurrIndex].check);
			    }
		    }

		
		    if (ucEventType == IO_EVENT_ERROR || ucEventType == IO_EVENT_PEER_CLOSE)
		    {
			    m_RecvSemArray[usCurrIndex].clean();
			    return;
		    }
	    }
    }

    void CLuxIOManager::RECV_SEM::Initialize()
    {
	    Socket = -1;
	    ulAppendKey = 0;
	    iState = IDLE;
    }

    void CLuxIOManager::RECV_SEM::clean()
    {

	    Socket = -1;

	    ulAppendKey = 0;
	    wsBuff.pBuff = NULL;
	    wsBuff.ulLen = 0;
	    iState = IDLE;
    }

    void CLuxIOManager::RECV_SEM::SetRecvBuff(UCHAR *pBuff, ULONG ulBuffLen)
    {
	    wsBuff.pBuff = pBuff;
	    wsBuff.ulLen = ulBuffLen;
    }
}