#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// CModiyTaskScoreDlg 对话框

class CModiyTaskScoreDlg : public CComonDlg
{
	DECLARE_DYNAMIC(CModiyTaskScoreDlg)

public:
	CModiyTaskScoreDlg( const CTaskObj &obj,  CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModiyTaskScoreDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_DEPART_REVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	CString m_processReview;
	const CTaskObj& m_setTask;
	int             m_moidyScore;
	CScheduleActionInc* m_pScheduleSever;
public:
	afx_msg void OnBnClickedOk();
};
