#define MAX_TEXTURE 32

Texture2D g_Textures[MAX_TEXTURE];
SamplerState g_Textures_sampler;

struct PSInput 
{ 
    float4 Pos          : SV_POSITION;
    float4 Color        : Color;
    float2 UV           : UV;
    uint TexIndex       : TexIndex;
    int EntityID        : EntityID;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
    float EntityID : SV_TARGET1;
};

void main(in PSInput PSIn, out PSOutput PSOut)
{
    float4 oColor = g_Textures[NonUniformResourceIndex(PSIn.TexIndex)].Sample(g_Textures_sampler, PSIn.UV) * PSIn.Color;
    PSOut.Color = oColor;

    // 0 is clear color
    PSOut.EntityID = PSIn.EntityID + 1;
}