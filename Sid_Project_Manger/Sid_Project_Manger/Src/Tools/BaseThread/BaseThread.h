#pragma once

#include <list>
#include <vector>
//#include <boost/scoped_ptr.hpp>
#include "SmartLock.h"			/* ��������������ͬ������ */

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
	CBaseThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBaseThread();

	virtual void	create();		/* �����߳� */
	virtual void	destroy();		/* �����̣߳��������̱߳����е��� */
	virtual BOOL	isRunning();	/* �߳��Ƿ��������� */


	void addObserver(HWND hWnd);			/* �����߳��е��ã���¼��Ϊ�����ߵĴ��ھ�� */
	void addObserver(MsgHandlerList hWndList);
	void subObserver(HWND hWnd);			/* �����߳��е��ã�ȡ����Ϊ�����ߵĴ��ھ�� */
	void subObserver(MsgHandlerList hWndList);
	void clearObserverArray();				/* ��չ۲��߶��� */
	void postObserverMessage(UINT message,
		WPARAM wParam = NULL, LPARAM lParam = NULL) const;/* �����߳��е��ã�������߶��з�����Ϣ */

protected:
	virtual BOOL InitInstance();			/* ����û���ر�����Ҫ���� */
	virtual int ExitInstance();				/* ����û���ر�����Ҫ���� */

	virtual void	quit();			/* �����̣߳�����������Ҫ���� */
	virtual void	wait();			/* �ȴ��߳̽���������������Ҫ���� */

	virtual CString	GetThreadName();	/* ���߳̿�������ʵ��Ϊ�߳����� */

	DECLARE_MESSAGE_MAP()



private:
	BOOL						m_bCreated;			/* �̴߳�����ʶ */
	std::unique_ptr<CEvent>		m_pCreateEvent;		/* �̴߳����¼� */
	std::unique_ptr<CEvent>		m_pDestroyEvent;	/* �߳������¼� */
protected:
	Observer_Array	m_observerArray;	/* ��Ҫ֪ͨ�ġ������ߡ����� */

};	