#include <Windows.h>

#include "comm.h"
#include "commdef.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD lp_ulreason_for_call, LPVOID lpReserved)
{
	UNREF_PARAMETER(hModule);
	UNREF_PARAMETER(lp_ulreason_for_call);
	UNREF_PARAMETER(lpReserved);
	return true;
}