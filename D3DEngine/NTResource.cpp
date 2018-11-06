#include "PreCom.h"
#include "NTResource.h"


NTResource::NTResource()
{
}

NTResource::NTResource(const NTResource & _Other) : NTNameTag(_Other), NTTypeParent(_Other), RefCounter(_Other), Path(_Other.Path), Pathkey(_Other.Pathkey), FullFileName(_Other.FullFileName), FileName(_Other.FileName), Extension(_Other.Extension)
{

}


NTResource::~NTResource()
{
}

bool NTResource::Create()
{
	return false;
}

bool NTResource::Load()
{
	return false;
}

void NTResource::SetPath(const wchar_t* _Path)
{
	wchar_t ArrDrive[128];
	wchar_t ArrFolder[128];
	wchar_t ArrFile[128];
	wchar_t ArrExtension[128];

	_wsplitpath_s(_Path, ArrDrive, ArrFolder, ArrFile, ArrExtension);

	Path = _Path;
	FileName = ArrFile;
	Extension = ArrExtension;
	FullFileName = FileName + Extension;
}