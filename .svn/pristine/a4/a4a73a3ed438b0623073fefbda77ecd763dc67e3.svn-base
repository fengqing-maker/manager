#pragma once
#include "afxdtctl.h"
#include "resource.h"
#include "../../action/factory/actionServerFactory.h"

// CFinisTaskDlg 对话框

class CFinisTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFinisTaskDlg)

public:
	CFinisTaskDlg(const CTaskObj &obj, const COleDateTime &FinishTime, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFinisTaskDlg();

	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DLG_ENDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedBtnTaskFinish();
	DECLARE_MESSAGE_MAP()
protected:
	CTaskObj       m_handTask;
	CDateTimeCtrl  m_endDateCtrl;
	CTaskSeverInc  *m_pTaskSever;
	COleDateTime    m_finishDate;

	CFont           m_font;
};
