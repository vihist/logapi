#include "logapp.h"

#ifdef _LINUX_
#include <errno.h>
#include <string.h>

#include "./linux/luxdefine.h"
#include "./linux/directoryop.h"
#endif

#include "logapi.h"
#include "logqueue.h"
#include "loginfo.h"
#include "recorder.h"
#include "ofstreammanager.h"
#include "logexception.h"

namespace space_logapi
{


    CLogApp::CLogApp() : m_pOfstreamManager(NULL), m_pLogQueue(NULL), m_pLogRecorder(NULL), m_ucCurrLogLevel(LEVEL_INFO)
    {

    }

    CLogApp::~CLogApp()
    {

    }

    void CLogApp::Initailize(const char *pLogPath, unsigned short usMaxFileSize, unsigned char ucMaxFileCount)
    {
        CheckParamVaild(usMaxFileSize, ucMaxFileCount);

        CreateLogDirectory(pLogPath);

        m_strLogPath       = pLogPath;
        m_pOfstreamManager = new COfstreamManager;
        m_pLogQueue        = new CLogQueue;
        m_pLogRecorder     = new CLogRecorder;

        m_pOfstreamManager->Initialize(pLogPath, usMaxFileSize, ucMaxFileCount);
        m_pLogQueue->Initialize();
        m_pLogRecorder->Initialize(m_pLogQueue, m_pOfstreamManager);

        m_pLogRecorder->StartThread();
    }

    void CLogApp::Exit()
    {
        if (m_pLogRecorder != NULL)
        {
            m_pLogRecorder->StopThread();

            delete m_pLogRecorder;
            m_pLogRecorder = NULL;
        }

        delete m_pLogQueue;
        m_pLogQueue = NULL;

        delete m_pOfstreamManager;
        m_pOfstreamManager = NULL;
    }

    void CLogApp::WriteLog(unsigned char ucLogLevel, char* pstrLog)
    {

        if (ucLogLevel > m_ucCurrLogLevel)
        {
            return;
        }

        CLogInfo  LogInfo(ucLogLevel, pstrLog);

        string strLogInfo;
        if(LogInfo.ToString(strLogInfo))
        {
            m_pLogQueue->PutLog(strLogInfo.c_str(), (ushort) strLogInfo.length());
        }

    }

    bool CLogApp::SetLogLevel(unsigned char ucLevel)
    {
        if (ucLevel > MAX_LOG_LEVEL)
        {
            return false;
        }

        m_ucCurrLogLevel = ucLevel;

        return true;
    }

    string CLogApp::GetLogPath()
    {
        return m_strLogPath;
    }

    void CLogApp::CreateLogDirectory(const char *pLogPath)
    {
        if (pLogPath == NULL)
        {
            throw CLogException("the path is null");
        }

        if (strlen(pLogPath) > (size_t)(MAX_LOG_PATH_SIZE-1))
        {
            throw CLogException("the path length is too long");
        }

        char pDirectory[MAX_LOG_PATH_SIZE];

        strncpy(pDirectory, pLogPath, sizeof(pDirectory));
        pDirectory[MAX_LOG_PATH_SIZE-1] = '\0';

#ifndef _LINUX_
        char cSlash = '\\';
#else
        char cSlash = '/';
#endif

        char *pLocation = strrchr(pDirectory, cSlash);

        //找不到斜杠则说明传入的只有文件名，没有路径，则默认按照当前目录
        if (pLocation == NULL)
        {
            return;
        }

        *pLocation = '\0';

        if(!CreateDirectory(pDirectory, NULL))
        {
            if (GetLastError() != ERROR_ALREADY_EXISTS)
            {
                throw CLogException("can not create log dir");
            }
        }
    }

    void CLogApp::CheckParamVaild(unsigned short usMaxFileSize, unsigned char ucMaxFileCount)
    {
        if (usMaxFileSize > MAX_FILE_SIZE || usMaxFileSize < MIN_FILE_SIZE)
        {
            throw CLogException("param usMaxFileSize invaild");
        }
        if( ucMaxFileCount > MAX_FILE_COUNT || ucMaxFileCount < MIN_FILE_COUNT)
        {
            throw CLogException("param ucMaxFileCount invaild");
        }
    }
}
