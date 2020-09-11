#ifndef WORHOUR_DB_IMP_347
#define WORHOUR_DB_IMP_347
#include "../inc/WorkHourDbInterface.h"

class CWorkHourDbImp:public CWorkHourDbInc
{
public:
	CWorkHourDbImp( CDaoSupportInterface* pDao );

	virtual ~CWorkHourDbImp();

	virtual BOOL UpdateTimeData( CWorkHourData &data ); //如果不存在此条记录则用插入，若存在则修改
	//virtual BOOL UpdateTimeData( int dataId, const TaskIdStringMap &fields );
	virtual BOOL GetTimeData( int timeType, int userId, const COleDateTime &time, CWorkHourData &obj );
	virtual BOOL GetNorTaskHourSum( int userId, const COleDateTime &time, double &relsut ); //timetype < 0查询所有的
	
protected:
	BOOL ValidateTimeDataExist( const CWorkHourData &data );
	void FillTimeDataFromFild( CAdoRecordset* pRecordSet, CWorkHourData &obj  ) const;
protected:
	const CString m_timeDataTableName;
};

#endif