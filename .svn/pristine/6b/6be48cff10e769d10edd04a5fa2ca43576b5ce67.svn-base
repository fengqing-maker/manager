#ifndef  COMMON_DEF_H_3I47
#define  COMMON_DEF_H_3I47

#include <map>

/**@brief defination for grid control  */
typedef struct gridcolumninfo
{
	int		type;			/* 列类型 */
	CString title;			/* 文本 */
	int		width;			/* 宽度 */
	UINT    alignment;		/* 对齐方式 */
	int     dataType;       /*列类型*/
	gridcolumninfo()
	{
		type=0;title.Empty();width=0;alignment=0;
		dataType = 0;
	}
}GridColumnInfo;
typedef std::tr1::shared_ptr<GridColumnInfo>			GridColumnPtr;
typedef std::map<int,GridColumnPtr>						GridColumnMap;	/* (列序号，列信息) */
typedef GridColumnMap::iterator							GridColumnMapItor;
typedef GridColumnMap::const_iterator					GridColumnMapConstItor;

/**@brief defination for Rich Edit Control */
typedef struct textformat 
{
	CString  font;		/* 使用字体名称 */
	COLORREF color;		/* 使用字体颜色 */
	BOOL	 bUnderLine;/* 是否使用下划线 */
	BOOL	 bBold;		/* 是否使用粗体 */
	textformat()
	{
		font = _T("宋体");
		color = RGB(0,0,0);
		bUnderLine = FALSE;
		bBold = FALSE;
	}
}RichEditTextFormat;

#endif
