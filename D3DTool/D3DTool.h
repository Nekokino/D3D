
// D3DTool.h : D3DTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CD3DToolApp:
// �� Ŭ������ ������ ���ؼ��� D3DTool.cpp�� �����Ͻʽÿ�.
//

class CD3DToolApp : public CWinApp
{
public:
	CD3DToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	virtual BOOL OnIdle(LONG lCount);
};

extern CD3DToolApp theApp;
