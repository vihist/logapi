#ifndef COMM_CTHREAD_FSS_2013_12_29
#define COMM_CTHREAD_FSS_2013_12_29


#ifndef NULL
#define NULL 0
#endif

namespace space_logapi
{
    //PROCESS_FUNC �̺߳�������������
    typedef void ( *PROCESS_FUNC)(void *);

    //NOTIFY_END�߳��˳��������������ô˺����󣬿��Ե���CThread::NotifyEnd֪ͨ��ǰ�߳��˳�
    typedef bool ( *NOTIFY_END)();

    class CThread
    {
    public:
        CThread(): m_pProcessFunc(NULL), m_pNotifyEndFunc(NULL), m_pProcessParam(NULL)
        {
        };

        virtual ~CThread()
        {
            m_pProcessFunc = NULL;
            m_pNotifyEndFunc = NULL;
            m_pProcessParam = NULL;
        };

    public:
        virtual bool Initialize(PROCESS_FUNC pProcessFunc, void* pProcessParam, NOTIFY_END pNotifyEndFunc) = 0;
        virtual bool StartThread() = 0;

        virtual bool NotifyEnd()
        {
            if (m_pNotifyEndFunc == NULL || !m_pNotifyEndFunc())
            {
                return false;
            }

            return true;
        };

        virtual void WaitEnd() = 0;

    protected:
        PROCESS_FUNC m_pProcessFunc;
        NOTIFY_END   m_pNotifyEndFunc;

        void	*m_pProcessParam;
    };
}
#endif
