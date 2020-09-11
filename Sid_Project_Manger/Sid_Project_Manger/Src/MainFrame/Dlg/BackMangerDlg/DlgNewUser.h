#pragma once

#include "resource.h"
#include "../CComonDlg.h"
#include "afxwin.h"
// CDlgNewUser 对话框

class CDlgNewUser : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgNewUser)

public:
	CDlgNewUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewUser();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnInitDepart();
	virtual BOOL OnInitRoleList();
	DECLARE_MESSAGE_MAP()

protected:
	CBackStageActionInc *m_pBackActionInc;
	ListDepart           m_allDepart;
	ListRole             m_allRole;
	CComboBox m_ctrlRole;
	CComboBox m_ctrldepart;
	
public:
	afx_msg void OnBnClickedOk();
};
