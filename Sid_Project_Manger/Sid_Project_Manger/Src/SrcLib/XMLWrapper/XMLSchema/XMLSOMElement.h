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
	
	MSXML2::ISchemaItemPtr		m_pSchemaAttribute;	/* 对于schema中未定义的属性其schema属性节点指针为NULL */
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
	
	/** @brief 初始化SOM元素约束 */
	BOOL	InitConstraint(MSXML2::IXMLDOMElementPtr pXMLDOMElement);


	/** @brief 获取元素节点存在属性名称列表 */
	XMLStringList	GetExsitAttributeNameList();
	/** @brief 获取元素节点非存在属性名称列表 */
	XMLStringList	GetNonExsitAttributeNameList();

	/** @brief 获取元素未存在的前兄弟节点名称列表 */
	XMLStringList	GetNonExsitPrevElementNameList();
	/** @brief 获取元素未存在的后兄弟节点名称列表 */
	XMLStringList	GetNonExsitNextElementNameList();

	/** @brief 获取当元素没有子节点时可以插入的子节点名称列表 */
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


	/** @brief 属性节点是否已经包含于存在属性列表 */
	BOOL	IncludedByExistAttrList(SOMAttr somAttr);
	/** @brief 属性节点是否已经包含于非存在属性列表 */
	BOOL	IncludedByNonExistAttrList(SOMAttr somAttr);

	BOOL	IncludeByExistPrevElemList(SOMElem somElem);	/**< @brief 判断元素是否在前兄弟列表中出现 */
	int		OccurCountInPrevElemList(SOMElem somElem);		/**< @brief 获取指定元素在前兄弟列表中的出现次数 */
	BOOL	IncludeByExistNextElemList(SOMElem somElem);	/**< @brief 判断元素是否在后兄弟列表中出现 */
	int		OccurCountInNextElemList(SOMElem somElem);		/**< @brief 获取指定元素在后兄弟列表中的出现次数 */
	BOOL	IsCurrentElem(SOMElem somElem);					/**< @brief 判断元素是否是当前元素 */

	/** @brief 获取指定元素当前的出现次数，包括前、后兄弟列表与当前元素自身在内 */
	int		OccurCountOfElem(SOMElem somElem);

	/** @brief 获取当前节点的SOMElem */
	SOMElem GetCurrentElem();
	/** @brief 获取当前元素前兄弟节点列表序号为index的元素节点,index从0开始 */
	SOMElem	GetExistPrevElem(int index);
	/** @brief  获取当前元素后兄弟节点列表序号为index的元素节点,index从0开始 */
	SOMElem GetExistNextElem(int index);
	/** @brief 获取全部当前存在的节点列表中元素节点数目 */
	int		GetExistElemCount();
	/** @brief 获取全部当前存在的节点列表中序号为index的元素节点,index从0开始 */
	SOMElem GetExistElem(int index);

	/** @brief 判断两个schema定义中的元素是否一样 */
	BOOL	_isSameSchemaElem(SOMElem firstElem,SOMElem secondElem);
	/** @brief 判断两个schema定义中的属性是否一样 */
	BOOL	_isSameSchemaAttr(SOMAttr firstAttr,SOMAttr secondAttr);
	/** @brief 判断SOMElem是否在modelGroup中定义 */
	BOOL	_isElemInModelGroup(SOMElem somElem,MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);


	/** @brief 获取<choice>在schema中实际上的最小出现次数 */
	int		_getMinOccurCountOfChoice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);
	/** @brief 获取<choice>在当前实际出现次数 */
	int		_getOccurCountOfChoice(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);

	/** @brief 获取<sequence>在schema中实际上的最小出现次数 */
	int		_getMinOccurCountOfSequence(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);

/**********************************************************************************************/


	/** @brief 将somElem不重复地插入到somElemList前 */
	void	_pushFrontDuplicate(SOMElemList& somElemList,SOMElem& somElem);
	/** @brief 将somElem不重复地插入到somElemList后 */
	void	_pushBackDuplicate(SOMElemList& somElemList,SOMElem& somElem);


	
	
	MSXML2::IXMLDOMElementPtr				m_pXMLDOMElement;			/**< @brief DOM元素节点指针 */
	MSXML2::IXMLDOMElementPtr				m_pXMLDOMParentElement;		/**< @brief 父元素DOM节点指针 */

	MSXML2::ISchemaElementPtr				m_pSchemaElement;			/**< @brief schema元素节点指针 */
	MSXML2::ISchemaElementPtr				m_pSchemaParentElement;		/**< @brief schema父元素节点指针 */



	/** @brief schema定义中该元素的已存在属性节点列表 */
	SOMAttrList								m_exsitAttrList;
	/** @brief schema定义中该元素的未存在属性节点列表 */
	SOMAttrList								m_nonexistAttrList;


	/** @brief 该元素之前已经存在的兄弟节点列表 */
	SOMElemList								m_exsitPrevElemList;
	/** @brief 该元素之后已经存在的兄弟节点列表 */
	SOMElemList								m_exsitNextElemList;
	/** @brief schema定义中该元素之前尚未存在的兄弟节点列表 */
	SOMElemList								m_nonExsitPrevElemList;
	/** @brief schema定义中该元素之后尚未存在的兄弟节点列表 */
	SOMElemList								m_nonExsitNextElemList;

	/** @brief schema定义中该元素的子节点map */
	SOMElemList								m_childElemList;


	/** @brief 是否开始记录该元素之前可以存在的元素节点 */
	BOOL									m_bPrevRecordStart;
	/** @brief 是否开始记录该元素之后可以存在的元素节点 */
	BOOL									m_bNextRecordStart;

	BOOL									m_bChildRecordStart;

};

