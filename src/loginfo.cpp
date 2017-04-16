#pragma warning ( disable : 4996)
#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifdef _LINUX_
#include <sys/time.h>
#else
#include <Windows.h>
#endif

#include "loginfo.h"

using std::string;
using std::ostringstream;
using std::setw;

namespace space_logapi
{

    char* g_LevelDesc[] = {
        "ERRO",
        "WARN",
        "INFO",
        "DEBU"
    };

    void CreateTimeString(char* pDateInfo, int usLen)
    {
        if (pDateInfo == NULL)
        {
            return;
        }

        tm tm_time;

        time_t long_time;
        (void)time( &long_time ); 

        ulong ulMillSecond;

#ifndef _LINUX_
        (void)localtime_s(&tm_time, &long_time);

        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        ulMillSecond = sysTime.wMilliseconds;
#else
        localtime_r(&long_time, &tm_time);

        timeval tv;
        gettimeofday(&tv, NULL);
        ulMillSecond = tv.tv_usec/1000;
#endif


        ostringstream strTimeLogStream;
        (void)strTimeLogStream.fill('0');

        strTimeLogStream << setw(4) << 1900+tm_time.tm_year
            << setw(2) << 1+tm_time.tm_mon
            << setw(2) << tm_time.tm_mday
            << ' '
            << setw(2) << tm_time.tm_hour  << ':'
            << setw(2) << tm_time.tm_min   << ':'
            << setw(2) << tm_time.tm_sec   << ':'
            << setw(3) << ulMillSecond     << ' ';

        const string& strTimeLog = strTimeLogStream.str();

        strncpy(pDateInfo, strTimeLog.c_str(), usLen);
        pDateInfo[DATE_INFO_LEN - 1] = '\0';
    }

    CLogInfo::CLogInfo(uchar ucLogLevel, char* pLogDesc): m_ucLevel(ucLogLevel), m_bInited(true)
    {
        CreateTimeString(m_pDateInfo, DATE_INFO_LEN);

        strncpy(m_pLogInfo, pLogDesc, MAX_LOGINF_LEN);
    }

    CLogInfo::CLogInfo():m_bInited(false),m_ucLevel(0)
    {
        m_pDateInfo[0] = '\0';
        m_pLogInfo[0]  = '\0';
    }

    bool CLogInfo::ToString(string& strLog) const
    {
        if (!IsValid())
        {
            return false;
        }

        ostringstream strTimeLogStream;
        (void)strTimeLogStream.fill('0');

        strTimeLogStream << m_pDateInfo
            << " "
            << "["
            << g_LevelDesc[m_ucLevel]
        << "]"
            << " "
            << m_pLogInfo
#ifndef _LINUX_
            << "\r"  //win下ascii文件换行符号前有回车符。若字符串中没有回车符，win也会自动添加的，但是会造成按照COfstreamManager::m_ulCurrFileSize不准
#endif
            << "\n";

        strLog = strTimeLogStream.str();

        return true;
    }

    bool CLogInfo::IsValid() const
    {
        return m_bInited;
    }
}