/******************************************************************** 
		    created:    2015/03/31 
		    created:    27:9:2009   9:14 
		    filename:   /GridCtrl/NewCellTypes/GridCellButton.h 
		    file path:  /GridCtrl/NewCellTypes 
            file base:  GridCellButton 
		    file ext:   h 
		    author:     Li huizhen 
		     
		    purpose:    Create button cell 
*********************************************************************/  
#include "../GridCell.h"  

class CGridCellButton : public CGridCell  
{  
	friend class CGridCtrl;  
	DECLARE_DYNCREATE(CGridCellButton)  
public:  
	CGridCellButton(void);  
	~CGridCellButton(void);  
public:  
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);  
protected:  
	virtual void OnClick( CPoint PointCellRelative);  
protected:  
	CRect m_rect;  
	BOOL m_bPushing;   
};  
