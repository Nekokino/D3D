#pragma once
#include <map>
#include <set>
#include <list>
#include "RefNAutoptr.h"
#include "NTCamera.h"

class NTRenderer;
class NTCamera;
class NTScene;
class NTObject;
class NTLight;
class NTRenderSystem
{
public:
	friend NTCamera;
	friend NTRenderer;
	friend NTObject;
	friend NTScene;
	friend class DebugFunc;

private:
	static bool ZOrderSort(Autoptr<NTRenderer> _Left, Autoptr<NTRenderer> _Right);

private:
	std::set<Autoptr<NTCamera>> CameraSet;
	std::set<Autoptr<NTCamera>>::iterator SetStartIter;
	std::set<Autoptr<NTCamera>>::iterator SetEndIter;

	std::map<int, std::list<Autoptr<NTRenderer>>> RendererMap;
	std::map<int, std::list<Autoptr<NTRenderer>>>::iterator GroupFindIter;
	std::map<int, std::list<Autoptr<NTRenderer>>>::iterator MapStartIter;
	std::map<int, std::list<Autoptr<NTRenderer>>>::iterator MapEndIter;
	std::list<Autoptr<NTRenderer>>::iterator ListStartIter;
	std::list<Autoptr<NTRenderer>>::iterator ListEndIter;

private:
	void PushCamera(NTCamera* _Cam);
	void PushRenderer(NTRenderer* _Renderer);
	void PushOverRenderer(Autoptr<NTRenderer> _Renderer);

	////////////// Light
public:
	std::set<Autoptr<NTLight>> LightSet;
	std::set<Autoptr<NTLight>>::iterator LightStartIter;
	std::set<Autoptr<NTLight>>::iterator LightEndIter;

	void PushLight(NTLight* _Camera);
	void LightCheck(const std::set<Autoptr<NTCamera>>::iterator& _CamIter, int _Group);
	void Render_Defferd_Light(const std::set<Autoptr<NTCamera>>::iterator& _CamIter, int _Group);

	/////////////// Light End

private:
	void ResetSampler();

public:
	void Render();

	void Render_Forward(std::map<int, std::list<Autoptr<NTRenderer>>>::iterator _Iter, size_t _Index);
	void Render_Defferd(std::map<int, std::list<Autoptr<NTRenderer>>>::iterator _Iter, size_t _Index);

	void Release();

	Autoptr<NTRenderer> GetRenderer(const wchar_t* _Name, int _Order);
public:
	NTRenderSystem();
	~NTRenderSystem();
};

