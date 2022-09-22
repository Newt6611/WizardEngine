struct PSInput 
{ 
    float4 Pos : SV_POSITION;
    float4 Color : Color;
    float2 UV    : UV;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

// cbuffer PSObjConst
// {
//     float4 g_Color;
// };

void main(in PSInput PSIn, out PSOutput PSOut)
{
    PSOut.Color = PSIn.Color;
}