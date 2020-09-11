#ifndef CUSER_DAO_IMP_H_34IU8
#define CUSER_DAO_IMP_H_34IU8

#include "../inc/CUserDaoInterface.h"




class CUerDaoImp:public CUserDaoInterface
{
public:
	CUerDaoImp( CDaoSupportInterface *pDaoSupportInc );
	virtual ~CUerDaoImp( ){};

	//向user表添加一条记录
    virtual	BOOL AddUser( const CUser& user);
	virtual BOOL AddRole( CRole &role );

	virtual BOOL ValidateUser(  CUser& user );
	
	virtual BOOL UpdateUserOnline( int id, UserOnLineStatus status );
	virtual BOOL UpdateUserPwd( int id,  const CString & pwd);
	virtual BOOL UpdateUser( int id, const TaskIdStringMap &fields );
	virtual BOOL UpdateRole( const CRole &role );
	virtual BOOL UpdateRolePower( int roleId, const IntStringMap &lst );
	virtual BOOL UpdateExportSeq( int id, int seq );

	//power
	virtual BOOL GetUserById( int id, CUser &obj );
	virtual BOOL GetUserByName( const CString &name, CUser &obj );
	virtual BOOL GetUserByLoginName( const CString &name, CUser &obj );
    virtual BOOL GetUserPowersByRoleId( int id, RolePowerList &rtlist);

	virtual BOOL GetRoleById( int roleId, CRole &role );
	virtual BOOL GetRoleByName( const CString &roleName, CRole &role ) ;
	virtual BOOL GetAllRole( ListRole &lst );
	virtual BOOL GetRoleByUserId( int userId, CRole &role );
	virtual BOOL GetPowerByRoleId( int roleId, IntStringMap &map );
	virtual BOOL GetAllPower( IntStringMap &lst );
	virtual BOOL GetGernalManger( CUser &obj );
	virtual BOOL GetUserByPower( Power power, ListUser &obj );
	virtual BOOL GetAllUser( ListUser &obj  );
	virtual BOOL GetAllUserOrdyByExport( ListUser &obj );

	virtual BOOL GetDepartMember( int departId, ListUser &listObj );
	virtual BOOL GetPrjectMember( int prjId, ListUser& listObj );
	//
	virtual BOOL DeleteUser( int userId );
	virtual BOOL DeleteRole( int roleId );
protected:
	BOOL         ValidateUser( int id );
	void         FillUserFromFild( CAdoRecordset *pRecordSet, CUser &obj );
	 BOOL        ExcuteSqlResult( const CString &str);
	 BOOL        ExcuteSqlResult( const CString &str, ListUser &obj );
	 CString     GetUserFieldByType(int fieldType );
	 void        FillRoleFromFild(CAdoRecordset *pRecordSet, CRole &role );
private:
	CString  m_strUserTableName;
	CString  m_roleTableName;
	CString  m_roleMapTableName;
};
#endif