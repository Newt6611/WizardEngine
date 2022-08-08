#pragma once
#include "wzpch.h"
#include "Renderer.h"
#include "vulkan/vulkan.hpp"

namespace Wizard {
    class VulkanRenderer : public Renderer 
    {
    public:
        VulkanRenderer();
        virtual ~VulkanRenderer();

        inline RendererAPI GetAPI() override {
            return RendererAPI::Vulkan;
        }

        inline std::string GetAPIName() override {
            return "Vulkan";
        }

    private:
        void Init();
        void Shutdown();

        void CreateSurface();
        void CreateInstance();
        std::vector<const char*> GetRequireInstanceExtensions();
        bool CheckValidationSupport();
        
        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);
        void FindQueueFamilies(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        void CreateLogicalDevice();

    private:
#ifdef WZ_DEBUG
        std::vector<const char*> m_ValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
#endif
        std::vector<const char*>  m_DeviceExtensions = {
            "VK_KHR_portability_subset",
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkSurfaceKHR m_Surface;
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;

        struct QueueFamilies {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool IsComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        QueueFamilies m_QueueFamilies;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
    };
}

