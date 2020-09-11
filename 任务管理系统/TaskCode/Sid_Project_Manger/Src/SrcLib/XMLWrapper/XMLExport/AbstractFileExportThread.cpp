#include "StdAfx.h"
#include "AbstractFileExportThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(AbstractFileExportThread,CBaseThread)
AbstractFileExportThread::AbstractFileExportThread(void)
{
	
}
AbstractFileExportThread::~AbstractFileExportThread(void)
{
	CoUninitialize();
}

BEGIN_MESSAGE_MAP(AbstractFileExportThread,CBaseThread)

END_MESSAGE_MAP()

void AbstractFileExportThread::startFileExport()
{

}


/******************************protected function*************************/
CString AbstractFileExportThread::GetThreadName()
{
	return _T("抽象文件导出线程");
}