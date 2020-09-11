#pragma once
#include "BaseThread.h"


class AbstractSchemaManageThread;
typedef std::unique_ptr<AbstractSchemaManageThread>		Unique_SchemaThread;

// AbstractSchemaManageThread

class AbstractSchemaManageThread : public CBaseThread
{
	DECLARE_DYNCREATE(AbstractSchemaManageThread)

public:
	AbstractSchemaManageThread();           // 动态创建所使用的受保护的构造函数
	virtual ~AbstractSchemaManageThread();

	/** @brief 重载以启动Schema加载操作 */
	virtual void startSchemaOpen();
protected:
	DECLARE_MESSAGE_MAP()



};


