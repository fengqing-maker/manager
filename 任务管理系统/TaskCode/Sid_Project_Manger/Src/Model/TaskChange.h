#ifndef TASK_CHANGE_HEARD2938_374
#define TASK_CHANGE_HEARD2938_374
#include "TypeDefHead.h"
enum changType
{
	changeTime = 0, //计划结束时间，以及计划工期，以及负责人
	changeSupsend, //任务状态：由进行中，变挂起/中止。 
	changeReStart, //重启挂起的任务，需要填写计划完成时间，追加工期？
	changeStop, //中止任务
	
};
enum ChangeResult
{
	changenewing = 0,
	prj_remark_on = 1, //2015-10-26 add待项目经理点评
	up_apper = 2, //已提交申请
	departAgree =3,
	departUnAgree = 4,
	genernalUnAgree,
	genernalAgree,
	
};
//变更任务的类型
enum ChangeTaskType
{
	changeNormalDepartTask, //变更部门任务
	changeNormalPrjTask, //变更普通项目任务
	changePrjPhase,      //变更项目阶段
};
struct ReviewContent 
{
	int          m_id;
	CString      m_reviewContent; //评审意见
	COleDateTime m_reviewTime;
	int          m_result;
	CString GetReiewTimeString() const
	{
		return m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S") );
	}
};
enum ChangeTaskOption
{
	schemeChange = 0,     //方案变更
	ralateTaskAffect = 1, //关联任务影响
	technoloageNeck= 2, //技术瓶颈
	timeNotEnough = 3, //时间不够，第0位为1则为选中
	priorityMoidy = 4, //优先级调整
	changeOwner = 5, //切换负责人，第1位为1则为选中反之未选中
	otherOption = 31,      //变更项目阶段
};
class CTaskChange
{
public:
	CTaskChange();
	CTaskChange(const CTaskChange &obj );
	void SetId( int id ){m_id = id;}
	int  GetId() const{return m_id;}

	void SetType( int id ){m_type = id;}
	int  GetType() const{return m_type;}

	void SetStatus( int id ){m_status = id;}
	int  GetStatus() const{ return m_status;}

	void SetTaskId( int id ){m_taskId = id;}
	int  GetTaskId() const{return m_taskId;}

	void SetBakTaskId( int id ){m_bakTaskId = id;}
	int  GetBakTaskId() const{return m_bakTaskId;}

	void SetChangeContent( const CString &str ){ m_changeContent = str;}
	const CString& GetChangeContent() const{return m_changeContent;}

	void SetChangeDateTime( const COleDateTime &time ){ m_applyForTime = time;}
	const COleDateTime& GetChangeDateTime() const{return m_applyForTime;}

	void SetChangeReason( const CString &str ){ m_changeReason = str;}
	const CString& GetChangeReason() const{return m_changeReason;}

	void SetReviewContent( const CString &str ){ m_reviewContent = str;}
	const CString& GetReviewContent() const{return m_reviewContent;}
	void SetAuditorId( int id ){m_auditorId = id;}
	int  GetAuditorId() const{return m_auditorId;}
	void SetReviewTime( const COleDateTime &time ){ m_reviewTime = time;}
	const COleDateTime& GetReviewTime() const{return m_reviewTime;}

	void SetGeneralReviewContent( const CString &str ){ m_generalReviewContent = str;}
	const CString& GetGeneralReviewContent() const{return m_generalReviewContent;}
	void SetGeneralAuditorId( int id ){m_generalId = id;}
	int  GetGeneralAuditorId() const{return m_generalId;}
	void SetGeneralReviewTime( const COleDateTime &time ){ m_generalReviewTime = time;}
	const COleDateTime& GetGeneralReviewTime() const{return m_generalReviewTime;}

	void           SetChangeDateTimeStr( const CString &time );
	CString        GetChangeDateTimeStr() const;
	void           SetReviewTimeStr( const CString &time );
	CString        GetReviewTimeStr() const;
	void           SetGeneralReviewTimeStr( const CString &time );
	CString        GetGeneralReviewTimeStr() const;

	void           SetImpactSchudle( BOOL id ){ m_isImpactSchdule = id;}
	BOOL           GetImpactSchudle() const{ return m_isImpactSchdule; }
	void           SetChangeTaskType( int id ){ m_taskType = id;}
	int            GetChangeTaskType() const{ return m_taskType; }
	void           SetProjectReview( const CString &str ) { m_projectReview = str;};
	CString        GetProjectReview() const { return m_projectReview;}
	void           SetChangeResonOption( int id ){ m_changeResonOption = id;}
	int            GetChangeResonOption() const{ return m_changeResonOption; }
	void           SetResonOptionSelectSt(int index, bool val); 
	bool           GetResonOptionSelectSt(int index ) const { return 1 == ((m_changeResonOption>>index) & 0x1) ;}
	
    CTaskChange&   operator=( const CTaskChange &obj );
    void           CopyObj( const CTaskChange &obj );
	bool           operator==( const CTaskChange &obj ) const;

	CString        GetChangeStatusString( int st, bool isShowReson = false ) const;
	CString        GetChangeTypeStr() const;
	CString        GetResonOptionString( int index ) const;
	CString        GetResonSelsOptionString() const;
    void           GetAllResonOptionMap( IntStringMap &mapStr) const;
	CString        GetChangeResultString() const;
	bool           IsFinishChange() const;

private:
	int     m_id;
	
	int     m_type; //变更类型
	CString m_changeContent;
	COleDateTime m_applyForTime; // 申请时间
	CString m_changeReason;
	int     m_taskId;
	int     m_bakTaskId; //变更前的备份任务id号

	CString m_reviewContent; //评审意见
	int     m_auditorId;     //评审人
	COleDateTime m_reviewTime;

	CString m_generalReviewContent; //评审意见
	int     m_generalId;     //副总评审人
	COleDateTime m_generalReviewTime; //评审时间

	int       m_status; //变更结果，1同意， 不同意
    //2015-10-26 add 修改变更流程
    BOOL      m_isImpactSchdule; //是否影响
    int       m_taskType;  //是否属于阶段变更
    CString   m_projectReview;  //项目点评
	int       m_changeResonOption; //变更原因选项
};
#endif