#pragma once
#include "TabDlgBase.h"
#include "afxcmn.h"
#include <MoveTest.h>

// BasicDlg ��ȭ �����Դϴ�.

class NTCollisionComponent;
class NTBone;
class BasicDlg : public TabDlgBase
{
	DECLARE_DYNAMIC(BasicDlg)

public:
	BasicDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BasicDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASICDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedCreate();

	void ColTest(NTCollisionComponent* _Left, NTCollisionComponent* _Right);
	CTreeCtrl BoneTree;
	afx_msg void OnTvnSelchangedBonetree(NMHDR *pNMHDR, LRESULT *pResult);

	Autoptr<MoveTest> MT;

	HTREEITEM FindTreeItem(const CString& _Name, CTreeCtrl& _Tree, HTREEITEM _Root)
	{
		CString Text = _Tree.GetItemText(_Root);
		if (Text.Compare(_Name) == 0)
		{
			return _Root;
		}

		HTREEITEM Sub = _Tree.GetChildItem(_Root);

		while (Sub)
		{
			HTREEITEM Find = FindTreeItem(_Name, _Tree, Sub);
			if (Find)
			{
				return Find;
			}

			Sub = _Tree.GetNextSiblingItem(Sub);
		}

		return nullptr;
	}
};
