#include "stdafx.h"
#include "XMLCommonDef.h"


CString GetAttributeNameFromXPath(CString xpath)
{	
	CString attrName;
	int slashPos = xpath.ReverseFind(_T('/'));
	int atPos = xpath.ReverseFind(_T('@'));
	if (slashPos == -1)
	{
		/* 仅有一层路径信息 */
		if (atPos == slashPos+1)
		{
			/* 仅有的一层路径是属性限定 */
			attrName = xpath.Mid(atPos+1,xpath.GetLength() - 1);
		}
	}
	else
	{
		/* 找到最后一层路径信息 */
		if (atPos == slashPos+1)
		{
			/* 最后一层路径为属性限定 */
			attrName = xpath.Mid(atPos+1,xpath.GetLength() - atPos - 1);
		}
	}
	return attrName;
}

CString JoinXMLStringList(const XMLStringList& stringList)
{
	CString result;
	XMLStringListConstItor itor = stringList.begin();
	while (itor != stringList.end())
	{
		result += *itor;
		itor++;
	}
	return result;
}

CString JoinQName(CString prefix,CString name)
{
	CString qName;
	if (prefix.IsEmpty())
	{
		qName = name;
	}
	else
	{
		qName = prefix +_T(":") + name;
	}
	return qName;
}

CString JoinXmlnsAttrName(CString prefix)
{
	CString attrName;
	CString separatorStr = _T(":");
	if (prefix.IsEmpty())
	{
		attrName = STR_XML_XMLNS;
	}
	else
	{
		attrName = STR_XML_XMLNS + separatorStr + prefix;
	}
	return attrName;
}
BOOL SplitXmlnsAttrName(CString attrName,CString& prefix)
{
	prefix.Empty();
	if (attrName.IsEmpty())
		return FALSE;
	CString xmlnsStr = STR_XML_XMLNS;
	CString separatorStr = _T(":");
	int		totalLen = attrName.GetLength();
	int		separatorPos = -1;
	if (attrName.Find(xmlnsStr) != 0)
	{
		return FALSE;	/* 起始不是xmlns */
	}
	separatorPos = attrName.Find(separatorStr);
	if (separatorPos == -1)
	{
		if (totalLen == xmlnsStr.GetLength())
		{
			return TRUE;	/* 前缀信息为空 */
		}
		else
		{
			return FALSE;	/* 错误的属性格式 */
		}
	}
	else
	{
		prefix = attrName.Mid(separatorPos+1,totalLen-(separatorPos+1));
		return TRUE;
	}
}

CString JoinSchemaLocation(CString schemaNamespace,CString schemaPath)
{
	ASSERT(!schemaNamespace.IsEmpty() && !schemaPath.IsEmpty());
	CString schemaLocation = schemaNamespace + _T(" ") + schemaPath;
	return schemaLocation;
}
BOOL SplitSchemaLocation(CString schemaLocation,CString& schemaNamespace,CString& schemaPath)
{
	schemaNamespace.Empty();
	schemaPath.Empty();
	if(schemaLocation.IsEmpty())
		return FALSE;
	CString separatorStr = _T(" ");
	int		totalLen = schemaLocation.GetLength();
	int		separatorLen = separatorStr.GetLength();
	int		separatorPos = -1;
	separatorPos = schemaLocation.Find(separatorStr);
	ASSERT(separatorPos != -1);
	schemaNamespace = schemaLocation.Mid(0,separatorPos);
	schemaPath = schemaLocation.Mid(separatorPos+separatorLen,totalLen-(separatorPos+separatorLen));
	if (schemaNamespace.IsEmpty() || schemaPath.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


/******************************************************************************/
MSXML2::ISchemaItemPtr	_getDeclaration(MSXML2::IXMLDOMNodePtr pXMLDOMNode)
{
	if (pXMLDOMNode == NULL)
	{
		return NULL;
	}
	MSXML2::ISchemaItemPtr pSchemaItem = NULL;
	try
	{
		MSXML2::DOMNodeType nodeType = pXMLDOMNode->GetnodeType();
		CString nodeName = (LPCTSTR)(_bstr_t)pXMLDOMNode->GetnodeName();
		switch (nodeType)
		{
		case MSXML2::NODE_ELEMENT:
			break;
		case MSXML2::NODE_ATTRIBUTE:
			if (_isAttributeHasNoDeclaration(pXMLDOMNode))
			{
				return NULL;
			}
			break;
		default:
			break;
		}
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = pXMLDOMNode->GetownerDocument();
		if (pXMLDOMDocument == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMSchemaCollection2Ptr pXMLDOMSchemaCollection = 
			pXMLDOMDocument->Getnamespaces();
		if (pXMLDOMSchemaCollection == NULL)
		{
			return NULL;
		}
		pSchemaItem = pXMLDOMSchemaCollection->getDeclaration(pXMLDOMNode);
	}
	catch (_com_error& e)
	{
#if 0
		ShowComExceptionWithReason(_T("SOM Element _getDeclaration() failed."),e);

#else
		CString desc = (LPCTSTR)(_bstr_t)e.Description();
		TRACE(desc);
#endif
		return NULL;
	}
	catch (...)
	{
#if 0
		ShowComExceptionUnknow(_T("SOM Element _getDeclaration() failed."));
#endif
		return NULL;
	}
	return pSchemaItem;
}
BOOL _isAttributeHasNoDeclaration(MSXML2::IXMLDOMNodePtr pXMLDOMNode)
{
	if (pXMLDOMNode == NULL)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		MSXML2::DOMNodeType nodeType = pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ATTRIBUTE);
		CString nodeName = (LPCTSTR)(_bstr_t)pXMLDOMNode->GetnodeName();
		/* 名称空间相关属性 */
		if (nodeName.Find(_T("xmlns"),0) != -1)
		{
			result = TRUE;
		}
		/* schema位置相关属性 */
		if (nodeName.Find(_T("schemaLocation"),0) != -1
			||nodeName.Find(_T("noNamespaceSchemaLocation"),0) != -1)
		{
			result = TRUE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("SOM Element _isAttributeHasNoDeclaration() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("SOM Element _isAttributeHasNoDeclaration() failed."));
		return FALSE;
	}
	return result;
}

XMLStringList ShowFileValidateError(CString fileName,MSXML2::IXMLDOMParseError2Ptr pParseError)
{
	XMLStringList errorInfoList;
	CString position;
	CString source;
	CString xpath;
	CString reason;
	if (pParseError == NULL || fileName.IsEmpty())
	{
		return errorInfoList;
	}
	try
	{
		MSXML2::IXMLDOMParseErrorCollectionPtr pParseErrorCollection = 
			pParseError->GetallErrors();
		ASSERT(pParseErrorCollection != NULL);
		long errorCount = pParseErrorCollection->Getlength();
		MSXML2::IXMLDOMParseError2Ptr pChildParseError = NULL;
		for(long i = 0; i < errorCount; i++)
		{
			pChildParseError = pParseErrorCollection->Getitem(i);
			ASSERT(pChildParseError != NULL);
			
			long linePos=pChildParseError->Getline();
			long charPos=pChildParseError->Getlinepos();
			position.Format(_T("( %d ) Error Position: line \"%ld\",char \"%ld\" of file \"%s\".\n"),
				linePos,charPos,fileName);
			
			CString text = (LPCTSTR)(_bstr_t)pChildParseError->GetsrcText();
			source.Format(_T("Error Source Text: %s.\n"),text);
			text.Empty();
			
			text = (LPCTSTR)(_bstr_t)pChildParseError->GeterrorXPath();
			xpath.Format(_T("Error XPath:%s.\n"),text);
			text.Empty();

			text = (LPCTSTR)(_bstr_t)pChildParseError->Getreason();
			reason.Format(_T("Error Reason:%s"),text);	/* Getreason()获取的字符串结尾自带"\n",因此这里不增加"\n" */
			text.Empty();

			CString error;
			error+=position;
			error+=source;
			error+=xpath;
			error+=reason;

			errorInfoList.push_back(error);
		} 
	}
	catch (...)
	{
		ASSERT(0);
		return errorInfoList;
	}
	CString errInfo;
	int errorCount = errorInfoList.size();
	XMLStringListItor itor = errorInfoList.begin();
	for (int i = 0; i < errorCount && i < 5; i++)	/* 仅显示前5条错误信息 */
	{
		errInfo += *itor;
		itor++;
	}
	if (!errInfo.IsEmpty())
	{
		HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
		::MessageBox(hWnd,errInfo,_T("File Validation Error!"),MB_OK|MB_ICONERROR);
	}
	return errorInfoList;
}
XMLStringList ShowElemmentValidateError(CString elemName,MSXML2::IXMLDOMParseError2Ptr pParseError)
{
	XMLStringList errorInfoList;
	CString position;
	CString source;
	CString xpath;
	CString reason;
	if (pParseError == NULL || elemName.IsEmpty())
	{
		return errorInfoList;
	}
	try
	{
		MSXML2::IXMLDOMParseErrorCollectionPtr pParseErrorCollection = 
			pParseError->GetallErrors();
		ASSERT(pParseErrorCollection != NULL);
		long errorCount = pParseErrorCollection->Getlength();
		MSXML2::IXMLDOMParseError2Ptr pChildParseError = NULL;
		for(long i = 0; i < errorCount; i++)
		{
			pChildParseError = pParseErrorCollection->Getitem(i);
			ASSERT(pChildParseError != NULL);
			position.Format(_T("( %d ) Error Positon: element \"%s\".\n"),i,elemName);

			CString text = (LPCTSTR)(_bstr_t)pChildParseError->GetsrcText();
			source.Format(_T("Error Source Text: %s.\n"),text);
			text.Empty();

			text = (LPCTSTR)(_bstr_t)pChildParseError->GeterrorXPath();
			xpath.Format(_T("Error XPath:%s.\n"),text);
			text.Empty();

			text = (LPCTSTR)(_bstr_t)pChildParseError->Getreason();
			reason.Format(_T("Error Reason:%s"),text);	/* Getreason()获取的字符串结尾自带"\n",因此这里不增加"\n" */
			text.Empty();

			CString error;
			error+=position;
			error+=source;
			error+=xpath;
			error+=reason;
			errorInfoList.push_back(error);
		}
	}
	catch (...)
	{
		ASSERT(0);
		return errorInfoList;
	}
	CString errInfo;
	int errorCount = errorInfoList.size();
	XMLStringListItor itor = errorInfoList.begin();
	for (int i = 0; i < errorCount && i < 5; i++)	/* 仅显示前5条错误信息 */
	{
		errInfo += *itor;
		itor++;
	}
	if (!errInfo.IsEmpty())
	{
		HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
		::MessageBox(hWnd,errInfo,_T("Element Validation Error!"),MB_OK|MB_ICONERROR);
	}
	return errorInfoList;
}
XMLStringList ShowAttributeValidateError(CString elemName,CString attrName,MSXML2::IXMLDOMParseError2Ptr pParseError)
{
	XMLStringList errorInfoList;
	CString position;
	CString source;
	CString xpath;
	CString reason;
	if (pParseError == NULL || elemName.IsEmpty() || attrName.IsEmpty())
	{
		return errorInfoList;
	}
	try
	{
		MSXML2::IXMLDOMParseErrorCollectionPtr pParseErrorCollection = 
			pParseError->GetallErrors();
		ASSERT(pParseErrorCollection != NULL);
		long errorCount = pParseErrorCollection->Getlength();
		MSXML2::IXMLDOMParseError2Ptr pChildParseError = NULL;
		for (long i = 0; i < errorCount; i++)
		{
			pChildParseError = pParseErrorCollection->Getitem(i);
			ASSERT(pChildParseError != NULL);

			
			position.Format(_T("( %d ) Error Position: attribute \"%s\" of element \"%s\".\n"),
				i,attrName,elemName);

			CString text = (LPCTSTR)(_bstr_t)pChildParseError->GetsrcText();
			source.Format(_T("Error Source Text: %s.\n"),text);
			text.Empty();

			text = (LPCTSTR)(_bstr_t)pChildParseError->GeterrorXPath();
			xpath.Format(_T("Error XPath:%s.\n"),text);
			text.Empty();

			text = (LPCTSTR)(_bstr_t)pChildParseError->Getreason();
			reason.Format(_T("Error Reason:%s"),text);	/* Getreason()获取的字符串结尾自带"\n",因此这里不增加"\n" */
			text.Empty();

			CString error;
			error+=position;
			error+=source;
			error+=xpath;
			error+=reason;
			errorInfoList.push_back(error);
		}
	}
	catch (...)
	{
		ASSERT(0);
		return errorInfoList;
	}
	CString errInfo;
	int errorCount = errorInfoList.size();
	XMLStringListItor itor = errorInfoList.begin();
	for (int i = 0; i < errorCount && i < 5; i++)	/* 仅显示前5条错误信息 */
	{
		errInfo += *itor;
		itor++;
	}
	if (!errInfo.IsEmpty())
	{
		HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
		::MessageBox(hWnd,errInfo,_T("Attribute Validation Error!"),MB_OK|MB_ICONERROR);
	}
	return errorInfoList;
}

CString ShowComExceptionUnknow(CString info)
{
	CString errInfo = info+_T("\n");
	errInfo+=_T("Unknown Exception!");

	HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::MessageBox(hWnd,errInfo,_T("Unknown Exception!"),MB_OK|MB_ICONERROR);
	return errInfo;
}
CString ShowComExceptionWithReason(CString info,_com_error& e)
{
	CString errInfo = info+_T("\n");
	CString Code;
	CString CodeMeaning;
	CString Source;
	CString Description;

	Code.Format(_T("Exception Code:%08lx\n"),e.Error());
	CString text = e.ErrorMessage();
	CodeMeaning.Format(_T("Exception Code meaning:%s\n"),text);
	text = (LPCTSTR)(e.Source());
	Source.Format(_T("Exception Source:%s\n"),text);
	text = (LPCTSTR)(e.Description());
	Description.Format(_T("Exception Description:%s\n"),text);

	errInfo+=Code;
	errInfo+=CodeMeaning;
	errInfo+=Source;
	errInfo+=Description;

	HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::MessageBox(hWnd,errInfo,_T("COM Exception!"),MB_OK|MB_ICONERROR);
	return errInfo;
}