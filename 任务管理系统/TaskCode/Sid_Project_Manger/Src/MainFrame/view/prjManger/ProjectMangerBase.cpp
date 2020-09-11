#include "stdafx.h"
#include "ProjectMangerBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPrjBaseGridManger::CPrjBaseGridManger(  CActionServerFactory *pSeverFactory )
	:CBaseGridManger(pSeverFactory)
{
	m_pProjectSever = m_pSeverFactory->GetProjectSever();
}
void CPrjBaseGridManger::InsertPrjNameColumn()
{
	InsertEditColumn(  _T("项目简称"), 100, DT_LEFT,ProjectType_Colom );
}
void    CPrjBaseGridManger::InsertPrjLongNameColumn()
{
	InsertEditColumn(  _T("项目全名"), 300, DT_LEFT, PrjLongName );
}
void    CPrjBaseGridManger::InsertPrjStepType()
{
	InsertEditColumn(_T("研发类型"), 100, DT_LEFT,  PrjStepType );
}
void    CPrjBaseGridManger::InsertPrjLine()
{
	InsertColumn(_T("所属产品线"), 100, DT_LEFT, GridItemType_Combo,  PrjLine );
}
void    CPrjBaseGridManger::InsertPrjStep()
{//实际所属阶段
	InsertEditColumn(  _T("所属阶段"), 100, DT_LEFT,PrjNowStep );
}
void    CPrjBaseGridManger::InsertBegongToStep()
{//应属阶段
	InsertEditColumn(  _T("应属阶段"), 100, DT_LEFT,PrjBelongToStep );
}

//void    CPrjBaseGridManger::InsertPrjDc()
//{
//	InsertEditColumn(  _T("项目描述"), 100, DT_LEFT, PrjDC );
//}
void    CPrjBaseGridManger::InsertPrjMember()
{
	InsertColumn( _T("项目成员"), 350, DT_LEFT, GridItemType_MutilEdit,  PrjMember );
}

CProject* CPrjBaseGridManger::GetProjectByRow( int nRow, ListProject &lst, const IntToIntMap &showMap )
{
	if ( lst.size() <= 0)
	{
		return NULL;
	}
	IntToIntMap::const_iterator dataItor = showMap.find( nRow );
	if ( dataItor != showMap.end() && dataItor->second >= 0 )
	{
		ListProject::iterator itor = lst.begin();
		advance( itor, dataItor->second );
		if ( itor == lst.end() )
		{
			return NULL;
		}
		return &(*itor);
	}
	return NULL;
}
void   CPrjBaseGridManger::GetProjectLineNameById( int lineId, CString &str )
{
	CProductLine line;
	if ( userExcOk == m_pProjectSever->GetProjectLineById( lineId, line ) )
	{
		str = line.GetName();
	}
}
//
BOOL    CPrjBaseGridManger::GetProjectMemberString( int prjId, CString &str )
{
	ListUser lstUser;
	if ( userExcOk == m_pProjectSever->GetProjectMember(prjId, lstUser))
	{
		ListUserItor itor = lstUser.begin();
        int count = 0; 
		while( itor != lstUser.end() )
		 {
			 CString name = GetOwnerNameStr( itor->GetId() );
			 str += name + _T("   ");
             if ( count > 0 && 0 == count%5 )
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
void    CPrjBaseGridManger::UpdateProject()
{
	m_allProject.clear();
	const CUser& loginUser = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	BOOL hasPowerReadAll = m_pSeverFactory->GetLoginServer()->FindLoginPower( LookUpAllPrj );
	m_pProjectSever->GetProjectByUser( loginUser, hasPowerReadAll, m_allProject );

}