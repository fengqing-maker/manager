
// Sid_Project_Manger.h : Sid_Project_Manger Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSid_Project_MangerApp:
// �йش����ʵ�֣������ Sid_Project_Manger.cpp
//

class CSid_Project_MangerApp : public CWinAppEx
{
public:
	CSid_Project_MangerApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSid_Project_MangerApp theApp;
