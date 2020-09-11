#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// CNewWorkType 对话框

class CDlgNewWorkType : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgNewWorkType)

public:
	CDlgNewWorkType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewWorkType();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_WORKTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
