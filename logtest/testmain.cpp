#include <stdio.h>

#ifndef _LINUX_
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "logapi.h"

void Log_Sleep(unsigned long ulTime)
{
#ifndef _LINUX_
    Sleep(ulTime);
#else
    sleep(ulTime/1000);
#endif
}

void WriteAllLevelInfo()
{
    WriteLogInfo(LEVEL_DEBU, "loginfo=%s, index=%d", "this is debug", 0);
    WriteLogInfo(LEVEL_INFO,  "loginfo=%s, index=%d", "this is info", 0);
    WriteLogInfo(LEVEL_WARN,  "loginfo=%s, index=%d", "this is warn", 0);
    WriteLogInfo(LEVEL_ERRO,  "loginfo=%s, index=%d", "this is error", 0);
}

int main(int argc, char* argv[])
{
    LogInitialize(argv[0], 1024, 10);

    WriteLogInfo(LEVEL_ERRO, "use default level");

    for (int i=0; i<3000; i++)
    {
        WriteAllLevelInfo();
    }
	
//     SetLogLevel(LEVEL_WARN);
//     WriteLogInfo(LEVEL_ERRO, "use warn level");
//  
//     for (int i=0; i<3000; i++)
//     {
//         WriteAllLevelInfo();
//     }
//     
// 	Log_Sleep(1000);
//  
//     SetLogLevel(LEVEL_INFO);
//     WriteLogInfo(LEVEL_ERRO, "use info level");
// 
//     for (int i=0; i<3000; i++)
//     {
//         WriteAllLevelInfo();
//     }
// 
// 	Log_Sleep(1000);
// 
//     SetLogLevel(LEVEL_DEBU);
//     WriteLogInfo(LEVEL_ERRO, "use debug level");
// 
//     for (int i=0; i<3000; i++)
//     {
//         WriteAllLevelInfo();
//     }

	Log_Sleep(10*1000);

//    for (int i=0; i<5000; i++)
//     {
//         SetLogLevel(LOG_DEBU);
//         WriteLogInfo(LOG_ERRO, "use debug level");
// 
//         WriteAllLevelInfo();
//     }

    LogExit();
}