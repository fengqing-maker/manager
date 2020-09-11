/**
*@file	AbstractTreeViewManager.h
*@brief	�����XML�ļ�����ͼ������
*@note	�����������ͼ����������XML�ļ�ģ�͵�����ͼ��ʾ��
*		ʹ�÷����Ǽ̳иó�����
*@author	�߷�
*/
#pragma once

#include <map>

#include "XMLModel.h"
#include "XMLElemNode.h"

#define WM_CATALOGUE_POPUP_MENUID_MIN			WM_USER+3000	/* ����ͼ�����˵���ID��Сֵ */
#define WM_CATALOGUE_POPUP_MENUID_MAX			WM_USER+3100	/* ����ͼ�����˵���ID���ֵ */


/**
*@brief	XML���ڵ�Map����
*@note	XML���ڵ�Map����ڵ�����ڵ�ָ��֮���ӳ���ϵ������ӽڵ���ӳ�䵽�ڵ�ָ��
*		��ӽڵ�ָ��ӳ�䵽�ڵ�����
*/
typedef std::map<HTREEITEM,Weak_XMLElem>		XMLTreeItemMap;
typedef XMLTreeItemMap::iterator				XMLTreeItemMapItor;
typedef XMLTreeItemMap::const_iterator			XMLTreeItemMapConstItor;

typedef struct treepopmenuinfo
{
	UINT			menuID;			/**< @brief �˵�ID�� */
	int				menuType;		/**< @brief �˵����ͺ� */
	CString			menuTitle;		/**< @brief �˵����� */

	Weak_XMLElem	weak_elem;		/**< @brief ��ǰ�˵�����Ե���ʼ�ڵ�ָ�� */
	CString			elemName;		/**< @brief ��ǰ�˵�����Ե�Ŀ��ڵ����� */
	CString			attrName;		/**< @brief ��ǰ�˵�����Ե�Ŀ��ڵ����� */

	treepopmenuinfo()
	{
		menuID = UINT_MAX;
		menuType = -1;
		menuTitle.Empty();

		weak_elem.reset();
		elemName.Empty();
		attrName.Empty();
	}
}TreePopMenuInfo;
typedef std::tr1::shared_ptr<TreePopMenuInfo>		TreePopMenuPtr;
typedef std::map<UINT,TreePopMenuPtr>				TreePopMenuMap;
typedef TreePopMenuMap::iterator					TreePopMenuMapItor;
typedef TreePopMenuMap::const_iterator				TreePopMenuMapConstItor;

/** @brief �����XML�ļ�����ͼ������ */
class AbstractTreeViewManager
{
public:
	AbstractTreeViewManager(void);
	virtual ~AbstractTreeViewManager(void) = 0;

	BOOL RegeisterAsTreeView(CTreeCtrl* treeCtrl);	/**< @brief ע�����ؼ�ָ�뵽������ */
	CTreeCtrl* GetRegisteredTreeView();				/**< @brief ��ȡ��ע�ᵽ�����������ؼ�ָ�� */

	BOOL CreateDefTreeView();						/**< @brief ����Ĭ������ͼ */

	BOOL CreateTreeView(Weak_XMLElem rootNode);		/**< @brief ���û����ڵ㴴���û�����ͼ */
	void DestroyTreeView();							/**< @brief �����û�����ͼ */

	/**
	*@brief ��������ͼ�Ҽ������˵�
	*note	pointΪ����ͼ��Χ�ڵĵ�,������Ļ��Χ�ڵ�
	*/
	BOOL CreateTreeViewContextMenu(CPoint clientPoint);
	/**
	*@brief ��Ӧ����ͼ�Ҽ������˵��� 
	*@note	�ڴ�������ͼ�����˵�ʱ�����˲˵���map(�˵�ID���˵�����)
	*/
	BOOL ResponseTreeViewContextMenu(UINT menuID);

/******************************select operation********************************/
	HTREEITEM		GetSelectedHItem();							/**< @brief ��õ�ǰѡ�еĽڵ��� */
	Weak_XMLElem	GetSelectedPItem();							/**< @brief ��õ�ǰѡ�еĽڵ�ָ�� */
	Weak_XMLElem	GetPNodeFromHNode(HTREEITEM hItem);			/**< @brief �ɽڵ������ҽڵ�ָ�� */
	HTREEITEM		GetHNodeFromPNode(Weak_XMLElem pNode);		/**< @brief �ɽڵ�ָ����ҽڵ��� */

/******************************node operation**********************************/

	/** @brief ������ͼָ��Ԫ�ؽڵ�ǰ����һ����ָ�����ƿռ��Ԫ�ؽڵ� */
	BOOL TreeViewInsertBefore(Weak_XMLElem weak_refElem,CString nodeName);
	/** @brief ������ͼָ��Ԫ�ؽڵ�ǰ����һ������ָ�����ƿռ��Ԫ�ؽڵ� */
	BOOL TreeViewInsertBefore(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI);
	/** @brief ������ͼָ��Ԫ�ؽڵ�����һ����ָ�����ƿռ��Ԫ�ؽڵ� */
	BOOL TreeViewInsertAfter(Weak_XMLElem weak_refElem,CString nodeName);
	/** @brief ������ͼָ��Ԫ�ؽڵ�����һ������ָ�����ƿռ��Ԫ�ؽڵ� */
	BOOL TreeViewInsertAfter(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI);
	/** @brief ɾ������ͼָ��Ԫ�ؽڵ�(����ȫ����Ԫ�ؽڵ�) */
	BOOL TreeViewDelete(Weak_XMLElem weak_refElem);

	/** @brief ������ͼָ��Ԫ�ؽڵ���ӽڵ��б�ĩβ���һ����ָ�����ƿռ��Ԫ�ؽڵ� */
	BOOL TreeViewAppendChild(Weak_XMLElem weak_refElem,CString nodeName);
	/** @brief ������ͼָ��Ԫ�ؽڵ���ӽڵ��б�ĩβ���һ��ָ�����ƿռ��Ԫ�ؽڵ� */
	BOOL TreeViewAppendChild(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI);
	/** @brief ������ͼָ��Ԫ�ؽڵ���ӽڵ��б�ĩβ���һ��ָ����Ԫ�ؽڵ� */
	BOOL TreeViewAppendChild(Weak_XMLElem weak_refElem,Weak_XMLElem weak_newElem);
	/** @brief ɾ������ͼָ��Ԫ�ؽڵ��ȫ����Ԫ�ؽڵ� */
	BOOL TreeViewDeleteChildren(Weak_XMLElem weak_refElem);

protected:
	/**
	*@brief	��������ͼ�������������޸�Ĭ����ͼ����ʾ���ı�
	*@note 	����ͼ��Ĭ����ͼ����ʾһ���ı�
	*@see 	AbstractTreeViewManager::CreateDefTreeView()
	*/
	virtual CString GetDefTreeViewText();

	/**
	*@brief	�����Խ��������û���ͼ����ǰ�ĳ�ʼ��
	*@note 	�������������Ҫ�ڴ����û�����ͼǰ���г�ʼ��������Ӧ�÷�������
	*/
	virtual void PreCreateTreeView();	
	/**
	*@brief	�����Խ��������û���ͼ������ĺ������� 
	*@note 	�������������Ҫ�ڴ����û�����ͼ����к���������Ӧ�÷�������
	*/
	virtual void AftCreateTreeView();

	/**
	*@brief ���������������޸�ͼ�����
	*return		�����趨��ͼ���б����ָ�롣
	*@note	Ĭ�ϲ���������ͼͼ���б�
	*@see	AbstractTreeViewManager::SetTreeViewImageList()
	*/
	virtual CImageList*	GetTreeViewImageList();

	/**
	*@brief	���������������޸��û�����ͼ��ʼչ���Ĳ���
	*@return 		�û�ָ��������ͼ��ʼչ������
	*@note 	���ڵ�Ϊ��һ��ڵ㣬���ڵ��ֱ���ӽڵ�Ϊ��2��ڵ�,�Դ����ơ�
	*		Ĭ��չ������һ�㡣
	*@see	AbstractTreeViewManager::SetTreeViewInitExpandLayer()
	*/
	virtual int	GetTreeViewInitExpandLayer();

	/**
	*@brief	���������������޸����ڵ���ʾ��������Ϣ��
	*@param [in]	weak_node	��Ҫ�趨ͼ����ŵ����ڵ�ָ�롣
	*@return 		�����趨��������Ϣ��
	*@note 	Ĭ�Ϸ��ؽڵ��������Ϊ�ڵ��������Ϣ��
	*/
	virtual CString TreeNodeText(Weak_XMLElem weak_node);
	/**
	*@brief	���������������޸����ڵ���ʾ��ͼ��
	*@param [in]	weak_node	��Ҫ�趨ͼ����ŵ����ڵ�ָ��
	*@return 		�����趨��ͼ�����
	*@note 	���������ͼ������û��Ϊ����ͼ����ͼ���б���ͼ�������Ч��
	*@see 	AbstractTreeViewManager::GetTreeViewImageList()
	*/
	virtual int	TreeNodeImageIndex(Weak_XMLElem weak_node);


	/**
	*@brief	����������ͼ�����������Գ�ʼ�����ڵ��Ҽ������˵�
	*@param	[in]	weak_note	�����Ҽ��˵��ĵ�ǰ���ڵ�ָ��	
	*note	������ͼ���������Ҽ������˵������Ӳ˵���˵�������ɳ�ʼ��
	*/
	virtual BOOL InitNodeContextMenu(Weak_XMLElem weak_node,CMenu* popupMenu);
	/** 
	*@brief ����������ͼ��������������Ӧ���ڵ��Ҽ������˵�
	*@param	[in]	menuID	����Ӧ���Ҽ������˵��˵���ID
	*/
	virtual BOOL ResponseNodeContextMenu(UINT menuID);


	/** @brief ����������ͼ������������ˢ��������ͼ��Ӧ��Ԫ���б���ͼ��Ĭ��ʲô������ */
	virtual BOOL UpdateElemGridView();
	/** @brief ����������ͼ������������ˢ��������ͼ��Ӧ�������б���ͼ��Ĭ��ʲô������ */
	virtual BOOL UpdateAttrGridView();

	/** @brief ����������ͼ���������������������ļ�������Ϣ */
	virtual void OutputFileMsg(CString msg);
	/** @brief ����������ͼ���������������������ļ��������� */
	virtual void OutputFileErr(CString err);
	/** @brief ����������ͼ���������������������ļ�У����Ϣ */
	virtual void OutputValidateMsg(CString msg);
	/** @brief ����������ͼ���������������������ļ�У����� */
	virtual void OutputValidateErr(CString err);


	/** @brief ע�ᵽ�����������ؼ�ָ�� */
	CTreeCtrl*				m_treeCtrl;
	/** @brief �û���ͼ�¸��ڵ�ָ��*/
	Weak_XMLElem			m_pRoot;
	/** @brief �û���ͼ�¸��ڵ��� */
	HTREEITEM				m_hRoot;

	/**
	*@brief �û���ͼ�½ڵ�Map��[Key:�ڵ�����Value:�ڵ�ָ��] 
	*@see	XMLTreeItemMap
	*/
	XMLTreeItemMap			m_treeItemMap;

	/** 
	*@brief	�û���ͼ�½ڵ㵯���˵�map��[Key:�˵���ID��Value:�˵�����Ϣ]
	*@see	TreePopMenuMap 
	*/
	TreePopMenuMap			m_treePopMenuMap;

	/** @brief �Ƿ�������ͼ�� */
	BOOL					m_bImageValidate;
	/** @brief ����ͼ��ʼչ������ */
	int						m_initExpandLayer;

private:

	/**
	*@brief	��������ͼͼ���б�
	*@param [in]	void
	*@return 		void
	*@note 	���������������ͼ���������û��趨��ͼ���б�����Ϊ����ͼͼ���б�
	*@see 	AbstractTreeViewManager::GetTreeViewImageList()
	*/
	void SetTreeViewImageList(CImageList* imageList);
	/**
	*@brief	�����û�����ͼ��ʼչ������
	*@param [in]	layerNum	�û�ָ���ĳ�ʼչ������
	*@note 	���ڵ�Ϊ��һ��ڵ㣬���ڵ��ֱ���ӽڵ�Ϊ��2��ڵ�,�Դ����ơ�
	*@see 	AbstractTreeViewManager::GetTreeViewInitExpandLayer()
	*/
	void SetTreeViewInitExpandLayer(int layerNum);

	/**
	*@brief	�ݹ��������ͼ����
	*@param [in]	hItem		��Ҫ����������ͼ�ĸ��ڵ��<b>���ڵ�</b>���
	*@param [out]	weak_node	��Ҫ����������ͼ�ĸ��ڵ�ָ��
	*@return 		TRUE�������ɹ���FALSE������ʧ�ܡ�
	*@note 	�ú�������һ�ε���ʱΪ������ڣ���������ΪXMLģ�ͽڵ��޸��ӽڵ����ֵܽڵ㡣
	*		���������еĽڵ�������Ӧ�Ľڵ�ָ�뱻������m_treeItemMap��
	*@see 	m_treeItemMap
	*/
	BOOL DoCreateTreeView(HTREEITEM hItem,Weak_XMLElem weak_node);
	/**
	*@brief	��������ͼ�ڵ������Ľڵ����
	*@param [in]	hItem	ָ������ͼ�ڵ���
	*@return 		��ǰ�ڵ���������
	*@note 	���ڵ�Ϊ��һ��ڵ㣬���ڵ��ֱ���ӽڵ�Ϊ��2��ڵ�,�Դ����ơ�
	*		-1��ʾ���ؽ��ʧ�ܡ�
	*/
	int GetHNodeLayer(HTREEITEM hItem);
	/**
	*@brief	�ݹ��������ͼ��ʼչ��
	*@param [in]	hItem		��ǰչ���Ľڵ���
	*@note 	�ú�����һ�ε���ʱΪ�ݹ���ڣ��ݹ����Ϊ��ǰҪչ���Ľڵ����ڲ�������
	*		ָ���ı�չ��������
	*@see 	AbstratctTreeViewManager::SetTreeViewInitExpandLayer()
	*/
	void DoTreeViewInitExpand(HTREEITEM hItem);

	/** 
	*@brief �����������ڵ������Ӧָ�������ұ�
	*@note	���ڵ����Ӧ��һ��һ�����У����AddToTreeItemMapÿ�ν���ӵ�
	*		ǰ�����һ�Խڵ�����ָ��
	*/
	BOOL AddToTreeItemMap(HTREEITEM hItem,Weak_XMLElem weak_node);
	/** 
	*@brief �Ӳ��ұ���ɾ��ָ���ڵ�ľ����ָ��(����ɾ���ýڵ�ȫ�����ӽڵ�) 
	*@note	���ڵ��ɾ��ʱֻҪɾ�����ڵ㣬���ӽڵ�ͻᱻɾ����
	*		���RemoveFromTreeItemMap���Ƴ�map��ָ�����ڵ㼰��ȫ���ӽڵ�ľ��-ָ���
	*/
	BOOL RemoveFromTreeItemMap(HTREEITEM hItem);


	/** @brief ���Ҽ������˵��в���˵���ڵ�ǰ���롱 */
	BOOL	InsertPopupMenuSiblingBFInsert(Weak_XMLElem	weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵���ڵ����롱 */
	BOOL	InsertPopupMenuSiblingAFInsert(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵�������ӽڵ㡱 */
	BOOL	InsertPopupMenuChildInsert(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵��ɾ��ȫ���ӽڵ㡱 */
	BOOL	InsertPopupMenuChildDelete(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵���������ԡ� */
	BOOL	InsertPopupMenuAttributeInsert(Weak_XMLElem weak_elem,CString elemName,CString attrName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵��ɾ�����ԡ� */
	BOOL	InsertPopupMenuAttributeDelete(Weak_XMLElem weak_elem,CString elemName,CString attrName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵��У�鵱ǰ�ڵ㡱 */
	BOOL	InsertPopupMenuValidate(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief ���Ҽ������˵��в���˵��ɾ����ǰ�ڵ㡱 */
	BOOL	InsertPopupMenuDelete(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);


	/** @brief ��Ӧ�Ҽ������˵��в˵���ڵ�ǰ���롱 */
	BOOL	ResponsePopupMenuSiblingBFInsert(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵���ڵ����롱 */
	BOOL	ResponsePopupMenuSiblingAFInsert(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵�������ӽڵ㡱 */
	BOOL	ResponsePopupMenuChildInsert(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵��ɾ��ȫ���ӽڵ㡱 */
	BOOL	ResponsePopupMenuChildDelete(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵���������ԡ� */
	BOOL	ResponsePopupMenuAttributeInsert(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵��ɾ�����ԡ� */
	BOOL	ResponsePopupMenuAttributeDelete(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵��У�鵱ǰ�ڵ㡱 */
	BOOL	ResponsePopupMenuCurrentValidate(TreePopMenuPtr popmenuInfo);
	/** @brief ��Ӧ�Ҽ������˵��в˵��ɾ����ǰ�ڵ㡱 */
	BOOL	ResponsePopupMenuCurrentDelete(TreePopMenuPtr popmenuInfo);

	enum MenuType
	{
		MenuType_Sibling_InsertBF,	/**< @brief �ڵ�ǰ�ڵ�ǰ�����ֵܽڵ� */
		MenuType_Sibling_InsertAF,	/**< @brief �ڵ�ǰ�ڵ������ֵܽڵ� */
		MenuType_Child_Insert,		/**< @brief Ϊ��ǰ�ڵ����һ���ӽڵ� */
		MenuType_Child_Delete,		/**< @brief ɾ����ǰ�ڵ�������ӽڵ� */
		MenuType_Attribute_Insert,	/**< @brief Ϊ��ǰ�ڵ����һ������ */
		MenuType_Attribute_Delete,	/**< @brief ɾ����ǰ�ڵ��һ������ */
		MenuType_Current_Validate,	/**< @brief У�鵱ǰ�ڵ���Ч�� */
		MenuType_Current_Delete,	/**< @brief ɾ����ǰ�ڵ� */
	};
};

