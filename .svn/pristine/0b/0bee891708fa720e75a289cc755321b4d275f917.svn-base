#pragma once
#include "resource.h"
#include "../BaseFormView.h"
#include"../BackManger/ProductLineGridManger.h"

class CProductLineView: public CBaseFormView
{
	DECLARE_DYNCREATE(CProductLineView)
public:
	CProductLineView();           // 动态创建所使用的受保护的构造函数
	virtual ~CProductLineView();

public:
	enum { IDD = IDD_PRODECT_LINE_VIEW };

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
	afx_msg void OnEditOwner( void );
	afx_msg void OnDeleteProductLine( void );

	afx_msg void OnBnClickedBtnNew();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int      CreateCtrl();
	void     OnInitDepart();
private:
	CGridCtrl		m_taskGrid; //任务发布列表
	boost::scoped_ptr<CProductLineGridManger>   m_gridManger;

};
