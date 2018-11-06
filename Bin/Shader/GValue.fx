cbuffer TransformBuffer : register(b10)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WV;
    matrix WVP;
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
    TexData TexArr[8];
}