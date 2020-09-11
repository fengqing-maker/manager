//#ifndef  CEXCEL_TASK_H_2345
//#define  CEXCEL_TASK_H_2345
#pragma  once
#include "../Schedule.h"
#include "../TaskModel.h"
class CTaskObj;
class CExcelTaskModel
{
public:
	CExcelTaskModel();
	void                 SetNum( const int &id ){ m_num = id; };
	const int            GetNum() const{return m_num;}       
	void                 SetFatherNum( const int &id ){ m_fatherNum = id; };
	const int            GetFatherNum() const{return m_fatherNum;}       
	void                 SetTaskStatus( const CString &str ){ m_status = str; };
	const CString &      GetTaskStatus() const{return m_status;}       
	void                 SetOwnerName( const CString &str ){ m_ownerName = str; };
	const CString &      GetOwnerName() const{return m_ownerName;}       
	void                 SetWorkHourTypeStr( const CString &str ){ m_workHourTypeStr = str; };
	const CString &      GetWorkHourTypeStr() const{return m_workHourTypeStr;}      

    void                 SetRelateTaskId( const int &id ){ m_relatedTaskId = id; };
	const int            GetRelateTaskId() const{return m_relatedTaskId;}       
 //   void                 SetFactFatherId( const int &id ){ m_factFatherId = id; };
	//const int            GetFactFatherId() const{return m_factFatherId;}       
	
	//任务
	void                 SetTaskName( const CString &str ){ if ( m_taskName != str ){m_taskName = str;m_isModify = TRUE;}}
	const CString&       GetTaskName() const{return m_taskName;}
	void                 SetScore( int id ){ if ( m_taskScore!= id){m_taskScore = id; m_isModify = TRUE;}}
	int                  GetScore() const{return m_taskScore;}
	void                 SetDifficultyParam( double id ){ if (m_difficultyParm != id ){m_difficultyParm = id;m_isModify = TRUE;}}
	double                GetDifficultyParam() const{return m_difficultyParm;}
	void                 SetPlanStartTime( const COleDateTime &time );
	const COleDateTime   GetPlanStartTime() const;
	void                 SetPlanEndTime( const COleDateTime &time );
	const COleDateTime   GetPlanEndTime() const;
	void                 SetPlanWorkHour( double id ){if (m_planWorkHour!=id ){m_planWorkHour = id; m_isModify = TRUE;}}
	double                GetPlanWorkHour() const{return m_planWorkHour;}
	void                 SetFactStartTime( const COleDateTime &time ){if (m_factStartTime != time){m_factStartTime = time; m_isModify = TRUE;}}
	const COleDateTime&  GetFactStartTime() const{return m_factStartTime;}
	void                 SetFactEndTime( const COleDateTime &time ){ if (m_factEndTime!=time){m_factEndTime = time; m_isModify = TRUE;}}
	const COleDateTime&  GetFactEndTime() const{return m_factEndTime;}
	void                 SetFactWorkHour( double id ){ if ( m_factWorkHour != id){m_factWorkHour = id; m_isModify = TRUE;}}
	double                GetFactWorkHour() const{return m_factWorkHour;}
	void                 SetDec( const CString &str ){ if ( m_dec != str ){m_dec = str;m_isModify = TRUE;}}
	const CString&       GetDec() const{return m_dec;}

	void      SetPanStartDateStr(const CString timeStr);
	void      SetPanEndDateStr( const CString timeStr);
	void      SetFactStartDateStr( const CString timeStr);
	void      SetFactEndDateStr( const CString timeStr );
	//不带时分秒的时间字符串
	CString   GetPanStartDateStr() const;
	CString   GetPanEndDateStr() const;
	CString   GetFactStartDateStr() const;
	CString   GetFactEndDateStr() const;
	CString   GetUpdateTimeStr() const;
	//进度
	void                 SetUpdateContent( const CString &str ){ if ( m_strdec != str){m_strdec = str;m_isModify = TRUE;}}
	const  CString&      GetUpdateContent() const{return m_strdec;}
	void                 SetWorkHours( double para ){ if ( m_workHours != para)
	                     {m_workHours = para;m_isModify = TRUE;}}
	double                GetWorkHours() const{return m_workHours;}
	void                 SetPercent( int id );
	int                  GetPercent() const{return m_percent;}
	void                 SetUpdateTime( const COleDateTime &time ){if ( time != m_updateTime ){m_updateTime = time;m_isModify = TRUE;}}
	const                COleDateTime& GetUpdateTime() const{return m_updateTime;}

	void                 FillContentFromTask( const CTaskObj &obj );
    void                 FillContentFromSchedul( const CSchedule &obj );

	void                 SetTaskTypeString( const CString &str ){ m_taskTypeString = str;}
	const CString&       GetTaskTypeString() const{ return m_taskTypeString;}

	void                 SetTaskAtrrbuteString( const CString &str ){ m_taskAtrrString = str;}
	const CString&       GetTaskAtrrbuteString() const{ return m_taskAtrrString;}

	
	CExcelTaskModel( const CExcelTaskModel& obj );
	void CopyObj( const CExcelTaskModel& obj );
    CExcelTaskModel& operator=( const CExcelTaskModel &obj );
private:
   int             m_num;
   int             m_fatherNum;
   CString         m_status;
   CString         m_ownerName;
   CString         m_workHourTypeStr;
   int             m_relatedTaskId;
   int             m_factFatherId;
   CString         m_taskTypeString;
   CString         m_taskAtrrString;
   //任务
   CString         m_taskName;
   int             m_taskScore;     //任务总分
   double           m_difficultyParm; //任务难度系数，评审人同样发布后必填的值
   COleDateTime    m_planStartTime; //计划开启时间
   COleDateTime    m_planEndTime;   //计划结束时间
   double           m_planWorkHour;  //计划工时
   COleDateTime    m_factStartTime;
   COleDateTime    m_factEndTime;
   double           m_factWorkHour;
   CString         m_dec;           //任务备注
   //进度相关
   double       m_workHours; //工时
   int         m_percent; //完成百分比
   CString     m_strdec; //进度描述
   COleDateTime m_updateTime;

   BOOL           m_isModify;    //是否修改过对象
};


//#endif