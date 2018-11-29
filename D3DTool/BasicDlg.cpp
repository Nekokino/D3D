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
#include <ResourceSystem.h>
#include <NTImage.h>
#include <NT3DMeshRenderer.h>
#include <NTLight.h>
#include <NTFBX.h>
#include <NTBoneAniRenderer.h>
#include <NTFbxData.h>
#include <NTThread.h>
#include <NTSphere.h>
#include <NTRay.h>
#include <NT3DTerrainRenderer.h>
#include <NTTexture.h>
#include <NTMultiTexture.h>



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
	DDX_Control(pDX, IDC_BONETREE, BoneTree);
}


BEGIN_MESSAGE_MAP(BasicDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &BasicDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &BasicDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LOAD, &BasicDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_CREATE, &BasicDlg::OnBnClickedCreate)
	ON_NOTIFY(TVN_SELCHANGED, IDC_BONETREE, &BasicDlg::OnTvnSelchangedBonetree)
	ON_BN_CLICKED(IDC_TERRAIN, &BasicDlg::OnBnClickedTerrain)
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

	ResourceSystem<NTTexture>::Load(L"Texture", L"rock2.png");
	ResourceSystem<NTTexture>::Load(L"Texture", L"rock2_bump.png");

	ResourceSystem<NTTexture>::Load(L"Texture", L"Tile01.png");
	ResourceSystem<NTTexture>::Load(L"Texture", L"Tile01_Bump.png");

	ResourceSystem<NTTexture>::Load(L"Texture", L"Splatting.png");

	Autoptr<NTMultiTexture> MultiTex = ResourceSystem<NTMultiTexture>::Create(L"Floor01");
	MultiTex->CreateMultiTexture(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"rock2.png", L"rock2_bump.png");

	Autoptr<NTMultiTexture> MultiTex2 = ResourceSystem<NTMultiTexture>::Create(L"Floor01_01");
	MultiTex2->CreateMultiTexture(D3D11_USAGE::D3D11_USAGE_DEFAULT, L"Tile01.png", L"Tile01_Bump.png");

	

	NTFbxLoader FBXLoader;

	//FBXLoader.LoadFbx((PathSystem::FindPathString(L"Mesh") + L"Warehouse01.FBX").c_str());
	//FBXLoader.LoadFbx((PathSystem::FindPathString(L"Mesh") + L"Monster3.FBX").c_str());
	//FBXLoader.LoadFbx((PathSystem::FindPathString(L"Mesh") + L"unitychan.fbx").c_str());
	

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Autoptr<NTScene> TabScene = NTWinShortCut::GetMainSceneSystem().FindScene(SceneName.GetString());

	if (nullptr == TabScene)
	{
		tassert(TRUE);
	}

	//ResourceSystem<NTImage>::Load(L"Texture", L"SkyBox.png");

	TabScene->GetMainCamera()->AddComponent<NTFreeCamera>();
	TabScene->GetMainCamera()->SetFar(10000.0f);
	TabScene->GetMainCamera()->GetNTObject()->GetTransform()->SetLocalPosition(NTVEC(0.0f, 0.0f, -50.0f));

	//Autoptr<NTObject> Light = TabScene->CreateObject(L"PixLight", 0);
	//Autoptr<NTLight> PP = Light->AddComponent<NTLight>();

	//PP->SetLightType(NTLight::LightType::Point);
	//PP->GetTransform()->SetWorldScale(NTVEC(30.0f, 30.0f, 30.0f));
	//PP->PushLightLayer(0, 1, 2, 3, 4, 5);

	//Autoptr<NTObject> Light2 = TabScene->CreateObject(L"PixLight2", 0);
	//Autoptr<NTLight> a = Light2->AddComponent<NTLight>();
	//a->SetLightType(NTLight::LightType::Point);
	//a->GetTransform()->SetWorldPosition(NTVEC(30.0f, 0.0f, 0.0f));
	//a->GetTransform()->SetWorldScale(NTVEC(30.0f, 30.0f, 30.0f));
	//a->PushLightLayer(0, 1, 2, 3, 4, 5);

	Autoptr<NTObject> Light3 = TabScene->CreateObject(L"PixLight3", 0);
	Autoptr<NTLight> b = Light3->AddComponent<NTLight>();
	b->SetLightType(NTLight::LightType::Dir);
	b->GetTransform()->SetWorldRotation(NTVEC(90.0f, 0.0f, 0.0f));
	b->PushLightLayer(0, 1, 2, 3, 4, 5);

	//Autoptr<NTObject> Obj01 = TabScene->CreateObject(L"Obj01", 0);
	//Obj01->GetTransform()->SetLocalScale(NTVEC(1000.0f, 1000.0f, 1000.0f));
	//Autoptr<NT3DMeshRenderer> TT = Obj01->AddComponent<NT3DMeshRenderer>();
	//TT->SetMaterial(L"SkyBoxMat");
	//TT->SetMesh(L"Sphere");
	//TT->SetRasterState(L"SNONE");
	////TT->SetImage(L"SkyBox.png");

	Autoptr<NTObject> GridObj = TabScene->CreateObject(L"Grid", 0);
	GridObj->GetTransform()->SetWorldRotation(NTVEC(90.0f, 0.0f, 0.0f));
	GridObj->GetTransform()->SetWorldScale(NTVEC(10000.0f, 10000.0f, 10000.0f));
	GridObj->AddComponent<NT3DGrid>();

	Autoptr<NTObject> SphereLeft = TabScene->CreateObject(L"Left", 0);
	SphereLeft->GetTransform()->SetLocalScale(NTVEC(10.0f, 10.0f, 10.0f));
	SphereLeft->GetTransform()->SetLocalPosition(NTVEC{ -15.0f, 0.0f, 0.0f });
	Autoptr<NT3DMeshRenderer> SphereLeftMesh = SphereLeft->AddComponent<NT3DMeshRenderer>();
	//SphereLeftMesh->SetMaterial(L"VertexLightMat");
	SphereLeftMesh->SetMesh(L"Sphere");
	SphereLeftMesh->RndOpt.IsLight = 1;
	SphereLeftMesh->SetMaterial(L"DefferdMat");
	SphereLeftMesh->GetMaterial()->AddTextureData(TEXTYPE::TT_COLOR, 0, L"rock2.png");
	SphereLeftMesh->GetMaterial()->AddTextureData(TEXTYPE::TT_BUMP, 1, L"rock2_bump.png");

	Autoptr<NTObject> SphereRight = TabScene->CreateObject(L"Right", 0);
	SphereRight->GetTransform()->SetLocalScale(NTVEC(10.0f, 10.0f, 10.0f));
	SphereRight->GetTransform()->SetLocalPosition(NTVEC{ 15.0f, 0.0f, 0.0f });
	Autoptr<NT3DMeshRenderer> SphereRightMesh = SphereRight->AddComponent<NT3DMeshRenderer>();
	SphereRightMesh->RndOpt.IsDefferdOrForward = 0;
	SphereRightMesh->SetMesh(L"Sphere");

	TabScene->_3DCollisionSystem.Link(101, 100);
	TabScene->_3DCollisionSystem.Link(100, 101);

	Autoptr<NTSphere> ColLeft = SphereLeft->AddComponent<NTSphere>(100);
	Autoptr<NTSphere> ColRight = SphereRight->AddComponent<NTSphere>(101);

	Autoptr<NTRay> RayCol = TabScene->GetMainCamera()->AddComponent<NTRay>(101);
	RayCol->EnterFunc(this, &BasicDlg::ColTest);

	ColLeft->EnterFunc(this, &BasicDlg::ColTest);
	ColRight->EnterFunc(this, &BasicDlg::ColTest);
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

unsigned int __stdcall FbxLoadingThreadFunc(void* _Arg)
{
	Autoptr<NTFbxData> FbxData = ResourceSystem<NTFbxData>::Load((PathSystem::FindPathString(L"Mesh") + L"Warehouse01.FBX").c_str(), LOADMODE::FBXMODE);
	FbxData->SaveBinaryData((PathSystem::FindPathString(L"Data") + L"Warehouse01.NTFBX").c_str());

	OutputDebugString(L"Fbx Load Complete...");

	return 0;
}

void BasicDlg::OnBnClickedLoad()
{
	NTThread::NTStartThread(L"Load1", FbxLoadingThreadFunc);
}

void BasicDlg::OnBnClickedCreate()
{
	Autoptr<NTScene> TabScene = NTWinShortCut::GetMainSceneSystem().FindScene(SceneName.GetString());
	
	Autoptr<NTObject> TestAniobj = TabScene->CreateObject(L"TestAniObj", 0);
	TestAniobj->GetTransform()->SetLocalScale(NTVEC(1.0f, 1.0f, 1.0f));
	TestAniobj->GetTransform()->SetLocalPosition(NTVEC(0.0f, 0.0f, 0.0f));
	Autoptr<NTBoneAniRenderer> TestAniRenderer = TestAniobj->AddComponent<NTBoneAniRenderer>();

	Autoptr<NTFbxData> FbxData = ResourceSystem<NTFbxData>::Load((PathSystem::FindPathString(L"Data") + L"Monster3.NTFBX").c_str());
	
	
	TestAniRenderer->SetFbx(L"Monster3.NTFBX");

	NTFbxLoader FbxBoneLoader;
	FbxBoneLoader.LoadFbxBone((PathSystem::FindPathString(L"Mesh") + L"Monster3.FBX").c_str());

	BoneTree.DeleteAllItems();

	for (size_t i = 0; i < FbxBoneLoader.NewFbx->BoneVec.size(); i++)
	{
		if (FbxBoneLoader.NewFbx->BoneVec[i]->Parent == nullptr)
		{
			HTREEITEM Item = BoneTree.InsertItem(FbxBoneLoader.NewFbx->BoneVec[i]->Name, nullptr);
			BoneTree.SetItemData(Item, (DWORD_PTR)FbxData->FindBone(FbxBoneLoader.NewFbx->BoneVec[i]->Name));
		}
		else
		{
			HTREEITEM ParentItem = FindTreeItem(FbxBoneLoader.NewFbx->BoneVec[i]->Parent->Name, BoneTree, BoneTree.GetRootItem());
			HTREEITEM Item = BoneTree.InsertItem(FbxBoneLoader.NewFbx->BoneVec[i]->Name, ParentItem);
			BoneTree.SetItemData(Item, (DWORD_PTR)FbxData->FindBone(FbxBoneLoader.NewFbx->BoneVec[i]->Name));
		}
	}

	Autoptr<NTObject> BoneSphere = TabScene->CreateObject(L"Left", 0);
	BoneSphere->GetTransform()->SetLocalScale(NTVEC(10.0f, 10.0f, 10.0f));
	BoneSphere->GetTransform()->SetLocalPosition(NTVEC{ 0.0f, 0.0f, 0.0f });
	Autoptr<NT3DMeshRenderer> BoneSphereMesh = BoneSphere->AddComponent<NT3DMeshRenderer>();
	BoneSphereMesh->SetMesh(L"Sphere");
	BoneSphereMesh->RndOpt.IsLight = 1;
	BoneSphereMesh->SetMaterial(L"DefferdMat");
	BoneSphereMesh->GetMaterial()->AddTextureData(TEXTYPE::TT_COLOR, 0, L"rock2.png");
	BoneSphereMesh->GetMaterial()->AddTextureData(TEXTYPE::TT_BUMP, 1, L"rock2_bump.png");

	MT = BoneSphere->AddComponent<MoveTest>();

	MT->SetAniRenderer(TestAniRenderer);
}

void BasicDlg::ColTest(NTCollisionComponent * _Left, NTCollisionComponent * _Right)
{
	int a = 0; // 충돌 확인용 임시 함수입니다.
}


void BasicDlg::OnTvnSelchangedBonetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	UpdateData(TRUE);

	CString BoneName;

	BoneName = BoneTree.GetItemText(pNMTreeView->itemNew.hItem);

	MT->SetBoneName(BoneName);

	UpdateData(FALSE);

	*pResult = 0;
}


void BasicDlg::OnBnClickedTerrain()
{
	UINT X = 64;
	UINT Y = 64;

	Autoptr<NTScene> TabScene = NTWinShortCut::GetMainSceneSystem().FindScene(SceneName.GetString());

	Autoptr<NTObject> TerrainObj = TabScene->CreateObject(L"Terrain", 0);
	TerrainObj->GetTransform()->SetLocalPosition(NTVEC(0.0f, 0.0f, 0.0f));
	TerrainObj->GetTransform()->SetLocalScale(NTVEC(10.0f, 10.0f, 10.0f));

	Autoptr<NT3DTerrainRenderer> TerrainRenderer = TerrainObj->AddComponent<NT3DTerrainRenderer>();
	TerrainRenderer->CreateTerrain(64, 64);
	TerrainRenderer->SetRasterState(L"SNONE");
	TerrainRenderer->SetBaseTexture(L"Floor01");
	TerrainRenderer->AddSplattingTexture(L"Floor01_01", L"Splatting.png");
}
