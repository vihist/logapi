#pragma warning ( disable : 4996)

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include "buildintype.h"
#include "criticalsection.h"

#include "logexception.h"

#include "logapi.h"
#include "loginfo.h"
#include "logapp.h"

#ifdef _LINUX_
#include <errno.h>

#include "./linux/luxdefine.h"
#include "./linux/directoryop.h"
#endif

using std::cerr;
using std::endl;

using namespace space_logapi;

extern const ushort MAX_FILE_SIZE  = 10*1024;
extern const ushort MIN_FILE_SIZE  = 200;
extern const uchar  MAX_FILE_COUNT = 100;
extern const uchar  MIN_FILE_COUNT = 1;

CLock_CS g_LogApiLock;
CLogApp* g_pLogApp;

void CheckIsInited();
void CheckParamVaild(unsigned short usMaxFileSize, unsigned char ucMaxFileCount);
void CreateLogDirectory(const char* pLogPath);

bool LogInitialize(const char *pLogPath, unsigned short usMaxFileSize, unsigned char ucMaxFileCount)
{
	AUTO_CRITICAL_SECTION(g_LogApiLock);

    try
    {
        if (g_pLogApp != NULL)
        {
            cerr<<"Log Initialize failed, reason: already initialized!"<<endl;
            return false;
        }

        g_pLogApp = new CLogApp();
        g_pLogApp->Initailize(pLogPath, usMaxFileSize, ucMaxFileCount);

        return true;
    }
    catch (exception& excp)
    {
        if (g_pLogApp != NULL)
        {
            g_pLogApp->Exit();

            delete g_pLogApp;
            g_pLogApp = NULL;
        }

        cerr<<"Log Initialize failed, reason: "<<excp.what()<<endl;
        return false;
    }
}

void WriteLogInfo(unsigned char ucLogLevel, char* pLogFormat, ...)
{
    AUTO_CRITICAL_SECTION(g_LogApiLock);

    try
    {
        if (g_pLogApp == NULL)
        {
            throw CLogException("logapi not initialized!");
        }

        char pLogBuff[MAX_LOGINF_LEN];

        va_list argList;
        va_start(argList, pLogFormat);
        (void)vsnprintf(pLogBuff, MAX_LOGINF_LEN, pLogFormat, argList);
        pLogBuff[MAX_LOGINF_LEN-1] = '\0';
        va_end(argList); 

        g_pLogApp->WriteLog(ucLogLevel, pLogBuff);
    }
    catch(exception& excpt)
    {
        cerr<<"WriteLogInfo error! "<<excpt.what()<<endl;
    }
    //lint -e438
}

void LogExit()
{
	AUTO_CRITICAL_SECTION(g_LogApiLock);

    if (g_pLogApp == NULL)
    {
        return;
    }

    g_pLogApp->Exit();

    delete g_pLogApp;
    g_pLogApp = NULL;
}

 bool SetLogLevel(uchar ucLogLevel)
{
	AUTO_CRITICAL_SECTION(g_LogApiLock);

    try
    {
        if (g_pLogApp == NULL)
        {
            throw CLogException("logapi not initialized!");
        }

        return g_pLogApp->SetLogLevel(ucLogLevel);
    }
    catch (exception& excpt)
    {
        cerr<<"set log level error! "<<excpt.what()<<endl;
    	return false;
    }

}
