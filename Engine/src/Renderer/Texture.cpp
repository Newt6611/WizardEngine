#include "wzpch.h"
#include "Texture.h"

namespace Wizard {
    int Texture2D::CurrentTextureID = 0;

    Texture2D::Texture2D(const char* filepath, bool isRGB, IRenderDevice* device)
    {
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB = isRGB;

        RefCntAutoPtr<ITexture> tex;
        CreateTextureFromFile(filepath, loadInfo, device, &tex);

        m_TextureView = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
        m_TextureID = Texture2D::CurrentTextureID++;
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height, const void* data, IRenderDevice* device, IDeviceContext* deviceContext)
    {
        TextureDesc desc;
        desc.ArraySize = 1;
        desc.BindFlags = BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = CPU_ACCESS_WRITE;
        desc.Usage = USAGE_DYNAMIC;
        desc.Format = TEX_FORMAT_RGBA8_UNORM;
        desc.SampleCount = 1;
        desc.MipLevels = 1;
        desc.Width = width;
        desc.Height = height;
        desc.Type = RESOURCE_DIM_TEX_2D;

        uint32_t color = (uint32_t)data;
        uint32_t stride = desc.Width * 4;
        TextureSubResData subResData[] {{&color, stride}};
        
        TextureData textureData;
        textureData.pContext = deviceContext;
        textureData.pSubResources = subResData;
        textureData.NumSubresources = _countof(subResData);
        
        RefCntAutoPtr<ITexture> tex;
        device->CreateTexture(desc, &textureData, &tex);
        m_TextureView = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
        m_TextureID = Texture2D::CurrentTextureID++;
    }
}