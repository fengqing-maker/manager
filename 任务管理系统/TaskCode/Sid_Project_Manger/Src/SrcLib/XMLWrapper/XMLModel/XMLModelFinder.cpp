#include "StdAfx.h"
#include "XMLModelFinder.h"


#ifdef _DEBUG                      	/* 判断是否定义_DEBUG */
#undef THIS_FILE                   	/* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;  	/* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              	/* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */

XMLModelFinder::XMLModelFinder(void)
{
}
XMLModelFinder::~XMLModelFinder(void)
{
}

Weak_XMLElem XMLModelFinder::FindFirstElementFromGlobal(Weak_XMLModel weak_model,CString elemName)
{
	Weak_XMLElem resultElem;
	Shared_XMLModel shared_model = weak_model.lock();
	if (shared_model != 0)
	{
		CXMLDOMFilePtr domFile = shared_model->GetModelDOMFile();
		ASSERT(domFile != 0);
		CString defPrefix = domFile->GetDefaultElementPrefix();
		CString xpath;
		if (defPrefix.IsEmpty())
		{
			xpath.Format(_T("//%s"),elemName);
		}
		else
		{
			xpath.Format(_T("//%s:%s"),defPrefix,elemName);
		}
		CXMLDOMNodePtr domNode = domFile->SelectSingleNode(xpath);
		if (domNode != NULL)
		{
			MSXML2::IXMLDOMNodePtr domPtr = domNode->GetInnerPtr();
			/* 通过模型中内建的查找表查找对应的模型节点 */
			Weak_XMLElem weak_elem = shared_model->FindXMLElem(domPtr);
			Shared_XMLElem shared_elem = weak_elem.lock();
			if (shared_elem != 0)
			{
				resultElem = shared_elem;
			}
		}
	}
	return resultElem;
}
Weak_XMLElemList XMLModelFinder::FindAllElementsFromGlobal(Weak_XMLModel weak_model,CString elemName)
{
	Weak_XMLElemList resultList;
	Shared_XMLModel shared_model = weak_model.lock();
	if (shared_model != 0)
	{
		CXMLDOMFilePtr domFile = shared_model->GetModelDOMFile();
		ASSERT(domFile != 0);
		CString defPrefix = domFile->GetDefaultElementPrefix();
		CString xpath;
		if (defPrefix.IsEmpty())
		{
			xpath.Format(_T("//%s"),elemName);
		}
		else
		{
			xpath.Format(_T("//%s:%s"),defPrefix,elemName);
		}
		CXMLDOMNodesPtr nodeList = domFile->SelectNodes(xpath);
		int elemCount = nodeList->GetCount();
		for (int i = 0; i < elemCount; i++)
		{
			CXMLDOMNodePtr domNode = nodeList->GetItem(i);
			MSXML2::IXMLDOMNodePtr domPtr = domNode->GetInnerPtr();
			/* 通过模型中内建的查找表查找对应的模型节点 */
			Weak_XMLElem weak_elem = shared_model->FindXMLElem(domPtr);
			Shared_XMLElem shared_elem = weak_elem.lock();
			if (shared_elem != 0)
			{
				resultList.push_back(weak_elem);
			}
		}
	}
	return resultList;
}


/******************************protected function*************************/


/******************************private function***************************/
