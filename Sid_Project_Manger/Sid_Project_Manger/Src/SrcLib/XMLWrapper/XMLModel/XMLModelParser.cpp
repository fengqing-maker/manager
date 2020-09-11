#include "stdafx.h"
#include "XMLModelParser.h"


#ifdef _DEBUG                      	/* 判断是否定义_DEBUG */
#undef THIS_FILE                   	/* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;  	/* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              	/* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */


XMLModelParser::XMLModelParser(void)
{
}
XMLModelParser::~XMLModelParser(void)
{
}

Shared_XMLModel XMLModelParser::ParseXMLDOM(CXMLDOMFilePtr xmlDOMFile)
{
	CString fileName = xmlDOMFile->GetXMLFilePath();
	
	m_xmlDOMFile = xmlDOMFile;
	m_xmlModel.reset(new XMLModel());
	CXMLDOMNodePtr domRoot = xmlDOMFile->GetRoot();
	Shared_XMLElem modelRoot;
	if (domRoot == NULL)
	{
		m_xmlDOMFile = NULL;
		m_xmlModel.reset();
		return NULL;
	}
	modelRoot = TraverseXMLDOM(domRoot);
	if (modelRoot == 0)
	{
		m_xmlDOMFile = NULL;
		m_xmlModel.reset();
		return NULL;
	}
	m_xmlModel->SetModelDOMFile(xmlDOMFile);
	m_xmlModel->SetModelRootElem(modelRoot);
	return m_xmlModel;
}

/****************************protected function*****************************/
Shared_XMLElem XMLModelParser::ParseXMLDOMItem(CXMLDOMNodePtr domNode)
{
	Shared_XMLElem modelNode = m_xmlModel->CreateXMLElem(domNode);
	return modelNode;
}

/****************************private function********************************/
Shared_XMLElem XMLModelParser::TraverseXMLDOM(CXMLDOMNodePtr domRoot)
{
	Shared_XMLElem modelRoot = ParseXMLDOMItem(domRoot);

	int nChild = domRoot->GetChildCount();
	if(nChild == 0)
	{
		return modelRoot;
	}
	else
	{
		for (int i = 0; i < nChild; i++)
		{
			CXMLDOMNodePtr domChild = domRoot->GetChildNode(i);
			ASSERT(domChild != 0);
			Shared_XMLElem modelChild = TraverseXMLDOM(domChild);

			if(modelChild != 0)
				VERIFY(modelRoot->AppendChild(modelChild));
		}
	}
	return modelRoot;
}