#include <string.h>
#include <algorithm>

#include "circlebuff.h"

using std::find;
namespace space_logapi
{
    CCircleBuff::CCircleBuff(ulong ulSize): m_ulSize(ulSize), m_ulRead(0), m_ulWrite(0)
    {
        m_pBuf = new uchar [m_ulSize];
        memset(m_pBuf, m_ulSize, 0x00);
    }

    CCircleBuff::~CCircleBuff()
    {
        delete [] m_pBuf;
        m_pBuf = NULL;
    }

    ushort CCircleBuff::Write(uchar* pData, ulong ulDataLen)
    {
        AUTO_CRITICAL_SECTION(m_cs);

        ulong ulFreeLen = FreeLength();
        if (ulFreeLen < ulDataLen)
        {
            return ERR_CIRCBUFF_NOT_SUFFI;
        }

        if (m_ulRead > m_ulWrite) 
        {
            memcpy(m_pBuf+m_ulWrite, pData, ulDataLen);
            m_ulWrite += ulDataLen;
        }
        else 
        {
            ulong ulTailLen = m_ulSize - m_ulWrite;

            ulong ulWritelLen = ulTailLen>ulDataLen ? ulDataLen : ulTailLen;
            memcpy(m_pBuf+m_ulWrite, pData, ulWritelLen);

            m_ulWrite = (m_ulWrite + ulDataLen) % m_ulSize;

            memcpy(m_pBuf, pData+ulWritelLen, ulDataLen-ulWritelLen);
        }

        return CIRC_SUCC;
    }

    ushort CCircleBuff::Read(uchar * pBuff, ulong& ulBuffLen)
    {
        AUTO_CRITICAL_SECTION(m_cs);

        if (IsEmpty())
        {
            ulBuffLen = 0;
            return CIRC_SUCC;
        }

        ulong ulReadLen = GetReadLen(ulBuffLen);

        if (m_ulWrite > m_ulRead) 
        {
            memcpy(pBuff, m_pBuf+m_ulRead, ulReadLen);
            m_ulRead += ulReadLen;
        }
        else 
        {
            ulong ulTailLen = m_ulSize - m_ulRead;

            ulong ulTailReadLen = (ulTailLen > ulReadLen ? ulReadLen : ulTailLen);
            memcpy(pBuff, m_pBuf+m_ulRead, ulTailReadLen);

            m_ulRead = (m_ulRead + ulReadLen) % m_ulSize;
            memcpy(pBuff+ulTailReadLen, m_pBuf, ulReadLen-ulTailReadLen);
        }

        ulBuffLen = ulReadLen;

        return CIRC_SUCC;
    }

    bool CCircleBuff::IsFull()
    {
        AUTO_CRITICAL_SECTION(m_cs);

        return (FreeLength() == 0);
    }

    bool CCircleBuff::IsEmpty()
    {
        AUTO_CRITICAL_SECTION(m_cs);

        return (m_ulRead == m_ulWrite);
    }

    ulong CCircleBuff::DataLength()
    {
        AUTO_CRITICAL_SECTION(m_cs);

        if (m_ulWrite >= m_ulRead)
        {
            return m_ulWrite - m_ulRead;
        }

        return m_ulSize - (m_ulRead - m_ulWrite);
    }

    ulong CCircleBuff::FreeLength() const
    {
        if (m_ulRead > m_ulWrite)
        {
            return m_ulRead-m_ulWrite-1;
        }

        return m_ulSize - (m_ulWrite - m_ulRead)-1;
    }

    ulong CCircleBuff::GetReadLen(ulong ulBuffLen)
    {
        ulong ulDataLen = DataLength();

        if (ulBuffLen > ulDataLen)
        {
            return ulDataLen;
        }

        ulong ulReadLen = 0;
        ulong ulIndex = 0;

        uchar *pStart = m_pBuf + m_ulRead;
        uchar *pEnd   = m_pBuf + m_ulWrite;
        uchar *pTail  = m_pBuf + m_ulSize;

        while (ulReadLen + ulIndex < ulBuffLen)
        {
            ulReadLen += ulIndex;

            if (pEnd > pStart)
            {
                uchar *pNewStart = std::find(pStart, pEnd, '\n');
                if (pNewStart == pEnd)
                {
                    break;
                }

                ulIndex = pNewStart - pStart +1;
                pStart = pNewStart + 1;
            }
            else
            {
                uchar *pNewStart = std::find(pStart, pTail, '\n');
                if (pNewStart == pTail)
                {
                    pNewStart = std::find(m_pBuf, pEnd, '\n');
                    if (pNewStart == pEnd)
                    {
                        break;
                    }

                    ulIndex =  m_ulSize - (pStart - pNewStart);
                    pStart = pNewStart + 1;
                }

                ulIndex = pNewStart - pStart +1;
                pStart = pNewStart + 1;
            }

        }

        return ulIndex;
    }
}