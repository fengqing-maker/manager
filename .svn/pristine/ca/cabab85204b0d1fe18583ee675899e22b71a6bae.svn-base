// DepartProcessReview.cpp : 实现文件
//

#include "stdafx.h"
#include "DepartProcessReview.h"
#include "afxdialogex.h"


// DepartProcessReview 对话框

IMPLEMENT_DYNAMIC(DepartProcessReview, CComonDlg)

DepartProcessReview::DepartProcessReview(const CTaskObj &obj, const CString &newSchedul,CWnd* pParent /*=NULL*/)
	: CComonDlg(DepartProcessReview::IDD, pParent),m_setTask(obj), m_strSchedul(newSchedul)
	, m_processScore(0)
	, m_processReview(_T(""))
{
	m_pScheduleSever = m_pSever->GetScheduleSever();
}

DepartProcessReview::~DepartProcessReview()
{
}

void DepartProcessReview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEPART_SCORE, m_processScore);
	DDX_Text(pDX, IDC_EDIT_DEPART_REVIEW, m_processReview);
}


BEGIN_MESSAGE_MAP(DepartProcessReview, CComonDlg)
	ON_BN_CLICKED(IDOK, &DepartProcessReview::OnBnClickedOk)
END_MESSAGE_MAP()


// DepartProcessReview 消息处理程序
BOOL DepartProcessReview::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString taskDc;
	taskDc.Format(_T("任务名:%s\r\n最新进度:%s\r\n计划:%s-%s %s-%s"), 
		m_setTask.GetName(), m_strSchedul.IsEmpty()?_T("无"):m_strSchedul,
		m_setTask.GetPanStartDateShowStr(), m_setTask.GetPanEndDateShowStr(),
		m_setTask.GetFactStartDateShowStr(), m_setTask.GetFactEndDateShowStr() );
	GetDlgItem( IDC_EDIT_TASK_DEC )->SetWindowText( taskDc );
	m_processScore = 0;
	UpdateData( FALSE );
	return TRUE;
}

void DepartProcessReview::OnBnClickedOk()
{
	UpdateData( TRUE );
	BOOL ret = m_pScheduleSever->OnReviewSchedule( m_setTask, m_pLoginAction->GetLoginUser().GetUserName_R(),m_processReview, m_processScore );
	if ( ret == userExcOk )
	{
		CDialogEx::OnOK();
		return;
	}
	if ( ret != paraError )
	{
		AfxMessageBox( m_pSever->GetErrorString(ret) );
	}
}
