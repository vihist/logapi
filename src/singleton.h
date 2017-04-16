#ifndef COMM_SINGLETON_FSS_2013_12_10
#define COMM_SINGLETON_FSS_2013_12_10

#ifdef UTTEST
#define private public
#endif

namespace space_logapi
{
    template <typename T>
    class CSingleton
    {
    public:
        static T* CreateInstance()
        {
            if (m_pSingleInstance == 0)
            {
                m_pSingleInstance = new T;
            }

            return m_pSingleInstance;
        };

        static T* GetInstance()
        {
            return m_pSingleInstance;
        }

        static void ReleaseInstance()
        {
            delete m_pSingleInstance;
            m_pSingleInstance = 0;
        }

        virtual ~CSingleton()
        {
            //lint -e{1551}
            delete m_pSingleInstance;
            m_pSingleInstance = 0;
        }

    private:
        static T* m_pSingleInstance;
    };

    template<typename T> 
    T * CSingleton<T>::m_pSingleInstance;
}
#endif
