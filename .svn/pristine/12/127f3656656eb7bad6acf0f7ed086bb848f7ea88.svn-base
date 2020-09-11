#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口
#include "../ui/CMultiLineListCtrl/MultiLineListBox.h"
#include "systemOutput/SvrOutputList.h"

class COutputList : public CMultiLineListBox
{
// 构造
public:
	COutputList();

// 实现
public:
	virtual ~COutputList();
	
protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	//afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};
#include "Src/action/factory/actionServerFactory.h"

class COutputWnd : public CDockablePane
{
// 构造
public:
	COutputWnd();

	void SetActionFactory( CActionServerFactory *pFactory ){ m_pSeverFactory = pFactory;};
	void UpdateFonts();

	void FillTaskWindow( int taskId );
	void FillLoginWindow();
	void     AdjustLayout();

// 特性
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList  m_wndTaskInfo;
	CListBox     m_wndLoginInfo;
    CSvrOutputList m_svrOutputList; //系统输出列表

	
protected:
	
	

	void AdjustHorzScroll(CListBox& wndListBox);

// 实现
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSelectSystemOutput( WPARAM wpara, LPARAM lpara );
	DECLARE_MESSAGE_MAP()
protected: 
	CActionServerFactory *m_pSeverFactory;
	int                   m_showTaskId;
};

