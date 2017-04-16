#ifndef LOGINFO_FSS_2014_02_13
#define LOGINFO_FSS_2014_02_13

#include <string>

//lint -e537
#include "buildintype.h" 

#ifdef _UTTEST
#include "ut_stub.h"
#endif

using std::string;

namespace space_logapi
{
    const ushort MAX_LOGINF_LEN = 1024;
    const uchar  DATE_INFO_LEN = 22;

    class CLogInfo
    {
        friend class CLogQueue;
    public:
        CLogInfo();
        CLogInfo(uchar ucLogLevel, char* pLogDesc);

        bool ToString(string& strLog) const;

        bool IsValid() const;

    private:

    private:
        bool m_bInited;
        uchar m_ucLevel;
        char m_pDateInfo[DATE_INFO_LEN];
        char m_pLogInfo[MAX_LOGINF_LEN];

    private:
    };
}

#endif

