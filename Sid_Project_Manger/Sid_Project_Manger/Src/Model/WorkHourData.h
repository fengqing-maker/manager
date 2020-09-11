#ifndef WORK_HOUR_DATA_DIFIJE_HEARD_12
#define WORK_HOUR_DATA_DIFIJE_HEARD_12

class CWorkHourData
{
public:
	CWorkHourData();
	CWorkHourData( const CWorkHourData & );
	CWorkHourData& operator==( const CWorkHourData & );
	void CopyObj( const CWorkHourData & );

	void SetId( int id ){ m_id = id; }
	int  GetId() const { return m_id; }

	void SetTimeType( int data ){ m_timeType = data; }
	int  GetTimeType() const { return m_timeType;}

	void SetUserId( int id ) { m_userId = id; }
	int  GetUserId() const{ return m_userId;}

	void SetTaskWorkHour( double data ) { m_taskWorkHour = data; }
	double GetTaskWorkHour() const { return m_taskWorkHour;}

    void SetNorTaskWorkHour( double data ) { m_norTaskWorkHour = data; }
	double GetNorTaskWorkHour() const { return m_norTaskWorkHour;}

	//void SetStrBak( const CString &str ){ m_strBak = str;}
	//const CString& GetStrBak() const { return m_strBak;}

	void SetDate( const COleDateTime &data ){ m_date = data;}
	const COleDateTime& GetDate() const{ return m_date;}

	void SetDateByString( const CString &str );
	CString GetDateStr() const;

private:
	int          m_id;
	int          m_timeType;
	int          m_userId;
	double        m_taskWorkHour;
    double        m_norTaskWorkHour;
	COleDateTime m_date;
	//CString      m_strBak;
};

#endif