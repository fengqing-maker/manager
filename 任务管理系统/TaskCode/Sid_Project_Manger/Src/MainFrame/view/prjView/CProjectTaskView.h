#pragma once

#include "resource.h"
#include "../BaseFormView.h"
#include "../prjManger/projectTaskManger.h"
#include "afxwin.h"

class CProjectTaskView: public CBaseFormView
{
	DECLARE_DYNCREATE(CProjectTaskView)
public:
	CProjectTaskView();           // 动态创建所使用的受保护的构造函数
	virtual ~CProjectTaskView();

	void     SetSelProjectByPrjId( int prjId, int taskSt = -1 );
	ListTask*  GetTaskListPoint();
public:
	enum { IDD = IDD_FROMVIEW_TASK_PRJ };

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
	afx_msg void OnPrjoctProcessReview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int      CreateCtrl();
	void     InitFindCtrl();
	void     OnInitOwnerList();
private:
	CGridCtrl		m_taskGrid; //项目列表任务发布列表
	boost::scoped_ptr<CProjectTaskManger>   m_gridManger;
	int             m_selPrj;
	CActionServerFactory* m_pActionFactory;
	//
	CComboBox m_searchTask;
	CComboBox m_taskStatusCtrl;
	CComboBox m_ctrlOwnern;
	int       m_diffPerCondition;
	ListUser  m_lstUser;
public:
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnExport();


	afx_msg void OnBnClickedButtonSearch();
};
