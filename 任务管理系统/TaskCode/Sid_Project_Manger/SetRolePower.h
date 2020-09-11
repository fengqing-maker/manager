#pragma once
#include "resource.h"

// CSetRolePower 对话框

class CSetRolePower : public CComonDlg
{
	DECLARE_DYNAMIC(CSetRolePower)

public:
	CSetRolePower(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetRolePower();

// 对话框数据
	enum { IDD = IDD_DLG_SELECT_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:

};
