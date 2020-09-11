#ifndef ABSTRACT_FILE_COMMON_DEF_H
#define ABSTRACT_FILE_COMMON_DEF_H


/** @brief SCL文件管理模块文件类型定义 */
enum SclFileType
{
	/**@brief XML文件 */
	File_XML,
	/**@brief SSDL文件 */
	File_SSD,
	/**@brief SCD文件 */
	File_SCD,
	/**@brief ICD文件 */
	File_ICD,
	/**@brief CID文件 */
	File_CID,
	/**@brief 任意文件 */
	File_Any,
	/**@brief 不存在的文件 */
	File_None
};


typedef struct xmlfilecreatectrl
{
	CString filePath;			/**< @brief 新建文件名称(包括完整路径) */
	CString rootName;			/**< @brief 根节点名称(包括前缀) */
	CString rootNamespaceURI;	/**< @brief 根节点名称空间 */
	CString schemaLocation;		/**< @brief 引用schema位置 */
}XmlFileCreateCtrl;


#endif