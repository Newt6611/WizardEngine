#include "VulkanRenderer.h"
#include "Core/Log.h"
#include <GLFW/glfw3.h>
#include "Core/Window.h"

namespace Wizard {
    
    VulkanRenderer::VulkanRenderer()
    {
        Init();
    }

    VulkanRenderer::~VulkanRenderer()
    {
        Shutdown();
    }

    void VulkanRenderer::Init()
    {
        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
    }

    void VulkanRenderer::Shutdown()
    {
        vkDestroyDevice(m_Device, nullptr);
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }

    void VulkanRenderer::CreateSurface()
    {
        if (glfwCreateWindowSurface(m_Instance, Window::Get()->GetWindow(), nullptr, &m_Surface) != VK_SUCCESS) {
            WZ_ENGINE_ERROR("Failed To Create Window Surface");
            return;
        }
    }

    void VulkanRenderer::CreateInstance()
    {
#ifdef WZ_DEBUG
        if (!CheckValidationSupport()) {
            WZ_ENGINE_ERROR("Vulkan Validation Requested But Not Found");
        }
#endif

        VkApplicationInfo appInfo {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Wizard";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Winzard Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;

#ifdef WZ_APPLE
        instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        std::vector<const char*> extensions = GetRequireInstanceExtensions();

        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

#ifdef WZ_DEBUG
        instanceCreateInfo.enabledLayerCount = m_ValidationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
#else
        instanceCreateInfo.enabledLayerCount = 0;
#endif
        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);
        if (result != VK_SUCCESS) {
            WZ_ENGINE_ERROR("Failed When Createing Vulkan Instance Error Code: {}", result);
            return;
        }
    }

    std::vector<const char*> VulkanRenderer::GetRequireInstanceExtensions()
    {
        uint32_t extensionCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + extensionCount);

#ifdef WZ_APPLE
        extensions.emplace_back("VK_KHR_portability_enumeration");
#endif

        return extensions;
    }

    bool VulkanRenderer::CheckValidationSupport()
    {
#ifdef WZ_DEBUG
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availabalLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availabalLayers.data());
        
        for (const char* name : m_ValidationLayers) {
            bool layerFound = false;
            for (const auto& layerProperties : availabalLayers) {
                if (strcmp(name, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
                return false;
        }
#endif
        return true;
    }

    void VulkanRenderer::PickPhysicalDevice()
    {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            WZ_ENGINE_ERROR("Can't Find GPUs Support");
            return;
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        for (int i = 0; i < devices.size(); ++i) {
            if (IsDeviceSuitable(devices[i])) {
                m_PhysicalDevice = devices[i];
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE) {
            WZ_ENGINE_ERROR("Can't Find Suitable PhysicalDevice");
            return;
        }
    }

    bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
    {
        FindQueueFamilies(device);
        bool deviceExtensionSupport = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (deviceExtensionSupport) {
            QuerySwapChainSupport(device);
            swapChainAdequate = !m_SwapChainDetails.formats.empty() && !m_SwapChainDetails.presentModes.empty();
        }
        return m_QueueFamilies.IsComplete() && deviceExtensionSupport && swapChainAdequate;
    }

    void VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device) 
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilies.size(); ++i) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_QueueFamilies.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
            if (presentSupport) {
                m_QueueFamilies.presentFamily = i;
            }

            if (m_QueueFamilies.IsComplete()) {
                break;
            }
        }
    }

    bool VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
        
        // TODO: could be a better code to optmize
        std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void VulkanRenderer::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos { };
        std::set<uint32_t> uniqueQueueFamilies = {
            m_QueueFamilies.graphicsFamily.value(),
            m_QueueFamilies.presentFamily.value()
        };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures { };

        VkDeviceCreateInfo createInfo { };
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = m_DeviceExtensions.size();
        createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

        VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device);
        if (result != VK_SUCCESS) {
            WZ_ENGINE_ERROR("Failed To Create Logical Device Error Code: {}", result);
            return;
        }

        vkGetDeviceQueue(m_Device, m_QueueFamilies.graphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, m_QueueFamilies.presentFamily.value(), 0, &m_PresentQueue);
    }

    void VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &m_SwapChainDetails.capabilities);
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

        if (formatCount != 0) {
            m_SwapChainDetails.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, m_SwapChainDetails.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            m_SwapChainDetails.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, m_SwapChainDetails.presentModes.data());
        }
    }

    VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat()
    {
        for (const auto& availableFormat : m_SwapChainDetails.formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return m_SwapChainDetails.formats[0];
    }

    VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode()
    {
        for (const auto& availablePresentMode : m_SwapChainDetails.presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
}