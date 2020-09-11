#ifndef CUSER_DAO_INTERFACE_H_24234
#define CUSER_DAO_INTERFACE_H_24234

#include "DaoSupportInterface.h"
#include "../../Model/User.h"
#include "../../Model/power.h"
#include "../../Model/Role.h"


typedef std::list<CRole>               ListRole;
typedef ListRole::iterator        ListRoleItor;
typedef ListRole::const_iterator  ListRoleConstItor;

class CUserDaoInterface
{
public:
	CUserDaoInterface( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
	virtual ~CUserDaoInterface( ){};

	virtual BOOL AddUser( const CUser& user) = 0;  //返回0成功
	virtual BOOL AddRole( CRole &role ) = 0;
	virtual BOOL ValidateUser( CUser& user ) = 0; //输入输出参数返回0成功

	virtual BOOL UpdateUserOnline( int id,  UserOnLineStatus status ) = 0; //更新登陆状态
	virtual BOOL UpdateUserPwd( int id,  const CString & pwd) = 0; //更新登陆密码
	virtual BOOL UpdateUser( int id, const TaskIdStringMap &fields ) = 0;
	virtual BOOL UpdateRole( const CRole &role ) = 0;
	virtual BOOL UpdateRolePower( int roleId, const IntStringMap &lst ) = 0;
	virtual BOOL UpdateExportSeq( int id, int seq ) = 0;

    virtual BOOL GetUserPowersByRoleId( int id, RolePowerList &rtlist) = 0;
	virtual BOOL GetUserById( int id, CUser &obj ) = 0;
	virtual BOOL GetUserByName( const CString &name, CUser &obj ) = 0;
	virtual BOOL GetUserByLoginName( const CString &name, CUser &obj ) = 0;

	virtual BOOL GetRoleById( int roleId, CRole &role ) = 0;
	virtual BOOL GetRoleByName( const CString &roleName, CRole &role ) = 0;
	virtual BOOL GetAllRole( ListRole &lst ) = 0;
	virtual BOOL GetRoleByUserId( int userId, CRole &role )  = 0;
	virtual BOOL GetPowerByRoleId( int roleId, IntStringMap &map ) = 0;
	
	virtual BOOL  GetAllPower( IntStringMap &map ) = 0;
	virtual BOOL GetGernalManger( CUser &obj ) = 0; //获取总经理
	virtual BOOL GetUserByPower( Power power, ListUser &obj ) = 0;
	virtual BOOL GetAllUser( ListUser &obj  ) = 0;
	virtual BOOL GetAllUserOrdyByExport( ListUser &obj ) = 0;

	virtual BOOL GetPrjectMember( int prjId, ListUser& listObj ) = 0;
	virtual BOOL GetDepartMember( int departId, ListUser &listObj )=0;
    
    virtual BOOL DeleteUser( int userId ) = 0;
	virtual BOOL DeleteRole( int roleId ) = 0;
	virtual BOOL    GetConect() { 
		if ( m_pDaoSupportInc)
		{	m_pDaoSupportInc->IsOpen();}
		return FALSE;}
protected:
	CDaoSupportInterface* m_pDaoSupportInc;
};
#endif