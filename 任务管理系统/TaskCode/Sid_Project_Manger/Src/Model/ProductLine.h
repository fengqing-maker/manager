#ifndef PRODUCT_LINE_HEARD_3559
#define PRODUCT_LINE_HEARD_3559

class CProductLine
{
public:
	CProductLine();
	void SetId( int id ){m_id = id;}
	int GetId() const{return m_id;}
    
	void SetMangerId( int id ){m_mangerId = id;}
	int GetMangerId() const{return m_mangerId;}

	void SetName( const CString &str ){m_strName = str;}
	const CString& GetName() const{return m_strName;}

	void SetDec( const CString &str ){m_bakStr = str;}
	const CString& GetDec() const{return m_bakStr;}

	void CopyObj( const CProductLine &obj );
	CProductLine( const CProductLine &obj  );
	CProductLine& operator=( const CProductLine &obj);
private:
	int     m_id;
	int     m_mangerId;
	CString m_bakStr;
	CString m_strName;
};
#endif