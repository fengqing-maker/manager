#pragma once

#include <list>
#include <vector>
//#include <boost/scoped_ptr.hpp>
#include "SmartLock.h"			/* “智能锁”用于同步互斥 */

typedef std::list<HWND>						MsgHandlerList;
typedef MsgHandlerList::iterator			MsgHandlerListItor;
typedef MsgHandlerList::const_iterator		MsgHandlerListConstItor;

typedef std::vector<HWND>					Observer_Array;
typedef std::vector<HWND>::iterator			Observer_Itor;
typedef std::vector<HWND>::const_iterator	Observer_ConstItor;


// CBaseThread

class CBaseThread : public CWinThread
{
	DECLARE_DYNCREATE(CBaseThread)
	//DECLARE_OBSERVER_ARRAY()
public:
	CBaseThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CBaseThread();

	virtual void	create();		/* 创建线程 */
	virtual void	destroy();		/* 销毁线程，不能在线程本身中调用 */
	virtual BOOL	isRunning();	/* 线程是否正在运行 */


	void addObserver(HWND hWnd);			/* 在主线程中调用，记录作为监视者的窗口句柄 */
	void addObserver(MsgHandlerList hWndList);
	void subObserver(HWND hWnd);			/* 在主线程中调用，取消作为监视者的窗口句柄 */
	void subObserver(MsgHandlerList hWndList);
	void clearObserverArray();				/* 清空观察者队列 */
	void postObserverMessage(UINT message,
		WPARAM wParam = NULL, LPARAM lParam = NULL) const;/* 在子线程中调用，向监视者队列发送消息 */

protected:
	virtual BOOL InitInstance();			/* 子类没有特别需求不要重载 */
	virtual int ExitInstance();				/* 子类没有特别需求不要重载 */

	virtual void	quit();			/* 结束线程，无特殊需求不要重载 */
	virtual void	wait();			/* 等待线程结束，无特殊需求不要重载 */

	virtual CString	GetThreadName();	/* 子线程可重载来实现为线程命名 */

	DECLARE_MESSAGE_MAP()



private:
	BOOL						m_bCreated;			/* 线程创建标识 */
	std::unique_ptr<CEvent>		m_pCreateEvent;		/* 线程创建事件 */
	std::unique_ptr<CEvent>		m_pDestroyEvent;	/* 线程销毁事件 */
protected:
	Observer_Array	m_observerArray;	/* 需要通知的“监视者”队列 */

};	