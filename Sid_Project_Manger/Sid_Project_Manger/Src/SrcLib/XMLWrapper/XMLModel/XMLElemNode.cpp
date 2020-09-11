#include "stdafx.h"
#include "XMLElemNode.h"

#include "XMLModel.h"	/* 解除model与elem的相互引用关系，使用预定义声明 */

#ifdef _DEBUG                      	/* 判断是否定义_DEBUG */
#undef THIS_FILE                   	/* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;  	/* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              	/* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */


XMLElemNode::XMLElemNode(void)
{
	m_domNode.reset();
}
XMLElemNode::~XMLElemNode(void)
{
	//this->CleanUp();
}

CXMLDOMNodePtr XMLElemNode::GetDOMNode()
{
	return m_domNode;
}
void XMLElemNode::SetDOMNode(CXMLDOMNodePtr domNode)
{
	if (m_domNode != domNode)
	{
		m_domNode = domNode;
		m_elemName = m_domNode->GetNodeName();
	}
}
Weak_XMLModel XMLElemNode::GetOwnerModel()
{
	return m_ownerModel;
}
void XMLElemNode::SetOwnerModel(Weak_XMLModel weak_model)
{
	m_ownerModel = weak_model;
}

CString XMLElemNode::GetNodeName()
{
	CString nodeName;
	if (m_domNode != 0)
	{
		nodeName = m_domNode->GetNodeName();
	}
	return nodeName;
}
CString XMLElemNode::GetNodeNamespace()
{
	CString nodeNamespace;
	if (m_domNode != 0)
	{
		nodeNamespace = m_domNode->GetNodeNamespace();
	}
	return nodeNamespace;
}

CString XMLElemNode::GetNodeValue()
{
	return m_domNode->GetNodeValue();
}
BOOL XMLElemNode::SetNodeValue(CString nodeValue)
{
	if (m_domNode == NULL)
		return FALSE;
	return m_domNode->SetNodeValue(nodeValue);
}

/******************************************************************************/
Weak_XMLElem XMLElemNode::GetParentNode()
{
	return m_parent;
}

int XMLElemNode::GetChildCount()
{
	int childCount = 0;
	if (m_domNode == NULL)
	{
		return childCount;
	}
	childCount = m_domNode->GetChildCount();
	ASSERT(m_childList.size() == childCount);
	return childCount;
}
Weak_XMLElem XMLElemNode::GetChildNode(int index)
{
	Weak_XMLElem weak_child;
	ASSERT(index < (int)m_childList.size() && index >= 0);
	Weak_XMLElemListItor itor = m_childList.begin();
	int curIndex = 0;
	while(itor != m_childList.end())
	{
		if (curIndex == index)
		{
			weak_child = *itor;
			break;
		}
		curIndex++;
		itor++;
	}
	Shared_XMLElem shared_child = weak_child.lock();
	CXMLDOMNodePtr childDom1 = shared_child->m_domNode;
	CXMLDOMNodePtr childDom2 = m_domNode->GetChildNode(index);
	ASSERT(childDom1 == childDom2);

	return weak_child;
}

BOOL XMLElemNode::AppendChild(Weak_XMLElem weak_child)
{
	Shared_XMLElem shared_child = weak_child.lock();
	if (shared_child == 0)
	{
		return FALSE;
	}
	/* 修改子节点原始父节点的子节点列表 */
	Shared_XMLElem shared_oldParent = shared_child->m_parent.lock();
	Shared_XMLElem shared_this = shared_from_this();
	if (shared_oldParent != 0)
	{
		Weak_XMLElemListItor oldSiblingItor = shared_oldParent->m_childList.begin();
		while (oldSiblingItor != shared_oldParent->m_childList.end())
		{
			Shared_XMLElem shared_oldSibling = (*oldSiblingItor).lock();
			if (shared_oldSibling == shared_child)
			{
				shared_oldParent->m_childList.erase(oldSiblingItor);
				break;
			}
			oldSiblingItor++;
		}
	}
	shared_child->m_parent = shared_this;
	/* 修改当前节点的子节点列表 */
	shared_this->m_childList.push_back(shared_child);
	/* 修改子节点所属模型 */
	Shared_XMLModel shared_model = m_ownerModel.lock();
	ASSERT(shared_model != 0);
	shared_model->ImportXMLElem(weak_child);
	return TRUE;
}
BOOL XMLElemNode::InsertSiblingBefore(Weak_XMLElem weak_sibling)
{
	/* 修改兄弟节点原始父节点的子节点列表 */
	Shared_XMLElem shared_sibling = weak_sibling.lock();
	Shared_XMLElem shared_oldParent = shared_sibling->m_parent.lock();
	if (shared_oldParent != 0)
	{
		Weak_XMLElemListItor oldItor = shared_oldParent->m_childList.begin();
		while (oldItor != shared_oldParent->m_childList.end())
		{
			Shared_XMLElem shared_temp = (*oldItor).lock();
			if (shared_temp == shared_sibling)
			{
				shared_oldParent->m_childList.erase(oldItor);
				break;
			}
			oldItor++;
		}
	}
	/* 重置父节点 */
	Shared_XMLElem shared_parent = m_parent.lock();
	ASSERT(shared_parent != 0);
	shared_sibling->m_parent = shared_parent;
	/* 将其插入父节点列表 */
	Weak_XMLElemListItor newItor = shared_parent->m_childList.begin();
	while (newItor != shared_parent->m_childList.end())
	{
		Shared_XMLElem shared_temp = (*newItor).lock();
		if (shared_temp == shared_from_this())
		{
			/* 将新的兄弟节点插入到该节点在父节点的子节点列表中的位置之前 */
			Weak_XMLElemListItor resultItor = shared_parent->m_childList.insert(newItor,shared_sibling);
			ASSERT(resultItor != shared_parent->m_childList.end());
			break;
		}
		newItor++;
	}
	/* 修改兄弟节点所属模型 */
	Shared_XMLModel shared_model = shared_parent->GetOwnerModel().lock();
	ASSERT(shared_model != 0);
	shared_model->ImportXMLElem(weak_sibling);
	return TRUE;
}
BOOL XMLElemNode::InsertSiblingAfter(Weak_XMLElem weak_sibling)
{
	/* 清除原始父子关系 */
	Shared_XMLElem shared_sibling = weak_sibling.lock();
	Shared_XMLElem shared_oldParent = shared_sibling->m_parent.lock();
	if (shared_oldParent != 0)
	{
		Weak_XMLElemListItor oldItor = shared_oldParent->m_childList.begin();
		while (oldItor != shared_oldParent->m_childList.end())
		{
			Shared_XMLElem shared_temp = (*oldItor).lock();
			if (shared_temp == shared_sibling)
			{
				shared_oldParent->m_childList.erase(oldItor);
				break;
			}
			oldItor++;
		}
	}
	/* 重置父节点 */
	Shared_XMLElem shared_parent = m_parent.lock();
	ASSERT(shared_parent != 0);
	shared_sibling->m_parent = shared_parent;
	/* 将其插入父节点列表 */
	Weak_XMLElemListItor newItor = shared_parent->m_childList.begin();
	while (newItor != shared_parent->m_childList.end())
	{
		Shared_XMLElem shared_temp = (*newItor).lock();
		if (shared_temp == shared_from_this())
		{
			/* 将新的兄弟节点插入到该节点在父节点的子节点列表中的位置之后 */
			Weak_XMLElemListItor resultItor = shared_parent->m_childList.insert(++newItor,shared_sibling);
			ASSERT(resultItor != shared_parent->m_childList.end());
			break;
		}
		newItor++;
	}
	/* 修改兄弟节点所属模型 */
	Shared_XMLModel shared_model = shared_parent->GetOwnerModel().lock();
	ASSERT(shared_model != 0);
	shared_model->ImportXMLElem(weak_sibling);
	return TRUE;
}
BOOL XMLElemNode::DeleteCurrent()
{
	/* 清空当前节点的父节点信息 */
	Shared_XMLElem shared_parent = this->m_parent.lock();
	Shared_XMLElem shared_this = shared_from_this();
	if (shared_parent != 0)
	{
		Weak_XMLElemListItor oldItor = shared_parent->m_childList.begin();
		while (oldItor != shared_parent->m_childList.end())
		{
			Shared_XMLElem shared_temp = (*oldItor).lock();
			if (shared_temp == shared_this)
			{
				shared_parent->m_childList.erase(oldItor);
				break;
			}
			oldItor++;
		}
	}
	this->m_parent.reset();
	/* 修改子节点 */
	Weak_XMLElemListItor childItor = this->m_childList.begin();
	while (childItor != this->m_childList.end())
	{
		Shared_XMLElem shared_child = (*childItor).lock();
		if (shared_child != 0)
		{
			/* 子节点未释放,清空子节点的父节点信息 */
			shared_child->m_parent.reset();
		}
	}
	this->m_childList.clear();
	/* 将当前节点从模型中删除 */
	Shared_XMLModel shared_model = m_ownerModel.lock();
	ASSERT(shared_model != 0);
	shared_model->DeleteXMLElem(shared_this);
	return TRUE;
}

Weak_XMLElem XMLElemNode::AppendChildElement(CString nodeName)
{
	Weak_XMLElem weak_newElem;
	if (m_domNode == NULL || nodeName.IsEmpty())
	{
		return weak_newElem;
	}
	CXMLDOMNodePtr pNewElem = m_domNode->AppendChildNode(nodeName);
	if (pNewElem == NULL)
	{
		return weak_newElem;
	}
	Shared_XMLModel shared_model = m_ownerModel.lock();
	Shared_XMLElem shared_newElem = shared_model->CreateXMLElem(pNewElem);
	if(!this->AppendChild(shared_newElem))
	{
		return weak_newElem;
	}
	weak_newElem = shared_newElem;
	return weak_newElem;
}
Weak_XMLElem XMLElemNode::AppendChildElement(CString nodeName,CString namespaceURI)
{
	Weak_XMLElem weak_newElem;
	if (m_domNode == NULL || nodeName.IsEmpty())
	{
		return weak_newElem;
	}
	CXMLDOMNodePtr pNewElem = m_domNode->AppendChildNode(nodeName,namespaceURI);
	if (pNewElem == NULL)
	{
		return weak_newElem;
	}
	Shared_XMLModel shared_model = m_ownerModel.lock();
	Shared_XMLElem shared_newElem = shared_model->CreateXMLElem(pNewElem);
	if(!this->AppendChild(shared_newElem))
	{
		return weak_newElem;
	}
	weak_newElem = shared_newElem;
	return weak_newElem;
}
BOOL XMLElemNode::AppendChildElement(Weak_XMLElem weak_node)
{
	Shared_XMLElem shared_node = weak_node.lock();
	if (shared_node == 0 || m_domNode == NULL)
	{
		return FALSE;
	}
	CXMLDOMNodePtr domChild = shared_node->m_domNode;
	if(!m_domNode->AppendChildNode(domChild))
	{
		return FALSE;
	}
	if (!this->AppendChild(shared_node))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL XMLElemNode::DeleteChildrenElements()
{
	if (m_domNode == NULL)
	{
		return FALSE;
	}
	Weak_XMLElemListItor childItor = m_childList.begin();
	while (childItor != m_childList.end())
	{
		Weak_XMLElem weak_child = (*childItor).lock();
		{
			Shared_XMLElem shared_child = weak_child.lock();
			ASSERT(shared_child != 0);
			VERIFY(shared_child->DeleteCurrentElement());
			/* 子节点删除成功后应该重置子节点迭代器指针 */
			childItor = m_childList.begin();
		}
		/* 验证子节点内存释放成功 */
		ASSERT(weak_child.lock() == 0);
	}
	return TRUE;
}

Weak_XMLElem XMLElemNode::InsertElementBefore(CString nodeName)
{
	Weak_XMLElem weak_sibling;
	if (m_domNode == NULL)
		return weak_sibling;
	
	/* dom树插入，默认名称空间 */
	CXMLDOMNodePtr siblingDom = m_domNode->InsertSiblingBefore(nodeName);
	ASSERT(siblingDom != NULL);
	if (siblingDom == NULL)
	{
		return weak_sibling;
	}
	Shared_XMLModel shared_model = m_ownerModel.lock();
	Shared_XMLElem shared_sibling = shared_model->CreateXMLElem(siblingDom);
	/* 模型插入 */
	BOOL result = InsertSiblingBefore(shared_sibling);
	ASSERT(result);
	if (result == FALSE)
		return weak_sibling;
	weak_sibling = shared_sibling;
	return weak_sibling;
}
Weak_XMLElem XMLElemNode::InsertElementBefore(CString nodeName,CString namespaceURI)
{
	Weak_XMLElem weak_sibling;
	if (m_domNode == NULL)
		return weak_sibling;
	/* dom树插入,指定名称空间 */
	CXMLDOMNodePtr siblingDom = m_domNode->InsertSiblingBefore(nodeName,namespaceURI);
	ASSERT(siblingDom != NULL);
	if (siblingDom == NULL)
	{
		return weak_sibling;
	} 
	Shared_XMLModel shared_model = m_ownerModel.lock();
	Shared_XMLElem shared_sibling = shared_model->CreateXMLElem(siblingDom);
	/* 模型插入 */
	BOOL result = InsertSiblingBefore(shared_sibling);
	ASSERT(result);
	if (result == FALSE)
		return weak_sibling;
	weak_sibling = shared_sibling;
	return weak_sibling;
}
Weak_XMLElem XMLElemNode::InsertElementAfter(CString nodeName)
{
	Weak_XMLElem weak_sibling;
	if (m_domNode == NULL)
		return weak_sibling;
	/* dom树插入,使用默认名称空间 */
	CXMLDOMNodePtr siblingDom = m_domNode->InsertSiblingAfter(nodeName);
	ASSERT(siblingDom != NULL);
	if (siblingDom == NULL)
	{
		return weak_sibling;
	} 
	Shared_XMLModel shared_model = m_ownerModel.lock();
	Shared_XMLElem shared_sibling = shared_model->CreateXMLElem(siblingDom);
	/* 模型插入 */
	BOOL result = InsertSiblingAfter(shared_sibling);
	ASSERT(result);
	if (result == FALSE)
		return weak_sibling;
	weak_sibling = shared_sibling;
	return weak_sibling;
}
Weak_XMLElem XMLElemNode::InsertElementAfter(CString nodeName,CString namespaceURI)
{
	Weak_XMLElem weak_sibling;
	if (m_domNode == NULL)
		return weak_sibling;
	/* dom树插入,指定名称空间 */
	CXMLDOMNodePtr siblingDom = m_domNode->InsertSiblingAfter(nodeName,namespaceURI);
	ASSERT(siblingDom != NULL);
	if (siblingDom == NULL)
	{
		return weak_sibling;
	} 
	Shared_XMLModel shared_model = m_ownerModel.lock();
	Shared_XMLElem shared_sibling = shared_model->CreateXMLElem(siblingDom);
	/* 模型插入 */
	BOOL result = InsertSiblingAfter(shared_sibling);
	ASSERT(result);
	if (result == FALSE)
		return weak_sibling;
	weak_sibling = shared_sibling;
	return weak_sibling;
}
BOOL XMLElemNode::DeleteCurrentElement()
{
	if (m_domNode == NULL)
	{
		return FALSE;
	}
	int childCount = this->m_childList.size();
	int childElemCount = m_domNode->GetChildCount();
	ASSERT(childElemCount == childCount);
	if (childCount != 0)
	{
		Weak_XMLElemListItor itor = this->m_childList.begin();
		
		while (itor != this->m_childList.end())
		{
			Weak_XMLElem weak_child = *itor;
			{
				Shared_XMLElem shared_child = weak_child.lock();
				ASSERT(shared_child != 0);
				VERIFY(shared_child->DeleteCurrentElement());
				/* 子节点被删除掉后应重置子节点迭代器的位置，而不是递增 */
				itor = this->m_childList.begin();
			}
			/* 验证子节点内存已经释放 */
			ASSERT(weak_child.lock() == 0);
		}		
	}
	childCount = this->m_childList.size();
	childElemCount = m_domNode->GetChildCount();
	ASSERT(childCount == 0);
	ASSERT(childElemCount == 0);
	if (childCount == 0)
	{
		Shared_XMLElem shared_this = shared_from_this();
		VERIFY(m_domNode->DeleteCurrentNode());
		VERIFY(this->DeleteCurrent());
	}
	return TRUE;
}

/******************************************************************************/
BOOL XMLElemNode::ValidateElement(XMLStringList& errorInfoList)
{
	ASSERT(m_domNode != 0);
	BOOL result = m_domNode->ValidateCurrentNode(errorInfoList);
	return result;
}

CXMLSOMElementPtr XMLElemNode::GetElementConstraint()
{
	if (m_domNode == NULL)
	{
		return NULL;
	}
	CXMLSOMElementPtr pSOMElement = m_domNode->GetElementConstraint();
	return pSOMElement;
}

XMLStringList XMLElemNode::GetNonExsitPrevElementList()
{
	XMLStringList elemList;
	if (m_domNode == NULL)
	{
		return elemList;
	}
	CXMLSOMElementPtr pSOMElement = m_domNode->GetElementConstraint();
	if (pSOMElement != NULL)
	{
		elemList = pSOMElement->GetNonExsitPrevElementNameList();
	}
	return elemList;
}
XMLStringList XMLElemNode::GetNonExsitNextElementList()
{
	XMLStringList elemList;
	if (m_domNode == NULL)
	{
		return elemList;
	}
	CXMLSOMElementPtr pSOMElement = m_domNode->GetElementConstraint();
	if (pSOMElement != NULL)
	{
		elemList = pSOMElement->GetNonExsitNextElementNameList();
	}
	return elemList;
}

XMLStringList XMLElemNode::GetAllowedChildElementList()
{
	XMLStringList childList;
	if (m_domNode == NULL || GetChildCount() != 0)
	{
		return childList;
	}
	CXMLSOMElementPtr pSOMElement = m_domNode->GetElementConstraint();
	if (pSOMElement != NULL)
	{
		childList = pSOMElement->GetChildElementNamList();
	}
	return childList;
}
/******************************attribute operation*****************************/
BOOL XMLElemNode::AddAttributeNode(CString attrName)
{
	if (m_domNode == NULL || attrName.IsEmpty())
	{
		return FALSE;
	}
	if (m_domNode->IsAttributeExist(attrName))
	{
		return FALSE;
	}
	return m_domNode->AddAttributeNode(attrName);
}
BOOL XMLElemNode::AddAttributeNode(CString attrName,CString namespaceURI)
{
	if (m_domNode == NULL || attrName.IsEmpty())
	{
		return FALSE;
	}
	if (m_domNode->IsAttributeExist(attrName))
	{
		return FALSE;
	}
	return m_domNode->AddAttributeNode(attrName,namespaceURI);
}
BOOL XMLElemNode::AddAttributeNode(CString attrName,CString namespaceURI,CString attrValue)
{
	if (m_domNode == NULL || attrName.IsEmpty())
	{
		return FALSE;
	}
	if (m_domNode->IsAttributeExist(attrName))
	{
		return FALSE;
	}
	return m_domNode->AddAttributeNode(attrName,namespaceURI,attrValue);
}
BOOL XMLElemNode::RemoveAttributeNode(CString attrName)
{
	if (m_domNode == NULL || attrName.IsEmpty())
	{
		return FALSE;
	}
	if (!m_domNode->IsAttributeExist(attrName))
	{
		return FALSE;
	}
	return m_domNode->RemoveAttributeNode(attrName);
}

int XMLElemNode::GetAttributeCount()
{
	int attrCount = 0;
	if (m_domNode != NULL)
	{
		attrCount = m_domNode->GetAttributeCount();
	}
	return attrCount;
}
CString XMLElemNode::GetAttributeName(int index)
{
	CString attrName;
	if (m_domNode != NULL)
	{
		attrName = m_domNode->GetAttributeName(index);
	}
	return attrName;
}

CString XMLElemNode::GetAttributeValue(int index)
{
	CString value;
	if (m_domNode != NULL)
	{
		value = m_domNode->GetAttributeValue(index);
	}
	return value;
}
CString XMLElemNode::GetAttributeValue(CString attrName)
{
	CString value;
	if (m_domNode != NULL)
	{
		value = m_domNode->GetAttributeValue(attrName);
	}
	return value;
}

BOOL XMLElemNode::SetAttributeValue(int index,CString attrValue)
{
	BOOL result = FALSE;
	if (m_domNode != NULL)
	{
		result = m_domNode->SetAttributeValue(index,attrValue);
	}
	return result;
}
BOOL XMLElemNode::SetAttributeValue(CString attrName,CString attrValue)
{
	BOOL result = FALSE;
	if (m_domNode != NULL)
	{
		result = m_domNode->SetAttributeValue(attrName,attrValue);
	}
	return result;
}

BOOL XMLElemNode::ValidateAttribute(int index,XMLStringList& errorInfoList)
{
	ASSERT(m_domNode != NULL);
	BOOL result = m_domNode->ValidateAttribute(index,errorInfoList);
	return result;
}
BOOL XMLElemNode::ValidateAttribute(CString attrName,XMLStringList& errorInfoList)
{
	ASSERT(m_domNode != NULL);
	BOOL result = m_domNode->ValidateAttribute(attrName,errorInfoList);
	return result;
}

CXMLSOMAttributePtr XMLElemNode::GetAttributeConstraint(int index)
{
	CString attrName = GetAttributeName(index);
	return GetAttributeConstraint(attrName);
}
CXMLSOMAttributePtr XMLElemNode::GetAttributeConstraint(CString attrName)
{
	if (m_domNode == NULL || attrName.IsEmpty())
	{
		return NULL;
	}
	CXMLSOMAttributePtr pSOMAttribute = m_domNode->GetAttributeConstraint(attrName);

	return pSOMAttribute;
}

XMLStringList XMLElemNode::GetAttributeValueEnumList(int index)
{
	CString attrName = GetAttributeName(index);
	return GetAttributeValueEnumList(attrName);
}
XMLStringList XMLElemNode::GetAttributeValueEnumList(CString attrName)
{
	XMLStringList enumList;
	if (m_domNode != NULL && !attrName.IsEmpty())
	{
		CXMLSOMAttributePtr somAttribute = m_domNode->GetAttributeConstraint(attrName);
		if (somAttribute != NULL)
		{
			enumList = somAttribute->GetAttributeValueEnumList();
		}
	}
	return enumList;
}

XMLStringList XMLElemNode::GetAttributeValueRefList(int index)
{
	CString attrName = GetAttributeName(index);
	return GetAttributeValueRefList(attrName);
}
XMLStringList XMLElemNode::GetAttributeValueRefList(CString attrName)
{
	XMLStringList refList;
	if (m_domNode != NULL && !attrName.IsEmpty())
	{
		CXMLSOMAttributePtr somAttribute = m_domNode->GetAttributeConstraint(attrName);
		if (somAttribute != NULL)
		{
			refList = somAttribute->GetAttributeValueRefList();
		}
	}
	return refList;
}

XMLStringList XMLElemNode::GetExistentAttributeList()
{
	XMLStringList attrList;
	if (m_domNode == NULL)
		return attrList;
	CXMLSOMElementPtr schemaElement = m_domNode->GetElementConstraint();
	if (schemaElement != NULL)
	{
		attrList = schemaElement->GetExsitAttributeNameList();
	}
	return attrList;
}
XMLStringList XMLElemNode::GetNonExistentAttributeList()
{
	XMLStringList attrList;
	if (m_domNode == NULL)
		return attrList;
	CXMLSOMElementPtr schemaElement = m_domNode->GetElementConstraint();
	if (schemaElement != NULL)
	{
		attrList = schemaElement->GetNonExsitAttributeNameList();
	}
	return attrList;
}
/*************************protected function*****************************/


/*************************private function*******************************/

