#include "stdafx.h"
#include "LoginActionImp.h"

#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoginActionImp::CLoginActionImp(  CDaoFactory *pObj ):CLoginActionInc( pObj )
{
	ASSERT( pObj != NULL );
	m_pUserInc = pObj->GetUserDao();
	m_pDepartmentInc = pObj->GetDepartDbDao();
}

BOOL   CLoginActionImp::LoginSystem(const CString &loginName,const CString &loginPwd )
{
    CUser user;
	user.SetLoginName( loginName );
	user.SetPwd( loginPwd );
	BOOL ret = 0;
    if( userExcOk == (ret = m_pUserInc->ValidateUser( user )) )
	{
		if ( user.GetOnLine() == onLine )
		{
			int selId = AfxMessageBox( _T("此用户已登录，是否强制其他用户下线，再进行登录"), MB_YESNO );
			if( selId == IDYES )
			{
				m_pUserInc->UpdateUserOnline( user.GetId(), leftLine );
				return paraError;
			}
			return paraError;
		}
		if ( userExcOk == (ret = m_pUserInc->UpdateUserOnline( user.GetId(), onLine) ) )
		{
			m_loginUser = user;
			m_loginUser.SetOnLineStatus( onLine );
		}
	}
	return ret;
}
//登陆成功，进行加载信息
BOOL CLoginActionImp::Loading()
{
	BOOL ret = InitDepartInfo();
	if ( userExcOk ==  ret )
	{//加载角色
		CRole role;
		if ( userExcOk == m_pUserInc->GetRoleByName( _T("代理人"),  role ) )
		{
			ListDepart departs;
			m_pDepartmentInc->FindDepartByBakMangerId( m_loginUser.GetId(), departs  );
			if ( departs.size() > 0)
			{
				m_role = role;
			}
		}
		if ( m_role.GetId() <= 0 )
		{
			ret = m_pUserInc->GetRoleByUserId( m_loginUser.GetId(), m_role );
		}
	    if ( m_role.GetId() > 0 )
	    {
			ret = m_pUserInc->GetUserPowersByRoleId( m_role.GetId(), m_powerList );
	    }
		m_pUserInc->GetGernalManger( m_generManger );
		GetUserByPower( taskAgree, m_publishUser );
	}
    return ret;
}
BOOL     CLoginActionImp::GetRoleById( int roleId, CRole &role )
{
	return m_pUserInc->GetRoleById( roleId, role );
}
const CUser& CLoginActionImp::GetLoginUser()
{
	return m_loginUser;
}
BOOL        CLoginActionImp::InitDepartInfo()
{
	return m_pDepartmentInc->GetDepartmentById( m_loginUser.GetDepartId(), m_loginUserDepart );
}
CString     CLoginActionImp::GetOrganizationString()
{
	if ( m_loginUserDepart.GetId() < 0 )
	{
		InitDepartInfo();
	}
	return GetOrganizationString( m_loginUserDepart );
}
CString  CLoginActionImp::GetOrganizationString( const CDepartment &departSrc )
{
	CString ret;
	if ( 0 != departSrc.GetTopDepartId() )
	{
		ListDepart list;
		CDepartment depart = departSrc;
		while( 0 != depart.GetTopDepartId() )
		{
			if ( userExcOk != m_pDepartmentInc->GetDepartmentById( depart.GetTopDepartId(), depart ))
			{
				break;
			}
			list.push_back( depart );
		}

		ListDepart::reverse_iterator itor = list.rbegin();
		
		while( itor != list.rend() )
		{
			ret += itor->GetName();
			if ( itor->GetId() != departSrc.GetId() )
			{
				ret += _T("->");
			}
			itor++;
		}
		ret += departSrc.GetName();
	}else
	{
		ret = departSrc.GetName();
	}
	return ret;
}
BOOL   CLoginActionImp::GetUserById( int userId, CUser &user )
{
	return m_pUserInc->GetUserById( userId, user );
}
BOOL CLoginActionImp::GetUserByName( const CString &strName, CUser &obj )
{
	return m_pUserInc->GetUserByName( strName, obj );
}
BOOL  CLoginActionImp::GetUserByLoginName( const CString &strName, CUser &obj )
{
	return m_pUserInc->GetUserByLoginName( strName, obj );
}
BOOL   CLoginActionImp::GetUserLeader(const CUser &obj, CUser &leader )
{
	//获取部门
	int  departId = obj.GetDepartId();
	BOOL ret = -1;
	if ( departId >= 0 )
	{
		CDepartment depart;
		ret = m_pDepartmentInc->GetDepartmentById( departId , depart);
		while( userExcOk == ret && 0 != depart.GetTopDepartId() )
		{
			int mangerId = depart.GetMangerId();
			if (  obj.GetId() != mangerId )
			{
				ret = m_pUserInc->GetUserById( mangerId, leader );
				if ( userExcOk == ret )
				{
					return ret;
				}
			}else 
			{
				departId = depart.GetTopDepartId();
				ret = m_pDepartmentInc->GetDepartmentById( departId , depart);
			}
		}
		if ( userExcOk == ret && 0 == depart.GetTopDepartId() )
		{
			ret = m_pUserInc->GetUserById( depart.GetId(), leader );
			if ( userExcOk == ret )
			{
				return ret;
			}
		}
	}
	if ( m_generManger.GetId() > 0 )
	{
		leader = m_generManger;
		ret = userExcOk;
	}
	return ret;
}

BOOL CLoginActionImp::GetUserPower(int userId, RolePowerList &listObjs )
{
	CRole role;
	BOOL ret = m_pUserInc->GetRoleByUserId( userId, role );
	if ( userExcOk == ret ) 
	{
		ret = m_pUserInc->GetUserPowersByRoleId( role.GetId(), listObjs );
	}
	return ret;
}
const RolePowerList& CLoginActionImp::GetLoginUserPower( ) 
{
	return m_powerList;
}
 BOOL    CLoginActionImp::FindLoginPower( Power pt ) const
 {
	return FindPower( pt, m_powerList);
 }
BOOL CLoginActionImp::FindPower( Power pt, const RolePowerList &list) const
{
	RolePowerList::const_iterator itor = list.begin();
	 while( itor != list.end() )
	 {
		 if ( *itor == pt )
		 {
			 return TRUE;
		 }
		 itor++;
	 }
    return FALSE;
}
BOOL        CLoginActionImp::FindUserPower( int userId, Power pt ) 
{
	RolePowerList list;
	BOOL ret = GetUserPower( userId, list );
	if ( userExcOk == ret )
	{
		return FindPower( pt, list);
	}
	return FALSE;
}
 void                CLoginActionImp::QuitLogin()
 {
	 int userId = m_loginUser.GetId();
	 if ( userId >= 0 )
	 {
		 m_pUserInc->UpdateUserOnline( userId, leftLine );
		 m_loginUser.SetId( -1 );
		 m_loginUser.SetOnLineStatus(leftLine);
	 }
 }
 BOOL       CLoginActionImp::SetLoginUserPwd( const CString &newPwd )
 {
	CString md5String =  CUtilityTool::GetObj()->GetMd5String( newPwd );
	return m_pUserInc->UpdateUserPwd( m_loginUser.GetId(), md5String);
 }
BOOL   CLoginActionImp::GetUserByPower( Power pt, ListUser &list )
{
	 return m_pUserInc->GetUserByPower( pt, list );
}
BOOL      CLoginActionImp::GetAllUser( ListUser &list )
{
	return m_pUserInc->GetAllUser(  list );
}
BOOL  CLoginActionImp::GetAllUserOrdyByExport( ListUser &obj )
{
	return m_pUserInc->GetAllUserOrdyByExport(  obj );
}
const CUser &       CLoginActionImp::GetLoginUserLeader()
{
	if ( m_loginUserLeader.GetId() < 0 )
	{
		GetUserLeader(  m_loginUser, m_loginUserLeader);
	}
	return m_loginUserLeader;
}
 const ListUser& CLoginActionImp::GetPubishPowweUser()
 {
	 if ( m_publishUser.size() <= 0 )
	 {
		GetUserByPower( taskAgree, m_publishUser );
	 }
	 return m_publishUser;
 }
 //获取登录者的组织成员
 BOOL      CLoginActionImp::GetLoginOrginMember( ListUser &obj )
 {
	 BOOL ret = m_pUserInc->GetDepartMember( m_loginUserDepart.GetId(), obj );
	 if ( ret != userExcOk || m_loginUser.GetId() != m_loginUserDepart.GetMangerId() )
	 {
		 return ret;
	 }
	 ListDepart list; 

	 ret = m_pDepartmentInc->GetChildDepart( m_loginUserDepart.GetId(), list );
	 ListDepartConstItor itor = list.begin();
	 while( (ret == userExcOk || ret == notfind ) && itor != list.end() )
	 {
		 ret = m_pUserInc->GetDepartMember( itor->GetId(), obj );
		 itor++;
	 }
	 if ( ret == notfind )
	 {
		 ret = userExcOk;
	 }
	 return ret;
 }
