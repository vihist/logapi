#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>

#ifdef _LINUX_
#include <stdlib.h>
#include "./linux/luxthread.h"
#elif _UTTEST  //lint -e553
#include "winthread_stub.h"
#else
#include "windows/winthread.h"
#endif

#include "logexception.h"

#include "logqueue.h"

#include "recorder.h"
#include "ofstreammanager.h"

#include "loginfo.h"

using std::cout;
using std::cerr;
using std::endl;

namespace space_logapi
{
    const unsigned long MAX_LOG_BUFF_LEN = 500*1024;
    const unsigned long MAX_CACHE_TIME = 5*1000;

    CLogRecorder::CLogRecorder() : 
    m_pThread(NULL), 
        m_bExist(false),
        m_pLogQueue(NULL), 
        m_pOstreamManager(NULL), 
        m_ulLogBuffLen(MAX_LOG_BUFF_LEN),
        m_pLogBuff(new char[m_ulLogBuffLen])
    {

    }

    CLogRecorder::~CLogRecorder()
    {
        delete []m_pLogBuff;
        m_pLogBuff = NULL;
    }

    void CLogRecorder::Initialize(CLogQueue* pLogQueue, COstreamManager* pOstreamManager)
    {
        m_pLogQueue = pLogQueue;
        m_pOstreamManager = pOstreamManager;
    }

    void CLogRecorder::StartThread()
    {
        m_pThread = new space_logapi::CCurrPlatformThread;
        if( !m_pThread->Initialize(ThreadFunc, this, NULL) || !m_pThread->StartThread())
        {
            throw CLogException("start log record thread failed");
        }
    }

    void CLogRecorder::ThreadFunc(void* pParam)
    {
        try
        {
            CLogRecorder* pLogRecorder = static_cast<CLogRecorder *>(pParam);
            if (pLogRecorder == NULL)
            {
                return;
            }

            while(!pLogRecorder->m_bExist)
            {
                pLogRecorder->Record(MAX_CACHE_TIME);
            }

            pLogRecorder->Record(0);
        }
        catch(exception& expt)
        {
            cerr<< "record file failed, "<<expt.what()<<endl;
        }

    }

    void CLogRecorder::Record(ulong ulCacheTime)
    {
        if (m_pLogQueue == NULL || m_pOstreamManager == NULL)
        {
            m_bExist = true;
            return;
        }

        (void)m_pLogQueue->GetLog(m_pLogBuff, m_ulLogBuffLen, ulCacheTime);

        size_t LogLen = strlen(m_pLogBuff);
        if (LogLen != 0)
        {
            ostream& OutStream = m_pOstreamManager->GetCurrOutStream(LogLen);

            OutStream << m_pLogBuff;
            OutStream.flush();
        }

        m_pLogBuff[0] = '\0';
        m_ulLogBuffLen = MAX_LOG_BUFF_LEN;
    }

    void CLogRecorder::StopThread()
    {
        if (m_pLogQueue == NULL || m_pThread == NULL)
        {
            return;
        }

        m_bExist = true;
        m_pThread->WaitEnd();
    }
}
