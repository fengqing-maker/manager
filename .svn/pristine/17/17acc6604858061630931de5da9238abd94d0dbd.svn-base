#include "stdafx.h"
#include "CUserDaoImp.h"

#define USER_PWD  _T("user_pwd")  //用户名字段

CUerDaoImp::CUerDaoImp( CDaoSupportInterface *pDaoSupportInc ):CUserDaoInterface(pDaoSupportInc)
{
	m_strUserTableName = _T("user");
	m_roleTableName = _T("role");
	m_roleMapTableName = _T("role_map");
}
BOOL CUerDaoImp::AddUser( const CUser& user)
{
	CString str;
	str.Format(_T("select * from %s where user_login_name = '%s' or user_name = '%s'"), m_strUserTableName,
		           user.GetLoginName(), user.GetUserName_R() );
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
	str.Format( _T("insert into %s (user_name, user_login_name, user_depid, user_roleid,\
				   user_status, user_pwd,m_online) values('%s','%s',%d,%d,'%s','%s',%d)"), m_strUserTableName,
				   user.GetUserName_R(), user.GetLoginName(), user.GetDepartId(), user.GetRolId(),
				   user.GetUserStatus(), user.GetPwd(),user.GetOnLine() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CUerDaoImp::AddRole( CRole &role )
{
	CString str;
	str.Format( _T("insert into %s (role_name, role_des) values('%s','%s')"), m_roleTableName,
				    role.GetRoleName(), role.GetRolDec() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	GetRoleByName( role.GetRoleName(), role );
	return userExcOk;
}
//验证用户名
BOOL CUerDaoImp::ValidateUser( CUser& user )
{
	CString str;
	str.Format(_T("select * from %s where user_login_name = '%s' "), m_strUserTableName, user.GetLoginName() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return userExcOk;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return validateUserNone;
	}
	CString loginPwd = (*pRecordSet)[USER_PWD].ToString();
	if ( loginPwd != user.GetPwd() )
	{
		return userPwdError;
	}
	FillUserFromFild( pRecordSet, user );
	return userExcOk;
}
BOOL CUerDaoImp::UpdateUserPwd( int id,  const CString & pwd)
{
	BOOL ret = 0;
	if( userExcOk != ValidateUser(id))
	{
		return ret;
	}
	CString str;
	str.Format(_T("update %s t set t.user_pwd = '%s' where t.user_id = %d"),
		m_strUserTableName, pwd, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CUerDaoImp::UpdateExportSeq( int id, int seq )
{
	CString str;
	str.Format(_T("update %s t set t.export_seq = %d where t.user_id = %d"),
		m_strUserTableName, seq, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL  CUerDaoImp::UpdateRole( const CRole &role )
{
	CString str;
	str.Format(_T("update %s t set t.role_name = '%s', t.role_des='%s' \
				     where t.role_id = %d"),
		m_roleTableName, role.GetRoleName(), role.GetRolDec(), role.GetId() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CUerDaoImp::UpdateRolePower( int roleId, const IntStringMap &lst )
{
	CString str;
	str.Format(_T("delete  from %s where role_id = %d"), m_roleMapTableName, roleId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql( str ))
	{
		return excSqlError;
	}
	for ( IntStringMapConstItor itor = lst.begin(); itor != lst.end(); itor++ )
	{
		str.Format(_T("insert into %s (role_id, permiss_id) values(%d, %d)"), m_roleMapTableName,
			               roleId,  itor->first );
		if ( FALSE == m_pDaoSupportInc->ExecuteSql( str ))
		{
			return excSqlError;
		}
	}
	return userExcOk;
}
CString CUerDaoImp::GetUserFieldByType(int fieldType )
{
	CString  strFild;
	switch( fieldType )
	{
	case fild_user_name:
		strFild = _T("user_name");
		break;
	case fild_user_login_name:
		strFild = _T("user_login_name");
		break;
	case fild_user_depid:
		strFild = _T("user_depid");
		break;
	case fild_user_roleid:
		strFild = _T("user_roleid");
		break;
	case  fild_user_status:
		strFild = _T("user_status");
		break;
	}
	return strFild;
}
BOOL CUerDaoImp::UpdateUser( int id, const TaskIdStringMap &fields )
{
	CString str;
	str.Format( _T("update %s t set "), m_strUserTableName );
	TaskIdStringMap::const_iterator itor = fields.begin();
	CString tempStr,fieldStr;
	while( itor != fields.end()  )
	{
		fieldStr = GetUserFieldByType( itor->first);
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
	sqlString.Format( _T("%s where t.user_id = %d"), str, id );
	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	{
		return excSqlError;
	}
	return userExcOk;
}
// 验证用户是否存在
BOOL CUerDaoImp::ValidateUser( int id )
{
	CString str;
	str.Format(_T("select * from %s where user_id = %d"), m_strUserTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsBOF() )
	{
		return validateUserNone;
	}
	return userExcOk;
}
BOOL CUerDaoImp::GetUserById( int id, CUser &obj )
{
	CString str;
	str.Format(_T("select * from %s where user_id = %d"), m_strUserTableName, id );
    BOOL ret = ExcuteSqlResult( str );
	if ( userExcOk == ret )
	{
		FillUserFromFild( m_pDaoSupportInc->GetRecordSet(), obj );
	}
	return userExcOk;
}
 BOOL CUerDaoImp::GetUserByName( const CString &name, CUser &obj )
 {
	 CString str;
	 str.Format(_T("select * from %s where user_name = '%s'"), m_strUserTableName, name );
	 BOOL ret = ExcuteSqlResult( str );
	 if ( userExcOk == ret )
	 {
		 FillUserFromFild( m_pDaoSupportInc->GetRecordSet(), obj );
	 }
	 return ret;
 }
 BOOL CUerDaoImp::GetUserByLoginName( const CString &name, CUser &obj )
 {
	 CString str;
	 str.Format(_T("select * from %s where user_login_name = '%s'"), m_strUserTableName, name );
	 BOOL ret = ExcuteSqlResult( str );
	 if ( userExcOk == ret )
	 {
		 FillUserFromFild( m_pDaoSupportInc->GetRecordSet(), obj );
	 }
	 return ret;
 }
 BOOL CUerDaoImp::GetGernalManger( CUser &obj )
 {
	 CString str;
	 str = _T( "select * from role_map where role_id = (select role_id from role where role_name = '副总经理')" );
	  BOOL ret = ExcuteSqlResult( str );
	  if ( userExcOk == ret )
	  {
		  CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
		  int id = (*pRecordSet)[_T("role_id")];
		  str.Format(_T("select * from %s where user_roleid = %d"), m_strUserTableName, id );
		  ret = ExcuteSqlResult( str );
		  if ( userExcOk == ret )
		  {
			  FillUserFromFild( pRecordSet, obj );
		  }
	  }
	 return ret;
 }
 //获取有某种权限的人
 BOOL CUerDaoImp::GetUserByPower( Power power, ListUser &obj )
 {
	 CString str;
	 str.Format( _T( "select * from %s where permiss_id = %d" ), m_roleMapTableName, power ) ;
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 ListInt tempRole;
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 while( !pRecordSet->IsEOF() )
	 {
		 tempRole.push_back( (*pRecordSet)[_T("role_id")]);
		 pRecordSet->MoveNext();
	 }
	 ListInt::iterator itor = tempRole.begin();
	 while( itor != tempRole.end() )
	 {
		 str.Format( _T("select * from %s where user_roleid = %d and user_status = '在职'"), m_strUserTableName, *itor);
		 ExcuteSqlResult( str, obj );
		 itor++;
	 }
	 return userExcOk;
 }
 //获取所有在职员工
 BOOL CUerDaoImp::GetAllUser( ListUser &obj  )
 {
	 CString str;
	 str.Format( _T("select * from %s where user_status = '在职' "), m_strUserTableName);
	 return ExcuteSqlResult( str, obj );
 }
 BOOL CUerDaoImp::GetAllUserOrdyByExport( ListUser &obj )
 {
	 CString str;
	 str.Format( _T("select * from %s where user_status = '在职' order by export_seq asc"), m_strUserTableName);
	 return ExcuteSqlResult( str, obj );
 }
 BOOL CUerDaoImp::ExcuteSqlResult( const CString &str )
 {
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 //
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 if ( pRecordSet->IsEOF() )
	 {
		 return notfind;
	 }
	 return userExcOk;
 }
 BOOL CUerDaoImp::ExcuteSqlResult( const CString &str, ListUser &obj )
 {
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 while( !pRecordSet->IsEOF() )
	 {
		 CUser tmp;
		 FillUserFromFild( pRecordSet, tmp );
		 obj.push_back( tmp );
		 pRecordSet->MoveNext();
	 }
	 return userExcOk;
 }
void CUerDaoImp::FillUserFromFild( CAdoRecordset *pRecordSet, CUser &user )
{
	user.SetId( (int)((*pRecordSet)[_T("user_id")]) );
	user.SetRolId( (int)((*pRecordSet)[_T("user_roleid")]) );
	user.SetDepartId( (int)((*pRecordSet)[_T("user_depid")]) );
	user.SetUserName_R( (*pRecordSet)[_T("user_name")].ToString() );
	user.SetLoginName( (*pRecordSet)[_T("user_login_name")].ToString() );
	user.SetUserStatus( (*pRecordSet)[_T("user_status")].ToString() );
	user.SetOnLineStatus( (*pRecordSet)[_T("m_online")] );
	user.SetExportSeq( (int)((*pRecordSet)[_T("export_seq")]) );
}
 BOOL CUerDaoImp::UpdateUserOnline( int id, UserOnLineStatus status )
 {
	 BOOL ret = 0;
	 if( userExcOk != ValidateUser(id))
	 {
		 return ret;
	 }
	 CString str;
	 str.Format(_T("update %s t set t.m_online= %d where t.user_id = %d"),
		 m_strUserTableName, status, id );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 return userExcOk;
 }
 //更加用户id获取权利
 BOOL CUerDaoImp::GetUserPowersByRoleId( int roleId, RolePowerList &rtlist)
 {
	 CString str;
	 str.Format(_T("select * from %s where role_id = %d"), m_roleMapTableName, roleId );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();

	 if ( pRecordSet->IsEOF()  )
	 {
		 return notfind;
	 }
	 //
	 while( !pRecordSet->IsEOF() )
	 {
		 rtlist.push_back( (int) (*pRecordSet)[_T("permiss_id")] );
		 pRecordSet->MoveNext();
	 }
	 return userExcOk;
 }
BOOL CUerDaoImp::GetRoleById( int roleId, CRole &role )
{
	CString str;
	str.Format(_T("select * from %s where role_id = %d"), m_roleTableName, roleId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	
	CAdoRecordset *pRecordSet = m_pDaoSupportInc->GetRecordSet();
	//
	int cnt = pRecordSet->GetRecordCount();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillRoleFromFild( pRecordSet, role );
	return userExcOk;
}
BOOL CUerDaoImp::GetRoleByName( const CString &roleName, CRole &role )
{
	CString str;
	str.Format(_T("select * from %s where role_name = '%s'"), m_roleTableName, roleName );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecordSet = m_pDaoSupportInc->GetRecordSet();
	//
	int cnt = pRecordSet->GetRecordCount();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillRoleFromFild( pRecordSet, role );
	return userExcOk;
}
void CUerDaoImp::FillRoleFromFild(CAdoRecordset *pRecordSet, CRole &role )
{
	role.SetId( (*pRecordSet)[_T("role_id")] );
	role.SetRoleDec( (*pRecordSet)[_T("role_des")].ToString() );
	role.SetRoleName( (*pRecordSet)[_T("role_name")].ToString() );
}
BOOL CUerDaoImp::GetAllRole( ListRole &lst )
{
	CString str;
	str.Format(_T("select * from %s"), m_roleTableName );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecordSet = m_pDaoSupportInc->GetRecordSet();
    while( !pRecordSet->IsEOF() )
	{
		CRole role;
		FillRoleFromFild( pRecordSet, role );
		pRecordSet->MoveNext();
		lst.push_back( role );
	}
	
	return userExcOk;
}
BOOL CUerDaoImp::GetRoleByUserId( int userId, CRole &role )
{
	CString str;
	str.Format(_T("select user_roleid from %s where user_id = %d"), m_strUserTableName, userId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	pRecordSet->MoveFirst();
	int roleId = (*pRecordSet)[_T("user_roleid")];
    return GetRoleById(  roleId, role );
}
BOOL CUerDaoImp::GetPowerByRoleId( int roleId, IntStringMap &lst )
{
	CString str;
	str.Format(_T("select * from permissions where Id in (select permiss_id from %s where role_id = %d)"), 
		        m_roleMapTableName, roleId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		lst.insert( std::make_pair( (*pRecordSet)[_T("Id")], (*pRecordSet)[_T("permiss_dc")].ToString() ) );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL  CUerDaoImp::GetAllPower( IntStringMap &lst )
{
	CString str = _T("select * from permissions");
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		lst.insert( std::make_pair( (*pRecordSet)[_T("Id")], (*pRecordSet)[_T("permiss_dc")].ToString() ) );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL CUerDaoImp::GetDepartMember( int departId, ListUser &listObj )
{
	CString str;
	str.Format( _T("select * from %s where user_depid = %d and user_status = '在职'"), m_strUserTableName, departId );
	return	ExcuteSqlResult( str, listObj );
}

 BOOL CUerDaoImp::GetPrjectMember( int prjId, ListUser& listObj )
 {
	 CString str;
	 str.Format( _T("select * from project_member where prj_id = %d"), prjId );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 ListInt tList;
	 while( !pRecordSet->IsEOF() )
	 {
		 tList.push_back( (*pRecordSet)[_T("prj_memberId")] );
		 pRecordSet->MoveNext();
	 }
	 ListIntItor itor = tList.begin();
	 while( itor != tList.end())
	 {
		 CUser obj;
		if ( userExcOk == GetUserById( *itor, obj) )
		{
			listObj.push_back( obj );
		}
		itor++;
	 }
	 return userExcOk;
 }
 BOOL CUerDaoImp::DeleteUser( int userId )
 {
	 CString str;
	 str.Format( _T("delete  from %s where user_id = %d"), m_strUserTableName, userId );
	 if ( m_pDaoSupportInc->Execute( str ) )
	 {
		 return userExcOk;
	 }
	 return excSqlError;
 }
 BOOL CUerDaoImp::DeleteRole( int roleId )
 {
	 CString str;
	 str.Format( _T("delete  from %s where role_id = %d"), m_roleMapTableName, roleId );
	 if ( !m_pDaoSupportInc->Execute( str ) )
	 {
		 return excSqlError;
	 }
	str.Format(_T("delete from %s where role_id = %d"), m_roleTableName, roleId  );
	if ( !m_pDaoSupportInc->Execute( str ) )
	{
		 return excSqlError;
	}
	return userExcOk;
 }