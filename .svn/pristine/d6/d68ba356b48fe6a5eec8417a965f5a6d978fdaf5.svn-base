#ifndef PUBLISH_TASK_MANGER
#define PUBLISH_TASK_MANGER

/**
*@file	myTaskGirdManager
*@brief	项目管理的任务页面
*@note	
*		
*@author	lihuizhen
*/
#include "./ProjectMangerBase.h"

struct TaskChildInfo
{
   BOOL hasChild;
   bool showChild;
   bool isShow; //是否显示
   int  showIndex;//显示的数字
   int  fatherId;
   int  isLoadChild; //是否加载过孩子列表进入任务队列
   TaskChildInfo()
   {
	   hasChild = false;
	   showChild = false;
	   isShow = true;
	   showIndex = 0;
       fatherId = -1;
       isLoadChild = false;
   }
};
typedef std::map<int, TaskChildInfo> ShowInfoMap;
class CProjectTaskManger: public CPrjBaseGridManger
{
public:
	//
	CProjectTaskManger( CActionServerFactory *pSeverFactory );
	void      SetDefProject(  int prjId, int taskST ); 
	void      SetSelProject( int prjId, int taskST, int userId, int diffPer, BOOL onlyShowQuality = false );
	void      SetFirstShowTaskStatus( int st ){ m_showStatus = st;};
	void      ImportProjectTask( const CProject &prj );
	void      ExportProjectTask( const CProject &prj );

    void      SetTaskStatus( int st );
	//消息处理
	void OnGridClick( NMHDR* pNMHDR,LRESULT* pResult );
    void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridProjectReview();
	ListTask*  GetTaskListPoint() {return &m_taskList;}  
protected:
	 //重写
	 virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	 virtual CString GetFixedItemText(int nRow, int nColumn);
	 virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	 virtual BOOL    GetFlexItemEditable(int nRow,int nColumn){ return FALSE;};

	 virtual void CreateColumnMap(); //创建列头
	 virtual void CreateRowItemMap();
	 //行列设置
	 virtual int     GetFlexRowCount();

	
protected:
	CTaskObj*        GetTaskByRow( int nRow );
	void             RefreshShowMap(   );
	void             UpdateDbData();
	BOOL             AddShowChild( ShowInfoMap::iterator &fahterItor, BOOL updateGrid = TRUE  );
    BOOL             ShowHideChild( ShowInfoMap::iterator &fahterItor, BOOL updateGrid = TRUE );
    void             GetTaskShowNum( int taskId, CString &text );
	//展开/收缩一个任务的所有子任务
	void             ExtentShowAllChild( int taskId, BOOL isShow, BOOL updateGrid = TRUE );
	void             ResetChildInfo( int beginRow, int childCount );
protected:
	 ShowInfoMap        m_childInfo;
	 ListTask           m_taskList; //所有要处理的任务
	 IntToIntMap       m_showTaskMap;
	 int               m_showPrj;
	 int               m_showStatus;
	 int               m_diffPer;
	 int               m_selUserId; //0,表示所有员工
	 BOOL              m_isOnlyShow; //只显示质量问题任务

	 COLORREF          m_fatherColor;
	 bool              m_extentAllTask; //展开所有任务
	 int               m_hasChildCount;
	 BOOL              m_hasReviewPower;
};

#endif