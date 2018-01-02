#include "stdafx.h"
#include "LogRecord.h"
#include <string>
#include <vector>

/*exe程序*/
#define  _EXELOG
CLogRecord CLogRecord::singleton;

CLogRecord::CLogRecord(void)
{

}


CLogRecord::~CLogRecord(void)
{
	if (logFile.m_hFile != CFile::hFileNull)
	{
		logFile.Close();
	}
}

#ifdef _EXELOG
CString  CLogRecord::ReturnCALLPath()  
{   
	CString    sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	sPath.ReleaseBuffer();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	return    sPath;   
}
#else
CString  CLogRecord::ReturnOCXPath()  
{   
	CString    sPath;   
	GetModuleFileName(GetModuleHandle(_T("MakeInvActiveXCtr.ocx")),sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	sPath.ReleaseBuffer();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	return    sPath;   
}
#endif
BOOL CLogRecord::InitLogRecord()
{
	singleton.m_apppath = ReturnCALLPath();

	CString logStr;
	logStr.Format(_T("%s\\logfiles"), singleton.m_apppath);

	if(!PathFileExists(logStr))
	{
		if(!CreateDirectory(_T("logfiles"), NULL))
		{
			return FALSE;
		}
	}
	/*定时 清理*/
	{
		CString strIniPath = singleton.m_apppath +_T("\\logfiles\\day.ini");
		if (!PathFileExists(strIniPath))
		{
			::WritePrivateProfileString(_T("clear"),_T("day"),_T("30"),strIniPath);
		}
		singleton.ClearLog(::GetPrivateProfileInt(_T("clear"),_T("day"), 30,strIniPath));
	}


	
	logStr.Format(_T("%s\\logfiles\\%s.log"), singleton.m_apppath, CTime::GetCurrentTime().Format("%Y-%m-%d"));
	if(!singleton.logFile.Open(logStr,CFile::modeWrite | CFile::shareDenyNone))
	{
		if(singleton.logFile.Open(logStr,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
		{
			singleton.logFile.SeekToEnd();
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		singleton.logFile.SeekToEnd();
		return TRUE;
	}
	return FALSE;
}

void CLogRecord::WriteRecordToFile(CString strLog)
{
	std::string str;
	CString strCurDate;
	strCurDate = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S:\r\n");
	strCurDate += strLog;
//	singleton.logFile.Write(strCurDate,strCurDate.GetLength());
#ifdef UNICODE
	int len =WideCharToMultiByte(CP_ACP,0,(LPCTSTR)strCurDate,-1,NULL,0,NULL,NULL);
	char *ptxtTemp =new char[len +1];  
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)strCurDate,-1,ptxtTemp,len,NULL,NULL );
	str = ptxtTemp;
	str += "\r\n";
	singleton.logFile.Write(str.c_str(),str.length());
	delete ptxtTemp;
#else
	singleton.logFile.Write(strCurDate,strCurDate.GetLength());
	singleton.logFile.Write("\r\n",2);
#endif // UNICODE
}

CString CLogRecord::GetAppPath()
{
	return singleton.m_apppath;
}

/*
date: 20160921
author: xiezhao
add: 定期清理 日志功能 
参数说明: "0"表示只保留当天的 "1"表示 保留昨天 ... "5" 表示保留前5天的 以此类推
*/
BOOL CLogRecord::ClearLog(int day)
{
	CString logStr;
	CTime curTime = CTime::GetCurrentTime() - CTimeSpan( day, 0, 0, 0 );
	logStr.Format(_T("%s\\logfiles"), singleton.m_apppath);

	std::vector<CString> vec;
	TraverseDir(logStr,vec);
	for(std::vector<CString>::const_iterator it = vec.begin(); it < vec.end(); ++it)
	{
		CString strPath = *it;
		CString strLog = strPath.Right(strPath.GetLength() - strPath.ReverseFind(_T('\\'))-1);
		if (strLog < curTime.Format(_T("%Y-%m-%d.log")))
		{
			DeleteFile(strPath);
		}
	}
	return TRUE;
}

void CLogRecord::TraverseDir(CString& dir, std::vector<CString>& vec)
{
	CFileFind ff;
	if (dir.Right(1) != "\\")
		dir += "\\";
	dir += "*.*";

	BOOL ret = ff.FindFile(dir);
	while (ret)
	{
		ret = ff.FindNextFile();
		if (ret != 0)
		{
			if (ff.IsDirectory() && !ff.IsDots())
			{
				CString path = ff.GetFilePath();
				TraverseDir(path, vec);
			}
			else if (!ff.IsDirectory() && !ff.IsDots())
			{
				CString name = ff.GetFileName();
				CString path = ff.GetFilePath();
				vec.push_back(path);
			}
		}
	}
}
