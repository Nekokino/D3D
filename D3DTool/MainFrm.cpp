
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "D3DTool.h"

#include "MainFrm.h"
#include "RightView.h"
#include "LeftView.h"
#include "ToolBuilder.h"

#include <WinCore.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("뷰 창을 만들지 못했습니다.\n");
	//	return -1;
	//}
	MainWindow.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE);
	MainWindow.CreateView(0, 0, RUNTIME_CLASS(LeftView), SIZE{ 1280, 800 }, nullptr);

	CWnd* Wnd = MainWindow.GetPane(0, 0);
	WinCore::Start<ToolBuilder>(AfxGetInstanceHandle(), L"ToolMainWindow", Wnd->m_hWnd);

	MainWindow.CreateView(0, 1, RUNTIME_CLASS(RightView), SIZE{ 600, 800 }, nullptr);
	
	MainWindow.Unlock();

	

	

	

	RECT Tmp = { 0, 0, 2000, 800 };

	AdjustWindowRect(&Tmp, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(nullptr, 0, 0, 2200, Tmp.bottom - Tmp.top + MainWindow.GetBorderY() * 2, SWP_NOMOVE | SWP_NOZORDER);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.hMenu = nullptr;
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	MainWindow.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

