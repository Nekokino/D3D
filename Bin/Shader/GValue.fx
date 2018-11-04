cbuffer TransformBuffer : register(b10)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WVP;
}

class TexData
{
    int Tex_Idx;
    int Tex_Smp;
    int Eff_Idx;
    int Eff_Smp;
    int Type;
};

cbuffer RenderOption : register(b11)
{
    int IsLight;
    int IsDefferdOrForward;
    int IsLightVtxOrPix;
    int TexCount;
    TexData TexArr[12];
}