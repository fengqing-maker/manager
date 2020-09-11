#pragma once
#include "BaseThread.h"

class AbstractFileManageThread;
typedef std::unique_ptr<AbstractFileManageThread>		Unique_FileThread;


class AbstractFileManageThread : public CBaseThread
{
	DECLARE_DYNCREATE(AbstractFileManageThread);
public:
	AbstractFileManageThread(void);
	virtual ~AbstractFileManageThread(void);

	/** @brief 子线程类重载实现启动新建文件操作 */
	virtual void startFileNew();

	/** @brief 子线程类重载实现启动打开文件操作 */
	virtual void startFileOpen();

	/** @brief 子线程类重载实现启动关闭文件操作 */
	virtual void startFileClose();

protected:
	/** 
	*@brief 设置线程名称
	*@see	CBaseThread::GetThreadName()
	*/
	virtual CString GetThreadName();

	DECLARE_MESSAGE_MAP()

};

