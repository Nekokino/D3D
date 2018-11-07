#include "Light.fx"

cbuffer LightBuffer : register(b12)
{
    LightData ArrLight[10];
    int LightCount;
    int Dummy1;
    int Dummy2;
    int Dummy3;
}