#pragma once
#define NOMINMAX
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <set>
#include <algorithm>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <array>
#include <unordered_map>
#include "Scene.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

const int WIDTH = 1280;
const int HEIGHT = 720;
const int SHADOWMAP_WIDTH = 2048;
const int SHADOWMAP_HEIGHT = 2048;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct CameraBufferObject {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec3 pos;
};

struct ShadowsBufferObject {
	alignas(16) glm::mat4 lightSpace;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class GraphicsEngine {
public:
	void setScene(Scene* newScene);
	int start();
private:
	void run();
	void frameEvents(GLFWwindow* window);

	static void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
		auto app = reinterpret_cast<GraphicsEngine*>(glfwGetWindowUserPointer(window));

		// First time we move the mouse
		if (app->firstMouse) {
			app->xMouseLast = xPos;
			app->yMouseLast = yPos;
			app->firstMouse = false;
		}

		double xOffset = (xPos - app->xMouseLast) * app->sensitivity;
		double yOffset = (yPos - app->yMouseLast) * app->sensitivity;
		app->xMouseLast = (float)xPos;
		app->yMouseLast = (float)yPos;
		app->yaw = glm::mod(app->yaw + (float)xOffset, 360.0f);
		app->pitch += (float)yOffset;

		if (app->pitch > 89.0f) {
			app->pitch = 89.0f;
		}
		if (app->pitch < -89.0f) {
			app->pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(app->pitch)) * cos(glm::radians(app->yaw));
		front.y = sin(glm::radians(app->pitch)) * -1;
		front.z = cos(glm::radians(app->pitch)) * sin(glm::radians(app->yaw));
		Camera* camera = app->scene->getCamera();
		glm::vec3 frontN = glm::normalize(front);
		camera->setFront(frontN.x, frontN.y, frontN.z);
	}

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void initWindow();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<GraphicsEngine*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void initVulkan();
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void recreateSwapChain();
	void cleanupSwapChain();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createColorResources();
	void createDepthResources();
	void createTextures();
	void createModels();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void updateUniformBuffer(Object* obj, uint32_t currentImage);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	VkSampleCountFlagBits getMaxUsableSampleCount();
	void createSkybox();
	void createTextureImage(Object* obj);
	void createTextureImageView(Object* obj);
	void createTextureSampler(Object* obj);
	void loadModel(Object* obj);
	void createVertexBuffer(Object* obj);
	void createIndexBuffer(Object* obj);
	void updateDescriptorSets(Object* obj, int frame, int nbDesc);
	void updateShadowsDescriptorSets(Object* obj, int frame, int nbDesc);
	void mainLoop();
	void drawFrame();
	void cleanup();

	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	VkSampler shadowsSampler;
	VkRenderPass renderPass;
	VkRenderPass shadowsRenderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayout shadowsDescriptorSetLayout;
	VkPipeline graphicsPipeline;
	VkPipeline shadowsGraphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkPipelineLayout shadowsPipelineLayout;
	VkPipeline skyboxGraphicsPipeline;
	VkPipelineLayout skyboxPipelineLayout;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkFramebuffer> shadowsFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;
	bool framebufferResized = false;
	VkDescriptorPool descriptorPool;
	VkDescriptorPool shadowsDescriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> shadowsDescriptorSets;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	VkImage shadowsImage;
	VkDeviceMemory shadowsImageMemory;
	VkImageView shadowsImageView;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<VkBuffer> cameraBuffers;
	std::vector<VkDeviceMemory> cameraBuffersMemory;
	std::vector<VkBuffer> lightsBuffers;
	std::vector <VkDeviceMemory> lightsBuffersMemory;
	std::vector<VkBuffer> shadowsBuffers;
	std::vector<VkDeviceMemory> shadowsBuffersMemory;

	// Game scene
	Scene* scene;

	// Inputs
	void (*inputs)(GLFWwindow* window);

	// Camera
	float savedZAxis = 0.3f;

	// Movement speed
	double lastFrame = 0.0f;

	// Mouse movement
	float firstMouse = true;
	float sensitivity = 0.05f;
	double xMouseLast = WIDTH / 2.f;
	double yMouseLast = HEIGHT / 2.f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};
