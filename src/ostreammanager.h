#ifndef OUT_STREAM_MANAGER_FSS_2014_02_17
#define OUT_STREAM_MANAGER_FSS_2014_02_17

#include <iostream>

using std::ostream;

namespace space_logapi
{
    class COstreamManager
    {
    public:
        virtual ~COstreamManager(){};
        virtual ostream& GetCurrOutStream(ulong ulLogInfoLen) = 0;
    };
}
#endif
