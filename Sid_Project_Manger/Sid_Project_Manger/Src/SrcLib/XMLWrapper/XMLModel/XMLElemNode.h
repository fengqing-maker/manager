#pragma once

#include <map>
#include <list>


#include "XMLDOMNode.h"
#include "XMLSOMElement.h"
#include "XMLSOMAttribute.h"

/* ���model��elem���໥���ù�ϵ��ʹ��Ԥ�������� */
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

/** @brief �������Ͷ��� */
enum AttrType
{
	AttrType_NormalString,		/* һ���ַ��� */
	AttrType_Enum,				/* ö������ */
	AttrType_PatternString,		/* ģʽ�ַ��� */
	AttrType_Reference,			/* ���� */
};

/* ��ֹ��������͸�ֵ���캯�� */
#define DISALLOW_COPY_AND_ASSIGN(TypeName)\
	TypeName(const TypeName&);\
	void operator=(const TypeName&)

class XMLElemNode  : public std::tr1::enable_shared_from_this<XMLElemNode>
{
	/* ˽�л����캯�������Ҫ����Ԫ�ؽڵ���ͨ��XMLModel������ */
	friend class XMLModel;
private:
	XMLElemNode(void);
	
public:
	~XMLElemNode(void);
	/** @brief ��ȡԪ��DOM�ڵ�ָ�� */
	CXMLDOMNodePtr GetDOMNode();
	/** @brief ����Ԫ��DOM�ڵ�ָ�� */
	void SetDOMNode(CXMLDOMNodePtr domNode);
	/** @brief ��ȡԪ������ģ��ָ�� */
	Weak_XMLModel GetOwnerModel();
	/** @brief ����Ԫ������ģ��ָ�� */
	void SetOwnerModel(Weak_XMLModel weak_model);

	/**
	*@brief ��ȡԪ�ؽڵ�����
	*@note	�ڵ����ƽ���Ԫ�ؽڵ�ı��ڲ�DOMָ��ʱ��ʼ��������ʱ���ܸı�
	*/
	CString GetNodeName();
	/** @brief ��ȡԪ�����ƿռ� */
	CString GetNodeNamespace();

	/** @brief ��ȡԪ�ؽڵ���ֵ */
	CString GetNodeValue();
	/** @brief ����Ԫ�ؽڵ���ֵ */
	BOOL SetNodeValue(CString nodeValue);

/*********************************************************************************/
	/**
	*@brief	��ȡ��ǰ�ڵ�ĸ��ڵ�ָ��
	*@return 		��ǰ�ڵ�ĸ��ڵ������������ָ��
	*@see 	XMLElemNode::SetParentNode()
	*/
	Weak_XMLElem GetParentNode();

	/**
	*@brief	��ȡ��ǰ�ڵ���ӽڵ���Ŀ
	*@return 		��ǰ�ڵ��ӽڵ���
	*/
	int GetChildCount();
	/**
	*@brief	��ȡ��ǰ�ڵ���ӽڵ�
	*@param [in]	index	�ӽڵ����,��0��ʼ
	*@return 		��ǰ�ڵ�ĵ�index���ӽڵ�
	*@see 	XMLElemNode::GetChildCount()
	*/
	Weak_XMLElem GetChildNode(int index);

	/** @brief �ڵ�ǰ�ڵ���ӽڵ�ĩ����ӽڵ�,������DOM�� */
	BOOL AppendChild(Weak_XMLElem weak_child);				
	/** @brief �ڵ�ǰ�ڵ�ǰ�����ֵܽڵ㣬������DOM��  */
	BOOL InsertSiblingBefore(Weak_XMLElem weak_sibling);	
	/** @brief �ڵ�ǰ�ڵ������ֵܽڵ㣬������DOM��  */
	BOOL InsertSiblingAfter(Weak_XMLElem weak_sibling);
	/** @brief ɾ����ǰԪ�ؽڵ� */
	BOOL DeleteCurrent();


	/** @brief Ϊ�ڵ����ӽڵ��б�ĩ�����һ���µ��ӽڵ�(���ƿռ��뵱ǰ�ڵ���ͬ) */
	Weak_XMLElem AppendChildElement(CString nodeName);
	/** @brief Ϊ�ڵ����ӽڵ��б�ĩ�����һ���µ��ӽڵ�(ָ�����ƿռ�) */
	Weak_XMLElem AppendChildElement(CString nodeName,CString namespaceURI);
	/** 
	*@brief Ϊ�ڵ����ӽڵ��б�ĩ�����һ���µ��ӽڵ�
	*@note	���µ��ӽڵ��뵱ǰ�ڵ����ڲ�ͬ��ģ��������ӽڵ������ɺ�
	*		���µ��ӽڵ���ӵ���ǰ�ڵ�ģ����
	*/
	BOOL AppendChildElement(Weak_XMLElem weak_node);
	/** @brief ɾ����ǰԪ�ؽڵ��ȫ����Ԫ�ؽڵ� */
	BOOL DeleteChildrenElements();
	/**
	*@brief	�ڵ�ǰ�ڵ�֮ǰ����һ��Ԫ�ؽڵ�
	*@param	[in]	nodeName �²���Ԫ�ر�ǩ���ƣ�QName
	*@return	TRUE:����Ԫ�سɹ���FASLE:����Ԫ��ʧ��
	*@note	�ú���ֻ�������ڵ�ǰԪ��ǰ����һ���븸�ڵ�Ԫ�ؾ�����ͬ���ƿռ��Ԫ�ؽڵ�
	*/
	Weak_XMLElem InsertElementBefore(CString nodeName);
	/**
	*@brief	�ڵ�ǰ�ڵ�֮ǰ����һ��ָ�����ƿռ��Ԫ�ؽڵ�
	*@param	[in]	nodeName �²���Ԫ�ر�ǩ����,QName
	*@param	[in]	namespaceURI �²���Ԫ�����ƿռ䣬
	*@return	TRUE:������Ԫ�سɹ���FALSE:������Ԫ��ʧ��		
	*@note	�ú�������Ϊ��Ԫ��ָ��һ�������������ƿռ䲻ͬ�����ƿռ�
	*/
	Weak_XMLElem InsertElementBefore(CString nodeName,CString namespaceURI);
	/**
	*@brief	�ڵ�ǰ�ڵ�֮�����һ��Ԫ�ؽڵ�
	*@param	[in]	nodeName �²���Ԫ�ر�ǩ���ƣ�QName
	*@return	TRUE:����Ԫ�سɹ���FASLE:����Ԫ��ʧ��
	*@note	�ú���ֻ�������ڵ�ǰԪ�غ����һ���븸�ڵ�Ԫ�ؾ�����ͬ���ƿռ��Ԫ�ؽڵ�
	*/
	Weak_XMLElem InsertElementAfter(CString nodeName);
	/**
	*@brief	�ڵ�ǰ�ڵ�֮�����һ��ָ�����ƿռ��Ԫ�ؽڵ�
	*@param	[in]	nodeName �²���Ԫ�ر�ǩ����,QName
	*@param	[in]	namespaceURI �²���Ԫ�����ƿռ䣬
	*@return	TRUE:������Ԫ�سɹ���FALSE:������Ԫ��ʧ��		
	*@note	�ú�������Ϊ��Ԫ��ָ��һ�������������ƿռ䲻ͬ�����ƿռ�
	*/
	Weak_XMLElem InsertElementAfter(CString nodeName,CString namespaceURI);
	/** @brief ɾ����ǰ�ڵ㣬�����ͷ��ڴ�ռ� */
	BOOL DeleteCurrentElement();
	

/**********************************************************************************/	
	/** @brief schemaУ�鵱ǰ�ڵ� */
	BOOL ValidateElement(XMLStringList& errorInfoList);

	/** @brief ��ȡ��ǰԪ�ص���Ӧ��SchemaԼ�� */
	CXMLSOMElementPtr GetElementConstraint();
	/** @brief ��ȡ��ǰԪ��֮ǰ����δ���ڵ��ֵ�Ԫ�������б�*/
	XMLStringList GetNonExsitPrevElementList();
	/** @brief ��ȡ��ǰԪ��֮�����δ���ڵ��ֵ�Ԫ�������б� */
	XMLStringList GetNonExsitNextElementList();
	/** @brief ��ȡ��Ԫ����û����Ԫ��ʱ���Դ��ڵ���Ԫ���б� */
	XMLStringList GetAllowedChildElementList();

/**********************************************************************************/
	/**
	*@brief	����ָ�����Ƶ����Խڵ�
	*@note	��ָ�����Ƶ����Խڵ��Ѵ��������ʧ��
	*/
	BOOL AddAttributeNode(CString attrName);
	/** 
	*@brief	����ָ�����ƵĵĴ�ǰ׺�����Խڵ�  
	*@note	ָ����ǰ׺����������ʱҪ��ͬʱָ�����ƿռ�RUI�������ƿռ�����ڸ��ڵ����ж���
	*/
	BOOL AddAttributeNode(CString attrName,CString namespaceURI);
	/** 
	*@brief	����ָ�����ƵĵĴ�ǰ׺�����Խڵ�,����������ָ��������ֵ 
	*@note	ָ����ǰ׺����������ʱҪ��ͬʱָ�����ƿռ�RUI�������ƿռ�����ڸ��ڵ����ж���
	*/
	BOOL AddAttributeNode(CString attrName,CString namespaceURI,CString attrValue);
	/**
	*@brief	ɾ��ָ�������Խڵ�
	*@note	��ָ�����Ƶ����Խڵ㲻���������ʧ��
	*/
	BOOL RemoveAttributeNode(CString attrName);

	/** @brief ��ȡ�ڵ���е����Խڵ�����*/
	int	GetAttributeCount();
	/** @brief ��ȡ��ǰ�ڵ��index�����Խڵ���������� */
	CString			GetAttributeName(int index);

	/** @brief ��ȡ��ǰ�ڵ��index�����Խڵ������ֵ */
	CString			GetAttributeValue(int index);
	/** @brief ��ȡ��ǰ�ڵ���е�ָ���������Խڵ������ֵ */
	CString			GetAttributeValue(CString attrName);

	/** @brief ���õ�ǰ�ڵ��index�����Խڵ������ֵ */
	BOOL			SetAttributeValue(int index,CString attrValue);
	/** @brief ���õ�ǰ�ڵ���е�ָ���������Խڵ������ֵ */
	BOOL			SetAttributeValue(CString attrName,CString attrValue);

/***********************************************************************************/

	/** @brief ��֤��ǰ�ڵ��index�����Խڵ������ֵ */
	BOOL			ValidateAttribute(int index,XMLStringList& errorInfoList);
	/** @brief ��֤��ǰ�ڵ����ָ�����Ƶ����Խڵ������ֵ */
	BOOL			ValidateAttribute(CString attrName,XMLStringList& errorInfoList);

	/** @brief ��ȡ��ǰ�ڵ��index�����Խڵ��SchemaԼ�� */
	CXMLSOMAttributePtr GetAttributeConstraint(int index);
	/** @brief ��ȡ��ǰ�ڵ����ָ�����Ƶ����Խڵ��SchemaԼ�� */
	CXMLSOMAttributePtr GetAttributeConstraint(CString attrName);
	/** @brief ��ȡ��ǰ�ڵ��index�����Խڵ��ö��ֵ�б� */
	XMLStringList	GetAttributeValueEnumList(int index);
	/** @brief ��ȡ��ǰ�ڵ����ָ�����Ƶ����Խڵ��ö��ֵ�б� */
	XMLStringList	GetAttributeValueEnumList(CString attrName);
	/** @brief ��ȡ��ǰ�ڵ��index�����Խڵ������ֵ�б� */
	XMLStringList	GetAttributeValueRefList(int index);
	/** @brief ��ȡ��ǰ�ڵ����ָ�����Ƶ����Խڵ������ֵ�б� */
	XMLStringList	GetAttributeValueRefList(CString attrName);


	/** @brief ��ȡ��ǰ�ڵ����Ѵ��ڵ������б� */
	XMLStringList	GetExistentAttributeList();
	/** @brief ��ȡ��ǰ�ڵ���δ���ڵ������б� */
	XMLStringList	GetNonExistentAttributeList();
protected:



private:
	DISALLOW_COPY_AND_ASSIGN(XMLElemNode);

	/** @brief Ϊ���Է��㣬��ȡ��Ԫ�ؽڵ������ */
	CString						   m_elemName;
	
	/** @brief ����ģ��ָ�� */
	Weak_XMLModel					m_ownerModel;
	/** @brief ��Ԫ�ؽڵ�ָ�� */
	Weak_XMLElem					m_parent;
	/** @brief ��Ԫ�ؽڵ�ָ���б� */
	Weak_XMLElemList				m_childList;


	/** @brief Ԫ��DOM�ڵ� */
	CXMLDOMNodePtr					m_domNode;

};

