#ifndef DEPARTEMNT_HEARD_12343985
#define DEPARTEMNT_HEARD_12343985

class CDepartment
{
public:
	CDepartment();
	void SetId( int id ){m_id = id;}
	int GetId() const{return m_id;}

	void SetName( const CString &str ){m_strName = str;}
	const CString& GetName() const{return m_strName;}

	void SetMangerId( int id ){m_mangerId = id;}
	int GetMangerId() const{return m_mangerId;}

	void SetDec( const CString &str ){m_bakStr = str;}
	const CString& GetDec() const{return m_bakStr;}

	int  GetBakManger( ) const{ return m_bakMangerId;}
	void SetBakManger(int id ) { m_bakMangerId = id;}

	BOOL  GetBakValidate( ) const { return m_isBakValidate;}
	void  SetBakValidate(BOOL id ) { m_isBakValidate = id;}

	BOOL  GetTopDepartId( ) const { return m_fatherDepartId;}
	void  SetTopDepartId(BOOL id ) { m_fatherDepartId = id;}

	CDepartment(const CDepartment &depart );
	void CopyObj( const CDepartment &depart );
	CDepartment& operator=( const CDepartment &depart ); 
protected:
private:
	int m_id;										//����ID
	int m_mangerId;									//���ŵĹ�����ID
	CString m_bakStr;								//���ŵĺ�̨����
	CString m_strName;								//��������
	int m_bakMangerId;    //���ù���Ա				//���ŵĺ�̨������ID
	BOOL m_isBakValidate; //���ù���Ա�Ƿ���Ч			
	BOOL m_fatherDepartId; //�Ƿ�Ϊ��������
};
#endif