#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>

struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                  FenceValue;
};

class ImGuiD3D12
{
public:
    ImGuiD3D12();
    ~ImGuiD3D12();

    void NewFrame();
    void EndFrame();

private:
    bool Init();
    FrameContext* WaitForNextFrameResources();

private:
    static int const             NUM_FRAMES_IN_FLIGHT = 3;
    static int const             NUM_BACK_BUFFERS = 3;
    FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
    UINT                         g_frameIndex = 0;
    ID3D12Device*                g_pd3dDevice = NULL;
    ID3D12DescriptorHeap*        g_pd3dRtvDescHeap = NULL;
    ID3D12DescriptorHeap*        g_pd3dSrvDescHeap = NULL;
    ID3D12CommandQueue*          g_pd3dCommandQueue = NULL;
    ID3D12GraphicsCommandList*   g_pd3dCommandList = NULL;
    ID3D12Fence*                 g_fence = NULL;
    HANDLE                       g_fenceEvent = NULL;
    UINT64                       g_fenceLastSignaledValue = 0;
    IDXGISwapChain3*             g_pSwapChain = NULL;
    HANDLE                       g_hSwapChainWaitableObject = NULL;
    ID3D12Resource*              g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
    D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

};