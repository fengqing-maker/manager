#include "stdafx.h"
#include "CComonDlg.h"
#include "../MainFrm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CComonDlg, CDialogEx)

CComonDlg::CComonDlg( UINT nIDTemplate, CWnd *pParent)
	        : CDialogEx(nIDTemplate, pParent)
{
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != m_pSever );
	m_pTaskSever = m_pSever->GetTaskSever();
	m_pLoginAction = m_pSever->GetLoginServer();

}
CComonDlg::~CComonDlg()
{
		
}
BEGIN_MESSAGE_MAP(CComonDlg, CDialogEx)
END_MESSAGE_MAP()

//BOOL  CComonDlg::IsHasPower( int userId, int power )
//{
//	//BOOL         GetUserPower( int userId, RolePowerList &listObjs ) 
//}