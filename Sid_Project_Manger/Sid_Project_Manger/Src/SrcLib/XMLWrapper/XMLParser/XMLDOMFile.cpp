#include "stdafx.h"
#include "XMLDOMFile.h"
#include "XMLDOMNode.h"
#include "XMLDOMNodes.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//Precompile processor - For MFC
#ifdef _DEBUG                      	/* �ж��Ƿ���_DEBUG */
#undef THIS_FILE                   	/* ȡ��THIS_FILE�Ķ��� */
static char THIS_FILE[]=__FILE__;  	/* ����THIS_FILEָ���ļ��� */
#define new DEBUG_NEW              	/* �������new�꣬ȡ��new�ؼ��� */
#endif								/* ���� */




#define RETURN_NULL_PTR		CXMLDOMNodePtr pRetNode( new CXMLDOMNode( NULL ) );return pRetNode; 
#define RETURN_NULL_LIST	CXMLDOMNodesPtr pRetNodes( new CXMLDOMNodes( NULL ) );return pRetNodes; 
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// static variable declaration:
XFilePropertyMap CXMLDOMFile::m_filePropertyMap = XFilePropertyMap();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Constructor
CXMLDOMFile::CXMLDOMFile()
{
	m_pXMLDOMDocument = NULL ;
}
// Deconstructor
CXMLDOMFile::~CXMLDOMFile()
{
	this->Close(FALSE);
}

BOOL CXMLDOMFile::IsFileExist( CString strFileName )
{
	if( strFileName.IsEmpty() )
	{
		return FALSE;
	} 

	BOOL bExist = FALSE;
	HANDLE hFile = NULL;
	try
	{
		WIN32_FIND_DATA fd;
		hFile = FindFirstFile(strFileName,&fd);
		if (hFile != INVALID_HANDLE_VALUE )
		{
			bExist = TRUE;
			VERIFY(FindClose(hFile));
		}
		hFile = NULL;
	}
	catch (_com_error &e)
	{
		ShowComExceptionWithReason(_T("XML IsFileExist() failed."),e);
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML IsFileExist() failed."));
	}
	return bExist;
}
BOOL CXMLDOMFile::IsFileOpened()
{
	return GetXMLFileOpenState(m_pXMLDOMDocument);
}

DesBool CXMLDOMFile::New(CString filePath,CString rootName,CString rootNamespace,CXMLSOMFilePtr schemaFile /*=NULL*/)
{
	DesBool desBool;
	if (filePath.IsEmpty())
	{
		desBool.result = FALSE;
		desBool.description.Format(_T("File name must not be empty."));
		return desBool;
	}
	if(IsFileExist(filePath))
	{
		VERIFY(DeleteFile(filePath));	/* ɾ��ԭ�ȵ��ļ� */
	}
	/* ���Ѵ��ļ������Ƚ��б��桢�ر� */
	if (IsFileOpened())
	{
		VERIFY(Close(TRUE).result);
	}

	HRESULT hr;
	try
	{
		hr = m_pXMLDOMDocument.CreateInstance( __uuidof( MSXML2::FreeThreadedDOMDocument60 ) );
		TESTHR(hr);
		MSXML2::IXMLDOMNodePtr pRoot = m_pXMLDOMDocument->
			createNode(MSXML2::NODE_ELEMENT,(_bstr_t)(LPCTSTR)rootName,(_bstr_t)(LPCTSTR)rootNamespace);
		ASSERT(pRoot != NULL);
		VERIFY(m_pXMLDOMDocument->appendChild(pRoot));
		m_pXMLDOMDocument->Putasync(VARIANT_FALSE);
		if (schemaFile != NULL)
		{
			if(!SetRootSchema(schemaFile))
			{
				CString schemaPath = schemaFile->GetFileName();
				desBool.description.Format(_T("Init file %s with schema file %s failed."),filePath,schemaPath);
				desBool.result = FALSE;
				m_pXMLDOMDocument=NULL;
				return desBool;
			}
		}
		hr = m_pXMLDOMDocument->save((_bstr_t)(LPCTSTR)filePath);
		TESTHR(hr);
	}
	catch (_com_error& e)
	{
		m_pXMLDOMDocument=NULL;
		desBool.description = ShowComExceptionWithReason(_T("XML New() failed.\nExecption thrown."),e);
		desBool.result = FALSE;
		return desBool;
	}
	catch (...)
	{
		m_pXMLDOMDocument=NULL;
		desBool.description = ShowComExceptionUnknow(_T("XML New() failed.\nExecption thrown."));
		desBool.result = FALSE;
		return desBool;
	}
	m_pXMLDOMDocument = NULL;
	desBool.description.Format(_T("New file %s create succeed."),filePath);
	desBool.result = TRUE;
	return desBool;
}

DesBool CXMLDOMFile::Open(CString filePath,CXMLSOMFilePtr schemaFile /*=NULL*/)
{
	DesBool desBool;
	if (filePath.IsEmpty())
	{
		desBool.result = FALSE;
		desBool.description.Format(_T("File name must not be empty."));
		return desBool;
	}
	if (IsFileOpened())
	{
		if (filePath != GetXMLFilePath(m_pXMLDOMDocument))
		{
			VERIFY(Close(TRUE).result);
		}
		else
		{
			desBool.result = TRUE;
			desBool.description = _T("File was opened already.");
			return desBool;
		}
	}
	HRESULT hr ;
	try
	{
		hr = m_pXMLDOMDocument.CreateInstance( __uuidof( MSXML2::FreeThreadedDOMDocument60 ) );
		TESTHR(hr);
		m_pXMLDOMDocument->Putasync(VARIANT_FALSE);
		m_pXMLDOMDocument->PutvalidateOnParse(VARIANT_FALSE);
		if ( m_pXMLDOMDocument->load((_bstr_t)(LPCTSTR)filePath) != VARIANT_TRUE )
		{
			desBool.description.Format(_T("Load file %s failed."),filePath);
			desBool.result = FALSE;
			m_pXMLDOMDocument=NULL;
			return desBool;
		}
		if (schemaFile == NULL)	
		{
			/* �����ļ�ʱδ��ȷָ��Schema�ļ�����ʹ�ø�Ԫ����Ĭ�ϵ�Schema�ļ� */
			schemaFile = GetRootSchema();
		}
		else
		{
			/* �����ļ�ʱ��ȷָ��Schema�ļ�����ʹ���û�ָ����Schema�ļ������޸ĸ��ڵ�Schema��Ϣ */
			/* �޸ĸ��ڵ�schema��ϢӦ����DOM����֮����ܽ��� */
			if (!SetRootSchema(schemaFile))
			{
				CString schemaPath = schemaFile->GetFileName();
				desBool.description.Format(_T("Init file %s with schema file %s failed."),filePath,schemaPath);
				desBool.result = FALSE;
				m_pXMLDOMDocument=NULL;
				return desBool;
			}
			/* �޸ĸ��ڵ�Schema�ɹ�����Ҫ����һ���ļ� */
			hr = m_pXMLDOMDocument->save((_bstr_t)(LPCTSTR)filePath);
			if (FAILED(hr))
			{
				VERIFY(0);	/* Ҫ���ܹ�����ɹ� */
			}
		}
		if (schemaFile != NULL)
		{
			MSXML2::IXMLDOMSchemaCollection2Ptr pSchemaCollection = schemaFile->GetInnerPtr();
			if (pSchemaCollection != NULL)
			{
				/* putref_SchemasҪ����load��validate֮ǰ���������� */
				m_pXMLDOMDocument->PutRefschemas((_variant_t)(IDispatch*)pSchemaCollection);
				/* ��������schema�����¼����ļ� */
				VERIFY(m_pXMLDOMDocument->load((_bstr_t)(LPCTSTR)filePath) == VARIANT_TRUE );
			}
		}
		if (!InitXPathQuery(m_pXMLDOMDocument)/* ��ʼ��XPath��ѯ */)
		{
			desBool.description.Format(_T("Init file %s with XPath Query failed."),filePath);
			desBool.result = FALSE;
			m_pXMLDOMDocument=NULL;
			return desBool;
		}
		if (!InitXSLTScript(m_pXMLDOMDocument)/* ��ʼ��XSLT�ű� */)
		{
			desBool.description.Format(_T("Init file %s with XSLT Script failed."),filePath);
			desBool.result = FALSE;
			m_pXMLDOMDocument=NULL;
			return desBool;
		}
		if (!InitMultiErrorMessages(m_pXMLDOMDocument)/* �����ض���������� */)
		{
			desBool.description.Format(_T("Init file %s with Multiple Messapge failed."),filePath);
			desBool.result = FALSE;
			m_pXMLDOMDocument=NULL;
			return desBool;
		}
	}
	catch (_com_error& e)
	{
		desBool.description = ShowComExceptionWithReason(_T("XML Open() failed.\nExecption thrown."),e);
		desBool.result = FALSE;
		m_pXMLDOMDocument=NULL;
		return desBool;
	}
	catch ( ... )
	{
		desBool.description = ShowComExceptionUnknow(_T("XML Open() failed.\nException thrown."));
		desBool.result = FALSE;
		m_pXMLDOMDocument=NULL;
		return desBool;
	}
	AddToFilePropertyMap(m_pXMLDOMDocument);
	SetXMLFilePath(filePath);
	SetXMLFileOpenState(TRUE);
	SetXMLFileSchemaFile(schemaFile);
	desBool.description.Format(_T("Open file %s succeed."),filePath);
	desBool.result = TRUE;
	return desBool;
	
}

DesBool CXMLDOMFile::Save( CString newFilePath /*=_T("")*/)
{
	DesBool desBool;
	if (!IsFileOpened())
	{
		desBool.description.Format(_T("File not opened."));
		desBool.result = FALSE;
		return desBool;
	}
	CString filePath;
	if(newFilePath.IsEmpty())
		filePath = GetXMLFilePath();
	else
		filePath = newFilePath;
	XCodecType codeType = GetXMLFileCodeType();
	HRESULT hr;
	try
	{
		hr = m_pXMLDOMDocument->save( (_bstr_t)filePath );
		TESTHR(hr);
		if (hr != S_OK || !NormalizeFormat(filePath,codeType))
		{
			desBool.description.Format(_T("Save file %s failed."),filePath);
			desBool.result = FALSE;
			return desBool;
		}
	}
	catch (_com_error& e)
	{
		desBool.description = ShowComExceptionWithReason(_T("XML Save() Failed.\nExecption thrown."),e);
		desBool.result = FALSE;
		return desBool;
	}
	catch ( ... )
	{
		desBool.description = ShowComExceptionUnknow(_T("XML Save() failed.\nExecption thrown."));
		desBool.result = FALSE;
		return desBool;
	}
	SetXMLFilePath(filePath);
	SetXMLFileModified(FALSE);

	desBool.description.Format(_T("Save file %s succeed."),filePath);
	desBool.result = TRUE;
	return desBool;
}

DesBool CXMLDOMFile::Close(BOOL bSaveOrNot)
{
	DesBool desBool;
	if (IsFileOpened())
	{
		if (bSaveOrNot)
		{
			desBool = Save();
			VERIFY(desBool.result);
		}
		CString filePath = GetXMLFilePath();
		RemoveFromFilePropertyMap(m_pXMLDOMDocument);
		desBool.description.Format(_T("Close file %s succeed."),filePath);
		desBool.result = TRUE;
		return desBool;
	}
	else
	{
		desBool.description.Format(_T("File was not opened."));
		desBool.result = TRUE;
		return desBool;
	}
}

CXMLDOMNodePtr CXMLDOMFile::GetRoot( void )
{	
	if( !IsFileOpened() )
	{
		return NULL;
	}
	
	MSXML2::IXMLDOMElementPtr pXMLDOMElement = NULL;
	try
	{
		pXMLDOMElement = m_pXMLDOMDocument->GetdocumentElement();
		if (pXMLDOMElement==NULL)
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML GetRoot() failed.\nException thrown."),e);
		return NULL;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML GetRoot() failed.\nException thrown."));
		return NULL;
	}

	CXMLDOMNodePtr pXMLDOMNode( new CXMLDOMNode( pXMLDOMElement ) );// Must implementation for CXMLDOMNode( MSXML2::IXMLDOMNodePtr pXMLDOMNode )

	return pXMLDOMNode;
}
BOOL CXMLDOMFile::SetRoot(CXMLDOMNodePtr root)
{
	if (!IsFileOpened() || root == NULL)
	{
		return FALSE;
	}
	try
	{
		MSXML2::IXMLDOMElementPtr pNewRootElem = root->GetInnerPtr();
		m_pXMLDOMDocument->PutRefdocumentElement(pNewRootElem);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML SetRoot() failed.\nException thrown."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML SetRoot() failed.\nException thrown."));
		return FALSE;
	}
	return TRUE;
}

CXMLSOMFilePtr CXMLDOMFile::GetRootSchema()
{
	if (m_pXMLDOMDocument == NULL )
	{
		return NULL;
	}
	CXMLSOMFilePtr xmlSOMFile = NULL;
	try
	{
		MSXML2::IXMLDOMElementPtr pIXMLDOMRoot = m_pXMLDOMDocument->GetdocumentElement();
		ASSERT(pIXMLDOMRoot != NULL);
		if(pIXMLDOMRoot == NULL)
			return FALSE;
		MSXML2::IXMLDOMAttributePtr pIXMLDOMInstanceNS = NULL;		/* xmlns:xxx="namespace for instance" */
		MSXML2::IXMLDOMAttributePtr pIXMLDOMSchemaLocation = NULL;	/* xxx:schemaLocation="path for schema file" */
		MSXML2::IXMLDOMAttributePtr pIXMLDOMRootElemNS = NULL;		/* xmlns:xxx="namespace for xml element" */
		CString schemaPath;
		CString schemaNamespace;

		MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMRootAttributeMap = 
			pIXMLDOMRoot->Getattributes();
		long attrCount = pIXMLDOMRootAttributeMap->Getlength();
		for (long i = 0; i < attrCount; i++)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMRootAttribute = 
				pIXMLDOMRootAttributeMap->Getitem(i);
			if (pIXMLDOMRootAttribute != NULL
				&&VARIANT_TRUE == pIXMLDOMRootAttribute->Getspecified())
			{
				CString attrName = (LPCTSTR)(_bstr_t)pIXMLDOMRootAttribute->GetnodeName();
				CString attrValue = (LPCTSTR)(_bstr_t)pIXMLDOMRootAttribute->GetnodeValue();
				if (attrName.Find(STR_XML_XMLNS) != -1
					&& attrValue.Find(STR_XML_INSTANCE_NAMESPACE) != -1)
				{
					pIXMLDOMInstanceNS = pIXMLDOMRootAttribute;
				}
				if (attrName.Find(STR_XML_XMLNS) != -1
					&& attrValue.Find(STR_XML_INSTANCE_NAMESPACE) == -1)
				{
					pIXMLDOMRootElemNS = pIXMLDOMRootAttribute;
				}
				if (attrName.Find(STR_XML_SCHEMALOCATION) != -1)
				{
					pIXMLDOMSchemaLocation = pIXMLDOMRootAttribute;
				}
			}
		}
		if (pIXMLDOMInstanceNS != NULL 
			&& pIXMLDOMRootElemNS != NULL 
			&& pIXMLDOMSchemaLocation != NULL)
		{
			CString schemaLocation = (LPCTSTR)(_bstr_t)pIXMLDOMSchemaLocation->GetnodeValue();
			if (SplitSchemaLocation(schemaLocation,schemaNamespace,schemaPath))
			{
				xmlSOMFile.reset(new CXMLSOMFile());
				DesBool desRet = xmlSOMFile->Open(schemaPath,schemaNamespace);
				if (desRet.result == FALSE)
				{
					return NULL;
				}
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML GetRootSchema() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML GetRootSchema() failed."));
		return NULL;
	}
	return xmlSOMFile;
}
BOOL CXMLDOMFile::SetRootSchema(CXMLSOMFilePtr xmlSOMFile)
{
	if (m_pXMLDOMDocument == NULL || xmlSOMFile == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		MSXML2::IXMLDOMElementPtr pIXMLDOMRoot = m_pXMLDOMDocument->GetdocumentElement();
		ASSERT(pIXMLDOMRoot != NULL);
		if(pIXMLDOMRoot == NULL)
			return FALSE;
		MSXML2::IXMLDOMAttributePtr pIXMLDOMInstanceNS = NULL;		/* xmlns:xxx="namespace for instance" */
		MSXML2::IXMLDOMAttributePtr pIXMLDOMSchemaLocation = NULL;	/* xxx:schemaLocation="path for schema file" */
		MSXML2::IXMLDOMAttributePtr pIXMLDOMRootElemNS = NULL;		/* xmlns:xxx="namespace for xml element" */
		CString prefixForInstance;	/* "xxx" of xmlns:xxx in xmlns:xxx="namespace for instance" */
		CString prefixForRootElem;	/* "xxx" of xmlns:xxx in xmlns:xxx="namespace for root element" */

		CString schemaPath = xmlSOMFile->GetFileName();
		CString schemaNamespace = xmlSOMFile->GetFileNamespace();

		MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMRootAttributeMap = 
			pIXMLDOMRoot->Getattributes();
		long attrCount = pIXMLDOMRootAttributeMap->Getlength();
		for (long i = 0; i < attrCount; i++)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMRootAttribute = 
				pIXMLDOMRootAttributeMap->Getitem(i);
			if (pIXMLDOMRootAttribute != NULL
				&&VARIANT_TRUE == pIXMLDOMRootAttribute->Getspecified())
			{
				CString attrName = (LPCTSTR)(_bstr_t)pIXMLDOMRootAttribute->GetnodeName();
				CString attrValue = (LPCTSTR)(_bstr_t)pIXMLDOMRootAttribute->GetnodeValue();
				if (attrName.Find(STR_XML_XMLNS) != -1
					&& attrValue.Find(STR_XML_INSTANCE_NAMESPACE) != -1)
				{
					pIXMLDOMInstanceNS = pIXMLDOMRootAttribute;;
				}
				if (attrName.Find(STR_XML_XMLNS) != -1
					&& attrValue.Find(STR_XML_INSTANCE_NAMESPACE) == -1)
				{
					pIXMLDOMRootElemNS = pIXMLDOMRootAttribute;
				}
				if (attrName.Find(STR_XML_SCHEMALOCATION) != -1)
				{
					pIXMLDOMSchemaLocation = pIXMLDOMRootAttribute;
				}
			}
		}
		/* �ڸ��ڵ�����������xmlns:xxx="namespace for instance" */
		CString attrValue = STR_XML_INSTANCE_NAMESPACE;
		if (pIXMLDOMInstanceNS == NULL)
		{
			prefixForInstance = STR_XML_DEF_INSTANCE_PREFIX;
			CString attrName = JoinXmlnsAttrName(prefixForInstance);
			CString namespaceURI = _T("");
			pIXMLDOMInstanceNS = m_pXMLDOMDocument->createAttribute((_bstr_t)(LPCTSTR)attrName);
			pIXMLDOMInstanceNS->PutnodeValue((_bstr_t)(LPCTSTR)attrValue);
			MSXML2::IXMLDOMAttributePtr pOldAttribute = pIXMLDOMRoot->setAttributeNode(pIXMLDOMInstanceNS);
		}
		else
		{
			CString attrName = (LPCTSTR)(_bstr_t)pIXMLDOMInstanceNS->Getname();
			if (!SplitXmlnsAttrName(attrName,prefixForInstance))
			{
				return FALSE;
			}
			/* ����xmlns���͵����ԣ��������ʱ����ֱ���޸�����ֵ������ͨ��clone�����޸� */
			pIXMLDOMInstanceNS = pIXMLDOMInstanceNS->cloneNode(VARIANT_TRUE);
			pIXMLDOMInstanceNS->PutnodeValue((_bstr_t)(LPCTSTR)attrValue);
			MSXML2::IXMLDOMAttributePtr pOldAttribute = pIXMLDOMRoot->setAttributeNode(pIXMLDOMInstanceNS);
		}	
		ASSERT(!prefixForInstance.IsEmpty());
		ASSERT(pIXMLDOMInstanceNS != NULL);
		
		/* �ڸ��ڵ�����������xxx:schemaLocation="path for schema file" */
		attrValue = schemaNamespace + _T(" ") + schemaPath;
		if (pIXMLDOMSchemaLocation == NULL)
		{
			CString attrName = JoinQName(prefixForInstance,STR_XML_SCHEMALOCATION);
			pIXMLDOMSchemaLocation = m_pXMLDOMDocument->createAttribute((_bstr_t)(LPCTSTR)attrName);
			MSXML2::IXMLDOMAttributePtr pOldAttribute = pIXMLDOMRoot->setAttributeNode(pIXMLDOMSchemaLocation);
		}
		ASSERT((LPCTSTR)(_bstr_t)pIXMLDOMSchemaLocation->Getprefix() == prefixForInstance);
		ASSERT(pIXMLDOMSchemaLocation != NULL);
		pIXMLDOMSchemaLocation->PutnodeValue((_bstr_t)(LPCTSTR)attrValue);
		/* �ڸ��ڵ�����������xmlns:xxx="namespace for xml element" */
		attrValue = schemaNamespace;
		if (pIXMLDOMRootElemNS == NULL)
		{
			prefixForRootElem = (LPCTSTR)(_bstr_t)pIXMLDOMRoot->Getprefix();
			CString attrName = JoinXmlnsAttrName(prefixForRootElem);
			CString namespaceURI = _T("");
			pIXMLDOMRootElemNS = m_pXMLDOMDocument->createAttribute((_bstr_t)(LPCTSTR)attrName);
			pIXMLDOMRootElemNS->PutnodeValue((_bstr_t)(LPCTSTR)attrValue);
			MSXML2::IXMLDOMAttributePtr pOldAttribute = pIXMLDOMRoot->setAttributeNode(pIXMLDOMRootElemNS);
		}
		else
		{
			/* ����xmlns���͵����ԣ��������ʱ����ֱ���޸�����ֵ������ͨ��clone�����޸� */
			pIXMLDOMRootElemNS = pIXMLDOMRootElemNS->cloneNode(VARIANT_TRUE);
			pIXMLDOMRootElemNS->PutnodeValue((_bstr_t)(LPCTSTR)attrValue);
			MSXML2::IXMLDOMAttributePtr pOldAttribute = pIXMLDOMRoot->setAttributeNode(pIXMLDOMRootElemNS);
		}
		ASSERT(pIXMLDOMRootElemNS != NULL);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML SetRootSchema() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML SetRootSchema() failed."));
		return FALSE;
	}
	return TRUE;
}

CString CXMLDOMFile::GetDefaultElementPrefix()
{
	CString defaultPrefix;
	if (m_pXMLDOMDocument == NULL)
	{
		return defaultPrefix;
	}
	try
	{
		MSXML2::IXMLDOMElementPtr pXMLDOMRootElem = m_pXMLDOMDocument->GetdocumentElement();
		if (pXMLDOMRootElem == NULL)
		{
			return defaultPrefix;
		}
		CString namespaceURI = (LPCTSTR)(_bstr_t)pXMLDOMRootElem->GetnamespaceURI();
		if (namespaceURI.IsEmpty())
		{
			return defaultPrefix;
		}
		CString selNamespace = m_pXMLDOMDocument->getProperty((_bstr_t)(LPCTSTR)_T("SelectionNamespaces"));
		if (selNamespace.IsEmpty())
		{
			return defaultPrefix;
		}
		else
		{
			int pos1 = selNamespace.Find(_T(":"));
			int pos2 = selNamespace.Find(_T("="));
			defaultPrefix = selNamespace.Mid(pos1+1,pos2-pos1-1);
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML GetDefaultElementPrefix() failed."),e);
		return defaultPrefix;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML GetDefaultElementPrefix() failed."));
		return defaultPrefix;
	}
	return defaultPrefix;
}
BOOL CXMLDOMFile::SetDefaultElementPrefix(CString defaultPrefix)
{
	if (m_pXMLDOMDocument == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		MSXML2::IXMLDOMElementPtr pXMLDOMRootElem = m_pXMLDOMDocument->GetdocumentElement();
		if (pXMLDOMRootElem == NULL)
		{
			return FALSE;
		}
		CString namespaceURI = (LPCTSTR)(_bstr_t)pXMLDOMRootElem->GetnamespaceURI();
		if (namespaceURI.IsEmpty())
		{
			return TRUE;
		}
		CString selNamespace;
		selNamespace.Format(_T("xmlns:%s=\"%s\""),defaultPrefix,namespaceURI);
		HRESULT hr = m_pXMLDOMDocument->setProperty
			((_bstr_t)(LPCTSTR)_T("SelectionNamespaces"),(_bstr_t)(LPCTSTR)selNamespace);
		TESTHR(hr);
		result = TRUE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML SetDefaultElementPrefix() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML SetDefaultElementPrefix() failed."));
		return FALSE;
	}
	return result;
}

CString  CXMLDOMFile::GetXMLFilePath()
{
	return GetXMLFilePath(m_pXMLDOMDocument);
}
void CXMLDOMFile::SetXMLFilePath(CString filePath)
{
	SetXMLFilePath(m_pXMLDOMDocument,filePath);
}
XCodecType  CXMLDOMFile::GetXMLFileCodeType()
{
	return GetXMLFileCodeType(m_pXMLDOMDocument);
}
void CXMLDOMFile::SetXMLFileCodeType(XCodecType codeType)
{
	SetXMLFileCodeType(m_pXMLDOMDocument,codeType);
}
BOOL CXMLDOMFile::GetXMLFileModified()
{
	return GetXMLFileModified(m_pXMLDOMDocument);
}
void CXMLDOMFile::SetXMLFileModified(BOOL bModified)
{
	return SetXMLFileModified(m_pXMLDOMDocument,bModified);
}
BOOL CXMLDOMFile::GetXMLFileOpenState()
{
	return GetXMLFileOpenState(m_pXMLDOMDocument);
}
void CXMLDOMFile::SetXMLFileOpenState(BOOL bOpened)
{
	SetXMLFileOpenState(m_pXMLDOMDocument,bOpened);
}
CXMLSOMFilePtr CXMLDOMFile::GetXMLFileSchemaFile()
{
	return GetXMLFileSchemaFile(m_pXMLDOMDocument);
}
void CXMLDOMFile::SetXMLFileSchemaFile(CXMLSOMFilePtr pSchemaFile)
{
	return SetXMLFileSchemaFile(m_pXMLDOMDocument,pSchemaFile);
}

CXMLDOMNodePtr CXMLDOMFile::SelectSingleNode(CString xpath)
{
	if(!IsFileOpened())
	{
		return NULL;
	}

	if(xpath.IsEmpty())
	{
		return NULL;
	}

	MSXML2::IXMLDOMNodePtr pXMLDOMNode = NULL;
	try
	{
		pXMLDOMNode = m_pXMLDOMDocument->selectSingleNode((_bstr_t)(LPCTSTR)(xpath));
		if( NULL == pXMLDOMNode )
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML SelectSingleNode() failed."),e);
		return NULL;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML SelectSingleNode() failed."));
		return NULL;
	}


	CXMLDOMNodePtr pRetXMLDOMNode ( new CXMLDOMNode( pXMLDOMNode ) );

	return pRetXMLDOMNode;
}

CXMLDOMNodesPtr CXMLDOMFile::SelectNodes(CString xpath)
{
	if(!IsFileOpened())
	{
		return NULL;
	}

	if(xpath.IsEmpty())
	{
		return NULL;
	}

	MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList = NULL;
	try
	{
		pXMLDOMNodeList = m_pXMLDOMDocument->selectNodes((_bstr_t)(LPCTSTR)(xpath));
		if( NULL == pXMLDOMNodeList )
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML SelectNodes() failed."),e);
		return NULL;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML SelectNodes() failed."));
		return NULL;
	}


	CXMLDOMNodesPtr pRetXMLDOMNodes ( new CXMLDOMNodes( pXMLDOMNodeList ) );

	return pRetXMLDOMNodes;
}

DesBool CXMLDOMFile::TransformByXSL(XSLTCtrl& transCtrl)
{
	DesBool desBool;
	if (!IsFileOpened())
	{
		desBool.result = FALSE;
		desBool.description = _T("The source file not opened.");
		return desBool;
	}
	if (!IsFileExist(transCtrl.xslFileName))
	{
		desBool.result = FALSE;
		desBool.description = _T("The extensible stylesheet language file not exists.");
		return desBool;
	}

	VERIFY(Save().result);

	BOOL result = FALSE;
	VARIANT_BOOL varBool = VARIANT_FALSE;
	HRESULT hr;
	MSXML2::IXMLDOMDocument3Ptr pXSLDoc = NULL;
	MSXML2::IXMLDOMDocument3Ptr pDesDoc = NULL;
	MSXML2::IXSLTemplatePtr pIXSLTemplate = NULL;
	MSXML2::IXSLProcessorPtr pIXSLProcessor = NULL;
	try
	{
		/* ��ʼ����Ҫ��IXSLTemplateģ�� */
		hr = CoCreateInstance(CLSID_XSLTemplate60,NULL,CLSCTX_SERVER,IID_IXSLTemplate,(LPVOID*)(&pIXSLTemplate));
		TESTHR(hr);
		if (!pIXSLTemplate)
		{
			desBool.result = FALSE;
			desBool.description = _T("Create xsl template failed.");
			return desBool;
		}
		/* ��ʼ��XSLT��ʽ�ļ���Ӧ��IXMLDomDocument */
		hr = CoCreateInstance(CLSID_FreeThreadedDOMDocument60,NULL,
			CLSCTX_SERVER,IID_IXMLDOMDocument3,(LPVOID*)(&pXSLDoc));
		TESTHR(hr);
		if (!pXSLDoc)
		{
			desBool.result = FALSE;
			desBool.description = _T("Create xsl document object component for xsl file failed.");
			return desBool;
		}
		/* ���뽫�첽ģʽ�رգ����������ת��ʱ���ļ�δ������ɵĿ��� */
		pXSLDoc->Putasync(VARIANT_FALSE);
		/* ����XSLT��ʽ�ļ����ݣ�����ͨ���ļ�����(ʹ��load)��ͨ���ı�����(ʹ��loadXML) */
		varBool=pXSLDoc->load(_bstr_t(transCtrl.xslFileName));
		if(varBool != VARIANT_TRUE)
		{
			desBool.result = FALSE;
			desBool.description = _T("Load xsl file failed.");
			return desBool;
		}
		/* ����XSL�ļ�����Script�ű� */
		hr = pXSLDoc->setProperty(_T("AllowXsltScript"),VARIANT_TRUE);
		TESTHR(hr);
		/* �����ص�XSLT�ĵ���Ϊģ��ĸ�ʽ�ο��ĵ� */
		pIXSLTemplate->PutRefstylesheet(pXSLDoc);
		/* ��ģ�崴������ */
		pIXSLProcessor=pIXSLTemplate->createProcessor();
		if(!pIXSLProcessor)
		{
			desBool.result = FALSE;
			desBool.description = _T("Create process for xsl transformation failed.");
			return desBool;
		}
		/* Ҫת�����ĵ�ҲҪ�ر��첽ģʽ */
		m_pXMLDOMDocument->Putasync(VARIANT_FALSE);
		/* ��Ҫת����XML�ĵ���Ϊ���̵����� */
		pIXSLProcessor->Putinput(_variant_t ((IUnknown*)m_pXMLDOMDocument));
		/* ��ʼ��һ�����ڴ�Ž��XML���ĵ� */
		hr=CoCreateInstance(CLSID_FreeThreadedDOMDocument60,NULL, CLSCTX_SERVER, 
			IID_IXMLDOMDocument3, (LPVOID*)(&pDesDoc));
		TESTHR(hr);
		if(!pDesDoc)
		{
			desBool.result = FALSE;
			desBool.description = _T("Create document object component for target file failed.");
			return desBool;
		}
		/* ͬ���ر��첽ģʽ */
		pDesDoc->Putasync(VARIANT_FALSE);
		/* �����XML�ĵ���Ϊ���̵������� */
		pIXSLProcessor->put_output(_variant_t((IUnknown*)pDesDoc));
		/* Ϊת���������ò��� */
		XSLParamMapItor paraItor = transCtrl.paraMap.begin();
		while(paraItor != transCtrl.paraMap.end())
		{
			CString paraName = paraItor->first;
			CString paraValue = paraItor->second;
			CString namespaceURI = _T("");
			hr = pIXSLProcessor->
				addParameter(_bstr_t(paraName),_bstr_t(paraValue),_bstr_t(namespaceURI));
			TESTHR(hr);
			paraItor++;
		}
		/* ִ�н��̣�����ת�� */
		varBool=pIXSLProcessor->transform();
		if(varBool == VARIANT_TRUE)
		{
			pDesDoc->save(_bstr_t(transCtrl.desFileName));
			result = TRUE;
		}
		else
		{	
			desBool.result = FALSE;
			desBool.description = _T("Excute transformation failed.");
			return desBool;
		}
	}
	catch (_com_error &e)
	{
		desBool.description = ShowComExceptionWithReason(_T("XML TransformByXSL() failed.\nException thrown."),e);
		desBool.result = FALSE;
		return desBool;
	}
	catch (...)
	{
		desBool.description = ShowComExceptionUnknow(_T("XML TransformByXSL() failed.\nException thrown."));
		desBool.result = FALSE;
		return desBool;
	}
	desBool.result = result;
	desBool.description=_T("Excute transformation succeed.");
	return desBool;
}
/******************************private function***************************/
BOOL CXMLDOMFile::InitXPathQuery(MSXML2::IXMLDOMDocument3Ptr pXMLDoc)
{
	if (!pXMLDoc)
	{
		return FALSE;
	}
	try
	{
		HRESULT hr;
		/* ʹ��XPath��Ϊ���ݲ�ѯ���� */
		CString oldSelLanguage = (_bstr_t)pXMLDoc->getProperty(_T("SelectionLanguage"));
		hr = pXMLDoc->setProperty(_T("SelectionLanguage"),(_bstr_t)_T("XPath"));
		TESTHR(hr);
		/* ����XPath��ѯʹ�õ����ƿռ� */
		MSXML2::IXMLDOMElementPtr pDOMRootElem = pXMLDoc->GetdocumentElement();
		ASSERT(pDOMRootElem != NULL);
		/* �Ը��ڵ�������ռ�ΪXML�ļ���Ĭ�������ռ� */
		/* �����ڵ���ǰ׺���ø��ڵ��Ԫ��ǰ׺��ΪXPath��ѯ��Ԫ�ص�����ǰ׺��
		������Ĭ��ǰ׺ΪXPath��ѯ��Ԫ�ص�����ǰ׺�� */
		CString namespaceURI = (LPCTSTR)(_bstr_t)pDOMRootElem->GetnamespaceURI();
		CString prefix = (LPCTSTR)(_bstr_t)pDOMRootElem->Getprefix();
		if (namespaceURI.IsEmpty() && prefix.IsEmpty())
			return TRUE;		/* XML�ļ�û��Ĭ�������ռ� */
		if (prefix.IsEmpty())
		{
			prefix = STR_XML_DEF_ELEM_PREFIX;
		}
		if (!SetDefaultElementPrefix(prefix))
		{
			return FALSE;
		}
	}
	catch(_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML InitXPathQuery() failed.\nException thrown."),e);
		return FALSE;
	}
	catch(...)
	{
		ShowComExceptionUnknow(_T("XML InitXPathQuery() failed.\nException thrown."));
		return FALSE;
	}
	return TRUE;
}

BOOL CXMLDOMFile::InitXSLTScript(MSXML2::IXMLDOMDocument3Ptr pXMLDoc)
{
	if (!pXMLDoc)
	{
		return FALSE;
	}
	try
	{
		HRESULT hr;
		/* ʹ��XPath��Ϊ���ݲ�ѯ���� */
		VARIANT_BOOL oldProperty = pXMLDoc->getProperty(_T("AllowXsltScript"));
		hr = pXMLDoc->setProperty(_T("AllowXsltScript"),VARIANT_TRUE);
		TESTHR(hr);
	}
	catch(_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML InitXSLTScript() failed.\nException thrown."),e);
		return FALSE;
	}
	catch(...)
	{
		ShowComExceptionUnknow(_T("XML InitXSLTScript() failed.\nException thrown."));
		return FALSE;
	}
	return TRUE;
}

BOOL CXMLDOMFile::InitMultiErrorMessages(MSXML2::IXMLDOMDocument3Ptr pXMLDoc)
{
	if (!pXMLDoc)
	{
		return FALSE;
	}
	try
	{
		HRESULT hr;
		/* ����XML�ĵ��ڷ�����������ʱ���ض��������Ϣ */
		VARIANT_BOOL oldProperty = pXMLDoc->getProperty(_T("MultipleErrorMessages"));
		hr = pXMLDoc->setProperty(_T("MultipleErrorMessages"),VARIANT_TRUE);
		TESTHR(hr);
	}
	catch(_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML InitMultiErrorMessages() failed.\nException thrown."),e);
		return FALSE;
	}
	catch(...)
	{
		ShowComExceptionUnknow(_T("XML InitMultiErrorMessages() failed.\nException thrown."));
		return FALSE;
	}
	return TRUE;
}

BOOL CXMLDOMFile::NormalizeFormat(CString filePath,XCodecType codeType)
{
	// Style for normalization
	char *pszStyle = 0;
	char pszStyleGB2312[]=
		"<?xml version=\"1.0\" encoding=\"UTF-16\"?>\
		<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:fn=\"http://www.w3.org/2005/02/xpath-functions\">\
		<xsl:output method=\"xml\" encoding=\"gb2312\" indent=\"yes\" />\
		<xsl:template match=\"/|@*|node()\">\
		<xsl:copy>\
		<xsl:apply-templates select=\"@*|node()\"/>\
		</xsl:copy>\
		</xsl:template>\
		</xsl:stylesheet>";
	char pszStyleUTF8[]=
		"<?xml version=\"1.0\" encoding=\"UTF-16\"?>\
		<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:fn=\"http://www.w3.org/2005/02/xpath-functions\">\
		<xsl:output method=\"xml\" encoding=\"UTF-8\" indent=\"yes\" />\
		<xsl:template match=\"/|@*|node()\">\
		<xsl:copy>\
		<xsl:apply-templates select=\"@*|node()\"/>\
		</xsl:copy>\
		</xsl:template>\
		</xsl:stylesheet>";
	char pszStyleUTF16[]=
		"<?xml version=\"1.0\" encoding=\"UTF-16\"?>\
		<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:fn=\"http://www.w3.org/2005/02/xpath-functions\">\
		<xsl:output method=\"xml\" encoding=\"UTF-16\" indent=\"yes\" />\
		<xsl:template match=\"/|@*|node()\">\
		<xsl:copy>\
		<xsl:apply-templates select=\"@*|node()\"/>\
		</xsl:copy>\
		</xsl:template>\
		</xsl:stylesheet>";
	switch (codeType)
	{
	case XCodecGB2312:
		pszStyle = (char*)pszStyleGB2312;
		break;
	case XCodecUTF8:
		pszStyle = (char*)pszStyleUTF8;
		break;
	case XCodecUTF16:
		pszStyle = (char*)pszStyleUTF16;
		break;
	default:
		pszStyle = (char*)pszStyleGB2312;
		break;
	}
	HRESULT hr;
	MSXML2::IXMLDOMDocument3Ptr pXMLStyle=NULL;		
	MSXML2::IXMLDOMDocument3Ptr pOld = NULL;
	MSXML2::IXMLDOMDocument3Ptr pNew = NULL;
	MSXML2::IXSLTemplatePtr pIXSLTemplate = NULL;
	MSXML2::IXSLProcessorPtr pIXSLProcessor = NULL;
	VARIANT_BOOL varBool = VARIANT_FALSE;
	
	try
	{
		/* ��ʼ��XSLT��ʽ�ļ���Ӧ��IXMLDomDocument */
		hr = CoCreateInstance(CLSID_FreeThreadedDOMDocument60,NULL,
			CLSCTX_SERVER,IID_IXMLDOMDocument3,(LPVOID*)(&pXMLStyle));
		TESTHR(hr);
		/* ���뽫�첽ģʽ�رգ����������ת��ʱ���ļ�δ������ɵĿ��� */
		pXMLStyle->Putasync(VARIANT_FALSE);
		/* ����XSLT��ʽ�ļ����ݣ�����ͨ���ļ�����(ʹ��load)��ͨ���ı�����(ʹ��loadXML) */
		varBool = pXMLStyle->loadXML(/*_T(*/pszStyle/*)*/);
		if(varBool != VARIANT_TRUE)
		{
			return FALSE;
		}
		/* ��ʼ��ԴXML�ļ���Ӧ��IXMLDomDocument */
		hr = CoCreateInstance(CLSID_FreeThreadedDOMDocument60,NULL,
			CLSCTX_SERVER,IID_IXMLDOMDocument3,(LPVOID*)(&pOld));
		TESTHR(hr);
		/* ���뽫�첽ģʽ�رգ����������ת��ʱ���ļ�δ������ɵĿ��� */
		pOld->Putasync(VARIANT_FALSE);
		/* ����XSLT��ʽ�ļ����ݣ�����ͨ���ļ�����(ʹ��load)��ͨ���ı�����(ʹ��loadXML) */
		varBool = pOld->load((_bstr_t)(filePath));
		if(varBool != VARIANT_TRUE)
		{
			return FALSE;
		}

		/* ��ʼ�����XML�ļ���Ӧ��IXMLDomDocument */
		hr=CoCreateInstance(CLSID_FreeThreadedDOMDocument60, NULL, CLSCTX_SERVER, 
			IID_IXMLDOMDocument3, (LPVOID*)(&pNew));
		TESTHR(hr);
		/* ͬ���ر��첽ģʽ */
		pNew->Putasync( VARIANT_FALSE);

		/* ��ʼ����Ҫ��IXSLTemplateģ�� */
		hr = CoCreateInstance(CLSID_XSLTemplate60,NULL,CLSCTX_SERVER,IID_IXSLTemplate,(LPVOID*)(&pIXSLTemplate));
		TESTHR(hr);
		/* �����ص�XSLT�ĵ���Ϊģ��ĸ�ʽ�ο��ĵ� */
		pIXSLTemplate->PutRefstylesheet(pXMLStyle);
		/* ��ģ�崴������ */
		pIXSLProcessor=pIXSLTemplate->createProcessor();


		/* ��Ҫת����XML�ĵ���Ϊ���̵����� */
		pIXSLProcessor->Putinput(_variant_t ((IUnknown*)pOld));
		/* �����XML�ĵ���Ϊ���̵������� */
		pIXSLProcessor->put_output(_variant_t((IUnknown*)pNew));
		/* ִ�н��̣�����ת�� */
		varBool=pIXSLProcessor->transform();
		if(varBool != VARIANT_TRUE)
		{
			return FALSE;
		}
		hr = pNew->save((_bstr_t)(filePath));
		TESTHR(hr);
	}
	catch(_com_error &e)
	{
		ShowComExceptionWithReason(_T("XML NormalizeFormat() failed.\nException thrown."),e);
		return FALSE;
	}
	catch(...)
	{
		ShowComExceptionUnknow(_T("XML NormalizeFormat() failed.\nException thrown."));
		return FALSE;
	}
	return TRUE;
}

void CXMLDOMFile::AddToFilePropertyMap(MSXML2::IXMLDOMDocument3Ptr domDoc)
{
	XFilePropertyMapItor itor = m_filePropertyMap.find(domDoc);
	if (itor == m_filePropertyMap.end())
	{
		XFileProperty fileProperty;
		m_filePropertyMap.insert(std::make_pair(domDoc,fileProperty));
	}
}
void CXMLDOMFile::RemoveFromFilePropertyMap(MSXML2::IXMLDOMDocument3Ptr domDoc)
{
	XFilePropertyMapItor itor = m_filePropertyMap.find(domDoc);
	if (itor != m_filePropertyMap.end())
	{
		m_filePropertyMap.erase(itor);
	}
}

/*******************************************************************************/
CString  CXMLDOMFile::GetXMLFilePath(MSXML2::IXMLDOMDocument3Ptr pDomDoc)
{
	CString filePath;
	if (pDomDoc == NULL)
	{
		return filePath;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		filePath = itor->second.filePath;
	}
	else
	{
		ASSERT(0);
	}
	return filePath;
}
void CXMLDOMFile::SetXMLFilePath(MSXML2::IXMLDOMDocument3Ptr pDomDoc,CString filePath)
{
	if (pDomDoc == NULL)
	{
		return;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		itor->second.filePath = filePath;
	}
	else
	{
		ASSERT(0);
	}
}
XCodecType  CXMLDOMFile::GetXMLFileCodeType(MSXML2::IXMLDOMDocument3Ptr pDomDoc)
{
	XCodecType codeType = XCodecUnknown;	
	if (pDomDoc == NULL)
	{
		return codeType;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		codeType = itor->second.codeType;
	}
	else
	{
		ASSERT(0);
	}
	return codeType;
}
void CXMLDOMFile::SetXMLFileCodeType(MSXML2::IXMLDOMDocument3Ptr pDomDoc,XCodecType codeType)
{
	if (pDomDoc == NULL)
	{
		return;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		itor->second.codeType = codeType;
	}
	else
	{
		ASSERT(0);
	}
}
BOOL CXMLDOMFile::GetXMLFileModified(MSXML2::IXMLDOMDocument3Ptr pDomDoc)
{
	BOOL bModified = FALSE;
	if (pDomDoc == NULL)
	{
		return pDomDoc;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		bModified = itor->second.bModified;
	}
	return bModified;
}
void CXMLDOMFile::SetXMLFileModified(MSXML2::IXMLDOMDocument3Ptr pDomDoc,BOOL bModified)
{
	if (pDomDoc == NULL)
	{
		return;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		itor->second.bModified = bModified;
	}
	else
	{
		ASSERT(0);
	}
}
BOOL CXMLDOMFile::GetXMLFileOpenState(MSXML2::IXMLDOMDocument3Ptr pDomDoc)
{
	BOOL openState = FALSE;
	if (pDomDoc == NULL)
	{
		return openState;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		openState = itor->second.bOpened;
	}
	return openState;
}
void CXMLDOMFile::SetXMLFileOpenState(MSXML2::IXMLDOMDocument3Ptr pDomDoc,BOOL bOpened)
{
	if (pDomDoc == NULL)
	{
		return;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		itor->second.bOpened = bOpened;
	}
	else
	{
		ASSERT(0);
	}
}
CXMLSOMFilePtr CXMLDOMFile::GetXMLFileSchemaFile(MSXML2::IXMLDOMDocument3Ptr pDomDoc)
{
	CXMLSOMFilePtr pSchemaFile = NULL;
	if (pDomDoc == NULL)
	{
		return pSchemaFile;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if (itor != m_filePropertyMap.end())
	{
		pSchemaFile = itor->second.schemaFile;
	}
	return pSchemaFile;
}
void CXMLDOMFile::SetXMLFileSchemaFile(MSXML2::IXMLDOMDocument3Ptr pDomDoc,CXMLSOMFilePtr pSchemaFile)
{
	if (pDomDoc == NULL)
	{
		return;
	}
	XFilePropertyMapItor itor = m_filePropertyMap.find(pDomDoc);
	if(itor !=m_filePropertyMap.end())
	{
		itor->second.schemaFile = pSchemaFile;
	}
	else
	{
		ASSERT(0);
	}
}