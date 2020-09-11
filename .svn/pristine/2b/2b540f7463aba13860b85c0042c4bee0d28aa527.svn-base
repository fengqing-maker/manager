#pragma once

#include "resource.h"
#include "../../action/factory/actionServerFactory.h"
#include "afxwin.h"
#include <map>
#include "afxdtctl.h"
// CNewTaskDlg 对话框
typedef std::map<int, const CUser *> ListConstUserMap;
typedef std::map<int, CProject> ListPrjMap;

class CNewTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewTaskDlg)

public:
	CNewTaskDlg(const CTaskObj *pObj /*= NULL*/,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewTaskDlg();

	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DLG_NEW_TASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnSaveSend();
	afx_msg void OnCbnSelchangeComboOwner();
	afx_msg void OnCbnSelchangeComboTaskType();
	afx_msg void OnCbnSelchangeComboProjectType();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitTaskTypeList();
	void OnInitReviwerUser();
	BOOL OnInitOwernCtrl();
	void OnInitPrjBox();
	void OnInitWorkHourBox();
	void OnInitFatherBox();
	void OnInitPrjStepBox();
	BOOL GetTaskFromView(CTaskObj &obj );
	BOOL UpdateFatherTask(const CTaskObj &obj );
protected:
	CComboBox m_taskTypeBox;
	CComboBox m_publishUserBox;
	CComboBox   m_owerUserBox;
	CComboBox   m_prjTextBox;
	CComboBox  m_workHourBox;
	CComboBox   m_prjStepBox;
	CEdit      m_taskContentEdit;
	CEdit      m_decText;
	CComboBox  m_fatherTaskBox;
	CDateTimeCtrl m_startDateTimeCtrl;
	CDateTimeCtrl m_endTimeCtrl;
	CButton m_ctrlSelQualityTask;

	int            m_lastPrjSel;
	
	ListConstUserMap m_publishUser;
	ListUser         m_owerUser;
	ListProject      m_prjList;
	const CTaskObj   *m_pOldTask;
	CTaskObj         m_newTask;
	ListTask         m_fatherTaskList;
	ListProjectStep  m_listStep;
private: 
	CActionServerFactory *m_pSever;
	CTaskSeverInc        *m_pTaskSever;
	CLoginActionInc      *m_pLoginSever;




};
