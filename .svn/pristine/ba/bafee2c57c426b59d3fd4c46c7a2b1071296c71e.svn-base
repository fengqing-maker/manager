#ifndef JISE_ROL_SHOW_2I3U
#define JISE_ROL_SHOW_2I3U
/*
  目前系统角色
  研发工程师
  部门经理
  项目经理
  副总经理
  系统管理
  部门主管 对应组长
  代理人   
*/
class CRole
{
public:
	CRole(){ m_id = -1; }
	void SetId( int id ){m_id = id;}
	int GetId() const{return m_id;}

	void SetRoleName( const CString &str ){m_roleName = str;}
	const CString& GetRoleName() const{return m_roleName;}

	void SetRoleDec( const CString &str ){m_strDec = str;}
	const CString& GetRolDec() const{return m_strDec;}

	void CopyObj( const CRole &obj );
	CRole& operator==( const CRole &obj );
	CRole( const CRole &obj );
private:
	int m_id;					//权限的ID号
	CString m_roleName;			//对应权限的名称
	CString m_strDec;			//这个是超级权限？
};

#endif