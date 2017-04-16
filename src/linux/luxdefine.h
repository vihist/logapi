#ifndef COMM_LINUXDEF_FSS_2013_12_31
#define COMM_LINUXDEF_FSS_2013_12_31

namespace space_logapi
{
    #define closesocket close
    #define GetLastError() errno
    #define WSAGetLastError() errno
    #define _snprintf snprintf
    #define INVALID_HANDLE_VALUE NULL
    #define WSAECONNRESET  ECONNRESET
    #define MAKEWORD(low,high)  ((unsigned short )(((unsigned char)(low)) | ((unsigned short )((unsigned char)(high))) << 8))
    #define SD_BOTH SHUT_RDWR

    typedef int SOCKET;

    const int INVALID_SOCKET = -1;
    const int SOCKET_ERROR = -1;
}

#endif


