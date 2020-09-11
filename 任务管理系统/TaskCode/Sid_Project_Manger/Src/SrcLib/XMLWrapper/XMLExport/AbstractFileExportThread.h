#pragma once
#include "BaseThread.h"

class AbstractFileExportThread;
typedef std::unique_ptr<AbstractFileExportThread>		Unique_ExportThread;

class AbstractFileExportThread : public CBaseThread
{
	DECLARE_DYNCREATE(AbstractFileExportThread)
public:
	AbstractFileExportThread(void);
	virtual ~AbstractFileExportThread(void);

	/** @brief 必须被重载以启动文件导出操作 */
	virtual void startFileExport();

protected:

	/** @see CBaseThread::GetThreadName() */
	virtual CString GetThreadName();


	DECLARE_MESSAGE_MAP()


};

