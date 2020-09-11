#pragma once
#include "resource.h"
#include "../BaseFormView.h"
#include"../DepartManger/DepartManger.h"

class CDepartView: public CBaseFormView
{
	DECLARE_DYNCREATE(CDepartView)
public:
	CDepartView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDepartView();

public:
	enum { IDD = IDD_DEPART_VIEW };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange( CDataExchange* pDX);    // DDX/DDV ֧��
	// Generated message map functions
	//{{AFX_MSG(CPublishTaskView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnFresh();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridClick( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult );
	afx_msg void OnChangeOwner( void );
	afx_msg void OnChangeBakOwner( void );
	afx_msg void OnDeleteDepart( void );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int      CreateCtrl();
	void     OnInitCtrl();
private:
	/** @brief ���񷢲�����  */
	CGridCtrl		m_taskGrid; //���񷢲��б�
	boost::scoped_ptr<CDepartViewManger>   m_gridManger;
	
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnNewDepart();
};
