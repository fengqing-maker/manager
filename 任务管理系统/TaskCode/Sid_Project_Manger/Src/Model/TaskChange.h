#ifndef TASK_CHANGE_HEARD2938_374
#define TASK_CHANGE_HEARD2938_374
#include "TypeDefHead.h"
enum changType
{
	changeTime = 0, //�ƻ�����ʱ�䣬�Լ��ƻ����ڣ��Լ�������
	changeSupsend, //����״̬���ɽ����У������/��ֹ�� 
	changeReStart, //���������������Ҫ��д�ƻ����ʱ�䣬׷�ӹ��ڣ�
	changeStop, //��ֹ����
	
};
enum ChangeResult
{
	changenewing = 0,
	prj_remark_on = 1, //2015-10-26 add����Ŀ�������
	up_apper = 2, //���ύ����
	departAgree =3,
	departUnAgree = 4,
	genernalUnAgree,
	genernalAgree,
	
};
//������������
enum ChangeTaskType
{
	changeNormalDepartTask, //�����������
	changeNormalPrjTask, //�����ͨ��Ŀ����
	changePrjPhase,      //�����Ŀ�׶�
};
struct ReviewContent 
{
	int          m_id;
	CString      m_reviewContent; //�������
	COleDateTime m_reviewTime;
	int          m_result;
	CString GetReiewTimeString() const
	{
		return m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S") );
	}
};
enum ChangeTaskOption
{
	schemeChange = 0,     //�������
	ralateTaskAffect = 1, //��������Ӱ��
	technoloageNeck= 2, //����ƿ��
	timeNotEnough = 3, //ʱ�䲻������0λΪ1��Ϊѡ��
	priorityMoidy = 4, //���ȼ�����
	changeOwner = 5, //�л������ˣ���1λΪ1��Ϊѡ�з�֮δѡ��
	otherOption = 31,      //�����Ŀ�׶�
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
	
	int     m_type; //�������
	CString m_changeContent;
	COleDateTime m_applyForTime; // ����ʱ��
	CString m_changeReason;
	int     m_taskId;
	int     m_bakTaskId; //���ǰ�ı�������id��

	CString m_reviewContent; //�������
	int     m_auditorId;     //������
	COleDateTime m_reviewTime;

	CString m_generalReviewContent; //�������
	int     m_generalId;     //����������
	COleDateTime m_generalReviewTime; //����ʱ��

	int       m_status; //��������1ͬ�⣬ ��ͬ��
    //2015-10-26 add �޸ı������
    BOOL      m_isImpactSchdule; //�Ƿ�Ӱ��
    int       m_taskType;  //�Ƿ����ڽ׶α��
    CString   m_projectReview;  //��Ŀ����
	int       m_changeResonOption; //���ԭ��ѡ��
};
#endif