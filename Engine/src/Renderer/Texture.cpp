#include "wzpch.h"
#include "Texture.h"

namespace Wizard {
    Texture::Texture(const char* filepath, bool isRGB, IRenderDevice* device)
    {
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB = isRGB;
        RefCntAutoPtr<ITexture> Tex;
        TextureData textureData; 
        CreateTextureFromFile(filepath, loadInfo, device, &Tex);

        m_TextureView = Tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
    }
}