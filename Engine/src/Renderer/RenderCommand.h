#pragma once
#include "Renderer.h"


namespace Wizard {
    class RenderCommand
    {
    public:
        static void ClearRenderTarget(ITextureView *pView, const float *RGBA)
        {
            Renderer::Get()->GetDeviceContext()->ClearRenderTarget(pView, RGBA, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        }

        static void ClearDepthStencil(ITextureView *pView, CLEAR_DEPTH_STENCIL_FLAGS ClearFlags, 
            float fDepth, Uint8 Stencil)
        {
            Renderer::Get()->GetDeviceContext()->ClearDepthStencil(pView, ClearFlags, fDepth, Stencil, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        }

        static void SetRenderTarget(uint32_t numberTarget, ITextureView* renderTargets[], ITextureView* depthStencil)
        {
            Renderer::Get()->GetDeviceContext()->SetRenderTargets(numberTarget, renderTargets, depthStencil, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        }

        static void SetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffersSet, 
            IBuffer **ppBuffers, const uint64_t *pOffsets, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode, 
                SET_VERTEX_BUFFERS_FLAGS Flags)
        {
            Renderer::Get()->GetDeviceContext()->SetVertexBuffers(StartSlot, NumBuffersSet, ppBuffers, pOffsets, StateTransitionMode, Flags);
        }

        static void SetIndexBuffer(IBuffer *pIndexBuffer, uint64_t ByteOffset, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode)
        {
            Renderer::Get()->GetDeviceContext()->SetIndexBuffer(pIndexBuffer, ByteOffset, StateTransitionMode);
        }

        static void SetPipelineState(IPipelineState* state)
        {
            Renderer::Get()->GetDeviceContext()->SetPipelineState(state);
        }

        static void CommitShaderResources(IShaderResourceBinding *pShaderResourceBinding)
        {
            Renderer::Get()->GetDeviceContext()->CommitShaderResources(pShaderResourceBinding, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        }

        static void DrawIndexed(const Diligent::DrawIndexedAttribs &Attribs)
        {
            Renderer::Get()->GetDeviceContext()->DrawIndexed(Attribs);
        }

    };
}