cbuffer TransformBuffer : register(b10)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WV;
    matrix VP;
    matrix WVP;
    float4 CameraPos;
}

class TexData
{
    int Type;
    int Tex_Idx;
    int Tex_Smp;
    int Dummy;
};

cbuffer RenderOption : register(b11)
{
    int IsLight;
    int IsDefferdOrForward;
    int IsLightVtxOrPix;
    int TexCount;
    int IsAni;
    int Dummy01;
    int Dummy02;
    int Dummy03;
    TexData TexArr[8];
}