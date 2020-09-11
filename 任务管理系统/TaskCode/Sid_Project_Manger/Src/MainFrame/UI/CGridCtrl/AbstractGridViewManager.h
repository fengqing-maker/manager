/*******************************************************/
/**
*@file	AbstractGridViewManager.h
*@brief	抽象表格视图管理器模块
*@note	抽象表格视图管理器不能实例化具体管理器，而应该通过继承重载帮助
*		用户定制满足自己需要的表格视图管理器。
*@author	高峰
*@date		[4/10/2014]
********************************************************/
#pragma once

#include "./NewCellTypes/GridCellCombo.h"
#include "./NewCellTypes/GridCellCheck.h"
#include "./NewCellTypes/GridCellDateTime.h"
#include "./NewCellTypes/GridCellNumeric.h"
#include "./NewCellTypes/GridCellRich.h"

#include "GridCtrl.h"

#define WM_GRID_POPUP_MENUID_MIN			WM_USER+5000	/* 列表视图弹出菜单项ID最小值 */
#define WM_GRID_POPUP_MENUID_MAX			WM_USER+5100	/* 列表视图弹出菜单项ID最大值 */

/** @brief 表格表项类型枚举定义 */
enum GridItemType								//表格框
{
	GridItemType_Edit,			/**< @brief 编辑框类型表项 */
	GridItemType_MutilEdit,		/**< @brief 编辑框类型表项 */
	GridItemType_Combo,			/**< @brief 组合框类型表项 */
	GridItemType_Check,			/**< @brief 复选框类型表项 */
	GridItemType_DateTime,		/**< @brief 时间控件类型表项 */
	GridItemType_Extension,		/**< @brief 自定义扩展类型表项 */
	GridItemType_Button,        /**< @brief 按键 */
	GridItemType_NumEidt,       /**< @brief 数字，浮点，有符号，以及货币 */
	GridItemType_RichEidt,       /**< @brief rich Eidt */
};


//class CGridCtrl;
/**@brief 抽象的表格视图管理器 */
class AbstractGridViewManager
{
public:
	AbstractGridViewManager(void);
	virtual ~AbstractGridViewManager(void) = 0;
	/**
	*@brief	向表格视图管理器中注册被管理的表格控件
	*@param [in]	gridCtrl	注册至视图管理器的受控表格控件指针
	*@return 		TRUE:注册成功;FALSE注册失败.
	*@note 	表格视图管理器经过向其中注册表格控件后才能使用表格视图的其它方法。
	*@see 	AbstractGridViewManager::GetRegisteredGridView()
	*/
	BOOL RegisterAsGridView(CGridCtrl* gridCtrl);
	/**
	*@brief	获取当前表格视图管理器中注册的受控表格指针
	*@see 	AbstractGridViewManager::GetRegisteredGridView()
	*/
	CGridCtrl* GetRegisteredGridView() { return m_gridCtrl;};

	/**
	*@brief	创建默认的表格视图
	*@return 		TRUE：创建视图成功;FALSE：创建视图失败。
	*@note 	当用户未指定用户表格视图时，创建一个默认的表格视图。
	*		默认表格视图看起来像一个Excel表格，横向表头使用字母标识，纵向表头使用数字标识。
	*@see 	AbstractGridViewManager::CreateGridView()
	*/
	BOOL CreateDefaultGridView();

	/**
	*@brief	创建用户表格视图
	*@return 		TRUE：创建视图成功;FALSE：创建视图失败。
	*@note 	创建用户视图的函数将会帮助用户调用与用户视图有关的函数。
	*@see 	
	*/
	BOOL CreateGridView();
	/**
	*@brief	销毁用户表格视图
	*@note	用户可以重写此函数实现特定的销毁操作
	*@see 	AbstractGridViewManager::CreateGridView()
	*/
	virtual void DestroyGridView();
	/**
	*@brief	创建用户表格视图的右键弹出菜单
	*@param	[in]	point	右键菜单弹出点,必须是表格区内坐标，非屏幕坐标
	*@return		TRUE:创建成功，FALSE:创建失败
	*@note	根据右键菜单弹出点确定菜单相关项，然后动态创建菜单，并显示。
	*/
	virtual BOOL CreateGridViewContextMenu(CPoint clientPoint);
	/**
	*@brief	响应用户表格视图右键弹出菜单指定菜单项
	*@param	[in]	menuID	指定的菜单项ID
	*@return		TRUE:响应成功，FALSE:响应失败
	*@note	在弹出菜单创建过程中保存了菜单map(菜单项ID，菜单项内容)
	*/
	virtual BOOL RespGridViewContextMenu(UINT menuID);

	/** 
	*@brief 应在需要验证表格视图表项时调用该函数 
	*@note	此处的nRow、nColumn行、列号包含固定表项，不是可变表项的行、列号
	*/
	virtual BOOL ValidateGridView(int nRow,int nColumn);
	/** 
	*@brief 应在需要回写表格视图表项数据时调用该函数
	*@note	此处的nRow、nColumn行、列号包含固定表项，不是可变表项的行、列号
	*/
	virtual BOOL EndEditGridView(int nRow,int nColumn);

	/** 
	*@brief 更新表格视图所有表项数据
	*@note	更新所有表项内的数据和表格行、列数目和宽度
	*/
	BOOL UpdateGridView();
	/** 
	*@brief 更新表格内容	
	*@note	
	*/
	void RefreshRow( int nRow, int nColumn );
    /** 
	*@brief 更新表格视图行数据
	*@note	lhz add
	*/
    BOOL UpdateGridRowView( int row );
	BOOL UpdateGridFixedRowView( int row );
/******************************select operation***************************/
	/**
	*@brief	获取当前表格视图活动表项的选取范围
	*@param	[out]	rowMin	选择范围最小行号(0~rowMax)
	*@param	[out]	colMin	选择范围最小列号(0~colMax)
	*@param	[out]	rowMax	选择范围最大行号(rowMin~n)
	*@param [out]	colMax	选择范围最大列号(colMin~n)
	*@return		TRUE:获取成功；FALSE:获取失败；
	*@note	若当前表格视图可以选取表项，则返回一个当前选择的矩形范围，
	*		返回的矩形范围在活动表项范围内。
	*		若当前表格视图不可以选取表项，则返回FALSE。	
	*		若矩形返回的矩形为(-1,-1,-1,-1)则表示当前未选取任何表项。
	*/
	BOOL GetGridViewFlexSelection(int& rowMin,int& colMin,int& rowMax,int& colMax);
/********************************check operation******************************/
	/** 
	*@brief	获取表格中GridItemType_Check类型的表项的选中状态
	*@param	[in]	nRow	可变表项的行号
	*@param	[in]	nColumn	可变表项的列号
	*@return	TRUE：表项处于选中状态；
				FALSE：表项处于未选中状态或不是GridItemType_Check类型表项	
	*/
	BOOL GetCheckItemStatus(int nRow,int nColumn);
	/**
	*@brief	对表格中全部的GridItemType_Check类型的表项进行选中
	*@see	AbstractGridViewManager::DeselectAllCheckItem()
	*/
	void SelectAllCheckItem();
	/**
	*@brief	对表格中全部的GridItemType_Check类型的表项进行反选
	*@see	AbstractGridViewManager::SelectAllCheckItem()
	*/
	void DeselectAllCheckItem();
    void SelectRow(int nRow );
	void SetColumnBkColor( int nColNum, const COLORREF &cofor );
	inline	int	 OnGetCountRowCol1()
	{
		return m_fixedRowCount;
	}
	inline	int	 OnGetCountRowCol2()
	{
		return m_fixedRowCount;
	}
	inline	int	 OnGetCountRowCol3()
	{
		return m_fixedColumnCount;
	}
	inline	int	 OnGetCountRowCol4()
	{
		return m_flexColumnCount;
	}
protected:
	/**
	*@brief	重载以进行子类默认视图创建前的初始化
	*@note	若子类视图管理器需要在创建默认表格视图前进行初始化操作，应该放在这里
	*@see	AbstractGridViewManager::AftCreateDefGridView()
	*/
	virtual void PreCreateDefGridView(){};
	/**
	*@brief	重载以进行子类默认视图创建后的后续操作 
	*@note 	若子类视图管理器需要在创建默认表格视图后进行后续操作，应该放在这里
	*@see	AbstractGridViewManager::PreCreateDefGridView()
	*/
	virtual void AftCreateDefGridView(){};

	/**
	*@brief	重载以进行子类用户视图创建前的初始化
	*@note 	若子类视图管理器需要在创建用户表格视图前进行初始化操作，应该放在这里
	*see	AbstractGridViewManager::AftCreateGridView()
	*/
	virtual void PreCreateGridView(){};
	/**
	*@brief	重载以进行子类用户视图创建后的后续操作 
	*@note 	若子类视图管理器需要在创建用户表格视图后进行后续操作，应该放在这里
	*@see	AbstractGridViewManager::PreCreateGridView()
	*/
	virtual void AftCreateGridView(){};

	/**
	*@brief	提供给子类视图管理器重载以完成视图更新前的准备工作
	*@note	子类视图管理器在视图更新前需要更新视图对应的数据内容，从而影响对应的视图
	*/
	virtual void PreUpdateGridView(){};
	/** 
	*@brief	提供给子类视图管理器重载以完成视图更新后的后续操作
	*@note	子类视图管理器在视图更新后可以对视图进行特殊操作
	*/
	virtual void AftUpdateGridView(){};

	/**
	*@brief	用于子类重载以修改表格默认视图的图标列表
	*@note	若不设置默认视图的图标列表则返回0。
	*/
	virtual CImageList* GetDefGridImageList() {return NULL;};
	/**
	*@brief	用于子类继承以修改表格默认视图是否使用Virtual Mode
	*@note 	默认情况下默认视图不使用Virtual Mode
	*/
	virtual BOOL GetDefGridVirtualMode(){ return FALSE; };


	virtual int GetDefFixedColumnCount(){return 1;};	/**<@brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>固定</b>列数目 */
	virtual int GetDefFixedRowCount(){return 1;};		/**<@brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>固定</b>行数目 */
	virtual int GetDefFlexColumnCount(){return 26;};	/**<@brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>可变</b>列数目 */
	virtual int GetDefFlexRowCount(){return 20;};/**<@brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>可变</b>行数目 */
	
	/** @brief 用于子类视图管理器重载以设置<b>默认视图</b>的<b>固定行</b>的高度 */
	virtual int GetDefFixedRowHeight(int rowIndex){ return 25;};
	/** @brief 用于子类视图管理器重载以设置<b>默认视图</b>的<b>固定列</b>的宽度 */
	virtual int GetDefFixedColumnWidth(int columnIndex){return 35;};
	/** @brief 用于子类视图管理器重载以设置<b>默认视图</b>的<b>可动行</b>的高度 */
	virtual int GetDefFlexRowHeight(int rowIndex) {return 25;};
	/** @brief 用于子类视图管理器重载以设置<b>默认视图</b>的<b>可动列</b>的宽度 */
	virtual int GetDefFlexColumnWidth(int columnIndex){ return 50;};	


	/** 
	*@brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>可变</b>表项是否可编辑 
	*note	只有可变表项是能够修改为可编辑状态的，固定表项不能修改为可编辑状态。
	*		默认情况下默认视图的可变表项都是可编辑状态。
	*/
	virtual BOOL GetDefFlexItemEditable(int nRow,int nColumn){return TRUE;};

	/** @brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>固定</b>表项内容 */
	virtual CString GetDefFixedItemText(int nRow, int nColumn);
	/** @brief 用于子类视图管理器重载以改变<b>默认视图</b>的<b>可变</b>表项内容 */
	virtual CString GetDefFlexItemText(int nRow, int nColumn){ 	CString text; return text;};
	/** 
	*@brief 用于子类视图管理器重载以获取<b>默认视图</b>的<b>可变</b>表项内容更改 
	*@note	若默认表格视图的表项是可写的，则当用户修改默认表项视图的表项后该函数
	*		将会被调用，帮助用户把修改后的内容写入视图代表的<b>模型</b>中。
	*/
	virtual BOOL	SetDefFlexItemText(int nRow, int nColumn,CString text){ return FALSE;};

	/**
	*@brief	用于子类重载以修改表格用户视图的图标列表
	*@note	若不设置用户视图的图标列表则返回0。
	*/
	virtual CImageList* GetGridImageList() { return NULL;};
	/**
	*@brief	用于子类继承以修改表格用户视图是否使用Virtual Mode
	*@note 	默认情况下用户视图不使用Virtual Mode
	*/
	virtual BOOL GetGridVirtualMode(){ return FALSE;};
	//行列数默认设置
	virtual int GetFixedColumnCount() { return 1;};	/**<@brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>固定</b>列数目 */
	virtual int GetFixedRowCount()   { return 1;};	/**<@brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>固定</b>行数目 */
	virtual int GetFlexColumnCount() {
		return 26;};	/**<@brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>可变</b>列数目 */
	virtual int GetFlexRowCount()    { return 20;};	/**<@brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>可变</b>行数目 */
	//列高默认设置
	virtual int GetFixedRowHeight(int rowIndex){	return 25;};/** @brief 用于子类视图管理器重载以设置<b>用户视图</b>的<b>固定行</b>的高度 */
	virtual int GetFixedColumnWidth(int columnIndex){ return 35;};/** @brief 用于子类视图管理器重载以设置<b>用户视图</b>的<b>固定列</b>的宽度 */
	virtual int GetFlexRowHeight(int rowIndex){ return 25;};/** @brief 用于子类视图管理器重载以设置<b>用户视图</b>的<b>可动行</b>的高度 */
	virtual int GetFlexColumnWidth(int columnIndex){ return 50;};/** @brief 用于子类视图管理器重载以设置<b>用户视图</b>的<b>可动列</b>的宽度 */

	/**
	*@brief	用于子类试图管理器重载以改变<b>用户视图</b>的<b>可变</b>表项类型
	*@note	只有可变表项能够修改表项类型，固定表项不可修改表项类型。
	*		默认情况下使用编辑控件类型的表项。
	*/
	virtual GridItemType GetFlexItemType(int nRow,int nColumn){ return GridItemType_Edit;};
	virtual GridItemType GetFixedItemType( int nRow, int nColumn ){ return GridItemType_Edit;}
	/** @brief 用于子类视图管理器重载以初始化<b>用户视图</b>中<b>组合框</b>类型的可变表项 */
	virtual BOOL InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn){return TRUE;};
	/** @brief 用于子类视图管理器重载以初始化<b>用户视图</b>中<b>复选框</b>类型的可变表项 */
	virtual BOOL InitFlexCheckItem(CGridCellCheck* pCell,int nRow,int nColumn){return TRUE;};
	/** @brief 用于子类视图管理器重载以初始化<b>用户视图</b>中<b>日期控件</b>类型的可变表项 */
	virtual BOOL InitFlexDateTimeItem(CGridCellDateTime* pCell,int nRow,int nColumn){return TRUE;};
	/** @brief 用于初始化数字控件类型，是整数，还浮点等*/
	virtual BOOL InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn){return TRUE;};
	/** @brief 用于初始化数字控件类型，是整数，还浮点等*/
	virtual BOOL InitFlexRichEditItem(CGridCellRich* pCell,int nRow,int nColumn){return TRUE;};
	
	/** @brief 若表项不是默认定义类型中的一种，返回用户自定义扩展类型 */
	virtual int	GetFlexItemExtensionType(int nRow,int nColumn){return -1;};
	/** @brief 用于子类视图管理器重载以初始化<b>用户视图</b>中<b>自定义扩展</b>类型的可变表项 */
	virtual BOOL InitFlexExtensionItem(int extensionItemType,int nRow,int nColumn){return TRUE;};

	/** 
	*@brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>可变</b>表项是否可编辑 
	*note	只有可变表项是能够修改为可编辑状态的，固定表项不能修改为可编辑状态。
	*		默认情况下用户视图的可变表项都是可编辑状态。
	*/
	virtual BOOL GetFlexItemEditable(int nRow,int nColumn){ return TRUE;};
    virtual BOOL GetFixedItmeEditable( int nRow, int nColumn){ return FALSE;}

	/** @brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>固定</b>表项内容 */
	virtual CString GetFixedItemText(int nRow, int nColumn);
	/** @brief 用于子类视图管理器重载以改变<b>用户视图</b>的<b>可变</b>表项内容 */
	virtual CString GetFlexItemText(int nRow, int nColumn){ return CString();};
	/** 
	*@brief 用于子类视图管理器重载以获取<b>用户视图</b>的<b>可变</b>表项内容更改 
	*@note	若用户表格视图的表项是可写的，则当用户修改用户表项视图的表项后该函数
	*		将会被调用，帮助用户把修改后的内容写入视图代表的<b>模型</b>中。
	*/
	virtual BOOL	SetFlexItemText(int nRow, int nColumn,CString text){return TRUE;}
	
	/** 
	*@brief 用于子类视图管理器重载以验证<b>用户视图</b>可变表项的内容
	*@note	 默认无任何校验，重载覆盖
	*/
	virtual BOOL	ValidateFlexItemText(int nRow,int nColumn,CString text){ return TRUE;};

	/** 
	*@brief 用于子类视图管理器重载以初始化<b>用户视图</b>的<b>固定</b>表项弹出菜单
	*@note	用户对popupMenu增加子菜单与菜单项来完成菜单初始化。
	*		增加菜单项需要保留菜单项ID等用于响应菜单命令；
	*		增加子菜单要确保子菜单对象与子菜单句柄分离。
	*/
	virtual BOOL	InitFixedItemPopupMenu(int nRow,int nColumn,CMenu* popupMenu){ return TRUE;};
	/** 
	*@brief  
	*@note
	*/
	virtual BOOL	ResponseFixedItemPopupMenu(UINT menuID) { return FALSE;};
	/** 
	*@brief 用于子类视图管理器重载以获取<b>用户视图</b>的<b>可变</b>表项弹出菜单
	*@note	用户对popupMenu增加子菜单与菜单项来完成菜单初始化。
	*		增加菜单项需要保留菜单项ID等用于响应菜单命令；
	*		增加子菜单要确保子菜单对象与子菜单句柄分离。
	*/
	virtual BOOL	InitFlexItemPopupMenu(int nRow,int nColumn,CMenu* popupMenu){ return FALSE;};
	/** 
	*@brief	  
	*@note
	*/
	virtual BOOL	ResponseFlexItemPopupMenu(UINT menuID){ return FALSE;};


	/**@brief 注册到管理器的表格控件指针 */
	CGridCtrl*				m_gridCtrl;


	/**@brief 表格视图是否设置图标标识 */
	BOOL					m_bImageValid;
	/**@brief 表格视图是否使用Virtual Mode标识 */
	BOOL					m_bVirtualMode;

	/** @brief 当前视图固定行数目 */
	int						m_fixedRowCount;				
	/** @brief 当前视图固定列数目 */	
	int						m_fixedColumnCount;				
	/** @brief 当前视图可动行数目 */
	int						m_flexRowCount;					
	/** @brief 当前视图可动列数目 */
	int						m_flexColumnCount;				

	int						m_popupMenuRowIndex;	/** @brief 弹出菜单所在的行号 */
	int						m_popupMenuColumnIndex;	/** @brief 弹出菜单所在的列号 */

private:

	/**
	*@brief	重置表格视图管理器的默认视图
	*@note 	包括重置表格默认视图的名称、外观、尺寸、内容
	*@see 	
	*/
	void ResetDefGridView();
	/**@brief 设置默认视图的名称 */
	void SetDefGridName();
	/**@brief 设置默认视图的外观 */
	void SetDefGridLook();
	/**@brief 设置默认视图尺寸 */
	void SetDefGridSize();
	/**@brief 设置默认视图内容 */
	void SetDefGridContent();
	/** @brief 设置默认视图表项类型 */
	void SetGridItemType(int nRow,int nColumn);
	/** @brief 设置默认视图表项状态，包括表项文本内容 */
	void SetDefGridItemState(int nRow,int nColumn);
	/** 
	*@brief	默认视图Virtual Mode回调函数 
	*@param	[in]	lParam		将视图管理器的this指针通过强制类型转换传入函数
	*@param	[out]	pDispInfo	用户修改该结构体信息控制表项的内容、格式
	*@note	当默认视图使用Virtual Mode时，用户对视图表项的内容在表格控件中不进行保存，
	*		而使用该回调函数获得表项内容。
	*/
	static BOOL CALLBACK DefCallBackFunction(GV_DISPINFO* pDispInfo,LPARAM lParam);

protected:
	/**
	*@brief	重置表格视图管理器的用户视图
	*@note 	包括重置表格用户视图的名称、外观、尺寸、内容
	*@see 	
	*/
	void ResetGridView();
	/**@brief 设置用户视图的外观 */
	void SetGridLook();
	/**@brief 设置用户视图尺寸 */
	void SetGridSize();

	/**@brief 设置用户视图内容 */
	void SetGridContent();

	/** @brief 设置用户视图表项内容 */
	void SetGridItemState(int nRow,int nColumn);
	/** 
	*@brief	用户视图Virtual Mode回调函数 
	*@param	[in]	lParam		将视图管理器的this指针通过强制类型转换传入函数
	*@param	[out]	pDispInfo	用户修改该结构体信息控制表项的内容、格式
	*@note	当用户视图使用Virtual Mode时，用户对视图表项的内容在表格控件中不进行保存，
	*		而使用该回调函数获得表项内容。
	*/
	static BOOL CALLBACK CallBackFunction(GV_DISPINFO* pDispInfo,LPARAM lParam);




};

