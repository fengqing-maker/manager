/**
*@file	XMLModelFinder.h
*@brief	抽象XML模型搜索器
*@note	抽象XML模型搜索器提供在XML模型中根据XPath搜索节点的通用功能,用户可以
*		根据需要继承重载抽象搜索器以实现定制子类搜索器针对不同的XML文件提供搜索。
*		抽象XML模型搜索器不能用于实例化对象。
*@author	高峰
*/
#pragma once

#include "XMLElemNode.h"
#include "XMLModel.h"





/** @brief 抽象XML模型搜索器 */
class XMLModelFinder
{
public:
	XMLModelFinder(void);
	virtual ~XMLModelFinder(void) = 0;
	/** @brief XPath全局搜索，根据元素名称进行搜索，取得首个符合要求的元素 */
	Weak_XMLElem	 FindFirstElementFromGlobal(Weak_XMLModel weak_model,CString elemName);
	/** @brief XPath全局搜索，根据元素名称进行搜索，取得所有符合要求的元素列表 */
	Weak_XMLElemList FindAllElementsFromGlobal(Weak_XMLModel weak_model,CString elemName);



protected:



private:



};

