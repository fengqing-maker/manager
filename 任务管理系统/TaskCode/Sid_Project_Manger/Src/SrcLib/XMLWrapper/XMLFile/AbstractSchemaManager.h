/**
*@file	AbstractSchemaManager.h
*@brief	抽象的Schema文件管理器
*@note	抽象Schema文件管理器用于继承生成用户自定义的Schema文件管理器，
		不能直接用于实例化。
		在Schema文件管理器中用户完成Schema文件的读取与解析，并从解析结果
		中查询Schema约束条件。
*@author	高峰
*/
#pragma once
#include "AbstractLeefModule.h"

#include "XMLSOMFile.h"
#include "AbstractSchemaManageThread.h"

/**
*@brief	抽象Schema文件管理器
*/
class AbstractSchemaManager : public AbstractLeefModule
{
public:
	AbstractSchemaManager(void);
	virtual ~AbstractSchemaManager(void);

	/** @brief 获取Schema文件管理器中保存的SOM文件指针 */
	CXMLSOMFilePtr	GetSOMFile();

	/** @brief 判断Schema文件是否已经打开 */
	BOOL IsFileOpened();


	/**
	*@brief	打开指定的Schema文件，加载指定名称空间的Schema约束
	*@param	[in]	filePath Schema文件路径
	*@param	[in]	namespaceURI Schema文件targetNamespace
	*@return	TRUE：子线程启动成功；FALSE：子线程启动失败
	*@note	Schema管理器通过多线程方式加载Schema文件，Schema文件加载结果
	*		应通过线程消息取得。
	*/
	BOOL	Open(CString filePath,CString namespaceURI);

	/**
	*@brief	关闭当前打开的Schema文件，释放所有Schema约束
	*@return	TRUE：关闭成功；FALSE：关闭失败
	*@note	Schema管理器在主线程中进行关闭Schema文件的操作
	*/
	BOOL	Close();

	/** 
	*@brief Schema文件具体打开过程，应在子线程中调用 
	*@note	子类Schema管理器可以通过重载该函数，做一些额外工作，如错误信息输出
	*/
	virtual BOOL	doOpen();
	/**
	*@brief Schema文件具体关闭过程，在主线程中调用
	*@ntoe	子类Schema管理器可以通过重载该函数，做一些额外工作，如错误信息输出
	*/
	virtual BOOL	doClose();
protected:

	/** @brief 重载以设置Schema文件操作子线程 */
	virtual Unique_SchemaThread	GetSchemaThread() = 0;


	/** @brief 重载以设置对外消息输出方法 */
	virtual void OutputMsg(CString msg);
	/** @brief 重载以设置对外错误输出方法 */
	virtual void OutputErr(CString err);


	/** @brief Schema文件名称 */
	CString								m_filePath;
	/** @brief Schema文件targetNamespace */
	CString								m_namespaceURI;
	/** @brief Schema文件指针 */
	CXMLSOMFilePtr						m_schemaFile;
	/** @brief Schema文件操作线程指针 */
	Unique_SchemaThread					m_schemaThread;


};

