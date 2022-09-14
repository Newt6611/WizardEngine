//#pragma once
//#include "wzpch.h"
//#include "Renderer.h"
//#include "vulkan/vulkan.hpp"
//
//namespace Wizard {
//    class VulkanRenderer : public Renderer 
//    {
//    public:
//        VulkanRenderer();
//        virtual ~VulkanRenderer();
//
//        inline RendererAPI GetAPI() override {
//            return RendererAPI::Vulkan;
//        }
//
//        inline std::string GetAPIName() override {
//            return "Vulkan";
//        }
//
//        void DrawFrame();
//
//    public:
//        struct QueueFamilies {
//            std::optional<uint32_t> graphicsFamily;
//            std::optional<uint32_t> presentFamily;
//
//            bool IsComplete() {
//                return graphicsFamily.has_value() && presentFamily.has_value();
//            }
//        };
//
//        struct SwapChainSupportDetails {
//            VkSurfaceCapabilitiesKHR capabilities;
//            std::vector<VkSurfaceFormatKHR> formats;
//            std::vector<VkPresentModeKHR> presentModes;
//        };
//
//    private:
//        void Init();
//        void Shutdown();
//
//        void CreateSurface();
//        void CreateInstance();
//        std::vector<const char*> GetRequireInstanceExtensions();
//        bool CheckValidationSupport();
//        
//        void PickPhysicalDevice();
//        bool IsDeviceSuitable(VkPhysicalDevice device);
//        void FindQueueFamilies(VkPhysicalDevice device);
//        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
//
//        void CreateLogicalDevice();
//
//        void CreateSwapChain();
//        void QuerySwapChainSupport(VkPhysicalDevice device);
//        VkSurfaceFormatKHR ChooseSwapSurfaceFormat();
//        VkPresentModeKHR ChooseSwapPresentMode();
//        VkExtent2D ChooseSwapExtent();
//
//        void CreateImageViews();
//
//        void CreateRenderPass();
//        void CreatePipeline();
//
//        void CreateFramebuffers();
//
//        void CreateCommandPool();
//        void CreateCommandBuffer();
//
//        void RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);
//
//        VkShaderModule CreateShaderModule(std::vector<char>& bytecode);
//    private:
//#ifdef WZ_DEBUG
//        std::vector<const char*> m_ValidationLayers = {
//            "VK_LAYER_KHRONOS_validation"
//        };
//#endif
//
//
//        std::vector<const char*>  m_DeviceExtensions = {
//#ifdef WZ_APPLE
//            "VK_KHR_portability_subset",
//#endif      
//            VK_KHR_SWAPCHAIN_EXTENSION_NAME
//        };
//
//        VkSurfaceKHR m_Surface;
//        VkInstance m_Instance = VK_NULL_HANDLE;
//        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
//        VkDevice m_Device = VK_NULL_HANDLE;
//
//
//        QueueFamilies m_QueueFamilies;
//        VkQueue m_GraphicsQueue;
//        VkQueue m_PresentQueue;
//
//        SwapChainSupportDetails m_SwapChainDetails;
//        VkFormat m_SwapChainFormat;
//        VkExtent2D m_Extent;
//        VkSwapchainKHR m_SwapChain;
//        std::vector<VkImage> m_SwapChainImages;
//        std::vector<VkImageView> m_SwapChainImageViews;
//
//        VkRenderPass m_RenderPass;
//        VkPipelineLayout m_PipelineLayout;
//        VkPipeline m_GraphicsPipeline;
//
//        std::vector<VkFramebuffer> m_Framebuffers;
//
//        VkCommandPool m_CommandPool;
//        VkCommandBuffer m_CommandBuffer;
//    };
//}
//
