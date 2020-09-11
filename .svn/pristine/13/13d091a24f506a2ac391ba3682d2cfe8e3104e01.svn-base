#ifndef  LOGINE_ACTIONG_IMP_H_347
#define  LOGINE_ACTIONG_IMP_H_347

#include "../inc/loginActionInterfac.h"

class CLoginActionImp:public CLoginActionInc
{
public:
	CLoginActionImp(  CDaoFactory *pFactory );
	//登陆系统
	virtual BOOL         LoginSystem( const CString &loginName, const CString &loginPwd );//输入输出，输入:登陆名，登陆密码
	virtual const CUser& GetLoginUser() ;
	virtual CString      GetOrganizationString(); //获取组织名称
	virtual  const ListUser& GetPubishPowweUser();
	virtual  const CDepartment&  GetLoginDepart(){ return m_loginUserDepart;}
	virtual  const CRole&        GetLoginRole(){ return m_role;}
	virtual  BOOL                GetLoginOrginMember( ListUser &obj );

	virtual BOOL                   GetUserPower( int userId, RolePowerList &listObjs )  ;
	virtual const RolePowerList&   GetLoginUserPower() ;
	virtual BOOL                   Loading();
	virtual void                   QuitLogin();
	virtual BOOL                   SetLoginUserPwd( const CString &newPwd );
	virtual BOOL                   GetUserByPower( Power pt, ListUser &list );
	virtual BOOL                   GetAllUser( ListUser &list );
	virtual BOOL                   GetAllUserOrdyByExport( ListUser &obj );
	//权限
    virtual BOOL                  FindLoginPower( Power pt ) const;
	virtual BOOL                  FindUserPower( int userId, Power pt );
	virtual BOOL                  GetRoleById( int roleId, CRole &role );

	virtual const CUser &        GetLoginUserLeader();
	virtual BOOL                 GetUserLeader(const CUser &obj, CUser &leader );
	virtual BOOL                 GetUserById( int userId, CUser &user );
	virtual BOOL                 GetUserByName( const CString &strName, CUser &obj );
	 virtual BOOL                GetUserByLoginName( const CString &strName, CUser &obj );
	virtual const CUser&         GetGernalMangerUser() const { return m_generManger;}

	
protected:
	CString  GetOrganizationString( const CDepartment &depart );
    BOOL        InitDepartInfo();
	BOOL        FindPower( Power pt, const RolePowerList &list) const;

protected:
	CUser                m_loginUser;
	CUser                m_generManger;
	CUser                m_loginUserLeader;
	ListUser             m_publishUser; //有发布任务权限的人

	CDepartment          m_loginUserDepart;
	RolePowerList        m_powerList;
	CRole                m_role;

	CUserDaoInterface    *m_pUserInc;
	CDepartmentAdoInc    *m_pDepartmentInc;
};

#endif