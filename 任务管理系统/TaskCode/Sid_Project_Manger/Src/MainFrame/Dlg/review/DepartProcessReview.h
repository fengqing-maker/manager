#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// DepartProcessReview �Ի���

class DepartProcessReview : public CComonDlg
{
	DECLARE_DYNAMIC(DepartProcessReview)

public:
	DepartProcessReview( const CTaskObj &obj, const CString &newSchedul, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DepartProcessReview();

// �Ի�������
	enum { IDD = IDD_DLG_DEPART_REVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
