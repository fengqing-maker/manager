// AbstractSchemaManageThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EP61850Client.h"
#include "AbstractSchemaManageThread.h"


// AbstractSchemaManageThread

IMPLEMENT_DYNCREATE(AbstractSchemaManageThread, CBaseThread)
AbstractSchemaManageThread::AbstractSchemaManageThread()
{
}
AbstractSchemaManageThread::~AbstractSchemaManageThread()
{
}
void AbstractSchemaManageThread::startSchemaOpen()
{
	/* ���������߳�������ʵ�� */
	ASSERT(0);
}


BEGIN_MESSAGE_MAP(AbstractSchemaManageThread, CBaseThread)
END_MESSAGE_MAP()


// AbstractSchemaManageThread ��Ϣ�������
