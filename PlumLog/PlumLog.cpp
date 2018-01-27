/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumLog.cpp
* @brief	This Program is PlumLog DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2018-1-26	v1.00a	alopex	Create Project.
*/
#include "PlumLog.h"

//This Class is Code Log.(�������м�¼/��־)
#pragma warning(disable: 4996)

//------------------------------------------------------------------
// @Function:	 CPlumLog()
// @Purpose: CPlumLog���캯��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumLog::CPlumLog()
{
	InitializeCriticalSection(&m_csThreadSafe);		//��ʼ���ٽ���
	m_bIsUseLog = true;								//Ĭ��ʹ��
	m_fp = NULL;									//��ʼ���ļ�ָ��
}

//------------------------------------------------------------------
// @Function:	 ~CPlumLog()
// @Purpose: CPlumLog��������
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumLog::~CPlumLog()
{
	DeleteCriticalSection(&m_csThreadSafe);			//ɾ���ٽ���
}

//------------------------------------------------------------------
// @Function:	 CPlumLog(bool bIsUseLog)
// @Purpose: CPlumLog���캯��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumLog::CPlumLog(bool IsUseLog)
{
	InitializeCriticalSection(&m_csThreadSafe);		//��ʼ���ٽ���
	m_bIsUseLog = IsUseLog;							//���촫�����
	m_fp = NULL;									//��ʼ���ļ�ָ��
}

//------------------------------------------------------------------
// @Function:	 PlumLogInit()
// @Purpose: CPlumLog��ʼ��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::PlumLogInit()
{
	char ModulePathArr[MAX_PATH];
	char* pModuleNameArr = NULL;
	char* pTemp = NULL;
	int nSize = 0;

	//��ȡģ������
	memset(ModulePathArr, 0, sizeof(ModulePathArr));
	GetModuleFileNameA(NULL, ModulePathArr, MAX_PATH);

	//ģ������
	pTemp = strrchr(ModulePathArr, '\\') + 1;
	nSize = strlen(pTemp);
	pModuleNameArr = new char[nSize + 1];
	memset(pModuleNameArr, 0, nSize + 1);
	memcpy_s(pModuleNameArr, nSize + 1, pTemp, nSize);

	//ɾ����׺
	for (pTemp = strrchr(pModuleNameArr, '.'); pTemp != (pModuleNameArr + nSize); ++pTemp)
	{
		*pTemp = '\0';
	}

	//��һ��·��
	for (pTemp = strrchr(ModulePathArr, '\\'); pTemp != &ModulePathArr[MAX_PATH - 1]; ++pTemp)
	{
		*pTemp = '\0';
	}

	//��ȡϵͳʱ��
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);

	//������־����
	char LogDirArr[MAX_PATH];
	char LogPathArr[MAX_PATH];

	memset(LogDirArr, 0, sizeof(LogDirArr));
	memset(LogPathArr, 0, sizeof(LogPathArr));
	_snprintf_s(LogDirArr, sizeof(LogDirArr), "%s\\log", ModulePathArr);
	_snprintf_s(LogPathArr, sizeof(LogPathArr), "%s\\log\\%s_RuntimeLog_%d_%02d_%02d_%02d%02d%02d.log", ModulePathArr, pModuleNameArr, LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);

	//���������־�ļ�
	char WriteArr[MAX_PATH];

	fopen_s(&m_fp, LogPathArr, "w");
	if (m_fp == NULL)
	{
		CreateDirectoryA(LogDirArr, NULL);
		fopen_s(&m_fp, LogPathArr, "w");
		if (m_fp == NULL)
		{
			delete[] pModuleNameArr;
			pModuleNameArr = NULL;
			return;
		}
	}

	//����
	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), "%s RuntimeLog File\n\n", pModuleNameArr);
	fputs(WriteArr, m_fp);

	delete[] pModuleNameArr;
	pModuleNameArr = NULL;

	//������Ϣ
	PlumLogWriteLine("[Infomation]");

	//��������
	WriteLineComputerName();

	//�û�����
	WriteLineUserName();

	//Windows�汾
	WriteLineWindowsVision();

	//ʱ��
	WriteLineCurrentTime();

	//����
	WriteLineProcessID();

	//��ʽ��ʼ
	PlumLogWriteLine("[Process Start]");

}

//------------------------------------------------------------------
// @Function:	 PlumLogExit()
// @Purpose: CPlumLog�˳�
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::PlumLogExit()
{
	//��ʽ����
	PlumLogWriteLine("");
	PlumLogWriteLine("[Process Exit]");

	fclose(m_fp);
}

//------------------------------------------------------------------
// @Function:	 PlumLogWrite()
// @Purpose: CPlumLogд��(ASCII)
// @Since: v1.00a
// @Para: LPCSTR lpcstr		//�����ַ
// @Return: None
//------------------------------------------------------------------
void CPlumLog::PlumLogWrite(LPCSTR lpcstr)
{
	char WriteArr[MAX_PATH];

	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), lpcstr);
	fputs(WriteArr, m_fp);
}

//------------------------------------------------------------------
// @Function:	 PlumLogWriteLine()
// @Purpose: CPlumLogд��(ASCII)
// @Since: v1.00a
// @Para: LPCSTR lpcstr		//�����ַ
// @Return: None
//------------------------------------------------------------------
void CPlumLog::PlumLogWriteLine(LPCSTR lpcstr)
{
	char WriteArr[MAX_PATH];

	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), lpcstr);
	fputs(WriteArr, m_fp);
	fputs("\n", m_fp);
}

//------------------------------------------------------------------
// @Function:	 WriteLineComputerName()
// @Purpose: CPlumLogд����������
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::WriteLineComputerName()
{
	char WriteArr[MAX_PATH];
	char* pComputerNameArr = NULL;
	DWORD dwComputerNameSize = MAX_COMPUTERNAME_LENGTH;

	pComputerNameArr = new char[MAX_COMPUTERNAME_LENGTH + 1];
	memset(pComputerNameArr, 0, sizeof(pComputerNameArr));

	GetComputerNameA(pComputerNameArr, &dwComputerNameSize);

	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), "Machine Name: %s\n", pComputerNameArr);
	fputs(WriteArr, m_fp);

	delete[] pComputerNameArr;
	pComputerNameArr = NULL;
}

//------------------------------------------------------------------
// @Function:	 WriteLineUserName()
// @Purpose: CPlumLogд���û�����
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::WriteLineUserName()
{
	char WriteArr[MAX_PATH];
	char* pUserNameArr = NULL;
	DWORD dwUserNameSize = MAX_PATH;

	pUserNameArr = new char[MAX_PATH + 1];
	memset(pUserNameArr, 0, sizeof(pUserNameArr));

	GetUserNameA(pUserNameArr, &dwUserNameSize);

	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), "User Name: %s\n", pUserNameArr);
	fputs(WriteArr, m_fp);

	delete[] pUserNameArr;
	pUserNameArr = NULL;
}

//------------------------------------------------------------------
// @Function:	 WriteLineWindowsVision()
// @Purpose: CPlumLogд��Windows�汾����
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::WriteLineWindowsVision()
{
	char WriteArr[MAX_PATH];
	char* pWindowsVision = NULL;
	SYSTEM_INFO Info;
	OSVERSIONINFOEX Os;

	GetSystemInfo(&Info);
	Os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	pWindowsVision = (char*)"Unknown OperatingSystem";

	if (GetVersionEx((OSVERSIONINFO*)&Os))
	{
		switch (Os.dwMajorVersion)
		{
		case 4:
			switch (Os.dwMinorVersion)
			{
			case 0:
				if (Os.dwPlatformId == VER_PLATFORM_WIN32_NT)
				{
					pWindowsVision = (char*)"Microsoft Windows NT 4.0";
				}
				else if (Os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				{
					pWindowsVision = (char*)"Microsoft Windows 95";
				}
				break;
			case 10:
				pWindowsVision = (char*)"Microsoft Windows 98";
				break;
			case 90:
				pWindowsVision = (char*)"Microsoft Windows Me";
				break;
			}
			break;

		case 5:
			switch (Os.dwMinorVersion)
			{
			case 0:
				pWindowsVision = (char*)"Microsoft Windows 2000";
				break;
			case 1:
				pWindowsVision = (char*)"Microsoft Windows XP";
				break;

			case 2:
				if (Os.wProductType == VER_NT_WORKSTATION
					&& Info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					pWindowsVision = (char*)"Microsoft Windows XP Professional x64 Edition";
				}
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2003";
				}
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2003 R2";
				}
				break;
			}
			break;

		case 6:
			switch (Os.dwMinorVersion)
			{
			case 0:
				if (Os.wProductType == VER_NT_WORKSTATION)
				{
					pWindowsVision = (char*)"Microsoft Windows Vista";
				}
				else
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2008";
				}
				break;
			case 1:
				if (Os.wProductType == VER_NT_WORKSTATION)
				{
					pWindowsVision = (char*)"Microsoft Windows 7";
				}
				else
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2008 R2";
				}
				break;
			case 2:
				if (Os.wProductType == VER_NT_WORKSTATION)
				{
					pWindowsVision = (char*)"Microsoft Windows 8";
				}
				else
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2012";
				}
				break;
			case 3:
				if (Os.wProductType == VER_NT_WORKSTATION)
				{
					pWindowsVision = (char*)"Microsoft Windows 8.1";
				}
				else
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2012 R2";
				}
				break;
			}
			break;

		case 10:
			switch (Os.dwMinorVersion)
			{
			case 0:
				if (Os.wProductType == VER_NT_WORKSTATION)
				{
					pWindowsVision = (char*)"Microsoft Windows 10";
				}
				else
				{
					pWindowsVision = (char*)"Microsoft Windows Server 2016 Technical Preview";
				}
				break;
			}
			break;
		}
	}

	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), "Windows Vision: %s\n", pWindowsVision);
	fputs(WriteArr, m_fp);
}

//------------------------------------------------------------------
// @Function:	 WriteLineProcessID()
// @Purpose: CPlumLogд�뵱ǰ����ID
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::WriteLineProcessID()
{
	char WriteArr[MAX_PATH];
	DWORD dwProcessID = 0;
	dwProcessID = GetCurrentProcessId();
	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), "Current Process ID: %d\n\n", dwProcessID);
	fputs(WriteArr, m_fp);
}

//------------------------------------------------------------------
// @Function:	 WriteLineCurrentTime()
// @Purpose: CPlumLogд�뵱ǰʱ��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
void CPlumLog::WriteLineCurrentTime()
{
	char WriteArr[MAX_PATH];
	SYSTEMTIME LocalTime;

	GetLocalTime(&LocalTime);
	memset(WriteArr, 0, sizeof(WriteArr));
	_snprintf_s(WriteArr, sizeof(WriteArr), "Current Time: %d_%02d_%02d %02d:%02d:%02d\n", LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);
	fputs(WriteArr, m_fp);
}