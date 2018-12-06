#pragma once
#include <WinCore.h>

class CoreStarter : public WinCore::WinCoreBuilder
{
public:
	void Build() override;

public:
	CoreStarter();
	~CoreStarter();
};

COREBEGIN(CoreStarter, L"MainWindow");