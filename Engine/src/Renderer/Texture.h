#pragma once

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/Texture.h"
#include "Graphics/GraphicsEngine/interface/TextureView.h"

#include "TextureLoader/interface/TextureLoader.h"
#include "TextureLoader/interface/TextureUtilities.h"

#include "Common/interface/RefCntAutoPtr.hpp"

namespace Wizard {
    using namespace Diligent;

    class Texture 
    {
    public:
        Texture(const char* filepath, bool isRGB, IRenderDevice* device);
        ~Texture() = default;

        inline ITexture* GetTexture() { return m_Texture; }
        inline const ITextureView* GetTextureView() { return m_TextureView; }
        inline const TextureDesc& GetTextureDesc() { return m_Texture->GetDesc(); }
    private:
        RefCntAutoPtr<ITexture> m_Texture;
        RefCntAutoPtr<ITextureView> m_TextureView;
    };
}