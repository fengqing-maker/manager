#pragma once
#include <list>

#include "XMLSOMAttribute.h"
#include "../XMLParser/XMLCommonDef.h"


class SOMAttr
{
public:
	SOMAttr()
	{
		m_attributeName.Empty();
		m_pSchemaAttribute = NULL;
	}
	~SOMAttr()
	{
	}
	CString						m_attributeName;
	
	MSXML2::ISchemaItemPtr		m_pSchemaAttribute;	/* ����schema��δ�����������schema���Խڵ�ָ��ΪNULL */
};
typedef std::list<SOMAttr>				SOMAttrList;
typedef SOMAttrList::iterator			SOMAttrListItor;
typedef SOMAttrList::const_iterator		SOMAttrListConstItor;


class SOMElem
{
public:
	SOMElem()
	{
		m_elementName.Empty();
		m_pSchemaElement = NULL;
	}
	~SOMElem()
	{
	}
	BOOL operator==(const SOMElem& other)
	{
		if (m_elementName == other.m_elementName
			&&m_pSchemaElement == other.m_pSchemaElement)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	CString						m_elementName;
	MSXML2::ISchemaItemPtr		m_pSchemaElement;
};
typedef std::list<SOMElem>				SOMElemList;
typedef SOMElemList::iterator			SOMElemListItor;
typedef SOMElemList::const_iterator		SOMElemListConstItor;

typedef struct processcontext 
{
	
}ProcessContext;


class CXMLSOMElement;
typedef std::tr1::shared_ptr<CXMLSOMElement>		CXMLSOMElementPtr;
class CXMLSOMElement
{
public:
	CXMLSOMElement(void);
	~CXMLSOMElement(void);
	
	/** @brief ��ʼ��SOMԪ��Լ�� */
	BOOL	InitConstraint(MSXML2::IXMLDOMElementPtr pXMLDOMElement);


	/** @brief ��ȡԪ�ؽڵ�������������б� */
	XMLStringList	GetExsitAttributeNameList();
	/** @brief ��ȡԪ�ؽڵ�Ǵ������������б� */
	XMLStringList	GetNonExsitAttributeNameList();

	/** @brief ��ȡԪ��δ���ڵ�ǰ�ֵܽڵ������б� */
	XMLStringList	GetNonExsitPrevElementNameList();
	/** @brief ��ȡԪ��δ���ڵĺ��ֵܽڵ������б� */
	XMLStringList	GetNonExsitNextElementNameList();

	/** @brief ��ȡ��Ԫ��û���ӽڵ�ʱ���Բ�����ӽڵ������б� */
	XMLStringList	GetChildElementNamList();

private:
	BOOL _initWithElement(MSXML2::IXMLDOMElementPtr	pXMLDOMElement);
	BOOL _initWithParentElement(MSXML2::IXMLDOMElementPtr pXMLDOMElement);

	BOOL _processElement(MSXML2::ISchemaElementPtr pSchemaElement);
	BOOL _processType(MSXML2::ISchemaTypePtr	pSchemaType);
	BOOL _processSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType);
	BOOL _processComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType);
	BOOL _processAnyAttribute(MSXML2::ISchemaAnyPtr pSchemaAnyAttribute);
	BOOL _processAttribute(MSXML2::ISchemaAttributePtr pSchemaAttribute);

	BOOL _processModelGroup(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);
	BOOL _processModelGroup_All(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);
	BOOL _processModelGroup_Choice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);
	BOOL _processModelGroup_Sequence(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);

	BOOL _processParentElement(MSXML2::ISchemaElementPtr pSchemaParentElement);
	BOOL _processParentType(MSXML2::ISchemaTypePtr pSchemaParentType);
	BOOL _processParentSimpleType(MSXML2::ISchemaTypePtr pSchemaParentSimpleType);
	BOOL _processParentComplexType(MSXML2::ISchemaComplexTypePtr pSchemaParentComplexType);
	BOOL _processParentModelGroup(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup);

	BOOL _processParentModelGroup_All(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup);
	BOOL _processParentModelGroup_Choice(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup);
	BOOL _processParentModelGroup_Sequence(MSXML2::ISchemaModelGroupPtr pSchemaParentModelGroup);


	/** @brief ���Խڵ��Ƿ��Ѿ������ڴ��������б� */
	BOOL	IncludedByExistAttrList(SOMAttr somAttr);
	/** @brief ���Խڵ��Ƿ��Ѿ������ڷǴ��������б� */
	BOOL	IncludedByNonExistAttrList(SOMAttr somAttr);

	BOOL	IncludeByExistPrevElemList(SOMElem somElem);	/**< @brief �ж�Ԫ���Ƿ���ǰ�ֵ��б��г��� */
	int		OccurCountInPrevElemList(SOMElem somElem);		/**< @brief ��ȡָ��Ԫ����ǰ�ֵ��б��еĳ��ִ��� */
	BOOL	IncludeByExistNextElemList(SOMElem somElem);	/**< @brief �ж�Ԫ���Ƿ��ں��ֵ��б��г��� */
	int		OccurCountInNextElemList(SOMElem somElem);		/**< @brief ��ȡָ��Ԫ���ں��ֵ��б��еĳ��ִ��� */
	BOOL	IsCurrentElem(SOMElem somElem);					/**< @brief �ж�Ԫ���Ƿ��ǵ�ǰԪ�� */

	/** @brief ��ȡָ��Ԫ�ص�ǰ�ĳ��ִ���������ǰ�����ֵ��б��뵱ǰԪ���������� */
	int		OccurCountOfElem(SOMElem somElem);

	/** @brief ��ȡ��ǰ�ڵ��SOMElem */
	SOMElem GetCurrentElem();
	/** @brief ��ȡ��ǰԪ��ǰ�ֵܽڵ��б����Ϊindex��Ԫ�ؽڵ�,index��0��ʼ */
	SOMElem	GetExistPrevElem(int index);
	/** @brief  ��ȡ��ǰԪ�غ��ֵܽڵ��б����Ϊindex��Ԫ�ؽڵ�,index��0��ʼ */
	SOMElem GetExistNextElem(int index);
	/** @brief ��ȡȫ����ǰ���ڵĽڵ��б���Ԫ�ؽڵ���Ŀ */
	int		GetExistElemCount();
	/** @brief ��ȡȫ����ǰ���ڵĽڵ��б������Ϊindex��Ԫ�ؽڵ�,index��0��ʼ */
	SOMElem GetExistElem(int index);

	/** @brief �ж�����schema�����е�Ԫ���Ƿ�һ�� */
	BOOL	_isSameSchemaElem(SOMElem firstElem,SOMElem secondElem);
	/** @brief �ж�����schema�����е������Ƿ�һ�� */
	BOOL	_isSameSchemaAttr(SOMAttr firstAttr,SOMAttr secondAttr);
	/** @brief �ж�SOMElem�Ƿ���modelGroup�ж��� */
	BOOL	_isElemInModelGroup(SOMElem somElem,MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);


	/** @brief ��ȡ<choice>��schema��ʵ���ϵ���С���ִ��� */
	int		_getMinOccurCountOfChoice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);
	/** @brief ��ȡ<choice>�ڵ�ǰʵ�ʳ��ִ��� */
	int		_getOccurCountOfChoice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);

	/** @brief ��ȡ<sequence>��schema��ʵ���ϵ���С���ִ��� */
	int		_getMinOccurCountOfSequence(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);

/**********************************************************************************************/


	/** @brief ��somElem���ظ��ز��뵽somElemListǰ */
	void	_pushFrontDuplicate(SOMElemList& somElemList,SOMElem& somElem);
	/** @brief ��somElem���ظ��ز��뵽somElemList�� */
	void	_pushBackDuplicate(SOMElemList& somElemList,SOMElem& somElem);


	
	
	MSXML2::IXMLDOMElementPtr				m_pXMLDOMElement;			/**< @brief DOMԪ�ؽڵ�ָ�� */
	MSXML2::IXMLDOMElementPtr				m_pXMLDOMParentElement;		/**< @brief ��Ԫ��DOM�ڵ�ָ�� */

	MSXML2::ISchemaElementPtr				m_pSchemaElement;			/**< @brief schemaԪ�ؽڵ�ָ�� */
	MSXML2::ISchemaElementPtr				m_pSchemaParentElement;		/**< @brief schema��Ԫ�ؽڵ�ָ�� */



	/** @brief schema�����и�Ԫ�ص��Ѵ������Խڵ��б� */
	SOMAttrList								m_exsitAttrList;
	/** @brief schema�����и�Ԫ�ص�δ�������Խڵ��б� */
	SOMAttrList								m_nonexistAttrList;


	/** @brief ��Ԫ��֮ǰ�Ѿ����ڵ��ֵܽڵ��б� */
	SOMElemList								m_exsitPrevElemList;
	/** @brief ��Ԫ��֮���Ѿ����ڵ��ֵܽڵ��б� */
	SOMElemList								m_exsitNextElemList;
	/** @brief schema�����и�Ԫ��֮ǰ��δ���ڵ��ֵܽڵ��б� */
	SOMElemList								m_nonExsitPrevElemList;
	/** @brief schema�����и�Ԫ��֮����δ���ڵ��ֵܽڵ��б� */
	SOMElemList								m_nonExsitNextElemList;

	/** @brief schema�����и�Ԫ�ص��ӽڵ�map */
	SOMElemList								m_childElemList;


	/** @brief �Ƿ�ʼ��¼��Ԫ��֮ǰ���Դ��ڵ�Ԫ�ؽڵ� */
	BOOL									m_bPrevRecordStart;
	/** @brief �Ƿ�ʼ��¼��Ԫ��֮����Դ��ڵ�Ԫ�ؽڵ� */
	BOOL									m_bNextRecordStart;

	BOOL									m_bChildRecordStart;

};

