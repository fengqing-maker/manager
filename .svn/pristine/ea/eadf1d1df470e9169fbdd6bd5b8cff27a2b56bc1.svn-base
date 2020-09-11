#include "stdafx.h"
#include "projectDbDaoImp.h"

CProjectDbDaoImp::CProjectDbDaoImp(CDaoSupportInterface* pDao)
	:CProjectDbDaoInc(pDao),m_taskTableName(_T("task"))
{
   m_prjTableName = _T("project");
   m_prjLineTableName = _T( "product_line" );
}
BOOL CProjectDbDaoImp::InsertProjct( const CProject &obj )
{
	CString str;
	str.Format(_T("select * from %s where project_shortName = '%s' or project_name = '%s'"), 
		           m_prjTableName, obj.GetShortName(), obj.GetLongName() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	//得到执行结果
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	//pRecordSet->MoveFirst();
	int count = 0;
	count = pRecordSet->GetRecordCount();
	BOOL eof = pRecordSet->IsEOF();
	if ( !eof )
	{
		return insertNameRepeat;
	}
	str.Format( _T("insert into %s (project_name, project_managerid, line_id, project_shortName,product_type)\
		            values('%s',%d,%d,'%s','%s')"), m_prjTableName,
		   obj.GetLongName(),obj.GetMangerId(), obj.GetLineId(), obj.GetShortName(), obj.GetPrjStepType() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::InsertProjctLine( const CProductLine &obj )
{
	CString str;
	str.Format(_T("select * from %s where name = '%s' "),
		m_prjLineTableName, obj.GetName() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	//得到执行结果
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( !pRecordSet->IsEOF() )
	{
		return insertNameRepeat;
	}
	str.Format( _T("insert into %s (name, mangerId, line_bes)\
				   values('%s',%d,'%s')"), m_prjLineTableName, obj.GetName(), obj.GetMangerId(), obj.GetDec() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
void CProjectDbDaoImp::FillProjectFromFild( CAdoRecordset* pRecordSet, CProject &obj)
{
	ASSERT( pRecordSet != NULL );
	obj.SetId( (*pRecordSet)[_T("project_id")] );
	obj.SetLongName ( (*pRecordSet)[_T("project_name")].ToString() );
	obj.SetMangerId ( (*pRecordSet)[_T("project_managerid")]);
	obj.SetLineId  ( (*pRecordSet)[_T("line_id")] );
	obj.SetShortName( (*pRecordSet)[_T("project_shortName")].ToString() );
	obj.SetPrjStepType( (*pRecordSet)[_T("product_type")].ToString() );
};
//产品
BOOL CProjectDbDaoImp::GetProjectByMangerId( int mangerId, ListProject &listObj)
{
	CString str;
	str.Format(_T("select * from %s where project_managerid = %d order by priority asc"),
		m_prjTableName, mangerId );
	return ExecSqLReslut( str, listObj);
}
BOOL CProjectDbDaoImp::GetTaskPartProject( int mangerId, ListProject&listObj )
{
	CString str;
	str.Format(_T("select * from %s where project_managerid = %d \
				  or project_id in (select prj_id from project_member where prj_memberId = %d) order by priority asc"),
		m_prjTableName, mangerId, mangerId );
	return ExecSqLReslut( str, listObj);
}
BOOL CProjectDbDaoImp::GetProjectByLine( int lineId, ListProject &listObj)
{
	CString str;
	str.Format(_T("select * from %s where line_id = %d order by priority asc"),
		m_prjTableName, lineId );
	return ExecSqLReslut( str, listObj);
}
BOOL CProjectDbDaoImp::GetProjectByPrjLineNotManger( int lineId, int userId, ListProject &listObj )
{
	CString str;
	str.Format(_T("select * from %s where line_id = %d and project_managerid != %d  order by priority asc"),
		m_prjTableName,lineId, userId );
	return ExecSqLReslut( str, listObj);
}
BOOL CProjectDbDaoImp::GetProjectLineByMangerId( int mangerId, ListProjectLine& listObj )
{
	CString str;
	str.Format(_T("select * from %s where mangerId = %d "),
		m_prjLineTableName, mangerId );
	return ExecSqLReslut( str, listObj );
}
BOOL CProjectDbDaoImp::GetProjectLineById( int id, CProductLine& obj )
{
	CString str;
	str.Format(_T("select * from %s where Id = %d "),
		m_prjLineTableName,id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql( str ))
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillProductLineFromFild( pRecordSet, obj );
	return  userExcOk;
}
BOOL CProjectDbDaoImp::GetProjectLineByName( const CString &name, CProductLine& obj  )
{
	CString str;
	str.Format(_T("select * from %s where name = '%s' "),
		m_prjLineTableName, name );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql( str ))
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillProductLineFromFild( pRecordSet, obj );
	return  userExcOk;
}
BOOL CProjectDbDaoImp::ExecSqLReslut( const CString &str, ListProject &listObj)
{
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		CProject obj;
		FillProjectFromFild( pRecordSet, obj );
		listObj.push_back( obj );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::ExecSqLReslut( const CString &str, ListProjectLine &listObj)
{
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		CProductLine obj;
		FillProductLineFromFild( pRecordSet, obj );
		listObj.push_back( obj );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
//
void    CProjectDbDaoImp::FillProductLineFromFild( CAdoRecordset* pRecordSet, CProductLine &obj )
{
	ASSERT( pRecordSet != NULL );
	obj.SetId( (*pRecordSet)[_T("Id")] );
	obj.SetName( (*pRecordSet)[_T("name")].ToString() );
	obj.SetMangerId( (*pRecordSet)[_T("mangerId")]);
	obj.SetDec( (*pRecordSet)[_T("line_bes")] );
}
BOOL CProjectDbDaoImp::GetAllProject( ListProject &prj )
{
	CString str;
	str.Format(_T("select * from %s order by priority asc"),m_prjTableName );
	return  ExecSqLReslut( str, prj);
}


BOOL CProjectDbDaoImp::GetPrjectById( int id, CProject &obj )
{
	CString str;
	str.Format(_T("select * from %s where project_id = %d"),m_prjTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillProjectFromFild( pRecordSet, obj );
	return userExcOk;
}
BOOL CProjectDbDaoImp::GetProjectByName( const CString &strName, CProject &obj )
{
	CString str;
	str.Format(_T("select * from %s where project_name = '%s'"),m_prjTableName, strName );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillProjectFromFild( pRecordSet, obj );
	return userExcOk;
}
BOOL CProjectDbDaoImp::GetProjectByMember( int memberId, ListProject &prjList )
{
	CString str;
	str.Format(_T("select * from project_member where prj_memberId = %d order by priority asc"), memberId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	ListInt tmpPrj;
	while( !pRecordSet->IsEOF() )
	{
		int prjId = (*pRecordSet)[_T("prj_id")];
		tmpPrj.push_back( prjId );
		pRecordSet->MoveNext();
	}
	ListIntItor itor = tmpPrj.begin();

	while( itor != tmpPrj.end() )
	{
		CProject obj;
		if (userExcOk == GetPrjectById( *itor, obj ) )
		{
			prjList.push_back( obj );
		}
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::DeleteMemberByPrj( int prjId, int userId /*= -1*/)
{
	CString str;
	if ( userId < 0 )
	{
		str.Format(_T("delete  from project_member where prj_id = %d "), prjId );
	}else
	{
		str.Format(_T("delete  from project_member where prj_id = %d and prj_memberId = %d "), prjId, userId );
	}
	if ( FALSE == m_pDaoSupportInc->Execute( str ) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::DeleteProjectById( int prjId )
{
     CString str;
	 str.Format(_T("delete  from %s where project_id = %d "), m_prjTableName, prjId );
	 if ( FALSE == m_pDaoSupportInc->Execute( str ) )
	 {
		 return excSqlError;
	 }
	 return userExcOk;
}
BOOL CProjectDbDaoImp::DeleteProductLineById( int id )
{
	CString str;
	str.Format(_T("delete  from %s where Id = %d "), m_prjLineTableName, id );
	if ( FALSE == m_pDaoSupportInc->Execute( str ) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::InsertPrjoctMember( int prjId, int userId )
{
	CString str;
	str.Format(_T("select * from project_member where prj_id = %d and prj_memberId = %d "), prjId, userId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql( str ) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( !pRecordSet->IsEOF() )
	{
		return userExcOk;
	}
	str.Format( _T("insert into project_member (prj_id,prj_memberId) values(%d,%d)"),  prjId, userId );
	if ( FALSE == m_pDaoSupportInc->Execute( str ) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::GetAllProjectLine( ListProjectLine &obj )
{
	CString str;
	str.Format(_T("select * from %s"), m_prjLineTableName );
	return  ExecSqLReslut( str, obj );
}
BOOL CProjectDbDaoImp::UpdateProjectField( int prjId, const TaskIdStringMap &fields )
{
	BOOL ret  = ValidateProjectExit( prjId )  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	CString str;
	str.Format( _T("update %s t set "), m_prjTableName );
	TaskIdStringMap::const_iterator itor = fields.begin();
	CString tempStr,fieldStr;
	while( itor != fields.end()  )
	{
		fieldStr = GetFieldStringByType( itor->first);
		if ( fieldStr.GetLength() <= 0 )
		{
			return paraError;
		}
		tempStr.Format(_T("t.%s = %s,"), fieldStr, itor->second );
		str += tempStr;
		itor++;
	}
	str.Delete( str.GetLength() - 1 );
	CString sqlString;
	sqlString.Format( _T("%s where t.project_id = %d"), str, prjId );
	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::ValidateProjectExit( int id )
{
	CString str;
	str.Format( _T("select * from %s where project_id = %d") , m_prjTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	return userExcOk;
}
BOOL    CProjectDbDaoImp::ValidateProductLintExit( int id )
{
	CString str;
	str.Format( _T("select * from %s where Id = %d") , m_prjLineTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	return userExcOk;
}
CString CProjectDbDaoImp::GetFieldStringByType(int fieldType )
{
	CString  strFild;
	switch( fieldType )
	{
	case fild_project_name:
		strFild = _T("project_name");
		break;
	case fild_project_managerid:
		strFild = _T("project_managerid");
		break;
	case fild_line_id:
		strFild = _T("line_id");
		break;
	case fild_project_shortName:
		strFild = _T("project_shortName");
		break;
	case  fild_prj_product_type:
		strFild = _T("product_type");
		break;
	}
	return strFild;
}
CString CProjectDbDaoImp::GetProductLineStringFieldByType(int fieldType )
{
	CString  strFild;
	switch( fieldType )
	{
	case fild_name:
		strFild = _T("name");
		break;
	case fild_mangerId:
		strFild = _T("mangerId");
		break;
	case  fild_line_bes:
		strFild = _T("line_bes");
		break;
	}
	return strFild;
}
BOOL CProjectDbDaoImp::UpdateProductLineField( int id, const TaskIdStringMap &fields )
{
	BOOL ret  = ValidateProductLintExit( id )  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	
	CString str;
	str.Format( _T("update %s t set "), m_prjLineTableName );
	TaskIdStringMap::const_iterator itor = fields.begin();
	CString tempStr,fieldStr;
	while( itor != fields.end()  )
	{
		fieldStr = GetProductLineStringFieldByType( itor->first);
		if ( fieldStr.GetLength() <= 0 )
		{
			return paraError;
		}
		tempStr.Format(_T("t.%s = %s,"), fieldStr, itor->second );
		str += tempStr;
		itor++;
	}
	str.Delete( str.GetLength() - 1 );
	CString sqlString;
	sqlString.Format( _T("%s where t.Id = %d"), str, id );
	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL  CProjectDbDaoImp::GetCntPrjTask( int prjId, int stepId, int taskSt, int &cnt )
{
	CString str;
	if ( taskSt <= 0 )
	{
		str.Format( _T("select count(*) as taskNum from %s where task_project_id = %d \
					   and task_attri = %d"),
					   m_taskTableName, prjId, stepId );
	}else
	{
		str.Format( _T("select count(*) as taskNum from %s where task_project_id = %d \
					   and task_attri = %d and task_statusid = %d "),
					   m_taskTableName, prjId, stepId, taskSt );
	}
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	cnt = 0;
	if ( !pRecordSet->IsEOF() )
	{
		DbField value = (*pRecordSet)[_T("taskNum")];
		if ( !value.IsNull() )
		{
			cnt = value;
		}
	}
	return userExcOk;
}
BOOL CProjectDbDaoImp::GetCntPrjTaskByEndTime( int prjId, int stepId, const COleDateTime &cuDate, int &cnt )
{
	CString str;
	str.Format( _T("select count(*) as taskNum from %s where task_project_id = %d \
					   and task_attri = %d and task_plan_closetime <= '%s' "),
					   m_taskTableName, prjId, stepId, cuDate.Format(_T("%Y-%m-%d")));
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( !pRecordSet->IsEOF() )
	{
		DbField value = (*pRecordSet)[_T("taskNum")];
		if ( !value.IsNull() )
		{
			cnt = value;
		}
	}
	return userExcOk;
}