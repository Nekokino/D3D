// MenuDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "D3DTool.h"
#include "MenuDlg.h"
#include "afxdialogex.h"
#include "DlgShortCut.h"

// MenuDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(MenuDlg, TabDlgBase)

MenuDlg::MenuDlg(CWnd* pParent /*=NULL*/)
	: TabDlgBase(IDD_MENU, pParent)
{
	DlgShortCut::pMenuDlg = this;
}

MenuDlg::~MenuDlg()
{
}

void MenuDlg::DoDataExchange(CDataExchange* pDX)
{
	TabDlgBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MenuDlg, TabDlgBase)
	ON_BN_CLICKED(IDOK, &MenuDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MenuDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// MenuDlg �޽��� ó�����Դϴ�.


BOOL MenuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(168, 0, 102), TRUE);

	return TRUE;
}

void MenuDlg::OnBnClickedOk()
{

}


void MenuDlg::OnBnClickedCancel()
{

}
