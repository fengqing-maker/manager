#ifndef  TASK_DAO_INTERFACE_H_156485
#define  TASK_DAO_INTERFACE_H_156485

#include "DaoSupportInterface.h"
#include "daocomonDef.h"
#include "../../Model/Schedule.h"
/*
  任务条目及进度
*/
typedef std::list<CSchedule> ListSchedule;

typedef std::map<int, CSchedule> TaskScheduleMap;

typedef ListSchedule::iterator       ListScheduleItor;
typedef ListSchedule::const_iterator ListScheduleConstItor;
typedef ListString::iterator         ListStringItor;
typedef TaskScheduleMap::iterator  TaskScheduleMapItor;

typedef std::list<COleDateTime> ListDate;
typedef ListDate::iterator ListDateItor;
typedef ListDate::const_iterator ListDateConstItor;


class CTaskDaoInc
{
public:
	CTaskDaoInc( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
    virtual ~CTaskDaoInc(){};
    //增
	virtual BOOL InsertTask( CTaskObj &obj ) = 0;
	virtual BOOL AddTaskSchedule( const CSchedule &obj ) = 0;
	virtual BOOL AddEmptySchedule( int taskId, const COleDateTime &date ) = 0;
	//改
	virtual BOOL UpdateTask( const CTaskObj &obj ) = 0;
	virtual BOOL UpdateTaskField(int taskId, const TaskIdStringMap &fields ) = 0;
	virtual BOOL UpdateTaskField(int taskId, int fieldType, const CString &value ) = 0; //更新字段数值
	virtual BOOL UpdateFactTimeTask( const CTaskObj &obj ) = 0;

	virtual BOOL UpdateSchedule( int id, const TaskIdStringMap &fields ) = 0; //比如更新点评
	virtual BOOL UpdateTaskFather( int taskId, int fatherId ) = 0;
	virtual BOOL GetScheduleByPer( int taskId, int percent, CSchedule &obj ) = 0;
	virtual BOOL GetScheduleByUpdateDate( int taskId, const COleDateTime &day, CSchedule &obj ) = 0;
	//删了
	virtual BOOL DeleteTaskById( int taskId ) = 0;
	virtual BOOL DeleteNoneShcdule( int taskId ) = 0;
	//
	virtual BOOL  IsHasChildTask( int taskId, BOOL &isHasChild ) = 0;
	virtual BOOL  GetChildTask( int taskId, int st, int ownerId, ListTask &taskIds ) = 0; //获取子任务
	virtual BOOL  GetFatherTask( int taskId, CTaskObj &fatherId ) = 0; //获取父任务

	virtual BOOL GetTaskContentById( int taskid, CTaskObj& obj ) = 0;
	virtual BOOL GetTaskByOwnerId( int owernId, ListTask &listObj, int year = 0, int quater = 0 ) = 0;
	virtual BOOL GetTaskByOwernerId(int owernId, EnumStaus status, ListTask &listObj, int year = 0, int quater = 0 ) = 0;
	virtual BOOL GetNewingTaskByCreateId( int createId, ListTask &listObj ) = 0;
	virtual BOOL GetNewTaskButNotOwer( int cretateId, ListTask &listObj ) = 0;
	virtual BOOL GetTaskByReviewId( int reviewId, EnumStaus status,ListTask &listObj ) = 0; //获取
	virtual BOOL GetTaskByProjectManger( int prjMangerId, EnumStaus status,ListTask &listObj ) = 0; //根据项目管理者获取项目任务

	virtual BOOL GetTaskByProjectId( int prjId, EnumStaus status,ListTask &listTask ) = 0; //获取项目的任务
	virtual BOOL GetTaskByProjectId( int prjId, ListTask &listTask )= 0;
	virtual BOOL GetPerformProjectTask( int year, int quater, int owerId, ListTask &listObj ) = 0;
	virtual BOOL GetPerformDepartTask( int year, int quater, int owerId, ListTask &listObj ) = 0;
	virtual BOOL GetTaskByDate( int ownerId, const COleDateTime &day, ListTask &listObj ) = 0;

	//工时
	virtual BOOL    AddWorkType(const CString &name ) = 0;
	virtual BOOL    GetWorkTimeTypeStr( int id, CString &retStr ) = 0; //获取工时类型
	virtual BOOL    GetAllWorkTimeType( TaskIdStringMap &obj) = 0;
	virtual BOOL    UpdateWorkTimeType( int id, const CString &str ) = 0;
	virtual BOOL    DeleteWorkTypeString( int id ) = 0;

	//进度相关查询
	virtual BOOL   GetScheduleContentById( int id, CSchedule& obj ) = 0;
	virtual BOOL   GetMaxNewScheduleByTaskId( int taskid, CSchedule& obj ) = 0;
	virtual BOOL   GetScheduleContentByTaskId( int taskid, ListSchedule& obj ) = 0;
    virtual BOOL   GetScheduleContentByTaskId( int taskId, ReviewType, ListSchedule& obj ) = 0;
    virtual BOOL   GetScheduleContentByPrjTaskId( int taskId, ReviewType type, ListSchedule & obj ) = 0;//根据项目状态进行筛选进度
    virtual BOOL   GetScheduleContentByDate(int ownerId, const COleDateTime &beginTime, const COleDateTime &endTime, ListSchedule &obj) = 0;
	virtual BOOL   GetTaskWorkHourSum(int ownerId, int timeType, const COleDateTime &time, double &relust ) = 0;
	virtual BOOL   GetTaskWorkHourSum( int taskId,  double &relust ) = 0;
	virtual BOOL GetTaskWorkHourSum( int taskId, const COleDateTime &beginDate, const COleDateTime &endDate,double &relust ) = 0;
	virtual BOOL   GetTaskHourSumExceptSt(int ownerId, int st, const COleDateTime &time, double &relust ) = 0; 
	//
	virtual BOOL    GetTaskStatusMap( CTaskStaus &obj ) = 0;

	virtual BOOL    GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj ) = 0;
	virtual BOOL GetProjectTaskNoFatherHasChild( int prjId, int taskSt, int ownerId, ListTask &obj, BOOL taskFlag = 0 ) = 0;
	virtual BOOL GetProjectTaskHasFahter( int prjId, int taskSt, int ownerId,ListTask &fahterId, ListTask &obj, BOOL taskFlag = 0 ) = 0; 
	virtual BOOL GetProjectTaskNoFatherNoChild( int prjId, int taskSt,int ownerId,  ListTask &obj, BOOL taskFlag = 0 ) = 0;
	
		
	//更新日期检查相关的
	virtual BOOL GetUnUpdateDate( int taskId, ListDate &lst ) = 0;
	virtual BOOL AddUnUpdateDate( int taskId, const COleDateTime &date ) = 0;
	virtual BOOL DeleteUnUpdateDate( int taskId, const COleDateTime &date ) = 0;
	virtual BOOL DeleteUnUpdateDateByUserId( int taskId, const COleDateTime &date ) = 0;
	virtual BOOL GetUnUpdateCnt( int taskId, int &cnt ) = 0;

	virtual BOOL    GetLastInsertId( int & ret ) = 0;
	virtual BOOL    GetConect() { 
    if ( m_pDaoSupportInc)
	{	m_pDaoSupportInc->IsOpen();}
	return FALSE;}
protected:
	CDaoSupportInterface* m_pDaoSupportInc;
};

#endif