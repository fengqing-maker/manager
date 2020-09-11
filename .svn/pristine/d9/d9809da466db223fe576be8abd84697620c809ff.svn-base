#include "stdafx.h"
#include "XMLModel.h"

#include "XMLElemNode.h"	/* 解除model与elem的相互引用关系，使用预定义声明 */

XMLModel::XMLModel(void)
{
}
XMLModel::~XMLModel(void)
{
}

CString XMLModel::GetModelName()
{
	CString modelName;
	if (m_xmlDOMFile != NULL)
	{
		modelName = m_xmlDOMFile->GetXMLFilePath();
	}
	return modelName;
}

void XMLModel::SetModelDOMFile(CXMLDOMFilePtr domFile)
{
	m_xmlDOMFile = domFile;
}
CXMLDOMFilePtr XMLModel::GetModelDOMFile()
{
	return m_xmlDOMFile;
}

void XMLModel::SetModelRootElem(Shared_XMLElem rootItem)
{
	m_xmlRootItem = rootItem;
}
Weak_XMLElem XMLModel::GetModelRootElem()
{
	return m_xmlRootItem;
}

Shared_XMLElem XMLModel::CreateXMLElem()
{
	Shared_XMLElem shared_elem(new XMLElemNode());
	Shared_XMLModel shared_this = shared_from_this();
	shared_elem->SetOwnerModel(shared_this);
	m_xmlElemList.push_back(shared_elem);
	return shared_elem;
}
Shared_XMLElem XMLModel::CreateXMLElem(CXMLDOMNodePtr domNode)
{
	if (domNode == NULL)
	{
		return NULL;
	}
	Shared_XMLElem shared_elem(new XMLElemNode());
	Shared_XMLModel shared_this = shared_from_this();
	shared_elem->SetOwnerModel(shared_this);
	shared_elem->SetDOMNode(domNode);
	VERIFY(AddXMLElemToFindMap(shared_elem));
	VERIFY(AddXMLElemToStoreList(shared_elem));
	return shared_elem;
}
Shared_XMLElem XMLModel::CloneXMLElem(Weak_XMLElem weak_src,BOOL bDeepClone)
{
	/* 拷贝当前节点,但未拷贝父子关系 */
	Shared_XMLElem shared_des;
	Shared_XMLElem shared_src = weak_src.lock();
	ASSERT(shared_src != 0);
	CXMLDOMNodePtr srcDOMNode = shared_src->GetDOMNode();
	CXMLDOMNodePtr desDOMNode = NULL;
	if(!srcDOMNode->CloneNode(desDOMNode,FALSE))
		return shared_des;
	shared_des.reset(new XMLElemNode());
	Shared_XMLModel shared_this = shared_from_this();
	shared_des->SetOwnerModel(shared_this);
	shared_des->SetDOMNode(desDOMNode);
	VERIFY(AddXMLElemToFindMap(shared_des));
	VERIFY(AddXMLElemToStoreList(shared_des));

	/* 拷贝子节点，同时拷贝父子关系 */
	if (bDeepClone && shared_src->GetChildCount() != 0)
	{
		int childCount = shared_src->GetChildCount();
		for (int i = 0; i < childCount ; i++)
		{
			Weak_XMLElem weak_srcChild = shared_src->GetChildNode(i);
			Shared_XMLElem shared_desChild = CloneXMLElem(weak_srcChild,bDeepClone);
			CXMLDOMNodePtr desChildDOMNode = shared_desChild->GetDOMNode();
			VERIFY(desDOMNode->AppendChildNode(desChildDOMNode));
			VERIFY(shared_des->AppendChild(shared_desChild));
		}
	}
	return shared_des;
}
BOOL XMLModel::ImportXMLElem(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		ASSERT(0);
		return FALSE;
	}
	Weak_XMLModel weak_oldModel = shared_elem->GetOwnerModel();
	Shared_XMLModel shared_oldModel = weak_oldModel.lock();
	Shared_XMLModel shared_thisModel = shared_from_this();
	if (shared_oldModel == shared_thisModel)
	{
		return TRUE;
	}
	if (shared_oldModel != 0)
	{
		VERIFY(shared_oldModel->DeleteXMLElem(weak_elem));
	}
	shared_elem->SetOwnerModel(shared_thisModel);
	VERIFY(AddXMLElemToFindMap(shared_elem));
	VERIFY(AddXMLElemToStoreList(shared_elem));
	/* 导入子节点 */
	if (shared_elem->GetChildCount() != 0)
	{
		int childCount = shared_elem->GetChildCount();
		for (int i = 0; i < childCount ; i++)
		{
			Weak_XMLElem weak_child = shared_elem->GetChildNode(i);
			if (!ImportXMLElem(weak_child))
			{
				ASSERT(0);
				return FALSE;
			}
		}
	}
	return TRUE;
}
BOOL XMLModel::DeleteXMLElem(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	VERIFY(RemoveXMLElemFromFindMap(shared_elem));
	VERIFY(RemoveXMLElemFromStoreList(shared_elem));
	Shared_XMLModel emptyModel;
	shared_elem->SetOwnerModel(emptyModel);
	return TRUE;
}

Weak_XMLElem XMLModel::FindXMLElem(MSXML2::IXMLDOMNodePtr domPtr)
{
	Weak_XMLElem weak_elem;
	Weak_XMLFindMapItor itor = m_modelFindMap.find(domPtr);
	if (itor != m_modelFindMap.end())
	{
		weak_elem = itor->second;
	}
	return weak_elem;
}
/******************************protected function*************************/


/******************************private function***************************/
BOOL XMLModel::AddXMLElemToStoreList(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	m_xmlElemList.push_back(shared_elem);
	return TRUE;
}
BOOL XMLModel::RemoveXMLElemFromStoreList(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	BOOL result = FALSE;
	Shared_XMLElemListItor itor = m_xmlElemList.begin();
	while (itor != m_xmlElemList.end())
	{
		Shared_XMLElem shared_temp = *itor;
		if (shared_temp == shared_elem)
		{
			m_xmlElemList.erase(itor);
			result = TRUE;
			break;
		}
		itor++;
	}
	ASSERT(result);
	return result;
}

BOOL XMLModel::AddXMLElemToFindMap(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	MSXML2::IXMLDOMNodePtr pXMLDOMNode = shared_elem->GetDOMNode()->GetInnerPtr();
	ASSERT(pXMLDOMNode != NULL);
	m_modelFindMap.insert(std::make_pair(pXMLDOMNode,weak_elem));
	return TRUE;
}
BOOL XMLModel::RemoveXMLElemFromFindMap(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	MSXML2::IXMLDOMNodePtr pXMLDOMNode = shared_elem->GetDOMNode()->GetInnerPtr();
	ASSERT(pXMLDOMNode != NULL);
	Weak_XMLFindMapItor itor = m_modelFindMap.find(pXMLDOMNode);
	if (itor != m_modelFindMap.end())
	{
		m_modelFindMap.erase(itor);
	}
	return TRUE;
}