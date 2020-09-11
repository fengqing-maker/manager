#include "stdafx.h"
#include "EP61850Client.h"
#include "AbstractTreeViewManager.h"

//#define DEBUG_TRACE
#include "trace_d.h"
#include "XMLSOMElement.h"
#include "XMLElementInsertDialog.h"
#include "XMLAttributeInsertDialog.h"
#include "UtilityTool.h"

AbstractTreeViewManager::AbstractTreeViewManager(void)
{
	m_treeCtrl = 0;
	m_bImageValidate = FALSE;
	m_initExpandLayer = -1;
}
AbstractTreeViewManager::~AbstractTreeViewManager(void)
{
}

BOOL AbstractTreeViewManager::RegeisterAsTreeView(CTreeCtrl* treeCtrl)
{
	if (treeCtrl != 0 && treeCtrl->IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
	{
		m_treeCtrl = treeCtrl;
		TRACE_D0("ע������ͼ�ɹ�.\n");
		return TRUE;
	}
	else
	{
		TRACE_D0("ע������ͼʧ��.\n");
		return FALSE;
	}
}
CTreeCtrl* AbstractTreeViewManager::GetRegisteredTreeView()
{
	return m_treeCtrl;
}

BOOL AbstractTreeViewManager::CreateDefTreeView()
{
	if(m_treeCtrl == 0)
		return FALSE;
	DestroyTreeView();
	CString defTreeViewText = GetDefTreeViewText();
	m_hRoot = m_treeCtrl->InsertItem(defTreeViewText,TVI_ROOT,TVI_LAST);
	return TRUE;
}

BOOL AbstractTreeViewManager::CreateTreeView(Weak_XMLElem rootNode)
{
	Shared_XMLElem shared_root = rootNode.lock();
	if (m_treeCtrl == 0 || shared_root == 0)
		return FALSE;		/* ���ؼ�δע�� */
	/* ��ʼ������ */
	DestroyTreeView();
	m_pRoot = rootNode;
	CImageList* imageList = GetTreeViewImageList();		/* ����ͼ�� */
	SetTreeViewImageList(imageList);
	PreCreateTreeView();
	/* ������������ */
	BOOL ret = DoCreateTreeView(NULL,m_pRoot);	/* ���ڵ�ĸ��ڵ���ΪNULL */
	if (ret)
	{
		m_hRoot = GetHNodeFromPNode(rootNode);	/* ��������ĸ��ڵ��� */
		CString text = m_treeCtrl->GetItemText(m_hRoot);
		/* ʵ������ͼ��ʼչ�� */
		int layerNum = GetTreeViewInitExpandLayer();
		SetTreeViewInitExpandLayer(layerNum);
		/* ����ͼ�����ɹ���ѡ����ڵ� */
		VERIFY(m_treeCtrl->SelectSetFirstVisible(m_hRoot));
	}
	AftCreateTreeView();

	return ret;
}
BOOL AbstractTreeViewManager::CreateTreeViewContextMenu(CPoint clientPoint)
{
	if (m_treeCtrl == 0)
	{
		return FALSE;
	}
	HTREEITEM hItem = NULL;
	hItem = m_treeCtrl->HitTest(clientPoint);
	CString itemText = m_treeCtrl->GetItemText(hItem);
	if (hItem == NULL)
		return FALSE;

	CMenu popupMenu;
	if(!popupMenu.CreatePopupMenu())
		return FALSE;
	TRACE("���������˵�.\n");
	Weak_XMLElem weak_elem = GetPNodeFromHNode(hItem);
	if (!InitNodeContextMenu(weak_elem,&popupMenu))
	{
		return FALSE;
	}
	CPoint screenPoint = clientPoint;
	m_treeCtrl->ClientToScreen(&screenPoint);
	HMENU hMenu = popupMenu.Detach();
	UINT menuID = 0;

	CWnd* pParent = m_treeCtrl->GetParent();
	VERIFY(pParent != 0 && pParent->IsKindOf(RUNTIME_CLASS(CWnd)));
	theApp.GetContextMenuManager()->ShowPopupMenu(hMenu,screenPoint.x,screenPoint.y,
		pParent,TRUE,TRUE,FALSE);
	
	return TRUE;
}
BOOL AbstractTreeViewManager::ResponseTreeViewContextMenu(UINT menuID)
{
	if (menuID == 0 || !(menuID >= WM_CATALOGUE_POPUP_MENUID_MIN && menuID <= WM_CATALOGUE_POPUP_MENUID_MAX))
	{
		/* �˵���ID���� */
		return FALSE;
	}
	if (!ResponseNodeContextMenu(menuID))
	{
		/* ��Ӧ�˵���ʧ�� */
		return FALSE;
	}
	return TRUE;
}

void AbstractTreeViewManager::DestroyTreeView()
{
	if (m_treeCtrl != 0)
	{
		m_treeCtrl->DeleteAllItems();
	}
	m_pRoot.reset();
	m_hRoot = NULL;
	m_treeItemMap.clear();
	m_bImageValidate = FALSE;
	m_initExpandLayer = 0;
}

/****************************select operation**********************************/
HTREEITEM AbstractTreeViewManager::GetSelectedHItem()
{
	if (m_treeCtrl == 0)
	{
		return NULL;
	}
	else
	{
		HTREEITEM hSelected = m_treeCtrl->GetSelectedItem();
		CString itemName = m_treeCtrl->GetItemText(hSelected);
		TRACE_D1("��ǰѡ��ڵ�( %s ).\n",
			itemName);
		return hSelected;
	}
}
Weak_XMLElem AbstractTreeViewManager::GetSelectedPItem()
{
	Weak_XMLElem weak_elem;
	HTREEITEM hSelected = GetSelectedHItem();
	if (hSelected != NULL)
	{
		weak_elem = GetPNodeFromHNode(hSelected);
	}
	return weak_elem;
}
Weak_XMLElem AbstractTreeViewManager::GetPNodeFromHNode(HTREEITEM hItem)
{
	Shared_XMLElem shared_node;
	if (hItem != 0 && m_treeCtrl != 0)
	{
		XMLTreeItemMapItor itor = m_treeItemMap.find(hItem);
		ASSERT(itor != m_treeItemMap.end());
		Weak_XMLElem weak_find = itor->second;
		shared_node = weak_find.lock();
		ASSERT(shared_node != 0);
	}
	return shared_node;
}
HTREEITEM AbstractTreeViewManager::GetHNodeFromPNode(Weak_XMLElem pNode)
{
	Shared_XMLElem shared_node = pNode.lock();
	if (m_treeCtrl != 0 && shared_node != 0)
	{
		XMLTreeItemMapItor itor = m_treeItemMap.begin();
		while (itor != m_treeItemMap.end())
		{
			Shared_XMLElem shared_mapNode = itor->second.lock();
			if (shared_mapNode == shared_node)
			{
				return itor->first;
			}
			itor++;
		}
	}
	return NULL;
}

/*****************************node operation**********************************/
BOOL AbstractTreeViewManager::TreeViewInsertBefore(Weak_XMLElem weak_refElem,CString nodeName)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == NULL)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	Weak_XMLElem weak_newElem = shared_refElem->InsertElementBefore(nodeName);
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if(shared_newElem == 0)
	{
		return FALSE;
	}
	/* �����½ڵ� */
	UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
	CString nodeText = TreeNodeText(shared_newElem);
	int imageIndex = TreeNodeImageIndex(shared_newElem);
	int state = 0;
	LPARAM lParam = 0;
	HTREEITEM hParentItem = m_treeCtrl->GetParentItem(hRefItem);
	HTREEITEM hPrevItem = m_treeCtrl->GetPrevSiblingItem(hRefItem);
	HTREEITEM newItem = NULL;
	if (hPrevItem == NULL)
	{
		/* ���սڵ�֮ǰû�нڵ� */
		newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
			state,state,lParam,hParentItem,TVI_FIRST);
	} 
	else
	{
		newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
			state,state,lParam,hParentItem,hPrevItem);
	}
	ASSERT(newItem != NULL);
	if (newItem == NULL)
	{
		return FALSE;
	}
	AddToTreeItemMap(newItem,shared_newElem);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewInsertBefore(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == NULL)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	Weak_XMLElem weak_newElem = shared_refElem->InsertElementBefore(nodeName,namespaceURI);
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if(shared_newElem == 0)
	{
		return FALSE;
	}
	/* �����½ڵ� */
	UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
	CString nodeText = TreeNodeText(shared_newElem);
	int imageIndex = TreeNodeImageIndex(shared_newElem);
	int state = 0;
	LPARAM lParam = 0;
	HTREEITEM hParentItem = m_treeCtrl->GetParentItem(hRefItem);
	HTREEITEM hPrevItem = m_treeCtrl->GetPrevSiblingItem(hRefItem);
	HTREEITEM newItem = NULL;
	if (hPrevItem == NULL)
	{
		/* ���սڵ�֮ǰû�нڵ� */
		newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
			state,state,lParam,hParentItem,TVI_FIRST);
	} 
	else
	{
		newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
			state,state,lParam,hParentItem,hPrevItem);
	}
	ASSERT(newItem != NULL);
	if (newItem == NULL)
	{
		return FALSE;
	}
	AddToTreeItemMap(newItem,shared_newElem);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewInsertAfter(Weak_XMLElem weak_refElem,CString nodeName)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == NULL)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	Weak_XMLElem weak_newElem = shared_refElem->InsertElementAfter(nodeName);
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if(shared_newElem == 0)
	{
		return FALSE;
	}
	/* �����½ڵ� */
	UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
	CString nodeText = TreeNodeText(shared_newElem);
	int imageIndex = TreeNodeImageIndex(shared_newElem);
	int state = 0;
	LPARAM lParam = 0;
	HTREEITEM hParentItem = m_treeCtrl->GetParentItem(hRefItem);
	HTREEITEM newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
		state,state,lParam,hParentItem,hRefItem);
	ASSERT(newItem != NULL);
	if (newItem == NULL)
	{
		return FALSE;
	}
	AddToTreeItemMap(newItem,shared_newElem);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewInsertAfter(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == NULL)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	Weak_XMLElem weak_newElem = shared_refElem->InsertElementAfter(nodeName,namespaceURI);
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if(shared_newElem == 0)
	{
		return FALSE;
	}
	/* �����½ڵ� */
	UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
	CString nodeText = TreeNodeText(shared_newElem);
	int imageIndex = TreeNodeImageIndex(shared_newElem);
	int state = 0;
	LPARAM lParam = 0;
	HTREEITEM hParentItem = m_treeCtrl->GetParentItem(hRefItem);
	HTREEITEM newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
		state,state,lParam,hParentItem,hRefItem);
	ASSERT(newItem != NULL);
	if (newItem == NULL)
	{
		return FALSE;
	}
	AddToTreeItemMap(newItem,shared_newElem);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewDelete(Weak_XMLElem weak_refElem)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == 0)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(shared_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	if (!shared_refElem->DeleteCurrentElement())
	{
		return FALSE;
	}
	shared_refElem.reset();	/* ���״��ڴ���ɾ�� */
	/* ɾ�����ڵ� */
	VERIFY(RemoveFromTreeItemMap(hRefItem));
	VERIFY(m_treeCtrl->DeleteItem(hRefItem));
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}

BOOL AbstractTreeViewManager::TreeViewAppendChild(Weak_XMLElem weak_refElem,CString nodeName)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == 0)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	Weak_XMLElem weak_newElem = shared_refElem->AppendChildElement(nodeName);
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if (shared_newElem == 0)
	{
		return FALSE;
	}
	/* �����½ڵ� */
	UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
	CString nodeText = TreeNodeText(shared_newElem);
	int imageIndex = TreeNodeImageIndex(shared_newElem);
	int state = 0;
	LPARAM lParam = 0;
	HTREEITEM newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
		state,state,lParam,hRefItem,TVI_LAST);
	ASSERT(newItem != NULL);
	if (newItem == NULL)
	{
		return FALSE;
	}
	m_treeCtrl->Expand(hRefItem,TVE_EXPAND);
	AddToTreeItemMap(newItem,shared_newElem);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewAppendChild(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == 0)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	Weak_XMLElem weak_newElem = shared_refElem->AppendChildElement(nodeName,namespaceURI);
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if (shared_newElem == 0)
	{
		return FALSE;
	}
	/* �����½ڵ� */
	UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
	CString nodeText = TreeNodeText(shared_newElem);
	int imageIndex = TreeNodeImageIndex(shared_newElem);
	int state = 0;
	LPARAM lParam = 0;
	HTREEITEM newItem = m_treeCtrl->InsertItem(mask,nodeText,imageIndex,imageIndex,
		state,state,lParam,hRefItem,TVI_LAST);
	ASSERT(newItem != NULL);
	if (newItem == NULL)
	{
		return FALSE;
	}
	m_treeCtrl->Expand(hRefItem,TVE_EXPAND);
	AddToTreeItemMap(newItem,shared_newElem);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewAppendChild(Weak_XMLElem weak_refElem,Weak_XMLElem weak_newElem)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	Shared_XMLElem shared_newElem = weak_newElem.lock();
	if (shared_refElem == 0 || shared_newElem == 0)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	if(!shared_refElem->AppendChildElement(shared_newElem))
	{
		return FALSE;
	}
	/* �����½ڵ� */
	if (!DoCreateTreeView(hRefItem,shared_newElem))	/* hRefItem����Ҫ����ĸ��ڵ��� */
	{
		return FALSE;
	}
	m_treeCtrl->Expand(hRefItem,TVE_EXPAND);	/* չ���ӽڵ��б� */
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}
BOOL AbstractTreeViewManager::TreeViewDeleteChildren(Weak_XMLElem weak_refElem)
{
	Shared_XMLElem shared_refElem = weak_refElem.lock();
	if (shared_refElem == 0)
	{
		return FALSE;
	}
	HTREEITEM hRefItem = GetHNodeFromPNode(weak_refElem);
	if (hRefItem == NULL)
	{
		return FALSE;
	}
	BOOL result = shared_refElem->DeleteChildrenElements();
	if (result == FALSE)
	{
		return FALSE;
	}
	shared_refElem.reset();	/* ���״��ڴ���ɾ�� */
	/* ɾ�����ڵ� */
	HTREEITEM hChildItem = m_treeCtrl->GetNextItem(hRefItem,TVGN_CHILD);
	while (hChildItem != NULL)
	{
		HTREEITEM hTempItem = hChildItem;
		hChildItem = m_treeCtrl->GetNextItem(hChildItem,TVGN_NEXT);
		/* ���ҵ���һ���ӽڵ���ɾ����ǰ���ӽڵ� */
		VERIFY(RemoveFromTreeItemMap(hTempItem));
		VERIFY(m_treeCtrl->DeleteItem(hTempItem));
	}
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	return TRUE;
}

/****************************protected function*****************************/
CString AbstractTreeViewManager::GetDefTreeViewText()
{
	return TRANS_T("XML�ļ�Ŀ¼��ͼ.");
}

void AbstractTreeViewManager::PreCreateTreeView()
{

}
void AbstractTreeViewManager::AftCreateTreeView()
{

}

CImageList* AbstractTreeViewManager::GetTreeViewImageList()
{
	return 0;	/* Ĭ����ͼ����ʾ */
}
int AbstractTreeViewManager::GetTreeViewInitExpandLayer()
{
	return 1;
}

CString AbstractTreeViewManager::TreeNodeText(Weak_XMLElem weak_node)
{
	CString nodeName;
	Shared_XMLElem shared_node = weak_node.lock();
	if (shared_node != 0)
	{
		nodeName = shared_node->GetNodeName();
	}
	return nodeName;
}
int AbstractTreeViewManager::TreeNodeImageIndex(Weak_XMLElem weak_node)
{
	if (m_bImageValidate == TRUE)
	{
		return 0;
	} 
	else
	{
		return -1;
	}
}

BOOL AbstractTreeViewManager::InitNodeContextMenu(Weak_XMLElem weak_node,CMenu* popupMenu)
{
	m_treePopMenuMap.clear();
	Shared_XMLElem shared_node = weak_node.lock();
	if (shared_node == 0)
		return FALSE; 
	CXMLSOMElementPtr pSOMElement = shared_node->GetElementConstraint();
	/* �Ӳ˵���Ԫ��ǰ���롱 */
	BOOL prevMenuExist = FALSE;
	XMLStringList nonExsitPrevList;
	if (pSOMElement != NULL)
	{
		nonExsitPrevList = pSOMElement->GetNonExsitPrevElementNameList();
	}
	else
	{
		/* ��δ����SchemaԼ����Ԫ��֮ǰ���Բ�������Ԫ�� */
		nonExsitPrevList.push_back(_T("AnyElement"));
	}
	if (nonExsitPrevList.size() != 0)
	{
		CMenu siblingBFInsertMenu;
		if(!siblingBFInsertMenu.CreatePopupMenu())
			return FALSE;
		XMLStringListItor prevItor = nonExsitPrevList.begin();
		while (prevItor != nonExsitPrevList.end())
		{
			CString elemName = *prevItor;
			if (!InsertPopupMenuSiblingBFInsert(weak_node,elemName,&siblingBFInsertMenu))
			{
				return FALSE;
			}
			prevItor++;
		}
		HMENU hBFMenu = siblingBFInsertMenu.Detach();
		VERIFY(popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hBFMenu,_T("Ԫ��ǰ����")));
		prevMenuExist = TRUE;
	}
	/* �Ӳ˵���Ԫ�غ���롱 */
	BOOL nextMenuExist = FALSE;
	XMLStringList nonExsitNextList;
	if (pSOMElement != NULL)
	{
		nonExsitNextList = pSOMElement->GetNonExsitNextElementNameList();
	} 
	else
	{
		/* ��δ����SchemaԼ����Ԫ��֮����Բ�������Ԫ�� */
		nonExsitNextList.push_back(_T("AnyElement"));
	}
	if (nonExsitNextList.size() != 0)
	{
		CMenu siblingAFInsertMenu;
		if (!siblingAFInsertMenu.CreatePopupMenu())
			return FALSE;
		XMLStringListItor nextItor = nonExsitNextList.begin();
		while (nextItor != nonExsitNextList.end())
		{
			CString elemName = *nextItor;
			if (!InsertPopupMenuSiblingAFInsert(weak_node,elemName,&siblingAFInsertMenu))
			{
				return FALSE;
			}
			nextItor++;
		}
		HMENU hAFMenu = siblingAFInsertMenu.Detach();
		VERIFY(popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hAFMenu,_T("Ԫ�غ����")));
		nextMenuExist = TRUE;
	}
	/* �Ӳ˵���������Ԫ�ء� */
	BOOL childInsertMenuExist = FALSE;
	if (shared_node->GetChildCount() == 0)
	{
		/* �ڵ�ǰ�ڵ�û���ӽڵ��ǰ���²���������ӽڵ㣬���ӽڵ�ʱӦ�����ӽڵ���ֵܽڵ� */
		XMLStringList childList;
		if (pSOMElement != NULL)
		{
			childList = pSOMElement->GetChildElementNamList();
		}
		else
		{
			/* ��δ����SchemaԼ����Ԫ�ؿ��Բ���������Ԫ�� */
			childList.push_back(_T("AnyElement"));
		}
		if (childList.size() != 0)
		{
			CMenu childInsertMenu;
			if (!childInsertMenu.CreatePopupMenu())
				return FALSE;

			XMLStringListItor childItor = childList.begin();
			while (childItor != childList.end())
			{
				CString childName = *childItor;
				if (!InsertPopupMenuChildInsert(weak_node,childName,&childInsertMenu))
				{
					return FALSE;
				}
				childItor++;
			}
			HMENU hChildMenu = childInsertMenu.Detach();
			VERIFY(popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hChildMenu,_T("������Ԫ��")));
			childInsertMenuExist = TRUE;
		}
	}
	/* �˵��ɾ��������Ԫ�ء� */
	BOOL childDeleteMenuExist = FALSE;
	CString currentNodeName = shared_node->GetNodeName();
	if (shared_node->GetChildCount() != 0)
	{
		if (!InsertPopupMenuChildDelete(weak_node,currentNodeName,popupMenu))
		{
			return FALSE;
		}
		childDeleteMenuExist = TRUE;
	}
	/* �˵���������ԡ� */
	BOOL attrInsertMenuExist = FALSE;
	XMLStringList nonExsitAttrList;
	if (pSOMElement != NULL)
	{
		nonExsitAttrList = pSOMElement->GetNonExsitAttributeNameList();
	} 
	else
	{
		nonExsitAttrList.push_back(_T("AnyAttribute"));
	}
	int nonExsitAttrCount = nonExsitAttrList.size();
	if (nonExsitAttrCount != 0)
	{
		CMenu attrInsertMenu;
		if(!attrInsertMenu.CreatePopupMenu())
			return FALSE;
		XMLStringListItor nonExsitAttrItor = nonExsitAttrList.begin();
		while (nonExsitAttrItor != nonExsitAttrList.end())
		{
			CString attrName = *nonExsitAttrItor;
			if (!InsertPopupMenuAttributeInsert(weak_node,currentNodeName,attrName,&attrInsertMenu))
			{
				return FALSE;
			}
			nonExsitAttrItor++;
		}
		HMENU hAttrInsertMenu = attrInsertMenu.Detach();
		VERIFY(popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hAttrInsertMenu,_T("��������")));
		attrInsertMenuExist = TRUE;
	}
	/* �˵��ɾ�����ԡ� */
	BOOL attrDeleteMenuExist = FALSE;
	XMLStringList exsitAttrList;
	int exsitAttrCount = shared_node->GetAttributeCount();
	for (int i = 0; i < exsitAttrCount; i++)
	{
		CString attrName = shared_node->GetAttributeName(i);
		exsitAttrList.push_back(attrName);
	}
	if (exsitAttrCount != 0)
	{
		CMenu attrDeleteMenu;
		if(!attrDeleteMenu.CreatePopupMenu())
			return FALSE;
		XMLStringListItor exsitAttrItor = exsitAttrList.begin();
		while (exsitAttrItor != exsitAttrList.end())
		{
			CString attrName = *exsitAttrItor;
			if (!InsertPopupMenuAttributeDelete(weak_node,currentNodeName,attrName,&attrDeleteMenu))
			{
				return FALSE;
			}
			exsitAttrItor++;
		}
		HMENU hAttrDeleteMenu = attrDeleteMenu.Detach();
		VERIFY(popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hAttrDeleteMenu,_T("ɾ������")));
		attrDeleteMenuExist = TRUE;
	}
	/* �˵��У�鵱ǰԪ�ء� */
	BOOL currentValidateMenuExist = FALSE;
	if (!InsertPopupMenuValidate(weak_node,currentNodeName,popupMenu))
	{
		return FALSE;
	}
	currentValidateMenuExist = TRUE;
	/* �˵��ɾ����ǰԪ�ء� */
	BOOL currentDeleteMenuExist = FALSE;
	Shared_XMLModel shared_model = shared_node->GetOwnerModel().lock();
	if (shared_model != 0)
	{
		Shared_XMLElem shared_root = shared_model->GetModelRootElem().lock();
		if (shared_root != shared_node)
		{
			/* ֻ�е�ǰԪ�ؽڵ㲻�Ǹ��ڵ�ʱ���Ա�ɾ�� */
			if (!InsertPopupMenuDelete(weak_node,currentNodeName,popupMenu))
			{
				return FALSE;
			}
			currentDeleteMenuExist = TRUE;
		}
	}
	
	/* Ϊ�˵���ӷָ��� */
	int pos = 0;
	if ((prevMenuExist || nextMenuExist) 
		&& (childInsertMenuExist || childDeleteMenuExist
			||attrInsertMenuExist || attrDeleteMenuExist 
			||currentValidateMenuExist || currentDeleteMenuExist))
	{
		if(prevMenuExist)
			pos++;
		if(nextMenuExist)
			pos++;
		VERIFY(popupMenu->InsertMenu(pos,MF_BYPOSITION|MF_SEPARATOR));
		pos++;
	}
	if ((childInsertMenuExist || childDeleteMenuExist)
		&& (attrInsertMenuExist || attrDeleteMenuExist
			||currentValidateMenuExist || currentDeleteMenuExist))
	{
		if(childInsertMenuExist)
			pos++;
		if(childDeleteMenuExist)
			pos++;
		VERIFY(popupMenu->InsertMenu(pos,MF_BYPOSITION|MF_SEPARATOR));
		pos++;
	}
	if ((attrInsertMenuExist || attrDeleteMenuExist)
		&&(currentValidateMenuExist || currentDeleteMenuExist))
	{
		if(attrInsertMenuExist)
			pos++;
		if(attrDeleteMenuExist)
			pos++;
		VERIFY(popupMenu->InsertMenu(pos,MF_BYPOSITION|MF_SEPARATOR));
		pos++;
	}
	return TRUE;
}
BOOL AbstractTreeViewManager::ResponseNodeContextMenu(UINT menuID)
{
	TreePopMenuMapItor popmenuItor = m_treePopMenuMap.find(menuID);
	BOOL result = FALSE;
	if (popmenuItor != m_treePopMenuMap.end())
	{
		TreePopMenuPtr popmenuInfo = popmenuItor->second;
		switch (popmenuInfo->menuType)
		{
		case MenuType_Sibling_InsertBF:
			result = ResponsePopupMenuSiblingBFInsert(popmenuInfo);
			break;
		case MenuType_Sibling_InsertAF:
			result = ResponsePopupMenuSiblingAFInsert(popmenuInfo);
			break;
		case MenuType_Child_Insert:
			result = ResponsePopupMenuChildInsert(popmenuInfo);
			break;
		case MenuType_Child_Delete:
			result = ResponsePopupMenuChildDelete(popmenuInfo);
			break;
		case MenuType_Attribute_Insert:
			result = ResponsePopupMenuAttributeInsert(popmenuInfo);
			break;
		case MenuType_Attribute_Delete:
			result = ResponsePopupMenuAttributeDelete(popmenuInfo);
			break;
		case MenuType_Current_Validate:
			result = ResponsePopupMenuCurrentValidate(popmenuInfo);
			break;
		case MenuType_Current_Delete:
			result = ResponsePopupMenuCurrentDelete(popmenuInfo);
			break;
		default:
			ASSERT(0);	/* should never happen */
			result = FALSE;
			break;
		}
	}
	return result;
}

BOOL AbstractTreeViewManager::UpdateElemGridView()
{
	return TRUE;
}
BOOL AbstractTreeViewManager::UpdateAttrGridView()
{
	return TRUE;
}

void AbstractTreeViewManager::OutputFileMsg(CString msg)
{
	TRACE("������AbstractTreeViewManager::OutputFileMsg()����ļ�������Ϣ���.");
}
void AbstractTreeViewManager::OutputFileErr(CString err)
{
	TRACE("������AbstractTreeViewManager::OutputFileErr()����ļ������������.");
}

void AbstractTreeViewManager::OutputValidateMsg(CString msg)
{
	TRACE("������AbstractTreeViewManager::OutputValidateMsg()����ļ�У����Ϣ���.");
}
void AbstractTreeViewManager::OutputValidateErr(CString err)
{
	TRACE("������AbstractTreeViewManager::OutputValidateErr()����ļ�У��������.");
}
/****************************private function********************************/
void AbstractTreeViewManager::SetTreeViewImageList(CImageList* imageList)
{
	if (imageList == 0 || !imageList->IsKindOf(RUNTIME_CLASS(CImageList)))
	{
		m_bImageValidate = FALSE;
	}
	else
	{
		m_bImageValidate = TRUE;
		m_treeCtrl->SetImageList(imageList,TVSIL_NORMAL);
	}
}
void AbstractTreeViewManager::SetTreeViewInitExpandLayer(int layerNum)
{
	m_initExpandLayer = layerNum;
	DoTreeViewInitExpand(m_hRoot);
}

BOOL AbstractTreeViewManager::DoCreateTreeView(HTREEITEM hItem , Weak_XMLElem weak_node)
{
	ASSERT(m_treeCtrl != 0);
	Shared_XMLElem shared_node = weak_node.lock();
	if(shared_node == 0)
	{
		ASSERT(0);
		return FALSE;
	}
	else
	{	
		/* ���뵱ǰ�ڵ� */
		UINT mask = m_bImageValidate?(TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT):(TVIF_TEXT);
		CString nodeName = TreeNodeText(shared_node);
		int imageIndex = TreeNodeImageIndex(shared_node);
		int state = 0;
		LPARAM lParam = 0;
		HTREEITEM newItem = m_treeCtrl->InsertItem(mask,nodeName,imageIndex,imageIndex,
			state,state,lParam,hItem,TVI_LAST);
		ASSERT(newItem != NULL);
		m_treeItemMap.insert(std::make_pair(newItem,shared_node));
		/* �����ӽڵ���еݹ� */
		int nChildCount = shared_node->GetChildCount();
		for(int i=0; i < nChildCount; ++i)
		{
			Weak_XMLElem weak_child = shared_node->GetChildNode(i);
			Shared_XMLElem shared_child = weak_child.lock();
			if (shared_child != 0)
			{
				DoCreateTreeView(newItem,shared_child);
			}
		}
		return TRUE;
	}
}
int AbstractTreeViewManager::GetHNodeLayer(HTREEITEM hItem)
{
	if(m_treeCtrl == 0)
		return -1;
	int layerNum = 1;
	CString text  = m_treeCtrl->GetItemText(hItem);
	HTREEITEM hParent = m_treeCtrl->GetParentItem(hItem);
	while (hParent != NULL)
	{
		hParent = m_treeCtrl->GetParentItem(hParent);
		layerNum++;
	}
	return layerNum;
}
void AbstractTreeViewManager::DoTreeViewInitExpand(HTREEITEM hItem)
{
	if (m_treeCtrl == 0 ||m_initExpandLayer <= 0)
		return;
	m_treeCtrl->Expand(hItem,TVE_EXPAND);
	int curLayer = GetHNodeLayer(hItem);
	if (curLayer >= m_initExpandLayer)
	{
		return;
	}
	else
	{
		HTREEITEM hChild = m_treeCtrl->GetNextItem(hItem,TVGN_CHILD);
		while (hChild != NULL)
		{
			DoTreeViewInitExpand(hChild);
			hChild = m_treeCtrl->GetNextItem(hChild,TVGN_NEXT);
		}
	}
	return;
}

BOOL AbstractTreeViewManager::AddToTreeItemMap(HTREEITEM hItem,Weak_XMLElem weak_node)
{
#ifdef _DEBUG
	ASSERT(m_treeCtrl != 0);
	CString itemText = m_treeCtrl->GetItemText(hItem);
#endif
	Shared_XMLElem shared_node = weak_node.lock();
	if (hItem == NULL || shared_node == 0)
	{
		ASSERT(0);
		return FALSE;
	}
	m_treeItemMap.insert(std::make_pair(hItem,weak_node));
	return FALSE;
}
BOOL AbstractTreeViewManager::RemoveFromTreeItemMap(HTREEITEM hItem)
{
#ifdef _DEBUG
	ASSERT(m_treeCtrl != 0);
	CString itemText = m_treeCtrl->GetItemText(hItem);
#endif
	if (m_treeCtrl->ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem = m_treeCtrl->GetNextItem(hItem,TVGN_CHILD);
		while (hChildItem != NULL)
		{
			HTREEITEM hTempItem = hChildItem;
			hChildItem = m_treeCtrl->GetNextItem(hChildItem,TVGN_NEXT);
			VERIFY(RemoveFromTreeItemMap(hTempItem));
		}
	}
#ifdef _DEBUG
	/* �����Ƿ��Ѿ���ȫ���ӽڵ��map�Ƴ� */
	if(m_treeCtrl->ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem = m_treeCtrl->GetNextItem(hItem,TVGN_CHILD);
		while (hChildItem != NULL)
		{
			XMLTreeItemMapItor childItor = m_treeItemMap.find(hChildItem);
			ASSERT(childItor == m_treeItemMap.end());
			hChildItem = m_treeCtrl->GetNextItem(hChildItem,TVGN_NEXT);
		}
	}
#endif
	XMLTreeItemMapItor itor = m_treeItemMap.find(hItem);
	ASSERT(itor != m_treeItemMap.end());
	m_treeItemMap.erase(itor);
	return TRUE;
}

BOOL AbstractTreeViewManager::InsertPopupMenuSiblingBFInsert
	(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Sibling_InsertBF;
	menuInfo->menuTitle = elemName;
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;

	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuSiblingAFInsert
	(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Sibling_InsertAF;
	menuInfo->menuTitle = elemName;
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuChildInsert
	(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Child_Insert;
	menuInfo->menuTitle = elemName;
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuChildDelete
	(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Child_Delete;
	menuInfo->menuTitle = _T("ɾ��ȫ����Ԫ��");
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuAttributeInsert
	(Weak_XMLElem weak_elem,CString elemName,CString attrName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Attribute_Insert;
	menuInfo->menuTitle = attrName;
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	menuInfo->attrName = attrName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuAttributeDelete
	(Weak_XMLElem weak_elem,CString elemName,CString attrName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Attribute_Delete;
	menuInfo->menuTitle = attrName;
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	menuInfo->attrName = attrName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuValidate
	(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Current_Validate;
	menuInfo->menuTitle = _T("У�鵱ǰԪ��");
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractTreeViewManager::InsertPopupMenuDelete
	(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	TreePopMenuPtr menuInfo(new TreePopMenuInfo());
	menuInfo->menuID = m_treePopMenuMap.size()+WM_CATALOGUE_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Current_Delete;
	menuInfo->menuTitle = _T("ɾ����ǰԪ��");
	menuInfo->weak_elem = weak_elem;
	menuInfo->elemName = elemName;
	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_treePopMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}

BOOL AbstractTreeViewManager::ResponsePopupMenuSiblingBFInsert(TreePopMenuPtr popmenuInfo)
{
	ASSERT(popmenuInfo->menuType == MenuType_Sibling_InsertBF);
	BOOL result = FALSE;
	Shared_XMLElem shared_refElem = popmenuInfo->weak_elem.lock();
	CString elemName;
	CString elemNamespace;
	if (popmenuInfo->elemName == _T("AnyElement"))
	{
		CWnd* pWnd = theApp.GetMainWnd();
		CString defElemName = _T("AnyElement");							/* Ĭ��Ԫ������ */
		CString defElemNamespace = shared_refElem->GetNodeNamespace();	/* Ĭ��Ԫ�����ƿռ� */
		CXMLElementInsertDialog elemDialog(defElemName,defElemNamespace,pWnd);
		elemDialog.DoModal();
		int choice = elemDialog.GetChoice();
		if (choice == IDCANCEL)
		{
			return TRUE;
		}
		else if (choice == IDOK)
		{
			elemName = elemDialog.GetElementName();
			elemNamespace = elemDialog.GetElementNamespaceURI();
			ASSERT(!elemName.IsEmpty());
			result = TreeViewInsertBefore(popmenuInfo->weak_elem,elemName,elemNamespace);
		}
		else
		{
			ASSERT(0);
			result = FALSE;
		}
	}
	else
	{
		elemName = popmenuInfo->elemName;
		result = TreeViewInsertBefore(popmenuInfo->weak_elem,elemName);
	}
	CString refElemName=TRANS_T("δ֪");
	if (shared_refElem != 0)
	{
		refElemName = shared_refElem->GetNodeName();
	}
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("��Ԫ��\"%s\"ǰ����Ԫ��\"%s\"�ɹ�."),
			refElemName,elemName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("��Ԫ��\"%s\"ǰ����Ԫ��\"%s\"ʧ��."),
			refElemName,elemName);
		OutputFileErr(err);
	}
	return result;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuSiblingAFInsert(TreePopMenuPtr popmenuInfo)
{
	ASSERT(popmenuInfo->menuType == MenuType_Sibling_InsertAF);
	BOOL result = FALSE;
	Shared_XMLElem shared_refElem = popmenuInfo->weak_elem.lock();
	CString elemName;
	CString elemNamespace;
	if (popmenuInfo->elemName == _T("AnyElement"))
	{
		CWnd* pWnd = theApp.GetMainWnd();
		CString defElemName = _T("AnyElement");							/* Ĭ��Ԫ������ */
		CString defElemNamespace = shared_refElem->GetNodeNamespace();	/* Ĭ��Ԫ�����ƿռ� */
		CXMLElementInsertDialog elemDialog(defElemName,defElemNamespace,pWnd);
		elemDialog.DoModal();
		int choice = elemDialog.GetChoice();
		if (choice == IDCANCEL)
		{
			return TRUE;
		}
		else if (choice == IDOK)
		{
			elemName = elemDialog.GetElementName();
			elemNamespace = elemDialog.GetElementNamespaceURI();
			ASSERT(!elemName.IsEmpty());
			result = TreeViewInsertAfter(popmenuInfo->weak_elem,elemName,elemNamespace);
		}
		else
		{
			ASSERT(0);
			result = FALSE;
		}
	}
	else
	{
		elemName = popmenuInfo->elemName;
		result = TreeViewInsertAfter(popmenuInfo->weak_elem,elemName);
	}
	CString refElemName=TRANS_T("δ֪");
	if (shared_refElem != 0)
	{
		refElemName = shared_refElem->GetNodeName();
	}
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("��Ԫ��\"%s\"�����Ԫ��\"%s\"�ɹ�."),
			refElemName,elemName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("��Ԫ��\"%s\"�����Ԫ��\"%s\"ʧ��."),
			refElemName,elemName);
		OutputFileErr(err);
	}
	return result;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuChildInsert(TreePopMenuPtr popmenuInfo)
{
	ASSERT(popmenuInfo->menuType == MenuType_Child_Insert);
	Shared_XMLElem shared_refElem = popmenuInfo->weak_elem.lock();
	BOOL result = FALSE;
	CString elemName;
	CString elemNamespace;
	if (popmenuInfo->elemName == _T("AnyElement"))
	{
		CWnd* pWnd = theApp.GetMainWnd();
		CString defElemName = _T("AnyElement");							/* Ĭ��Ԫ������ */
		CString defElemNamespace = shared_refElem->GetNodeNamespace();	/* Ĭ��Ԫ�����ƿռ� */
		CXMLElementInsertDialog elemDialog(defElemName,defElemNamespace,pWnd);
		elemDialog.DoModal();
		int choice = elemDialog.GetChoice();
		if (choice == IDCANCEL)
		{
			return TRUE;
		}
		else if (choice == IDOK)
		{
			elemName = elemDialog.GetElementName();
			elemNamespace = elemDialog.GetElementNamespaceURI();
			result = TreeViewAppendChild(popmenuInfo->weak_elem,elemName,elemNamespace);
		}
		else
		{
			ASSERT(0);		/* should never happen */
			result = FALSE;	
		}
	}
	else
	{
		elemName = popmenuInfo->elemName;
		result = TreeViewAppendChild(popmenuInfo->weak_elem,elemName);
	}
	CString refElemName=TRANS_T("δ֪");
	if (shared_refElem != 0)
	{
		refElemName = shared_refElem->GetNodeName();
	}
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("��Ԫ��\"%s\"������Ԫ��\"%s\"�ɹ�."),
			refElemName,elemName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("��Ԫ��\"%s\"������Ԫ��\"%s\"ʧ��."),
			refElemName,elemName);
		OutputFileErr(err);
	}
	return result;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuChildDelete(TreePopMenuPtr popmenuInfo)
{
	ASSERT(popmenuInfo->menuType == MenuType_Child_Delete);
	BOOL result = TreeViewDeleteChildren(popmenuInfo->weak_elem);
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("ɾ��Ԫ��\"%s\"ȫ����Ԫ�سɹ�."),
			popmenuInfo->elemName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("ɾ��Ԫ��\"%s\"ȫ����Ԫ��ʧ��."),
			popmenuInfo->elemName);
		OutputFileErr(err);
	}
	return result;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuAttributeInsert(TreePopMenuPtr popmenuInfo)
{
	ASSERT(MenuType_Attribute_Insert == popmenuInfo->menuType);
	Shared_XMLElem shared_node = popmenuInfo->weak_elem.lock();
	if (shared_node == NULL || popmenuInfo->attrName.IsEmpty())
	{
		return FALSE;
	}
	HTREEITEM hItem = GetHNodeFromPNode(shared_node);
	if (hItem == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	CString attrName;
	CString attrNamespace;
	CString attrValue;
	if (popmenuInfo->attrName == _T("AnyAttribute"))
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		CString defAttrName = _T("AnyAttribute");
		CString defAttrNamespace = _T("");
		CString defAttrValue = _T("");
		CXMLAttributeInsertDialog attrDialog(defAttrName,defAttrNamespace,defAttrValue,pMainFrm);
		attrDialog.DoModal();
		int choice = attrDialog.GetChoice();
		if (choice == IDCANCEL)
		{
			return TRUE;
		}
		else if (choice == IDOK)
		{
			attrName = attrDialog.GetAttributeName();
			attrNamespace = attrDialog.GetAttributeNamespaceURI();
			attrValue = attrDialog.GetAttributeValue();
			result = shared_node->AddAttributeNode(attrName,attrNamespace,attrValue);
		}
		else
		{
			ASSERT(0);	/* should never happen */
			result = FALSE;
		}
	}
	else
	{
		attrName = popmenuInfo->attrName;
		result = shared_node->AddAttributeNode(attrName);
	}
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("��Ԫ��\"%s\"��������\"%s\"�ɹ�."),
			popmenuInfo->elemName,attrName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("��Ԫ��\"%s\"��������\"%s\"ʧ��."),
			popmenuInfo->elemName,attrName);
		OutputFileErr(err);
	}
	return TRUE;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuAttributeDelete(TreePopMenuPtr popmenuInfo)
{
	ASSERT(MenuType_Attribute_Delete == popmenuInfo->menuType);
	Shared_XMLElem shared_node = popmenuInfo->weak_elem.lock();
	if (shared_node == NULL || popmenuInfo->attrName.IsEmpty())
	{
		return FALSE;
	}
	HTREEITEM hItem = GetHNodeFromPNode(shared_node);
	if (hItem == NULL)
	{
		return FALSE;
	}
	BOOL result = shared_node->RemoveAttributeNode(popmenuInfo->attrName);
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("ɾ��Ԫ��\"%s\"������\"%s\"�ɹ�."),
			popmenuInfo->elemName,popmenuInfo->attrName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("ɾ��Ԫ��\"%s\"������\"%s\"ʧ��."),
			popmenuInfo->elemName,popmenuInfo->attrName);
		OutputFileErr(err);
	}
	return TRUE;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuCurrentValidate(TreePopMenuPtr popmenuInfo)
{
	ASSERT(MenuType_Current_Validate == popmenuInfo->menuType);
	Shared_XMLElem shared_node = popmenuInfo->weak_elem.lock();
	if (shared_node == NULL)
	{
		return FALSE;
	}
	HTREEITEM hItem = GetHNodeFromPNode(shared_node);
	if (hItem == NULL)
	{
		return FALSE;
	}
	XMLStringList errorInfoList;
	BOOL bValid = shared_node->ValidateElement(errorInfoList);
	if (bValid)
	{
		XMLStringListItor itor = errorInfoList.begin();
		while(itor != errorInfoList.end())
		{
			CString msg = *itor;
			OutputValidateMsg(msg);
			itor++;
		}
	}
	else
	{
		XMLStringListItor itor = errorInfoList.begin();
		while(itor != errorInfoList.end())
		{
			CString err = *itor;
			OutputValidateErr(err);
			itor++;
		}
	}
	return TRUE;
}
BOOL AbstractTreeViewManager::ResponsePopupMenuCurrentDelete(TreePopMenuPtr popmenuInfo)
{
	ASSERT(popmenuInfo->menuType == MenuType_Current_Delete);
	BOOL result = TreeViewDelete(popmenuInfo->weak_elem);
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("ɾ��Ԫ��\"%s\"�ɹ�."),
			popmenuInfo->elemName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("ɾ��Ԫ��\"%s\"ʧ��."),
			popmenuInfo->elemName);
		OutputFileErr(err);
	}
	return result;
}