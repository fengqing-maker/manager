#pragma once
#include "resource.h"
#include "afxdtctl.h"
#include "../../action/factory/actionServerFactory.h"
// CStartTaskDlg �Ի��� ������������

class CStartTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartTaskDlg)

public:
	CStartTaskDlg( const CTaskObj &obj,const COleDateTime &startTime, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartTaskDlg();

	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DLG_STARTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:

	CTaskObj m_handTask;
	CDateTimeCtrl m_startFactTime;
private: 
	CActionServerFactory *m_pSever;
	CTaskSeverInc        *m_pTaskSever;
	COleDateTime          m_startDate;
	CFont m_font;
public:
	afx_msg void OnBnClickedBtnStart();
};
