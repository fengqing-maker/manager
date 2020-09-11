// DlgNewProductLine.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgNewProductLine.h"
#include "../DlgUserChose.h"

// CDlgNewProductLine 对话框

IMPLEMENT_DYNAMIC(CDlgNewProductLine, CDialogEx)

CDlgNewProductLine::CDlgNewProductLine(CWnd* pParent /*=NULL*/)
	: CComonDlg(CDlgNewProductLine::IDD, pParent)
{
	m_prjSever = m_pSever->GetProjectSever();
}

CDlgNewProductLine::~CDlgNewProductLine()
{
}

void CDlgNewProductLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OWNER, m_ctrlOwner);
}


BEGIN_MESSAGE_MAP(CDlgNewProductLine, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CHOCE_OWNER, &CDlgNewProductLine::OnBnClickedBtnChoceOwner)
	ON_BN_CLICKED(IDOK, &CDlgNewProductLine::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDlgNewProductLine::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	return TRUE;
}
// CDlgNewProductLine 消息处理程序


void CDlgNewProductLine::OnBnClickedBtnChoceOwner()
{
	CDlgUserChose dlg( FALSE );
	if ( m_productLine.GetMangerId() > 0 )
	{
		dlg.SetDelSelUser( m_productLine.GetMangerId() );
	}
	if ( IDOK == dlg.DoModal() )
	{
		const CPtrArray* pUserArray = dlg.GetSelArrary();
		if ( 1 == pUserArray->GetCount() )
		{
			const CUser *pUser = (const CUser *)pUserArray->GetAt( 0 );
			m_productLine.SetMangerId( pUser->GetId() );
			m_ctrlOwner.SetWindowText( pUser->GetUserName_R() );
		}
	}
}


void CDlgNewProductLine::OnBnClickedOk()
{
	CString str;
	this->GetDlgItem( IDC_EDIT_NAME )->GetWindowText( str );
	m_productLine.SetName( str );

	str.ReleaseBuffer();
	GetDlgItem( IDC_EDIT_DEC  )->GetWindowText( str );
	m_productLine.SetDec( str );

	BOOL ret = m_prjSever->NewProductLine( m_productLine );
	if ( ret == userExcOk )
	{
		CDialogEx::OnOK();
	}else
	{
		AfxMessageBox( m_pSever->GetErrorString( ret ) );
	}
}
