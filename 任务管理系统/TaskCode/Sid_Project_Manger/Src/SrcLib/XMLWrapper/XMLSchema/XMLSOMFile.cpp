#include "StdAfx.h"
#include "XMLSOMFile.h"

#define RETURN_FALSE_IF_FALSE(ret)\
	if (ret == FALSE)\
{\
	return FALSE;\
}
#define RETURN_FALSE_IF_NULL(ret)\
	if (ret == NULL)\
{\
	return FALSE;\
}


CXMLSOMFile::CXMLSOMFile(void)
{
	m_pSchemaCollection = NULL;
}
CXMLSOMFile::~CXMLSOMFile(void)
{
}
MSXML2::IXMLDOMSchemaCollection2Ptr CXMLSOMFile::GetInnerPtr()
{
	if (IsFileOpened())
	{
		return m_pSchemaCollection;
	}
	else
	{
		return NULL;
	}
}
DesBool CXMLSOMFile::Open(CString filePath,CString namespaceURI)
{
	DesBool desRet;
	if (filePath.IsEmpty())
		RETURN_DESBOOL_FALSE(desRet,"The name of file must not be empty.");
	if (!IsFileExist(filePath))
		RETURN_DESBOOL_FALSE(desRet,"The file not exists.");
	if (IsFileOpened())
		RETURN_DESBOOL_FALSE(desRet,"The file opened already,close it first.");
	HRESULT hr;
	try
	{
		hr = m_pSchemaCollection.CreateInstance(__uuidof(MSXML2::XMLSchemaCache60));
		TESTHR(hr);
		hr = m_pSchemaCollection->add((_bstr_t)(LPCTSTR)namespaceURI,(_bstr_t)(LPCTSTR)filePath);
		TESTHR(hr);
	}
	catch (_com_error& e)
	{
		CString desc = ShowComExceptionWithReason(_T("SOM Open() failed."),e);
		RETURN_DESBOOL_FALSE(desRet,desc);
	}
	catch (...)
	{
		CString desc = ShowComExceptionUnknow(_T("SOM Open() failed."));
		RETURN_DESBOOL_FALSE(desRet,desc);
	}
	m_filePath = filePath;
	m_namespaceURI = namespaceURI;

	RETURN_DESBOOL_TRUE(desRet,_T("The file open succeed."));
	return desRet;
}
DesBool CXMLSOMFile::Close()
{
	DesBool desRet;
	if (!IsFileOpened())
	{
		RETURN_DESBOOL_FALSE(desRet,_T("The file is not open."));
	}
	else
	{		
		m_filePath.Empty();
		m_namespaceURI.Empty();
		m_pSchemaCollection = NULL;
		RETURN_DESBOOL_TRUE(desRet,_T("The file close succed."));
	}
}

BOOL CXMLSOMFile::IsFileOpened()
{
	if (m_pSchemaCollection != NULL 
		&& !m_filePath.IsEmpty() && !m_namespaceURI.IsEmpty())
	{
		return TRUE;
	}
	else if (m_pSchemaCollection == NULL 
		&& m_filePath.IsEmpty() && m_namespaceURI.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
}

CString CXMLSOMFile::GetFileName()
{
	return m_filePath;
}
CString CXMLSOMFile::GetFileNamespace()
{
	return m_namespaceURI;
}
/******************************private function***************************/
BOOL CXMLSOMFile::IsFileExist( CString strFileName )
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

//BOOL CXMLSOMFile::_processSchema(MSXML2::IXMLDOMSchemaCollection2Ptr pSchemaCollection,CString namespaceURI)
//{
//	if (pSchemaCollection == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = TRUE;
//	try
//	{
//		MSXML2::ISchemaPtr pSchema = pSchemaCollection->getSchema((_bstr_t)(LPCTSTR)namespaceURI);
//#ifdef _DEBUG
//		CString itemName = (LPCTSTR)(_bstr_t)pSchema->Getname();
//		MSXML2::SOMITEMTYPE itemType = pSchema->GetitemType();
//		ASSERT(itemType == MSXML2::SOMITEM_SCHEMA);
//#endif
//		/* 遍历元素 */
//		MSXML2::ISchemaItemCollectionPtr pSchemaElementCollection = pSchema->Getelements();
//		long elementCount = pSchemaElementCollection->Getlength();
//		for (long i = 0; i < elementCount; i++)
//		{
//			MSXML2::ISchemaItemPtr pSchemaElement = pSchemaElementCollection->Getitem(i);
//#ifdef _DEBUG
//			CString elementName = (LPCTSTR)(_bstr_t)pSchemaElement->Getname();
//#endif
//			_processElement(pSchemaElement);
//		}
//		/* 遍历类型 */
//		MSXML2::ISchemaItemCollectionPtr pSchemaTypeCollection = pSchema->Gettypes();
//		long typeCount = pSchemaTypeCollection->Getlength();
//		for (long i = 0; i < typeCount; i++)
//		{
//			MSXML2::ISchemaTypePtr pSchemaType = pSchemaTypeCollection->Getitem(i);
//#ifdef _DEBUG
//			CString typeName = (LPCTSTR)(_bstr_t)pSchemaType->Getname();
//#endif
//			//_processType(pSchemaType);
//		}
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processSchema() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processSchema() failed."));
//		return FALSE;
//	}
//	return result;
//}
//
//BOOL CXMLSOMFile::_processElement(MSXML2::ISchemaElementPtr pSchemaElement)
//{
//	if (pSchemaElement == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = TRUE;
//	try
//	{
//#ifdef _DEBUG
//		CString elementName = (LPCTSTR)(_bstr_t)pSchemaElement->Getname();
//		if (elementName == _T("SDI"))
//		{
//			Sleep(1);
//		}
//		ASSERT(MSXML2::SOMITEM_ELEMENT == pSchemaElement->GetitemType());
//#endif
//		/* 检查元素是否是引用 */
//		if(VARIANT_TRUE == pSchemaElement->GetisReference())
//		{
//			return TRUE;
//		}
//		else
//		{
//			if (_isProcessedElement(pSchemaElement))
//			{
//				return TRUE;
//			}
//			MSXML2::ISchemaItemCollectionPtr pSchemaIdentityConstraintCollection = 
//				pSchemaElement->GetidentityConstraints();
//			long identityConstraintCount = pSchemaIdentityConstraintCollection->Getlength();
//			for (long i = 0; i < identityConstraintCount; i++)
//			{
//				MSXML2::ISchemaIdentityConstraintPtr pSchemaIdentityConstraint = 
//					pSchemaIdentityConstraintCollection->Getitem(i);
//				if (pSchemaIdentityConstraint->GetreferencedKey() == NULL)
//				{
//					result = _processKeyConstraint(pSchemaIdentityConstraint);
//				}
//				else
//				{
//					result = _processKeyrefConstraint(pSchemaIdentityConstraint);
//				}
//				RETURN_FALSE_IF_FALSE(result);
//			}
//
//			MSXML2::ISchemaTypePtr pSchemaType = pSchemaElement->Gettype();
//#ifdef _DEBUG
//			CString typeName = pSchemaType->Getname();
//#endif
//			result = _processType(pSchemaType);
//			RETURN_FALSE_IF_FALSE(result);
//
//
//			result = _addProcessedElement(pSchemaElement);
//			RETURN_FALSE_IF_FALSE(result);
//			
//		}
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processElement() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processElement() failed."));
//		return FALSE;
//	}
//	return result;
//}
//
//BOOL CXMLSOMFile::_processType(MSXML2::ISchemaTypePtr pSchemaType)
//{
//	if (pSchemaType == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = TRUE;
//	try
//	{
//#ifdef _DEBUG
//		CString typeName = (LPCTSTR)(_bstr_t)pSchemaType->Getname();
//#endif
//		MSXML2::SOMITEMTYPE schemaType = pSchemaType->GetitemType();
//		if(MSXML2::SOMITEM_SIMPLETYPE == schemaType)
//		{
//			result = _processSimpleType(pSchemaType);
//		}
//		else if(MSXML2::SOMITEM_COMPLEXTYPE == schemaType)
//		{
//			result = _processComplexType(pSchemaType);
//		}
//		else if (MSXML2::SOMITEM_DATATYPE == (schemaType&MSXML2::SOMITEM_DATATYPE))
//		{
//			result = TRUE;
//		}
//		else
//		{
//			ASSERT(0);
//			result = FALSE;
//		}
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processType() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processType() failed."));
//		return FALSE;
//	}
//	return result;
//}
//BOOL CXMLSOMFile::_processSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType)
//{
//	if (pSchemaSimpleType == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = TRUE;
//	try
//	{
//		CString typeName = (LPCTSTR)(_bstr_t)pSchemaSimpleType->Getname();
//		ASSERT(MSXML2::SOMITEM_SIMPLETYPE == pSchemaSimpleType->GetitemType());
//
//		if (_isProcessedSimpleType(pSchemaSimpleType))
//		{
//			return TRUE;
//		}
//
//		result = _addProcessedSimpleType(pSchemaSimpleType);
//		RETURN_FALSE_IF_FALSE(result);
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processSimpleType() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processSimpleType() failed."));
//		return FALSE;
//	}
//	return result;
//}
//BOOL CXMLSOMFile::_processComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType)
//{
//	if (pSchemaComplexType == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = TRUE;
//	try
//	{
//		CString typeName = (LPCTSTR)(_bstr_t)pSchemaComplexType->Getname();
//		if (typeName == _T("tService"))
//		{
//			Sleep(1);
//		}
//		ASSERT(MSXML2::SOMITEM_COMPLEXTYPE == pSchemaComplexType->GetitemType());
//
//		if (_isProcessedComplexType(pSchemaComplexType))
//		{
//			return TRUE;
//		}
//		MSXML2::ISchemaModelGroupPtr pSchemaModelGroup = pSchemaComplexType->GetcontentModel();
//		MSXML2::SCHEMACONTENTTYPE contentType = pSchemaComplexType->GetcontentType();
//		switch (contentType)
//		{
//		case MSXML2::SCHEMACONTENTTYPE_EMPTY:
//		case MSXML2::SCHEMACONTENTTYPE_TEXTONLY:
//			/* 没有子元素 */
//			result = TRUE;	
//			break;
//		case MSXML2::SCHEMACONTENTTYPE_ELEMENTONLY:
//		case MSXML2::SCHEMACONTENTTYPE_MIXED:
//			{
//				/* 具有子元素，递归处理子元素 */
//				MSXML2::SCHEMADERIVATIONMETHOD derivationMethod = pSchemaComplexType->GetderivedBy();
//				switch (derivationMethod)
//				{
//				case SCHEMADERIVATIONMETHOD_EMPTY:
//					result = TRUE;
//					break;
//				case SCHEMADERIVATIONMETHOD_SUBSTITUTION:
//					result = TRUE;
//					break;
//				case SCHEMADERIVATIONMETHOD_EXTENSION:
//				case SCHEMADERIVATIONMETHOD_RESTRICTION:
//					result = _processModelGroup(pSchemaModelGroup);
//					break;
//				case SCHEMADERIVATIONMETHOD_LIST:
//				case SCHEMADERIVATIONMETHOD_UNION:
//					{
//
//					}
//					break;
//				case SCHEMADERIVATIONMETHOD_ALL:
//					result = _processModelGroup(pSchemaModelGroup);
//					break;
//				case SCHEMADERIVATIONMETHOD_NONE:
//					ASSERT(0);	/* 未处理情况 */
//					result = TRUE;
//					break;
//				default:
//					result = FALSE;
//					break;
//				}
//			}
//			result = TRUE;
//			break;
//		default:
//			result = FALSE;
//			break;
//		}
//		RETURN_FALSE_IF_FALSE(result);
//		result = _addProcessedComplexType(pSchemaComplexType);
//		RETURN_FALSE_IF_FALSE(result);
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processComplexType() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processComplexType() failed."));
//		return FALSE;
//	}
//	return result;
//}
//
//BOOL CXMLSOMFile::_processModelGroup(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
//{
//	if (pSchemaModelGroup == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = TRUE;
//	try
//	{
//		MSXML2::ISchemaItemCollectionPtr pParticlesCollection = pSchemaModelGroup->Getparticles();
//		long particlesCount = pParticlesCollection->Getlength();
//		for (long i = 0; i < particlesCount; i++)
//		{
//			MSXML2::ISchemaItemPtr pParticles = pParticlesCollection->Getitem(i);
//#ifdef _DEBUG
//			CString particleName = (LPCTSTR)(_bstr_t)pParticles->Getname();
//#endif
//			MSXML2::SOMITEMTYPE particleType = pParticles->GetitemType();
//			switch (particleType)
//			{
//			case SOMITEM_ANY:
//				result = TRUE;
//				break;
//			case SOMITEM_ELEMENT:
//				result = _processElement(pParticles);
//				break;
//			case SOMITEM_ALL:
//			case SOMITEM_CHOICE:
//			case SOMITEM_SEQUENCE:
//				result = _processModelGroup(pParticles);
//				break;
//			case SOMITEM_EMPTYPARTICLE:
//				result = TRUE;		/* 空情况 */
//				break;
//			default:
//				result = FALSE;		/* 未处理情况 */
//				break;
//			}
//		}
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processModelGroup() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processModelGroup() failed."));
//		return FALSE;
//	}
//	return result;
//}
//
//BOOL CXMLSOMFile::_processKeyConstraint(MSXML2::ISchemaIdentityConstraintPtr pSchemaKeyConstraint)
//{
//	if (pSchemaKeyConstraint == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = FALSE;
//	try
//	{
//		if (_isProcessedKey(pSchemaKeyConstraint))
//		{
//			return TRUE;
//		}
//		CString keyName = (LPCTSTR)(_bstr_t)pSchemaKeyConstraint->Getname();
//		CString selector =(LPCTSTR)(_bstr_t) pSchemaKeyConstraint->Getselector();
//		MSXML2::ISchemaStringCollectionPtr pFieldsCollection = pSchemaKeyConstraint->Getfields();
//		long fieldsCount = pFieldsCollection->Getlength();
//		for (long i = 0 ; i < fieldsCount; i ++)
//		{
//			CString fields = (LPCTSTR)(_bstr_t)pFieldsCollection->Getitem(i);
//		}
//		result = _addProcessedKey(pSchemaKeyConstraint);
//		RETURN_FALSE_IF_FALSE(result);
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processKeyConstraint() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processKeyConstraint() failed."));
//		return FALSE;
//	}
//	return result;
//}
//BOOL CXMLSOMFile::_processKeyrefConstraint(MSXML2::ISchemaIdentityConstraintPtr pSchemaKeyrefConstraint)
//{
//	if (pSchemaKeyrefConstraint == NULL)
//	{
//		return FALSE;
//	}
//	BOOL result = FALSE;
//	try
//	{
//		if (_isProcessedKeyref(pSchemaKeyrefConstraint))
//		{
//			return TRUE;
//		}
//		CString keyrefName = (LPCTSTR)(_bstr_t)pSchemaKeyrefConstraint->Getname();
//		CString selector =(LPCTSTR)(_bstr_t) pSchemaKeyrefConstraint->Getselector();
//		MSXML2::ISchemaStringCollectionPtr pFieldsCollection = pSchemaKeyrefConstraint->Getfields();
//		long fieldsCount = pFieldsCollection->Getlength();
//		for (long i = 0 ; i < fieldsCount; i ++)
//		{
//			CString fields = (LPCTSTR)(_bstr_t)pFieldsCollection->Getitem(i);
//		}
//		result = _addProcessedKeyref(pSchemaKeyrefConstraint);
//		RETURN_FALSE_IF_FALSE(result);
//	}
//	catch (_com_error& e)
//	{
//		ShowComExceptionWithReason(_T("SOM _processKeyrefConstraint() failed."),e);
//		return FALSE;
//	}
//	catch (...)
//	{
//		ShowComExceptionUnknow(_T("SOM _processKeyrefConstraint() failed."));
//		return FALSE;
//	}
//	return result;
//}
//
//BOOL CXMLSOMFile::_addProcessedElement(MSXML2::ISchemaElementPtr pSchemaElement)
//{
//	if (pSchemaElement == NULL)
//	{
//		return FALSE;
//	}
//	if (!_isProcessedElement(pSchemaElement))
//	{
//		CString elementName = (LPCTSTR)(_bstr_t)pSchemaElement->Getname();
//		m_processedElementMap.insert(std::make_pair(pSchemaElement,elementName));
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//BOOL CXMLSOMFile::_isProcessedElement(MSXML2::ISchemaElementPtr pSchemaElement)
//{
//	if (pSchemaElement == NULL)
//	{
//		return FALSE;
//	}
//#ifdef _DEBUG
//	CString elementName = (LPCTSTR)(_bstr_t)pSchemaElement->Getname();
//#endif
//	ProcessedElementMapItor itor = m_processedElementMap.find(pSchemaElement);
//	if (itor != m_processedElementMap.end())
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//BOOL CXMLSOMFile::_addProcessedSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType)
//{
//	if (pSchemaSimpleType == NULL)
//	{
//		return FALSE;
//	}
//	if (!_isProcessedSimpleType(pSchemaSimpleType))
//	{
//		CString typeName = (LPCTSTR)(_bstr_t)pSchemaSimpleType->Getname();
//		m_processedSimpleTypeMap.insert(std::make_pair(pSchemaSimpleType,typeName));
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//BOOL CXMLSOMFile::_isProcessedSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType)
//{
//	if (pSchemaSimpleType == NULL)
//	{
//		return FALSE;
//	}
//#ifdef _DEBUG
//	CString typeName = (LPCTSTR)(_bstr_t)pSchemaSimpleType->Getname();
//#endif
//	ProcessedSimpleTypeMapItor itor = m_processedSimpleTypeMap.find(pSchemaSimpleType);
//	if (itor != m_processedSimpleTypeMap.end())
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//BOOL CXMLSOMFile::_addProcessedComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType)
//{
//	if (pSchemaComplexType == NULL)
//	{
//		return FALSE;
//	}
//	if (!_isProcessedComplexType(pSchemaComplexType))
//	{
//		CString typeName = (LPCTSTR)(_bstr_t)pSchemaComplexType->Getname();
//		m_processedComplexTypeMap.insert(std::make_pair(pSchemaComplexType,typeName));
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//BOOL CXMLSOMFile::_isProcessedComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType)
//{
//	if (pSchemaComplexType == NULL)
//	{
//		return FALSE;
//	}
//#ifdef _DEBUG
//	CString typeName = (LPCTSTR)(_bstr_t)pSchemaComplexType->Getname();
//#endif
//	ProcessedComplexTypeMapItor itor = m_processedComplexTypeMap.find(pSchemaComplexType);
//	if (itor != m_processedComplexTypeMap.end())
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//BOOL CXMLSOMFile::_addProcessedKey(MSXML2::ISchemaIdentityConstraintPtr pSchemaKey)
//{
//	if (pSchemaKey == NULL)
//	{
//		return FALSE;
//	}
//	CString keyName = (LPCTSTR)(_bstr_t)pSchemaKey->Getname();
//	if (!_isProcessedKey(pSchemaKey))
//	{
//		m_processedKeyMap.insert(std::make_pair(pSchemaKey,keyName));
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//BOOL CXMLSOMFile::_isProcessedKey(MSXML2::ISchemaIdentityConstraintPtr pSchemaKey)
//{
//	if (pSchemaKey == NULL)
//	{
//		return TRUE;
//	}
//	ProcessedKeyMapItor itor = m_processedKeyMap.find(pSchemaKey);
//	if (itor != m_processedKeyMap.end())
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//BOOL CXMLSOMFile::_addProcessedKeyref(MSXML2::ISchemaIdentityConstraintPtr pSchemaKeyref)
//{
//	if (pSchemaKeyref == NULL)
//	{
//		return FALSE;
//	}
//	CString keyrefName = (LPCTSTR)(_bstr_t)pSchemaKeyref->Getname();
//	if (!_isProcessedKeyref(pSchemaKeyref))
//	{
//		m_processedKeyrefMap.insert(std::make_pair(pSchemaKeyref,keyrefName));
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//BOOL CXMLSOMFile::_isProcessedKeyref(MSXML2::ISchemaIdentityConstraintPtr pSchemaKeyref)
//{
//	if (pSchemaKeyref == NULL)
//	{
//		return TRUE;
//	}
//	ProcessedKeyMapItor itor = m_processedKeyrefMap.find(pSchemaKeyref);
//	if (itor != m_processedKeyrefMap.end())
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}