#pragma once
#include "afxwin.h"
#include "CComonDlg.h"
#include "resource.h"
// CDlgNewDepart 对话框

class CDlgNewDepart : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgNewDepart)

public:
	CDlgNewDepart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewDepart();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_DEPART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
protected:
    void OnInitFatherDepart();

protected:
	CComboBox m_ctrlFatherDepart;
	CEdit m_ctrlManger;
	CEdit m_ctrlBakManger;

	CDepartSeverInc *m_pDepartSever;
	ListDepart    m_lstDeparts;
	CDepartment	  m_newDepart;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnChoceOwner();
	afx_msg void OnBnClickedBtnChoceBakOwner();
	CButton m_btnBakValidate;
};
