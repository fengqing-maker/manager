#ifndef PROJ_ECT_STEP_34J32
#define PROJ_ECT_STEP_34J32

/*
项目流程阶段
*/

class CProjectStep
{
public:
	CProjectStep();
	CProjectStep( const CProjectStep & );
	CProjectStep& operator==( const CProjectStep & );
	void CopyObj( const CProjectStep & );

	void SetId( int id ){ m_id = id; }
	int  GetId() const { return m_id; }

	void SetNum( int num ) { m_num = num;}  
	int	 GetNum() const { return m_num; }

	void SetName( const CString &name ){ m_name = name;}
	const CString& GetName() const { return m_name;}

	void SetTypeName( const CString &name ){ m_strType = name;}
	const CString& GetTypeName() const { return m_strType;}

private:
	int      m_id;
	int      m_num;
	CString  m_name;
	CString  m_strType;
};

#endif