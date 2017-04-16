#ifndef OUT_FILE_STREAM_MANAGER_FSS_2014_02_17
#define OUT_FILE_STREAM_MANAGER_FSS_2014_02_17

#include <string>
#include <fstream>

#include "singleton.h"

#include "buildintype.h" //lint -e537

#include "ostreammanager.h"

#ifdef _UTTEST
#include "ut_stub.h"
#include "ofstream_stub.h"
#else
using std::ofstream;
#endif

using std::string;

namespace space_logapi
{
    const ushort MAX_LOG_PATH_SIZE = 1024;

    class COfstreamManager : public COstreamManager
    {
    public:
        COfstreamManager();
        ~COfstreamManager();

        void Initialize(const char *pLogPath, ushort usMaxFileSize, uchar ucMaxFileCount);

        ostream& GetCurrOutStream(ulong usLogInfoLen);

    private:
        void InitLogPathAndFileName(const char* pLogFullPath);
        void InitCurrFileInfo();

        void GetLogFullPathExt(ulong ulFileNO, string& strLogFullPathExt) const;

        void RemoveOldFiles();
        void CreateNewFile();

        void UpdateFileNO();

    private:
        string m_strLogPath;
        string m_strFileName;
        string m_strIniFileName;

        ulong  m_ulMaxFileSize;
        uchar  m_ucMaxFileCount;

        ulong  m_ulCurrFileNO;
        ulong  m_ulCurrFileSize;

        ofstream m_OutFileStream;
    };

    typedef CSingleton<COfstreamManager> COfstreamManagerS;
}
#endif


