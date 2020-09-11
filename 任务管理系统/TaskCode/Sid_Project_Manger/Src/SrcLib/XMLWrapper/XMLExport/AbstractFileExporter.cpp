#include "StdAfx.h"
#include "AbstractFileExporter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AbstractFileExporter::AbstractFileExporter(void)
{
}
AbstractFileExporter::~AbstractFileExporter(void)
{
}


BOOL AbstractFileExporter::ExportFile(BOOL bMultiThread)
{
	if (bMultiThread)
	{
		return StartFileExportThread();
	}
	else
	{
		return DoExportFile();
	}
}

/******************************protected function*************************/
BOOL AbstractFileExporter::DoExportFile()
{
	return TRUE;
}

Unique_ExportThread AbstractFileExporter::GetFileExportThread()
{
	Unique_ExportThread exportThread;
	return exportThread;
}

/******************************private function***************************/
BOOL AbstractFileExporter::StartFileExportThread()
{
	m_exportThread.reset();
	if (m_exportThread == 0)
	{
		m_exportThread = GetFileExportThread();
		if (m_exportThread == 0)
			return FALSE;
	}
	m_exportThread->addObserver(m_msgHandlerList);
	if (!m_exportThread->isRunning())
	{
		m_exportThread->create();
		if(!m_exportThread->isRunning())
			return FALSE;
	}
	m_exportThread->startFileExport();
	return TRUE;
}