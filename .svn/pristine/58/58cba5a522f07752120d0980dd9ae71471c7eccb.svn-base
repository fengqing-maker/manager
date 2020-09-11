#ifndef BACK_STAGE_SEVER_IMP_HERAD_34U
#define BACK_STAGE_SEVER_IMP_HERAD_34U

#include "../inc/BackStageActionInc.h"

class CBackStageActionImp: public CBackStageActionInc
{
public:
	CBackStageActionImp(  CDaoFactory *pFactory );

	//用户
	virtual BOOL  InsertUser( const CUser &obj );
	virtual BOOL  UpdateUserName( int userId,const CString &name );
	virtual BOOL  UpdateUserLoginName( int userId, const CString &loginName );
	virtual BOOL  UpdateUserRole(int userId, int roleId );
	virtual BOOL  UpdateUserDepart(int userId, int departId );
	virtual BOOL  UpdateUserStatus( int userId, const CString &status );
	virtual BOOL  UpdateUserOnlineStatus( int userId, int onlineId );
	virtual BOOL  UpdateExportSeq( int id, int seq );
	virtual BOOL  ClearnUserLoginPwd( int userId );
	virtual BOOL  DeleteUser( int userId );
	//角色权限
	virtual BOOL  InsertRole( CRole &role, const IntStringMap &powers );
	virtual BOOL  GetAllRole( ListRole &lst );
	virtual BOOL  GetAllPower( IntStringMap &map );
	virtual BOOL  GetPowerByRoleId( int roleId, IntStringMap &map );
	virtual BOOL  UpdateRoleName( const CRole &role );
	virtual BOOL  UpdateRoleDec( const CRole &role );
	virtual BOOL  UpdateRolePower( const CRole &role );
	virtual BOOL  DeleteRole( int roleId );
	//工时
	virtual BOOL  GetAllWorkType( TaskIdStringMap &lst );
	virtual BOOL  InsertWorkType( const CString &);
	virtual BOOL  GetWorkTypeString( int id, CString &str  );
	virtual BOOL  UpdateWorkTypeStringById( int id, const CString &str  );
	virtual BOOL  DeleteWorkTypeString( int id );


protected:
	CUserDaoInterface *m_pUserDao;
	CTaskDaoInc       *m_pTaskDbDAo;
};

#endif