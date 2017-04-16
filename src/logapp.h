#include <string>

#include "criticalsection.h"
#include "buildintype.h"

using std::string;

namespace space_logapi
{
    class COfstreamManager;
    class CLogQueue;
    class CLogRecorder;

    class CLogApp
    {
    public:
        CLogApp();
        ~CLogApp();

        void Initailize(const char *pLogPath, unsigned short usMaxFileSize, unsigned char ucMaxFileCount);
        void Exit();

        void WriteLog(unsigned char ucLogLevel, char* pstrLog);
        bool SetLogLevel(unsigned char ucLevel);

        string GetLogPath();

    private:
        void CreateLogDirectory(const char *pLogPath);
        void CheckParamVaild(unsigned short usMaxFileSize, unsigned char ucMaxFileCount);

    private:
        COfstreamManager* m_pOfstreamManager;
        CLogQueue*        m_pLogQueue;
        CLogRecorder*     m_pLogRecorder;

        unsigned char     m_ucCurrLogLevel;

        string            m_strLogPath;


        static const ushort MAX_FILE_SIZE  = 10*1024;
        static const ushort MIN_FILE_SIZE  = 200;
        static const uchar  MAX_FILE_COUNT = 100;
        static const uchar  MIN_FILE_COUNT = 1;
    };
}