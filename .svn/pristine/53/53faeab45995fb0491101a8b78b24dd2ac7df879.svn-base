#pragma once

#include <map>
#include <list>


#include "XMLDOMNode.h"
#include "XMLSOMElement.h"
#include "XMLSOMAttribute.h"

/* 解除model与elem的相互引用关系，使用预定义声明 */
class XMLModel;
typedef	std::tr1::shared_ptr<XMLModel>				Shared_XMLModel;
typedef std::tr1::weak_ptr<XMLModel>				Weak_XMLModel;

class XMLElemNode;
typedef std::tr1::shared_ptr<XMLElemNode>			Shared_XMLElem;
typedef std::tr1::weak_ptr<XMLElemNode>				Weak_XMLElem;

typedef std::map<CString,Shared_XMLElem>			Shared_XMLElemMap;
typedef Shared_XMLElemMap::iterator					Shared_XMLElemMapItor;
typedef Shared_XMLElemMap::const_iterator			Shared_XMLElemMapConstItor;

typedef std::map<CString,Weak_XMLElem>				Weak_XMLElemMap;
typedef Weak_XMLElemMap::iterator					Weak_XMLElemMapItor;
typedef Weak_XMLElemMap::const_iterator				Weak_XMLElemMapConstItor;

typedef std::list<Shared_XMLElem>					Shared_XMLElemList;
typedef Shared_XMLElemList::iterator				Shared_XMLElemListItor;
typedef Shared_XMLElemList::const_iterator			Shared_XMLElemListConstItor;

typedef std::list<Weak_XMLElem>						Weak_XMLElemList;
typedef Weak_XMLElemList::iterator					Weak_XMLElemListItor;
typedef Weak_XMLElemList::const_iterator			Weak_XMLElemListConstItor;

/** @brief 属性类型定义 */
enum AttrType
{
	AttrType_NormalString,		/* 一般字符串 */
	AttrType_Enum,				/* 枚举类型 */
	AttrType_PatternString,		/* 模式字符串 */
	AttrType_Reference,			/* 引用 */
};

/* 禁止拷贝构造和赋值构造函数 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName)\
	TypeName(const TypeName&);\
	void operator=(const TypeName&)

class XMLElemNode  : public std::tr1::enable_shared_from_this<XMLElemNode>
{
	/* 私有化构造函数，如果要生成元素节点则通过XMLModel创建。 */
	friend class XMLModel;
private:
	XMLElemNode(void);
	
public:
	~XMLElemNode(void);
	/** @brief 获取元素DOM节点指针 */
	CXMLDOMNodePtr GetDOMNode();
	/** @brief 设置元素DOM节点指针 */
	void SetDOMNode(CXMLDOMNodePtr domNode);
	/** @brief 获取元素所在模型指针 */
	Weak_XMLModel GetOwnerModel();
	/** @brief 设置元素所在模型指针 */
	void SetOwnerModel(Weak_XMLModel weak_model);

	/**
	*@brief 获取元素节点名称
	*@note	节点名称仅在元素节点改变内部DOM指针时初始化，其他时候不能改变
	*/
	CString GetNodeName();
	/** @brief 获取元素名称空间 */
	CString GetNodeNamespace();

	/** @brief 获取元素节点数值 */
	CString GetNodeValue();
	/** @brief 设置元素节点数值 */
	BOOL SetNodeValue(CString nodeValue);

/*********************************************************************************/
	/**
	*@brief	获取当前节点的父节点指针
	*@return 		当前节点的父节点的弱引用智能指针
	*@see 	XMLElemNode::SetParentNode()
	*/
	Weak_XMLElem GetParentNode();

	/**
	*@brief	获取当前节点的子节点数目
	*@return 		当前节点子节点数
	*/
	int GetChildCount();
	/**
	*@brief	获取当前节点的子节点
	*@param [in]	index	子节点序号,从0开始
	*@return 		当前节点的第index个子节点
	*@see 	XMLElemNode::GetChildCount()
	*/
	Weak_XMLElem GetChildNode(int index);

	/** @brief 在当前节点的子节点末端添加节点,不操作DOM树 */
	BOOL AppendChild(Weak_XMLElem weak_child);				
	/** @brief 在当前节点前插入兄弟节点，不操作DOM树  */
	BOOL InsertSiblingBefore(Weak_XMLElem weak_sibling);	
	/** @brief 在当前节点后插入兄弟节点，不操作DOM树  */
	BOOL InsertSiblingAfter(Weak_XMLElem weak_sibling);
	/** @brief 删除当前元素节点 */
	BOOL DeleteCurrent();


	/** @brief 为节点在子节点列表末端添加一个新的子节点(名称空间与当前节点相同) */
	Weak_XMLElem AppendChildElement(CString nodeName);
	/** @brief 为节点在子节点列表末端添加一个新的子节点(指定名称空间) */
	Weak_XMLElem AppendChildElement(CString nodeName,CString namespaceURI);
	/** 
	*@brief 为节点在子节点列表末端添加一个新的子节点
	*@note	若新的子节点与当前节点属于不同的模型则添加子节点操作完成后
	*		将新的子节点添加到当前节点模型中
	*/
	BOOL AppendChildElement(Weak_XMLElem weak_node);
	/** @brief 删除当前元素节点的全部子元素节点 */
	BOOL DeleteChildrenElements();
	/**
	*@brief	在当前节点之前插入一个元素节点
	*@param	[in]	nodeName 新插入元素标签名称，QName
	*@return	TRUE:插入元素成功，FASLE:插入元素失败
	*@note	该函数只能用于在当前元素前插入一个与父节点元素具有相同名称空间的元素节点
	*/
	Weak_XMLElem InsertElementBefore(CString nodeName);
	/**
	*@brief	在当前节点之前插入一个指定名称空间的元素节点
	*@param	[in]	nodeName 新插入元素标签名称,QName
	*@param	[in]	namespaceURI 新插入元素名称空间，
	*@return	TRUE:插入新元素成功；FALSE:插入新元素失败		
	*@note	该函数可以为新元素指定一个与上下文名称空间不同的名称空间
	*/
	Weak_XMLElem InsertElementBefore(CString nodeName,CString namespaceURI);
	/**
	*@brief	在当前节点之后插入一个元素节点
	*@param	[in]	nodeName 新插入元素标签名称，QName
	*@return	TRUE:插入元素成功，FASLE:插入元素失败
	*@note	该函数只能用于在当前元素后插入一个与父节点元素具有相同名称空间的元素节点
	*/
	Weak_XMLElem InsertElementAfter(CString nodeName);
	/**
	*@brief	在当前节点之后插入一个指定名称空间的元素节点
	*@param	[in]	nodeName 新插入元素标签名称,QName
	*@param	[in]	namespaceURI 新插入元素名称空间，
	*@return	TRUE:插入新元素成功；FALSE:插入新元素失败		
	*@note	该函数可以为新元素指定一个与上下文名称空间不同的名称空间
	*/
	Weak_XMLElem InsertElementAfter(CString nodeName,CString namespaceURI);
	/** @brief 删除当前节点，包括释放内存空间 */
	BOOL DeleteCurrentElement();
	

/**********************************************************************************/	
	/** @brief schema校验当前节点 */
	BOOL ValidateElement(XMLStringList& errorInfoList);

	/** @brief 获取当前元素的相应的Schema约束 */
	CXMLSOMElementPtr GetElementConstraint();
	/** @brief 获取当前元素之前的尚未存在的兄弟元素名称列表*/
	XMLStringList GetNonExsitPrevElementList();
	/** @brief 获取当前元素之后的尚未存在的兄弟元素名称列表 */
	XMLStringList GetNonExsitNextElementList();
	/** @brief 获取当元素下没有子元素时可以存在的子元素列表 */
	XMLStringList GetAllowedChildElementList();

/**********************************************************************************/
	/**
	*@brief	增加指定名称的属性节点
	*@note	若指定名称的属性节点已存在则操作失败
	*/
	BOOL AddAttributeNode(CString attrName);
	/** 
	*@brief	增加指定名称的的带前缀的属性节点  
	*@note	指定带前缀的属性名称时要求同时指定名称空间RUI，且名称空间必须在根节点中有定义
	*/
	BOOL AddAttributeNode(CString attrName,CString namespaceURI);
	/** 
	*@brief	增加指定名称的的带前缀的属性节点,并赋予属性指定的属性值 
	*@note	指定带前缀的属性名称时要求同时指定名称空间RUI，且名称空间必须在根节点中有定义
	*/
	BOOL AddAttributeNode(CString attrName,CString namespaceURI,CString attrValue);
	/**
	*@brief	删除指定的属性节点
	*@note	若指定名称的属性节点不存在则操作失败
	*/
	BOOL RemoveAttributeNode(CString attrName);

	/** @brief 获取节点具有的属性节点数量*/
	int	GetAttributeCount();
	/** @brief 获取当前节点第index个属性节点的属性名称 */
	CString			GetAttributeName(int index);

	/** @brief 获取当前节点第index个属性节点的属性值 */
	CString			GetAttributeValue(int index);
	/** @brief 获取当前节点具有的指定名称属性节点的属性值 */
	CString			GetAttributeValue(CString attrName);

	/** @brief 设置当前节点第index个属性节点的属性值 */
	BOOL			SetAttributeValue(int index,CString attrValue);
	/** @brief 设置当前节点具有的指定名称属性节点的属性值 */
	BOOL			SetAttributeValue(CString attrName,CString attrValue);

/***********************************************************************************/

	/** @brief 验证当前节点第index个属性节点的属性值 */
	BOOL			ValidateAttribute(int index,XMLStringList& errorInfoList);
	/** @brief 验证当前节点具有指定名称的属性节点的属性值 */
	BOOL			ValidateAttribute(CString attrName,XMLStringList& errorInfoList);

	/** @brief 获取当前节点第index个属性节点的Schema约束 */
	CXMLSOMAttributePtr GetAttributeConstraint(int index);
	/** @brief 获取当前节点具有指定名称的属性节点的Schema约束 */
	CXMLSOMAttributePtr GetAttributeConstraint(CString attrName);
	/** @brief 获取当前节点第index个属性节点的枚举值列表 */
	XMLStringList	GetAttributeValueEnumList(int index);
	/** @brief 获取当前节点具有指定名称的属性节点的枚举值列表 */
	XMLStringList	GetAttributeValueEnumList(CString attrName);
	/** @brief 获取当前节点第index个属性节点的引用值列表 */
	XMLStringList	GetAttributeValueRefList(int index);
	/** @brief 获取当前节点具有指定名称的属性节点的引用值列表 */
	XMLStringList	GetAttributeValueRefList(CString attrName);


	/** @brief 获取当前节点下已存在的属性列表 */
	XMLStringList	GetExistentAttributeList();
	/** @brief 获取当前节点下未存在的属性列表 */
	XMLStringList	GetNonExistentAttributeList();
protected:



private:
	DISALLOW_COPY_AND_ASSIGN(XMLElemNode);

	/** @brief 为调试方便，获取该元素节点的名称 */
	CString						   m_elemName;
	
	/** @brief 所在模型指针 */
	Weak_XMLModel					m_ownerModel;
	/** @brief 父元素节点指针 */
	Weak_XMLElem					m_parent;
	/** @brief 子元素节点指针列表 */
	Weak_XMLElemList				m_childList;


	/** @brief 元素DOM节点 */
	CXMLDOMNodePtr					m_domNode;

};

