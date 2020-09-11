#include "stdafx.h"
#include "SmartLock.h"

#ifdef _DEBUG                      /* �ж��Ƿ���_DEBUG */
#undef THIS_FILE                   /* ȡ��THIS_FILE�Ķ��� */
static char THIS_FILE[]=__FILE__;   /* ����THIS_FILEָ���ļ��� */
#define new DEBUG_NEW              /* �������new�꣬ȡ��new�ؼ��� */
#endif								/* ���� */


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