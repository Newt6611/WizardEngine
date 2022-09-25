struct VSInput
{
    float3 Pos      : ATTRIB0;
    float4 Color    : ATTRIB1;
    float2 UV       : ATTRIB2;
    uint TexIndex   : ATTRIB3;
};

struct PSInput
{
    float4 Pos      : SV_POSITION;
    float4 Color    : Color;
    float2 UV       : UV;
    uint TexIndex   : TexIndex;
};

cbuffer PerFrame
{
    float4x4 ViewProjection;
};


void main(in VSInput VSIn, out PSInput PSIn) 
{
    PSIn.Pos = mul(float4(VSIn.Pos, 1.0), ViewProjection);
    PSIn.Color = VSIn.Color;
    PSIn.UV = VSIn.UV;
    PSIn.TexIndex = VSIn.TexIndex;
}
