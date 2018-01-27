/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumLog.h
* @brief	This Program is PlumLog DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2018-1-26	v1.00a	alopex	Create Project.
*/
#pragma once

#ifndef __PLUMLOG_H_

//Include Windows Header File
#include <Windows.h>

//Include C/C++ Run Header File
#include <stdio.h>

//Macro Definition
#ifdef	PLUMLOG_EXPORTS
#define PLUMLOG_API	__declspec(dllexport)
#else
#define PLUMLOG_API	__declspec(dllimport)
#endif

//Class Definition
class PLUMLOG_API CPlumLog
{
private:
	CRITICAL_SECTION m_csThreadSafe;		//Critical Section(~�̰߳�ȫ)

public:
	bool m_bIsUseLog;						//Use PlumLog Flag(~�Ƿ�ʹ��)
	FILE* m_fp;								//File Pointer(~�ļ�ָ��)

public:
	CPlumLog();								//Construction Function(~���캯��)
	~CPlumLog();							//Destruction Function(~��������)

	//����
	CPlumLog(bool bIsUseLog);				//Construction Function(~���캯��)

	//��ʼ��
	virtual void PlumLogInit();				//PlumLog Initialize(~��ʼ��)

	//�˳�
	virtual void PlumLogExit();				//PlumLog Exit(~�˳�)

	//д��
	virtual void PlumLogWrite(LPCSTR lpcstr);			//Plum Write Line(~д��)(ASCII)
	virtual void PlumLogWriteLine(LPCSTR lpcstr);		//Plum Write Line(~д��)(ASCII)

protected:
	virtual void WriteLineComputerName();	//PlumLog Write Line Computer Name(~д����������)
	virtual void WriteLineUserName();		//PlumLog Write Line User Name(~д���û�����)
	virtual void WriteLineWindowsVision();	//PlumLog Write Line Windows Vision(~д��Windows�汾)
	virtual void WriteLineProcessID();		//PlumLog Write Line Process ID(~д�����ID)
	virtual void WriteLineCurrentTime();	//PlumLog Write Line Current Time(~д�뵱ǰʱ��)

};

#endif // !__PLUMLOG_H_

