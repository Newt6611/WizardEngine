struct VSInput
{
    float3 Pos : ATTRIB0;
    float2 UV  : ATTRIB1;
};

struct PSInput 
{ 
    float4 Pos : SV_POSITION;
    float2 UV  : UV;
};

cbuffer PerFrame
{
    float4x4 View;
    float4x4 Projection;
};

cbuffer PerObj
{
    float4x4 World;
}

void main(in VSInput VSIn, out PSInput PSIn) 
{
    PSIn.Pos = mul(float4(VSIn.Pos, 1.0), World);
    PSIn.Pos = mul(PSIn.Pos, View);
    PSIn.Pos = mul(PSIn.Pos, Projection);
    PSIn.UV = VSIn.UV;
}
