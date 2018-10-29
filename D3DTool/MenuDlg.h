#pragma once
#include "TabDlgBase.h"
#include "afxcmn.h"
#include <NTWinShortCut.h>
// MenuDlg 대화 상자입니다.

class MenuDlg : public TabDlgBase
{
	DECLARE_DYNAMIC(MenuDlg)

public:
	MenuDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MenuDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);

	CTabCtrl MenuTab;
	size_t CurIndex;

private:
	std::vector<TabDlgBase*> DlgVec;

	template<typename T>
	void CreateDlg(UINT _DlgID, CString _ItemName, bool _IsSceneCreate = true)
	{
		UpdateData(TRUE);

		RECT rc;
		MenuTab.GetClientRect(&rc);
		MenuTab.InsertItem((int)DlgVec.size(), _ItemName);

		if (true == _IsSceneCreate)
		{
			Autoptr<NTScene> Scene = NTWinShortCut::GetMainSceneSystem().FindScene(_ItemName.GetString());

			if (nullptr == Scene)
			{
				Scene = NTWinShortCut::GetMainSceneSystem().CreateScene(_ItemName.GetString());
				Autoptr<NTObject> Camera = Scene->CreateObject(L"MainCamera", 0);
				Camera->GetTransform()->SetLocalPosition(NTVEC(0.0f, 0.0f, -10.0f));
				Autoptr<NTCamera> CamCom = Camera->AddComponent<NTCamera>();
				CamCom->PushRenderLayer(0, 1, 2, 3, 4);
			}
		}

		T* Dlg = new T();

		Dlg->SceneName = _ItemName;
		Dlg->Create(_DlgID, this);
		Dlg->SetBackgroundColor(RGB(255, 255, 255), TRUE);
		Dlg->SetWindowPos(nullptr, 36, 18, rc.right - 36, rc.bottom - 18, SWP_NOZORDER);
		Dlg->ShowWindow(SW_HIDE);
		Dlg->SceneChange = _IsSceneCreate;

		DlgVec.push_back(Dlg);

		UpdateData(FALSE);
	}

	void ShowDlg(size_t _Index);
};
