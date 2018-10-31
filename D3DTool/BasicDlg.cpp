// BasicDlg.cpp : 구현 파일입니다.
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



// BasicDlg 대화 상자입니다.

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


// BasicDlg 메시지 처리기입니다.


void BasicDlg::OnBnClickedOk()
{
	
}


void BasicDlg::OnBnClickedCancel()
{
	
}


BOOL BasicDlg::OnInitDialog()
{
	TabDlgBase::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
