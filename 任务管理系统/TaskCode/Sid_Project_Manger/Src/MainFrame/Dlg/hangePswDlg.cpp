// hangePswDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hangePswDlg.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ChangePswDlg dialog


ChangePswDlg::ChangePswDlg( CLoginActionInc *pSever, CWnd* pParent /*=NULL*/)
	: CDialog(ChangePswDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ChangePswDlg)
	m_nOriginalPsw = _T("");
	m_nModifyPsw1 = _T("");
	m_nModifyPsw2 = _T("");
	m_bCorrect = FALSE;
	//}}AFX_DATA_INIT
	ASSERT( NULL != pSever );
	m_pLoginSever = pSever;
}


void ChangePswDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ChangePswDlg)
	DDX_Text(pDX, IDC_ORIGINAL_PSW, m_nOriginalPsw);
	DDX_Text(pDX, IDC_MODIFY_PSW, m_nModifyPsw1);
	DDX_Text(pDX, IDC_MODIFY_CONFIRM, m_nModifyPsw2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ChangePswDlg, CDialog)
	//{{AFX_MSG_MAP(ChangePswDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChangePswDlg message handlers

void ChangePswDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	//char lstrPsw[256];
	CString str;
	if(m_nModifyPsw2 != m_nModifyPsw1)
	{
		MessageBox(_T("新密码两次输入不一致，请重新输入！"));
		return;
	}
	
	CString md5OrginPwd = CUtilityTool::GetObj()->GetMd5String( m_nOriginalPsw );
	const CUser &loginUser = m_pLoginSever->GetLoginUser();
	if( md5OrginPwd != loginUser.GetPwd() )
	{
		AfxMessageBox(_T("旧密码输入错误！"));
		UpdateData(FALSE);
		return;
	}
    BOOL ret;
	if( userExcOk != (ret = m_pLoginSever->SetLoginUserPwd( m_nModifyPsw1 )) )
	{
		CString info = CActionServerFactory::GetErrorString( ret );
		AfxMessageBox( info );
		return;
	}
	m_bCorrect = true;
	CDialog::OnOK();
}
