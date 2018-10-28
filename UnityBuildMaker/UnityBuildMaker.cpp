
// UnityBuildMaker.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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


// CUnityBuildMakerApp ����

CUnityBuildMakerApp::CUnityBuildMakerApp()
{
	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UnityBuildMaker.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CUnityBuildMakerApp ��ü�Դϴ�.

CUnityBuildMakerApp theApp;


// CUnityBuildMakerApp �ʱ�ȭ

BOOL CUnityBuildMakerApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	PathSystem::Init();

	CreateCpp((PathSystem::Root + L"D3DEngine\\").c_str(), L"D3DEngineUnity.cpp");
	CreateCpp((PathSystem::Root + L"D3DClient\\").c_str(), L"D3DClientUnity.cpp");
	CreateCpp((PathSystem::Root + L"D3DMaker\\").c_str(), L"D3DMakerUnity.cpp");
	CreateCpp((PathSystem::Root + L"D3DTool\\").c_str(), L"D3DToolUnity.cpp");

	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	return TRUE;
}

int CUnityBuildMakerApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
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

// CUnityBuildMakerApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CUnityBuildMakerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUnityBuildMakerApp �޽��� ó����



