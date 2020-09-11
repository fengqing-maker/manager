#pragma once

#include "resource.h"
#include "Src/Model/TaskModel.h"
// CStopTaskDlg 对话框

class CStopTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStopTaskDlg)

public:
	CStopTaskDlg( CTaskObj &task, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStopTaskDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SETSCORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSetScore();
	DECLARE_MESSAGE_MAP()

protected:
	CTaskObj &m_taskObj;

};
