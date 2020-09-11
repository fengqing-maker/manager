#ifndef  WORK_HOUR_SEVER_IMP_H_3847
#define  WORK_HOUR_SEVER_IMP_H_3847

#include "../inc/WorkHourActionInc.h"

class CWorkHourSverImp:public CWorkHourSeverInc
{
public:
	CWorkHourSverImp(  CDaoFactory *pFactory );
	virtual BOOL GetWorkTimeData( int timeType, int userId, const COleDateTime &time, CWorkHourData &obj );
	virtual BOOL GetHolidayHour( int userId,  const COleDateTime &time, CWorkHourData &obj );
	virtual BOOL AddOrUpdateWorkHour( CWorkHourData &data );
	virtual BOOL GetNorTaskHourSum( int userId, const COleDateTime &time, double &relsut ); //timetype < 0查询所有的
	virtual BOOL GetTaskHourSum( int userId, int timeType, const COleDateTime &time, double &relust );
	virtual BOOL GetTaskHourSumExceptOngoing( int userId, const COleDateTime &time, double &reluset );
	//更新日期检查
	virtual BOOL GetUnUpdateDate( int taskId, ListDate &lst );
	virtual BOOL AddUnUpdateDate( int taskId, const COleDateTime &date );
	virtual BOOL DeleteUnUpdateDate( int taskId, const  COleDateTime &date );
	virtual BOOL DeleteUnUpdateDateByUserId( int userId, const COleDateTime &date );
	virtual BOOL GetUnUpdateCnt( int taskId, int &num );

	//virtual BOOL  AddAskForLeaveContent( const COleDateTime &date, CString conttent );
	
protected:
	CWorkHourDbInc *m_pWorkDbDao;
};

#endif