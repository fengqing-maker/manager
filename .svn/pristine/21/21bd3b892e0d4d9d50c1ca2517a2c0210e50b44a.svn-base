#pragma once
#include "resource.h"
#include "../BaseFormView.h"
#include"../BackManger/UserGridManger.h"

class CUserMangerView: public CBaseFormView
{
	DECLARE_DYNCREATE(CUserMangerView)
public:
	CUserMangerView();           // 动态创建所使用的受保护的构造函数
	virtual ~CUserMangerView();

public:
	enum { IDD = IDD_USER_MANGER_VIEW };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange( CDataExchange* pDX);    // DDX/DDV 支持
	// Generated message map functions
	//{{AFX_MSG(CPublishTaskView)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnFresh();
	afx_msg void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnClearnUserPwd( void );
	afx_msg void OnDeleteUser( void );
	afx_msg void OnBnClickedBtnNewUser();
	afx_msg void OnCbnSelchangeComboFind();;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int      CreateCtrl();
	void     OnInitDepart();
private:
	CGridCtrl		m_taskGrid; //任务发布列表
	boost::scoped_ptr<CUserGridManger>   m_gridManger;
	CComboBox                            m_findTextBox;

};
