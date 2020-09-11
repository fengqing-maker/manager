#include "stdafx.h"
#include "AbstractXMLGridViewManager.h"


AbstractXMLGridViewManager::AbstractXMLGridViewManager(void)
{
}
AbstractXMLGridViewManager::~AbstractXMLGridViewManager(void)
{
}

BOOL AbstractXMLGridViewManager::CreateGridView(Weak_XMLElem weak_elem)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	m_pElem = weak_elem;

	return AbstractGridViewManager::CreateGridView();
}
void AbstractXMLGridViewManager::DestroyGridView()
{
	AbstractGridViewManager::DestroyGridView();
	m_pElem.reset();
}

/******************************protected function*************************/
void AbstractXMLGridViewManager::OutputFileMsg(CString msg)
{

}
void AbstractXMLGridViewManager::OutputFileErr(CString err)
{

}
void AbstractXMLGridViewManager::OutputValidateMsg(CString msg)
{

}
void AbstractXMLGridViewManager::OutputValidateErr(CString err)
{

}
