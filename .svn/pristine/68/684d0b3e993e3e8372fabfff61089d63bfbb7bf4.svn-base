// BaseThread.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseThread.h"

#define DEBUG_TRACE
#include "../TRACE_D/trace_d.h"			/* 打印调试信息 */

#ifdef _DEBUG                      /* 判断是否定义_DEBUG */
#undef THIS_FILE                   /* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;   /* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              /* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */

// CBaseThread

IMPLEMENT_DYNCREATE(CBaseThread, CWinThread)
//IMPLEMENT_OBSERVER_ARRAY(CBaseThread)
CBaseThread::CBaseThread()
{
	m_bCreated = FALSE;
	//事件初始化的时候设置为不发信息状态
	m_pCreateEvent.reset(new CEvent(FALSE,/* 初始处于未发信状态 */
								FALSE,/* 使用自动事件 */
								_T("线程创建同步事件"),NULL));
	m_pDestroyEvent.reset(new CEvent(FALSE,/* 初始处于未发信状态 */
								FALSE,/* 使用自动事件 */
								_T("线程销毁同步事件"),NULL));
	m_bAutoDelete = FALSE;		/* 线程结束时不释放线程对象 */

}

CBaseThread::~CBaseThread()
{
	this->destroy();
}

BEGIN_MESSAGE_MAP(CBaseThread, CWinThread)
END_MESSAGE_MAP()
void CBaseThread::create()
{
	if (m_bCreated == FALSE)
	{
		BOOL ret = this->CreateThread();	/* 线程创建后立即执行 */
		m_pCreateEvent->Lock();				/* 等待线程创建完成 */
		if (ret == FALSE)
		{
			TRACE_D2("创建线程( name : %s ID : %d)失败.\n",
				GetThreadName(),this->m_nThreadID);
		}
		else
		{
			TRACE_D2("创建线程( name : %s ID : %d)成功.\n",
				GetThreadName(),this->m_nThreadID);
		}
	}
	ASSERT(m_bCreated == TRUE);
}
void CBaseThread::destroy()
{
	if (m_bCreated == TRUE)
	{
		this->quit();				/* 退出线程 */
		this->wait();				/* 线程退出后要求等待退出结果 */
		this->clearObserverArray();	/* 清空观察者队列 */
		m_pDestroyEvent->Lock();	/* 当线程退出后销毁事件被置为有信号状态 */		
		TRACE_D2("销毁线程( name : %s ID : %d)成功.\n",
			GetThreadName(),this->m_nThreadID);
	}
	ASSERT(m_bCreated == FALSE);
}
BOOL CBaseThread::isRunning()
{
	if (m_bCreated == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//查看需要通知的窗体是否已经存在了，如果没有存在就添加
void CBaseThread::addObserver(HWND hWnd)
{											
	if (!::IsWindow(hWnd))
	{
		TRACE("无效的“监视者”窗口句柄。\n");
		return ;
	}
	Observer_ConstItor itor = m_observerArray.begin();
	while (itor != m_observerArray.end())
	{
		if (hWnd == *itor)
		{
			return ;
		}
		itor++;
	}
	m_observerArray.push_back(hWnd);
}

//把所有监视者窗体添加的到监视者容器中
void CBaseThread::addObserver(MsgHandlerList hWndList)
{
	MsgHandlerListItor hWndItor = hWndList.begin();
	while(hWndItor != hWndList.end())
	{
		HWND hWnd = *hWndItor;
		addObserver(hWnd);
		hWndItor++;
	}
}

//删除一个监视者窗体
void CBaseThread::subObserver(HWND hWnd)
{
	if (!::IsWindow(hWnd))
	{
		TRACE("无效的“监视者”窗口句柄。\n");
		return ;
	}
	Observer_ConstItor itor = m_observerArray.begin();
	while (itor != m_observerArray.end())
	{
		if (hWnd == *itor)
		{
			m_observerArray.erase(itor);
			break;
		}
		itor++;
	}
}

//把所有的监视者窗体删除掉
void CBaseThread::subObserver(MsgHandlerList hWndList)
{
	MsgHandlerListItor hWndItor = hWndList.begin();
	while(hWndItor != hWndList.end())
	{
		HWND hWnd = *hWndItor;
		subObserver(hWnd);
		hWndItor++;
	}
}

//从容器中删除所有监视者
void CBaseThread::clearObserverArray()
{
	m_observerArray.clear();
}

//这个消息主要是用于针对哪一个操作，看得蒙蔽
void CBaseThread::postObserverMessage(UINT message,WPARAM wParam /* =NULL */, LPARAM lParam /* =NULL */) const
{
	Observer_ConstItor itor = m_observerArray.begin();			
	while (itor != m_observerArray.end())
	{
		HWND hWnd = (*itor);
		if(::IsWindow(hWnd) == FALSE)
		{
			itor++;
			continue;
		}
		CWnd* pWnd = CWnd::FromHandle(hWnd);
		if(pWnd == 0)
		{
			itor++;
			continue;
		}
		pWnd->PostMessage(message,wParam,lParam);
		itor++;
	}
}
/***************************protected functon*********************************/
//初始化事件，把事件设置为有信号状态
BOOL CBaseThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	//Typically, you override InitInstance to perform tasks 
	//that must be completed when a thread is first created. 
	
	VERIFY(SUCCEEDED(CoInitialize(NULL)));	/* 初始化线程COM组件环境 */

	this->m_pMainWnd = NULL;
	m_bCreated = TRUE;		/* 线程创建完成 */

	VERIFY(m_pCreateEvent->SetEvent());		//将事件对象设置为有信号对象

	return TRUE;		/* 返回TRUE创建消息循环 */
}

//退出时设置事件信号为有状态
int CBaseThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	//Your implementation of ExitInstance should call the 
	//base class's version after your code is executed. 

	CoUninitialize();		/* 关闭线程COM组件环境 */

	int ret = CWinThread::ExitInstance();
	TRACE_D2("线程( name: %s ID : %d)退出成功.\n",
		GetThreadName(),this->m_nThreadID);
	m_bCreated = FALSE;

	VERIFY(m_pDestroyEvent->SetEvent());

	return ret;
}

// CBaseThread 消息处理程序

/******************************private function***************************/
//发送线程退出消息
void CBaseThread::quit()
{
	if (m_bCreated == TRUE)
	{
		TRACE_D2("向线程( name: %s ID : %d)发送结束消息.\n",
			GetThreadName(),this->m_nThreadID);
		this->PostThreadMessage(WM_QUIT,0,0);
	}
}
//等待线程完全退出
void CBaseThread::wait()
{								/* 不可以在线程执行过程中等待自己退出，会死锁 */
	while (TRUE && m_bCreated == TRUE)
	{
		int nCount = 1;	/* 等待线程的数目 */
		DWORD ret = ::MsgWaitForMultipleObjects(nCount,&this->m_hThread,FALSE,INFINITE,QS_ALLINPUT);
		if (ret >= WAIT_OBJECT_0 && ret <= WAIT_OBJECT_0 + nCount - 1)
		{
			TRACE_D1("等待线程( name: %s)退出成功.\n",GetThreadName());
			/* 等待线程句柄成功 */
			break;
		}
		else if(ret == WAIT_OBJECT_0 + nCount)
		{
			/* 消息到达 */
			MSG msg;
			//TRACE_D1("等待线程( ID: %d)退出时收到消息.\n",this->m_nThreadID);
			while (::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			continue;
		}
		else if (ret == WAIT_FAILED)
		{
			/* 等待失败 */
			DWORD dErrCode=GetLastError();
			TRACE_D2("等待线程( name: %s)结束失败.\n失败原因：%d.\n",GetThreadName(),dErrCode);
			break;
		}
		else
		{
			/* 意外情况 */
			TRACE_D0("等待线程退出时遇到意外情况.\n");
			continue;
		}
	}
	CloseHandle(this->m_hThread);/* 线程结束时不会关闭自己的线程句柄 */
	this->m_hThread = NULL;
}

//获取线程ID
CString CBaseThread::GetThreadName()
{
	CString threadName;
	threadName.Format(_T("%d"),m_nThreadID);
	return threadName;
}