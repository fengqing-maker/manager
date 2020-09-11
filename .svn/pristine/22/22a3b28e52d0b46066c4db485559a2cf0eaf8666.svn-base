#ifndef CONNECT_CHECK_THREAD
#define CONNECT_CHECK_THREAD

#include "Src/Tools/BaseThread/BaseThread.h"
#include "Src/action/factory/actionWatchServerFactory.h"
#include "Src/action/factory/actionServerFactory.h"
#include "../messageDef.h"		//for WM_CONNECT_CHECK_BASE
#include <boost/scoped_ptr.hpp>

class CWatchTaskThread : public CBaseThread
{
	DECLARE_DYNCREATE(CWatchTaskThread)

public:
	CWatchTaskThread();
	virtual ~CWatchTaskThread();
	//function
	void StartCheckTask();		/* 开始在线程中检查连接状态 */
	void StopCheckTask();		/* 停止在线程中检查连接状态 */
	void RegistActionSever( CActionWatchServerFactory &action ){ m_watchSever = &action ; };
    void RegistOrginSever( CActionServerFactory &action ){ m_orginSever =  &action  ;}
protected:	
	afx_msg void OnCheckStart(WPARAM wParam,LPARAM lParam);

	afx_msg void OnCheckExcute(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCheckFinish(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	virtual CString GetThreadName();
    
    BOOL FindTaskById( int taskId, const ListTask &lst, ListTaskConstItor &itor ) const;
    BOOL FindScheduleById( int taskId, const ListSchedule &lst, ListScheduleConstItor &itor ) const;
    BOOL FindChangeById( int taskId, const ListChange &lst, ListChangeConstItor &itor ) const;
    void ChangeIncreaseHandle(int taskType, const ListTask &lst, const ListTask &orginLst ) const;
    bool OnCheckTask(  int taskType, const ListTask &lst, const ListTask &orginLst ) const;
    bool OnCheckScheule(  int taskType, const MapTaskSchedule &lst, const MapTaskSchedule &orginLst ) const;
    bool OnCheckTaskChange(  int taskType, const ListChange &lst, const ListChange &orginLst ) const;

	
private:

	BOOL			           m_bCheckStoped;	/* 线程停止检查连接标识 */
	CCriticalSection	       m_stopCS;	/* 停止标识同步 */
	CActionWatchServerFactory*  m_watchSever;
	CActionServerFactory*       m_orginSever;
};


#endif