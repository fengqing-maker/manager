#include "stdafx.h"
#include "AbstractFileManageThread.h"


IMPLEMENT_DYNCREATE(AbstractFileManageThread,CBaseThread)
AbstractFileManageThread::AbstractFileManageThread(void)
{
}
AbstractFileManageThread::~AbstractFileManageThread(void)
{
}

BEGIN_MESSAGE_MAP(AbstractFileManageThread,CBaseThread)

END_MESSAGE_MAP()

void AbstractFileManageThread::startFileNew()
{

}
void AbstractFileManageThread::startFileOpen()
{

}
void AbstractFileManageThread::startFileClose()
{

}

/****************************protected function*****************************/
CString AbstractFileManageThread::GetThreadName()
{
	return _T("抽象文件操作线程");
}