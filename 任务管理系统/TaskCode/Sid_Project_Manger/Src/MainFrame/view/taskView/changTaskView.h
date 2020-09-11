#pragma once

#include "resource.h"
#include "../BaseFormView.h"
#include "../../ui/CGridCtrl/GridCtrl.h"
#include "../gridManger/changeReviewManger.h"
#include <boost/scoped_ptr.hpp>
#include "afxwin.h"
// 
class CChangeTaskView : public CBaseFormView
{
	DECLARE_DYNCREATE(CChangeTaskView)
public:
	CChangeTaskView();           // 动态创建所使用的受保护的构造函数
	virtual ~CChangeTaskView();

public:
	enum { IDD = IDD_FROMVIEW_CHANGE };

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
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeComboFind();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );

	afx_msg LRESULT OnAddCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnContentChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnSelGridLineView( WPARAM wp, LPARAM lp );
	//}}AF	X_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int          CreateCtrl();
	void         OnInitCtrl();
private:
  /** @brief  任务评审*/
  CGridCtrl		                         m_taskGrid; //一级审核
  CComboBox                              m_searchTask;
  boost::scoped_ptr<CChangeReviewManger>   m_gridManger;

public:
	afx_msg void OnBnClickedBtnFresh();
};
