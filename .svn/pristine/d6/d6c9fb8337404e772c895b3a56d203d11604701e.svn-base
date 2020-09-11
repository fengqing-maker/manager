#pragma once

#include "../BaseFormView.h"
#include "../../ui/CGridCtrl/GridCtrl.h"
#include "../gridManger/WorkHourManger.h"
#include <boost/scoped_ptr.hpp>
#include "Src/action/factory/actionServerFactory.h"
#include "afxwin.h"

class CWorkHourView : public CBaseFormView
{
	DECLARE_DYNCREATE(CWorkHourView)
public:
	CWorkHourView();           // 动态创建所使用的受保护的构造函数
	virtual ~CWorkHourView();

	void    RefreshShow(); //刷新显示
public:
	enum { IDD = IDD_FROMVIEW_WORKHOUR };

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
	afx_msg LRESULT OnCheckWorkHour( WPARAM wp, LPARAM lp );

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int      CreateCtrl();
	void     OnInitMonth();
	void     InitSearchCondition();
	void     OnInitUserName();
	void     OnSelDateChange();
private:
	CGridCtrl		                         m_taskGrid;    //包括进度项目和部门审核
	boost::scoped_ptr<CWorkHourManger>       m_gridManger;
	CActionServerFactory*                    m_pActionFactory; 
	CComboBox                                m_findTextBox;
	CComboBox                                m_monthBox;
	COleDateTime                             m_currDate; //当天日期
	CUser                                    m_showUser;
	int                                      m_fristWeekPos; //一个月第一周的索引号
	BOOL                                     m_assginWorkHour;
	int                                      m_selYear;
public:
	afx_msg void OnBnClickedBtnFresh();
	afx_msg void OnCbnEditchangeComboFind();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedBtnCheck();
	afx_msg void OnCbnSelchangeComboYear();
	afx_msg void OnCbnSelchangeComboMonth();

	afx_msg void OnBnClickedBtnExportHour();
	afx_msg void OnBnClickedSetDefDate();
};
