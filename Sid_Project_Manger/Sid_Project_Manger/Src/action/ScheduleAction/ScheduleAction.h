#ifndef SCHEDULE_ACTION_SJIERU_234
#define SCHEDULE_ACTION_SJIERU_234

#include "../inc/scheduleAcionInterface.h"

class CSCheduleAction:public CScheduleActionInc
{
public:
	CSCheduleAction( CDaoFactory  *pObj );

	virtual ~CSCheduleAction();

	virtual BOOL InsertSchedule( const CSchedule &obj );
	virtual BOOL InsertEmptySchdule( int taskId, const  COleDateTime &date );

	virtual BOOL GetMaxNewScheduleByTaskId( int taskId, CSchedule& obj );
	virtual BOOL OnReviewSchedule( const CTaskObj &obj, const CString &reviewName, const CString &reviewStr, int score ); //进度点评
	virtual BOOL OnPrjReviewSchedule( const CTaskObj &obj, const CString &reviewName, const CString &reviewStr ); //项目点评
	virtual BOOL GetScheduleContentById( int id, CSchedule& obj );
	virtual BOOL GetUnUpdateDateList( const CTaskObj &obj, ListDate &lst);
	virtual BOOL GetSchduleByDate( int taskId, const COleDateTime &date, CSchedule &obj );
	virtual BOOL UpdateCheckBeginDate( const CTaskObj &obj, const COleDateTime &date );
    virtual BOOL GetTaskWorkHourSum( int taskId,  double &relust );
	virtual BOOL GetTaskWorkHourSum( int taskId, const COleDateTime &beginDate, const COleDateTime &endDate,double &relust ); //[beginDate ~ endDate]
	virtual BOOL SetTaskFactWorkHour( CTaskObj &obj );

	virtual BOOL OnModiyTaskScore( const CTaskObj &obj, int score, const CString &soreDec );

	virtual COleDateTime GetLastSetUpdateData();
protected:
	
	BOOL TaskFinishApply( const CTaskObj &obj, const COleDateTime &finishDate );
protected:
	CTaskDaoInc  *m_pTaskDbDAo;
};
#endif