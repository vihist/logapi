#ifndef CIRCLE_BUFF_FSS_2014_02_13
#define CIRCLE_BUFF_FSS_2014_02_13

#include "buildintype.h" //lint -e537

#include "criticalsection.h"

#ifdef _UTTEST
#include "ut_stub.h"
#endif
namespace space_logapi
{
    class CCircleBuff
    {

    public:
        CCircleBuff(ulong ulSize);
        ~CCircleBuff();

        ushort Write(uchar* pData, ulong len);
        ushort Read(uchar* pData,  ulong& len);

        bool IsFull();
        bool IsEmpty();

        ulong DataLength();

    private:

        ulong FreeLength() const;
        ulong ReadMsgLength() const;
        ulong GetReadLen(ulong ulBuffLen);

    private:
        ulong   m_ulSize; //缓冲区总长度

        ulong   m_ulRead;  //读点
        ulong   m_ulWrite; //写点

        uchar *  m_pBuf;

        CLock_CS m_cs;
    };

    enum ENUM_ERRORNO
    {
        CIRC_SUCC = 0,
        ERR_CIRCBUFF_NOT_SUFFI,
        ERR_USERBUFF_NOT_SUFFI,

        ERR_MAX = 0xffff,
    };
}

#endif
