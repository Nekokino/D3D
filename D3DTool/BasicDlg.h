#pragma once
#include "TabDlgBase.h"

// BasicDlg ��ȭ �����Դϴ�.

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
};
