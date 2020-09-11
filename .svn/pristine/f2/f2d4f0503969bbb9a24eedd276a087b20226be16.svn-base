#ifndef SMART_LOCK_H
#define SMART_LOCK_H

/* “临界区”智能锁 */
class SmartCSLock
{
public:
	explicit SmartCSLock(CCriticalSection *pCS);
	virtual ~SmartCSLock();

	const CCriticalSection * Get() const { return m_pCS;}
private:
	CCriticalSection *m_pCS;
};

/* “事件”内核对象智能锁 */

#endif