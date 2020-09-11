#include "StdAfx.h"
#include "AbstractSchemaManager.h"


AbstractSchemaManager::AbstractSchemaManager(void)
{
	m_schemaFile.reset();
	m_schemaThread.reset();
}
AbstractSchemaManager::~AbstractSchemaManager(void)
{
}
CXMLSOMFilePtr AbstractSchemaManager::GetSOMFile()
{
	if (IsFileOpened())
	{
		return m_schemaFile;
	}
	else
	{
		return NULL;
	}
}

BOOL AbstractSchemaManager::IsFileOpened()
{
	if (m_schemaFile != NULL && !m_filePath.IsEmpty())
	{
		return TRUE;
	}
	else if(m_schemaFile == NULL && m_filePath.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
}


BOOL AbstractSchemaManager::Open(CString filePath,CString namespaceURI)
{
	if (IsFileOpened())
	{
		return FALSE;
	}
	m_schemaThread.reset();
	m_schemaThread = GetSchemaThread();
	if (m_schemaThread == NULL)
	{
		return FALSE;
	}
	m_filePath = filePath;
	m_namespaceURI = namespaceURI;
	m_schemaThread->addObserver(m_msgHandlerList);
	m_schemaThread->create();
	m_schemaThread->startSchemaOpen();
	return TRUE;
}
BOOL AbstractSchemaManager::Close()
{
	return doClose();
}

BOOL AbstractSchemaManager::doOpen()
{
	ASSERT(m_schemaFile == NULL);
	DesBool desRet;
	m_schemaFile.reset(new CXMLSOMFile());
	desRet = m_schemaFile->Open(m_filePath,m_namespaceURI);
	if (!desRet.result)
	{
		m_filePath.Empty();
		m_namespaceURI.Empty();
		m_schemaFile.reset();
	}
	return desRet.result;
}
BOOL AbstractSchemaManager::doClose()
{
	ASSERT(m_schemaFile != NULL);
	DesBool desRet;
	desRet = m_schemaFile->Close();
	if (desRet.result)
	{
		m_filePath.Empty();
		m_namespaceURI.Empty();
		m_schemaFile.reset();
	}
	return desRet.result;
}
/******************************protected function*************************/
Unique_SchemaThread AbstractSchemaManager::GetSchemaThread()
{
	return NULL;
}

void AbstractSchemaManager::OutputMsg(CString msg)
{

}
void AbstractSchemaManager::OutputErr(CString err)
{

}