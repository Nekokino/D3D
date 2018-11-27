#include "PreCom.h"
#include "NTSphere.h"
#include "NTMesh.h"
#include "DirectValueHeader.h"

NTSphere::NTSphere()
{
	Sphere = CreateCollisionFigure<NTSphereData>(COLTYPE::CT_3D_SPHERE);
}


NTSphere::~NTSphere()
{
	if (Sphere != nullptr)
	{
		delete Sphere;
	}
}

void NTSphere::CollisionFigureUpdate()
{
	Sphere->Sphere.Center = GetTransform()->GetWorldMatrix().v4.DVec3;
	Sphere->Sphere.Radius = GetTransform()->GetWorldMatrix().v1.x * 0.5f;
}

void NTSphere::DbgRender()
{

}