#ifndef JISE_ROL_SHOW_2I3U
#define JISE_ROL_SHOW_2I3U
/*
  Ŀǰϵͳ��ɫ
  �з�����ʦ
  ���ž���
  ��Ŀ����
  ���ܾ���
  ϵͳ����
  �������� ��Ӧ�鳤
  ������   
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
	int m_id;					//Ȩ�޵�ID��
	CString m_roleName;			//��ӦȨ�޵�����
	CString m_strDec;			//����ǳ���Ȩ�ޣ�
};

#endif