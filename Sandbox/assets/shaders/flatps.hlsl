struct PSInput 
{ 
    float4 Pos : SV_POSITION; 
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

cbuffer PSObjConst
{
    float4 g_Color;
};


void main(in PSInput PSIn, out PSOutput PSOut)
{
    PSOut.Color = g_Color;
}