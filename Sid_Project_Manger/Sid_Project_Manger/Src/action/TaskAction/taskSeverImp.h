#ifndef TASK_SEVER_IMP_H_293874
#define TASK_SEVER_IMP_H_293874

#include "../inc/taskSeverInterface.h"
#include "Src/Tools/BaseThread/SmartLock.h"

class CTaskSeverImp:public CTaskSeverInc
{
public:
	CTaskSeverImp(CDaoFactory  *pObj);
	virtual BOOL InsertTask( CTaskObj &obj );
	virtual BOOL UpdateTask( CTaskObj &obj );
	virtual BOOL UpdateFactStartTime(const CTaskObj &obj ); //更新实际工期等内容
	virtual BOOL UpdateTaskFather( int taskId, int fatherId );

	virtual BOOL GetTaskByTaskId( int taskId, CTaskObj &obj );
	virtual BOOL GetTaskScheudle( int taskId, ListSchedule &obj);
	virtual BOOL GetTaskByStatus( int ownerId, int st, ListTask &list, int year = 0, int quarter = 0 );
	virtual BOOL GetNewingTaskByCreateId( int creatId, ListTask &list ); //
	virtual BOOL GetOnGoingTaskByPrjId( int prjId, ListTask &list ); //
	virtual BOOL GetTaskByPrjId( int prjId, ListTask &task );
	//
	virtual BOOL GetUserHandleTask( int useId, int year, int quater, ListTask &list ); //获取用户负责任务
	virtual BOOL GetTaskByOwnerId( int useId, ListTask &lst );
	virtual BOOL GetUserReviewTask( int reviewId, ListTask &list  );//获取用户负责审核的任务,且状态为等待审核
	virtual BOOL GetPerformProjectTask( int year, int quater, int owerId, ListTask &listObj );
	virtual BOOL GetPerformDepartTask( int year, int quater, int owerId, ListTask &listObj );
	//
	virtual BOOL GetUserRemarkOnTask( int reviewId, MapTaskSchedule &mapObj  );//获取用户负责的主管进度审核任务
	virtual BOOL GetUserPrjRemarkOnTask(  int prjId, MapTaskSchedule &mapObj ); //获取用户负责的项目的进度审核
	virtual BOOL GetScheduleContentByDate(int ownerId, const COleDateTime &beginTime, const COleDateTime &endTime, ListSchedule &obj);
	//
	virtual BOOL GetChangeFirstReviewTask( int reviewId, ListChange &list ); //获取用户负责的一级变更审核
	virtual BOOL GetChangeSecondReviewTask( int gerneralId, ListChange &list );//获取用户负责的二级变更审核
    //删
	virtual BOOL DeleteTaskById( int taskId ) ;
	//动作
	virtual BOOL TaskReviewAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &obj );
	virtual BOOL TaskFinishWaitScore( CTaskObj &obj );
	virtual BOOL TaskScoreAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &obj);
	virtual BOOL TaskReviewFinishAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &obj );
	//
	virtual BOOL UpdateOwernTask();
	virtual BOOL UpdateReviewTask();
	virtual BOOL UpdateAllChange();
	virtual BOOL UpdateScoreTask();
	virtual void SetOwernTask( const ListTask &lst );
	virtual void SetReviewTask( const ListTask &lst  );
	virtual void SetScoreTask( const ListTask &lst  );
	virtual void SetAllChange( const ListChange &lst ) ;

	//virtual BOOL UpdatePrjReviewSchedule();
	virtual const TaskIdStringMap & GetWorkHourString();
	virtual const ListTask& GetOwernTask()  { SmartCSLock lock( &m_ownerTaskCS ); return m_ownerTaskList; }
	virtual const ListTask& GetReviewTask()  { SmartCSLock lock( &m_reviewTaskCS ); return m_reviewTaskList; };
	virtual const ListChange& GetReviewChange()  { SmartCSLock lock( &m_reviewChangeCS ); return m_reviewChange; };
	virtual const ListTask&   GetScoreTask()  { SmartCSLock lock( &m_scoreTaskCS ); return m_scoreTaskList;};
	virtual const CTaskStaus&      GetTaskStausMap() const { return m_stausMap;};
	virtual size_t                 GetOwneChangeCount() const { return m_ownChangeCount;}
	virtual void                   SetLoginUser( const CUser &obj );
	virtual void                   UpdateLoginUser();
	virtual const CUser&            GetLoginUser() const{return m_loginUser;};

	virtual BOOL GetMyChange( int userId, ListChange &list );

	virtual const CWorkTimeType& GetWorkTypeObj() const{ return m_workHoureObj;}
protected:
	
	BOOL          UpdateReviewChange();
	BOOL          UpdateGernalChange();
	BOOL          UpdatePrjReviewSchedule();
	BOOL          LoadTaskByUser();
	void          InsetMapSchudle(int taskId , ListSchedule &obj, MapTaskSchedule &mapObj );
protected:
	
	CUser        m_loginUser;
	RolePowerList m_loginPower;

	CTaskDaoInc  *m_pTaskDbDAo;
	CChangeDaoInc *m_pChangeDbDao;
    CWorkTimeType     m_workHoureObj;
	//
	int              m_newingTaskCount; //m_ownerTaskList中新建中的任务个数
	int              m_goingTaskCount; //m_ownerTaskList中正在进行中的任务
	int              m_waitStartTaskCount; //m_ownerTaskList中正在进行中的任务
	
	ListTask         m_ownerTaskList; //我需要处理的任务，我新建中的任务，我负责的进度中任务， 我负责的未启动任务
	ListTask         m_reviewTaskList; //负责评审的任务列表
	ListTask         m_scoreTaskList;  //负责评分的任务列表
	ListChange       m_reviewChange;       //一级评审
	size_t           m_ownChangeCount;
//	ListChange       m_generalChange;      //二级评审
//    MapTaskSchedule  m_remarkSchedule;    //负责进度点评
	CTaskStaus       m_stausMap;  //任务状态字符串映射
	//MapTaskSchedule  m_prjRemarkSchedule; //负责的进度项目点评
	CCriticalSection	       m_ownerTaskCS;
	CCriticalSection	       m_reviewTaskCS;
	CCriticalSection	       m_scoreTaskCS;
    CCriticalSection	       m_reviewChangeCS;
	CCriticalSection	       m_remarkScheduleCS;
};

#endif