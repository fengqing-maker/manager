/******************************************************/
/**
*@file		AbstractFileManager.h
*@brief		抽象SCL文件管理类定义
*@note		AbstractFileManager不能用于实例化文件管理对象
*@author	高峰
*@date		[4/3/2014]
********************************************************/
#pragma once
#include "AbstractLeefModule.h"		/* 需要向子线程设置消息处理对象 */

#include "XMLModel.h"
#include "XMLModelParser.h"
#include "AbstractFileManageThread.h"
#include "AbstractFileCommonDef.h"


class XMLFileManageThread;
class AbstractFileManager : public AbstractLeefModule
{
public:
	AbstractFileManager(void);
	virtual ~AbstractFileManager(void) = 0;

	/**
	*@brief	判断文件管理模块是否打开了一个文件
	*@note 	
	*@see 	AbstractFileManager::OpenFile()
	*/
	BOOL IsFileOpened();
	/**
	*@brief	判断文件管理模块是否修改了文件
	*@note
	*/
	BOOL IsFileModified();


	/**
	*@brief	XML文件通用的新建函数
	*@return		
	*@note
	*@see	
	*/
	BOOL NewFile(XmlFileCreateCtrl fileCraeteCtrl);
	/**
	*@brief XML文件通用的打开函数
	*@param [in]	文件名称
	*@return		TRUE：成功；FALSE：失败
	*@note	打开文件实际过程在子线程中进行，这里的返回值仅是
	*		子线程启动结果，实际打开结果通过子线程消息获得。
	*@see	AbstractFileManager::doOpenFile()
	*/
	BOOL OpenFile(CString filePath);
	/**
	*@brief	XML文件通用的关闭函数
	*@return		TRUE：关闭成功；FALSE：关闭失败
	*note	关闭当前文件管理器中打开的文件。
	*/
	BOOL CloseFile();
	/**
	*@brief	XML文件通用的保存函数
	*@return		TRUE：保存成功；FALSE：保存失败
	*@note	以原文件名称保存文件管理器中打开的文件。
	*/
	BOOL SaveFile();
	/**
	*@brief	XML文件通用另存为函数
	*@return		TRUE：保存成功；FALSE：保存失败
	*@note	通过另存为对话框实现文件另存为
	*/
	BOOL SaveAsFile(CString newFilePath);

	/**
	*@brief	获取当前文件管理器管理的文件名称
	*@note 	获取的文件名称不包含路径信息
	*@see 	AbstractFileManager::GurrentFileName()
	*/
	CString CurrentFileName();
	/**
	*@brief	获取当前文件管理器管理的文件路径名称
	*@note 	获取的文件名称包含了文件名信息
	*@see 	AbstractFileManager::CurrentFilePath()
	*/
	CString CurrentFilePath();

	/**
	*@brief	获取当前文件管理器管理的<b>文件模型</b>
	*@see	Weak_XMLModel
	*/
	Weak_XMLModel	CurrentFileModel();
	/**
	*@brief	获取当前文件管理器管理的<b>文件模型根节点指针</b>
	*@see	Weak_XMLElem
	*/
	Weak_XMLElem	CurrentFileRootElem();

	/**
	*@brief	完成新建文件的具体操作
	*@return		TRUE:成功；FALSE：失败	
	*@note	此函数完成新建文件的实际操作，置于子线程中完成；
	*/
	virtual BOOL doNewFile();
	/**
	*@brief	完成打开文件的具体操作
	*@return		TRUE:成功；FALSE：失败
	*@note	此函数完成打开文件实际操作，置于子线程中完成；
	*/
	virtual BOOL doOpenFile();
	/**
	*@brief	完成关闭文件的具体操作
	*@return		TRUE:成功；FALSE：失败
	*@note	此函数完成关闭文件实际操作，置于子线程中完成；
	*/
	virtual BOOL doCloseFile();
protected:
	/** @brief 重载以改变文件管理器消息输出方式 */
	virtual void OutputMsg(CString msg);
	/** @brief 重载以改变文件管理器错误输出方式 */
	virtual void OutputErr(CString err);

	/**
	*@brief	重载以设置文件对应的Schema文件
	*@return		与XML文件DOM相关联的Schema模型
	*@note	对应的Schema Object Model将与XML文件在加载DOM时相关联
	*		若子类文件解析器指定了特殊Schema文件，则将会修改原文件中Schema信息，
	*		否则使用原文件中的Schema信息
	*@see	CXMLSOMFilePtr
	*/
	virtual CXMLSOMFilePtr	GetSchemaFile();
	/**
	*@brief 重载以设置文件解析过程中使用的解析器
	*@return	文件模型解析器智能指针
	*@note	被设置的解析器应继承于默认解析器XMLModelParser
	*@see	XMLModelParser
	*/
	virtual Unique_XMLModelParser GetModelParser();
	/**
	*@brief 重载以设置文件解析过程中使用的子线程对象
	*@return		文件解析子线程对象的智能指针
	*@note	被设置的线程类应继承于默认文件处理线程AbstractFileManageThread
	*@see	AbstractFileManageThread
	*/
	virtual Unique_FileThread	GetFileThread();

/******************************file new operation***************************/

	/** @brief 文件管理器使用的模型解析器 */
	Unique_XMLModelParser				m_xmlModelParser;
	/** @brief 文件操作线程对象 */
	Unique_FileThread					m_xmlFileThread;


	/** @brief 管理器是否打开文件标识，TRUE：已打开文件；FALSE：未打开文件 */
	BOOL								m_xmlFileOpened;
	/** @brief 当前管理器正在操作的文件名称信息，不包含路径信息 */
	CString								m_xmlFileName;
	/** @brief 当前管理器正在操作的文件路径名称，包含文件名信息 */
	CString								m_xmlFilePath;
	/** @brief 内部管理XML文件记录,[ key:文件路径 value:文件模型对象 ]	*/
	Shared_XMLModelMap					m_xmlModelMap;

	/** @brief 创建新文件控制结构 */
	XmlFileCreateCtrl					m_xmlFileCreateCtrl;

private:


};

