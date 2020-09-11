/**
*@file	AbstractXMLGridViewManager.h
*@brief	抽象XML文件列表视图管理器
*@note	该抽象类具有一般表格视图管理器的功能，且包含创建XML文件相关的用户表格视图的功能。
*@author	高峰
*/
#pragma once
#include "AbstractGridViewManager.h"

#include "XMLElemNode.h"

/**
*@brief	抽象XML文件列表视图管理器
*@see	AbstractGridViewManager
*/
class AbstractXMLGridViewManager : public AbstractGridViewManager
{
public:
	AbstractXMLGridViewManager(void);
	virtual ~AbstractXMLGridViewManager(void) = 0;


	/**
	*@brief	由XML模型节点创建用户表格视图
	*@param [in]	用户指定展示的XML模型节点
	*@return 		TRUE：创建视图成功;FALSE：创建视图失败。
	*@note 	根据用户指定的XML模型节点，表格视图将对节点的属性、文本进行列表展示。
	*@see 	AbstractGridViewManager::CreateGridView()
	*/
	BOOL CreateGridView(Weak_XMLElem weak_elem);
	/**
	*@brief	销毁用户表格视图
	*@note	用户可以重写此函数实现特定的销毁操作
	*@see 	AbstractGridViewManager::DestroyGridView()
	*/
	virtual void DestroyGridView();


protected:
	/** @brief 用以子类表格视图管理器重载向外部输出文件操作消息 */
	virtual void OutputFileMsg(CString msg);
	/** @brief 用以子类表格视图管理器重载向外部输出文件操作错误 */
	virtual void OutputFileErr(CString err);
	/** @brief 用以子类表格视图管理器重载向外部输出文件校验消息 */
	virtual void OutputValidateMsg(CString msg);
	/** @brief 用以子类表格视图管理器重载向外部输出文件校验错误 */
	virtual void OutputValidateErr(CString err);

	/**@brief 用户视图下XML模型节点指针*/
	Weak_XMLElem			m_pElem;

};

