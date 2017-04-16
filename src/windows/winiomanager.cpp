#ifdef UTTEST
#include "winsock2_stub.h"
#else
#include <WinSock2.h>
#endif

#include "winiomanager.h"
#include "writelog.h"
#include "commdef.h"


CIOManager* CWinIOManager::CreateInstance()
{
	if (m_pSingleInstance == NULL)
	{
		m_pSingleInstance = new CWinIOManager;
	}

	return m_pSingleInstance;
}


CWinIOManager::CWinIOManager():m_hCompletePort(INVALID_HANDLE_VALUE)
{
}

CWinIOManager::~CWinIOManager()
{
	//�ر�IOCP�˿�
	CloseHandle(m_hCompletePort);	
	m_hCompletePort = INVALID_HANDLE_VALUE;
}

bool CWinIOManager::Initialize()
{ 
	HANDLE hIocp  = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hIocp == INVALID_HANDLE_VALUE)
	{
		WriteLogInfo("Init Completion point failed, errno=%d", GetLastError());
		return false;
	}
	
	m_hCompletePort = hIocp;
	m_pSingleInstance = this;

	WriteLogInfo("Init Completion point success");
	return true;
}

void  CWinIOManager::AssocIOPort(int Socket, ULONG ulAppend)
{
	if (CreateIoCompletionPort((HANDLE)Socket, m_hCompletePort, ulAppend, 0 ) == NULL)
	{
		WriteLogInfo("Assocation Completion point failed, socket=%d, errno=%d", Socket, GetLastError());
	}
}

void CWinIOManager::RequestRead(int Socket, void* pBuff, ULONG ulBuffLen, UCHAR ucCheck)
{
	
	WSABUF RecvBuff;
	RecvBuff.buf = (CHAR *)pBuff;
	RecvBuff.len = ulBuffLen;

	//lint --e{429, 593}
	OVERLAPPED_OPTYPE *pOverlpType = new OVERLAPPED_OPTYPE(IO_EVENT_READ, ucCheck);

	DWORD dwRecv = 0; 
	DWORD dwFlags = 0;

	int iResult = WSARecv(Socket, (LPWSABUF)&RecvBuff, 1, &dwRecv, &dwFlags, (LPWSAOVERLAPPED)pOverlpType, NULL );

	if (iResult!=0 && WSAGetLastError() != WSA_IO_PENDING)
	{
		WriteLogInfo("Post read request failed errno=%d, socket=%d", WSAGetLastError(), Socket);
		delete pOverlpType;
	}
}

void  CWinIOManager::PostQuitEvent()
{
	//lint --e{429, 593}
	OVERLAPPED_OPTYPE *pOverlpType = new OVERLAPPED_OPTYPE(IO_EVENT_THEAD_QUIT, 0);
	if(!PostQueuedCompletionStatus(m_hCompletePort, 0, 0, (LPOVERLAPPED )pOverlpType))
	{
		WriteLogInfo("Post quit event failed errno=%d", GetLastError());
		delete pOverlpType;
	}
}

void  CWinIOManager::WaitIOEventProcess()
{
	while (true)
	{
		ULONG ulTrans = 0; 
		ULONG_PTR pCompletionKey = NULL;
		OVERLAPPED_OPTYPE *pPerIo  = NULL;

		BOOL bResult = GetQueuedCompletionStatus(m_hCompletePort, &ulTrans, &pCompletionKey, (LPOVERLAPPED*)&pPerIo, WSA_INFINITE );

		//ת��IOEVENT
		UCHAR ucIOEventType = UC_NO_USE;
		UCHAR ucCheckCount  = UC_NO_USE;

		if (!bResult || pPerIo == NULL) //����ʧ�ܻ���pPerIoΪ�գ���˵��IO�쳣
		{
			ucIOEventType = IO_EVENT_ERROR;
		}
	
		if (pPerIo != NULL)
		{
			ucIOEventType = pPerIo->ucOpType;
			if (ucIOEventType == IO_EVENT_THEAD_QUIT) //�յ��˳��¼�����ǰ�߳���Ҫ�˳�
			{
				break;
			}

			if( ulTrans == 0 && ucIOEventType == IO_EVENT_READ) //������������£�dwTransΪ0˵���Զ˹ر�����
			{
				ucIOEventType = IO_EVENT_PEER_CLOSE;

				if (WSAGetLastError() == WSA_OPERATION_ABORTED) //����������995ʱ��˵��������ر�socket�����ǶԶ˹ر�
				{
					ucIOEventType = IO_EVENT_ERROR;
				}
			}

			ucCheckCount = pPerIo->ucCheckCount;
		}

		//����IOEVENT����ִ�ж�Ӧע��ĺ���
		MAP_EVENT_TO_FUNC::iterator itermp = m_mpEventToProcess.find(ucIOEventType);
		if (itermp != m_mpEventToProcess.end())
		{
			EVENT_PROCESS EventProcess = itermp->second;
			if (EventProcess != NULL)
			{
				EventProcess((void *)pCompletionKey, (USHORT)ulTrans, ucCheckCount);
			}
		}

		delete pPerIo;
	}
}