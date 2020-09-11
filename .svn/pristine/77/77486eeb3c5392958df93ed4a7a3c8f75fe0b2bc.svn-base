// CModiyTaskScoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CModiyTaskScoreDlg.h"
#include "afxdialogex.h"


// CModiyTaskScoreDlg 对话框

IMPLEMENT_DYNAMIC(CModiyTaskScoreDlg, CComonDlg)

CModiyTaskScoreDlg::CModiyTaskScoreDlg(const CTaskObj &obj,CWnd* pParent /*=NULL*/)
	: CComonDlg(CModiyTaskScoreDlg::IDD, pParent),m_setTask(obj),m_processReview(_T(""))
{
	m_moidyScore = obj.GetScore();
	m_pScheduleSever = m_pSever->GetScheduleSever();
}

CModiyTaskScoreDlg::~CModiyTaskScoreDlg()
{
}

void CModiyTaskScoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEPART_SCORE, m_moidyScore);
	DDX_Text(pDX, IDC_EDIT_DEPART_REVIEW, m_processReview);
}


BEGIN_MESSAGE_MAP(CModiyTaskScoreDlg, CComonDlg)
	ON_BN_CLICKED(IDOK, &CModiyTaskScoreDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CModiyTaskScoreDlg 消息处理程序
BOOL CModiyTaskScoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString taskDc;
	taskDc.Format(_T("任务名:%s\r\n\r\n计划:%s-%s %s-%s"), 
		m_setTask.GetName(),
		m_setTask.GetPanStartDateShowStr(), m_setTask.GetPanEndDateShowStr(),
		m_setTask.GetFactStartDateShowStr(), m_setTask.GetFactEndDateShowStr() );
	GetDlgItem( IDC_EDIT_TASK_DEC )->SetWindowText( taskDc );

	GetDlgItem( IDC_STATIC_SCORE_DEC )->SetWindowText( _T("任务评分") );
	GetDlgItem( IDC_STATIC_DEC )->SetWindowText( _T("评分说明") );
	GetDlgItem( IDC_STATIC_SCORE_RANGE )->SetWindowText( _T("100~120") );
	SetWindowText( _T("修改任务评分") );
	UpdateData( FALSE );
	return TRUE;
}

void CModiyTaskScoreDlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	BOOL ret = m_pScheduleSever->OnModiyTaskScore( m_setTask, m_moidyScore, m_processReview );
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
