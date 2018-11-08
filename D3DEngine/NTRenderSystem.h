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
	std::map<int, Autoptr<NTCamera>> CameraMap;
	std::map<int, Autoptr<NTCamera>>::iterator CameraMapStartIter;
	std::map<int, Autoptr<NTCamera>>::iterator CameraMapEndIter;

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
	void LightCheck(Autoptr<NTCamera> _Camera, int _Group);
	void Render_Defferd_Light(Autoptr<NTCamera> _Cam, int _Group);

	/////////////// Light End

private:
	void ResetSampler();

public:
	void Render();

	void Render_Forward(Autoptr<NTCamera> _Camera, std::map<int, std::list<Autoptr<NTRenderer>>>::iterator _Iter, size_t _Index);
	void Render_Defferd(Autoptr<NTCamera> _Camera, std::map<int, std::list<Autoptr<NTRenderer>>>::iterator _Iter, size_t _Index);
	void MergeScreen();

	void Release();

	Autoptr<NTRenderer> GetRenderer(const wchar_t* _Name, int _Order);
public:
	NTRenderSystem();
	~NTRenderSystem();
};

