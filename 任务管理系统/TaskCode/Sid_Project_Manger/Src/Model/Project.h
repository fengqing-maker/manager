#ifndef PRJECT_HEARD_34MODEL
#define PRJECT_HEARD_34MODEL

class CProject
{
public:
	CProject();
	void SetId( int id ){m_id = id;}
	int GetId() const{return m_id;}

	void SetShortName( const CString &str ){m_shortName = str;}
	const CString& GetShortName() const{return m_shortName;}

	void SetMangerId( int id ){m_mangerId = id;}
	int GetMangerId() const{return m_mangerId;}

	void SetLineId( int id ){m_lineId = id;}
	int GetLineId() const{return m_lineId;}

	void SetLongName( const CString &str ){m_strLongName = str;}
	const CString& GetLongName() const{return m_strLongName;}

	void SetPrjStepType( const CString &str ){ m_prjStepType = str; }
	const CString& GetPrjStepType() const{ return m_prjStepType; }


	void CopyObj( const CProject &obj );
	CProject( const CProject &obj  );
	CProject& operator=( const CProject &obj);
private:
	int m_id;
	int m_mangerId;
	int m_lineId;
	CString m_shortName;
	CString m_strName;
	CString m_strLongName;
	CString m_prjStepType;
};

#endif