#pragma once

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/Texture.h"
#include "Graphics/GraphicsEngine/interface/TextureView.h"

#include "TextureLoader/interface/TextureLoader.h"
#include "TextureLoader/interface/TextureUtilities.h"

#include "Common/interface/RefCntAutoPtr.hpp"

namespace Wizard {
    using namespace Diligent;

    class Texture2D
    {
    public:
        // Todo: Create assets handler each asset has their own uuid 
        static int CurrentTextureID;

    public:
        Texture2D(const char* filepath, bool isRGB, IRenderDevice* device);
        Texture2D(uint32_t width, uint32_t height, const void* data, IRenderDevice* device, IDeviceContext* deviceContext);
        ~Texture2D() = default;

        inline ITextureView* GetTextureView() { return m_TextureView; }
        inline int GetTextureID() { return m_TextureID; }
    private:
        int m_TextureID = 0;
        RefCntAutoPtr<ITextureView> m_TextureView;
    };
}