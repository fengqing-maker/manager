#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H
/************************************************************************
��̬�����ĵ���ģ��
�ŵ��ǿ��Կ��Ƶ��������ٵ�ʱ����ͨ������߳�ͬʱʹ�á�
ȱ���Ǳ����ֶ��ͷŵ�������������ڴ�й¶��
************************************************************************/
template<typename T>
class SingletonD
{
public:
	static T* GetObj()
	{
		if (m_obj == 0)
		{
			m_obj = new T();
		}
		return m_obj;
	}
	static void ReleaseObj()
	{
		if (m_obj != 0)
		{
			delete m_obj;
		}
		m_obj = 0;
	}

protected:
	SingletonD(){};
	virtual ~SingletonD(){};

private:
	static T*		m_obj;
};

template<typename T>
T* SingletonD<T>::m_obj = 0;


/************************************************************************
��̬�����ĵ���ģ��
�ŵ��ǲ���Ҫ�û��ֶ��ͷŵ�������
ȱ���ǲ��ܿ��Ƶ��ж����ͷŵ�ʱ�������ͨ�������ڶ��߳�ģ�顣
************************************************************************/
template<typename T>
class SingletonS
{
public:
	static T* GetObj()
	{
		static T obj;
		return &obj;
	}

protected:
	SingletonS(){};
	virtual ~SingletonS(){};

private:

};

#endif
