#include "stdafx.h"
#include "BackStageActionImp.h"
#include "Src/MainFrame/Dlg/BackMangerDlg/SetRolePower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBackStageActionImp::CBackStageActionImp( CDaoFactory *pFactory )
	              :CBackStageActionInc( pFactory )
{
	m_pUserDao = pFactory->GetUserDao();
	m_pTaskDbDAo = pFactory->GetTaskDbDao();
}
BOOL  CBackStageActionImp::InsertUser( const CUser &obj )
{
	return m_pUserDao->AddUser( obj );
}
BOOL  CBackStageActionImp::UpdateUserName( int userId,const CString &name )
{
	CUser obj;
     BOOL ret = m_pUserDao->GetUserByName( name, obj );
	if ( userExcOk == ret )
	{
		AfxMessageBox( _T("用户名已存在") );
		return paraError;
	}
	if ( notfind != ret )
	{
		return ret;
	}
	TaskIdStringMap filds;
	CString valueStr;
	valueStr.Format(_T("'%s'"), name );
	filds.insert( std::make_pair( fild_user_name, valueStr ) );
	return m_pUserDao->UpdateUser( userId, filds );
}
BOOL  CBackStageActionImp::UpdateUserLoginName( int userId, const CString &loginName )
{
	CUser obj;
	BOOL ret = m_pUserDao->GetUserByLoginName( loginName, obj );
	if ( userExcOk == ret )
	{
		AfxMessageBox( _T("用户名已存在") );
		return paraError;
	}
	if ( notfind != ret )
	{
		return ret;
	}
	TaskIdStringMap filds;
	CString valueStr;
	valueStr.Format(_T("'%s'"), loginName );
	filds.insert( std::make_pair( fild_user_login_name, valueStr ) );
	return m_pUserDao->UpdateUser( userId, filds );
}
BOOL  CBackStageActionImp::UpdateUserRole(int userId, int roleId )
{
	TaskIdStringMap filds;
	CString valueStr;
	valueStr.Format(_T("%d"), roleId );
	filds.insert( std::make_pair( fild_user_roleid, valueStr ) );
	return m_pUserDao->UpdateUser( userId, filds );
}
BOOL  CBackStageActionImp::UpdateUserDepart(int userId, int departId )
{
	TaskIdStringMap filds;
	CString valueStr;
	valueStr.Format(_T("%d"), departId );
	filds.insert( std::make_pair( fild_user_depid, valueStr ) );
	return m_pUserDao->UpdateUser( userId, filds );
}
BOOL  CBackStageActionImp::UpdateUserStatus( int userId, const CString &status )
{
	TaskIdStringMap filds;
	CString valueStr;
	valueStr.Format(_T("'%s'"), status );
	filds.insert( std::make_pair( fild_user_status, valueStr ) );
	return m_pUserDao->UpdateUser( userId, filds );
}
BOOL  CBackStageActionImp::UpdateUserOnlineStatus( int userId, int onlineId )
{
	return m_pUserDao->UpdateUserOnline( userId, (UserOnLineStatus)onlineId );
}
BOOL  CBackStageActionImp::UpdateExportSeq( int id, int seq )
{
		return m_pUserDao->UpdateExportSeq( id, seq );
}
BOOL  CBackStageActionImp::ClearnUserLoginPwd( int userId )
{
	return m_pUserDao->UpdateUserPwd( userId, _T("d41d8cd98f00b204e9800998ecf8427e") );//空白密码的md5密码
}
BOOL CBackStageActionImp::DeleteUser( int userId )
{
	return m_pUserDao->DeleteUser( userId );
}


/////////////////////////////////////////////////////
//角色
BOOL   CBackStageActionImp::GetAllRole( ListRole &lst )
{
	return m_pUserDao->GetAllRole( lst );
}
BOOL  CBackStageActionImp::GetPowerByRoleId( int roleId, IntStringMap &lst )
{
	return m_pUserDao->GetPowerByRoleId( roleId, lst);
}
BOOL  CBackStageActionImp::GetAllPower( IntStringMap &map )
{
	return m_pUserDao->GetAllPower( map );
}
BOOL  CBackStageActionImp::UpdateRoleName( const CRole &role )
{
	CString roleName =  role.GetRoleName();
	if ( roleName.IsEmpty() )
	{
		AfxMessageBox( _T("角色名称不能为空") );
		return paraError;
	}
	CRole temp;
	BOOL ret = m_pUserDao->GetRoleByName( roleName, temp );
	if ( ret != notfind )
	{
		if ( userExcOk == ret )
		{
			AfxMessageBox(_T("角色名已存在"));
			return paraError;
		}
		return ret;
	}
	return m_pUserDao->UpdateRole( role );
}
BOOL  CBackStageActionImp::UpdateRoleDec( const CRole &role )
{
	return m_pUserDao->UpdateRole( role );
}
BOOL  CBackStageActionImp::UpdateRolePower( const CRole &role )
{
	CDlgSetRolePower dlg( &role );
	if ( IDOK == dlg.DoModal() )
	{
		const IntStringMap &selPowers = dlg.GetSelPowers();
		return m_pUserDao->UpdateRolePower( role.GetId(), selPowers );
	}
	return userExcOk;
}
BOOL  CBackStageActionImp::DeleteRole( int roleId )
{
	return m_pUserDao->DeleteRole( roleId );
}
BOOL CBackStageActionImp::InsertRole( CRole &role, const IntStringMap &powers )
{
	CString str = role.GetRoleName();
	if ( str.IsEmpty() )
	{
		AfxMessageBox( _T("角色名不能为空") );
		return paraError;
	}
	CRole tmp;
	m_pUserDao->GetRoleByName( str, tmp );
	if ( tmp.GetId() > 0 )
	{
		AfxMessageBox( _T("角色名已存在") );
		return paraError;
	}
	BOOL ret = m_pUserDao->AddRole( role );
	if ( userExcOk == ret )
	{
		if (role.GetId() < 0 )
		{
			int id = 0;
			m_pDbDao->GetTaskDbDao()->GetLastInsertId( id );
			role.SetId( id );
		}
	}else
	{
		return ret;
	}
	return m_pUserDao->UpdateRolePower( role.GetId(), powers );
}
/////////////////////////////////////////////////////////////
//
BOOL   CBackStageActionImp::GetAllWorkType( TaskIdStringMap &lst )
{
	return m_pTaskDbDAo->GetAllWorkTimeType( lst ); 
}
BOOL  CBackStageActionImp::GetWorkTypeString( int id, CString &str )
{
	return m_pTaskDbDAo->GetWorkTimeTypeStr( id, str );
}
BOOL  CBackStageActionImp::UpdateWorkTypeStringById( int id, const CString &str )
{
	return m_pTaskDbDAo->UpdateWorkTimeType( id, str );
}
BOOL  CBackStageActionImp::DeleteWorkTypeString( int id )
{
	return m_pTaskDbDAo->DeleteWorkTypeString( id );
}
BOOL   CBackStageActionImp::InsertWorkType( const CString &typeStr )
{
	return m_pTaskDbDAo->AddWorkType( typeStr );
}
