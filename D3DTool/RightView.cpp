// RightView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "D3DTool.h"
#include "RightView.h"


// RightView

IMPLEMENT_DYNCREATE(RightView, CView)

RightView::RightView()
{

}

RightView::~RightView()
{
}

BEGIN_MESSAGE_MAP(RightView, CView)
END_MESSAGE_MAP()


// RightView �׸����Դϴ�.

void RightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// RightView �����Դϴ�.

#ifdef _DEBUG
void RightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void RightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
BOOL RightView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	Dlg.Create(IDD_MENU, this);
	Dlg.ShowWindow(SW_SHOW);
	return 0;
}
#endif
#endif //_DEBUG


// RightView �޽��� ó�����Դϴ�.
