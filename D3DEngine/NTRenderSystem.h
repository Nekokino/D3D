#pragma once
#include <unordered_map>
#include <set>
#include <list>
#include "RefNAutoptr.h"
#include "NTCamera.h"

class NTRenderer;
class NTCamera;
class NTScene;
class NTObject;
class NTRenderSystem
{
public:
	friend NTCamera;
	friend NTRenderer;
	friend NTObject;
	friend NTScene;

private:
	static bool ZOrderSort(Autoptr<NTRenderer> _Left, Autoptr<NTRenderer> _Right);

private:
	std::set<Autoptr<NTCamera>> CameraSet;
	std::set<Autoptr<NTCamera>>::iterator SetStartIter;
	std::set<Autoptr<NTCamera>>::iterator SetEndIter;
	std::set<int>::iterator GroupStartIter;
	std::set<int>::iterator GroupEndIter;

	std::unordered_map<int, std::list<Autoptr<NTRenderer>>> RendererMap;
	std::unordered_map<int, std::list<Autoptr<NTRenderer>>>::iterator GroupFindIter;
	std::unordered_map<int, std::list<Autoptr<NTRenderer>>>::iterator MapStartIter;
	std::unordered_map<int, std::list<Autoptr<NTRenderer>>>::iterator MapEndIter;
	std::list<Autoptr<NTRenderer>>::iterator ListStartIter;
	std::list<Autoptr<NTRenderer>>::iterator ListEndIter;

private:
	void PushCamera(NTCamera* _Cam);
	void PushRenderer(NTRenderer* _Renderer, int _Order);
	void PushOverRenderer(Autoptr<NTRenderer> _Renderer);

public:
	void Render();
	void Release();

	Autoptr<NTRenderer> GetRenderer(const wchar_t* _Name, int _Order);
public:
	NTRenderSystem();
	~NTRenderSystem();
};

