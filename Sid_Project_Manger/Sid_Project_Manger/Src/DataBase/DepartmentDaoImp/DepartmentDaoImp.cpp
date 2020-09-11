#include "stdafx.h"
#include "DepartmentDaoImp.h"

CDepartmentAdoImp::CDepartmentAdoImp( CDaoSupportInterface* pDao ):CDepartmentAdoInc( pDao)
{
	m_strDepartTableName = _T("department");
	m_strGroupTableName = _T("group_sz");
}
BOOL CDepartmentAdoImp::AddDepartement( const CDepartment& obj )
{
	CString str;
	str.Format(_T("select * from %s where dep_name = '%s'"), m_strDepartTableName, obj.GetName() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	//得到执行结果
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	//pRecordSet->MoveFirst();
	if ( !pRecordSet->IsEOF() )
	{
		return insertNameRepeat;
	}
	str.Format( _T("insert into %s (dep_manager, dep_des, dep_name) values(%d,'%s','%s')"), m_strDepartTableName,
				   obj.GetMangerId(), obj.GetDec(), obj.GetName() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}


BOOL CDepartmentAdoImp::UpdateDepartement( const CDepartment& obj)
{//主要用于修改负责人，返回0成功
	CString str;
	str.Format(_T("update %s t set t.dep_manager=%d, t.dep_name='%s', t.dep_des='%s'\
	              t.dep_bak_manger = %d, t.dep_bak_validate = %d  where t.dep_id = %d"),
		      m_strDepartTableName, obj.GetMangerId(), obj.GetName(), obj.GetDec(), obj.GetId(),
			      obj.GetBakManger(), obj.GetBakValidate());
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
CString CDepartmentAdoImp::GetFieldStringByType(int fieldType )
{
	CString  strFild;
	switch( fieldType )
	{
	case fild_dep_manager:
		strFild = _T("dep_manager");
		break;
	case fild_dep_des:
		strFild = _T("dep_des");
		break;
	case fild_line_id:
		strFild = _T("dep_name");
		break;
	case fild_dep_bak_manger:
		strFild = _T("dep_bak_manger");
		break;
	case fild_dep_bak_validate:
		strFild = _T("dep_bak_validate");
		break;
	case  fild_dep_top:
		strFild = _T("dep_top");
		break;
	}
	return strFild;
}
BOOL CDepartmentAdoImp::UpdateDepartField( int id, const TaskIdStringMap &fields )
{
	CString str;
	str.Format( _T("update %s t set "), m_strDepartTableName );
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
	sqlString.Format( _T("%s where t.dep_id = %d"), str, id );
	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CDepartmentAdoImp::GetDepartmentByName( const CString &name, CDepartment &obj )
{
	CString str;
	str.Format(_T("select * from %s where dep_name = '%s'"), m_strDepartTableName, name );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsBOF() )
	{
		return notfind;
	}
	FillDepartFromFild( pRecordSet,  obj);
	return userExcOk;
}
BOOL  CDepartmentAdoImp::GetDepartmentById( int id, CDepartment &obj)
{
	CString str;
	str.Format(_T("select * from %s where dep_id = %d"), m_strDepartTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsBOF() )
	{
		return notfind;
	}
    FillDepartFromFild( pRecordSet,  obj);
	return userExcOk;
}
void    CDepartmentAdoImp::FillDepartFromFild( CAdoRecordset* pRecordSet, CDepartment &obj)
{
	obj.SetId( (*pRecordSet)[_T("dep_id")] );
	obj.SetName( (*pRecordSet)[_T("dep_name")].ToString() );
	obj.SetDec( (*pRecordSet)[_T("dep_des")].ToString() );
	obj.SetMangerId( (int)((*pRecordSet)[_T("dep_manager")]) );
	obj.SetBakManger( (*pRecordSet)[_T("dep_bak_manger")] );
	obj.SetBakValidate( (*pRecordSet)[_T("dep_bak_validate")] );
	obj.SetTopDepartId( (*pRecordSet)[_T("dep_top")] );
}
BOOL CDepartmentAdoImp::ExecSqLReslut( const CString &str, ListDepart &listObj)
{
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		CDepartment obj;
		FillDepartFromFild( pRecordSet, obj );
		listObj.push_back( obj );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}

BOOL CDepartmentAdoImp::GetDepartmentByMangerId( int mangerId, ListDepart &obj, int topId )
{
	CString str;
	if ( -1 == topId )
	{
		str.Format(_T("select * from %s where dep_manager = %d"), m_strDepartTableName, mangerId );
	}else
	{
		str.Format(_T("select * from %s where dep_manager = %d and dep_top = %d"), m_strDepartTableName, mangerId, topId );
	}
	//if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	//{
	//	return excSqlError;
	//}
	return ExecSqLReslut( str, obj );
}

BOOL CDepartmentAdoImp::FindDepartByBakMangerId( int userId, ListDepart &departs )
{
	CString str;
	str.Format(_T("select * from %s where dep_bak_manger = %d and dep_bak_validate = 1"), m_strDepartTableName, userId );
	return ExecSqLReslut( str, departs );
}
BOOL CDepartmentAdoImp::GetChildDepart( int departId, ListDepart &listObj )
{
	CString str;
	str.Format(_T("select * from %s where  dep_top = %d"), m_strDepartTableName,  departId  );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	ListDepart tempList;
	BOOL ret = ExecSqLReslut( str, tempList );
	if ( ret != userExcOk || tempList.size() <= 0 )
	{
		return ret;
	}
	//listObj.merge( tempList ); 
	//listObj.splice( listObj.end(), tempList ); //会删除tempList
	for (ListDepartConstItor itor=tempList.begin(); itor != tempList.end(); itor++)
	{
		listObj.push_back( *itor );
	}
	ListDepart::const_iterator itor = tempList.begin();
	while( ret == userExcOk && itor != tempList.end() )
	{
		ret = GetChildDepart( itor->GetId(), listObj );
		itor++;
	}
	return ret;
}
BOOL CDepartmentAdoImp::GetAllDepart( ListDepart& obj)
{
	CString str;
	str.Format(_T("select * from %s"), m_strDepartTableName );
	return ExecSqLReslut( str, obj );
}
BOOL CDepartmentAdoImp::DeleteDepart( int departId )
{
	CString str;
	str.Format( _T("delete from %s where dep_id = %d"), m_strDepartTableName, departId );
	if ( m_pDaoSupportInc->Execute( str ) )
	{
		return excSqlError;
	}
	return userExcOk;
}
