#ifndef SCHEDULE_HEARD_3455_H
#define SCHEDULE_HEARD_3455_H
/*
  进度更新表
*/
enum ReviewType
{
	notStartReview = 0, //未评审
	review = 1,//已评审
	skipReview = 2,//跳过不进行评审 
};
class CSchedule
{
public:
	CSchedule();
    CSchedule(const CSchedule &obj);
	void ReInit(); // 重置对象

	void SetId( int id ){ if ( m_id != id){ m_id = id; m_isChange = TRUE;} }
	int  GetId() const{return m_id;}

	void SetTaskId( int id ){  if ( m_taskId != id ){ m_taskId = id; m_isChange = TRUE;}}
	int  GetTaskId() const{return m_taskId;}

	void SetUpdateContent( const CString &str ){ if ( m_strdec != str){m_strdec = str;m_isChange = TRUE;}}
	const CString& GetUpdateContent() const{return m_strdec;}

	//void SetScore( int para ){if ( m_score != para){m_score = para;m_isChange = TRUE;}}
	//int  GetScore() const{return m_score;}

	void SetWorkHours( double para ){ if ( m_workHours != para)
	{m_workHours = para;m_isChange = TRUE;}}
	double  GetWorkHours() const{return m_workHours;}


	bool SetPercent( int id );
	int  GetPercent() const{return m_percent;}

	void SetUpdateTime( const COleDateTime &time ){if ( time != m_updateTime ){m_updateTime = time;m_isChange = TRUE;}}
	const COleDateTime& GetUpdateTime() const{return m_updateTime;}

	void    SetUpdateTimeStr( const CString timeStr);
	CString GetUpdateTimeStr() const;

	void   SetWorkTypeId( int id ){if ( m_timeType != id ){m_timeType = id;}}
	int    GetWorkTypeId() const{return m_timeType;}
	//
	//void  SetSkipReview( BOOL para );
	//BOOL  GetSkipReview() const{return m_isSkipReview;}

	//void  SetSkipPrjReview( BOOL para );
	//BOOL  GetSkipPrjReview() const{return m_isSkipPrjReview;}

	void CopyObj( const CSchedule &obj );
	CSchedule & operator = (const CSchedule & pt); 
	bool        operator == (const CSchedule & pt) const;

	void   CleanModify(){ m_isChange = FALSE;}
	BOOL   GetIsModify(){ return m_isChange;}
private:
	int     m_id;
	int     m_taskId;

	CString     m_strdec; //进度描述
	//int         m_score;  //加减分
	double       m_workHours; //工时
	int         m_percent; //完成百分比
	COleDateTime m_updateTime; //更新时间

	//BOOL         m_isSkipReview; //无任何评价
	//BOOL         m_isSkipPrjReview; //跳过项目点屏
    BOOL         m_isChange; //是否改变
	int             m_timeType;       //工时类型
};
#endif