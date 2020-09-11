#ifndef  WORK_HOUR_SEVER_INTERFAC_AW3948
#define  WORK_HOUR_SEVER_INTERFAC_AW3948

#include "../../DataBase/factory/CDaoFactory.h"

class CWorkHourSeverInc
{
public:
	CWorkHourSeverInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CWorkHourSeverInc(){};
	virtual BOOL GetWorkTimeData( int timeType, int userId, const COleDateTime &time, CWorkHourData &obj ) = 0; 
	virtual BOOL GetHolidayHour( int userId,  const COleDateTime &time, CWorkHourData &obj ) = 0;
    virtual BOOL AddOrUpdateWorkHour( CWorkHourData &data ) = 0;
    virtual BOOL GetNorTaskHourSum( int userId, const COleDateTime &time, double &relsut ) = 0; //timetype < 0查询所有的
	virtual BOOL GetTaskHourSum( int userId, int timeType, const COleDateTime &time, double &relust ) = 0;
	virtual BOOL GetTaskHourSumExceptOngoing( int userId, const COleDateTime &time, double &reluset ) = 0;
	//更新日期检查
	virtual BOOL GetUnUpdateDate( int taskId, ListDate &lst ) = 0;
	virtual BOOL AddUnUpdateDate( int taskId, const COleDateTime &date ) = 0;
	virtual BOOL DeleteUnUpdateDate( int taskId, const COleDateTime &date ) = 0;
	virtual BOOL DeleteUnUpdateDateByUserId( int userId, const COleDateTime &date ) = 0;
	virtual BOOL GetUnUpdateCnt( int taskId, int &num ) = 0;
protected:
	CDaoFactory       *m_pDbDao;

};

#endif