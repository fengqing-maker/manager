#pragma once
#include "resource.h"
#include "../CComonDlg.h"
#include "afxdtctl.h"
// ExportWorkHourDlg 对话框

class ExportWorkHourDlg : public CComonDlg
{
	DECLARE_DYNAMIC(ExportWorkHourDlg)

public:
	ExportWorkHourDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ExportWorkHourDlg();

// 对话框数据
	enum { IDD = IDD_DLG_EXPORT_WORK_HOUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnDir();
protected:

	CDateTimeCtrl m_ctrlDateBegin;
	CDateTimeCtrl m_ctrlDateEnd;

	
};
