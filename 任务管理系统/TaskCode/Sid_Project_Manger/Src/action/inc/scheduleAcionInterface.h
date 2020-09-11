#ifndef SCHEDULE_ACTION_INTERFACE_W3J4
#define SCHEDULE_ACTION_INTERFACE_W3J4

#include "../../DataBase/factory/CDaoFactory.h"



class CScheduleActionInc
{
public:
	CScheduleActionInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CScheduleActionInc(){};

	virtual BOOL InsertSchedule( const CSchedule &obj ) = 0;
    virtual BOOL InsertEmptySchdule( int taskId, const  COleDateTime &date ) = 0;

	virtual BOOL GetMaxNewScheduleByTaskId( int taskId, CSchedule& obj ) = 0;
	virtual BOOL OnReviewSchedule( const CTaskObj &obj, const CString &reviewName, const CString &reviewStr, int score ) = 0;
	virtual BOOL OnPrjReviewSchedule( const CTaskObj &obj, const CString &reviewName,const CString &reviewStr ) = 0;
	virtual BOOL GetScheduleContentById( int id, CSchedule& obj ) = 0;
    virtual BOOL GetUnUpdateDateList( const CTaskObj &obj, ListDate &lst) = 0;
    virtual BOOL GetSchduleByDate( int taskId, const COleDateTime &date, CSchedule &obj ) = 0;
	virtual BOOL UpdateCheckBeginDate( const CTaskObj &obj,const COleDateTime &date ) = 0;
	virtual BOOL OnModiyTaskScore( const CTaskObj &obj, int score, const CString &soreDec ) = 0;
    virtual BOOL GetTaskWorkHourSum( int taskId,  double &relust ) = 0;
    virtual BOOL GetTaskWorkHourSum( int taskId, const COleDateTime &beginDate, const COleDateTime &endDate,double &relust ) = 0; //[beginDate ~endDate]
	virtual BOOL SetTaskFactWorkHour( CTaskObj &obj ) = 0;

	virtual COleDateTime GetLastSetUpdateData() = 0;

protected:
	CDaoFactory  *m_pDbDao;
};

#endif