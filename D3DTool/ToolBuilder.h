#pragma once
#include <WinCore.h>

class ToolBuilder : public WinCore::WinCoreBuilder
{
public:
	void Build() override;
public:
	ToolBuilder();
	~ToolBuilder();
};

