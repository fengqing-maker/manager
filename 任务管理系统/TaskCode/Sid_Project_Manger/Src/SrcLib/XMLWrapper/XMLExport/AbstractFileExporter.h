/**
*@file	抽象的XML文件导出器
*@brief	抽象XML文件导出器不能用于实例化，定义了XML文件导出的通用过程
*@note	应该通过继承重载的方式使用抽象XML文件导出器
*@author	高峰
*/
#pragma once
#include "AbstractLeefModule.h"

#include "AbstractFileExportThread.h"

class AbstractFileExporter : public AbstractLeefModule
{
public:
	AbstractFileExporter(void);
	virtual ~AbstractFileExporter(void) = 0;
	/** @see AbstractLeefModule::GetModuleID() */
	virtual int GetModuleID() = 0;

	/**
	*@brief	执行文件导出
	*@param	[in]	bMultiThread	是否使用多线程执行
	*@return	不使用多线程时——TRUE：导出成功；FALSE，导出失败
	*			使用多线程时——TRUE：子线程启动成功；FALSE：子线程启动失败
	*@note	不使用多线程时该函数直接调用DoExportFile函数执行文件导出；
	*		使用多线程时该函数启动子线程
	*@see	
	*/
	BOOL ExportFile(BOOL bMultiThread);

	/**
	*@brief	文件导出执行函数
	*@return	TRUE：导出成功；FALSE：导出失败。
	*@note	该函数应由ExportFile()调用，完成文件导出的实际过程，
	*		子类文件导出器应重载该函数来控制文件导出的实际过程。
	*@see	AbstractFileExporter::ExportFile()
	*/
	virtual BOOL DoExportFile() = 0;

protected:

	/** @brief 必须由子类文件导出器重载以设置导出子线程指针 */
	virtual Unique_ExportThread	GetFileExportThread() = 0;


	/** @brief 文件导出子线程智能指针 */
	Unique_ExportThread					m_exportThread;

private:

	/** @brief 启动文件导出子线程 */
	BOOL StartFileExportThread();

};

