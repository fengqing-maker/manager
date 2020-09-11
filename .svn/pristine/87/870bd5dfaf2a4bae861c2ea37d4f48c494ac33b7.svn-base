#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// DepartProcessReview 对话框

class DepartProcessReview : public CComonDlg
{
	DECLARE_DYNAMIC(DepartProcessReview)

public:
	DepartProcessReview( const CTaskObj &obj, const CString &newSchedul, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DepartProcessReview();

// 对话框数据
	enum { IDD = IDD_DLG_DEPART_REVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	int m_processScore;
	CString m_processReview;
	const CTaskObj& m_setTask;
	const CString&  m_strSchedul;
	CScheduleActionInc* m_pScheduleSever;
public:
	afx_msg void OnBnClickedOk();
};
