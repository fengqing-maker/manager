#include "stdafx.h"
#include "User.h"

#define CONST_STRING_ONLINE  _T("在线")
#define CONST_STRING_LEFTLINE _T("离线")
#define CONST_STRING_WORK    _T("在职")
#define CONST_STRING_NOTWORK _T("离职")

CUser::CUser()
{
   m_strStatus = CONST_STRING_WORK;
   m_onLineStatus = onLine;
   m_workStatus = onTheJob;
   m_id = -1;
   m_departId = -1;
   m_roleId = -1;
   m_exportSeq = 0;
}
CUser::~CUser()
{
}
void CUser::SetId( int id )
{
	m_id = id;
	return;
}
int CUser::GetId() const
{
	return m_id;
}
void CUser::SetUserName_R(const CString &userName )
{
    m_strName = userName;
}
const CString& CUser::GetUserName_R() const
{
	return m_strName;
}

void CUser::SetDepartId( int id )
{
	m_departId = id;
}
int  CUser::GetDepartId() const
{
	return m_departId;
}

void CUser::SetLoginName( const CString &str )
{
	m_strLogName = str;
}
const CString& CUser::GetLoginName() const
{
	return m_strLogName;
}

void CUser::SetRolId( int id )
{
	m_roleId = id;
}
int  CUser::GetRolId() const
{
	return m_roleId;
}

void CUser::SetUserStatus( const CString &str )
{
	m_strStatus = str;
	if ( CONST_STRING_WORK == str )
	{
		m_workStatus = onTheJob;
	}else if ( CONST_STRING_NOTWORK == str )
	{
		m_workStatus = departure;
	}
	
}
const CString& CUser::GetUserStatus() const
{
	return m_strStatus;
}

void CUser::SetPwd( const CString &str )
{
	 m_strPwd = str;
}
const CString& CUser::GetPwd() const
{
	return m_strPwd;
}


//void CUser::SetOnLineStatus( const CString &str )
//{
//	m_strOnline = str;
//	if( CONST_STRING_ONLINE== m_strOnline )
//	{
//		m_onLineStatus = onLine;
//	}else if ( CONST_STRING_LEFTLINE == m_strStatus )
//	{
//		m_onLineStatus = leftLine;
//	}
//}
void CUser::SetOnLineStatus( int st )
{
	m_onLineStatus = st;
	switch(st)
	{
	case onLine:
	    m_strOnline = CONST_STRING_ONLINE;
		break;
	case  leftLine:
		m_strOnline = CONST_STRING_LEFTLINE;
		break;
	}
}
//const CString& CUser::GetOnLineStatus() const
//{
//	return m_strOnline;
//}
void CUser::GetOnLineString( int status, CString &retString )
{
	switch(status)
	{
	case onLine:
		retString = CONST_STRING_ONLINE;
		break;
	case leftLine:
		retString = CONST_STRING_LEFTLINE;
		break;
	}
}
void CUser::GetOnWorkString( UserWorkStatus status, CString &retString )
{
	switch(status)
	{
	case onTheJob:
		retString = CONST_STRING_WORK;
		break;
	case departure:
		retString = CONST_STRING_NOTWORK;
		break;
	}
}
void CUser::GetOnLineStrings( IntStringMap &map )
{
	map.insert( std::make_pair(onLine, CONST_STRING_ONLINE) );
	map.insert( std::make_pair(leftLine, CONST_STRING_LEFTLINE) );
}
void CUser::GetOnWorkStrings( IntStringMap &map )
{
	map.insert( std::make_pair(onTheJob, CONST_STRING_WORK) );
	map.insert( std::make_pair(departure, CONST_STRING_NOTWORK) );
}

void   CUser::CopyObj( const CUser &obj )
{
	m_id = obj.m_id; //用户id
	m_strName = obj.m_strName; //用户名
	m_strLogName = obj.m_strLogName; //用户登陆名
	m_departId = obj.m_departId; //部门ID
	m_roleId = obj.m_roleId;  //角色ID
	m_strStatus = obj.m_strStatus; //状态，在职，还是离职
	m_strPwd = obj.m_strPwd; //登陆密码
	m_strOnline = obj.m_strStatus; //下线，在线
	m_onLineStatus = obj.m_onLineStatus;
	m_workStatus = obj.m_workStatus; //
	m_exportSeq = obj.m_exportSeq;
}
CUser & CUser::operator = (const CUser & obj)
{
	CopyObj( obj );
	return *this;
}
CUser::CUser( const CUser &obj )
{
	CopyObj( obj );
}