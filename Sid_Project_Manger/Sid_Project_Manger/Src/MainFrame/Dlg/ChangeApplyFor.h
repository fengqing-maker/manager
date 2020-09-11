#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "CComonDlg.h"
// CChangeApplyFor 对话框
#define SET_DLG_BTN_FIRST_ID  5000
class CChangeApplyFor : public CComonDlg
{
	DECLARE_DYNAMIC(CChangeApplyFor)

public:
	CChangeApplyFor(const CTaskObj &taskObj, const CTaskChange &cheObj, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangeApplyFor();

// 对话框数据
	enum { IDD = IDD_DLG_CHANGE_APPLYFOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeComboChangeType();
	afx_msg void OnDtnDatetimechangeDatePlanTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPanWorkHour();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnDelete();
	DECLARE_MESSAGE_MAP()
protected:
	void        OnInitStaticText();
	void        OnInitChangId();
	void        OnInitChangeType();
	void        OnInitOtherEdit();
	BOOL        OnInitReviewUser();
	BOOL        OnInitSecondReviewUser();
	CString     OnInitSysChangeContent();
	void        OnInitChangeResonOption();
	void        OnSetEnable();
	BOOL        GetChangeAndTask( CTaskObj &bakObj, CTaskChange &chObj );
	BOOL        SaveChangeAndTask( BOOL isAppFor );

	BOOL        GetBakMangerUser( const CUser  &userObj, CUser &bakUser,BOOL isSecond = FALSE );
protected:
	CComboBox m_changeTypeBox;
	CDateTimeCtrl m_panEndTimeCtrl;
	CEdit m_addWorkHourTask;
	CButton  m_resonOptionBtns[32];
	IntStringMap m_optionStrings;

	CTaskObj             m_bakTaskObj;
	CTaskObj             m_taskObj;
	CTaskChange          m_changeObj;
	//CActionServerFactory *m_pSever;
	//CTaskSeverInc        *m_pTaskSever;
	CChangeActionInc     *m_pChangeSever;



	
};
