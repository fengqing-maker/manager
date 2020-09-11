#include "stdafx.h"
#include "DepartMangerBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDepartBaseGridManger::CDepartBaseGridManger(  CActionServerFactory *pSeverFactory )
	:CBaseGridManger(pSeverFactory)
{
	m_pDepartSever = m_pSeverFactory->GetDepartSever();
}
void CDepartBaseGridManger::InsertDepartNameColumn()
{
	InsertEditColumn(  _T("部门名称"), 150, DT_LEFT, DepartName );
}
void   CDepartBaseGridManger::InsertFahterDepart()
{
	InsertColumn( _T("上级部门"), 150, DT_LEFT, GridItemType_Combo,  DepartFather );
}

void CDepartBaseGridManger::InsertDepartDecColumn()
{
	InsertEditColumn(  _T("部门描述"), 200, DT_LEFT, DepartDec );
}
void      CDepartBaseGridManger::InsertAgentUser()
{
	InsertEditColumn(  _T("代理人"), 100, DT_LEFT, DepartAgentUser );
}
void    CDepartBaseGridManger::InsertAgentValidate()
{
	InsertColumn(_T("代理是否生效"), 100, DT_LEFT, GridItemType_Check,  DepartAgentValidate );
}

void    CDepartBaseGridManger::InsertDepartMember()
{
	InsertColumn( _T("部门成员"), 450, DT_LEFT, GridItemType_MutilEdit,  DepartMember );
}

CDepartment* CDepartBaseGridManger::GetDepartByRow(	int nRow, ListDepart &lst )
{
	if ( lst.size() <= 0 || nRow < 0 )
	{
		return NULL;
	}

	ListDepart::iterator itor = lst.begin();
	advance( itor, nRow );
	if ( itor == lst.end() )
	{
		return NULL;
	}
	return &(*itor);

}
void   CDepartBaseGridManger::GetDepartNameById( int id, CString &str )
{
	CDepartment obj;
	if ( userExcOk == m_pDepartSever->GetDepartById( id, obj ) )
	{
		str = obj.GetName();
	}
}
//
BOOL    CDepartBaseGridManger::GetDepartMemberString( int prjId, CString &str )
{
	ListUser lstUser;
	if ( userExcOk == m_pDepartSever->GetDepartUser( prjId, lstUser, FALSE ) )
	{
		ListUserItor itor = lstUser.begin();
        int count = 0; 
		while( itor != lstUser.end() )
		 {
			 CString name = GetOwnerNameStr( itor->GetId() );
			 str += name + _T("   ");
             if ( count > 0 && 0 == count%8 )
             {
				 str += _T("\r\n");
             }
			 count++;
			 itor++;
		 }
		return TRUE;
	}
	return FALSE;
}
void    CDepartBaseGridManger::UpdateDepart()
{
	m_allDepart.clear();
	const CUser& loginUser = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	CString roleName = m_pSeverFactory->GetLoginServer()->GetLoginRole().GetRoleName();
	m_pDepartSever->GetDepartByMangerId( loginUser.GetId(), roleName, m_allDepart );
}