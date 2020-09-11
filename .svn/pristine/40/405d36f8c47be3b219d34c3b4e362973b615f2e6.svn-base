#pragma once


// CTaskTabView 视图
#include <map>
#include "../../../action/factory/actionServerFactory.h"
#include "myTaskView.h"
#include "publishTaskView.h"
#include "scoreTaskView.h"
//#include "scheduleReviewView.h"
#include "changTaskView.h"
#include "WorkHourView.h"
#include "ForecastWorkHourView.h"

class CTaskTabView : public CTabView
{
	DECLARE_DYNCREATE(CTaskTabView)

protected:
	CTaskTabView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTaskTabView();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	BOOL         SetActiveViewByTabFlg( int flg );
	int          GetActiveViewTabFlg( );

public:
		/** @brief 选项卡序号 */
	//部门经理，总经理，主管
	enum Tab_Index{ MyTask, TaskPublishReview, taskScoreReview, ScheduleReview, ChangeReview, TabWorkHourView, TabForecastView };
	/**
	*@brief	更新Tab标签文本内容
	*@param [in]	index	需要更新的标签序号 必须 >= 0 
	*@param [in]	cnt	    需要处理的条目数
	*@note 	更新指定标签。
	*@see 	Tab_Index
	*/
	void UpdateTabLabel(Tab_Index index, int cnt );

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	//virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateTabName( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnUpdateWorkHourTab( WPARAM wp, LPARAM lp );
	DECLARE_MESSAGE_MAP()
private:
		CString GetTabString( Tab_Index index, int cnt ) const;
		int     OnCreateTab();
private:
	TabIndxeMap                      m_tabIndex;
	CActionServerFactory*            m_pSever;
	std::unique_ptr<CMyTaskView>      m_myTaskView;
	std::unique_ptr<CPublishTaskView> m_publishTaskView;
	std::unique_ptr<CScoreTaskView>   m_scoreTaskView;
	//std::unique_ptr<CScheduleReviewView>   m_scheduleView;
	std::unique_ptr<CChangeTaskView>   m_changeTaskView;
	std::unique_ptr<CWorkHourView>     m_workHourView;
//	std::unique_ptr<CForecastWorkHourView>     m_forecastWorkHourView;
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};


