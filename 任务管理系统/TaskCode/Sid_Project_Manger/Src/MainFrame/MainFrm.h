
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "./Output/OutputWnd.h"
#include "../action/factory/actionServerFactory.h"
#include "../action/factory/actionWatchServerFactory.h"
#include "./infomsg/PopProgressMsgTheard.h"  // 
#include "./WatchThread/WatchTaskThread.h"

#define BOTTOM_VIEW_HEIGHT	250

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// 操作
public:
	CActionServerFactory* GetSeverFactory() { return &m_actionFactory;};
	void	 LoginLoading();
	BOOL     ReplaceView(CRuntimeClass *pViewClass);
	void     ShowProgressMsg( int msgId, CString ext = _T("") );
	void     CloseMsgDlg();
	afx_msg LRESULT OnUpdateTabName( WPARAM , LPARAM );
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd        m_wndOutput;

// 生成的消息映射函数
protected:
	afx_msg void OnChangePrjMemger();
	afx_msg void OnChangeMyTaskView();
	afx_msg void OnChangeDepartView();
	afx_msg void OnChangeBackMangerView();
	afx_msg void OnChangePwd();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnFillTaskOutput( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnAddCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnContentChangeUpdate( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnSelGridLineView( WPARAM wp, LPARAM lp );
	afx_msg LRESULT CMainFrame::OnCheckWorkHour( WPARAM wp, LPARAM lp );
	afx_msg LRESULT OnCloseApp( WPARAM , LPARAM );
	//afx_msg LRESULT OnUpdateTabName( WPARAM , LPARAM );
	DECLARE_MESSAGE_MAP()

	BOOL          CreateDockingWindows();
	void          SetDockingWindowIcons(BOOL bHiColorIcons);
	void          ShowMenu();
	void          DestroyAllWindow( CWnd *pWnd );
	void          ExitApp();
	void          OnShowInfoView();
	void          ShowOutput( BOOL isShow );
	CWnd*         OnSvOutputMsgComon( int taskType );
protected:
	CActionServerFactory      m_actionFactory;
	CActionWatchServerFactory m_watchFactory;
	CProgressMsgThread        m_progressThread;
	CWatchTaskThread          m_watchTaskThread;
	bool     m_isLogin; // false; 
private:
	CRichEditCtrl m_richEditTmp; //为网格创建不显示的编辑框	 
public:
	afx_msg void OnDestroy();
};

