#pragma once

#include "resource.h"
#include "../BaseFormView.h"
#include "../DepartManger/DepartTaskManger.h"
#include "afxwin.h"

class CDepartTaskView: public CBaseFormView
{
	DECLARE_DYNCREATE(CDepartTaskView)
public:
	CDepartTaskView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDepartTaskView();

	void       SetSelDepartById( int prjId, int taskSt = -1 );
	ListTask*  GetTaskListPoint();
public:
	enum { IDD = IDD_FROMVIEW_TASK_DEPART };

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
	afx_msg void OnCbnSelchangeComboTaskStatus();
	afx_msg void OnCbnSelchangeComboFind();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridClick( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnRclickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnDepartReview();
	afx_msg void OnMoidyScore();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int      CreateCtrl();
	void     OnInitCtrl();
	void     OnInitOwnerList();

private:
	CGridCtrl		m_taskGrid; //项目列表任务发布列表
	boost::scoped_ptr<CDepartTaskManger>   m_gridManger;
	int             m_selPrj;
	CActionServerFactory* m_pActionFactory;
	ListUser              m_lstUser;
	//
	CComboBox m_ctrlDepart;
	CComboBox m_taskStatusCtrl;
    CComboBox m_ctrlOwnern;
	int       m_diffPerCondition;

	UINT      m_editSerarchYear;
	CComboBox m_comboQuarter;
public:
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedBtnExportTable();

};
