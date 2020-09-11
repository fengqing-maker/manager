#pragma once

#include "resource.h"
#include "../../action/factory/actionServerFactory.h"
#include "SRC/MainFrame/ui/BluFontBotton.h"
#include "SRC/MainFrame/ui/NewEdit.h"
#include "afxwin.h"

// CChangeReview 对话框
#define  SET_DLG_BTN_FIRST_ID 5000
class CChangeReviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeReviewDlg)

public:
	CChangeReviewDlg(const CTaskObj &taskObj, const CTaskChange &cheObj, BOOL onlyRead = FALSE, 
		             CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangeReviewDlg();
// 对话框数据
	enum { IDD = IDD_DLG_CHANGE_REVIEW };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	//virtual HBRUSH OnCtlColor( CDC *pDCM, CWnd *pWnd, UINT nCtrlColor );
	afx_msg void OnBnClickedBtnAgrre();
	afx_msg void OnBnClickedBtnUnagree();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
protected:
	void         OnInitStaticText();
	void         OnInitDepartReview();
	void         OnInitGeneralReview();
	void         OnInitProjectReview();
	void         OnInitChangId();
	void         OnInitChangeResonOption();
	void         OnInitHistoryChange();
	void         OnHideReviewWnd();
	void         OnHideGeneralReviewWnd();
	void         OnHideProjectView();
private: 
	CActionServerFactory *m_pSever;
	CTaskSeverInc        *m_pTaskSever;
	CChangeActionInc     *m_pChangeSever;
	CTaskObj             m_taskObj;
	CTaskChange          m_changeObj;
	BOOL                 m_readOnly;
	BOOL                 m_isPrjReview;
	CButton  m_resonOptionBtns[32];
	IntStringMap m_optionStrings;

	CEdit m_firstReviewEdit;
	CEdit m_secondReviewEdit;
	CNewEdit m_eidtPrjReview;
	//CButton m_checkImpactBtn;
	CBluFontBotton m_ridoYes;
	CButton m_ridoNo;
	CFont   m_font;
	CFont   m_font1;

public:
	afx_msg void OnBnClickedBtnShowPictrue();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
