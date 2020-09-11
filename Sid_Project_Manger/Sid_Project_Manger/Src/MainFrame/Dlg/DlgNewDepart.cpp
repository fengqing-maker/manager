// DlgNewDepart.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgNewDepart.h"
#include "afxdialogex.h"
#include "DlgUserChose.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgNewDepart 对话框

IMPLEMENT_DYNAMIC(CDlgNewDepart, CComonDlg)

CDlgNewDepart::CDlgNewDepart(CWnd* pParent /*=NULL*/)
	: CComonDlg(CDlgNewDepart::IDD, pParent)
{
	m_pDepartSever = m_pSever->GetDepartSever();
}

CDlgNewDepart::~CDlgNewDepart()
{
}

void CDlgNewDepart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FATHER_DEPART, m_ctrlFatherDepart);
	DDX_Control(pDX, IDC_EDIT_OWNER, m_ctrlManger);
	DDX_Control(pDX, IDC_EDIT_OWNER_BAK, m_ctrlBakManger);
	DDX_Control(pDX, IDC_CHECK_BAK_VALIDATE, m_btnBakValidate);
}


BEGIN_MESSAGE_MAP(CDlgNewDepart, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgNewDepart::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CHOCE_OWNER, &CDlgNewDepart::OnBnClickedBtnChoceOwner)
	ON_BN_CLICKED(IDC_BTN_CHOCE_BAK_OWNER, &CDlgNewDepart::OnBnClickedBtnChoceBakOwner)
END_MESSAGE_MAP()


// CDlgNewDepart 消息处理程序
BOOL CDlgNewDepart::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	OnInitFatherDepart();

	return TRUE;
}

void CDlgNewDepart::OnInitFatherDepart()
{
    m_ctrlFatherDepart.ResetContent();
	m_ctrlFatherDepart.InsertString( 0, _T("无") );
	
    m_pDepartSever->GetAllDepart( m_lstDeparts );
	int count = 1;
	for(ListDepartConstItor itor = m_lstDeparts.begin(); itor != m_lstDeparts.end(); itor++ )
	{
		m_ctrlFatherDepart.InsertString(count++, itor->GetName() );
	}
	m_ctrlFatherDepart.SetCurSel( 0 );
}


void CDlgNewDepart::OnBnClickedOk()
{
	//责任人
	if ( m_newDepart.GetMangerId() <= 0 )
	{
		AfxMessageBox( _T("部门经理不能为空，请选择部门经理人") );
		return;
	}
	CString str;
	GetDlgItem( IDC_EDIT_DEPART_NAME )->GetWindowText( str );
	if ( str.IsEmpty() )
	{
		AfxMessageBox( _T("部门名称不能为空") );
		return; 
	}
	CDepartment tempObj;
	BOOL ret = m_pDepartSever->GetDepartByName( str, tempObj );
	if ( ret == userExcOk )
	{
		AfxMessageBox( _T("部门名称已存在") );
		return; 
	}
	if ( ret != notfind )
	{
		AfxMessageBox( m_pSever->GetErrorString(ret) );
		return;
	}
	m_newDepart.SetName( str );
	
	GetDlgItem( IDC_EDIT_DEPART_DEC )->GetWindowText( str );
	m_newDepart.SetDec( str );

	//上级部门
	int selIndex = m_ctrlFatherDepart.GetCurSel();
	int selFatherId = 0;
	if ( selIndex > 0 && m_lstDeparts.size() > 0)
	{
		ListDepartConstItor itor = m_lstDeparts.begin();
		advance( itor, selIndex-1 );
		if ( itor != m_lstDeparts.end() )
		{
			selFatherId = itor->GetId();
		}
	}
	m_newDepart.SetTopDepartId( selFatherId );
	//
	m_newDepart.SetBakValidate(  m_btnBakValidate.GetCheck() );
	ret = m_pDepartSever->InsertDepart( m_newDepart );
	if ( ret == userExcOk )
	{
		CDialogEx::OnOK();
	}else
	{
		AfxMessageBox( m_pSever->GetErrorString(ret) );
	}
}


void CDlgNewDepart::OnBnClickedBtnChoceOwner()
{
	CDlgUserChose dlg( FALSE );
	if ( m_newDepart.GetMangerId() > 0 )
	{
		dlg.SetDelSelUser( m_newDepart.GetMangerId() );
	}
	if ( IDOK == dlg.DoModal() )
	{
		const CPtrArray* pUserArray = dlg.GetSelArrary();
		if ( 1 == pUserArray->GetCount() )
		{
			const CUser *pUser = (const CUser *)pUserArray->GetAt( 0 );
			m_newDepart.SetMangerId( pUser->GetId() );
			m_ctrlManger.SetWindowText( pUser->GetUserName_R() );
		}
	}
}


void CDlgNewDepart::OnBnClickedBtnChoceBakOwner()
{
	CDlgUserChose dlg( FALSE );
	if ( m_newDepart.GetBakManger() > 0 )
	{
		dlg.SetDelSelUser( m_newDepart.GetBakManger() );
	}
	if ( IDOK == dlg.DoModal() )
	{
		const CPtrArray* pUserArray = dlg.GetSelArrary();
		if ( 1 == pUserArray->GetCount() )
		{
			const CUser *pUser = (const CUser *)pUserArray->GetAt( 0 );
			m_newDepart.SetBakManger( pUser->GetId() );
			m_ctrlBakManger.SetWindowText( pUser->GetUserName_R() );
		}else if ( 0== pUserArray->GetCount()  )
		{
			m_newDepart.SetBakManger( -1 );
		}
	}
}
