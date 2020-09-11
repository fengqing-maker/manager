#ifndef USER_MODEL_3435565
#define USER_MODEL_3435565

#include "TypeDefHead.h"

enum UserOnLineStatus
{
	leftLine,  //离线
	onLine, //在线
};
enum UserWorkStatus
{
	onTheJob, //在职
	departure, //离职
};
class CUser
{
public:
	CUser();
	~CUser();
	CUser( const CUser &obj );

	void SetId( int id );
	int  GetId() const;

	void SetUserName_R( const CString &userName );
	const CString& GetUserName_R() const;

	void SetLoginName( const CString &str );
	const CString& GetLoginName() const;

	void SetDepartId( int id );
	int  GetDepartId() const;

	void SetRolId( int id );
	int  GetRolId() const;

	void SetExportSeq( int id ){ m_exportSeq = id;};
	int  GetExportSeq() const { return m_exportSeq;};

	void SetUserStatus( const CString &str );
	const CString& GetUserStatus() const;

	void SetPwd( const CString &str );
	const CString& GetPwd() const;


	//void SetOnLineStatus( const CString &str );
	//const CString& GetOnLineStatus() const;
	
	void  SetOnLineStatus( int st );
	int  GetOnLine() const{ return m_onLineStatus;}
	UserWorkStatus    GetWorkStatus() const { return m_workStatus;}
	//static
	static void GetOnLineString( int status, CString &retString );
	static void GetOnWorkString( UserWorkStatus status, CString &retString );
	static void GetOnLineStrings( IntStringMap &map );
    static void GetOnWorkStrings( IntStringMap &map );

	void    CopyObj( const CUser &obj );
	CUser & operator = (const CUser & obj); 
private:
	int m_id; //用户id
	CString m_strName; //用户名
	CString m_strLogName; //用户登陆名
	int m_departId; //部门ID
	int m_roleId;  //角色ID
	CString m_strStatus; //状态，在职，还是离职
	CString m_strPwd; //登陆密码
	CString m_strOnline; //下线，在线
	int              m_onLineStatus;
	UserWorkStatus   m_workStatus;
	int              m_exportSeq;
};

#endif