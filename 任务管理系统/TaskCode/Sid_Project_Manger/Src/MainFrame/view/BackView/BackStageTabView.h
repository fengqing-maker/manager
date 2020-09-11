#pragma once


// CDepartTabView 视图
#include <map>
#include "../../../action/factory/actionServerFactory.h"


class CBackStageTabView : public CTabView
{
	DECLARE_DYNCREATE(CBackStageTabView)

protected:
	CBackStageTabView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBackStageTabView();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
		/** @brief 选项卡序号 */
	//部门经理，总经理，主管
	enum Tab_Index{ userMangerTabInex, productLineTabIndex, rolePowerTabIndex, workTimeType };
	/**
	*@brief	激活指定Tab页面
	*@param [in]	index	指定激活的页面号
	*@see 	Tab_Index
	*/
	void ActiveTab(int index);
	/**
	*@brief	更新Tab标签文本内容
	*@param [in]	index	需要更新的标签序号
	*@note 	标签序号使用默认值-1则全部标签被更新，否则仅更新指定标签。
	*@see 	Tab_Index
	*/
	void UpdateTabLabel(Tab_Index index);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	//virtual int OnCreate(LPCREATESTRUCT lpCreateStruct)
	afx_msg void    OnSize(UINT nType, int cx, int cy);
	//afx_msg LRESULT OnRecordShowPrjIs( WPARAM wPara, LPARAM lPara );
	//afx_msg LRESULT OnRecordTaskStSelIndex( WPARAM wPara, LPARAM lPara );

	DECLARE_MESSAGE_MAP()
private:
		CString GetTabString( Tab_Index index ) const;
		int     OnCreateTab();
private:
	TabIndxeMap                      m_tabIndex;
	CActionServerFactory*            m_pSever;
	int             m_selPrjId;
	int             m_selTaskStatusIndex; //选项表中选了第几个
	//std::unique_ptr<CMyTaskView>      m_myTaskView;

};


