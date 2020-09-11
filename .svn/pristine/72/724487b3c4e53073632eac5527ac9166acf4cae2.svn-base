#pragma once

#include "CComonDlg.h"
#include "resource.h"
#include "afxwin.h"
// NewProjectDlg 对话框

class NewProjectDlg : public CComonDlg
{
	DECLARE_DYNAMIC(NewProjectDlg)

public:
	NewProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NewProjectDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnChooseOwner();
	afx_msg void OnBnClickedBtnChooseMember();
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
protected:
	BOOL OnInitProductLine();

protected:
	CEdit m_eiditPrjName;
	CEdit m_editPrjLongName;
	CComboBox m_comboPrjLine;
	CEdit m_editOwner;
	CEdit m_eidtPrjMember;


	//数据
	ListProjectLine m_lstProductLine;
	CProjectSeverInc *m_projectSever;
	CProject           m_newPrjObj;
	ListUser           m_prjMember;

};
