#pragma once

#include "XMLModel.h"
#include "XMLElemNode.h"
#include "XMLDOMFile.h"

class XMLModelParser;
/** @brief XML模型解析器智能指针 */
typedef std::unique_ptr<XMLModelParser>		Unique_XMLModelParser;

class XMLModelParser
{
public:
	XMLModelParser(void);
	virtual ~XMLModelParser(void);

	/*
	*解析器将XML文件的DOM对象解析为XML模型对象
	*@param [in]	xmlDOMFile	XML文件DOM对象
	*@param [out]	
	*@return		返回XML模型对象
	*/
	virtual Shared_XMLModel ParseXMLDOM(CXMLDOMFilePtr xmlDOMFile);

protected:
	
	/*
	*将XML文件DOM模型节点解析为XML模型节点
	*@param [in]		XML文件DOM节点
	*@param [out]		
	*@return			XML模型节点
	*/
	virtual Shared_XMLElem ParseXMLDOMItem(CXMLDOMNodePtr domNode);

	
	

private:
	/**
	*遍历XML文件DOM树对其中节点进行解析
	*@param [in]	XML文件DOM树根节点
	*@param [out]
	*@return		XML模型根节点
	*/
	virtual Shared_XMLElem	TraverseXMLDOM(CXMLDOMNodePtr domRoot);


	/** @brief 当前被解析DOM文件 */
	CXMLDOMFilePtr			m_xmlDOMFile;
	/** @brief 解析结果模型 */
	Shared_XMLModel			m_xmlModel;

};

