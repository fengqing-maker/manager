#ifndef LOGINACTION_INTERFACE_H_2I34U
#define LOGINACTION_INTERFACE_H_2I34U

#include "../../DataBase/factory/CDaoFactory.h"

class CLoginActionInc
{
public:
	CLoginActionInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CLoginActionInc(){}

	//登陆系统
	virtual  BOOL                LoginSystem(const CString &loginName, const CString &loginPwd ) = 0;//输入输出，输入:登陆名，登陆密码
	virtual  const CUser&        GetLoginUser() = 0; 
	virtual  CString             GetOrganizationString() = 0; 
	virtual  const ListUser&     GetPubishPowweUser() = 0;
	virtual  const CDepartment&  GetLoginDepart() = 0;
	virtual  const CRole&        GetLoginRole() = 0;
	virtual  BOOL                GetLoginOrginMember( ListUser &obj ) = 0;

    virtual  BOOL                   GetUserPower( int userId, RolePowerList &listObjs ) = 0;
    virtual  const RolePowerList&   GetLoginUserPower() = 0;
	virtual  BOOL                   Loading() = 0;
	virtual BOOL                    GetUserByPower( Power pt, ListUser &list ) = 0;
	virtual BOOL                    GetAllUser( ListUser &list ) = 0;
	//权限
	virtual  BOOL                   FindLoginPower( Power pt ) const = 0;
	virtual  BOOL                   FindUserPower( int userId, Power pt ) = 0;
	virtual  BOOL                   GetRoleById( int roleId, CRole &role ) = 0;
	virtual  BOOL                   GetAllUserOrdyByExport( ListUser &obj ) = 0;

	virtual  void                   QuitLogin() = 0; //退出
	virtual  BOOL                   SetLoginUserPwd( const CString &newPwd ) = 0; //退出
	virtual  const CUser &          GetLoginUserLeader() = 0;
	virtual  BOOL                   GetUserLeader(const CUser &obj, CUser &leader ) = 0;
    virtual  BOOL                   GetUserById( int userId, CUser &user ) = 0;
	virtual const CUser&            GetGernalMangerUser() const = 0;
	//
	virtual BOOL GetUserByName( const CString &strName, CUser &obj ) = 0;
    virtual BOOL GetUserByLoginName( const CString &strName, CUser &obj ) = 0;
  
protected:
	CDaoFactory  *m_pDbDao;
};

#endif