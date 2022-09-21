struct VSInput
{
    float3 Pos   : ATTRIB0;
    float4 Color : ATTRIB1;
    float2 UV    : ATTRIB2;
};

struct PSInput
{
    float4 Pos   : SV_POSITION;
    float4 Color : Color;
    float2 UV    : UV;
};

cbuffer PerFrame
{
    float4x4 ViewProjection;
};

// cbuffer PerObj
// {
//     float4x4 World;
// }

void main(in VSInput VSIn, out PSInput PSIn) 
{
    PSIn.Pos = mul(float4(VSIn.Pos, 1.0), ViewProjection);
    PSIn.Color = VSIn.Color;
    PSIn.UV = VSIn.UV;
}
