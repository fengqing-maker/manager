#pragma once
#include "../../DataBase/factory/CDaoFactory.h"

class CBackStageActionInc
{
public:
	CBackStageActionInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CBackStageActionInc(){};
	//”√ªß
	virtual BOOL  InsertUser( const CUser &obj ) = 0;
	virtual BOOL  UpdateUserName( int userId,const CString &name ) = 0;
	virtual BOOL  UpdateUserLoginName( int userId, const CString &loginName ) = 0;
	virtual BOOL  UpdateUserRole(int userId, int roleId ) = 0;
	virtual BOOL  UpdateUserDepart(int userId, int departId ) = 0 ;
	virtual BOOL  UpdateUserStatus( int userId, const CString &status) = 0 ;
	virtual BOOL  UpdateUserOnlineStatus( int userId, int onlineId ) = 0;
	virtual BOOL UpdateExportSeq( int id, int seq ) = 0;
	virtual BOOL  ClearnUserLoginPwd( int userId ) = 0;
    virtual BOOL  DeleteUser( int userId ) = 0;
	
	//
	virtual BOOL  InsertRole( CRole &role, const IntStringMap &powers ) = 0;
	virtual BOOL  GetAllRole( ListRole &lst ) = 0;
	virtual BOOL  GetPowerByRoleId( int roleId, IntStringMap &map ) = 0; 
	virtual BOOL  GetAllPower( IntStringMap &map ) = 0;
	virtual BOOL  UpdateRoleName( const CRole &role ) = 0;
	virtual BOOL  UpdateRoleDec( const CRole &role ) = 0;
	virtual BOOL  UpdateRolePower( const CRole &role ) = 0;
	virtual BOOL  DeleteRole( int roleId ) = 0;
	//
	virtual BOOL  GetAllWorkType( TaskIdStringMap &lst ) = 0;
	virtual BOOL  InsertWorkType( const CString &) = 0;
	virtual BOOL  GetWorkTypeString( int id, CString &str ) = 0;
	virtual BOOL  UpdateWorkTypeStringById( int id, const CString &str ) = 0;
	virtual BOOL  DeleteWorkTypeString( int id ) = 0;
protected:
	CDaoFactory       *m_pDbDao;
};