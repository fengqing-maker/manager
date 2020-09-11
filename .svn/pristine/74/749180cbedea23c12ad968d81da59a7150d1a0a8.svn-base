#pragma once
#include "resource.h"
#include "../CComonDlg.h"
#include "afxwin.h"
// CDlgNewProductLine 对话框

class CDlgNewProductLine : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgNewProductLine)

public:
	CDlgNewProductLine(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewProductLine();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_PRODUCT_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL CDlgNewProductLine::OnInitDialog();
	
	afx_msg void OnBnClickedBtnChoceOwner();
	DECLARE_MESSAGE_MAP()
protected:
	CProjectSeverInc *m_prjSever;
    CProductLine      m_productLine;


	CEdit m_ctrlOwner;
public:
	afx_msg void OnBnClickedOk();
};
