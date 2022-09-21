Texture2D    g_Texture;
SamplerState g_Texture_sampler;

struct PSInput
{ 
    float4 Pos : SV_POSITION;
    float2 UV  : UV;
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
    PSOut.Color = g_Texture.Sample(g_Texture_sampler, PSIn.UV) * g_Color;
}