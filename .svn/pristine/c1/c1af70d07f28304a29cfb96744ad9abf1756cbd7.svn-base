#ifndef CEPORT_PERFORMANCE_TABLE_H
#define CEPORT_PERFORMANCE_TABLE_H



#include "Src/Tools/SingletonTemplate/SingletonTemplate.h"
#include "../LibHeard/excelLib/CApplication.h"
#include "../LibHeard/excelLib/CBorder.h"
#include "../LibHeard/excelLib/CBorders.h"
#include "../LibHeard/excelLib/CFont0.h"
#include "../LibHeard/excelLib/Cnterior.h"
#include "../LibHeard/excelLib/CRange.h"
#include "../LibHeard/excelLib/CShape.h"
#include "../LibHeard/excelLib/CShapes.h"
#include "../LibHeard/excelLib/CWorkbook.h"
#include "../LibHeard/excelLib/CWorkbooks.h"
#include "../LibHeard/excelLib/CWorksheets.h"
#include "../LibHeard/excelLib/CWorksheet.h"
#include "../inc/TaskExcelActionInc.h"

class CPosExcle
{
public:
	CPosExcle()
	{
		m_nRow = 0;
		m_nCol = 0;
		m_strPos = _T("");
	}
	~CPosExcle(){};

	INT32 SetPos(const INT32 f_row, const INT32 f_col)
	{
		m_nRow = f_row;
		m_nCol = f_col;
		return 2;
	}

	CString & GetPos(void)
	{
		return m_strPos;
	}

	CString & GetPos( const INT32 f_row, const INT32 f_col )
	{
		m_strPos = _T("");
		if( f_row > 0)
		{
			int step = 'Z'-'A'+1;
			if ( f_col <= step )
			{
				m_strPos.Format(_T("%c%d"), (_T('A')+f_col-1), f_row);
			}else
			{

				m_strPos.Format(_T("%c%c%d"), _T('A')+((f_col- step - 1)/step), _T('A')+(f_col-1)%step, f_row);
			}

			SetPos(f_row, f_col);
		}
		return GetPos();
	}

protected:
	INT32    m_nRow;
	INT32    m_nCol;
	CString  m_strPos;
};
#define  BEGIN_DATA_ROW  7  //数据内容开始行数
#define  DEF_PRJ_NUM     10 //默认项目任务格式为10
#define  BEGIN_DATA_COL  2  //数据内容开始列数
#define  DEF_DEPART_NUM  6  //默认部门个数为6

class CExportPerformanceTable: public CTaskExcelActionInc
{
public:
	CExportPerformanceTable( CActionServerFactory *pFactory );
	~CExportPerformanceTable();
	void ExportPerformanFile(const CString &modeDirStr, int year, int quarter, int departId, int userId );
protected:
	INT32 InitExcelCo();
	BOOL  BindExportTemplet();
	void  GetExportArray( int departId, int userId, ListInt &departIdArrary, ListInt &userIdArrary );
	BOOL  ExportUserTask( int year, int quarter, int userId );
	BOOL  WriteUserTask( int year, int quarter, int beginRow, ListTask &lstTask );
protected:
	void  SetCellValue( int row, int col, const CString &valueStr );
private:
	CApplication m_appExcel;
	CWorkbooks   m_books;
	CWorkbook    m_book;
	CWorksheets  m_sheets;
	CWorksheet   m_sheet;
	CRange       m_range;         //选择范围
	Cnterior     m_interior;    //
	CFont0       m_font;
	CBorders     m_borders;
	CBorder      m_border;
	//CRange       m_column;
	//CRange       m_row;

	CString      m_templetDirString; //模板路径
};
#endif