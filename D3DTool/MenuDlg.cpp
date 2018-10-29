// MenuDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "D3DTool.h"
#include "MenuDlg.h"
#include "afxdialogex.h"
#include "DlgShortCut.h"
#include "BasicDlg.h"

// MenuDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(MenuDlg, TabDlgBase)

MenuDlg::MenuDlg(CWnd* pParent /*=NULL*/)
	: TabDlgBase(IDD_MENU, pParent), CurIndex(0)
{
	DlgShortCut::pMenuDlg = this;
}

MenuDlg::~MenuDlg()
{
	for (size_t i = 0; i < DlgVec.size(); i++)
	{
		delete DlgVec[i];
	}
}

void MenuDlg::DoDataExchange(CDataExchange* pDX)
{
	TabDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, MenuTab);
}


BEGIN_MESSAGE_MAP(MenuDlg, TabDlgBase)
	ON_BN_CLICKED(IDOK, &MenuDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MenuDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &MenuDlg::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// MenuDlg 메시지 처리기입니다.


BOOL MenuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(168, 0, 102), TRUE);
	
	CreateDlg<BasicDlg>(IDD_BASICDLG, L"BasicDlg");
	
	ShowDlg(0);

	return TRUE;
}

void MenuDlg::OnBnClickedOk()
{

}


void MenuDlg::OnBnClickedCancel()
{

}


void MenuDlg::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void MenuDlg::ShowDlg(size_t _Index)
{
	if (true == DlgVec[_Index]->SceneChange)
	{
		NTWinShortCut::GetMainSceneSystem().ChangeScene(DlgVec[_Index]->SceneName.GetString());
	}
	else
	{
		NTWinShortCut::GetMainSceneSystem().ChangeScene(nullptr);
	}

	DlgVec[CurIndex]->ShowWindow(SW_HIDE);
	DlgVec[_Index]->ShowWindow(SW_SHOW);
	MenuTab.SetCurSel((int)_Index);

	CurIndex = _Index;
}
