#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// CModiyTaskScoreDlg �Ի���

class CModiyTaskScoreDlg : public CComonDlg
{
	DECLARE_DYNAMIC(CModiyTaskScoreDlg)

public:
	CModiyTaskScoreDlg( const CTaskObj &obj,  CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModiyTaskScoreDlg();

	// �Ի�������
	enum { IDD = IDD_DLG_DEPART_REVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
