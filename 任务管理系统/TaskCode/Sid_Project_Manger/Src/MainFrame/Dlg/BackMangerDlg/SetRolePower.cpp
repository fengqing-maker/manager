// SetRolePower.cpp : 实现文件
//

#include "stdafx.h"
#include "SetRolePower.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CSetRolePower 对话框

IMPLEMENT_DYNAMIC(CDlgSetRolePower, CDialogEx)

CDlgSetRolePower::CDlgSetRolePower(const  CRole *pRole, BOOL isNew /*= FALSE*/,CWnd* pParent /*=NULL*/)
	: CComonDlg(CDlgSetRolePower::IDD, pParent)
{
	m_isNewRole = isNew;
	m_pBackSever = m_pSever->GetBackStageSver();
	if ( !isNew )
	{
		if ( pRole )
		{
			m_setRole = *pRole;
			m_pBackSever->GetPowerByRoleId( m_setRole.GetId(), m_selPower );
		}
	}
	
	m_pBackSever->GetAllPower( m_allPower );

	m_pBtn = new CButton [ m_allPower.size() ];
	ASSERT( m_pBtn != NULL );
}

CDlgSetRolePower::~CDlgSetRolePower()
{
	delete [] m_pBtn;
}

void CDlgSetRolePower::DoDataExchange(CDataExchange* pDX)
{
	CComonDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetRolePower, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetRolePower::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetRolePower 消息处理程序
BOOL CDlgSetRolePower::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	if ( m_allPower.size() <= 0 )
	{
		AfxMessageBox(_T("初始化权力表失败"));
		return FALSE;
	}
	GetDlgItem( IDC_EDIT_ROLE_NAME )->EnableWindow( m_isNewRole );
	GetDlgItem( IDC_EDIT_DEC )->EnableWindow( m_isNewRole );
	if ( !m_isNewRole )
	{
		GetDlgItem( IDC_EDIT_ROLE_NAME )->SetWindowText( m_setRole.GetRoleName() );
		GetDlgItem( IDC_EDIT_DEC )->SetWindowText( m_setRole.GetRolDec() );
	}else
	{
		SetWindowText( _T("新建角色"));
	}
	
	CRect startRect;
	GetDlgItem( IDC_STATIC_DEC )->GetWindowRect( startRect );
	ScreenToClient(startRect);

	CRect rtFirst;
	int  wide = 180;
	int  hight = 20;
	rtFirst.top = startRect.top + 40;
	rtFirst.left = startRect.left + 10;
	rtFirst.bottom = rtFirst.top + hight;
	rtFirst.right = rtFirst.left + wide;

	int  horizDistance = 5;
	int  verticalDistance = 4;
	CRect temp = rtFirst;

	int count = m_allPower.size();
	IntStringMap::const_iterator itor = m_allPower.begin();
	IntStringMap::const_iterator findReslut;
	for( int i = 0; i < count && itor != m_allPower.end(); i++)
	{
		m_pBtn[i].Create( itor->second, BS_AUTOCHECKBOX , temp,
			this, IDC_STATIC_ROLE + i);

		temp.left = temp.right + horizDistance; //横间隔
		temp.right = temp.left + wide;
		if ((i % 4) == 3) //换行
		{
			temp.top = temp.bottom + verticalDistance;
			temp.bottom = temp.top + hight;
			temp.left = rtFirst.left;
			temp.right = rtFirst.right;
		}

		findReslut = m_selPower.find( itor->first );
		if ( findReslut != m_selPower.end() )
		{
			m_pBtn[i].SetCheck( TRUE );
		}else
		{
			m_pBtn[i].SetCheck( FALSE );
		}
		m_pBtn[i].EnableWindow( TRUE );
		m_pBtn[i].ShowWindow(SW_SHOW);
		itor++;
	}

	return TRUE;
}

void CDlgSetRolePower::OnBnClickedOk()
{
	IntStringMap::const_iterator itor = m_allPower.begin();
	int count = m_allPower.size();
	m_selPower.clear();
	for( int i = 0; i < count && itor != m_allPower.end(); i++, itor++)
	{
		if ( m_pBtn[i].GetCheck() )
		{
		   m_selPower.insert( std::make_pair( itor->first, itor->second) );
		}
	}
	if ( m_isNewRole )
	{
		CString str;
		GetDlgItem( IDC_EDIT_ROLE_NAME )->GetWindowText( str );
		m_setRole.SetRoleName( str );

		GetDlgItem( IDC_EDIT_DEC )->GetWindowText( str );
		m_setRole.SetRoleDec( str );
		BOOL ret = m_pBackSever->InsertRole( m_setRole, m_selPower );
		if ( ret == userExcOk )
		{
			CDialogEx::OnOK();
		}else if ( ret != paraError )
		{
			AfxMessageBox( m_pSever->GetErrorString(ret) );
		}
	}else
	{
		CDialogEx::OnOK();
	}
	
}
