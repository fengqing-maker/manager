#ifndef  TASK_DAO_IMP_HEARD_1234
#define  TASK_DAO_IMP_HEARD_1234

#include "../inc/taskdaointerface.h"


class CTaskDaoImp:public CTaskDaoInc
{
	
public:
	CTaskDaoImp( CDaoSupportInterface* pDao);
	//增
	virtual BOOL InsertTask( CTaskObj &obj );
	virtual BOOL AddTaskSchedule( const CSchedule &obj );
	virtual BOOL AddEmptySchedule( int taskId, const COleDateTime &date );
	//改
	virtual BOOL UpdateTask( const CTaskObj &obj );
	virtual BOOL UpdateTaskField( int taskId, const TaskIdStringMap &fields );
	virtual BOOL UpdateTaskField(int taskId, int fieldType, const CString &value );
	virtual BOOL UpdateFactTimeTask( const CTaskObj &obj );
	virtual BOOL UpdateTaskFather( int taskId, int fatherId );

	virtual BOOL UpdateSchedule( int id, const TaskIdStringMap &fields); //比如更新点评
	virtual BOOL GetScheduleByPer( int taskId, int percent, CSchedule &obj );
	virtual BOOL GetScheduleByUpdateDate( int taskId, const COleDateTime &day, CSchedule &obj );

	//删
	virtual BOOL DeleteTaskById( int taskId );
	virtual BOOL DeleteNoneShcdule( int taskId ); //删除空白进度
	//关系任务
	virtual BOOL  IsHasChildTask( int taskId, BOOL &isHasChild );
	virtual BOOL  GetChildTask( BOOL fatherId, int st ,int ownerId, ListTask &taskIds ); //获取子任务
	virtual BOOL  GetFatherTask( int taskId, CTaskObj &fatherId ); //获取父任务
	//任务相关
	virtual BOOL GetTaskContentById( int taskid, CTaskObj& obj );
	virtual BOOL GetTaskByOwnerId( int userId, ListTask &listObj, int year = 0, int quater = 0 ) ;
	virtual BOOL GetTaskByOwernerId( int owernId, EnumStaus status, ListTask &listObj, int year = 0, int quater = 0);
	virtual BOOL GetNewingTaskByCreateId( int createId, ListTask &listObj );
	virtual BOOL GetNewTaskButNotOwer( int createId, ListTask &listObj );
	virtual BOOL GetTaskByReviewId( int reviewId, EnumStaus status, ListTask &listObj); //获取
	virtual BOOL GetTaskByProjectManger( int prjMangerId, EnumStaus status,ListTask &listObj ); //获取某人管理的项目任务
	virtual BOOL GetTaskByProjectId( int prjId, EnumStaus status,ListTask &listTask ); //获取项目的任务
	virtual BOOL GetTaskByProjectId( int prjId, ListTask &listTask );
	virtual BOOL GetPerformProjectTask( int year, int quater, int owerId, ListTask &listObj );
	virtual BOOL GetPerformDepartTask( int year, int quater, int owerId, ListTask &listObj );
	virtual BOOL GetTaskByDate( int ownerId, const COleDateTime &day, ListTask &listObj );

	//工时相关
	virtual BOOL    AddWorkType(const CString &name );
	virtual BOOL    GetWorkTimeTypeStr( int id, CString &retStr ); //获取工时类型
	virtual BOOL    GetAllWorkTimeType( TaskIdStringMap &obj);
	virtual BOOL    UpdateWorkTimeType( int id, const CString &str );
	virtual BOOL    DeleteWorkTypeString( int id );
	//进度相关
	virtual BOOL GetScheduleContentById( int id, CSchedule& obj );
	virtual BOOL GetMaxNewScheduleByTaskId( int taskid, CSchedule& obj );
    virtual BOOL GetScheduleContentByTaskId( int taskId, ListSchedule& obj );
    virtual BOOL GetScheduleContentByTaskId( int taskId, ReviewType st, ListSchedule & obj );
	virtual BOOL GetScheduleContentByPrjTaskId( int taskId, ReviewType st, ListSchedule & obj );//根据项目状态进行筛选进度
	virtual BOOL GetScheduleContentByDate(int ownerId, const COleDateTime &beginTime, const COleDateTime &endTime, ListSchedule &obj);
	virtual BOOL GetTaskWorkHourSum(int ownerId, int timeType, const COleDateTime &time, double &relust );
	virtual BOOL GetTaskWorkHourSum( int taskId,  double &relust );
	virtual BOOL GetTaskWorkHourSum( int taskId, const COleDateTime &beginDate, const COleDateTime &endDate,double &relust );
	virtual BOOL GetTaskHourSumExceptSt(int ownerId, int st, const COleDateTime &time, double &relust );
	//
	virtual BOOL    GetTaskStatusMap( CTaskStaus &obj );
	//
	virtual BOOL    GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj );
	virtual BOOL GetProjectTaskNoFatherHasChild( int prjId, int taskSt, int ownerId,  ListTask &obj, BOOL taskFlag = 0 );
	virtual BOOL GetProjectTaskHasFahter( int prjId, int taskSt, int ownerId, ListTask &fahterId, ListTask &obj, BOOL taskFlag = 0 );
	virtual BOOL GetProjectTaskNoFatherNoChild( int prjId, int taskSt, int ownerId, ListTask &obj, BOOL taskFlag = 0 );
	//更新日期检查相关的
	virtual BOOL GetUnUpdateDate( int taskId, ListDate &lst );
	virtual BOOL AddUnUpdateDate( int taskId, const COleDateTime &date );
	virtual BOOL DeleteUnUpdateDate( int taskId, const COleDateTime &date );
	virtual BOOL DeleteUnUpdateDateByUserId( int taskId, const COleDateTime &date );
	virtual BOOL GetUnUpdateCnt( int taskId, int &cnt );

	virtual BOOL    GetLastInsertId( int & ret );

protected:
	virtual BOOL    ValidateTaskExit( int id );
	void	FillTaskFromFild(  CAdoRecordset* pRecordSet, CTaskObj &obj ) const;
	void    FillReslutList( CAdoRecordset* pRecordSet , ListTask &listObj) const;
	void    FillScheduleFromFild( CAdoRecordset* pRecordSet, CSchedule &obj  ) const;
	void    FillScheduleList( CAdoRecordset* pRecordSet , ListSchedule &listObj ) const;
    BOOL    ExcSqlScheduleList( const CString &seq, ListSchedule &obj );
	BOOL    ExcSqlTaskList( const CString &seq,  ListTask &listObj );
	CString GetFieldStringByType(int fieldType );
	CString GetSCheduleFieldString(int fieldType );
	//
	CString GetQuarterOfYearFilfterStr( int year, int  quarter, int st );
protected:
    CString m_taskTableName;
	CString m_scheduleName;
	CString m_fatherTableName;
	CString m_workTypeTableName;
    CString m_unUpateDateTableName;
};

#endif
