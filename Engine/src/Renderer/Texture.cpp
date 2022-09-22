#include "wzpch.h"
#include "Texture.h"

namespace Wizard {
    Texture::Texture(const char* filepath, bool isRGB, IRenderDevice* device)
    {
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB = isRGB;
        TextureData textureData; 
        CreateTextureFromFile(filepath, loadInfo, device, &m_Texture);

        m_TextureView = m_Texture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
    }
}