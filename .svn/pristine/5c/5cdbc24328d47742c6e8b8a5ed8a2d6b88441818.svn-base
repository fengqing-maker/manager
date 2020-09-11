#include "stdafx.h"
#include "projectSever.h"
#include <Src/MainFrame/Dlg/DlgUserChose.h>

CProjectSever::CProjectSever(  CDaoFactory  *pObj ):CProjectSeverInc( pObj )
{
	m_pDaoFactory = pObj;
	m_prjDbDao = pObj->GetPrjDbDao();
	m_systemCfgDbDao = pObj->GetSystemCfgDbDao();
}
BOOL CProjectSever::GetProjectByManger( int mangerId, ListProject& obj )
{
	return m_prjDbDao->GetProjectByMangerId( mangerId, obj );
}
//参与或管理的项目
BOOL CProjectSever::GetProjectByMember( int memberId, ListProject &obj ) 
{
	return m_prjDbDao->GetTaskPartProject( memberId, obj );
}
BOOL CProjectSever::GetProjectByLine( int lineId, ListProject &obj ) 
{
	return m_prjDbDao->GetProjectByLine( lineId, obj );
}
BOOL CProjectSever::GetAllProject( ListProject &list )
{
	return m_prjDbDao->GetAllProject( list );
}

BOOL CProjectSever::GetProjectMember( int prjId, ListUser &obj )
{
	return m_pDaoFactory->GetUserDao()->GetPrjectMember( prjId, obj );
}
 BOOL CProjectSever::GetProjectById( int prjId, CProject& obj)
 {
	 return m_prjDbDao->GetPrjectById(prjId, obj );
 }
 BOOL CProjectSever::GetProjectByUser( const CUser &useObj, BOOL isPowerReadAll, ListProject &list )
 {
	if ( isPowerReadAll )
    {
	   return m_prjDbDao->GetAllProject( list );
	}
	GetProjectByMember( useObj.GetId(), list );
	ListProject mangerList;
	BOOL ret = m_prjDbDao->GetProjectByMangerId( useObj.GetId(), mangerList );
	
	ListProjectLine lstLine;
	ret = m_prjDbDao->GetProjectLineByMangerId( useObj.GetId(), lstLine );
    if ( userExcOk != ret )
    {
		return ret;
    }
	ListProjectLine::iterator itor = lstLine.begin();
	while( itor != lstLine.end() )
	{
		m_prjDbDao->GetProjectByPrjLineNotManger(itor->GetId(), useObj.GetId(), mangerList );
		itor++;
	}
	for ( ListProject::iterator mangerItor= mangerList.begin(); mangerItor != mangerList.end(); mangerItor++ )
	{
		ListProject::const_iterator prjItor = list.begin();
		while( prjItor != list.end() )
		{
			if ( prjItor->GetId() == mangerItor->GetId() )
			{
				break;
			}
			prjItor++;
		}
		if ( prjItor == list.end() )
		{
			list.push_front( *mangerItor );
		}
	}
	

	return userExcOk;
 }
BOOL CProjectSever::GetAllProjectLine( ListProjectLine &list )
{
	return m_prjDbDao->GetAllProjectLine( list );
}
BOOL CProjectSever::GetProjectLineById( int lineId, CProductLine &obj )
{
	return m_prjDbDao->GetProjectLineById( lineId, obj );
}
BOOL  CProjectSever::UpdatePrjShortName( int prjId, const CString &str )
{
	if ( str.IsEmpty()  )
	{
		AfxMessageBox(_T("名称不能为空"));
		return paraError;
	}
	CProject obj;
	BOOL ret = m_prjDbDao->GetProjectByName( str, obj );
	if ( ret != notfind )
	{
		if ( userExcOk == ret )
		{
			AfxMessageBox(_T("项目名称已存在,请重新修改") );
			return paraError;
		}
		return ret;
	}
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("'%s'"), str );
	filds.insert( std::make_pair( fild_project_shortName, strValue ));
	return m_prjDbDao->UpdateProjectField(prjId ,filds );
}
BOOL CProjectSever::UpdatePrjLongName( int prjId, const CString &str )
{
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("'%s'"), str );
	filds.insert( std::make_pair( fild_project_name, strValue ));
	return m_prjDbDao->UpdateProjectField(prjId ,filds );
}
BOOL CProjectSever::UpdatePrjProductLine( int prjId, int lineId )
{
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("%d"), lineId );
	filds.insert( std::make_pair( fild_line_id, strValue ));
	return m_prjDbDao->UpdateProjectField(prjId ,filds );
}
BOOL CProjectSever::UpdatePrjOwner( const CProject &prj )
{
	CDlgUserChose dlg( FALSE );
	dlg.SetDelSelUser( prj.GetMangerId() );
	dlg.DoModal();
	const CPtrArray* pUser = dlg.GetSelArrary();
	if ( 1 == pUser->GetCount()  )
	{
		int selUserId = ( (const CUser*)pUser->GetAt( 0 ))->GetId();
		if ( selUserId != prj.GetMangerId() )
		{
			CString strValue;
			TaskIdStringMap filds;
			strValue.Format( _T("%d"), selUserId );
			filds.insert( std::make_pair( fild_project_managerid, strValue ));
			return m_prjDbDao->UpdateProjectField(prj.GetId() ,filds );
		}
	}else
	{
		AfxMessageBox(_T("未选择负责人"));
		return paraError;
	}
	return userExcOk;
}
BOOL CProjectSever::UpdatePrjMember( const CProject &prj )
{
	CDlgUserChose dlg( TRUE );
	ListUser selUser;
	GetProjectMember( prj.GetId(), selUser );
	dlg.SetDelSelUser( selUser );

	if ( IDCANCEL == dlg.DoModal() )
	{
		return paraError;
	}
	const CPtrArray* pUser = dlg.GetSelArrary();
	BOOL  ret = m_prjDbDao->DeleteMemberByPrj( prj.GetId() );
	int count = pUser->GetCount();
	if ( userExcOk == ret && count > 0   )
	{
	   const CUser *pTemp = NULL; 
	   for ( int i = 0; i < count; i++ )
	   {
		   pTemp = (const CUser *)pUser->GetAt( i );
		   if ( pTemp )
		   {
			   m_prjDbDao->InsertPrjoctMember( prj.GetId(), pTemp->GetId() );
		   }
	   }
	}
	return userExcOk;
}
BOOL CProjectSever::GetProjectTaskNoFather( int prjId, int taskSt, int userId, ListTask &obj, int &hasChildCount, BOOL isOnlyShow )
{
	hasChildCount = obj.size();
	ListTask fatherList;
	BOOL ret = m_pDaoFactory->GetTaskDbDao()->GetProjectTaskNoFatherHasChild( prjId, taskSt, userId, fatherList, isOnlyShow );
	if ( userExcOk != ret )
	{
		return ret;
	}
    for ( ListTaskItor itor = fatherList.begin(); itor != fatherList.end(); itor++ )
    {
		obj.push_back( *itor );
    }
	hasChildCount = fatherList.size();
	if ( taskSt > 0 || userId > 0 )
	{
		//搜索父级任务不为顶层任务
		ret = m_pDaoFactory->GetTaskDbDao()->GetProjectTaskHasFahter( prjId, taskSt, userId, fatherList, obj, isOnlyShow );
	}
	//else
	//{
		ret = m_pDaoFactory->GetTaskDbDao()->GetProjectTaskNoFatherNoChild( prjId, taskSt, userId, obj, isOnlyShow );
	/*}*/
	return ret;
}
//2015-6-25 moidy 
BOOL CProjectSever::GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj )
{
	return m_pDaoFactory->GetTaskDbDao()->GetProjectTask(  prjId,  taskSt,  ownerId,  obj );
}

BOOL CProjectSever::GetChildTask( int taskId, int st, int ownerId, ListTask &obj )
{
	return  m_pDaoFactory->GetTaskDbDao()->GetChildTask( taskId, st, ownerId, obj );
}
BOOL  CProjectSever::IsHasChildTask( int taskId, BOOL &isHasChild )
{
	return m_pDaoFactory->GetTaskDbDao()->IsHasChildTask( taskId, isHasChild );
}
BOOL CProjectSever::NewProject(const CProject &prj, const ListUser &lstLine)
{
	BOOL ret = m_prjDbDao->InsertProjct( prj );
	if ( ret != userExcOk )
	{
		return ret;
	}
	int prjId;
	m_pDaoFactory->GetTaskDbDao()->GetLastInsertId( prjId );
	for ( ListUser::const_iterator itor = lstLine.begin(); itor != lstLine.end(); itor++ )
	{
		m_prjDbDao->InsertPrjoctMember( prjId, itor->GetId() );
	}
	return ret;
}
BOOL  CProjectSever::DeleteProject( int prjId )
{
	BOOL ret = m_prjDbDao->DeleteMemberByPrj( prjId );
	if ( userExcOk == ret )
	{
		ret = m_prjDbDao->DeleteProjectById(prjId);
	}
	return ret;
}
BOOL CProjectSever::DeleteProduct( int id )
{
	return m_prjDbDao->DeleteProductLineById( id );
}
BOOL  CProjectSever::UpdateProductName( int id, const CString &str )
{
	if ( str.IsEmpty() )
	{
		AfxMessageBox( _T("名称不能为空"));
	   return paraError;
	}
	CProductLine obj;
	BOOL ret = m_prjDbDao->GetProjectLineByName( str, obj );
	if ( ret != notfind )
	{
		if ( userExcOk == ret )
		{
			AfxMessageBox(_T("产品线名称已存在,请重新修改") );
			return paraError;
		}
		return ret;
	}
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("'%s'"), str );
	filds.insert( std::make_pair( fild_name, strValue ));
	return m_prjDbDao->UpdateProductLineField(id ,filds );
}
BOOL CProjectSever::GetProductLineByName( const CString &str,  CProductLine &obj )
{
	return m_prjDbDao->GetProjectLineByName( str, obj );
}
BOOL CProjectSever::UpdateProductDC( int id, const CString &str )
{
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("'%s'"), str );
	filds.insert( std::make_pair( fild_line_bes, strValue ));
	return m_prjDbDao->UpdateProductLineField(id ,filds );
}
BOOL CProjectSever::UpdateProductOwner( const CProductLine &obj )
{
	CDlgUserChose dlg( FALSE );
	dlg.SetDelSelUser( obj.GetMangerId() );
	dlg.DoModal();
	const CPtrArray* pUser = dlg.GetSelArrary();
	if ( 1 == pUser->GetCount()  )
	{
		int selUserId = ( (const CUser*)pUser->GetAt( 0 ))->GetId();
		if ( selUserId != obj.GetMangerId() )
		{
			CString strValue;
			TaskIdStringMap filds;
			strValue.Format( _T("%d"), selUserId );
			filds.insert( std::make_pair( fild_mangerId, strValue ));
			return m_prjDbDao->UpdateProductLineField(obj.GetId() ,filds );
		}
	}else
	{
		AfxMessageBox(_T("未选择负责人"));
		return paraError;
	}
	return userExcOk;
}
BOOL CProjectSever::NewProductLine( const CProductLine &obj )
{
	CString str;
	str = obj.GetName();
	if ( str.IsEmpty() )
	{
		AfxMessageBox( _T("产品线名不能为空") );
		return paraError;
	}
	CProductLine tempObj;
	BOOL ret = GetProductLineByName( str, tempObj );
	if ( ret != notfind )
	{
		if ( userExcOk == ret )
		{
			AfxMessageBox( _T("产品线名已存在") );
			return paraError;
		}
	}
	return m_prjDbDao->InsertProjctLine( obj );
}
BOOL CProjectSever::GetProjectFactStep( const CProject& prj, CString &str )
{//获取项目实际所属阶段
	ListProjectStep step;
	m_systemCfgDbDao->GetProjectStepList( prj.GetPrjStepType(), step );
	BOOL ret;
	int taskCnt = 0, closeCnt = 0, midStopCnt = 0;
	BOOL isTaskCnt = TRUE;
	for ( ListProjectStepConstItor itor = step.begin(); itor != step.end(); itor++ )
	{    
		ret = m_prjDbDao->GetCntPrjTask( prj.GetId(), itor->GetId(), -1, taskCnt );
		if ( ret != userExcOk )
		{
			break;
		}
		if ( taskCnt != 0 )
		{
			isTaskCnt = FALSE;
		}
		ret = m_prjDbDao->GetCntPrjTask( prj.GetId(), itor->GetId(), task_close, closeCnt );
		if ( ret != userExcOk )
		{
			break;
		}
		ret = m_prjDbDao->GetCntPrjTask( prj.GetId(), itor->GetId(), midStop, midStopCnt );
		if ( ret != userExcOk )
		{
			break;
		}
		if ( (closeCnt + midStopCnt) != taskCnt )
		{
			str = itor->GetName();
			break;
		}
	}
	if ( userExcOk == ret && str.IsEmpty() && !isTaskCnt )
	{
		ListProjectStep::reverse_iterator itor = step.rbegin();
		str = itor->GetName();
	}
	return ret;
}
BOOL CProjectSever::GetProjectBelongStep( const CProject& prj, CString &str )
{//获取项目应属阶段
	ListProjectStep step;
	m_systemCfgDbDao->GetProjectStepList( prj.GetPrjStepType(), step );
	BOOL ret;
	COleDateTime time = COleDateTime::GetCurrentTime();
	int taskCnt = 0, closeCnt = 0;
	BOOL isTaskCnt = TRUE;
	for ( ListProjectStepConstItor itor = step.begin(); itor != step.end(); itor++ )
	{   
		ListTask obj;
		m_pDaoFactory->GetTaskDbDao()->GetProjectTaskNoFatherHasChild(prj.GetId(), task_suspend, -1, obj );
		m_pDaoFactory->GetTaskDbDao()->GetProjectTaskNoFatherNoChild(prj.GetId(), task_suspend, -1, obj );

		ret = m_prjDbDao->GetCntPrjTask( prj.GetId(), itor->GetId(), -1, taskCnt );
		if ( obj.size() > 0 && taskCnt > 0 )
		{
			str = itor->GetName() + _T("(挂起)"); //2015-6-23 11111
			return userExcOk;
		}
		if ( ret != userExcOk )
		{
			break;
		}
		if ( taskCnt != 0 )
		{
			isTaskCnt = FALSE;
		}
		ret = m_prjDbDao->GetCntPrjTaskByEndTime(  prj.GetId(), itor->GetId(), time, closeCnt );
		if ( ret != userExcOk )
		{
			break;
		}
		if ( closeCnt != taskCnt )
		{
			str = itor->GetName();
			break;
		}
	}
	if ( userExcOk == ret && str.IsEmpty()  )
	{
		ListProjectStep::reverse_iterator itor= step.rbegin();
		str = itor->GetName();
	}
	if ( isTaskCnt )
	{
		str = _T(" ");
	}
	return ret;
}
BOOL CProjectSever::GetFatherTask( int taskId, CTaskObj &fatherTask )
{
	return m_pDaoFactory->GetTaskDbDao()->GetFatherTask( taskId, fatherTask );
}
