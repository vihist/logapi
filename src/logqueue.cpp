#include <time.h>
#include "logexception.h"

#include "logqueue.h"
#include "loginfo.h"
#include "sem.h"

namespace space_logapi
{
    const ulong  MAX_CACHE_LEN  = MAX_LOGINF_LEN*200;
    const ushort MAX_CACHE_TIME = 5;

    CLogQueue::CLogQueue() : m_CircleBuff(MAX_QUEUE_BUFF_LEN), m_pSemaphore(new CSemaphore())
    {

    }

    CLogQueue::~CLogQueue()
    {
        delete m_pSemaphore;
        m_pSemaphore = NULL;
    }

    void CLogQueue::Initialize()
    {

    }


    ushort CLogQueue::PutLog(const char* pLogBuff, ushort usLen)
    {
        ushort usRet = m_CircleBuff.Write((uchar *)pLogBuff, usLen);
        if (usRet  == CIRC_SUCC)
        {
            if (m_CircleBuff.DataLength() >= MAX_CACHE_LEN)
            {
                m_pSemaphore->Post();
            }
        }

        return usRet;
    }

    ushort CLogQueue::GetLog(char* pLogBuff, ulong& ulLen, ulong ulCacheTime)
    {
        //不管超时还是有数据可取，都需要进行写文件操作，所以此处不区分返回值
        (void)m_pSemaphore->Wait(ulCacheTime);

        ulong  ulReadLen = ulLen-1;
        (void)m_CircleBuff.Read((uchar *)pLogBuff, ulReadLen);

        if (pLogBuff[ulReadLen] != '\0')
        {
            pLogBuff[ulReadLen] = '\0';
        }

        ulLen = ulReadLen;

        return 0;
    }

    bool CLogQueue::IsEmpty()
    {
        return m_CircleBuff.IsEmpty();
    }
}
