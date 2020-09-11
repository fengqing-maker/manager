#include "StdAfx.h"
#include "XMLSOMAttribute.h"

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



CXMLSOMAttribute::CXMLSOMAttribute(void)
{
}
CXMLSOMAttribute::~CXMLSOMAttribute(void)
{
}

BOOL CXMLSOMAttribute::InitConstraint
	(MSXML2::IXMLDOMElementPtr pXMLDOMElement,MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute)
{
	if (pXMLDOMElement == NULL||pXMLDOMAttribute == NULL)
	{
		return FALSE;
	}

	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString elemName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
		CString attrName = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->GetnodeName();
#endif
		m_pXMLDOMElement = pXMLDOMElement;
		m_pXMLDOMAttribute = pXMLDOMAttribute;
		result = _initWithElement(pXMLDOMElement);
		RETURN_FALSE_IF_FALSE(result);
		result = _initWithAttribute(pXMLDOMAttribute);
		RETURN_FALSE_IF_FALSE(result);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute InitConstraint() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute InitConstraint() failed."));
		return FALSE;
	}
	return result;
}

XMLStringList CXMLSOMAttribute::GetAttributeValueRefList()
{
	return m_valueRefList;
}
XMLStringList CXMLSOMAttribute::GetAttributeValueEnumList()
{
	return m_valueEnumList; 
}
/******************************private function***************************/
BOOL CXMLSOMAttribute::_initWithElement(MSXML2::IXMLDOMElementPtr pXMLDOMElement)
{
	if (pXMLDOMElement == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString elementName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
#endif
		MSXML2::ISchemaItemPtr pSchemaItem = NULL;
		MSXML2::ISchemaElementPtr pSchemaElement = NULL;
			
		do
		{
			pSchemaItem = _getDeclaration(pXMLDOMElement);
			if (pSchemaItem != NULL)
			{
				pSchemaElement = pSchemaItem;
#ifdef _DEBUG
				elementName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
				CString schemaElemName = (LPCTSTR)(_bstr_t)pSchemaElement->Getname();
#endif
				MSXML2::ISchemaItemCollectionPtr pIdentityCollection =
					pSchemaElement->GetidentityConstraints();
				long identityCount = pIdentityCollection->Getlength();
				for (long i = 0; i < identityCount; i++)
				{
					MSXML2::ISchemaIdentityConstraintPtr pIdentity = 
						pIdentityCollection->Getitem(i);
					if (pIdentity->GetreferencedKey() != NULL)
					{
						result = _processKeyref(pXMLDOMElement,pIdentity);
						RETURN_FALSE_IF_FALSE(result);
					}
				}
			}
			else
			{
				return FALSE;
			}
			pXMLDOMElement = pXMLDOMElement->GetparentNode();
		}while (pXMLDOMElement != NULL);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _initWithElement() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _initWithElement() failed."));
		return FALSE;
	}
	return TRUE;
}
BOOL CXMLSOMAttribute::_initWithAttribute(MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute)
{
	if (pXMLDOMAttribute == NULL)
	{
		return FALSE;
	}
	
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString attrName = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->Getname();
#endif
		MSXML2::ISchemaItemPtr pSchemaItem = NULL;
		MSXML2::ISchemaItemPtr pSchemaAttribute = NULL;
		pSchemaItem = _getDeclaration(pXMLDOMAttribute);
		if (pSchemaItem != NULL)
		{
			pSchemaAttribute = pSchemaItem;
#ifdef _DEBUG
			CString schemaAttrName = (LPCTSTR)(_bstr_t)pSchemaAttribute->Getname();
#endif
			m_pSchemaAttribute = pSchemaAttribute;
			result = _processAttribute(m_pSchemaAttribute);
			RETURN_FALSE_IF_FALSE(result);
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _initWithAttribute() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _initWithAttribute() failed."));
		return FALSE;
	}
	return result;
}

BOOL CXMLSOMAttribute::_processKeyref
	(MSXML2::IXMLDOMElementPtr pXMLDOMElement,MSXML2::ISchemaIdentityConstraintPtr pKeyref)
{
	if (pKeyref == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString elementName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
		CString keyrefName = (LPCTSTR)(_bstr_t)pKeyref->Getname();
#endif
		/* 获取引用约束节点列表 */
		CString refSelector = (LPCTSTR)(_bstr_t)pKeyref->Getselector();
		CString refFields = _joinIdentityConstraintXPath(pKeyref);
		MSXML2::IXMLDOMNodeListPtr pRefElementList = 
			pXMLDOMElement->selectNodes((_bstr_t)(LPCTSTR)refSelector);
		MSXML2::IXMLDOMNodeListPtr pRefAttributeList = 
			pXMLDOMElement->selectNodes((_bstr_t)(LPCTSTR)refFields);
		if (_isDOMNodeInDONNodeList(m_pXMLDOMElement,pRefElementList)
			&& _isDOMNodeInDONNodeList(m_pXMLDOMAttribute,pRefAttributeList))
		{
			/* 当前元素节点在引用约束范围内 */
			MSXML2::ISchemaStringCollectionPtr pRefFieldCollection = 
				pKeyref->Getfields();
			long refFieldCount = pRefFieldCollection->Getlength();

			MSXML2::ISchemaIdentityConstraintPtr pKey = pKeyref->GetreferencedKey();
#ifdef _DEBUG
			CString keyName = (LPCTSTR)(_bstr_t)pKey->Getname();
#endif
			CString keySelector = (LPCTSTR)(_bstr_t)pKey->Getselector();
			MSXML2::ISchemaStringCollectionPtr pKeyFieldCollection = 
				pKey->Getfields();
			long keyFieldCount = pKeyFieldCollection->Getlength();
			ASSERT(refFieldCount == keyFieldCount);

			MSXML2::IXMLDOMNodeListPtr pKeyElementList = 
				pXMLDOMElement->selectNodes((_bstr_t)(LPCTSTR)keySelector);

			long keyElementCount = pKeyElementList->Getlength();
			for (long i = 0; i < keyElementCount; i++)
			{
				MSXML2::IXMLDOMElementPtr pKeyElement = pKeyElementList->Getitem(i);
				ASSERT(pKeyElement != NULL);
				CString text;
				for (long j = 0; j < refFieldCount; j++)
				{
					CString refField = (LPCTSTR)(_bstr_t)pRefFieldCollection->Getitem(j);
					CString refAttrName = GetAttributeNameFromXPath(refField);
					CString keyField = (LPCTSTR)(_bstr_t)pKeyFieldCollection->Getitem(j);
					CString keyAttrName = GetAttributeNameFromXPath(keyField);
					MSXML2::IXMLDOMAttributePtr pKeyAttribute = 
						pKeyElement->getAttributeNode((_bstr_t)(LPCTSTR)keyAttrName);
					CString keyAttrValue;
					if (pKeyAttribute != NULL)
					{
						keyAttrValue = (LPCTSTR)(_bstr_t)pKeyAttribute->GetnodeValue();
					}
					text += refAttrName + STR_TOKEN_EQUAL + keyAttrValue;
					if (j != refFieldCount - 1)
					{
						text += STR_TOKEN_SEPARATOR;
					}
				}
				m_valueRefList.push_back(text);
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _processKeyref() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _processKeyref() failed."));
		return FALSE;
	}
	return result;
}

BOOL CXMLSOMAttribute::_processAttribute(MSXML2::ISchemaAttributePtr pSchemaAttribute)
{
	if (pSchemaAttribute == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		CString attrName = pSchemaAttribute->Getname();
#endif
		MSXML2::ISchemaTypePtr pAttributeType = pSchemaAttribute->Gettype();
		MSXML2::SOMITEMTYPE attributeType = pAttributeType->GetitemType();
		/* all attributes are declared as simple types or build-in types */
		if (attributeType == MSXML2::SOMITEM_SIMPLETYPE)
		{
			result = _processSimpleType(pAttributeType);
		}
		else if (attributeType == MSXML2::SOMITEM_COMPLEXTYPE)
		{
			ASSERT(0);
			result = FALSE;
		}
		else if (MSXML2::SOMITEM_DATATYPE == (attributeType&MSXML2::SOMITEM_DATATYPE))
		{
			result = TRUE;
		}
		else
		{
			ASSERT(0);
			result = FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _processAttribute() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _processAttribute() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMAttribute::_processSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType)
{
	if (pSchemaSimpleType == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		CString simpleTypeName = pSchemaSimpleType->Getname();
		MSXML2::SOMITEMTYPE itemType = pSchemaSimpleType->GetitemType();
		ASSERT(MSXML2::SOMITEM_SIMPLETYPE == itemType);

		MSXML2::SCHEMADERIVATIONMETHOD derivationMethod = pSchemaSimpleType->GetderivedBy();
		switch (derivationMethod)
		{
		case MSXML2::SCHEMADERIVATIONMETHOD_RESTRICTION:
			{
				MSXML2::ISchemaStringCollectionPtr pSchemaEnumCollection = 
					pSchemaSimpleType->Getenumeration();
				long enumCount = pSchemaEnumCollection->Getlength();
				if (enumCount != 0)	
				{
					/* 若正在解析的类型信息中包含有enum值，记录下enum值，不再向base类型内解析 */
					for (long i = 0; i < enumCount; i++)
					{
						CString enumValue = (LPCTSTR)(_bstr_t)pSchemaEnumCollection->Getitem(i);
						m_valueEnumList.push_back(enumValue);
					}
					result = TRUE;
				}
				else
				{
					/* 若正在解析的类型信息中没有enum值，解析base类型信息 */
					MSXML2::ISchemaItemCollectionPtr pBaseTypeCollection = pSchemaSimpleType->GetbaseTypes();
					long baseTypeCount = pBaseTypeCollection->Getlength();
					ASSERT(baseTypeCount == 1);
					MSXML2::ISchemaItemPtr pBaseType = pBaseTypeCollection->Getitem(0);
					MSXML2::SOMITEMTYPE baseType = pBaseType->GetitemType();
					if (MSXML2::SOMITEM_SIMPLETYPE == baseType)
					{
						/* base基类型是简单类型，向内递归解析 */
						result = _processSimpleType(pBaseType);	
						RETURN_FALSE_IF_FALSE(result);
					}
					else if (MSXML2::SOMITEM_DATATYPE == (baseType&MSXML2::SOMITEM_DATATYPE)) 
					{
						/* base基类型是基本数据类型，无法向内递归 */
						result = TRUE;
					}
					else
					{
						/* 不应出现的base基类型 */
						ASSERT(0);
						result = FALSE;
					}
				}
			}
			break;
		case MSXML2::SCHEMADERIVATIONMETHOD_UNION:
			{
				/* 解析union联合的类型信息 */
				MSXML2::ISchemaItemCollectionPtr pBaseTypeCollection = pSchemaSimpleType->GetbaseTypes();
				long baseTypeLen = pBaseTypeCollection->Getlength();
				for (long i = 0; i < baseTypeLen; i++)
				{
					MSXML2::ISchemaItemPtr pBaseType = pBaseTypeCollection->Getitem(i);
					if(MSXML2::SOMITEM_SIMPLETYPE == pBaseType->GetitemType())
					{
						result = _processSimpleType(pBaseType);	/* 联合类型是简单类型，向内递归解析 */	
						RETURN_FALSE_IF_FALSE(result);
					}
				}
				result = TRUE;
			}
			break;
		default:
			/* 其他约束方式，未处理 */
			ASSERT(0);
			result = TRUE;
			break;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _processSimpleType() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _processSimpleType() failed."));
		return FALSE;
	}
	return result;
}

CString CXMLSOMAttribute::_joinIdentityConstraintXPath(MSXML2::ISchemaIdentityConstraintPtr pIdentity)
{
	CString xpath;
	if (pIdentity == 0)
	{
		return xpath;
	}
	try
	{
		CString selector = (LPCTSTR)(_bstr_t)pIdentity->Getselector();
		MSXML2::ISchemaStringCollectionPtr pFieldsCollection = pIdentity->Getfields();
		long fieldCount = pFieldsCollection->Getlength();
		for (long i = 0; i < fieldCount; i++)
		{
			CString field = (LPCTSTR)(_bstr_t)pFieldsCollection->Getitem(i);
			xpath += selector + _T("/") + field;
			if (i != fieldCount-1)
			{
				xpath += _T("|");
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _joinIdentityConstraintXPath() failed."),e);
		return _T("");
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _joinIdentityConstraintXPath() failed."));
		return _T("");
	}
	return xpath;
}
BOOL CXMLSOMAttribute::_isDOMNodeInDONNodeList(MSXML2::IXMLDOMNodePtr pXMLDOMNode, 
	MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList)
{
	BOOL result = FALSE;
	try
	{
		if (pXMLDOMNode == NULL || pXMLDOMNodeList == NULL)
		{
			return FALSE;
		}
		long nodeCount = pXMLDOMNodeList->Getlength();
		for (long i = 0 ; i < nodeCount; i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = pXMLDOMNodeList->Getitem(i);
			if (pNode == pXMLDOMNode)
			{
				result = TRUE;
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Attribute _isDOMNodeInDONNodeList() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Attribute _isDOMNodeInDONNodeList() failed."));
		return FALSE;
	}
	return result;
}
