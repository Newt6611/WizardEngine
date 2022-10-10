#include "ImGuiD3D12.h"
#include "Wizard.h"
#include "../../Engine/thirdparty/DiligentCore/Graphics/GraphicsEngineD3D12/interface/RenderDeviceD3D12.h"
#include "../../Engine/thirdparty/DiligentCore/Graphics/GraphicsEngineD3D12/interface/DeviceContextD3D12.h"
#include "../../Engine/thirdparty/DiligentCore/Graphics/GraphicsEngineD3D12/interface/SwapChainD3D12.h"
#include "../../Engine/thirdparty/DiligentCore/Graphics/GraphicsEngineD3D12/interface/BufferD3D12.h"
#include "../../Engine/thirdparty/DiligentCore/Graphics/GraphicsEngineD3D12/interface/BufferViewD3D12.h"
using namespace Wizard;

ImGuiD3D12::ImGuiD3D12()
{
    Init();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplDX12_Init(g_pd3dDevice, NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap,
        g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
}

ImGuiD3D12::~ImGuiD3D12()
{

}

void ImGuiD3D12::NewFrame()
{
    ImGui_ImplDX12_NewFrame();
}

void ImGuiD3D12::EndFrame()
{
    FrameContext* frameCtx = WaitForNextFrameResources();
    UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
    frameCtx->CommandAllocator->Reset();
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource   = g_mainRenderTargetResource[backBufferIdx];
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
    g_pd3dCommandList->Reset(frameCtx->CommandAllocator, NULL);
    g_pd3dCommandList->ResourceBarrier(1, &barrier);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    // g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, NULL);
    // g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, NULL);
    // g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
    // barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    // barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
    // g_pd3dCommandList->ResourceBarrier(1, &barrier);
    // g_pd3dCommandList->Close();

    g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);
}

bool ImGuiD3D12::Init()
{
    // Get device from Diligent
    RefCntAutoPtr<IRenderDeviceD3D12> pDeviceD3D12(RenderTool::GetDevice(), IID_RenderDeviceD3D12);
    g_pd3dDevice = pDeviceD3D12->GetD3D12Device();

    // Get commandlist from Diligent
    RefCntAutoPtr<IDeviceContextD3D12> pDeviceContextD3D12(RenderTool::GetDeviceContext(), IID_DeviceContextD3D12);
    g_pd3dCommandList = pDeviceContextD3D12->GetD3D12CommandList();
    
    // Get swapchain from Dilitgent
    RefCntAutoPtr<ISwapChainD3D12> pSwapChainD3D12(RenderTool::GetSwapChain(), IID_SwapChainD3D12);
    pSwapChainD3D12->GetDXGISwapChain()->QueryInterface(IID_PPV_ARGS(&g_pSwapChain));

    // Get Render Target View Handle
    RefCntAutoPtr<ITextureViewD3D12> pRTVView(RenderTool::GetSwapChain()->GetCurrentBackBufferRTV(), IID_TextureViewD3D12);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = pRTVView->GetCPUDescriptorHandle();
    
    {
        // D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        // desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        // desc.NumDescriptors = NUM_BACK_BUFFERS;
        // desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        // desc.NodeMask = 1;
        // if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK) {
        //     WZ_ERROR("Failed When Creating DescriptorHeap");
        //     return false;
        // }

        SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        //D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        {
            g_mainRenderTargetDescriptor[i] = rtvHandle;
            rtvHandle.ptr += rtvDescriptorSize;
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK) {
            WZ_ERROR("Failed When Creatign DescriptorHeap2");
            return false;
        }
    }

    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK) {
            WZ_ERROR("Failed When Creatign Command Queue");
            return false;
        }
    }

    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
            return false;

    if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, NULL, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
        g_pd3dCommandList->Close() != S_OK)
        return false;

    if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
        return false;

    g_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (g_fenceEvent == NULL)
        return false;

    return true;
}

FrameContext* ImGuiD3D12::WaitForNextFrameResources()
{
    UINT nextFrameIndex = g_frameIndex + 1;
    g_frameIndex = nextFrameIndex;

    HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, NULL };
    DWORD numWaitableObjects = 1;

    FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue != 0) // means no fence was signaled
    {
        frameCtx->FenceValue = 0;
        g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
        waitableObjects[1] = g_fenceEvent;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return frameCtx;
}