#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// CSetRolePower 对话框

class CDlgSetRolePower : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgSetRolePower)

public:
	CDlgSetRolePower(const CRole *pRole, BOOL isNew = FALSE, CWnd* pParent = NULL);   // 标准构造函数
	
	virtual ~CDlgSetRolePower();
	const IntStringMap& GetSelPowers(){ return m_selPower;}
// 对话框数据
	enum { IDD = IDD_DLG_SELECT_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
protected:
	CBackStageActionInc*  m_pBackSever;
	CRole                 m_setRole;
    IntStringMap          m_selPower;
	IntStringMap          m_allPower;
	BOOL                  m_isNewRole;

	CButton*              m_pBtn;

	
};
