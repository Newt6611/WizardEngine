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

        inline ITextureView* GetTextureView() { return m_TextureView; }
    private:
        RefCntAutoPtr<ITextureView> m_TextureView;
    };
}