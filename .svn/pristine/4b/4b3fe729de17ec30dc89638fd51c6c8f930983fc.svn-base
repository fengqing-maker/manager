#pragma once

#include "resource.h"
#include "../ui/CGridCtrl/GridCtrl.h"
// 
class CBaseFormView : public CFormView
{
	DECLARE_DYNCREATE(CBaseFormView)
public:
	BOOL     CreateFormCtrl(UINT nID, CWnd *pParent);
	virtual void OnInitialUpdate();
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:

	CBaseFormView( UINT id );           // 动态创建所使用的受保护的构造函数
	virtual ~CBaseFormView();
	virtual void DoDataExchange( CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	// Generated message map functions
	//{{AFX_MSG(CEventView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	void       OnSize( UINT nType, int cx, int cy,CGridCtrl &ctrObj );
protected:
	BOOL  m_bFirstActive;
private:
	CBaseFormView( );
	
	

  
};