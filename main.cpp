#include <iostream>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

class Game {
private:
    GLFWwindow* m_window;
    vk::Instance m_instance;
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_device;
    vk::Queue m_queue;
    vk::SurfaceKHR m_surface;
    vk::SwapchainKHR m_swapChain;
    std::vector<vk::Image> m_swapChainImages;
    vk::Format m_swapChainImageFormat;
    vk::Extent2D m_swapChainExtent;
    std::vector<vk::ImageView> m_swapChainImageViews;
    void createWindow(){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_window = glfwCreateWindow(800, 600, "Vulkan Block Game", nullptr, nullptr);
    }
    void createInstance() {
        vk::ApplicationInfo appInfo("Vulkan Block Game", VK_MAKE_VERSION(1, 0, 0), "No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_2);
        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;
        std::vector<const char*> extensions = {VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME};
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back(glfwExtensions[i]);
        }
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
        m_instance = vk::createInstance(createInfo);
    }
    vk::PhysicalDevice getPhysicalDeviceOfType(vk::PhysicalDeviceType type) {
        std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
        for(auto& physicalDevice : physicalDevices){
            vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
            if(properties.deviceType == type){
                return physicalDevice;
            }
        }
        return VK_NULL_HANDLE;
    }
    void pickPhysicalDevice() {
        std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
        m_physicalDevice = getPhysicalDeviceOfType(vk::PhysicalDeviceType::eDiscreteGpu);
        if(m_physicalDevice==VK_NULL_HANDLE){
            m_physicalDevice = getPhysicalDeviceOfType(vk::PhysicalDeviceType::eIntegratedGpu);
        }
        if(m_physicalDevice==VK_NULL_HANDLE){
            m_physicalDevice = getPhysicalDeviceOfType(vk::PhysicalDeviceType::eCpu);
        }
        if(m_physicalDevice==VK_NULL_HANDLE){
            throw std::runtime_error("No physical devices found");
        }
    }
    void createLogicalDevice(){
        float queuePriority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = 0;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        vk::PhysicalDeviceFeatures deviceFeatures;
        vk::DeviceCreateInfo createInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.pEnabledFeatures = &deviceFeatures;
        std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        std::vector<vk::ExtensionProperties> availableExtensions = m_physicalDevice.enumerateDeviceExtensionProperties();
        std::string portabilityExtension = "VK_KHR_portability_subset";
        for(vk::ExtensionProperties extension : availableExtensions){
            if(std::string(extension.extensionName) == portabilityExtension){
                deviceExtensions.push_back(portabilityExtension.c_str());
            }
        }
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        m_device = m_physicalDevice.createDevice(createInfo);
        m_queue = m_device.getQueue(0, 0);
    }
    void createSurface(){
        VkSurfaceKHR surface;
        if(glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS){
            throw std::runtime_error("Failed to create window surface");
        }
        m_surface = surface;
    }
    void cleanupResources() {
        m_instance.destroySurfaceKHR(m_surface);
        m_device.destroy();
        m_instance.destroy();
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
public:
    Game(){
        createWindow();
        createInstance();
        pickPhysicalDevice();
        createLogicalDevice();
        createSurface();
    }
    ~Game(){
        cleanupResources();
    }
    void run() {
        //while (true) {
        //}
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
