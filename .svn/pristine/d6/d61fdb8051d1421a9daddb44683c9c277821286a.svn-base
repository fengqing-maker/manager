#pragma once

#include "resource.h"
#include "../BaseFormView.h"
#include <boost/scoped_ptr.hpp>
#include "../gridManger/scoreTaskManger.h"
#include "../../ui/CGridCtrl/GridCtrl.h"

// CScoreTaskView 任务完成评分

class CScoreTaskView: public CBaseFormView
{
	DECLARE_DYNCREATE(CScoreTaskView)
public:
	CScoreTaskView();           // 动态创建所使用的受保护的构造函数
	virtual ~CScoreTaskView();

public:
	enum { IDD = IDD_FORMVIEW_SCORE };

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeComboFind();
	afx_msg void OnBnClickedBtnFresh();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );

	afx_msg LRESULT OnAddCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnContentChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnSelGridLineView( WPARAM wp, LPARAM lp );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int        CreateCtrl();
private:
	CGridCtrl		                         m_taskGrid;    //任务完成进行评分
	boost::scoped_ptr<CScoreTaskManger>     m_gridManger;
	CComboBox                                 m_searchTask;
};
