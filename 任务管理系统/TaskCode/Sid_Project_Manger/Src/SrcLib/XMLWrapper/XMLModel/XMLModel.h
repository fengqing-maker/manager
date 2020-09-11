#pragma once
#include <map>
#include <list>

#include "XMLDOMFile.h"

/* 解除model与elem的相互引用关系，使用预定义声明 */
class XMLModel;
typedef	std::tr1::shared_ptr<XMLModel>				Shared_XMLModel;
typedef std::tr1::weak_ptr<XMLModel>				Weak_XMLModel;

class XMLElemNode;
typedef std::tr1::shared_ptr<XMLElemNode>			Shared_XMLElem;
typedef std::tr1::weak_ptr<XMLElemNode>				Weak_XMLElem;

typedef std::list<Shared_XMLElem>					Shared_XMLElemList;
typedef Shared_XMLElemList::iterator				Shared_XMLElemListItor;
typedef Shared_XMLElemList::const_iterator			Shared_XMLElemListConstItor;

/** @brief XML模型map */
typedef std::map<CString,Shared_XMLModel>			Shared_XMLModelMap;
typedef Shared_XMLModelMap::iterator				Shared_XMLModelMapItor;
typedef Shared_XMLModelMap::const_iterator			Shared_XMLModelMapConstItor;


/** @brief XML模型内部查找map,用于节点查找过程 */
typedef std::map<MSXML2::IXMLDOMNodePtr,Weak_XMLElem>	Weak_XMLFindMap;
typedef Weak_XMLFindMap::iterator						Weak_XMLFindMapItor;
typedef Weak_XMLFindMap::const_iterator					Weak_XMLFindMapConstItor;

class XMLModel : public std::tr1::enable_shared_from_this<XMLModel>
{
public:
	XMLModel(void);
	~XMLModel(void);

	/** @brief 获取XML模型名称 */
	CString GetModelName();

	/** @brief 设置XML模型DOM文件 */
	void SetModelDOMFile(CXMLDOMFilePtr domFile);
	/** @brief 获取XML模型DOM文件 */
	CXMLDOMFilePtr GetModelDOMFile();

	/** @brief 设置XML模型根节点 */
	void SetModelRootElem(Shared_XMLElem rootItem);
	/** @brief 获取XML模型根节点 */
	Weak_XMLElem GetModelRootElem();

	/**
	*@brief 创建XML元素节点 
	*@note	该方法创建的节点属于该模型所有，但不能通过FindXMLElem进行查找。
	*/
	Shared_XMLElem CreateXMLElem();
	/** 
	*@brief 创建XML元素节点 
	*@note	该方法创建的节点属于该模型所有，且可以通过FindXMLElem进行查找。
	*/
	Shared_XMLElem CreateXMLElem(CXMLDOMNodePtr domNode);
	/** 
	*@brief 拷贝XML元素节点 
	*@note	该方法拷贝产生的节点属于该模型所有
	*/
	Shared_XMLElem CloneXMLElem(Weak_XMLElem weak_src,BOOL bDeepClone);
	/** 
	@brief  从另一个模型中导入元素节点
	*@note	被导入的元素节点保持原有的DOM指针
	*/
	BOOL	ImportXMLElem(Weak_XMLElem weak_elem);
	/** 
	*@brief 从模型中销毁该元素节点 
	*@note	元素节点必须从模型中销毁后才真正在内存中释放
	*/
	BOOL DeleteXMLElem(Weak_XMLElem weak_elem);

	/** @brief 在m_modelFindMap中查找与DOM指针对应的节点 */
	Weak_XMLElem FindXMLElem(MSXML2::IXMLDOMNodePtr domPtr);
protected:


private:

	/** @brief 将节点加入到m_xmlElemList中用于存储 */
	BOOL AddXMLElemToStoreList(Weak_XMLElem weak_elem);
	/** @brief 将节点从m_xmlElemList中移除，被移除后的节点从内存中释放 */
	BOOL RemoveXMLElemFromStoreList(Weak_XMLElem weak_elem);

	/** @brief 将节点插入到m_modelFindMap中用于查找 */
	BOOL AddXMLElemToFindMap(Weak_XMLElem weak_elem);
	/** @brief 将节点从m_modelFindMap中移除，移除后不能再查找 */
	BOOL RemoveXMLElemFromFindMap(Weak_XMLElem weak_elem);

	/** @brief 模型关联的DOM文件 */
	CXMLDOMFilePtr					m_xmlDOMFile;
	/** @brief 模型根节点 */
	Weak_XMLElem					m_xmlRootItem;


	/** @brief 由于XML模型节点之间保存的是弱引用的智能指针，因此需要将所有
	*节点的强引用智能指针加以保存，从而不使其引用计数为0,在模型对象释放时其
	*保存的元素节点一起析构
	*/
	Shared_XMLElemList				m_xmlElemList;
	/** @brief XML节点查找表 */
	Weak_XMLFindMap					m_modelFindMap;
};

