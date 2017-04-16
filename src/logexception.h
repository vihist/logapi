#ifndef LOG_EXCEPTION_FSS_2014_02_20
#define LOG_EXCEPTION_FSS_2014_02_20

#include <exception>

using std::exception;

namespace space_logapi
{
    class CLogException : public exception
    {
    public:
        CLogException(const char *_Message = "bad exception") : 
#ifdef _LINUX_
          _Mywhat(_Message)
#else
          exception(_Message)
#endif  
          {

          }

          ~CLogException() throw()
          {
          }

#ifdef _LINUX_
          const char* what() const throw()
          {
              return _Mywhat;
          }
#endif

    protected:

#ifdef _LINUX_
    private:
        const char * _Mywhat;
#endif
    };

    class CLogReInitException : public CLogException
    {
    public:
        CLogReInitException(const char *_Message = "bad exception") : CLogException(_Message)
        {

        }
    };
}
#endif
