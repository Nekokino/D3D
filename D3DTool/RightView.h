#pragma once

#include "MenuDlg.h"
// RightView 뷰입니다.

class RightView : public CView
{
	DECLARE_DYNCREATE(RightView)

protected:
	RightView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~RightView();


public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	MenuDlg Dlg;

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


