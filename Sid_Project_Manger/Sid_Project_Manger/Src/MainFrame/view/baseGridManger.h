#ifndef BASE_GRID_MAGER_AW34I783
#define BASE_GRID_MAGER_AW34I783

#include "../ui/CGridCtrl/comondef/comondef.h"
#include "../ui/CGridCtrl/AbstractGridViewManager.h"
#include "Src/action/factory/actionServerFactory.h"


//typedef std::map<int, int>  IntToIntMap;
#define IDM_BASE_ID               10
#define IDM_DELETE_TASK                   (IDM_BASE_ID+1)         //ɾ������
#define IDM_CHANGE_TASK                   (IDM_BASE_ID+2)         //������
#define IDM_PROJECT_MODIY_OWENR           (IDM_BASE_ID+3)         //��Ŀ�޸ĸ�����
#define IDM_PROJECT_MODIY_MEMBER          (IDM_BASE_ID+4)         //��Ŀ�޸ĳ�Ա
#define IDM_PROJECT_DEL_PROJECT           (IDM_BASE_ID+5)         //ɾ����Ŀ
#define IDM_DEPART_MODIY_MANGER           (IDM_BASE_ID+6)         //�����޸ĸ�����
#define IDM_DEPART_MODIY_BAKMNAGER        (IDM_BASE_ID+7)         //�����޸ı��ø�����
#define IDM_DEL_DEPART                    (IDM_BASE_ID+8)         //ɾ����Ŀ
#define IDM_TASK_DEPART_REVIEW            (IDM_BASE_ID+9)         //�����ž������
#define IDM_TASK_PROJECT_REVIEW           (IDM_BASE_ID+10)         //������Ŀ�������
#define IDM_TASK_MOIDY_SCORE              (IDM_BASE_ID+11)         //�޸�����

#define GRID_MANGER_MSG_BASE              (WM_USER + 500)
#define PRJ_MANGER_MSG_BASE               (WM_USER + 700)
#define DEPART_MANGER_MSG_BASE            (WM_USER + 800)
//
#define GRID_MYTASK_UPDATE_DATE           (GRID_MANGER_MSG_BASE+0) //���� ����д���������б� 
#define GRID_UPDATE_WORKHOUR              (GRID_MANGER_MSG_BASE+1) //���� ��ʱ����ʾ����

//
#define PRJ_MANGER_MSG_CHANGE             (PRJ_MANGER_MSG_BASE+1) //�л���������ͼ��wpara��PrjId���ݸ��������
#define PRJ_VIEW_MSG_RECODE_PRJID         ( PRJ_MANGER_MSG_BASE+2) //��¼��һ��ѡ�����ĿId
#define PRJ_VIEW_MSG_RECODE_TASK_INDEX    ( PRJ_MANGER_MSG_BASE+3) //��¼��һ��ѡ������״̬��ѡ���б�ڼ�get
#define PRJ_VIEW_MSG_CHANGE_TASKLIST      ( PRJ_MANGER_MSG_BASE+4) //�����б��޸���Ϣ

//
#define DEPART_MANGER_MSG_CHANGE          (DEPART_MANGER_MSG_BASE+0) //�����л���������ͼ

#define SHOW_EXCEPT_COLOR                  50  //���ȴ���50 ��ʾ��ɫ

class CBaseGridManger:public AbstractGridViewManager
{
public:
	CBaseGridManger(  CActionServerFactory *pSeverFactory );

    virtual void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	static  void GetHistoryString( CTaskSeverInc *pSever, int taskId, CString &str );
	static  void GetHistoryString( CTaskSeverInc *pSever, int taskId, std::list<CString> &strLst );
protected:
	//AbstractGridViewManager��д
	virtual  void        PreCreateGridView(); //��������ǰ
	virtual  void        AftCreateGridView(); //���������
	virtual  void        AftUpdateGridView();
	virtual CString      GetFixedItemText(int nRow, int nColumn); //���̶���ͷ����
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
    //
	virtual int  GetFixedColumnCount(){return RowIndex_Title;} //����0������ABCd��ͷ
	virtual int	 GetFixedRowCount(){ return 1+RowIndex_Max; };	/* ����һ�б��� */
	virtual int  GetFlexColumnCount();
	//CBaseGridManger���麯��
	virtual void CreateColumnMap();
	virtual void CreateRowItemMap(){};


	CString GetWorkHourStringById( int id );
protected:
    virtual int GetFlexRowHeight(int rowIndex){ return 45;}

	//���ò���
	void InsertTaskNameColumn();
	void InsertTaskStatusColumn();
	void InsertTaskPercentColumn();
	void InsertMaxPerceneColumn();
	void InsertTaskUpdateColumn();
	void InsertTaskHistoryUpdateColumn();
	void InsertTaskWorkHoursColumn();
	void InsertTaskPlanColumn();
	void InsertTaskFactTimeColumn();
	//
	void InsertTaskOwernColum();//������
	void InsertDiffParaColum();  //�Ѷ�ϵ��
	void InsertProjectTypeColum(); //��Ŀ����
	void InsertWorkTimeTypeColum(); //��ʱ����
	void InsertSuggestionColumn(); //�������
	void InsertAgreeBtnColum();//ͬ�ⰴ��
	void InsertUnAgreeBtnColum(); //��ͬ�ⰴ��
	void InsertTaskPrjStep(); //����������Ŀ�׶�
	//��������
	void InsertReviewUpateColum(); //����Ľ���
	void InsertProjectReview(); //��Ŀ����
	void InsertScoreColumn(); //��������
	void InsertDepartReview(); //����˵��
	
	//�������
	void InsertForcastScore();//ϵͳԤ���
	void InsertUnUpdateCnt(); //©д���ȴ���

	//������
	void InsertChangeStatus(); //���״̬
	void InsertChangeTime(); //���ʱ��
	void InsertChangeReson(); //���ԭ��
	void InsertChangeContent(); //�������

	void InsertDatePercent();
	void InsertSchduleDate();

	void  InsertUniqueColumn(GridColumnPtr columnInfo);
	void  InsertUpDownColum(); //�ύ��
	void  InsertColumn( const CString &str, int width, int alignemnt, int type, int dataType );
	void  InsertRichEditColumn( const CString &str, int width, int alignemnt, int dataType );
	void  InsertEditColumn( const CString &str, int width, int alignemnt, int dataType );
	void  InsertNumEditColumn( const CString &str, int width, int alignemnt, int dataType );
	void  InsertDateColumn( const CString &str, int width, int alignemnt, int dataType );
	void  InsertCheckColumn( const CString &str, int alignemnt, int dataType );

	//
	void    GetHistoryString( int taskId, CString &str ) const;
	CString GetScheduleString( const CSchedule &obj ) const;
	CString GetPanTimeString(const CTaskObj &obj );
	CString GetFactTimeString( const CTaskObj &obj);
	void    GetTaskStatusSting( const CTaskObj &obj, CString &text );
	CString GetProjectShortName( const CTaskObj &obj );
	CString GetOwnerNameStr( int userId );
	void    GetTaskBelongToPrjStep( int taskAttrbute, CString &str );

	void    SetBkColorByTask(int nRow, int nColumn, const CTaskObj &obj, int diffPer );

	CTaskObj* GetTaskByRow( int nRow, ListTask &taskList, const IntToIntMap &showMap );
	CString*  GetCStringByPos( int nRow, ListString &showView,  const IntToIntMap &showMap );
	int       FindTaskListById( int taskId, ListTask &lst, ListTaskItor &itor );
	int       FindTaskById( int taskId,  ListTask &lst, ListTaskItor &itor,const IntToIntMap &postMap );
	
protected:
	/** @brief ���<b>��</b>����map */
	GridColumnMap				m_gridColumnMap;
	CActionServerFactory        *m_pSeverFactory;
	CTaskSeverInc               *m_pTaskSever;

	COLORREF              m_newingColor;
	COLORREF              m_waitPublishColor;
	COLORREF              m_goingonColor;
	COLORREF              m_waitStartColor;
	COLORREF              m_finishColor;
	COLORREF              m_otherColor;
	COLORREF              m_diffColor;
	COLORREF              m_overdueColor;
	int                   m_outputShowId;
	enum TitleType{ TaskName_col, TaskStatus_col, TaskPrecent_col, TaskUpdatePrecent, TaskWorkHour,TaskUpcontent, TaskHistoryContent, 
		 TaskPlanTime, TaskFactTime, TaskOperate1, TaskOperate2, 
		 taskOwern, ProjectType_Colom, DiffPara_Colom, SuggestionColumn, TaskTimeType,
	     ReviewCurrtentUpdate,SheduleProjectReview,  ScoreColum,
	     ForcostScoreColum,SheduleDepartReview, leaveUpdateCnt, taskBelongPrjStep,
	     ChangeReviewUser1,ChangeReviewUser2,ChangeStatus,ChangeAppTime, ChangeReson, ChangeContent, PERCENT_DIFF,SCH_UPDATE_DATE };
private:
	enum RowIndex{RowIndex_Title = 1,RowIndex_Max = 1};
};
#endif