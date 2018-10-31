// BasicDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "D3DTool.h"
#include "BasicDlg.h"
#include "afxdialogex.h"
#include <NTScene.h>
#include <NTWinShortCut.h>
#include <NTFreeCamera.h>
#include <NT3DRectRenderer.h>
#include <NT3DGrid.h>



// BasicDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(BasicDlg, TabDlgBase)

BasicDlg::BasicDlg(CWnd* pParent /*=NULL*/)
	: TabDlgBase(IDD_BASICDLG, pParent)
{

}

BasicDlg::~BasicDlg()
{
}

void BasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BasicDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &BasicDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &BasicDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// BasicDlg �޽��� ó�����Դϴ�.


void BasicDlg::OnBnClickedOk()
{
	
}


void BasicDlg::OnBnClickedCancel()
{
	
}


BOOL BasicDlg::OnInitDialog()
{
	TabDlgBase::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Autoptr<NTScene> TabScene = NTWinShortCut::GetMainSceneSystem().FindScene(SceneName.GetString());

	if (nullptr == TabScene)
	{
		tassert(TRUE);
	}

	TabScene->GetMainCamera()->AddComponent<NTFreeCamera>();
	TabScene->GetMainCamera()->SetFar(10000.0f);
	TabScene->GetMainCamera()->GetNTObject()->GetTransform()->SetLocalPosition(NTVEC(0.0f, 0.0f, -50.0f));

	Autoptr<NTObject> GridObj = TabScene->CreateObject(L"Grid", 0);
	GridObj->GetTransform()->SetWorldRotation(NTVEC(90.0f, 0.0f, 0.0f));
	GridObj->GetTransform()->SetWorldScale(NTVEC(100000.0f, 100000.0f, 100000.0f));
	GridObj->AddComponent<NT3DGrid>();

	Autoptr<NTObject> Obj01 = TabScene->CreateObject(L"Obj01", 0);
	Obj01->GetTransform()->SetLocalScale(NTVEC(1.0f, 100.0f, 1.0f));
	Autoptr<NT3DRectRenderer> TT = Obj01->AddComponent<NT3DRectRenderer>();
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
