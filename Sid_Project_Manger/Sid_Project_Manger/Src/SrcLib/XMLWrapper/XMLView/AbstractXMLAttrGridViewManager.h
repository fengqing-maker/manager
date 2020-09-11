/**
*@file	AbstractXMLAttrGridViewManager.h
*@brief	抽象XML文件<b>属性</b>列表表格视图管理器
*@note	定义抽象XML文件属性列表表格视图管理器视图显示方法，
*		如果是通用表格视图显示方法应定义在AbstractGridViewManager.h中。
*@author	高峰
*/
#pragma once
#include "AbstractXMLGridViewManager.h"

#include <map>
#include "XMLGridCellForRef.h"
#include "XMLGridCellForEnum.h"
#include "CommonCtrlDef.h"		/* need for GridColumnMap */

/** @brief 属性视图对于GridItemType的扩展 */
enum AttrGridItemType
{
	AttrGridItemType_String,

	AttrGridItemType_Ref,
	AttrGridItemType_Enum,
	AttrGridItemType_Bool,
};

/** @brief 属性表格视图<b>行</b>定义 */
typedef struct  attrgridrowinfo
{
	CString title;				/**< @brief 行标题 */
	int		type;				/**< @brief 行类型 */
	Weak_XMLElem weak_elem;		/**< @brief 元素节点指针,对所有类型行都有效 */
	CString attrName;			/**< @brief 元素属性名称 */

	attrgridrowinfo()
	{
		title.Empty();
		type = -1;
		weak_elem.reset();
		attrName.Empty();
	}
}AttrGridRowInfo;
typedef std::tr1::shared_ptr<AttrGridRowInfo>		AttrGridRowPtr;
/** @brief 属性表格视图<b>行map</b>定义 */
typedef std::map<int,AttrGridRowPtr>				AttrGridRowMap;
typedef AttrGridRowMap::iterator					AttrGridRowMapItor;
typedef AttrGridRowMap::const_iterator				AttrGridRowMapConstItor;

/** @brief 属性表格视图弹出菜单菜单项定义 */
typedef struct  attrpopmenuinfo
{
	UINT			menuID;			/**< @brief 菜单ID号 */
	int				menuType;		/**< @brief 菜单类型号 */
	CString			menuTitle;		/**< @brief 菜单标题 */

	Weak_XMLElem	weak_elem;		/**< @brief 元素节点指针,对所有类型行都有效 */
	CString			attrName;		/**< @brief 元素属性名称 */
	attrpopmenuinfo()
	{
		menuID = UINT_MAX;
		menuType = -1;
		menuTitle.Empty();

		weak_elem.reset();
		attrName.Empty();
	}
}AttrPopMenuInfo;
typedef std::tr1::shared_ptr<AttrPopMenuInfo>				AttrPopMenuPtr;
typedef std::map<UINT,AttrPopMenuPtr>						AttrPopMenuMap;		/**< @brief map<菜单ID，菜单信息> */
typedef AttrPopMenuMap::iterator							AttrPopMenuMapItor;
typedef AttrPopMenuMap::const_iterator						AttrPopMenuMapConstItor;


/**
*@brief	抽象XML文件元素列表表格视图管理器
*@see	AbstractGridViewManager
*@see	AbstractXMLGridViewManager
*@see	AbstractXMLElemGridViewManager
*/
class AbstractXMLAttrGridViewManager : public AbstractXMLGridViewManager
{
public:
	AbstractXMLAttrGridViewManager(void);
	virtual ~AbstractXMLAttrGridViewManager(void) = 0;



protected:
	/** @brief 用以子类表格视图管理器重载向外部输出文件操作消息 */
	virtual void OutputFileMsg(CString msg);
	/** @brief 用以子类表格视图管理器重载向外部输出文件操作错误 */
	virtual void OutputFileErr(CString err);
	/** @brief 用以子类表格视图管理器重载向外部输出文件校验消息 */
	virtual void OutputValidateMsg(CString msg);
	/** @brief 用以子类表格视图管理器重载向外部输出文件校验错误 */
	virtual void OutputValidateErr(CString err);

	/**
	*@brief	重载以完成用户XML属性表格视图创建前的初始化过程
	*@note 	在用户表格视图创建前初始化“行map”和“列map”确定表格结构
	*@see 	AbstractGridViewManager::PreCreateGridView()
	*/
	virtual void PreCreateGridView();
	/**
	*@brief	重载以完成用户XML属性表格视图创建成功后的后续操作
	*@note	用户XML属性表格视图创建成功后对表格列宽进行调整
	*@see	AbstractGridViewManager::AftCreateGridView()
	*/
	virtual void AftCreateGridView();

	/** 
	*@brief 重载以完成用户XML属性表格视图更新前的数据更新 
	*note	视图更新前需要更新“行map”和“列map”即更新表格内部数据
	*see	AbstractGridViewManager::PreUpdateGridVIew()
	*/
	virtual void PreUpdateGridView();
	/** 
	*@brief 重载以完成用户XML属性表格视图更新后调整表格
	*@note	用户XML属性表格视图更新成功后对表格列宽进行调整
	*/
	virtual void AftUpdateGridView();

	/** @see AbstractGridViewManager::GetFlexColumnCount() */
	virtual int GetFlexColumnCount();
	/** @see AbstractGridViewManager::GetFlexRowCount() */
	virtual int GetFlexRowCount();

	/** @see AbstractGridViewManager::GetFlexColumnWidth() */
	virtual int GetFlexColumnWidth(int columnIndex);

	/** @see AbstractGridViewManager::GetFlexItemType*/
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
	/** *@see	AbstractGridViewManager::GetFlexItemExtensionType() */
	virtual int	GetFlexItemExtensionType(int nRow,int nColumn);
	/** @see AbstractGridViewManager::InitFlexComboItem() */
	virtual BOOL InitFlexExtensionItem(int extensionItemType,int nRow,int nColumn);
	/** @brief 新增对引用类型可变表项的初始化操作 */
	virtual BOOL InitFlexRefItem(CXMLGridCellForRef* pCell,int nRow,int nColumn);
	/** @brief 新增对枚举类型可变表项的初始化操作 */
	virtual BOOL InitFlexEnumItem(CXMLGridCellForEnum* pCell,int nRow,int nColumn);


	/** @see AbstractGridViewManager::GetFlexItemEditable() */
	virtual BOOL GetFlexItemEditable(int nRow,int nColumn);
	/** @see AbstractGridViewManager::GetFixedItemText() */
	virtual CString GetFixedItemText(int nRow, int nColumn);
	/** @see AbstractGridViewManager::GetFlexItemText() */
	virtual CString GetFlexItemText(int nRow, int nColumn);
	/** @see AbstractGridViewManager::SetFlexItemText() */
	virtual BOOL SetFlexItemText(int nRow, int nColumn,CString text);
	/** @see AbstractGridViewManager::ValidateFlexItemText() */
	virtual BOOL ValidateFlexItemText(int nRow,int nColumn,CString text);

	/** @see AbstractGridViewManager::InitFlexItemPopupMenu() */
	virtual BOOL InitFlexItemPopupMenu(int nRow,int nColumn,CMenu* popupMenu);
	/** @see AbstractGridViewManager::ResponseFlexItemPopupMenu() */
	virtual BOOL ResponseFlexItemPopupMenu(UINT menuID);

	/** @brief 提供给子类属性列表视图管理器重载以刷新对应的元素列表视图 */
	virtual BOOL UpdateElemGridView();
	/** @brief 提供给子类属性列表视图管理器重载以刷新对应的属性列表视图 */
	virtual BOOL UpdateAttrGridView();


	/** 
	*@brief 表格视图的<b>列</b>内容定义，应在创建表格视图前进行初始化 
	*@see	XMLAttrGridViewManager::CreateColumnMap()
	*/
	GridColumnMap			m_gridColumnMap;
	/** 
	*@brief 表格视图的<b>行</b>内容定义，应在创建表格视图前进行初始化 
	*@see	XMLAttrGridViewManager::CreateRowMap()
	*/
	AttrGridRowMap			m_gridRowMap;

	/** @brief 用户表格视图固定项弹出菜单项map */
	AttrPopMenuMap				m_gridMenuMap;
private:
	/** @brief 输出文件校验消息列表 */
	void OutputValidateMsgList(XMLStringList msgList);
	/** @brief 输出文件校验错误列表 */
	void OutputValidateErrList(XMLStringList errList);

	/**
	*@brief	初始化表格列定义
	*@see 	AbstractXMLAttrGridViewManager::m_gridColumnMap
	*/
	void CreateGridColumnMap();
	/**
	*@brief	初始化表格行定义
	*@see 	AbstractXMLAttrGridViewManager::m_gridRowMap
	*/
	void CreateGridRowMap();

	/** @brief 插入<b>属性名称</b>列 */
	void InsertAttrNameColumn();
	/** @brief 插入<b>属性值</b>列 */
	void InsertAttrValueColumn();


	/** @brief 插入<b>元素名称</b>行 */
	void InsertElemNameRow(Weak_XMLElem weak_elem);
	/** @brief 插入<b>元素属性</b>行 */
	void InsertElemAttrRow(Weak_XMLElem weak_elem,CString attrName);
	/** @brief 插入<b>元素文本</b>行 */
	void InsertElemTextRow(Weak_XMLElem weak_elem);
	/** @brief 获取元素属性列标题 */
	CString GetElemAttrTitle(CString attrName);

	/** 
	@brief 获取元素属性行对应的属性值列表项类型
	*@note 若需要使用特殊的表各项则返回GridItemType_Extension,这样就会继续调用
	*	   GetFlexItemExtensionType,从而为表项指定一个扩展类型
	*/
	GridItemType		GetElemAttrItemType(Weak_XMLElem weak_elem,CString attrName);
	/** @brief 获取元素属性行对应的属性值列表项使用的扩展类型 */
	AttrGridItemType	GetElemAttrItemExtensionType(Weak_XMLElem weak_elem,CString attrName);
	/** @brief 为引用类型的组合框获取属性值选项 */
	BOOL			GetElemAttrRefOptionMap(Weak_XMLElem weak_elem,CString attrName,RefOptionMap& refOptionMap);
	/** @brief 为枚举类型的组合框获取属性值选项 */
	BOOL			GetElemAttrEnumArray(Weak_XMLElem weak_elem,CString attrName,CStringArray& enumArray);

	/** 
	*@brief 验证元素指定属性的属性值是否合法
	*@note	根据与元素属性相关的Schema内容进行验证，
	*		若通过验证则返回TRUE；
	*		若未通过验证则询问用户是否允许输入内容，若允许则返回TRUE，否则返回FALSE；
	*/
	BOOL	ValidateElemAttrValue(Weak_XMLElem weak_elem,CString attrName,CString attrValue);
	/** 
	*@brief 校验元素指定文本值是否合法 
	*@note	根据与元素相关的Schema内容进行验证，
	*		若通过验证则返回TRUE；
	*		若未通过验证则询问用户是否允许输入内容，若允许则返回TRUE，否则返回FALSE；
	*/
	BOOL	ValidateElemTextValue(Weak_XMLElem weak_elem,CString textValue);
	/** @brief 显示“是、否”对话框,返回IDYES或IDNO */
	int		YesNoDialog(CString msg);

	/** @brief 创建表格视图弹出菜单与菜单项map */
	BOOL	CreateGridPopupMenuMap(Weak_XMLElem weak_elem,CString attrName,CMenu* popupMenu);
	/** @brief 向表格视图的弹出菜单中插入一条“插入属性”菜单项 */
	BOOL	InsertPopupMenuInsertAttr(Weak_XMLElem weak_elem,CString attrName,CMenu* popupMenu);
	BOOL	ResponsePopupMenuInsertAttr(Weak_XMLElem weak_elem,CString attrName);
	/** @brief 向表格视图的弹出菜单中插入一条“删除属性”菜单项 */
	BOOL	InsertPopupMenuDeleteAttr(Weak_XMLElem weak_elem,CString attrName,CMenu* popupMenu);
	BOOL	ResponsePopupMenuDeleteAttr(Weak_XMLElem weak_elem,CString attrName);


	/** @brief 列表“列”类型定义 */
	enum ColumnType
	{
		ColumnType_AttrName,	/**< @brief 属性名称列 */
		ColumnType_AttrValue,	/**< @brief 属性值列 */
	};
	/** @brief 列表“行”类型定义 */
	enum RowType
	{
		RowType_ElemName,		/**< @brief 节点名称行 */
		RowType_ElemAttr,		/**< @brief 节点属性行 */
		RowType_ElemText,		/**< @brief 节点文本行 */
	};
	/** @brief 列表“列”宽定义 */
	enum ColumnWidth
	{
		ColumnWidth_AttrName = 100,			/**< @brief <b>属性名称</b>列宽 */
		ColumnWidth_AttrValue = 100,		/**< @brief <b>属性值</b>列宽 */
	};

	/** @brief 列表菜单类型定义 */
	enum MenuType
	{
		MenuType_Insert,			/**< @brief 插入类型菜单 */
		MenuType_Delete,			/**< @brief 删除类型菜单 */
	};
};

