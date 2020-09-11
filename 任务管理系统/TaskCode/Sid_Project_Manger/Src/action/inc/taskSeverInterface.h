#ifndef TASK_SERVER_INTERFACE_H_345_87
#define TASK_SERVER_INTERFACE_H_345_87

#include "../../DataBase/factory/CDaoFactory.h"
#include "Src/Model/WorkTimeType.h"
#include <list>
#include <map>

typedef std::map< int , ListSchedule > MapTaskSchedule;
typedef MapTaskSchedule::iterator        MapTaskScheduleItor;
typedef MapTaskSchedule::const_iterator  MapTaskScheduleConstItor;


class CTaskSeverInc
{
public:
	CTaskSeverInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CTaskSeverInc(){}

	virtual BOOL InsertTask( CTaskObj &obj ) = 0; //�ɹ����ش���0���½������
	virtual BOOL UpdateTask( CTaskObj &obj ) = 0;
    virtual BOOL UpdateFactStartTime(const CTaskObj &obj ) = 0;
	virtual BOOL UpdateTaskFather( int taskId, int fatherId ) = 0;

	virtual BOOL GetTaskByTaskId( int taskId, CTaskObj &obj ) = 0;
    virtual BOOL GetTaskScheudle( int taskId, ListSchedule &obj) = 0;
	virtual BOOL GetTaskByStatus( int ownerId, int st, ListTask &list, int year = 0, int quarter = 0 ) = 0;
	virtual BOOL GetNewingTaskByCreateId( int creatId, ListTask &list ) = 0;
	virtual BOOL GetOnGoingTaskByPrjId( int prjId, ListTask &list ) = 0;
	virtual BOOL GetTaskByPrjId( int prjId, ListTask &task ) = 0;

	virtual BOOL GetUserHandleTask( int useId, int year, int quater, ListTask &list ) = 0; //��ȡ�û���������
	virtual BOOL GetTaskByOwnerId( int useId, ListTask &lst ) = 0;
    virtual BOOL GetUserReviewTask( int reviewId, ListTask &list  ) = 0;//��ȡ�û�������˵�����,��״̬Ϊ�ȴ����
	virtual void UpdateLoginUser() = 0;
	virtual BOOL GetPerformProjectTask( int year, int quater, int owerId, ListTask &listObj ) = 0;
	virtual BOOL GetPerformDepartTask( int year, int quater, int owerId, ListTask &listObj ) = 0;
	//����
	virtual BOOL GetUserRemarkOnTask( int reviewId, MapTaskSchedule &mapObj  ) = 0;//��ȡ�û���������ܽ����������
	virtual BOOL GetUserPrjRemarkOnTask(  int prjMangerId, MapTaskSchedule &mapObj ) = 0; //��ȡ�û��������Ŀ�Ľ������
	virtual BOOL   GetScheduleContentByDate(int ownerId, const COleDateTime &beginTime, const COleDateTime &endTime, ListSchedule &obj) = 0;
	//���
	virtual BOOL GetChangeFirstReviewTask( int reviewId, ListChange &list ) = 0; //��ȡ�û������һ��������
	virtual BOOL GetChangeSecondReviewTask( int gerneralId, ListChange &list ) = 0;//��ȡ�û�����Ķ���������
	//����
	virtual BOOL TaskReviewAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &obj ) = 0;
	virtual BOOL TaskFinishWaitScore( CTaskObj &obj ) = 0;
	virtual BOOL TaskScoreAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &obj) = 0;
	virtual BOOL TaskReviewFinishAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &obj ) = 0;
	//ɾ
	virtual BOOL DeleteTaskById( int taskId ) = 0;

    //
	virtual void SetLoginUser( const CUser &obj ) = 0;
	virtual BOOL UpdateOwernTask() = 0;
	virtual BOOL UpdateReviewTask() = 0;
	virtual BOOL UpdateScoreTask() = 0;
	virtual BOOL UpdateAllChange() = 0;
	virtual void SetOwernTask( const ListTask &lst ) = 0;
	virtual void SetReviewTask( const ListTask &lst  ) = 0;
	virtual void SetScoreTask( const ListTask &lst  ) = 0;
	virtual void SetAllChange( const ListChange &lst ) = 0;
	
	virtual const ListTask& GetOwernTask()  = 0;
	virtual const ListTask& GetReviewTask()  = 0;
	virtual const ListChange& GetReviewChange()  = 0;
	virtual const ListTask&   GetScoreTask()  = 0;
	virtual const CTaskStaus& GetTaskStausMap() const = 0;
	virtual const TaskIdStringMap &   GetWorkHourString() = 0;
	virtual size_t               GetOwneChangeCount() const = 0;
	virtual const CWorkTimeType& GetWorkTypeObj() const = 0; 
    virtual const CUser&         GetLoginUser() const = 0;
protected:
	CDaoFactory  *m_pDbDao;

private:
};

#endif