#ifndef COMM_IOCPMANAGER_FSS_2013_12_21
#define COMM_IOCPMANAGER_FSS_2013_12_21


#include "iomanager.h"

using namespace std;

class CWinIOManager :public CIOManager
{
public:
	static CIOManager* CreateInstance();

	bool Initialize();
	void AssocIOPort(int Socket, ULONG ulAppend);
	void RequestRead(int Socket, void* pBuff, ULONG ulBuffLen, UCHAR ucCheck);
	void PostQuitEvent();

private:
	CWinIOManager();
	~CWinIOManager();

	void WaitIOEventProcess();

	HANDLE m_hCompletePort;
};

typedef CWinIOManager CCurrIOManager;

struct OVERLAPPED_OPTYPE
{
	OVERLAPPED_OPTYPE(UCHAR ucType, UCHAR ucCheck):ucOpType(ucType), ucCheckCount(ucCheck)
	{
		memset(&Overlp, 0x00, sizeof(Overlp));
	}

	OVERLAPPED Overlp;
	UCHAR	   ucOpType; //IO_EVENT_OPTYPE
	UCHAR	   ucCheckCount;
};
#endif