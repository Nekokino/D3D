#pragma once
#include <NTScene.h>
#include "MenuDlg.h"

class DlgShortCut
{
public:
	static Autoptr<NTScene> SelectScene;
	static MenuDlg* pMenuDlg;
public:
	DlgShortCut();
	~DlgShortCut();
};

