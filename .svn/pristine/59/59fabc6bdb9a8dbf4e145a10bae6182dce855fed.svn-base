#include "stdafx.h"
#include "MyXmlErrorHandler.h"

CDOMTreeErrorReporter::CDOMTreeErrorReporter(list<CString> *outPutStrList)
{
	m_outPutStrList = outPutStrList;
    m_bError = false;
}

CDOMTreeErrorReporter::~CDOMTreeErrorReporter()
{

}

void CDOMTreeErrorReporter::warning(const SAXParseException& toCatch)
{
	CString temp;
	const XMLCh *tmpInfo = toCatch.getMessage();
	CStringW info = tmpInfo;
	temp.Format(_T("Warning at file: %d, line:%d,  column: %d£¬ Message:"),
		toCatch.getSystemId(), toCatch.getLineNumber(), toCatch.getColumnNumber());
	temp += info;
	m_outPutStrList->push_back(temp);
}

void CDOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
	CString temp;
	CStringW info = toCatch.getMessage();
	temp.Format(_T("Error at file: %d, line:%d,  column: %d£¬ Message:"),
		toCatch.getSystemId(), toCatch.getLineNumber(), toCatch.getColumnNumber());
	temp += info;
	m_outPutStrList->push_back(temp);
	m_bError = true;
}

void CDOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
	CString temp;
	CStringW info = toCatch.getMessage();
	temp.Format(_T("Fatal Error at file: %d, line:%d,  column: %d£¬ Message:"),
		toCatch.getSystemId(), toCatch.getLineNumber(), toCatch.getColumnNumber());
	temp += info;
	m_outPutStrList->push_back(temp);
	m_bError = true;
}