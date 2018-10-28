// LeftView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "D3DTool.h"
#include "LeftView.h"


// LeftView

IMPLEMENT_DYNCREATE(LeftView, CView)

LeftView::LeftView()
{

}

LeftView::~LeftView()
{
}

BEGIN_MESSAGE_MAP(LeftView, CView)
END_MESSAGE_MAP()


// LeftView 그리기입니다.

void LeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// LeftView 진단입니다.

#ifdef _DEBUG
void LeftView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void LeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// LeftView 메시지 처리기입니다.
