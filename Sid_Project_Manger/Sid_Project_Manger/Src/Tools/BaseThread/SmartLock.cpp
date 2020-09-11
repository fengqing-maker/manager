#include "stdafx.h"
#include "SmartLock.h"

#ifdef _DEBUG                      /* 判断是否定义_DEBUG */
#undef THIS_FILE                   /* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;   /* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              /* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */


SmartCSLock::SmartCSLock(CCriticalSection *pCS)
{
	if (pCS != NULL && pCS->IsKindOf(RUNTIME_CLASS(CCriticalSection)))
	{
		m_pCS = pCS;
		m_pCS->Lock();
	}
	else
	{
		m_pCS = NULL;
	}
}
SmartCSLock::~SmartCSLock()
{
	if (m_pCS != NULL && m_pCS->IsKindOf(RUNTIME_CLASS(CCriticalSection)))
	{
		m_pCS->Unlock();
	}
	m_pCS = NULL;
}