/**
*@file	AbstractTreeViewManager.h
*@brief	抽象的XML文件树视图管理器
*@note	这个抽象树视图管理器用于XML文件模型的树视图显示，
*		使用方法是继承该抽象类
*@author	高峰
*/
#pragma once

#include <map>

#include "XMLModel.h"
#include "XMLElemNode.h"

#define WM_CATALOGUE_POPUP_MENUID_MIN			WM_USER+3000	/* 树视图弹出菜单项ID最小值 */
#define WM_CATALOGUE_POPUP_MENUID_MAX			WM_USER+3100	/* 树视图弹出菜单项ID最大值 */


/**
*@brief	XML树节点Map定义
*@note	XML树节点Map保存节点句柄与节点指针之间的映射关系，方便从节点句柄映射到节点指针
*		或从节点指针映射到节点句柄。
*/
typedef std::map<HTREEITEM,Weak_XMLElem>		XMLTreeItemMap;
typedef XMLTreeItemMap::iterator				XMLTreeItemMapItor;
typedef XMLTreeItemMap::const_iterator			XMLTreeItemMapConstItor;

typedef struct treepopmenuinfo
{
	UINT			menuID;			/**< @brief 菜单ID号 */
	int				menuType;		/**< @brief 菜单类型号 */
	CString			menuTitle;		/**< @brief 菜单标题 */

	Weak_XMLElem	weak_elem;		/**< @brief 当前菜单项针对的起始节点指针 */
	CString			elemName;		/**< @brief 当前菜单项针对的目标节点名称 */
	CString			attrName;		/**< @brief 当前菜单项针对的目标节点属性 */

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

/** @brief 抽象的XML文件树视图管理器 */
class AbstractTreeViewManager
{
public:
	AbstractTreeViewManager(void);
	virtual ~AbstractTreeViewManager(void) = 0;

	BOOL RegeisterAsTreeView(CTreeCtrl* treeCtrl);	/**< @brief 注册树控件指针到管理器 */
	CTreeCtrl* GetRegisteredTreeView();				/**< @brief 获取被注册到管理器的树控件指针 */

	BOOL CreateDefTreeView();						/**< @brief 创建默认树视图 */

	BOOL CreateTreeView(Weak_XMLElem rootNode);		/**< @brief 由用户根节点创建用户树视图 */
	void DestroyTreeView();							/**< @brief 销毁用户树视图 */

	/**
	*@brief 创建树视图右键弹出菜单
	*note	point为树视图范围内的点,而非屏幕范围内点
	*/
	BOOL CreateTreeViewContextMenu(CPoint clientPoint);
	/**
	*@brief 响应树视图右键弹出菜单项 
	*@note	在创建树视图弹出菜单时保存了菜单项map(菜单ID，菜单内容)
	*/
	BOOL ResponseTreeViewContextMenu(UINT menuID);

/******************************select operation********************************/
	HTREEITEM		GetSelectedHItem();							/**< @brief 获得当前选中的节点句柄 */
	Weak_XMLElem	GetSelectedPItem();							/**< @brief 获得当前选中的节点指针 */
	Weak_XMLElem	GetPNodeFromHNode(HTREEITEM hItem);			/**< @brief 由节点句柄查找节点指针 */
	HTREEITEM		GetHNodeFromPNode(Weak_XMLElem pNode);		/**< @brief 由节点指针查找节点句柄 */

/******************************node operation**********************************/

	/** @brief 向树视图指定元素节点前插入一个不指定名称空间的元素节点 */
	BOOL TreeViewInsertBefore(Weak_XMLElem weak_refElem,CString nodeName);
	/** @brief 向树视图指定元素节点前插入一个具有指定名称空间的元素节点 */
	BOOL TreeViewInsertBefore(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI);
	/** @brief 向树视图指定元素节点后插入一个不指定名称空间的元素节点 */
	BOOL TreeViewInsertAfter(Weak_XMLElem weak_refElem,CString nodeName);
	/** @brief 向树视图指定元素节点后插入一个具有指定名称空间的元素节点 */
	BOOL TreeViewInsertAfter(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI);
	/** @brief 删除树视图指定元素节点(包括全部子元素节点) */
	BOOL TreeViewDelete(Weak_XMLElem weak_refElem);

	/** @brief 向树视图指定元素节点的子节点列表末尾添加一个不指定名称空间的元素节点 */
	BOOL TreeViewAppendChild(Weak_XMLElem weak_refElem,CString nodeName);
	/** @brief 向树视图指定元素节点的子节点列表末尾添加一个指定名称空间的元素节点 */
	BOOL TreeViewAppendChild(Weak_XMLElem weak_refElem,CString nodeName,CString namespaceURI);
	/** @brief 向树视图指定元素节点的子节点列表末尾添加一个指定的元素节点 */
	BOOL TreeViewAppendChild(Weak_XMLElem weak_refElem,Weak_XMLElem weak_newElem);
	/** @brief 删除树视图指定元素节点的全部子元素节点 */
	BOOL TreeViewDeleteChildren(Weak_XMLElem weak_refElem);

protected:
	/**
	*@brief	用于子视图管理器重载以修改默认视图下显示的文本
	*@note 	树视图在默认视图下显示一行文本
	*@see 	AbstractTreeViewManager::CreateDefTreeView()
	*/
	virtual CString GetDefTreeViewText();

	/**
	*@brief	重载以进行子类用户视图创建前的初始化
	*@note 	若子类管理器需要在创建用户树视图前进行初始化操作，应该放在这里
	*/
	virtual void PreCreateTreeView();	
	/**
	*@brief	重载以进行子类用户视图创建后的后续操作 
	*@note 	若子类管理器需要在创建用户树视图后进行后续操作，应该放在这里
	*/
	virtual void AftCreateTreeView();

	/**
	*@brief 用于子类重载以修改图标对象。
	*return		子类设定的图标列表对象指针。
	*@note	默认不设置树视图图标列表。
	*@see	AbstractTreeViewManager::SetTreeViewImageList()
	*/
	virtual CImageList*	GetTreeViewImageList();

	/**
	*@brief	用于子类重载以修改用户树视图起始展开的层数
	*@return 		用户指定的树视图起始展开层数
	*@note 	根节点为第一层节点，根节点的直接子节点为第2层节点,以此类推。
	*		默认展开到第一层。
	*@see	AbstractTreeViewManager::SetTreeViewInitExpandLayer()
	*/
	virtual int	GetTreeViewInitExpandLayer();

	/**
	*@brief	用于子类重载以修改树节点显示的文字信息。
	*@param [in]	weak_node	需要设定图标序号的树节点指针。
	*@return 		子类设定的文字信息。
	*@note 	默认返回节点的名称作为节点的文字信息。
	*/
	virtual CString TreeNodeText(Weak_XMLElem weak_node);
	/**
	*@brief	用于子类重载以修改树节点显示的图标
	*@param [in]	weak_node	需要设定图标序号的树节点指针
	*@return 		子类设定的图标序号
	*@note 	如果子类视图管理器没有为树视图设置图标列表则图标序号无效。
	*@see 	AbstractTreeViewManager::GetTreeViewImageList()
	*/
	virtual int	TreeNodeImageIndex(Weak_XMLElem weak_node);


	/**
	*@brief	用于子类视图管理器重载以初始化树节点右键弹出菜单
	*@param	[in]	weak_note	弹出右键菜单的当前树节点指针	
	*note	子类视图管理器对右键弹出菜单增加子菜单与菜单项来完成初始化
	*/
	virtual BOOL InitNodeContextMenu(Weak_XMLElem weak_node,CMenu* popupMenu);
	/** 
	*@brief 用于子类视图管理器重载以响应树节点右键弹出菜单
	*@param	[in]	menuID	所响应的右键弹出菜单菜单项ID
	*/
	virtual BOOL ResponseNodeContextMenu(UINT menuID);


	/** @brief 用于子类视图管理器重载以刷新与树视图对应的元素列表视图，默认什么都不做 */
	virtual BOOL UpdateElemGridView();
	/** @brief 用于子类视图管理器重载以刷新与树视图对应的属性列表视图，默认什么都不做 */
	virtual BOOL UpdateAttrGridView();

	/** @brief 用于子类视图管理器重载以向界面输出文件操作消息 */
	virtual void OutputFileMsg(CString msg);
	/** @brief 用于子类视图管理器重载以向界面输出文件操作错误 */
	virtual void OutputFileErr(CString err);
	/** @brief 用于子类视图管理器重载以向界面输出文件校验信息 */
	virtual void OutputValidateMsg(CString msg);
	/** @brief 用于子类视图管理器重载以向界面输出文件校验错误 */
	virtual void OutputValidateErr(CString err);


	/** @brief 注册到管理器的树控件指针 */
	CTreeCtrl*				m_treeCtrl;
	/** @brief 用户视图下根节点指针*/
	Weak_XMLElem			m_pRoot;
	/** @brief 用户视图下根节点句柄 */
	HTREEITEM				m_hRoot;

	/**
	*@brief 用户视图下节点Map，[Key:节点句柄，Value:节点指针] 
	*@see	XMLTreeItemMap
	*/
	XMLTreeItemMap			m_treeItemMap;

	/** 
	*@brief	用户视图下节点弹出菜单map，[Key:菜单项ID，Value:菜单项信息]
	*@see	TreePopMenuMap 
	*/
	TreePopMenuMap			m_treePopMenuMap;

	/** @brief 是否有设置图标 */
	BOOL					m_bImageValidate;
	/** @brief 树视图初始展开层数 */
	int						m_initExpandLayer;

private:

	/**
	*@brief	设置树视图图标列表
	*@param [in]	void
	*@return 		void
	*@note 	这个函数将子类视图管理器中用户设定的图标列表设置为树视图图标列表
	*@see 	AbstractTreeViewManager::GetTreeViewImageList()
	*/
	void SetTreeViewImageList(CImageList* imageList);
	/**
	*@brief	设置用户树视图初始展开层数
	*@param [in]	layerNum	用户指定的初始展开层数
	*@note 	根节点为第一层节点，根节点的直接子节点为第2层节点,以此类推。
	*@see 	AbstractTreeViewManager::GetTreeViewInitExpandLayer()
	*/
	void SetTreeViewInitExpandLayer(int layerNum);

	/**
	*@brief	递归完成树视图创建
	*@param [in]	hItem		所要创建的树视图的根节点的<b>父节点</b>句柄
	*@param [out]	weak_node	所要创建的树视图的根节点指针
	*@return 		TRUE：创建成功；FALSE：创建失败。
	*@note 	该函数被第一次调用时为函数入口，出口条件为XML模型节点无父子节点与兄弟节点。
	*		创建过程中的节点句柄与相应的节点指针被保存在m_treeItemMap中
	*@see 	m_treeItemMap
	*/
	BOOL DoCreateTreeView(HTREEITEM hItem,Weak_XMLElem weak_node);
	/**
	*@brief	计算树视图节点所处的节点层数
	*@param [in]	hItem	指定树视图节点句柄
	*@return 		当前节点所处层数
	*@note 	根节点为第一层节点，根节点的直接子节点为第2层节点,以此类推。
	*		-1表示返回结果失败。
	*/
	int GetHNodeLayer(HTREEITEM hItem);
	/**
	*@brief	递归完成树视图初始展开
	*@param [in]	hItem		当前展开的节点句柄
	*@note 	该函数第一次调用时为递归入口，递归出口为当前要展开的节点所在层数等于
	*		指定的被展开层数。
	*@see 	AbstratctTreeViewManager::SetTreeViewInitExpandLayer()
	*/
	void DoTreeViewInitExpand(HTREEITEM hItem);

	/** 
	*@brief 将新增的树节点句柄与对应指针加入查找表
	*@note	树节点插入应该一个一个进行，因此AddToTreeItemMap每次仅添加当
	*		前插入的一对节点句柄与指针
	*/
	BOOL AddToTreeItemMap(HTREEITEM hItem,Weak_XMLElem weak_node);
	/** 
	*@brief 从查找表中删除指定节点的句柄与指针(包括删除该节点全部的子节点) 
	*@note	树节点的删除时只要删除根节点，其子节点就会被删除，
	*		因此RemoveFromTreeItemMap将移除map中指定根节点及其全部子节点的句柄-指针对
	*/
	BOOL RemoveFromTreeItemMap(HTREEITEM hItem);


	/** @brief 向右键弹出菜单中插入菜单项“节点前插入” */
	BOOL	InsertPopupMenuSiblingBFInsert(Weak_XMLElem	weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“节点后插入” */
	BOOL	InsertPopupMenuSiblingAFInsert(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“插入子节点” */
	BOOL	InsertPopupMenuChildInsert(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“删除全部子节点” */
	BOOL	InsertPopupMenuChildDelete(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“插入属性” */
	BOOL	InsertPopupMenuAttributeInsert(Weak_XMLElem weak_elem,CString elemName,CString attrName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“删除属性” */
	BOOL	InsertPopupMenuAttributeDelete(Weak_XMLElem weak_elem,CString elemName,CString attrName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“校验当前节点” */
	BOOL	InsertPopupMenuValidate(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);
	/** @brief 向右键弹出菜单中插入菜单项“删除当前节点” */
	BOOL	InsertPopupMenuDelete(Weak_XMLElem weak_elem,CString elemName,CMenu* popupMenu);


	/** @brief 响应右键弹出菜单中菜单项“节点前插入” */
	BOOL	ResponsePopupMenuSiblingBFInsert(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“节点后插入” */
	BOOL	ResponsePopupMenuSiblingAFInsert(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“插入子节点” */
	BOOL	ResponsePopupMenuChildInsert(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“删除全部子节点” */
	BOOL	ResponsePopupMenuChildDelete(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“插入属性” */
	BOOL	ResponsePopupMenuAttributeInsert(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“删除属性” */
	BOOL	ResponsePopupMenuAttributeDelete(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“校验当前节点” */
	BOOL	ResponsePopupMenuCurrentValidate(TreePopMenuPtr popmenuInfo);
	/** @brief 响应右键弹出菜单中菜单项“删除当前节点” */
	BOOL	ResponsePopupMenuCurrentDelete(TreePopMenuPtr popmenuInfo);

	enum MenuType
	{
		MenuType_Sibling_InsertBF,	/**< @brief 在当前节点前插入兄弟节点 */
		MenuType_Sibling_InsertAF,	/**< @brief 在当前节点后插入兄弟节点 */
		MenuType_Child_Insert,		/**< @brief 为当前节点插入一个子节点 */
		MenuType_Child_Delete,		/**< @brief 删除当前节点的所有子节点 */
		MenuType_Attribute_Insert,	/**< @brief 为当前节点插入一个属性 */
		MenuType_Attribute_Delete,	/**< @brief 删除当前节点的一个属性 */
		MenuType_Current_Validate,	/**< @brief 校验当前节点有效性 */
		MenuType_Current_Delete,	/**< @brief 删除当前节点 */
	};
};

