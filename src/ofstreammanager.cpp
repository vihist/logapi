#pragma warning ( disable : 4996)



#include <stdio.h>
#include <fcntl.h>

#ifndef _LINUX_
#include <windows.h>
#endif

//#define  __STDC__ 0

#ifndef _LINUX_
#include <io.h>
#else
#include <unistd.h>
#include "./linux/inifile.h"
#endif

#include <sys/stat.h>

#include <string>
#include <sstream>
#include <iomanip>

#include "logexception.h"
#include "ofstreammanager.h"

using std::ostringstream;
using std::ios;
using std::setw;

namespace space_logapi
{

    COfstreamManager::COfstreamManager() : m_ulMaxFileSize(0), m_ucMaxFileCount(0), m_ulCurrFileNO(0), m_ulCurrFileSize(0)
    {

    }

    COfstreamManager::~COfstreamManager()
    {
        try
        {
            m_OutFileStream.close();
        }
        catch (...)
        {

        }
    }

    void COfstreamManager::Initialize(const char *pLogFullPath, ushort usMaxFileSize, uchar ucMaxFileCount)
    {
        m_ulMaxFileSize  = usMaxFileSize*1024;
        m_ucMaxFileCount = ucMaxFileCount;

        InitLogPathAndFileName(pLogFullPath);

        InitCurrFileInfo();

        string strLogFullPathExt;
        GetLogFullPathExt(m_ulCurrFileNO, strLogFullPathExt);

        m_OutFileStream.open(strLogFullPathExt.c_str(), ios::app);
    }

    BOOL WritePrivateProfileULong(const char* pAppName, const char* pKeyName, ulong ulData, const char* pFileName)
    {
        ostringstream strStream;
        (void)strStream.fill('0');

        strStream << ulData;

        return WritePrivateProfileString(pAppName, pKeyName, strStream.str().c_str(), pFileName);
    }

    ostream& COfstreamManager::GetCurrOutStream(ulong ulLogInfoLen)
    {
        if (m_ulCurrFileSize >= m_ulMaxFileSize)
        {
            m_OutFileStream.close();

            if (m_ulCurrFileNO+1 >= m_ucMaxFileCount)
            {
                RemoveOldFiles();
            }

            UpdateFileNO();

            CreateNewFile();
        }

        m_ulCurrFileSize += ulLogInfoLen;

        //lint -e1536
        return m_OutFileStream; 
    }

    void COfstreamManager::InitLogPathAndFileName(const char* pLogFullPath)
    {
        string strLogFullPath(pLogFullPath);

#ifndef _LINUX_
        const char cSlash = '\\';
#else
        const char cSlash = '/';
#endif

        const size_t SlashIndex = strLogFullPath.find_last_of(cSlash);

        if (SlashIndex == string::npos)
        {
            (void)m_strLogPath.assign(".");
            (void)m_strLogPath.append(1, cSlash);

            (void)m_strFileName.assign(strLogFullPath);
        }
        else
        {
            (void)m_strLogPath.assign(strLogFullPath, 0, SlashIndex+1);
            (void)m_strFileName.assign(strLogFullPath, SlashIndex+1, string::npos);
        }

        m_strIniFileName = m_strLogPath + "." + m_strFileName + "_LOG_INFO.ini";
    }

    void COfstreamManager::GetLogFullPathExt(ulong ulFileNO, string& strLogFullPathExt) const
    {
        ostringstream strTimeLogStream;
        (void)strTimeLogStream.fill('0');

        strTimeLogStream << m_strLogPath
            <<m_strFileName
            << "_"
            << setw(2) << ulFileNO
            << ".LOG";

        strLogFullPathExt = strTimeLogStream.str();
    }

    ulong GetFileLength(const string& strFileFullPath)
    {
        struct stat FileStat;

        if(stat(strFileFullPath.c_str(), &FileStat) == 0)
        {
            return FileStat.st_size;
        }

        return 0;
    }

    bool ChkFileExist(const string& strFileFullPath)
    {
        return (access(strFileFullPath.c_str(), 0) == 0);
    }

    void COfstreamManager::InitCurrFileInfo()
    {
        string strFirstFileName;
        GetLogFullPathExt(0, strFirstFileName);

        string strIniFullPath = m_strLogPath + "." + m_strFileName + "_LOG_INFO.ini";

        if( !ChkFileExist(strIniFullPath) || !ChkFileExist(strFirstFileName))
        {
            if (! WritePrivateProfileString("LOG_INFO", "LOG_FILE_NO", "0", strIniFullPath.c_str()))
            {
                throw CLogException("can not create *.ini for log");
            }

#ifndef _LINUX_
            (void)SetFileAttributes(strIniFullPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
#endif

        }

        m_ulCurrFileNO = GetPrivateProfileInt("LOG_INFO", "LOG_FILE_NO", 0, strIniFullPath.c_str());

        string strLogFullPathExt;
        GetLogFullPathExt(m_ulCurrFileNO, strLogFullPathExt);

        m_ulCurrFileSize = GetFileLength(strLogFullPathExt);
    }

    void COfstreamManager::RemoveOldFiles()
    {
        string strFirstFileName;
        GetLogFullPathExt(0, strFirstFileName);

        (void)remove(strFirstFileName.c_str());

        for (uchar i=1; i<m_ucMaxFileCount; i++)
        {
            string strOldFileName;
            GetLogFullPathExt(i, strOldFileName);

            string strNewFileName;
            GetLogFullPathExt(i-1, strNewFileName);

            (void)rename(strOldFileName.c_str(), strNewFileName.c_str());
        }
    }

    void COfstreamManager::CreateNewFile()
    {
        m_ulCurrFileSize = 0;

        string strLogFullPathExt;
        GetLogFullPathExt(m_ulCurrFileNO, strLogFullPathExt);

        if (ChkFileExist(strLogFullPathExt.c_str()))
        {
            (void)remove(strLogFullPathExt.c_str());
        }

        m_OutFileStream.open(strLogFullPathExt.c_str(), ios::app);
    }

    void COfstreamManager::UpdateFileNO()
    {
        if (m_ulCurrFileNO+1 < m_ucMaxFileCount)
        {
            m_ulCurrFileNO++;
        }

        (void)WritePrivateProfileULong("LOG_INFO", "LOG_FILE_NO", m_ulCurrFileNO, m_strIniFileName.c_str());
    }
}
