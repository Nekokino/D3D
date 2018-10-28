
// UnityBuildMaker.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UnityBuildMaker.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::wstring PathSystem::Root;
std::wstring PathSystem::BinRoot;


// CUnityBuildMakerApp

BEGIN_MESSAGE_MAP(CUnityBuildMakerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CUnityBuildMakerApp::OnAppAbout)
END_MESSAGE_MAP()


// CUnityBuildMakerApp 생성

CUnityBuildMakerApp::CUnityBuildMakerApp()
{
	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UnityBuildMaker.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CUnityBuildMakerApp 개체입니다.

CUnityBuildMakerApp theApp;


// CUnityBuildMakerApp 초기화

BOOL CUnityBuildMakerApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	PathSystem::Init();

	CreateCpp((PathSystem::Root + L"D3DEngine\\").c_str(), L"D3DEngineUnity.cpp");
	CreateCpp((PathSystem::Root + L"D3DClient\\").c_str(), L"D3DClientUnity.cpp");
	CreateCpp((PathSystem::Root + L"D3DMaker\\").c_str(), L"D3DMakerUnity.cpp");
	CreateCpp((PathSystem::Root + L"D3DTool\\").c_str(), L"D3DToolUnity.cpp");

	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	return TRUE;
}

int CUnityBuildMakerApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	return CWinApp::ExitInstance();
}

void CUnityBuildMakerApp::CreateCpp(const wchar_t * _Path, const wchar_t * _FileName)
{
	CFileFind FileFind;
	CString FindPath = _Path;
	BOOL bFile = FileFind.FindFile(FindPath + L"*.cpp");

	CString SavePath = PathSystem::BinRoot.c_str();
	SavePath += L"Unity\\";
	SavePath += _FileName;

	_WIN32_FILE_ATTRIBUTE_DATA FolderAttData;
	_WIN32_FILE_ATTRIBUTE_DATA FileAttData;
	LARGE_INTEGER FolderTime;
	LARGE_INTEGER FileTime;

	GetFileAttributesEx(_Path, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FolderAttData);
	GetFileAttributesEx(SavePath.GetString(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttData);

	memcpy_s(&FolderTime, sizeof(LARGE_INTEGER), &(FolderAttData.ftLastWriteTime), sizeof(LARGE_INTEGER));
	memcpy_s(&FileTime, sizeof(LARGE_INTEGER), &(FileAttData.ftLastWriteTime), sizeof(LARGE_INTEGER));

	//if (FolderTime.QuadPart < FileTime.QuadPart)
	//{
	//	return;
	//}

	FILE* pFile;
	_wfopen_s(&pFile, SavePath.GetString(), L"wt");

	CString FilePath = PathSystem::Root.c_str();
	FilePath += L"D3DUnity\\";
	FilePath += _FileName;

	FILE* pFile2;
	_wfopen_s(&pFile2, FilePath.GetString(), L"wt");

	while (bFile)
	{
		bFile = FileFind.FindNextFileW();

		if (FileFind.IsDots())
		{
			continue;
		}
		else if (FileFind.IsDirectory())
		{
			continue;
		}
		else
		{
			CString FileName = FileFind.GetFileName();

			std::string TextName = "#include \"";
			TextName += CW2A(FileName);
			TextName += "\"\n";

			fwrite(TextName.c_str(), TextName.size(), 1, pFile);
			fwrite(TextName.c_str(), TextName.size(), 1, pFile2);
		}
	}

	fclose(pFile);
	fclose(pFile2);

	FileFind.Close();
}

// CUnityBuildMakerApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CUnityBuildMakerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUnityBuildMakerApp 메시지 처리기



