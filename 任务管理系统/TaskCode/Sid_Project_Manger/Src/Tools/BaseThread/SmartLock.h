#ifndef SMART_LOCK_H
#define SMART_LOCK_H

/* ���ٽ����������� */
class SmartCSLock
{
public:
	explicit SmartCSLock(CCriticalSection *pCS);
	virtual ~SmartCSLock();

	const CCriticalSection * Get() const { return m_pCS;}
private:
	CCriticalSection *m_pCS;
};

/* ���¼����ں˶��������� */

#endif