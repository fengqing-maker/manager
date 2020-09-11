//#ifndef TASK_MODEL_H_34532
//#define TASK_MODEL_H_34532
#pragma  once
#include "TaskStaus.h"
#include "excelObj/CExcelTaskModel.h"


#define TASKTYPE_PRJSTR     _T("��Ŀ����")
#define TASKTYPE_DEPARTSTR  _T("��������")

#define  NORMAL_TASK         0 //��������
#define  QUALITY_TASK        999999  //������������
class CExcelTaskModel;
class CTaskObj
{
public:
	CTaskObj();
	CTaskObj( const CTaskObj &obj);
	
	void InitObj();

	void SetId( int id ){ if ( m_id != id ){m_isModify = TRUE;m_id = id;}}
	int  GetId() const{return m_id;}
	void SetAttribute( int para ){ if (m_attr!= para){m_attr = para;m_isModify = TRUE;}}
	int  GetAttribute() const{return m_attr;}
	void SetTaskFlag( int para ){ if (m_taskFlag!= para){m_taskFlag = para;m_isModify = TRUE;}}
	int  GetTaskFlag() const{return m_taskFlag;}

	void SetTaskStatus( int para ){ if ( para != m_taskStatus){m_taskStatus = para;m_isModify = TRUE;}}
	int  GetTaskStatus() const{return m_taskStatus;}

	void          SetName( const CString &str ){ if ( m_name != str ){m_name = str;m_isModify = TRUE;}}
	const CString& GetName() const{return m_name;}
	void           SetDec( const CString &str ){ if ( m_dec != str ){m_dec = str;m_isModify = TRUE;}}
	const CString& GetDec() const{return m_dec;}
	void           SetType( const CString &str ){if ( str != m_type){m_type = str;m_isModify = TRUE;}}
	const CString& GetType() const{return m_type;}

	void   SetOwnerId( int para ){if ( para != m_ownerId ){m_ownerId = para;m_isModify = TRUE;}}
	int    GetOwnerId() const{return m_ownerId;}

	void   SetCreateId( int id ){if ( id != m_createId ){m_createId = id;m_isModify = TRUE;}}
	int    GetCreateId() const{return m_createId;}


	void   SetProjctId( int id ){if ( id != m_prjId){m_prjId = id;m_isModify = TRUE;}}
	int    GetProjctId() const{return m_prjId;}
	
	void   SetWorkTypeId( int id ){if ( m_timeType != id ){m_timeType = id; m_isModify = TRUE;}}
	int    GetWorkTypeId() const{return m_timeType;}
	
	void   SetDifficultyParam( double id ){ if (m_difficultyParm != id ){m_difficultyParm = id;m_isModify = TRUE;}}
	double  GetDifficultyParam() const{return m_difficultyParm;}

	void                SetCreateTime( const COleDateTime &time ){ if (m_createTime!= time){m_createTime = time;m_isModify = TRUE;}}
	const COleDateTime& GetCreateTime() const{return m_createTime;}
	void                SetPublishTime( const COleDateTime &time ){if ( m_taskPublishTime!= time){m_taskPublishTime = time;m_isModify = TRUE; }}
	const COleDateTime& GetPublishTime() const{return m_taskPublishTime;}

	void                      SetPlanStartTime( const COleDateTime &time );
	const COleDateTime        GetPlanStartTime() const;
	void                      SetPlanEndTime( const COleDateTime &time );
    const COleDateTime        GetPlanEndTime() const;
	void                      SetPlanWorkHour( double id ){if (m_planWorkHour!=id ){m_planWorkHour = id; m_isModify = TRUE;}}
	double                     GetPlanWorkHour() const{return m_planWorkHour;}
	void                      SetFactStartTime( const COleDateTime &time ){if (m_factStartTime != time){m_factStartTime = time; m_isModify = TRUE;}}
	const COleDateTime&       GetFactStartTime() const{return m_factStartTime;}
	void                      SetFactEndTime( const COleDateTime &time ){ if (m_factEndTime!=time){m_factEndTime = time; m_isModify = TRUE;}}
	const COleDateTime&       GetFactEndTime() const{return m_factEndTime;}
	void                      SetFactWorkHour( double id ){ if ( m_factWorkHour != id){m_factWorkHour = id; m_isModify = TRUE;}}
	double                     GetFactWorkHour() const{return m_factWorkHour;}

	void                 SetScore( int id ){ if ( m_taskScore!= id){m_taskScore = id; m_isModify = TRUE;}}
	int                  GetScore() const{return m_taskScore;}
	void                 SetForecastScore( int id ){ if ( m_forecastScore != id){m_forecastScore = id; m_isModify = TRUE;}}
	int                  GetForecastScore() const{return m_forecastScore;}
	void                 SetScoreDec( const CString &str ){if ( m_taskScoreDc != str ){m_taskScoreDc = str; m_isModify = TRUE;}}
	const CString&       GetScoreDec() const{return m_taskScoreDc;}
	void                 SetMakeScoreUserId( int id ){ if ( m_scoreUser != id ){m_scoreUser = id; m_isModify = TRUE;}}
	int                  GetMakeScoreUserId() const{return m_scoreUser;}
	void                 SetScoreTime( const COleDateTime &time ){ if (m_scoreTime != time){m_isModify = TRUE;m_scoreTime = time;	}}
	const COleDateTime&  GetScoreTime() const{return m_scoreTime;}

	//����ʱ�����ʱ���ַ���
	CString   GetPanStartDateStr() const;
	CString   GetPanEndDateStr() const;
	CString   GetFactStartDateStr() const;
	CString   GetFactEndDateStr() const;
	      //��ʾ�õ�
	CString   GetPanEndDateShowStr() const;
	CString   GetPanStartDateShowStr() const;
	CString   GetFactStartDateShowStr() const;
	CString   GetFactEndDateShowStr() const;
	//��ʱ�����ʱ���ַ���
	CString   GetCreateTimeStr() const;
	CString   GetPublishTimeStr() const;
	CString   GetScoreTimeStr() const;
	//����ʱ�����ʱ���ַ���
	void      SetPanStartDateStr(const CString timeStr);
	void      SetPanEndDateStr( const CString timeStr);
	void      SetFactStartDateStr( const CString timeStr);
	void      SetFactEndDateStr( const CString timeStr );
	//��ʱ�����ʱ���ַ���
	void      SetCreateTimeStr( const CString timeStr ) ;
	void      SetPublishTimeStr( const CString timeStr ) ;
	void      SetScoreTimeStr( const CString timeStr ) ;
	//��ӵ���
	void SetPrjReview( const CString &str ){ if ( m_strPrjReviw != str ){m_strPrjReviw = str;m_isModify = TRUE;}}
	const CString& GetPrjReview() const{return m_strPrjReviw;}
	void SetReview( const CString &str ){ if ( m_strReveiw != str){m_strReveiw = str; m_isModify = TRUE;}}
	const CString& GetReview() const{return m_strReveiw;}
	void                 SetCntLeakUpdate( int data ){ if ( m_cntLeakUpdate != data ){m_cntLeakUpdate = data; m_isModify = TRUE;}}
	int                  GetCntLeakUpdate() const{return m_cntLeakUpdate;}
	void                 SetCheckBeginDate( const COleDateTime& data );
	void                 SetCheckBeginDateStr( const CString &str );
	const CString       GetCheckBeginDateStr() const;
	const COleDateTime&  GetCheckBeginDate() const{return m_checkBeginDate;}
	int                  CaludateTimePercent( const COleDateTime &updateDate ) const;
	//
	void      CopyObj( const CTaskObj &obj );
	CTaskObj& operator=( const CTaskObj &obj );
    BOOL      operator==( const CTaskObj &obj ) const;

	//
	void   CleanModify(){ m_isModify = FALSE;}
	BOOL   GetIsModify(){ return m_isModify;}
	//
	void        FillObjFromExcel( const CExcelTaskModel &obj );
	CString     GetDiffContent( const CTaskObj &obj, const TaskIdStringMap &statusMap ) const;
	int         GetTaskPanDay() const; //����ƻ��������

protected:
private:
	int             m_id;
	int             m_attr;//0����������   1��������
	CString         m_name;
	CString         m_dec;
	CString         m_type; // ��Ŀ����
	int             m_ownerId;
	int             m_prjId;
	COleDateTime    m_createTime;      //����ʱ��
	COleDateTime    m_taskPublishTime; //���񷢲�ʱ��

	COleDateTime    m_planStartTime; //�ƻ�����ʱ��
	COleDateTime    m_planEndTime;   //�ƻ�����ʱ��
	double           m_planWorkHour;  //�ƻ���ʱ
	COleDateTime    m_factStartTime;
	COleDateTime    m_factEndTime;
	double           m_factWorkHour;
    
	int             m_taskScore;     //�����ܷ�
	int             m_forecastScore;  //ϵͳԤ��֣���100Ϊ��׼
	CString         m_taskScoreDc; //����˵��
	int             m_scoreUser;  //�����
	COleDateTime    m_scoreTime; //����ʱ��

	int             m_taskStatus;
	double           m_difficultyParm; //�����Ѷ�ϵ����������ͬ������������ֵ
	int             m_timeType;       //��ʱ����

	int            m_createId;    //������
	BOOL           m_isModify;    //�Ƿ��޸Ĺ�����

	CString     m_strPrjReviw;  //��Ŀ�����˵���
	CString     m_strReveiw; //���ܵ���
	int         m_cntLeakUpdate; //©д������ȴ���

    COleDateTime     m_checkBeginDate;  //����������
    static COleDateTime g_minCheckDate;

	int        m_taskFlag; //task_flag �����ʶ

};

//#endif