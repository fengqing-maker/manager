/**
*@file	AbstractXMLElemGridViewManager.h
*@brief	抽象XML文件<B>元素</B>列表表格视图管理器
*@note	定义抽象XML文件元素列表表格视图管理器视图显示方法，
*		如果是通用表格视图显示方法应定义在AbstractGridViewManager.h中。
*@author	高峰
*/
#pragma once
#include "AbstractXMLGridViewManager.h"

#include <map>
#include "CommonCtrlDef.h"			/* need for GridColumnMap */


/** @brief 元素表格视图“行”定义 */
typedef std::map<int,Weak_XMLElem>					ElemGridRowMap;
typedef ElemGridRowMap::iterator					ElemGridRowMapItor;
typedef ElemGridRowMap::const_iterator				ElemGridRowMapConstItor;

/**
*@brief	抽象XML文件元素列表表格视图管理器
*@see	AbstractGridViewManager
*@see	AbstractXMLGridViewManager
*@see	AbstractXMLAttrGridViewManager
*/
class AbstractXMLElemGridViewManager : public AbstractXMLGridViewManager
{
public:
	AbstractXMLElemGridViewManager(void);
	virtual ~AbstractXMLElemGridViewManager(void) = 0;

	/** @brief	获取当前列表视图选择的XML元素指针*/
	Weak_XMLElem GetSelectedElem();

protected:

	/**
	*@brief	重载以完成用户XML元素表格视图创建前的初始化过程
	*@note 	在用户表格视图创建前初始化“行map”和“列map”确定表格结构
	*@see 	AbstractGridViewManager::PreCreateGridView()
	*/
	virtual void PreCreateGridView();
	/**
	*@brief	重载以完成用户XML元素表格视图创建后自动调整表项宽度
	*@see	AbstractGridViewManager::AftCreateGridView()
	*/
	virtual void AftCreateGridView();

	/** 
	*@brief 重载以完成用户XML元素表格视图更新前的数据更新 
	*note	视图更新前需要更新“行map”和“列map”即更新表格内部数据
	*see	AbstractGridViewManager::PreUpdateGridVIew()
	*/
	virtual void PreUpdateGridView();
	/** 
	*@brief 重载以完成用户XML元素表格视图更新后调整表格
	*@note	用户XML元素表格视图更新成功后对表格列宽进行调整
	*/
	virtual void AftUpdateGridView();

	/** @see AbstractGridViewManager::GetFlexColumnCount() */
	virtual int GetFlexColumnCount();
	/** @see AbstractGridViewManager::GetFlexRowCount() */
	virtual int GetFlexRowCount();

	/** @see AbstractGridViewManager::GetFlexColumnWidth() */
	virtual int GetFlexColumnWidth(int columnIndex);


	/** @see AbstractGridViewManager::GetFlexItemEditable() */
	virtual BOOL GetFlexItemEditable(int nRow,int nColumn);
	/** @see AbstractGridViewManager::GetFixedItemText() */
	virtual CString GetFixedItemText(int nRow, int nColumn);
	/** @see AbstractGridViewManager::GetFlexItemText() */
	virtual CString GetFlexItemText(int nRow, int nColumn);



	/** 
	*@brief 元素表格视图的<b>列</b>内容定义，应在创建表格视图前进行初始化 
	*@see	AbstractXMLElemGridViewManager::CreateColumnMap()
	*/
	GridColumnMap			m_gridColumnMap;
	/** 
	*@brief 元素表格视图的<b>行</b>内容定义，应在创建表格视图前进行初始化 
	*@see	AbstractXMLElemGridViewManager::CreateRowMap()
	*/
	ElemGridRowMap			m_gridRowMap;


private:
	/**
	*@brief	设置元素列表视图样式
	*@note	1.将元素列表视图设置为列表模式(ListMode);
	*		2.将元素列表视图设置为单行选取模式
	*/
	void SetElemGridViewStyle();
	/**
	*@brief	初始化表格列定义
	*@see 	AbstractXMLElemGridViewManager::m_gridColumnMap
	*/
	void CreateGridColumnMap();
	/**
	*@brief	初始化表格行定义
	*@see 	AbstractXMLElemGridViewManager::m_gridRowMap
	*/
	void CreateGridRowMap();

	/** @brief 插入节点名称列 */
	void InsertNodeNameColumn();
	/** @brief 插入节点属性列 */
	void InsertNodeAttrColumn(CString attrName);
	/** @brief 插入节点文本列 */
	void InsertNodeTextColumn();
	/** @brief 根据属性名称获取属性列<b>标题</b> */
	CString GetAttrColumnTitle(CString attrName);

	/** @brief 获取节点名称列数据 */
	CString GetNodeNameValue(Weak_XMLElem weak_elem);
	/** @brief 获取节点属性列数据 */
	CString GetNodeAttrValue(Weak_XMLElem weak_attr,CString attrName);
	/** @brief 获取节点文本列 */
	CString GetNodeTextValue(Weak_XMLElem weak_elem);
	/** @brief 从列标题中获取元素属性名称 */
	CString GetAttrNameFromTitle(CString title);

	/** @brief 列表“列”类型定义 */
	enum ColumnType{ColumnType_Name,ColumnType_Attr,ColumnType_Text};
	/** @brief 列表“列”宽度定义 */
	enum ColumnWidth{ColumnWidth_Name = 100,ColumnWidth_Attr = 100,ColumnWidth_Text = 100};


};

