#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H
/************************************************************************
动态创建的单例模板
优点是可以控制单例被销毁的时机，通常与多线程同时使用。
缺点是必须手动释放单例，否则造成内存泄露。
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
静态创建的单例模板
优点是不需要用户手动释放单例对象。
缺点是不能控制单列对象被释放的时机，因此通常不用于多线程模块。
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
