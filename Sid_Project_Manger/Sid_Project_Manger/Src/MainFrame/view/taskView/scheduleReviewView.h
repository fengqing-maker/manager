#pragma once

#include "../BaseFormView.h"
#include "../../ui/CGridCtrl/GridCtrl.h"
#include "../gridManger/scheduleManger.h"
#include <boost/scoped_ptr.hpp>
#include "Src/action/factory/actionServerFactory.h"
#include "afxwin.h"

/** @brief  进度评审 */
typedef std::map<int, CProject> ProjectMap;

class CScheduleReviewView : public CBaseFormView
{
	DECLARE_DYNCREATE(CScheduleReviewView)
public:
	CScheduleReviewView();           // 动态创建所使用的受保护的构造函数
	virtual ~CScheduleReviewView();

public:
	enum { IDD = IDD_FROMVIEW_SCHEDULE };

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
	int      CreateCtrl();
	void     InitSearchCondition(  BOOL isInit );
private:
	
	CGridCtrl		                         m_taskGrid;    //包括进度项目和部门审核
	boost::scoped_ptr<CTaskScheduleManger>   m_gridManger;
	CActionServerFactory*                    m_pActionFactory; 
	ProjectMap                               m_prjConditionMap;
	int                                      m_prjConditionPos;
	CComboBox                                m_findTextBox;
public:
	afx_msg void OnBnClickedBtnFresh();
	afx_msg void OnCbnEditchangeComboFind();
};
