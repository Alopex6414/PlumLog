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
	CRITICAL_SECTION m_csThreadSafe;		//Critical Section(~线程安全)

public:
	bool m_bIsUseLog;						//Use PlumLog Flag(~是否使用)
	FILE* m_fp;								//File Pointer(~文件指针)

public:
	CPlumLog();								//Construction Function(~构造函数)
	~CPlumLog();							//Destruction Function(~析构函数)

	//构造
	CPlumLog(bool bIsUseLog);				//Construction Function(~构造函数)

	//初始化
	virtual void PlumLogInit();				//PlumLog Initialize(~初始化)

	//退出
	virtual void PlumLogExit();				//PlumLog Exit(~退出)

	//写入
	virtual void PlumLogWrite(LPCSTR lpcstr);			//Plum Write Line(~写入)(ASCII)
	virtual void PlumLogWriteLine(LPCSTR lpcstr);		//Plum Write Line(~写入)(ASCII)

protected:
	virtual void WriteLineComputerName();	//PlumLog Write Line Computer Name(~写入计算机名称)
	virtual void WriteLineUserName();		//PlumLog Write Line User Name(~写入用户名称)
	virtual void WriteLineWindowsVision();	//PlumLog Write Line Windows Vision(~写入Windows版本)
	virtual void WriteLineProcessID();		//PlumLog Write Line Process ID(~写入进程ID)
	virtual void WriteLineCurrentTime();	//PlumLog Write Line Current Time(~写入当前时间)

};

#endif // !__PLUMLOG_H_

