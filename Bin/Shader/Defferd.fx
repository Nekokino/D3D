struct PS_Out
{
    float4 Diffuse : SV_Target; // 색
    float4 Position : SV_Target1; // 위치
    float4 Normal : SV_Target2; // 노말
    float4 Depth : SV_Target3; // 깊이
};