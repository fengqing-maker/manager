#pragma once

#include "resource.h"
#include "../BaseFormView.h"
#include "Src/action/factory/actionServerFactory.h"
#include "../gridManger/myTaskGirdManager.h"
#include <boost/scoped_ptr.hpp>
#include "afxwin.h"

//我创建的任务，我负责的任务，
class CMyTaskView : public CBaseFormView
{
	DECLARE_DYNCREATE(CMyTaskView)
public:
	CMyTaskView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyTaskView();

public:
	enum { IDD = IDD_FORMVIEW_MYTASK };

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
	//{{AFX_MSG(CEventView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	BOOL CreateCtrl();
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnNewTask();
	afx_msg void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnRclickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnDeleteTask();
	afx_msg void OnApplyForChangeTask();
	afx_msg void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );

	afx_msg LRESULT OnAddCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnContentChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnSelGridLineView( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnUpdateSelDateList( WPARAM wp, LPARAM lp );
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnExportHour();
	afx_msg void OnBnClickedBtnUpdate();
protected:
	void  ResetSelDateList();
private:
	/** @brief 我的任务网格 */
	CGridCtrl			                 m_myTaskGrid;
	CActionServerFactory                 *m_pActionFactory;
	boost::scoped_ptr<CTaskGridManager>   m_gridManger; // 
	CComboBox                             m_searchTextBox;
	CComboBox                             m_comboUpdateDate;

	int m_editSerarchYear;
	CComboBox m_comboQuarter;
public:
	afx_msg void OnCbnSelchangeComboUpdatedAte();
};
