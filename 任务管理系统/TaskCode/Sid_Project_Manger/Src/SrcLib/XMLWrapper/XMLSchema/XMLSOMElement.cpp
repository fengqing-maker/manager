#include "StdAfx.h"
#include "XMLSOMElement.h"

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


CXMLSOMElement::CXMLSOMElement(void)
{
}
CXMLSOMElement::~CXMLSOMElement(void)
{
}

BOOL CXMLSOMElement::InitConstraint(MSXML2::IXMLDOMElementPtr pXMLDOMElement)
{
	if (pXMLDOMElement == NULL)
		return FALSE;
	

	m_exsitAttrList.clear();
	m_nonexistAttrList.clear();
	m_exsitPrevElemList.clear();
	m_exsitNextElemList.clear();
	m_nonExsitPrevElemList.clear();
	m_nonExsitNextElemList.clear();
	
	BOOL result = FALSE;
	try
	{
		result = _initWithElement(pXMLDOMElement);
		RETURN_FALSE_IF_FALSE(result);
		result = _initWithParentElement(pXMLDOMElement);
		RETURN_FALSE_IF_FALSE(result);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element InitConstraint() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element InitConstraint() failed."));
		return FALSE;
	}
	return result;
}

XMLStringList	CXMLSOMElement::GetExsitAttributeNameList()
{
	XMLStringList nameList;
	SOMAttrListItor itor = m_exsitAttrList.begin();
	while (itor != m_exsitAttrList.end())
	{
		nameList.push_back((*itor).m_attributeName);
		itor++;
	}
	return nameList;
}
XMLStringList	CXMLSOMElement::GetNonExsitAttributeNameList()
{
	XMLStringList nameList;
	SOMAttrListItor itor = m_nonexistAttrList.begin();
	while (itor != m_nonexistAttrList.end())
	{
		nameList.push_back((*itor).m_attributeName);
		itor++;
	}
	return nameList;
}

XMLStringList CXMLSOMElement::GetNonExsitPrevElementNameList()
{
	XMLStringList nameList;
	SOMElemListItor itor = m_nonExsitPrevElemList.begin();
	while (itor != m_nonExsitPrevElemList.end())
	{
		nameList.push_back((*itor).m_elementName);
		itor++;
	}
	return nameList;
}
XMLStringList CXMLSOMElement::GetNonExsitNextElementNameList()
{
	XMLStringList nameList;
	SOMElemListItor itor = m_nonExsitNextElemList.begin();
	while (itor != m_nonExsitNextElemList.end())
	{
		nameList.push_back((*itor).m_elementName);
		itor++;
	}
	return nameList;
}

XMLStringList CXMLSOMElement::GetChildElementNamList()
{
	XMLStringList childNameList;
	SOMElemListItor itor = m_childElemList.begin();
	while (itor != m_childElemList.end())
	{
		childNameList.push_back((*itor).m_elementName);
		itor++;
	}
	return childNameList;
}
/******************************private function***************************/
BOOL CXMLSOMElement::_initWithElement(MSXML2::IXMLDOMElementPtr pXMLDOMElement)
{
	if (pXMLDOMElement == NULL)
	{
		return FALSE;
	}
	m_pXMLDOMElement = pXMLDOMElement;
	BOOL result = TRUE;
	try
	{
		MSXML2::ISchemaItemPtr pSchemaItem = _getDeclaration(pXMLDOMElement);
		if (pSchemaItem == NULL)
		{
			return FALSE;	/* Schema中没有对应元素的定义 */
		}

		MSXML2::IXMLDOMNamedNodeMapPtr pXMLDOMAttributeMap = 
			pXMLDOMElement->Getattributes();
		RETURN_FALSE_IF_NULL(pXMLDOMAttributeMap);
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = NULL;
		while ((pXMLDOMAttribute = pXMLDOMAttributeMap->nextNode()) != NULL)
		{
			if (pXMLDOMAttribute->Getspecified() == VARIANT_TRUE)
			{
				SOMAttr somAttr;
				somAttr.m_attributeName = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->Getname();
				/* 对于schema中未定义的属性其schema属性节点指针为NULL */
				somAttr.m_pSchemaAttribute = _getDeclaration(pXMLDOMAttribute);
				if (somAttr.m_pSchemaAttribute == NULL)
				{
					ASSERT(!somAttr.m_attributeName.IsEmpty());
					somAttr.m_attributeName = _T("AnyAttribute");	/* 将名称修改为“AnyAttribute” */
				}
				if (!IncludedByExistAttrList(somAttr)
					&&!IncludedByNonExistAttrList(somAttr))
				{
					m_exsitAttrList.push_back(somAttr);
				}
			}
		}

		MSXML2::ISchemaElementPtr pSchemaElement = pSchemaItem;
#ifdef	_DEBUG
		CString elemName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();

		CString itemName = (LPCTSTR)(_bstr_t)pSchemaElement->Getname();
		MSXML2::SOMITEMTYPE itemType = pSchemaElement->GetitemType();
		ASSERT(itemType == MSXML2::SOMITEM_ELEMENT);
#endif
		m_pSchemaElement = pSchemaElement;
		result = _processElement(m_pSchemaElement);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _initWithElement() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _initWithElement() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_initWithParentElement(MSXML2::IXMLDOMElementPtr pXMLDOMElement)
{
	if (pXMLDOMElement == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		MSXML2::IXMLDOMElementPtr pParentElement = pXMLDOMElement->GetparentNode();
		m_pXMLDOMParentElement = pParentElement;
		m_pSchemaParentElement = _getDeclaration(pParentElement);
#ifdef _DEBUG
		CString parentElemName;
		if(m_pSchemaParentElement != NULL)
			parentElemName = (LPCTSTR)(_bstr_t)m_pSchemaParentElement->Getname();
#endif

		MSXML2::IXMLDOMElementPtr pPrevElement = pXMLDOMElement;
		while ((pPrevElement = pPrevElement->GetpreviousSibling()) != NULL)
		{
			SOMElem somElem;
			somElem.m_elementName = (LPCTSTR)(_bstr_t)pPrevElement->GetnodeName();
			somElem.m_pSchemaElement = _getDeclaration(pPrevElement);
			if (somElem.m_pSchemaElement == NULL)
			{
				ASSERT(!somElem.m_elementName.IsEmpty());
				somElem.m_elementName = _T("AnyElement");	/* 将名称修改为“AnyElement” */
			}
			_pushFrontDuplicate(m_exsitPrevElemList,somElem);
		}
		
		MSXML2::IXMLDOMElementPtr pNextElement = pXMLDOMElement;
		while ((pNextElement = pNextElement->GetnextSibling()) != NULL)
		{
			SOMElem somElem;
			somElem.m_elementName = (LPCTSTR)(_bstr_t)pNextElement->GetnodeName();
			somElem.m_pSchemaElement = _getDeclaration(pNextElement);
			if (somElem.m_pSchemaElement == NULL)
			{
				ASSERT(!somElem.m_elementName.IsEmpty());
				somElem.m_elementName = _T("AnyElement");	/* 将名称修改为“AnyElement” */
			}
			_pushBackDuplicate(m_exsitNextElemList,somElem);
		}

		if (pParentElement == NULL 
			&& m_exsitPrevElemList.size() == 0 
			&& m_exsitNextElemList.size() == 0)
		{
			/* 当前元素节点为根元素时没有兄弟节点 */
			m_nonExsitPrevElemList.clear();
			m_nonExsitNextElemList.clear();
			return TRUE;
		}

		/* 解析父元素的schema约束主要目的是解析出其允许插入的前一个与后一个兄弟节点 */
		result = _processParentElement(m_pSchemaParentElement);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _initWithParentElement() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _initWithParentElement() failed."));
		return FALSE;
	}
	return result;
}

BOOL CXMLSOMElement::_processElement(MSXML2::ISchemaElementPtr pSchemaElement)
{
	if (pSchemaElement == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		ASSERT(VARIANT_FALSE == pSchemaElement->GetisReference());		/* 不考虑引用 */
		MSXML2::ISchemaElementPtr pSubstitutionGroup = pSchemaElement->GetsubstitutionGroup();
		ASSERT(pSubstitutionGroup == NULL);							/* 不考虑替代 */
#endif
		/* 元素类型约束解析 */
		MSXML2::ISchemaTypePtr pSchemaType = pSchemaElement->Gettype();
#ifdef _DEBUG
		CString typeName = (LPCTSTR)(_bstr_t)pSchemaType->Getname();
#endif
		result = _processType(pSchemaType);

	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processElement() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processElement() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processType(MSXML2::ISchemaTypePtr pSchemaType)
{
	if (pSchemaType == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		MSXML2::SOMITEMTYPE elementType = pSchemaType->GetitemType();

		if (elementType == MSXML2::SOMITEM_SIMPLETYPE)
		{
			/* 简单类型的元素不包含属性信息 */
			result = _processSimpleType(pSchemaType);
		}
		else if (elementType == MSXML2::SOMITEM_COMPLEXTYPE)
		{
			/* 复杂类型的元素可以包含属性信息 */
			result = _processComplexType(pSchemaType);
		}
		else if (MSXML2::SOMITEM_DATATYPE == (elementType&MSXML2::SOMITEM_DATATYPE))
		{
			/* 内置数据类型的元素不包含属性信息 */
			result = TRUE;
		}
		else
		{
			ASSERT(0);	/* should never happen */
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processType() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processType() failed."));
		return FALSE;
	}
	return TRUE;
}
BOOL CXMLSOMElement::_processSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType)
{
	if (pSchemaSimpleType == NULL)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CXMLSOMElement::_processComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType)
{
	if (pSchemaComplexType == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{		
#ifdef _DEBUG
		CString typeName = (LPCTSTR)(_bstr_t)pSchemaComplexType->Getname();
#endif
		/* 取得所有指定attribute */
		MSXML2::ISchemaItemCollectionPtr pSchemaAttributeCollection = pSchemaComplexType->Getattributes();
		long attrCount = pSchemaAttributeCollection->Getlength();
		for (long i = 0; i < attrCount; i++)
		{
			MSXML2::ISchemaItemPtr pSchemaAttribute = pSchemaAttributeCollection->Getitem(i);
#ifdef _DEBUG
			CString attrName = (LPCTSTR)(_bstr_t)pSchemaAttribute->Getname();
			ASSERT(!attrName.IsEmpty());
			ASSERT(MSXML2::SOMITEM_ATTRIBUTE == pSchemaAttribute->GetitemType());
#endif
			result = _processAttribute(pSchemaAttribute);
			RETURN_FALSE_IF_FALSE(result);
		}

		/* 取得任意attribute */
		MSXML2::ISchemaAnyPtr pSchemaAnyAttribute = pSchemaComplexType->GetanyAttribute();
		if (pSchemaAnyAttribute != NULL)
		{
#ifdef _DEBUG
			CString anyName = (LPCTSTR)(_bstr_t)pSchemaAnyAttribute->Getname();
			ASSERT(anyName.IsEmpty());
			ASSERT(MSXML2::SOMITEM_ANYATTRIBUTE == pSchemaAnyAttribute->GetitemType());
#endif
			result = _processAnyAttribute(pSchemaAnyAttribute);
			RETURN_FALSE_IF_FALSE(result);
		}

		MSXML2::SCHEMACONTENTTYPE contentType = pSchemaComplexType->GetcontentType();
		MSXML2::ISchemaModelGroupPtr pSchemaModelGroup = pSchemaComplexType->GetcontentModel();
		switch (contentType)
		{
		case MSXML2::SCHEMACONTENTTYPE_EMPTY:		/* Type does not contain elements,text,or attributes */
		case MSXML2::SCHEMACONTENTTYPE_TEXTONLY:	/* Type contains only text.No elements or attributes. */
			result = TRUE;
			break;
		case MSXML2::SCHEMACONTENTTYPE_ELEMENTONLY:	/* Type contains only element declarations */
		case MSXML2::SCHEMACONTENTTYPE_MIXED:		/* Type contains elements,attribute,or text */
			result = _processModelGroup(pSchemaModelGroup);
			break;
		default:
			ASSERT(0);			/* should never happen */
			result = FALSE;
			break;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processComplexType() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processComplexType() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processAnyAttribute(MSXML2::ISchemaAnyPtr pSchemaAnyAttribute)
{
	if (pSchemaAnyAttribute == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		SOMAttr somAttr;
		somAttr.m_attributeName = _T("AnyAttribute");
		somAttr.m_pSchemaAttribute = pSchemaAnyAttribute;
		if (!IncludedByExistAttrList(somAttr)
			&& !IncludedByNonExistAttrList(somAttr))
		{
			m_nonexistAttrList.push_back(somAttr);
		}
		result = TRUE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processAnyAttribute() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processAnyAttribute() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processAttribute(MSXML2::ISchemaAttributePtr pSchemaAttribute)
{
	if (pSchemaAttribute == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		SOMAttr somAttr;
		somAttr.m_attributeName = (LPCTSTR)(_bstr_t)pSchemaAttribute->Getname();
		somAttr.m_pSchemaAttribute = pSchemaAttribute;
		if (!IncludedByExistAttrList(somAttr)
			&& !IncludedByNonExistAttrList(somAttr))
		{
			m_nonexistAttrList.push_back(somAttr);
		}
		result = TRUE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processAttribute() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processAttribute() failed."));
		return FALSE;
	}
	return result;
}

BOOL CXMLSOMElement::_processModelGroup(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	if (pSchemaModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		m_bChildRecordStart = TRUE;

		MSXML2::SOMITEMTYPE modelGroupType = pSchemaModelGroup->GetitemType();
		switch (modelGroupType)
		{
		case MSXML2::SOMITEM_ALL:
			/* 如果使用all则其中仅可以包含element */
			result = _processModelGroup_All(pSchemaModelGroup);
			break;
		case MSXML2::SOMITEM_CHOICE:
			/* choice下可以包含1~n个sequence\choice\element */
			result = _processModelGroup_Choice(pSchemaModelGroup);
			break;
		case MSXML2::SOMITEM_SEQUENCE:
			/* sequence下可以包含1~n个sequence\choice\element */
			result = _processModelGroup_Sequence(pSchemaModelGroup);
			break;
		case MSXML2::SOMITEM_EMPTYPARTICLE:
			/* 空的，不需要解析 */
			result = TRUE;	
			break;
		default:
			ASSERT(0);	/* should never happen */
			result = FALSE;
			break;
		}

		m_bChildRecordStart = FALSE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processModelGroup() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processModelGroup() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processModelGroup_All(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	if (pSchemaModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		ASSERT(MSXML2::SOMITEM_ALL == pSchemaModelGroup->GetitemType());
		int modelMinOccur = (int)pSchemaModelGroup->GetminOccurs();
		int modelMaxOccur = (int)pSchemaModelGroup->GetmaxOccurs();

		MSXML2::ISchemaItemCollectionPtr pParticleCollection = 
			pSchemaModelGroup->Getparticles();
		long particleCount = pParticleCollection->Getlength();
		for (int i = 0; i < particleCount; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = pParticleCollection->Getitem(i);
			int particleMinOccur = (int)pParticle->GetminOccurs();
			int particleMaxOccur = (int)pParticle->GetmaxOccurs();

			ASSERT(MSXML2::SOMITEM_ELEMENT == pParticle->GetitemType());
			SOMElem modelElem;
			modelElem.m_elementName = (LPCTSTR)(_bstr_t)pParticle->Getname();
			modelElem.m_pSchemaElement = pParticle;
			if ((modelMaxOccur >0 || modelMaxOccur == -1)			/* <all>自身出现次数符合要求 */
				&&(particleMaxOccur > 0 || particleMaxOccur == -1)	/* <element>自身出现次数符合要求 */
				&&(m_bChildRecordStart == TRUE))					/* 记录已启动 */
			{
				_pushBackDuplicate(m_childElemList,modelElem);
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processModelGroup_All() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processModelGroup_All() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processModelGroup_Choice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	if (pSchemaModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		ASSERT(MSXML2::SOMITEM_CHOICE == pSchemaModelGroup->GetitemType());
		int modelMinOccur = (int)pSchemaModelGroup->GetminOccurs();
		int modelMaxOccur = (int)pSchemaModelGroup->GetmaxOccurs();
		MSXML2::ISchemaItemCollectionPtr pParticleCollection = 
			pSchemaModelGroup->Getparticles();
		long particleCount = pParticleCollection->Getlength();
		for (long i = 0; i < particleCount; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = pParticleCollection->Getitem(i);
			MSXML2::SOMITEMTYPE particleType = pParticle->GetitemType();
			int particleMinOccur = (int)pParticle->GetminOccurs();
			int particleMaxOccur = (int)pParticle->GetmaxOccurs();
			switch (particleType)
			{
			case MSXML2::SOMITEM_ANY:
				{
					SOMElem anyElem;
					anyElem.m_elementName = _T("AnyElement");
					anyElem.m_pSchemaElement = NULL;

					if ((modelMaxOccur > 0 || modelMaxOccur == -1)			/* <choice>自身出现次数符合条件 */
						&&(particleMaxOccur > 0 || particleMaxOccur == -1)	/* <any>自身出现次数符合条件 */
						&& m_bChildRecordStart == TRUE)						/* 记录已启动 */
					{
						_pushBackDuplicate(m_childElemList,anyElem);
					}

					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_ELEMENT:
				{
					SOMElem modelElem;
					modelElem.m_elementName = (LPCTSTR)(_bstr_t)pParticle->Getname();
					modelElem.m_pSchemaElement = pParticle;

					if ((modelMaxOccur > 0 || modelMaxOccur == -1)			/* <choice>自身出现次数符合条件 */
						&&(particleMaxOccur > 0 || particleMaxOccur == -1)	/* <element>自身出现次数符合条件 */
						&& m_bChildRecordStart == TRUE)						/* 记录已启动 */
					{
						_pushBackDuplicate(m_childElemList,modelElem);
					}

					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_CHOICE:
				result = _processModelGroup_Choice(pParticle);
				break;
			case MSXML2::SOMITEM_SEQUENCE:
				result = _processModelGroup_Choice(pParticle);
				break;
			default:
				ASSERT(0);
				result = FALSE;	/* should never happen */
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processModelGroup_Choice() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processModelGroup_Choice() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processModelGroup_Sequence(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	if (pSchemaModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		ASSERT(MSXML2::SOMITEM_SEQUENCE == pSchemaModelGroup->GetitemType());
		int modelMinOccur = (int)pSchemaModelGroup->GetminOccurs();
		int modelMaxOccur = (int)pSchemaModelGroup->GetmaxOccurs();

		MSXML2::ISchemaItemCollectionPtr pParticleCollection = 
			pSchemaModelGroup->Getparticles();
		long particleCount = pParticleCollection->Getlength();
		for (long i = 0; i < particleCount ; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = 
				pParticleCollection->Getitem(i);
			MSXML2::SOMITEMTYPE particleType = pParticle->GetitemType();
			int particleMinOccur = 0;
			int particleMaxOccur = 0;
			switch (particleType)
			{
			case MSXML2::SOMITEM_ANY:
				{
					particleMinOccur = (int)pParticle->GetminOccurs();
					particleMaxOccur = (int)pParticle->GetmaxOccurs();
					SOMElem anyElem;
					anyElem.m_elementName = _T("AnyElement");
					anyElem.m_pSchemaElement = NULL;

					if ((modelMaxOccur > 0 || modelMaxOccur == -1)			/* <sequence>自身出现次数符合限制 */
						&&(particleMaxOccur > 0 || particleMaxOccur == -1)	/* <any>自身出现次数符合限制 */
						&&(m_bChildRecordStart == TRUE))					/* 记录已启动 */
					{
						_pushBackDuplicate(m_childElemList,anyElem);
					}
					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_ELEMENT:
				{
					particleMinOccur = (int)pParticle->GetminOccurs();
					particleMaxOccur = (int)pParticle->GetmaxOccurs();
					SOMElem somElem;
					somElem.m_elementName = (LPCTSTR)(_bstr_t)pParticle->Getname();
					somElem.m_pSchemaElement = pParticle;

					if ((modelMaxOccur > 0 || modelMaxOccur == -1)			/* <sequence>自身出现次数符合限制 */
						&&(particleMaxOccur > 0 || particleMaxOccur == -1)	/* <any>自身出现次数符合限制 */
						&&(m_bChildRecordStart == TRUE))					/* 记录已启动 */
					{
						_pushBackDuplicate(m_childElemList,somElem);
					}
					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_CHOICE:
				{
					particleMinOccur = _getMinOccurCountOfChoice(pParticle);
					particleMaxOccur = pParticle->GetmaxOccurs();
					result = _processModelGroup_Choice(pParticle);
				}
				break;
			case MSXML2::SOMITEM_SEQUENCE:
				{
					particleMinOccur = _getMinOccurCountOfSequence(pParticle);
					particleMaxOccur = pParticle->GetmaxOccurs();
					result = _processModelGroup_Sequence(pParticle);
				}
				break;
			default:
				ASSERT(0);		/* should never happen */
				result = FALSE;
				break;
			}

			/* 判断刚刚遍历到达的节点是否可以跳过，如果不可以跳过则停止记录 */
			if (m_bChildRecordStart == TRUE)
			{
				/* 如果particle的最小出现次数大于实际出现次数，则该particle不可跳过 */
				if (particleMinOccur > 0)
				{
					m_bChildRecordStart = FALSE;
				}
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processModelGroup_Sequence() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processModelGroup_Sequence() failed."));
		return FALSE;
	}
	return result;
}

BOOL CXMLSOMElement::_processParentElement(MSXML2::ISchemaElementPtr pSchemaParentElement)
{
	if (pSchemaParentElement == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString parentElemName = (LPCTSTR)(_bstr_t)pSchemaParentElement->Getname();
#endif
		if (m_exsitPrevElemList.size() == 0)
			m_bPrevRecordStart = TRUE;	/* 若元素之前不存在兄弟节点则从开始处启动记录 */
		else
			m_bPrevRecordStart = FALSE;	/* 若元素之前存在兄弟节点则从兄弟节点处启动记录 */
		m_bNextRecordStart = FALSE;		/* 从该节点被检测到之后启动记录 */

		MSXML2::ISchemaTypePtr pSchemaParentType = pSchemaParentElement->Gettype();
		result = _processParentType(pSchemaParentType);

		m_bPrevRecordStart = FALSE;		/* 解析完成，强制结束记录 */
		m_bNextRecordStart = FALSE;		/* 解析完成，强制结束记录 */
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentElement() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentElement() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentType(MSXML2::ISchemaTypePtr pSchemaParentType)
{
	if (pSchemaParentType == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString parentTypeName = (LPCTSTR)(_bstr_t)pSchemaParentType->Getname();
#endif
		MSXML2::SOMITEMTYPE parentType = pSchemaParentType->GetitemType();
		if(parentType == MSXML2::SOMITEM_SIMPLETYPE)
		{
			result = _processParentSimpleType(pSchemaParentType);
		}
		else if (parentType == MSXML2::SOMITEM_COMPLEXTYPE)
		{
			result = _processParentComplexType(pSchemaParentType);
		}
		else if (MSXML2::SOMITEM_DATATYPE == (parentType & MSXML2::SOMITEM_DATATYPE))
		{
			result = TRUE;
		}
		else
		{
			result = FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentType() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentType() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentSimpleType(MSXML2::ISchemaTypePtr pSchemaParentSimpleType)
{
	if (pSchemaParentSimpleType == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString parentTypeName = (LPCTSTR)(_bstr_t)pSchemaParentSimpleType->Getname();
#endif
		/* 简单类型的父元素没有子节点列表 */
		ASSERT(0);
		result = FALSE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentSimpleType() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentSimpleType() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentComplexType(MSXML2::ISchemaComplexTypePtr pSchemaParentComplexType)
{
	if (pSchemaParentComplexType == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
#ifdef _DEBUG
		CString parentTypeName = (LPCTSTR)(_bstr_t)pSchemaParentComplexType->Getname();
#endif
		/* 对于父元素的类型定义不解析属性信息 */
		/* 解析父元素的子元素信息 */
		MSXML2::SCHEMACONTENTTYPE contentType = pSchemaParentComplexType->GetcontentType();
		MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup = 
			pSchemaParentComplexType->GetcontentModel();
		switch (contentType)
		{
		case MSXML2::SCHEMACONTENTTYPE_EMPTY:			/* Type does not contain elements,text,or attributes */
		case MSXML2::SCHEMACONTENTTYPE_TEXTONLY:		/* Type contains only text.No elements or attributes. */
			break;
		case MSXML2::SCHEMACONTENTTYPE_ELEMENTONLY:
		case MSXML2::SCHEMACONTENTTYPE_MIXED:
			result = _processParentModelGroup(pSchemaParentModelGroup);
			break;
		default:
			ASSERT(0);		/* should never happen */
			result = FALSE;
			break;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentComplexType() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentComplexType() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentModelGroup(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup)
{
	if (pSchemaParentModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		MSXML2::SOMITEMTYPE modelGroupType = pSchemaParentModelGroup->GetitemType();
		switch (modelGroupType)
		{
		case MSXML2::SOMITEM_ALL:
			/* 如果使用all则其中仅可以包含element */
			result = _processParentModelGroup_All(pSchemaParentModelGroup);
			break;
		case MSXML2::SOMITEM_CHOICE:
			/* choice下可以包含1~n个sequence\choice\element */
			result = _processParentModelGroup_Choice(pSchemaParentModelGroup);
			break;
		case MSXML2::SOMITEM_SEQUENCE:
			/* sequence下可以包含1~n个sequence\choice\element */
			result = _processParentModelGroup_Sequence(pSchemaParentModelGroup);
			break;
		case MSXML2::SOMITEM_EMPTYPARTICLE:
			/* 空的，不需要解析 */
			result = TRUE;	
			break;
		default:
			ASSERT(0);	/* should never happen */
			result = FALSE;
			break;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentModelGroup() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentModelGroup() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentModelGroup_All(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup)
{
	if (pSchemaParentModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		ASSERT(MSXML2::SOMITEM_ALL == pSchemaParentModelGroup->GetitemType());
		int index = m_exsitPrevElemList.size() - 1;
		SOMElem lastPrevElem = GetExistPrevElem(index);
		SOMElem currentElem = GetCurrentElem();
		SOMElem firstNextElem = GetExistNextElem(0);
		if (m_bPrevRecordStart == FALSE)		/* 当前并没有启动元素前兄弟节点记录 */
		{
			if (_isElemInModelGroup(lastPrevElem,pSchemaParentModelGroup))
				m_bPrevRecordStart = TRUE;/* 最后一个前兄弟节点在modelGroup中，启动记录前兄弟节点 */
		}
		if (m_bNextRecordStart == FALSE)
		{
			if (_isElemInModelGroup(currentElem,pSchemaParentModelGroup))
				m_bNextRecordStart = TRUE;/* 当前节点在modelGroup中，启动记录后兄弟节点 */
		}
		/* 将all中仍然可以出现的元素记录下来 */
		MSXML2::ISchemaItemCollectionPtr pParentParticlesCollection = 
			pSchemaParentModelGroup->Getparticles();
		long particleCount = pParentParticlesCollection->Getlength();
		for (long i = 0 ; i < particleCount; i++)
		{
			MSXML2::ISchemaParticlePtr pParentParticle = 
				pParentParticlesCollection->Getitem(i);
			MSXML2::SOMITEMTYPE particleType = pParentParticle->GetitemType();
			/* 在all中仅可以出现element */
			ASSERT(MSXML2::SOMITEM_ELEMENT == particleType);
			SOMElem somElem;
			somElem.m_elementName = (LPCTSTR)(_bstr_t)pParentParticle->Getname();
			somElem.m_pSchemaElement = pParentParticle;
			int minOccurCount = (int)pParentParticle->GetminOccurs();
			int maxOccurCount = (int)pParentParticle->GetmaxOccurs();
			int	occurCount = OccurCountOfElem(somElem);
			if (occurCount < maxOccurCount || maxOccurCount == -1)
			{
				if (m_bPrevRecordStart)
				{
					_pushBackDuplicate(m_nonExsitPrevElemList,somElem);
				}
				if (m_bNextRecordStart)
				{
					_pushBackDuplicate(m_nonExsitNextElemList,somElem);
				}
			}
		}
		if (m_bPrevRecordStart == TRUE)
		{
			if (_isElemInModelGroup(currentElem,pSchemaParentModelGroup))
				m_bPrevRecordStart = FALSE;/* 当前节点在modelGroup中，停止记录前兄弟节点 */
		}
		if (m_bNextRecordStart == TRUE)
		{
			if (_isElemInModelGroup(firstNextElem,pSchemaParentModelGroup))
				m_bNextRecordStart = FALSE;/* 第一个后兄弟节点在modelGroup中，停止记录后兄弟节点 */
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentModelGroup_All() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentModelGroup_All() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentModelGroup_Choice(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup)
{
	if (pSchemaParentModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		ASSERT(MSXML2::SOMITEM_CHOICE == pSchemaParentModelGroup->GetitemType());
		/* 如果整个<choice>的最大出现次数大于其实际出现次数，则<choice>下的内容都可能再次出现 */
		int modelMinOccur = (int)pSchemaParentModelGroup->GetminOccurs();
		int modelMaxOccur = (int)pSchemaParentModelGroup->GetmaxOccurs();
		int modelOccur = _getOccurCountOfChoice(pSchemaParentModelGroup);
		BOOL bRecordAllChoice = FALSE;
		if (modelOccur < modelMaxOccur || modelMaxOccur == -1)
		{
			bRecordAllChoice = TRUE;
		}

		int index = m_exsitPrevElemList.size() - 1;
		SOMElem lastPrevElem = GetExistPrevElem(index);
		SOMElem currentElem = GetCurrentElem();
		SOMElem firstNextElem = GetExistNextElem(0);
		if (m_bPrevRecordStart == FALSE)		/* 当前并没有启动元素前兄弟节点记录 */
		{
			if (_isElemInModelGroup(lastPrevElem,pSchemaParentModelGroup))
			{
				/* 最后一个前兄弟节点在modelGroup中，启动记录前兄弟节点 */
				m_bPrevRecordStart = TRUE;
			}
		}
		if (m_bNextRecordStart == FALSE)
		{
			if (_isElemInModelGroup(currentElem,pSchemaParentModelGroup))
			{
				/* 当前节点在modelGroup中，启动记录后兄弟节点 */
				m_bNextRecordStart = TRUE;
			}
		}

		if (bRecordAllChoice == FALSE)
		{
			/* choice的出现次数达到上限，不能继续出现choice */
			BOOL bPrevFromChoice = FALSE;
			if (_isElemInModelGroup(lastPrevElem,pSchemaParentModelGroup))
			{
				/* 最后一个前兄弟节点来自于choice,则前面只能出现前一个兄弟节点 */
				bPrevFromChoice = TRUE;
				if (m_bPrevRecordStart)
				{
					MSXML2::ISchemaParticlePtr pPrevElement = lastPrevElem.m_pSchemaElement;
					int prevMaxOccurCount = pPrevElement->GetmaxOccurs();
					ASSERT(modelMaxOccur != -1);
					if (prevMaxOccurCount != -1)
						prevMaxOccurCount *= modelMaxOccur;
					int prevOccurCount = 0;
					/* 从最后一个前兄弟之前开始遍历 */
					for (int i = m_exsitPrevElemList.size() - 1 ; i >= 0 ; i--)
					{
						SOMElem prevElem = GetExistPrevElem(i);
						if (_isSameSchemaElem(prevElem,lastPrevElem))
						{
							prevOccurCount++;
						}
						else
						{
							break;	/* 在第一个不同的元素处停止记录出现次数 */
						}
					}
					if (_isSameSchemaElem(currentElem,lastPrevElem))
					{
						prevOccurCount++;
					}
					for (int i = 0; i < (int)m_exsitNextElemList.size(); i++)
					{
						SOMElem nextElem = GetExistNextElem(i);
						if (_isSameSchemaElem(nextElem,lastPrevElem))
						{
							prevOccurCount++;
						}
						else
						{
							break;	/* 在第一个不同的元素处停止记录出现次数 */
						}
					}
					if (prevOccurCount < prevMaxOccurCount || prevMaxOccurCount == -1)
					{
						_pushBackDuplicate(m_nonExsitPrevElemList,lastPrevElem);
					}
				}
			}
			BOOL bCurrentFromChoice = FALSE;
			if (_isElemInModelGroup(currentElem,pSchemaParentModelGroup))
			{
				/* 当前节点来自于choice，则前、后只能出现当前节点 */
				bCurrentFromChoice = TRUE;
				if (m_bPrevRecordStart || m_bNextRecordStart)
				{
					MSXML2::ISchemaParticlePtr pCurrentElement = currentElem.m_pSchemaElement;
					int curMaxOccurCount = pCurrentElement->GetmaxOccurs();
					ASSERT(modelMaxOccur != -1);
					if (curMaxOccurCount != -1)
						curMaxOccurCount *= modelMaxOccur;
					int curOccurCount = 0;
					/* 从最后一个前兄弟开始遍历 */
					for (int i = m_exsitPrevElemList.size() - 1 ; i >= 0 ; i--)
					{
						SOMElem prevElem = GetExistPrevElem(i);
						if (_isSameSchemaElem(prevElem,currentElem))
						{
							curOccurCount++;
						}
						else
						{
							break;	/* 在第一个不同的元素处停止记录出现次数 */
						}
					}
					curOccurCount++;	/* 加上当前元素本身的出现次数 */
					for (int i = 0; i < (int)m_exsitNextElemList.size(); i++)
					{
						SOMElem nextElem = GetExistNextElem(i);
						if (_isSameSchemaElem(nextElem,currentElem))
						{
							curOccurCount++;
						}
						else
						{
							break;	/* 在第一个不同的元素处停止记录出现次数 */
						}
					}
					if (curOccurCount < curMaxOccurCount || curMaxOccurCount == -1)
					{
						if (m_bPrevRecordStart)
							_pushBackDuplicate(m_nonExsitPrevElemList,currentElem);
						if (m_bNextRecordStart)
							_pushBackDuplicate(m_nonExsitNextElemList,currentElem);
					}
				}
			}
			BOOL bNextFromChoice = FALSE;
			if (_isElemInModelGroup(lastPrevElem,pSchemaParentModelGroup))
			{
				/* 第一个后兄弟节点来自于choice，则后面只能出现后一个兄弟节点相同的元素 */
				bNextFromChoice = TRUE;
				if (m_bNextRecordStart)
				{
					MSXML2::ISchemaParticlePtr pNextElement = firstNextElem.m_pSchemaElement;
					int nextMaxOccurCount = pNextElement->GetmaxOccurs();
					ASSERT(modelMaxOccur != -1);
					if (nextMaxOccurCount != -1)
						nextMaxOccurCount *= modelMaxOccur;
					int nextOccurCount = 0;
					/* 从最后一个前兄弟之前开始遍历 */
					for (int i = m_exsitPrevElemList.size() - 1 ; i >= 0 ; i--)
					{
						SOMElem prevElem = GetExistPrevElem(i);
						if (_isSameSchemaElem(prevElem,firstNextElem))
						{
							nextOccurCount++;
						}
						else
						{
							break;	/* 在第一个不同的元素处停止记录出现次数 */
						}
					}
					if (_isSameSchemaElem(currentElem,firstNextElem))
					{
						nextOccurCount++;
					}
					for (int i = 0; i < (int)m_exsitNextElemList.size(); i++)
					{
						SOMElem nextElem = GetExistNextElem(i);
						if (_isSameSchemaElem(nextElem,firstNextElem))
						{
							nextOccurCount++;
						}
						else
						{
							break;	/* 在第一个不同的元素处停止记录出现次数 */
						}
					}
					if (nextOccurCount < nextMaxOccurCount || nextMaxOccurCount == -1)
					{
						_pushBackDuplicate(m_nonExsitPrevElemList,firstNextElem);
					}
				}
			}
		}

		/* 将choice中仍然可以出现的元素记录下来 */
		MSXML2::ISchemaItemCollectionPtr pParentParticlesCollection = 
			pSchemaParentModelGroup->Getparticles();
		long particleCount = pParentParticlesCollection->Getlength();

		for (long i = 0 ; i < particleCount; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = 
				pParentParticlesCollection->Getitem(i);
			MSXML2::SOMITEMTYPE particleType = pParticle->GetitemType();

			SOMElem somElem;
			somElem.m_elementName = (LPCTSTR)(_bstr_t)pParticle->Getname();
			somElem.m_pSchemaElement = pParticle;

			switch (particleType)
			{
			case MSXML2::SOMITEM_ANY:
				ASSERT(0);	/* 若出现需要处理 */
				result = TRUE;
				break;
			case MSXML2::SOMITEM_ELEMENT:
				{
					int particleMinOccur = (int)pParticle->GetminOccurs();
					int particleMaxOccur = (int)pParticle->GetmaxOccurs();
					if (bRecordAllChoice == TRUE)
					{
						/* <choice>本身没有达到最大出现次数,对choice中 */
						if (particleMaxOccur > 0 || particleMaxOccur == -1)
						{
							/* <choice>下的全部最大出现次数大于0的元素都可能出现 */
							if (m_bPrevRecordStart == TRUE)
								_pushBackDuplicate(m_nonExsitPrevElemList,somElem);
							if (m_bNextRecordStart == TRUE)
								_pushBackDuplicate(m_nonExsitNextElemList,somElem);
						}
					}
					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_SEQUENCE:
				result = _processParentModelGroup_Sequence(pParticle);
				break;
			case MSXML2::SOMITEM_CHOICE:
				result = _processParentModelGroup_Choice(pParticle);
				break;
			default:
				result = FALSE;	/* should never happen */
				ASSERT(0);
				break;
			}

		}

		if (m_bPrevRecordStart == TRUE)
		{
			/* 如果当前节点在modelGroup中出现，则停止前兄弟节点记录 */
			if (_isElemInModelGroup(currentElem,pSchemaParentModelGroup))
				m_bPrevRecordStart = FALSE;
		}
		if (m_bNextRecordStart == TRUE)
		{
			/* 如果下一个兄弟节点在modelGroup中出现，则停止后兄弟节点记录 */
			if (_isElemInModelGroup(firstNextElem,pSchemaParentModelGroup))
				m_bNextRecordStart = FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentModelGroup_Choice() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentModelGroup_Choice() failed."));
		return FALSE;
	}
	return result;
}
BOOL CXMLSOMElement::_processParentModelGroup_Sequence(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup)
{
	if (pSchemaParentModelGroup == NULL)
	{
		return FALSE;
	}
	BOOL result = TRUE;
	try
	{
		ASSERT(MSXML2::SOMITEM_SEQUENCE == pSchemaParentModelGroup->GetitemType());
		/* 如果整个<sequence>的最大出现次数大于其实际出现次数，则<sequence>下的内容都可能再次出现 */

		int index = m_exsitPrevElemList.size() - 1;
		SOMElem lastPrevElem = GetExistPrevElem(index);
		SOMElem currentElem = GetCurrentElem();
		SOMElem firstNextElem = GetExistNextElem(0);
		MSXML2::ISchemaItemCollectionPtr pParentParticlesCollection = 
			pSchemaParentModelGroup->Getparticles();
		long particleCount = pParentParticlesCollection->Getlength();
		
		/* 记录遍历到的元素是否可以跳过，最小出现次数为0或已出现次数大于等于最小出现次数的元素可以跳过
		如果该元素可以跳过，则继续向后记录；如果该元素不可跳过，则不再向后记录 */
		for (long i = 0; i < particleCount ; i++)
		{
			/* sequence遍历必须是顺序进行 */
			MSXML2::ISchemaParticlePtr pSchemaParticle = 
				pParentParticlesCollection->Getitem(i);
			SOMElem somElem;	/* 当前遍历抵达节点 */
			somElem.m_elementName = (LPCTSTR)(_bstr_t)pSchemaParticle->Getname();
			somElem.m_pSchemaElement = pSchemaParticle;
			/* 检查是否启动记录 */
			if (m_bPrevRecordStart == FALSE)
			{
				if (_isSameSchemaElem(lastPrevElem,somElem))
				{
					/* 遍历modelGroup抵达前一个兄弟节点，启动记录前兄弟节点 */
					m_bPrevRecordStart = TRUE;
				}
			}
			if (m_bNextRecordStart == FALSE)
			{
				if (_isSameSchemaElem(currentElem,somElem))
				{
					/* 遍历modelGroup抵达当前节点，启动记录后兄弟节点 */
					m_bNextRecordStart = TRUE;
				}
			}
			MSXML2::SOMITEMTYPE particleType = 
				pSchemaParticle->GetitemType();					
			int particleMinOccurCount = 0;
			int particleMaxOccurCount = 0;
			int particleOccurCount = 0;

			/* 记录下现在的记录数目，如果处理完子元素后记录数目增多则说明对子元素进行了记录 */
			int oldPrevRecordCount = m_nonExsitPrevElemList.size();
			int oldNextRecordCount = m_nonExsitNextElemList.size();
			switch (particleType)
			{
			case MSXML2::SOMITEM_ANY:
				{
					particleMinOccurCount = (int)pSchemaParticle->GetminOccurs();
					particleMaxOccurCount = (int)pSchemaParticle->GetmaxOccurs();
					SOMElem somElem;
					somElem.m_elementName = _T("AnyElement");
					somElem.m_pSchemaElement = pSchemaParticle;
					particleOccurCount = OccurCountOfElem(somElem);

					if (m_bPrevRecordStart)
					{
						if (particleOccurCount < particleMaxOccurCount || particleMaxOccurCount == -1)
						{
							_pushBackDuplicate(m_nonExsitPrevElemList,somElem);
						}
					}
					if (m_bNextRecordStart)
					{
						if (particleOccurCount < particleMaxOccurCount || particleMaxOccurCount == -1)
						{
							_pushBackDuplicate(m_nonExsitNextElemList,somElem);
						}
					}
					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_ELEMENT:
				{
					particleMinOccurCount = (int)pSchemaParticle->GetminOccurs();
					particleMaxOccurCount = (int)pSchemaParticle->GetmaxOccurs();
					particleOccurCount = OccurCountOfElem(somElem);
					if (m_bPrevRecordStart)
					{
						if (particleOccurCount < particleMaxOccurCount || particleMaxOccurCount == -1)
						{
							_pushBackDuplicate(m_nonExsitPrevElemList,somElem);
						}
					}
					if (m_bNextRecordStart)
					{
						if (particleOccurCount < particleMaxOccurCount || particleMaxOccurCount == -1)
						{
							_pushBackDuplicate(m_nonExsitNextElemList,somElem);
						}
					}
					result = TRUE;
				}
				break;
			case MSXML2::SOMITEM_CHOICE:
				particleMinOccurCount = _getMinOccurCountOfChoice(pSchemaParticle);
				particleMaxOccurCount = (int)pSchemaParticle->GetmaxOccurs();
				result = _processParentModelGroup_Choice(pSchemaParticle);			
				break;
			case MSXML2::SOMITEM_SEQUENCE:
				particleMinOccurCount = _getMinOccurCountOfSequence(pSchemaParticle);
				particleMaxOccurCount = (int)pSchemaParticle->GetmaxOccurs();
				result = _processParentModelGroup_Sequence(pSchemaParticle);
				break;
			default:
				ASSERT(0);
				result = FALSE;
				break;
			}
			RETURN_FALSE_IF_FALSE(result);
			int newPrevRecordCount = m_nonExsitPrevElemList.size();
			int newNextRecordCount = m_nonExsitNextElemList.size();
			/* 检查是否停止记录 */
			if (m_bPrevRecordStart == TRUE)
			{
				/* 通过记录数目的变化判断在子元素处理过程中有没有进行记录，
				如果有记录则在记录完成后判断是否继续向后记录 */
				if (newPrevRecordCount > oldPrevRecordCount 
					&& particleMinOccurCount != 0 && particleOccurCount < particleMinOccurCount)
				{
					/* 当记录到一个不可跳过的元素时停止记录，
					不可跳过的条件为最小出现次数不为0且已出现次数小于最小出现次数 */
					m_bPrevRecordStart = FALSE;
				}

				SOMElem currentElem = GetCurrentElem();
				if (_isSameSchemaElem(currentElem,somElem))
				{
					/* 遍历modelGroup已完成当前节点，关闭前兄弟记录 */
					m_bPrevRecordStart = FALSE;
				}
			}
			if (m_bNextRecordStart == TRUE)
			{
				/* 通过记录数目的变化判断在子元素处理过程中有没有进行记录，
				如果有记录则在记录完成后判断是否继续向后记录 */
				if (newNextRecordCount > oldNextRecordCount
					&& particleMinOccurCount != 0 && particleOccurCount < particleMinOccurCount)
				{
					/* 当记录到一个不可跳过的元素时停止记录，
					不可跳过的条件为最小出现次数不为0且已出现次数小于最小出现次数 */
					m_bNextRecordStart = FALSE;
				}
				SOMElem firstNextElem = GetExistNextElem(0);
				if (_isSameSchemaElem(firstNextElem,somElem))
				{
					/* 遍历modelGroup已完成后一个兄弟节点，关闭后兄弟记录 */
					m_bNextRecordStart = FALSE;
				}
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _processParentModelGroup_Sequence() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _processParentModelGroup_Sequence() failed."));
		return FALSE;
	}
	return result;
}


BOOL CXMLSOMElement::IncludedByExistAttrList(SOMAttr somAttr)
{
	BOOL result = FALSE;
	SOMAttrListItor existItor = m_exsitAttrList.begin();
	while (existItor != m_exsitAttrList.end())
	{
		SOMAttr curAttr = *existItor;
		if (_isSameSchemaAttr(curAttr,somAttr))
		{
			result = TRUE;
			break;
		}
		existItor++;
	}
	return result;
}
BOOL CXMLSOMElement::IncludedByNonExistAttrList(SOMAttr somAttr)
{
	BOOL result = FALSE;
	SOMAttrListItor nonexistItor = m_nonexistAttrList.begin();
	while (nonexistItor != m_nonexistAttrList.end())
	{
		SOMAttr curAttr = *nonexistItor;
		if (_isSameSchemaAttr(curAttr,somAttr))
		{
			result = TRUE;
			break;
		}
		nonexistItor++;
	}
	return result;
}

BOOL CXMLSOMElement::IncludeByExistPrevElemList(SOMElem somElem)
{
	BOOL result = FALSE;
	SOMElemListItor itor = m_exsitPrevElemList.begin();
	while (itor != m_exsitPrevElemList.end())
	{
		SOMElem listElem = *itor;
		if (_isSameSchemaElem(listElem,somElem))
		{
			result = TRUE;
			break;
		}
		itor++;
	}
	return result;
}
int CXMLSOMElement::OccurCountInPrevElemList(SOMElem somElem)
{
	int occurCount = 0;
	SOMElemListItor itor = m_exsitPrevElemList.begin();
	while (itor != m_exsitPrevElemList.end())
	{
		SOMElem listElem = *itor;
		if (_isSameSchemaElem(listElem,somElem))
		{
			occurCount++;
		}
		itor++;
	}
	return occurCount;
}
BOOL CXMLSOMElement::IncludeByExistNextElemList(SOMElem somElem)
{
	BOOL result = FALSE;
	SOMElemListItor itor = m_exsitNextElemList.begin();
	while (itor != m_exsitNextElemList.end())
	{
		SOMElem listElem = *itor;
		if (_isSameSchemaElem(listElem,somElem))
		{
			result = TRUE;
			break;
		}
		itor++;
	}
	return result;
}
int CXMLSOMElement::OccurCountInNextElemList(SOMElem somElem)
{
	int occurCount = 0;
	SOMElemListItor itor = m_exsitNextElemList.begin();
	while (itor != m_exsitNextElemList.end())
	{
		SOMElem listElem = *itor;
		if (_isSameSchemaElem(listElem,somElem))
		{
			occurCount++;
		}
		itor++;
	}
	return occurCount;
}

int CXMLSOMElement::OccurCountOfElem(SOMElem somElem)
{
	int occurCount = 0;
	occurCount += OccurCountInPrevElemList(somElem);
	occurCount += OccurCountInNextElemList(somElem);
	SOMElem currentElem = GetCurrentElem();
	if (_isSameSchemaElem(currentElem,somElem))
	{
		occurCount++;
	}
	return occurCount;
}

SOMElem CXMLSOMElement::GetCurrentElem()
{
	SOMElem curElem;
	if (m_pSchemaElement != NULL)
	{
		curElem.m_elementName = (LPCTSTR)(_bstr_t)m_pSchemaElement->Getname();
		curElem.m_pSchemaElement = m_pSchemaElement;
	}
	return curElem;
}
SOMElem CXMLSOMElement::GetExistPrevElem(int index)
{
	SOMElem somElem;
	SOMElemListItor itor = m_exsitPrevElemList.begin();
	int curIndex = 0;
	while (itor != m_exsitPrevElemList.end())
	{
		if (curIndex == index)
		{
			somElem = *itor;
			break;
		}
		curIndex++;
		itor++;
	}
	return somElem;
}
SOMElem CXMLSOMElement::GetExistNextElem(int index)
{
	SOMElem somElem;
	SOMElemListItor itor = m_exsitNextElemList.begin();
	int curIndex = 0;
	while (itor != m_exsitNextElemList.end())
	{
		if (curIndex == index)
		{
			somElem = *itor;
			break;
		}
		curIndex++;
		itor++;
	}
	return somElem;
}
int		CXMLSOMElement::GetExistElemCount()
{
	int exsitPrevCount = m_exsitPrevElemList.size();
	int exsitCurCount = 1;
	int exsitNextCount = m_exsitNextElemList.size();
	int exsitTotalCount = exsitPrevCount + exsitCurCount + exsitNextCount;
	return exsitTotalCount;
}
SOMElem CXMLSOMElement::GetExistElem(int index)
{
	int exsitPrevCount = m_exsitPrevElemList.size();
	int exsitCurCount = 1;
	int exsitNextCount = m_exsitNextElemList.size();
	int exsitTotalCount = exsitPrevCount + exsitCurCount + exsitNextCount;
	SOMElem somElem;
	if ( index >= 0 && index < (int)m_exsitPrevElemList.size())
		somElem = GetExistPrevElem(index);
	if (index == (int)m_exsitPrevElemList.size())
		somElem = GetCurrentElem();
	if (index > (int)m_exsitPrevElemList.size() && index < exsitTotalCount)
		somElem = GetExistNextElem(index - exsitPrevCount - exsitCurCount);
	return somElem;
}

BOOL CXMLSOMElement::_isSameSchemaElem(SOMElem firstElem,SOMElem secondElem)
{
	/* 对未初始化和schema中未定义的元素节点指针都可能为NULL */
	if (firstElem.m_pSchemaElement == NULL && secondElem.m_pSchemaElement == NULL)
	{
		/* 指针都为NULL则根据名称判断 */
		if (firstElem.m_elementName != secondElem.m_elementName)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else if (firstElem.m_pSchemaElement != NULL && secondElem.m_pSchemaElement != NULL)
	{
		/* 指针都不为NULL则先判断指针类型 */
		MSXML2::SOMITEMTYPE firstItemType = firstElem.m_pSchemaElement->GetitemType();
		MSXML2::SOMITEMTYPE secondItemType = secondElem.m_pSchemaElement->GetitemType();
		if (firstItemType != secondItemType)
		{
			/* 类型不同则必定不同 */
			return FALSE;
		}
		if (firstItemType == MSXML2::SOMITEM_ELEMENT 
			&& secondItemType == MSXML2::SOMITEM_ELEMENT)
		{
			/* 类型都相同且为元素类型 */
			/* 对于元素类型需要判断是否为引用 */
			MSXML2::ISchemaElementPtr pFirstElement = firstElem.m_pSchemaElement;
			MSXML2::ISchemaElementPtr pSecondElement = secondElem.m_pSchemaElement;
			if (VARIANT_TRUE == pFirstElement->GetisReference() 
				&& VARIANT_TRUE == pSecondElement->GetisReference())
			{
				/* 两个都为引用时要求名称与指针都相同，元素才相同 */
				if (firstElem.m_elementName == secondElem.m_elementName
					&& firstElem.m_pSchemaElement == secondElem.m_pSchemaElement)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else if (VARIANT_TRUE == pFirstElement->GetisReference() 
				|| VARIANT_TRUE == pSecondElement->GetisReference())
			{
				/* 两个之中只有一个为引用时仅要求名称相同，元素就相同 */
				if (firstElem.m_elementName == secondElem.m_elementName)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				/* 两个都为不引用时要求名称与指针都相同，元素才相同 */
				if (firstElem.m_elementName == secondElem.m_elementName
					&& firstElem.m_pSchemaElement == secondElem.m_pSchemaElement)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
		else
		{
			/* 类型相同且不是元素类型时，要求名称与指针都相同 */
			if (firstElem.m_elementName == secondElem.m_elementName
				&&firstElem.m_pSchemaElement == secondElem.m_pSchemaElement)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		/* 对于any元素，其在Schema文件中约束指针不为空，
		但通过getDeclaration得到的Schema约束指针为空 */
		MSXML2::SOMITEMTYPE firstItemType;
		MSXML2::SOMITEMTYPE secondItemType;
		/* 判断两个元素是否是any元素 */
		if (firstElem.m_pSchemaElement != NULL)
		{
			firstItemType = firstElem.m_pSchemaElement->GetitemType();
		}
		else
		{
			firstItemType = MSXML2::SOMITEM_ANY;	/* 对找不到Schema约束的元素认为是any元素 */
		}
		if (secondElem.m_pSchemaElement != NULL)
		{
			secondItemType = secondElem.m_pSchemaElement->GetitemType();
		}
		else
		{
			secondItemType = MSXML2::SOMITEM_ANY;	/* 对找不到Schema约束的元素认为是any元素 */
		}
		if ((firstItemType == MSXML2::SOMITEM_ANY)
			&&(secondItemType == MSXML2::SOMITEM_ANY))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}
BOOL CXMLSOMElement::_isSameSchemaAttr(SOMAttr firstAttr,SOMAttr secondAttr)
{
	if (firstAttr.m_pSchemaAttribute == NULL && secondAttr.m_pSchemaAttribute == NULL)
	{
		if (firstAttr.m_attributeName == secondAttr.m_attributeName)
		{
			return TRUE;
		} 
		else
		{
			return FALSE;
		}
	}
	else if (firstAttr.m_pSchemaAttribute != NULL && secondAttr.m_pSchemaAttribute != NULL)
	{
		if (firstAttr.m_pSchemaAttribute == secondAttr.m_pSchemaAttribute
			&&firstAttr.m_attributeName == secondAttr.m_attributeName)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}
BOOL CXMLSOMElement::_isElemInModelGroup(SOMElem somElem,MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	if (pSchemaModelGroup == 0)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		MSXML2::ISchemaItemCollectionPtr pParticlesCollection = pSchemaModelGroup->Getparticles();
		long particleCount = pParticlesCollection->Getlength();
		for (long i = 0; i < particleCount ; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = pParticlesCollection->Getitem(i);
			SOMElem modelElem;
			modelElem.m_elementName = (LPCTSTR)(_bstr_t)pParticle->Getname();
			modelElem.m_pSchemaElement = pParticle;
			if (_isSameSchemaElem(somElem,modelElem))
			{
				result = TRUE;
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _isElemInModelGroup() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _isElemInModelGroup() failed."));
		return FALSE;
	}
	return result;
}

int CXMLSOMElement::_getMinOccurCountOfChoice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	/* 为<choice>，其本身minOccur不为0，但其中存在minOccur为0的内容时，
	其实际上的minOccur同样为0 */
	ASSERT(pSchemaModelGroup != NULL);
	ASSERT(pSchemaModelGroup->GetitemType() == MSXML2::SOMITEM_CHOICE);
	int minOccurCount = (int)pSchemaModelGroup->GetminOccurs();
	if (minOccurCount == 0)
	{
		return minOccurCount;
	}
	else
	{
		MSXML2::ISchemaItemCollectionPtr pSchemaParticlesCollection = 
			pSchemaModelGroup->Getparticles();
		long particleCount = pSchemaParticlesCollection->Getlength();
		int resultCount = -1;
		for (long i = 0; i < particleCount ; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = pSchemaParticlesCollection->Getitem(i);
			MSXML2::SOMITEMTYPE particleType = pParticle->GetitemType();
			int occurCount = 0;
			switch (particleType)
			{
			case MSXML2::SOMITEM_ANY:
			case MSXML2::SOMITEM_ELEMENT:
				occurCount = (int)pParticle->GetminOccurs();
				break;
			case MSXML2::SOMITEM_SEQUENCE:
				occurCount = _getMinOccurCountOfSequence(pParticle);
				break;
			case MSXML2::SOMITEM_CHOICE:
				occurCount = _getMinOccurCountOfChoice(pParticle);
				break;
			default:
				ASSERT(0);	/* should never happen */
				occurCount = 0;
				break;
			}

			if (resultCount != -1)
			{
				resultCount = min(occurCount,resultCount);
			}
			else
			{
				resultCount = occurCount;
			}
		}
		if (resultCount == 0)
		{
			return 0;
		}
		else
		{
			return minOccurCount;
		}
	}
}
int CXMLSOMElement::_getOccurCountOfChoice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	ASSERT(pSchemaModelGroup != NULL);
	ASSERT(pSchemaModelGroup->GetitemType() == MSXML2::SOMITEM_CHOICE);
	/* <choice>中任意一个元素连续出现出现小于等于自身最大出现次数时认为choice出现一次 */
	int		choiceCount = 0;	/* <choice>出现的次数 */
	try
	{
		SOMElem lastElem;			/* 记录上一次遍历的节点 */
		SOMElem curElem;			/* 记录当前遍历的节点 */
		int		sameElemCount = 0;	/* 遍历过程中连续的相同节点数目 */

		int exsitCount = GetExistElemCount();
		int tempOccurCount = 0;
		for (int i = 0; i < exsitCount; i++)
		{
			curElem = GetExistElem(i);
			if (_isElemInModelGroup(curElem,pSchemaModelGroup))
			{
				/* 遍历到的元素在choice中 */
				if (_isSameSchemaElem(curElem,lastElem))
				{
					sameElemCount++;
				}
				else
				{
					sameElemCount = 1;
				}
				MSXML2::ISchemaParticlePtr pParticle = curElem.m_pSchemaElement;
				int curMaxOccurCount = (int)pParticle->GetmaxOccurs();
				if (sameElemCount == 1)
				{
					choiceCount++;	/* 首次出现 */
				}
				if (sameElemCount < curMaxOccurCount || curMaxOccurCount == -1)
				{
					/* 出现次数未超过限制 */
				}
				else
				{
					/* 出现次数超过限制 */
					sameElemCount = 0;
				}
			}
			else
			{
				sameElemCount = 0;
			}
			lastElem = curElem;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Elem _getOccurCountOfChoice() failed."),e);
		return 0;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Elem _getOccurCountOfChoice() failed."));
		return 0;
	}
	return choiceCount;
}

int CXMLSOMElement::_getMinOccurCountOfSequence(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup)
{
	/* 对<sequence>，其本身minOccur不为0，但其中内容的minOccur都为0时，
	其实际上的minOccur同样为0 */
	ASSERT(pSchemaModelGroup != NULL);
	ASSERT(pSchemaModelGroup->GetitemType() == MSXML2::SOMITEM_SEQUENCE);
	int minOccurCount = (int)pSchemaModelGroup->GetminOccurs();
	if (minOccurCount == 0)
	{
		return minOccurCount;
	}
	else
	{
		MSXML2::ISchemaItemCollectionPtr pSchemaParticlesCollection = 
			pSchemaModelGroup->Getparticles();
		long particleCount = pSchemaParticlesCollection->Getlength();
		int resultCount = -1;	/* 初始值为-1 */
		for (long i = 0; i < particleCount ; i++)
		{
			MSXML2::ISchemaParticlePtr pParticle = pSchemaParticlesCollection->Getitem(i);
			MSXML2::SOMITEMTYPE particleType = pParticle->GetitemType();
			int occurCount = 0;
			switch (particleType)
			{
			case MSXML2::SOMITEM_ANY:
			case MSXML2::SOMITEM_ELEMENT:
				occurCount = (int)pParticle->GetminOccurs();
				break;
			case MSXML2::SOMITEM_SEQUENCE:
				occurCount = _getMinOccurCountOfSequence(pParticle);
				break;
			case MSXML2::SOMITEM_CHOICE:
				occurCount = _getMinOccurCountOfChoice(pParticle);
				break;
			default:
				ASSERT(0);	/* should never happen */
				occurCount = 0;
				break;
			}
			
			if (resultCount != -1)
			{
				resultCount = max(occurCount,resultCount);
			}
			else
			{
				resultCount = occurCount;
			}
		}
		if (resultCount == 0)
		{
			return 0;
		}
		else
		{
			return minOccurCount;
		}
	}
}

/*******************************************************************************/
void CXMLSOMElement::_pushFrontDuplicate(SOMElemList& somElemList,SOMElem& somElem)
{
	SOMElemListItor itor = somElemList.begin();
	while (itor != somElemList.end())
	{
		SOMElem listElem = *itor;
		if (_isSameSchemaElem(listElem,somElem))
		{
			break;
		}
		itor++;
	}
	somElemList.push_front(somElem);
}
void CXMLSOMElement::_pushBackDuplicate(SOMElemList& somElemList,SOMElem& somElem)
{
	SOMElemListItor itor = somElemList.begin();
	while (itor != somElemList.end())
	{
		SOMElem listElem = *itor;
		if (_isSameSchemaElem(listElem,somElem))
		{
			break;
		}
		itor++;
	}
	somElemList.push_back(somElem);
}
