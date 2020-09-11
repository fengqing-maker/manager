#ifndef BASE_GRID_MAGER_AW34I783
#define BASE_GRID_MAGER_AW34I783

#include "../ui/CGridCtrl/comondef/comondef.h"
#include "../ui/CGridCtrl/AbstractGridViewManager.h"
#include "Src/action/factory/actionServerFactory.h"


//typedef std::map<int, int>  IntToIntMap;
#define IDM_BASE_ID               10
#define IDM_DELETE_TASK                   (IDM_BASE_ID+1)         //删除任务
#define IDM_CHANGE_TASK                   (IDM_BASE_ID+2)         //任务变更
#define IDM_PROJECT_MODIY_OWENR           (IDM_BASE_ID+3)         //项目修改负责人
#define IDM_PROJECT_MODIY_MEMBER          (IDM_BASE_ID+4)         //项目修改成员
#define IDM_PROJECT_DEL_PROJECT           (IDM_BASE_ID+5)         //删除项目
#define IDM_DEPART_MODIY_MANGER           (IDM_BASE_ID+6)         //部门修改负责人
#define IDM_DEPART_MODIY_BAKMNAGER        (IDM_BASE_ID+7)         //部门修改备用负责人
#define IDM_DEL_DEPART                    (IDM_BASE_ID+8)         //删除项目
#define IDM_TASK_DEPART_REVIEW            (IDM_BASE_ID+9)         //任务部门经理点评
#define IDM_TASK_PROJECT_REVIEW           (IDM_BASE_ID+10)         //任务项目经理点评
#define IDM_TASK_MOIDY_SCORE              (IDM_BASE_ID+11)         //修改评分

#define GRID_MANGER_MSG_BASE              (WM_USER + 500)
#define PRJ_MANGER_MSG_BASE               (WM_USER + 700)
#define DEPART_MANGER_MSG_BASE            (WM_USER + 800)
//
#define GRID_MYTASK_UPDATE_DATE           (GRID_MANGER_MSG_BASE+0) //更新 可填写进度日期列表 
#define GRID_UPDATE_WORKHOUR              (GRID_MANGER_MSG_BASE+1) //更新 工时表显示内容

//
#define PRJ_MANGER_MSG_CHANGE             (PRJ_MANGER_MSG_BASE+1) //切换到任务视图，wpara将PrjId传递给任务界面
#define PRJ_VIEW_MSG_RECODE_PRJID         ( PRJ_MANGER_MSG_BASE+2) //记录上一次选择的项目Id
#define PRJ_VIEW_MSG_RECODE_TASK_INDEX    ( PRJ_MANGER_MSG_BASE+3) //记录上一次选择任务状态的选项列表第几get
#define PRJ_VIEW_MSG_CHANGE_TASKLIST      ( PRJ_MANGER_MSG_BASE+4) //任务列表修改消息

//
#define DEPART_MANGER_MSG_CHANGE          (DEPART_MANGER_MSG_BASE+0) //部门切换到任务视图

#define SHOW_EXCEPT_COLOR                  50  //进度大于50 显示颜色

class CBaseGridManger:public AbstractGridViewManager
{
public:
	CBaseGridManger(  CActionServerFactory *pSeverFactory );

    virtual void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
	static  void GetHistoryString( CTaskSeverInc *pSever, int taskId, CString &str );
	static  void GetHistoryString( CTaskSeverInc *pSever, int taskId, std::list<CString> &strLst );
protected:
	//AbstractGridViewManager重写
	virtual  void        PreCreateGridView(); //创建网格前
	virtual  void        AftCreateGridView(); //创建网格后
	virtual  void        AftUpdateGridView();
	virtual CString      GetFixedItemText(int nRow, int nColumn); //填充固定列头内容
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
    //
	virtual int  GetFixedColumnCount(){return RowIndex_Title;} //返回0，就无ABCd列头
	virtual int	 GetFixedRowCount(){ return 1+RowIndex_Max; };	/* 增加一行标题 */
	virtual int  GetFlexColumnCount();
	//CBaseGridManger的虚函数
	virtual void CreateColumnMap();
	virtual void CreateRowItemMap(){};


	CString GetWorkHourStringById( int id );
protected:
    virtual int GetFlexRowHeight(int rowIndex){ return 45;}

	//共用操作
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
	void InsertTaskOwernColum();//责任人
	void InsertDiffParaColum();  //难度系数
	void InsertProjectTypeColum(); //项目类型
	void InsertWorkTimeTypeColum(); //工时类型
	void InsertSuggestionColumn(); //审判意见
	void InsertAgreeBtnColum();//同意按键
	void InsertUnAgreeBtnColum(); //不同意按键
	void InsertTaskPrjStep(); //任务所属项目阶段
	//进度评审
	void InsertReviewUpateColum(); //评审的进度
	void InsertProjectReview(); //项目点评
	void InsertScoreColumn(); //进度评分
	void InsertDepartReview(); //评分说明
	
	//任务完成
	void InsertForcastScore();//系统预测分
	void InsertUnUpdateCnt(); //漏写进度次数

	//任务变更
	void InsertChangeStatus(); //变更状态
	void InsertChangeTime(); //变更时间
	void InsertChangeReson(); //变更原因
	void InsertChangeContent(); //变更内容

	void InsertDatePercent();
	void InsertSchduleDate();

	void  InsertUniqueColumn(GridColumnPtr columnInfo);
	void  InsertUpDownColum(); //提交列
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
	/** @brief 表格<b>列</b>定义map */
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