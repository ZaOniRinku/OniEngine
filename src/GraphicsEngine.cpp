#include "GraphicsEngine.h"
#include <stb_image.h>
#include <tiny_obj_loader.h>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
		"vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
		"vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void GraphicsEngine::setScene(Scene* newScene) {
	scene = newScene;
}

void GraphicsEngine::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void GraphicsEngine::frameEvents(GLFWwindow* window) {
	// Time management
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastFrame;

	// Frame Events for directional lights
	for (DirectionalLight* dirLight : scene->getDirectionalLights()) {
		if (dirLight->frameEvent) {
			dirLight->frameEvent(dirLight, window, deltaTime);
		}
	}

	// Frame Events for point lights
	for (PointLight* pointLight : scene->getPointLights()) {
		if (pointLight->frameEvent) {
			pointLight->frameEvent(pointLight, window, deltaTime);
		}
	}

	// Frame Events for spot lights
	for (SpotLight* spotLight : scene->getSpotLights()) {
		if (spotLight->frameEvent) {
			spotLight->frameEvent(spotLight, window, deltaTime);
		}
		if (spotLight->isTorchlight()) {
			spotLight->setPosition(scene->getCamera()->getPositionX(), scene->getCamera()->getPositionY(), scene->getCamera()->getPositionZ());
			spotLight->setDirection(scene->getCamera()->getFrontX(), scene->getCamera()->getFrontY(), scene->getCamera()->getFrontZ());
		}
	}

	// Frame Events for camera
	Camera* camera = scene->getCamera();
	if (camera->frameEvent) {
		camera->frameEvent(camera, window, deltaTime);
	}

	// Frame Events for skybox
	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		skybox->setPosition(camera->getPositionX(), camera->getPositionY(), camera->getPositionZ());
	}

	// Frame Events per object
	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Object *obj = elements.front()->getObject();
		if (obj->frameEvent) {
			obj->frameEvent(obj, window, deltaTime);
		}
		for (SGNode* child : elements.front()->getChildren()) {
			if (child->getObject()->frameEvent) {
				elements.insert(elements.end(), child);
			}
		}
		elements.erase(elements.begin());
	}
	
	lastFrame = currentTime;
}

bool GraphicsEngine::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// List validation layers
	std::cout << "available validation layers:" << std::endl;
	for (const auto& availableLayer : availableLayers) {
		std::cout << "\t" << availableLayer.layerName << std::endl;
	}

	// Check if all validation Layers are in the list
	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> GraphicsEngine::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void GraphicsEngine::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Change the first nullptr to glfwGetPrimaryMonitor() for fullscreen
	window = glfwCreateWindow(WIDTH, HEIGHT, "ONIEngine", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
}

void GraphicsEngine::initVulkan() {
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	createTextures();
	createModels();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();
}

void GraphicsEngine::createInstance() {
	// Check validation layers
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested but not available.");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "ONIEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance !");
	}

	// List extensions
	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : extensions) {
		std::cout << "\t" << extension << std::endl;
	}

	// Check if all required extensions are included in the supported extensions list
	std::vector<bool> extensionsAreSupported(createInfo.enabledExtensionCount);
	const char ** requiredExtensions = glfwGetRequiredInstanceExtensions(&createInfo.enabledExtensionCount);
	for (size_t i = 0; i < createInfo.enabledExtensionCount; i++) {
		for (const auto& extension : extensions) {
			if (strcmp(requiredExtensions[i], extension) == 0) {
				extensionsAreSupported[i] = true;
			}
		}
		if (!extensionsAreSupported[i]) {
			extensionsAreSupported[i] = false;
		}
	}
	for (size_t i = 0; i < createInfo.enabledExtensionCount; i++) {
		if (!extensionsAreSupported[i]) {
			std::cout << "\t" << requiredExtensions[i] << " is not supported!" << std::endl;
		}
	}
}

void GraphicsEngine::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void GraphicsEngine::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void GraphicsEngine::pickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			msaaSamples = getMaxUsableSampleCount();
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void GraphicsEngine::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),
	indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void GraphicsEngine::recreateSwapChain() {
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	cleanupSwapChain();

	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
}

void GraphicsEngine::cleanupSwapChain() {
	vkDestroyImageView(device, colorImageView, nullptr);
	vkDestroyImage(device, colorImage, nullptr);
	vkFreeMemory(device, colorImageMemory, nullptr);

	vkDestroyImageView(device, depthImageView, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);

	for (int i = 0; i < scene->getDirectionalLights().size() + scene->getSpotLights().size(); i++) {
		vkDestroyImageView(device, shadowsImageViews[i], nullptr);
		vkDestroyImage(device, shadowsImages[i], nullptr);
		vkFreeMemory(device, shadowsImageMemories[i], nullptr);
		for (int j = 0; j < swapChainImages.size(); j++) {
			vkDestroyFramebuffer(device, shadowsFramebuffers[j][i], nullptr);
		}
	}

	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	// Destroy pipelines
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipeline(device, skyboxGraphicsPipeline, nullptr);
	vkDestroyPipeline(device, shadowsGraphicsPipeline, nullptr);

	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyPipelineLayout(device, skyboxPipelineLayout, nullptr);
	vkDestroyPipelineLayout(device, shadowsPipelineLayout, nullptr);
	vkDestroyRenderPass(device, shadowsRenderPass, nullptr);

	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);

	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Object *obj = elements.front()->getObject();
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			vkFreeMemory(device, obj->getObjectBufferMemories()->at(i), nullptr);
			vkDestroyBuffer(device, obj->getObjectBuffers()->at(i), nullptr);
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	if (scene->getSkybox()) {
		Object *skybox = scene->getSkybox();
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			vkFreeMemory(device, skybox->getObjectBufferMemories()->at(i), nullptr);
			vkDestroyBuffer(device, skybox->getObjectBuffers()->at(i), nullptr);
		}
	}
	
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		vkFreeMemory(device, cameraBuffersMemory[i], nullptr);
		vkDestroyBuffer(device, cameraBuffers[i], nullptr);
		vkFreeMemory(device, lightsBuffersMemory[i], nullptr);
		vkDestroyBuffer(device, lightsBuffers[i], nullptr);
		vkFreeMemory(device, shadowsBuffersMemory[i], nullptr);
		vkDestroyBuffer(device, shadowsBuffers[i], nullptr);
	}
	
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroyDescriptorPool(device, shadowsDescriptorPool, nullptr);
}

void GraphicsEngine::createSwapChain() {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void GraphicsEngine::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

void GraphicsEngine::createRenderPass() {
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = msaaSamples;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachmentResolve = {};
	colorAttachmentResolve.format = swapChainImageFormat;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentResolveRef = {};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
		| VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}

	// Shadows

	VkAttachmentDescription shadowsDepthAttachment = {};
	shadowsDepthAttachment.format = VK_FORMAT_D16_UNORM;
	shadowsDepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	shadowsDepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	shadowsDepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	shadowsDepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	shadowsDepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	shadowsDepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	shadowsDepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

	VkAttachmentReference shadowsDepthAttachmentRef = {};
	shadowsDepthAttachmentRef.attachment = 0;
	shadowsDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription shadowsSubpass = {};
	shadowsSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	shadowsSubpass.colorAttachmentCount = 0;
	shadowsSubpass.pDepthStencilAttachment = &shadowsDepthAttachmentRef;

	std::array<VkSubpassDependency, 2> shadowsDependencies;
	shadowsDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	shadowsDependencies[0].dstSubpass = 0;
	shadowsDependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	shadowsDependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	shadowsDependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	shadowsDependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	shadowsDependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	shadowsDependencies[1].srcSubpass = 0;
	shadowsDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	shadowsDependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	shadowsDependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	shadowsDependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	shadowsDependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	shadowsDependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo shadowsRenderPassInfo = {};
	shadowsRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	shadowsRenderPassInfo.attachmentCount = 1;
	shadowsRenderPassInfo.pAttachments = &shadowsDepthAttachment;
	shadowsRenderPassInfo.subpassCount = 1;
	shadowsRenderPassInfo.pSubpasses = &shadowsSubpass;
	shadowsRenderPassInfo.dependencyCount = static_cast<uint32_t>(shadowsDependencies.size());
	shadowsRenderPassInfo.pDependencies = shadowsDependencies.data();

	if (vkCreateRenderPass(device, &shadowsRenderPassInfo, nullptr, &shadowsRenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadows render pass!");
	}
}

void GraphicsEngine::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding cboLayoutBinding = {};
	cboLayoutBinding.binding = 1;
	cboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	cboLayoutBinding.descriptorCount = 1;
	cboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	cboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding lboLayoutBinding = {};
	lboLayoutBinding.binding = 2;
	lboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lboLayoutBinding.descriptorCount = 1;
	lboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	lboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding sboLayoutBinding = {};
	sboLayoutBinding.binding = 3;
	sboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	sboLayoutBinding.descriptorCount = 1;
	sboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	sboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding shadowsSamplerLayoutBinding = {};
	shadowsSamplerLayoutBinding.binding = 4;
	shadowsSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	shadowsSamplerLayoutBinding.descriptorCount = static_cast<uint32_t>(scene->getDirectionalLights().size() + scene->getSpotLights().size());
	shadowsSamplerLayoutBinding.pImmutableSamplers = nullptr;
	shadowsSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding diffuseSamplerLayoutBinding = {};
	diffuseSamplerLayoutBinding.binding = 5;
	diffuseSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	diffuseSamplerLayoutBinding.descriptorCount = 1;
	diffuseSamplerLayoutBinding.pImmutableSamplers = nullptr;
	diffuseSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding normalSamplerLayoutBinding = {};
	normalSamplerLayoutBinding.binding = 6;
	normalSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	normalSamplerLayoutBinding.descriptorCount = 1;
	normalSamplerLayoutBinding.pImmutableSamplers = nullptr;
	normalSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding metallicSamplerLayoutBinding = {};
	metallicSamplerLayoutBinding.binding = 7;
	metallicSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	metallicSamplerLayoutBinding.descriptorCount = 1;
	metallicSamplerLayoutBinding.pImmutableSamplers = nullptr;
	metallicSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding roughnessSamplerLayoutBinding = {};
	roughnessSamplerLayoutBinding.binding = 8;
	roughnessSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	roughnessSamplerLayoutBinding.descriptorCount = 1;
	roughnessSamplerLayoutBinding.pImmutableSamplers = nullptr;
	roughnessSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding AOSamplerLayoutBinding = {};
	AOSamplerLayoutBinding.binding = 9;
	AOSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	AOSamplerLayoutBinding.descriptorCount = 1;
	AOSamplerLayoutBinding.pImmutableSamplers = nullptr;
	AOSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 10> bindings = { uboLayoutBinding, cboLayoutBinding, lboLayoutBinding, sboLayoutBinding, shadowsSamplerLayoutBinding, diffuseSamplerLayoutBinding, normalSamplerLayoutBinding, metallicSamplerLayoutBinding, roughnessSamplerLayoutBinding, AOSamplerLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	// Shadows

	VkDescriptorSetLayoutBinding shadowsUboLayoutBinding = {};
	shadowsUboLayoutBinding.binding = 0;
	shadowsUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	shadowsUboLayoutBinding.descriptorCount = 1;
	shadowsUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	shadowsUboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding shadowsSboLayoutBinding = {};
	shadowsSboLayoutBinding.binding = 1;
	shadowsSboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	shadowsSboLayoutBinding.descriptorCount = 1;
	shadowsSboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	shadowsSboLayoutBinding.pImmutableSamplers = nullptr;

	std::array<VkDescriptorSetLayoutBinding, 2> shadowsBindings = { shadowsUboLayoutBinding, shadowsSboLayoutBinding };

	VkDescriptorSetLayoutCreateInfo shadowsLayoutInfo = {};
	shadowsLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	shadowsLayoutInfo.bindingCount = static_cast<uint32_t>(shadowsBindings.size());
	shadowsLayoutInfo.pBindings = shadowsBindings.data();

	if (vkCreateDescriptorSetLayout(device, &shadowsLayoutInfo, nullptr, &shadowsDescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadows descriptor set layout!");
	}
}

void GraphicsEngine::createGraphicsPipeline() {
	auto vertShaderCode = readFile("shaders/pbr.vert.spv");
	auto fragShaderCode = readFile("shaders/pbr.frag.spv");

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkSpecializationMapEntry mapEntry = {};
	mapEntry.constantID = 0;
	mapEntry.offset = 0;
	mapEntry.size = sizeof(int);

	int specValue = (int)(scene->getDirectionalLights().size() + scene->getSpotLights().size());

	VkSpecializationInfo fragSpecInfo = {};
	fragSpecInfo.mapEntryCount = 1;
	fragSpecInfo.pMapEntries = &mapEntry;
	fragSpecInfo.dataSize = sizeof(int);
	fragSpecInfo.pData = &specValue;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	fragShaderStageInfo.pSpecializationInfo = &fragSpecInfo;

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.rasterizationSamples = msaaSamples;
	multisampling.minSampleShading = 0.1f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {};
	depthStencil.back = {};

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
		| VK_COLOR_COMPONENT_G_BIT
		| VK_COLOR_COMPONENT_B_BIT
		| VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Object *obj = elements.front()->getObject();
		obj->setGraphicsPipeline(&graphicsPipeline);
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	vkDestroyShaderModule(device, vertShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);

	// Skybox pipeline

	if (scene->getSkybox()) {
		auto skyboxVertShaderCode = readFile("shaders/skybox.vert.spv");
		auto skyboxFragShaderCode = readFile("shaders/skybox.frag.spv");

		VkShaderModule skyboxVertShaderModule = createShaderModule(skyboxVertShaderCode);
		VkShaderModule skyboxFragShaderModule = createShaderModule(skyboxFragShaderCode);

		VkPipelineShaderStageCreateInfo skyboxVertShaderStageInfo = {};
		skyboxVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		skyboxVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		skyboxVertShaderStageInfo.module = skyboxVertShaderModule;
		skyboxVertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo skyboxFragShaderStageInfo = {};
		skyboxFragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		skyboxFragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		skyboxFragShaderStageInfo.module = skyboxFragShaderModule;
		skyboxFragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo skyboxShaderStages[] = { skyboxVertShaderStageInfo, skyboxFragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo skyboxVertexInputInfo = {};
		auto skyboxBindingDescription = Vertex::getBindingDescription();
		auto skyboxAttributeDescriptions = Vertex::getAttributeDescriptions();
		skyboxVertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		skyboxVertexInputInfo.vertexBindingDescriptionCount = 1;
		skyboxVertexInputInfo.pVertexBindingDescriptions = &skyboxBindingDescription;
		skyboxVertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(skyboxAttributeDescriptions.size());
		skyboxVertexInputInfo.pVertexAttributeDescriptions = skyboxAttributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo skyboxInputAssembly = {};
		skyboxInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		skyboxInputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		skyboxInputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport skyboxViewport = {};
		skyboxViewport.x = 0.0f;
		skyboxViewport.y = 0.0f;
		skyboxViewport.width = (float)swapChainExtent.width;
		skyboxViewport.height = (float)swapChainExtent.height;
		skyboxViewport.minDepth = 0.0f;
		skyboxViewport.maxDepth = 1.0f;

		VkRect2D skyboxScissor = {};
		skyboxScissor.offset = { 0, 0 };
		skyboxScissor.extent = swapChainExtent;

		VkPipelineViewportStateCreateInfo skyboxViewportState = {};
		skyboxViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		skyboxViewportState.viewportCount = 1;
		skyboxViewportState.pViewports = &viewport;
		skyboxViewportState.scissorCount = 1;
		skyboxViewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo skyboxRasterizer = {};
		skyboxRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		skyboxRasterizer.depthClampEnable = VK_FALSE;
		skyboxRasterizer.rasterizerDiscardEnable = VK_FALSE;
		skyboxRasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		skyboxRasterizer.lineWidth = 1.0f;
		skyboxRasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		skyboxRasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		skyboxRasterizer.depthBiasEnable = VK_FALSE;
		skyboxRasterizer.depthBiasConstantFactor = 0.0f;
		skyboxRasterizer.depthBiasClamp = 0.0f;
		skyboxRasterizer.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo skyboxMultisampling = {};
		skyboxMultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		skyboxMultisampling.sampleShadingEnable = VK_TRUE;
		skyboxMultisampling.rasterizationSamples = msaaSamples;
		skyboxMultisampling.minSampleShading = 0.1f;
		skyboxMultisampling.pSampleMask = nullptr;
		skyboxMultisampling.alphaToCoverageEnable = VK_FALSE;
		skyboxMultisampling.alphaToOneEnable = VK_FALSE;

		VkPipelineDepthStencilStateCreateInfo skyboxDepthStencil = {};
		skyboxDepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		skyboxDepthStencil.depthTestEnable = VK_TRUE;
		skyboxDepthStencil.depthWriteEnable = VK_TRUE;
		skyboxDepthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		skyboxDepthStencil.depthBoundsTestEnable = VK_FALSE;
		skyboxDepthStencil.minDepthBounds = 0.0f;
		skyboxDepthStencil.maxDepthBounds = 1.0f;
		skyboxDepthStencil.stencilTestEnable = VK_FALSE;
		skyboxDepthStencil.front = {};
		skyboxDepthStencil.back = {};

		VkPipelineColorBlendAttachmentState skyboxColorBlendAttachment = {};
		skyboxColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
			| VK_COLOR_COMPONENT_G_BIT
			| VK_COLOR_COMPONENT_B_BIT
			| VK_COLOR_COMPONENT_A_BIT;
		skyboxColorBlendAttachment.blendEnable = VK_TRUE;
		skyboxColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		skyboxColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		skyboxColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		skyboxColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		skyboxColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		skyboxColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo skyboxColorBlending = {};
		skyboxColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		skyboxColorBlending.logicOpEnable = VK_FALSE;
		skyboxColorBlending.logicOp = VK_LOGIC_OP_COPY;
		skyboxColorBlending.attachmentCount = 1;
		skyboxColorBlending.pAttachments = &skyboxColorBlendAttachment;
		skyboxColorBlending.blendConstants[0] = 0.0f;
		skyboxColorBlending.blendConstants[1] = 0.0f;
		skyboxColorBlending.blendConstants[2] = 0.0f;
		skyboxColorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo skyboxPipelineLayoutInfo = {};
		skyboxPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		skyboxPipelineLayoutInfo.setLayoutCount = 1;
		skyboxPipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
		skyboxPipelineLayoutInfo.pushConstantRangeCount = 0;
		skyboxPipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device, &skyboxPipelineLayoutInfo, nullptr, &skyboxPipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create skybox pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo skyboxPipelineInfo = {};
		skyboxPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		skyboxPipelineInfo.stageCount = 2;
		skyboxPipelineInfo.pStages = skyboxShaderStages;
		skyboxPipelineInfo.pVertexInputState = &skyboxVertexInputInfo;
		skyboxPipelineInfo.pInputAssemblyState = &skyboxInputAssembly;
		skyboxPipelineInfo.pViewportState = &skyboxViewportState;
		skyboxPipelineInfo.pRasterizationState = &skyboxRasterizer;
		skyboxPipelineInfo.pMultisampleState = &skyboxMultisampling;
		skyboxPipelineInfo.pDepthStencilState = &skyboxDepthStencil;
		skyboxPipelineInfo.pColorBlendState = &skyboxColorBlending;
		skyboxPipelineInfo.pDynamicState = nullptr;
		skyboxPipelineInfo.layout = skyboxPipelineLayout;
		skyboxPipelineInfo.renderPass = renderPass;
		skyboxPipelineInfo.subpass = 0;
		skyboxPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		skyboxPipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &skyboxPipelineInfo, nullptr, &skyboxGraphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create skybox graphics pipeline!");
		}

		Object* skybox = scene->getSkybox();
		skybox->setGraphicsPipeline(&skyboxGraphicsPipeline);

		vkDestroyShaderModule(device, skyboxVertShaderModule, nullptr);
		vkDestroyShaderModule(device, skyboxFragShaderModule, nullptr);
	}

	// Shadows pipeline

	auto shadowsVertShaderCode = readFile("shaders/shadows.vert.spv");

	VkShaderModule shadowsVertShaderModule = createShaderModule(shadowsVertShaderCode);

	VkPipelineShaderStageCreateInfo shadowsVertShaderStageInfo = {};
	shadowsVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shadowsVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shadowsVertShaderStageInfo.module = shadowsVertShaderModule;
	shadowsVertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shadowsShaderStages[] = { shadowsVertShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo shadowsVertexInputInfo = {};
	auto shadowsBindingDescription = Vertex::getBindingDescription();
	auto shadowsAttributeDescriptions = Vertex::getAttributeDescriptions();
	shadowsVertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	shadowsVertexInputInfo.vertexBindingDescriptionCount = 1;
	shadowsVertexInputInfo.pVertexBindingDescriptions = &shadowsBindingDescription;
	shadowsVertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(shadowsAttributeDescriptions.size());
	shadowsVertexInputInfo.pVertexAttributeDescriptions = shadowsAttributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo shadowsInputAssembly = {};
	shadowsInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	shadowsInputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	shadowsInputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport shadowsViewport = {};
	shadowsViewport.x = 0.0f;
	shadowsViewport.y = 0.0f;
	shadowsViewport.width = (float)SHADOWMAP_WIDTH;
	shadowsViewport.height = (float)SHADOWMAP_HEIGHT;
	shadowsViewport.minDepth = 0.0f;
	shadowsViewport.maxDepth = 1.0f;

	VkRect2D shadowsScissor = {};
	shadowsScissor.offset = { 0, 0 };
	shadowsScissor.extent.width = SHADOWMAP_WIDTH;
	shadowsScissor.extent.height = SHADOWMAP_HEIGHT;

	VkPipelineViewportStateCreateInfo shadowsViewportState = {};
	shadowsViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	shadowsViewportState.viewportCount = 1;
	shadowsViewportState.pViewports = &shadowsViewport;
	shadowsViewportState.scissorCount = 1;
	shadowsViewportState.pScissors = &shadowsScissor;

	VkPipelineRasterizationStateCreateInfo shadowsRasterizer = {};
	shadowsRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	shadowsRasterizer.depthClampEnable = VK_FALSE;
	shadowsRasterizer.rasterizerDiscardEnable = VK_FALSE;
	shadowsRasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	shadowsRasterizer.lineWidth = 1.0f;
	shadowsRasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	shadowsRasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	shadowsRasterizer.depthBiasEnable = VK_FALSE;
	shadowsRasterizer.depthBiasConstantFactor = 0.0f;
	shadowsRasterizer.depthBiasClamp = 0.0f;
	shadowsRasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo shadowsMultisampling = {};
	shadowsMultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	shadowsMultisampling.sampleShadingEnable = VK_TRUE;
	shadowsMultisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	shadowsMultisampling.minSampleShading = 0.1f;
	shadowsMultisampling.pSampleMask = nullptr;
	shadowsMultisampling.alphaToCoverageEnable = VK_FALSE;
	shadowsMultisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo shadowsDepthStencil = {};
	shadowsDepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	shadowsDepthStencil.depthTestEnable = VK_TRUE;
	shadowsDepthStencil.depthWriteEnable = VK_TRUE;
	shadowsDepthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	shadowsDepthStencil.depthBoundsTestEnable = VK_FALSE;
	shadowsDepthStencil.minDepthBounds = 0.0f;
	shadowsDepthStencil.maxDepthBounds = 1.0f;
	shadowsDepthStencil.stencilTestEnable = VK_FALSE;
	shadowsDepthStencil.front = {};
	shadowsDepthStencil.back = {};

	VkPushConstantRange shadowsPushConstantRange = {};
	shadowsPushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	shadowsPushConstantRange.offset = 0;
	shadowsPushConstantRange.size = sizeof(int);

	VkPipelineLayoutCreateInfo shadowsPipelineLayoutInfo = {};
	shadowsPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	shadowsPipelineLayoutInfo.setLayoutCount = 1;
	shadowsPipelineLayoutInfo.pSetLayouts = &shadowsDescriptorSetLayout;
	shadowsPipelineLayoutInfo.pushConstantRangeCount = 1;
	shadowsPipelineLayoutInfo.pPushConstantRanges = &shadowsPushConstantRange;

	if (vkCreatePipelineLayout(device, &shadowsPipelineLayoutInfo, nullptr, &shadowsPipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadows pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo shadowsPipelineInfo = {};
	shadowsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	shadowsPipelineInfo.stageCount = 1;
	shadowsPipelineInfo.pStages = shadowsShaderStages;
	shadowsPipelineInfo.pVertexInputState = &shadowsVertexInputInfo;
	shadowsPipelineInfo.pInputAssemblyState = &shadowsInputAssembly;
	shadowsPipelineInfo.pViewportState = &shadowsViewportState;
	shadowsPipelineInfo.pRasterizationState = &shadowsRasterizer;
	shadowsPipelineInfo.pMultisampleState = &shadowsMultisampling;
	shadowsPipelineInfo.pDepthStencilState = &shadowsDepthStencil;
	shadowsPipelineInfo.pColorBlendState = nullptr;
	shadowsPipelineInfo.pDynamicState = nullptr;
	shadowsPipelineInfo.layout = shadowsPipelineLayout;
	shadowsPipelineInfo.renderPass = shadowsRenderPass;
	shadowsPipelineInfo.subpass = 0;
	shadowsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	shadowsPipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &shadowsPipelineInfo, nullptr, &shadowsGraphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadows graphics pipeline!");
	}

	vkDestroyShaderModule(device, shadowsVertShaderModule, nullptr);
}

void GraphicsEngine::createFramebuffers() {
	swapChainFramebuffers.resize(swapChainImageViews.size());
	shadowsFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		std::array<VkImageView, 3> attachments = {
			colorImageView,
			depthImageView,
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}

		// Shadows
		shadowsFramebuffers[i].resize(scene->getDirectionalLights().size() + scene->getSpotLights().size());
		for (int j = 0; j < scene->getDirectionalLights().size() + scene->getSpotLights().size(); j++) {
			VkFramebufferCreateInfo shadowsFramebufferInfo = {};
			shadowsFramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			shadowsFramebufferInfo.renderPass = shadowsRenderPass;
			shadowsFramebufferInfo.attachmentCount = 1;
			shadowsFramebufferInfo.pAttachments = &shadowsImageViews[j];
			shadowsFramebufferInfo.width = SHADOWMAP_WIDTH;
			shadowsFramebufferInfo.height = SHADOWMAP_HEIGHT;
			shadowsFramebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &shadowsFramebufferInfo, nullptr, &shadowsFramebuffers[i][j]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create shadow framebuffer!");
			}
		}
	}
}

void GraphicsEngine::createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0;

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void GraphicsEngine::createColorResources() {
	VkFormat colorFormat = swapChainImageFormat;
	createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	transitionImageLayout(colorImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
}

void GraphicsEngine::createDepthResources() {
	VkFormat depthFormat = findDepthFormat();
	createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

	// Shadows

	shadowsImages.resize(scene->getDirectionalLights().size() + scene->getSpotLights().size());
	shadowsImageViews.resize(scene->getDirectionalLights().size() + scene->getSpotLights().size());
	shadowsImageMemories.resize(scene->getDirectionalLights().size() + scene->getSpotLights().size());
	for (int i = 0; i < scene->getDirectionalLights().size() + scene->getSpotLights().size(); i++) {
		createImage(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 1, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_D16_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, shadowsImages[i], shadowsImageMemories[i]);
		shadowsImageViews[i] = createImageView(shadowsImages[i], VK_FORMAT_D16_UNORM, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	}
}

void GraphicsEngine::createTextures() {
	// Create textures for all elements
	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		if (!elements.front()->getObject()->getMaterial()->isConstructed()) {
			elements.front()->getObject()->getMaterial()->constructedTrue();
			createTextureImage(elements.front()->getObject());
			createTextureImageView(elements.front()->getObject());
			createTextureSampler(elements.front()->getObject());
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	// Create skybox texture
	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		if (!skybox->getMaterial()->isConstructed()) {
			skybox->getMaterial()->constructedTrue();
			createTextureImage(skybox);
			createTextureImageView(skybox);
			createTextureSampler(skybox);
		}
	}

	// Create shadow texture sampler
	VkSamplerCreateInfo shadowSamplerInfo = {};
	shadowSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	shadowSamplerInfo.magFilter = VK_FILTER_LINEAR;
	shadowSamplerInfo.minFilter = VK_FILTER_LINEAR;
	shadowSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	shadowSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	shadowSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	shadowSamplerInfo.maxAnisotropy = 1.0f;
	shadowSamplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	shadowSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	shadowSamplerInfo.minLod = 0.0f;
	shadowSamplerInfo.maxLod = 1.0f;
	shadowSamplerInfo.mipLodBias = 0.0f;

	if (vkCreateSampler(device, &shadowSamplerInfo, nullptr, &shadowsSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadow texture sampler!");
	}
}

void GraphicsEngine::createModels() {
	// Create models for all elements
	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		if (!elements.front()->getObject()->getMesh()->isConstructed()) {
			elements.front()->getObject()->getMesh()->constructedTrue();
			loadModel(elements.front()->getObject());
			createVertexBuffer(elements.front()->getObject());
			createIndexBuffer(elements.front()->getObject());
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	// Create skybox model
	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		if (!skybox->getMesh()->isConstructed()) {
			skybox->getMesh()->constructedTrue();
			loadModel(skybox);
			createVertexBuffer(skybox);
			createIndexBuffer(skybox);
		}
	}
}

void GraphicsEngine::createUniformBuffers() {
	int nbElems = scene->nbElements();

	VkDeviceSize bufferSize = sizeof(ObjectBufferObject);
	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Object *obj = elements.front()->getObject();
		obj->getObjectBuffers()->resize(swapChainImages.size());
		obj->getObjectBufferMemories()->resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, obj->getObjectBuffers()->at(i), obj->getObjectBufferMemories()->at(i));
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	// Skybox

	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		skybox->getObjectBuffers()->resize(swapChainImages.size());
		skybox->getObjectBufferMemories()->resize(swapChainImages.size());
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, skybox->getObjectBuffers()->at(i), skybox->getObjectBufferMemories()->at(i));
		}
	}

	// Camera

	cameraBuffers.resize(swapChainImages.size());
	cameraBuffersMemory.resize(swapChainImages.size());

	bufferSize = sizeof(CameraBufferObject);
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, cameraBuffers[i], cameraBuffersMemory[i]);
	}

	// Lights

	lightsBuffers.resize(swapChainImages.size());
	lightsBuffersMemory.resize(swapChainImages.size());

	bufferSize = sizeof(LightsBufferObject);
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, lightsBuffers[i], lightsBuffersMemory[i]);
	}

	// Shadows

	shadowsBuffers.resize(swapChainImages.size());
	shadowsBuffersMemory.resize(swapChainImages.size());

	bufferSize = sizeof(ShadowsBufferObject);
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, shadowsBuffers[i], shadowsBuffersMemory[i]);
	}
}

void GraphicsEngine::createDescriptorPool() {
	int nbElems = scene->nbElements();
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(nbElems*swapChainImages.size()*4);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(nbElems*swapChainImages.size()*6);

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(nbElems*swapChainImages.size());

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

	// Shadows

	VkDescriptorPoolSize shadowsPoolSize = {};
	shadowsPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	shadowsPoolSize.descriptorCount = static_cast<uint32_t>(nbElems*swapChainImages.size()*2);

	VkDescriptorPoolCreateInfo shadowsPoolInfo = {};
	shadowsPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	shadowsPoolInfo.poolSizeCount = 1;
	shadowsPoolInfo.pPoolSizes = &shadowsPoolSize;
	shadowsPoolInfo.maxSets = static_cast<uint32_t>(nbElems*swapChainImages.size());

	if (vkCreateDescriptorPool(device, &shadowsPoolInfo, nullptr, &shadowsDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadows descriptor pool!");
	}
}

void GraphicsEngine::createDescriptorSets() {
	int nbElems = scene->nbElements();
	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Object* obj = elements.front()->getObject();
		obj->getDescriptorSets()->resize(swapChainImages.size());
		if (vkAllocateDescriptorSets(device, &allocInfo, obj->getDescriptorSets()->data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			updateDescriptorSets(elements.front()->getObject(), (int)i);
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	// Skybox

	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		skybox->getDescriptorSets()->resize(swapChainImages.size());
		if (vkAllocateDescriptorSets(device, &allocInfo, skybox->getDescriptorSets()->data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			updateDescriptorSets(skybox, (int)i);
		}
	}

	// Shadows
	std::vector<VkDescriptorSetLayout> shadowsLayouts(swapChainImages.size(), shadowsDescriptorSetLayout);
	VkDescriptorSetAllocateInfo shadowsAllocInfo = {};
	shadowsAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	shadowsAllocInfo.descriptorPool = shadowsDescriptorPool;
	shadowsAllocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	shadowsAllocInfo.pSetLayouts = shadowsLayouts.data();

	elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Object* obj = elements.front()->getObject();
		obj->getShadowsDescriptorSets()->resize(swapChainImages.size());
		if (vkAllocateDescriptorSets(device, &shadowsAllocInfo, obj->getShadowsDescriptorSets()->data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			updateShadowsDescriptorSets(elements.front()->getObject(), (int)i);
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}
}

void GraphicsEngine::createCommandBuffers() {
	commandBuffers.resize(swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.f, 0.f, 0.f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkRenderPassBeginInfo shadowsRenderPassInfo = {};
		shadowsRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		shadowsRenderPassInfo.renderPass = shadowsRenderPass;
		shadowsRenderPassInfo.renderArea.offset = { 0, 0 };
		shadowsRenderPassInfo.renderArea.extent.width = SHADOWMAP_WIDTH;
		shadowsRenderPassInfo.renderArea.extent.height = SHADOWMAP_HEIGHT;
		shadowsRenderPassInfo.clearValueCount = 1;
		shadowsRenderPassInfo.pClearValues = &clearValues[1];

		VkDeviceSize offsets[] = { 0 };

		std::vector<SGNode*> elements;

		// First passes : Shadows
		for (int j = 0; j < scene->getDirectionalLights().size() + scene->getSpotLights().size(); j++) {
			shadowsRenderPassInfo.framebuffer = shadowsFramebuffers[i][j];
			vkCmdBeginRenderPass(commandBuffers[i], &shadowsRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shadowsGraphicsPipeline);

			elements = scene->getRoot()->getChildren();
			while (!elements.empty()) {
				Object* obj = elements.front()->getObject();
				VkBuffer vertexCmdBuffers[] = { *obj->getMesh()->getVertexBuffer() };
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexCmdBuffers, offsets);
				vkCmdBindIndexBuffer(commandBuffers[i], *obj->getMesh()->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shadowsPipelineLayout, 0, 1, &obj->getShadowsDescriptorSets()->at(i), 0, nullptr);
				vkCmdPushConstants(commandBuffers[i], shadowsPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(int), &j);
				vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(obj->getMesh()->getIndices()->size()), 1, 0, 0, 0);
				for (SGNode* child : elements.front()->getChildren()) {
					elements.insert(elements.end(), child);
				}
				elements.erase(elements.begin());
			}

			vkCmdEndRenderPass(commandBuffers[i]);
		}

		// Second pass : Objects
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Skybox if drawn first for color blending
		if (scene->getSkybox()) {
			Object* skybox = scene->getSkybox();
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *skybox->getGraphicsPipeline());
			VkBuffer vertexCmdBuffers[] = { *skybox->getMesh()->getVertexBuffer() };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexCmdBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], *skybox->getMesh()->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPipelineLayout, 0, 1, &skybox->getDescriptorSets()->at(i), 0, nullptr);
			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(skybox->getMesh()->getIndices()->size()), 1, 0, 0, 0);
		}
		// For each 3D model, bind its vertex and indices and the right descriptor set for its texture
		elements = scene->getRoot()->getChildren();
		while (!elements.empty()) {
			Object* obj = elements.front()->getObject();
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *obj->getGraphicsPipeline());
			VkBuffer vertexCmdBuffers[] = { *obj->getMesh()->getVertexBuffer() };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexCmdBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], *obj->getMesh()->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &obj->getDescriptorSets()->at(i), 0, nullptr);
			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(obj->getMesh()->getIndices()->size()), 1, 0, 0, 0);
			for (SGNode* child : elements.front()->getChildren()) {
				elements.insert(elements.end(), child);
			}
			elements.erase(elements.begin());
		}

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void GraphicsEngine::createSyncObjects() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
			|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}
	}
}

VkShaderModule GraphicsEngine::createShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

bool GraphicsEngine::isDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate && deviceFeatures.samplerAnisotropy;
}

bool GraphicsEngine::checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices GraphicsEngine::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

SwapChainSupportDetails GraphicsEngine::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR GraphicsEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR GraphicsEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}
	return bestMode;
}

VkExtent2D GraphicsEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

uint32_t GraphicsEngine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

void GraphicsEngine::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = numSamples;

	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(device, image, imageMemory, 0);
}

void GraphicsEngine::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}
	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void GraphicsEngine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
}

void GraphicsEngine::updateUniformBuffer(Object* obj, uint32_t currentImage) {
	void* data;

	ObjectBufferObject obo = {};
	// Using T * R * S transformation for models
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(obj->getPositionX(), obj->getPositionY(), obj->getPositionZ()));
	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(obj->getRotationX()), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(obj->getRotationY()), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(obj->getRotationZ()), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(obj->getScale()));
	obo.model = translate * rotateX * rotateY * rotateZ * scale;

	vkMapMemory(device, obj->getObjectBufferMemories()->at(currentImage), 0, sizeof(obo), 0, &data);
	memcpy(data, &obo, sizeof(obo));
	vkUnmapMemory(device, obj->getObjectBufferMemories()->at(currentImage));
}

VkCommandBuffer GraphicsEngine::beginSingleTimeCommands() {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void GraphicsEngine::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void GraphicsEngine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (hasStencilComponent(format)) {
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else {
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	endSingleTimeCommands(commandBuffer);
}

void GraphicsEngine::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endSingleTimeCommands(commandBuffer);
}

VkImageView GraphicsEngine::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

VkFormat GraphicsEngine::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat GraphicsEngine::findDepthFormat() {
	return findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool GraphicsEngine::hasStencilComponent(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void GraphicsEngine::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkImageBlit blit = {};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		if (mipWidth > 1) {
			mipWidth /= 2;
		}
		if (mipHeight > 1) {
			mipHeight /= 2;
		}
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	endSingleTimeCommands(commandBuffer);
}

VkSampleCountFlagBits GraphicsEngine::getMaxUsableSampleCount() {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }
	return VK_SAMPLE_COUNT_1_BIT;
}

void GraphicsEngine::createTextureImage(Object* obj) {
	// Diffuse Texture
	VkBuffer diffuseStagingBuffer;
	VkDeviceMemory diffuseStagingBufferMemory;
	VkDeviceSize diffuseImageSize;

	int diffuseTexWidth, diffuseTexHeight, diffuseTexChannels;

	stbi_uc* dPixels;
	if (obj->getMaterial()->getDiffusePath() != "") {
		dPixels = stbi_load(obj->getMaterial()->getDiffusePath().c_str(), &diffuseTexWidth, &diffuseTexHeight, &diffuseTexChannels, STBI_rgb_alpha);
		obj->getMaterial()->setDiffuseMipLevel(static_cast<uint32_t> (std::floor(std::log2(std::max(diffuseTexWidth, diffuseTexHeight)))) + 1);
		diffuseImageSize = diffuseTexWidth * diffuseTexHeight * 4;
	} else {
		unsigned char dRVal = round(obj->getMaterial()->getDiffuseRValue() * 255.0);
		unsigned char dGVal = round(obj->getMaterial()->getDiffuseGValue() * 255.0);
		unsigned char dBVal = round(obj->getMaterial()->getDiffuseBValue() * 255.0);
		unsigned char dAVal = round(obj->getMaterial()->getDiffuseAValue() * 255.0);
		std::array<unsigned char, 4> dArray = { dRVal, dGVal, dBVal, dAVal };
		dPixels = dArray.data();
		obj->getMaterial()->setDiffuseMipLevel(1);
		diffuseTexWidth = 1;
		diffuseTexHeight = 1;
		diffuseImageSize = 4;
	}

	if (!dPixels) {
		throw std::runtime_error("failed to load diffuse texture image!");
	}

	createBuffer(diffuseImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, diffuseStagingBuffer, diffuseStagingBufferMemory);
	void *ddata;
	vkMapMemory(device, diffuseStagingBufferMemory, 0, diffuseImageSize, 0, &ddata);
	memcpy(ddata, dPixels, static_cast<size_t>(diffuseImageSize));
	vkUnmapMemory(device, diffuseStagingBufferMemory);
	if (obj->getMaterial()->getDiffusePath() != "") {
		stbi_image_free(dPixels);
	}
	createImage(diffuseTexWidth, diffuseTexHeight, obj->getMaterial()->getDiffuseMipLevel(), VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMaterial()->getDiffuseTextureImage(), *obj->getMaterial()->getDiffuseTextureImageMemory());

	transitionImageLayout(*obj->getMaterial()->getDiffuseTextureImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, obj->getMaterial()->getDiffuseMipLevel());
	copyBufferToImage(diffuseStagingBuffer, *obj->getMaterial()->getDiffuseTextureImage(), static_cast<uint32_t>(diffuseTexWidth), static_cast<uint32_t>(diffuseTexHeight));
	generateMipmaps(*obj->getMaterial()->getDiffuseTextureImage(), VK_FORMAT_R8G8B8A8_SRGB, diffuseTexWidth, diffuseTexHeight, obj->getMaterial()->getDiffuseMipLevel());

	vkDestroyBuffer(device, diffuseStagingBuffer, nullptr);
	vkFreeMemory(device, diffuseStagingBufferMemory, nullptr);

	// Normal Texture
	VkBuffer normalStagingBuffer;
	VkDeviceMemory normalStagingBufferMemory;
	VkDeviceSize normalImageSize;

	int normalTexWidth, normalTexHeight, normalTexChannels;
	stbi_uc* nPixels;
	if (obj->getMaterial()->getNormalPath() != "") {
		nPixels = stbi_load(obj->getMaterial()->getNormalPath().c_str(), &normalTexWidth, &normalTexHeight, &normalTexChannels, STBI_rgb_alpha);
		obj->getMaterial()->setNormalMipLevel(static_cast<uint32_t> (std::floor(std::log2(std::max(normalTexWidth, normalTexHeight)))) + 1);
		normalImageSize = normalTexWidth * normalTexHeight * 4;
	} else {
		unsigned char nXVal = round(obj->getMaterial()->getNormalXValue() * 255.0);
		unsigned char nYVal = round(obj->getMaterial()->getNormalYValue() * 255.0);
		unsigned char nZVal = round(obj->getMaterial()->getNormalZValue() * 255.0);
		std::array<unsigned char, 4> nArray = { nXVal, nYVal, nZVal, 255 };
		nPixels = nArray.data();
		obj->getMaterial()->setNormalMipLevel(1);
		normalTexWidth = 1;
		normalTexHeight = 1;
		normalImageSize = 4;
	}

	if (!nPixels) {
		throw std::runtime_error("failed to load normal texture image!");
	}

	createBuffer(normalImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, normalStagingBuffer, normalStagingBufferMemory);
	void *ndata;
	vkMapMemory(device, normalStagingBufferMemory, 0, normalImageSize, 0, &ndata);
	memcpy(ndata, nPixels, static_cast<size_t>(normalImageSize));
	vkUnmapMemory(device, normalStagingBufferMemory);
	if (obj->getMaterial()->getNormalPath() != "") {
		stbi_image_free(nPixels);
	}
	createImage(normalTexWidth, normalTexHeight, obj->getMaterial()->getNormalMipLevel(), VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMaterial()->getNormalTextureImage(), *obj->getMaterial()->getNormalTextureImageMemory());

	transitionImageLayout(*obj->getMaterial()->getNormalTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, obj->getMaterial()->getNormalMipLevel());
	copyBufferToImage(normalStagingBuffer, *obj->getMaterial()->getNormalTextureImage(), static_cast<uint32_t>(normalTexWidth), static_cast<uint32_t>(normalTexHeight));
	generateMipmaps(*obj->getMaterial()->getNormalTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, normalTexWidth, normalTexHeight, obj->getMaterial()->getNormalMipLevel());

	vkDestroyBuffer(device, normalStagingBuffer, nullptr);
	vkFreeMemory(device, normalStagingBufferMemory, nullptr);

	// Metallic Texture
	VkBuffer metallicStagingBuffer;
	VkDeviceMemory metallicStagingBufferMemory;
	VkDeviceSize metallicImageSize;

	int metallicTexWidth, metallicTexHeight, metallicTexChannels;
	stbi_uc* mPixels;
	if (obj->getMaterial()->getMetallicPath() != "") {
		mPixels = stbi_load(obj->getMaterial()->getMetallicPath().c_str(), &metallicTexWidth, &metallicTexHeight, &metallicTexChannels, STBI_rgb_alpha);
		obj->getMaterial()->setMetallicMipLevel(static_cast<uint32_t> (std::floor(std::log2(std::max(metallicTexWidth, metallicTexHeight)))) + 1);
		metallicImageSize = metallicTexWidth * metallicTexHeight * 4;
	} else {
		unsigned char mVal = round(obj->getMaterial()->getMetallicValue() * 255.0);
		std::array<unsigned char, 4> mArray = { mVal, mVal, mVal, 255 };
		mPixels = mArray.data();
		obj->getMaterial()->setMetallicMipLevel(1);
		metallicTexWidth = 1;
		metallicTexHeight = 1;
		metallicImageSize = 4;
	}


	if (!mPixels) {
		throw std::runtime_error("failed to load metallic texture image!");
	}

	createBuffer(metallicImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, metallicStagingBuffer, metallicStagingBufferMemory);
	void *mdata;
	vkMapMemory(device, metallicStagingBufferMemory, 0, metallicImageSize, 0, &mdata);
	memcpy(mdata, mPixels, static_cast<size_t>(metallicImageSize));
	vkUnmapMemory(device, metallicStagingBufferMemory);
	if (obj->getMaterial()->getMetallicPath() != "") {
		stbi_image_free(mPixels);
	}
	createImage(metallicTexWidth, metallicTexHeight, obj->getMaterial()->getMetallicMipLevel(), VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMaterial()->getMetallicTextureImage(), *obj->getMaterial()->getMetallicTextureImageMemory());

	transitionImageLayout(*obj->getMaterial()->getMetallicTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, obj->getMaterial()->getMetallicMipLevel());
	copyBufferToImage(metallicStagingBuffer, *obj->getMaterial()->getMetallicTextureImage(), static_cast<uint32_t>(metallicTexWidth), static_cast<uint32_t>(metallicTexHeight));
	generateMipmaps(*obj->getMaterial()->getMetallicTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, metallicTexWidth, metallicTexHeight, obj->getMaterial()->getMetallicMipLevel());

	vkDestroyBuffer(device, metallicStagingBuffer, nullptr);
	vkFreeMemory(device, metallicStagingBufferMemory, nullptr);

	// Roughness Texture
	VkBuffer roughnessStagingBuffer;
	VkDeviceMemory roughnessStagingBufferMemory;
	VkDeviceSize roughnessImageSize;
	int roughnessTexWidth, roughnessTexHeight, roughnessTexChannels;

	stbi_uc* rPixels;
	if (obj->getMaterial()->getRoughnessPath() != "") {
		rPixels = stbi_load(obj->getMaterial()->getRoughnessPath().c_str(), &roughnessTexWidth, &roughnessTexHeight, &roughnessTexChannels, STBI_rgb_alpha);
		obj->getMaterial()->setRoughnessMipLevel(static_cast<uint32_t> (std::floor(std::log2(std::max(roughnessTexWidth, roughnessTexHeight)))) + 1);
		roughnessImageSize = roughnessTexWidth * roughnessTexHeight * 4;
	} else {
		unsigned char rVal = round(obj->getMaterial()->getRoughnessValue() * 255.0);
		std::array<unsigned char, 4> rArray = { rVal, rVal, rVal, 255 };
		rPixels = rArray.data();
		obj->getMaterial()->setRoughnessMipLevel(1);
		roughnessTexWidth = 1;
		roughnessTexHeight = 1;
		roughnessImageSize = 4;
;	}

	if (!rPixels) {
		throw std::runtime_error("failed to load roughness texture image!");
	}

	createBuffer(roughnessImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, roughnessStagingBuffer, roughnessStagingBufferMemory);
	void *rdata;
	vkMapMemory(device, roughnessStagingBufferMemory, 0, roughnessImageSize, 0, &rdata);
	memcpy(rdata, rPixels, static_cast<size_t>(roughnessImageSize));
	vkUnmapMemory(device, roughnessStagingBufferMemory);
	if (obj->getMaterial()->getRoughnessPath() != "") {
		stbi_image_free(rPixels);
	}
	createImage(roughnessTexWidth, roughnessTexHeight, obj->getMaterial()->getRoughnessMipLevel(), VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMaterial()->getRoughnessTextureImage(), *obj->getMaterial()->getRoughnessTextureImageMemory());

	transitionImageLayout(*obj->getMaterial()->getRoughnessTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, obj->getMaterial()->getRoughnessMipLevel());
	copyBufferToImage(roughnessStagingBuffer, *obj->getMaterial()->getRoughnessTextureImage(), static_cast<uint32_t>(roughnessTexWidth), static_cast<uint32_t>(roughnessTexHeight));
	generateMipmaps(*obj->getMaterial()->getRoughnessTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, roughnessTexWidth, roughnessTexHeight, obj->getMaterial()->getRoughnessMipLevel());

	vkDestroyBuffer(device, roughnessStagingBuffer, nullptr);
	vkFreeMemory(device, roughnessStagingBufferMemory, nullptr);

	// AO Texture
	VkBuffer AOStagingBuffer;
	VkDeviceMemory AOStagingBufferMemory;
	VkDeviceSize AOImageSize;

	int AOTexWidth, AOTexHeight, AOTexChannels;
	stbi_uc* aPixels;
	if (obj->getMaterial()->getAOPath() != "") {
		aPixels = stbi_load(obj->getMaterial()->getAOPath().c_str(), &AOTexWidth, &AOTexHeight, &AOTexChannels, STBI_rgb_alpha);
		obj->getMaterial()->setAOMipLevel(static_cast<uint32_t> (std::floor(std::log2(std::max(AOTexWidth, AOTexHeight)))) + 1);
		AOImageSize = AOTexWidth * AOTexHeight * 4;
	} else {
		unsigned char aVal = round(obj->getMaterial()->getAOValue() * 255.0);
		std::array<unsigned char, 4> aArray = { aVal, aVal, aVal, 255 };
		aPixels = aArray.data();
		obj->getMaterial()->setAOMipLevel(1);
		AOTexWidth = 1;
		AOTexHeight = 1;
		AOImageSize = 4;
	}

	if (!aPixels) {
		throw std::runtime_error("failed to load AO texture image!");
	}

	createBuffer(AOImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, AOStagingBuffer, AOStagingBufferMemory);
	void *adata;
	vkMapMemory(device, AOStagingBufferMemory, 0, AOImageSize, 0, &adata);
	memcpy(adata, aPixels, static_cast<size_t>(AOImageSize));
	vkUnmapMemory(device, AOStagingBufferMemory);
	if (obj->getMaterial()->getAOPath() != "") {
		stbi_image_free(aPixels);
	}
	createImage(AOTexWidth, AOTexHeight, obj->getMaterial()->getAOMipLevel(), VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMaterial()->getAOTextureImage(), *obj->getMaterial()->getAOTextureImageMemory());

	transitionImageLayout(*obj->getMaterial()->getAOTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, obj->getMaterial()->getAOMipLevel());
	copyBufferToImage(AOStagingBuffer, *obj->getMaterial()->getAOTextureImage(), static_cast<uint32_t>(AOTexWidth), static_cast<uint32_t>(AOTexHeight));
	generateMipmaps(*obj->getMaterial()->getAOTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, AOTexWidth, AOTexHeight, obj->getMaterial()->getAOMipLevel());

	vkDestroyBuffer(device, AOStagingBuffer, nullptr);
	vkFreeMemory(device, AOStagingBufferMemory, nullptr);
}

void GraphicsEngine::createTextureImageView(Object* obj) {
	// Diffuse Texture
	obj->getMaterial()->setDiffuseTextureImageView(createImageView(*obj->getMaterial()->getDiffuseTextureImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, obj->getMaterial()->getDiffuseMipLevel()));

	// Normal Texture
	obj->getMaterial()->setNormalTextureImageView(createImageView(*obj->getMaterial()->getNormalTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, obj->getMaterial()->getNormalMipLevel()));

	// Metallic Texture
	obj->getMaterial()->setMetallicTextureImageView(createImageView(*obj->getMaterial()->getMetallicTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, obj->getMaterial()->getMetallicMipLevel()));

	// Roughness Texture
	obj->getMaterial()->setRoughnessTextureImageView(createImageView(*obj->getMaterial()->getRoughnessTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, obj->getMaterial()->getRoughnessMipLevel()));

	// AO Texture
	obj->getMaterial()->setAOTextureImageView(createImageView(*obj->getMaterial()->getAOTextureImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, obj->getMaterial()->getAOMipLevel()));
}

void GraphicsEngine::createTextureSampler(Object* obj) {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float> (obj->getMaterial()->getDiffuseMipLevel());
	samplerInfo.mipLodBias = 0.0f;

	// Diffuse Texture
	if (vkCreateSampler(device, &samplerInfo, nullptr, obj->getMaterial()->getDiffuseTextureSampler()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create diffuse texture sampler!");
	}

	// Normal Texture (Lod update)
	samplerInfo.maxLod = static_cast<float> (obj->getMaterial()->getNormalMipLevel());

	if (vkCreateSampler(device, &samplerInfo, nullptr, obj->getMaterial()->getNormalTextureSampler()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create normal texture sampler!");
	}

	// Metallic Texture (Lod update)
	samplerInfo.maxLod = static_cast<float> (obj->getMaterial()->getMetallicMipLevel());

	if (vkCreateSampler(device, &samplerInfo, nullptr, obj->getMaterial()->getMetallicTextureSampler()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create metallic texture sampler!");
	}

	// Roughness Texture (Lod update)
	samplerInfo.maxLod = static_cast<float> (obj->getMaterial()->getRoughnessMipLevel());

	if (vkCreateSampler(device, &samplerInfo, nullptr, obj->getMaterial()->getRoughnessTextureSampler()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create roughness texture sampler!");
	}

	// AO Texture (Lod update)
	samplerInfo.maxLod = static_cast<float> (obj->getMaterial()->getAOMipLevel());

	if (vkCreateSampler(device, &samplerInfo, nullptr, obj->getMaterial()->getAOTextureSampler()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create AO texture sampler!");
	}
}

void GraphicsEngine::loadModel(Object* obj) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj->getMesh()->getModelPath().c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};
			vertex.color = {
				1.0f,
				1.0f,
				1.0f
			};
			vertex.tangent = {
				0.0f,
				0.0f,
				0.0f
			};
			vertex.bitangent = {
				0.0f,
				0.0f,
				0.0f
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(obj->getMesh()->getVertices()->size());
				obj->getMesh()->getVertices()->push_back(vertex);
			}
			obj->getMesh()->getIndices()->push_back(uniqueVertices[vertex]);
		}
	}

	// Tangents and bitangents for normal mapping
	for (int i = 0; i < obj->getMesh()->getIndices()->size(); i += 3) {
		Vertex* vertex0 = &obj->getMesh()->getVertices()->at(obj->getMesh()->getIndices()->at(i + 0));
		Vertex* vertex1 = &obj->getMesh()->getVertices()->at(obj->getMesh()->getIndices()->at(i + 1));
		Vertex* vertex2 = &obj->getMesh()->getVertices()->at(obj->getMesh()->getIndices()->at(i + 2));
		
		glm::vec3 dPos1 = vertex1->pos - vertex0->pos;
		glm::vec3 dPos2 = vertex2->pos - vertex0->pos;

		glm::vec2 dUV1 = vertex1->texCoord - vertex0->texCoord;
		glm::vec2 dUV2 = vertex2->texCoord - vertex0->texCoord;

		float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
		glm::vec3 tangent = (dPos1 * dUV2.y - dPos2 * dUV1.y) * r;
		glm::vec3 bitangent = (dPos2 * dUV1.x - dPos1 * dUV2.x) * r;

		vertex0->tangent += tangent;
		vertex1->tangent += tangent;
		vertex2->tangent += tangent;

		vertex0->bitangent += bitangent;
		vertex1->bitangent += bitangent;
		vertex2->bitangent += bitangent;
	}

	// Average tangents and bitangents
	for (int i = 0; i < obj->getMesh()->getVertices()->size(); i++) {
		Vertex* vert = &obj->getMesh()->getVertices()->at(i);
		vert->tangent = glm::normalize(vert->tangent);
		vert->bitangent = glm::normalize(vert->bitangent);
	}
}

void GraphicsEngine::createVertexBuffer(Object* obj) {
	VkDeviceSize bufferSize = sizeof(obj->getMesh()->getVertices()->front()) * obj->getMesh()->getVertices()->size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, obj->getMesh()->getVertices()->data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMesh()->getVertexBuffer(), *obj->getMesh()->getVertexBufferMemory());
	copyBuffer(stagingBuffer, *obj->getMesh()->getVertexBuffer(), bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void GraphicsEngine::createIndexBuffer(Object* obj) {
	VkDeviceSize bufferSize = sizeof(obj->getMesh()->getIndices()->front()) * obj->getMesh()->getIndices()->size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, obj->getMesh()->getIndices()->data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *obj->getMesh()->getIndexBuffer(), *obj->getMesh()->getIndexBufferMemory());
	copyBuffer(stagingBuffer, *obj->getMesh()->getIndexBuffer(), bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void GraphicsEngine::updateDescriptorSets(Object* obj, int frame) {
	VkDescriptorBufferInfo objectInfo = {};
	objectInfo.buffer = obj->getObjectBuffers()->at(frame);
	objectInfo.offset = 0;
	objectInfo.range = sizeof(ObjectBufferObject);

	VkDescriptorBufferInfo cameraInfo = {};
	cameraInfo.buffer = cameraBuffers[frame];
	cameraInfo.offset = 0;
	cameraInfo.range = sizeof(CameraBufferObject);

	VkDescriptorBufferInfo lightsInfo = {};
	lightsInfo.buffer = lightsBuffers[frame];
	lightsInfo.offset = 0;
	lightsInfo.range = sizeof(LightsBufferObject);

	VkDescriptorBufferInfo shadowsInfo = {};
	shadowsInfo.buffer = shadowsBuffers[frame];
	shadowsInfo.offset = 0;
	shadowsInfo.range = sizeof(ShadowsBufferObject);

	std::vector<VkDescriptorImageInfo> shadowsImageInfos;
	shadowsImageInfos.resize(scene->getDirectionalLights().size() + scene->getSpotLights().size());
	for (int i = 0; i < scene->getDirectionalLights().size() + scene->getSpotLights().size(); i++) {
		shadowsImageInfos[i] = {};
		shadowsImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		shadowsImageInfos[i].imageView = shadowsImageViews[i];
		shadowsImageInfos[i].sampler = shadowsSampler;
	}

	VkDescriptorImageInfo diffuseImageInfo = {};
	diffuseImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	diffuseImageInfo.imageView = *obj->getMaterial()->getDiffuseTextureImageView();
	diffuseImageInfo.sampler = *obj->getMaterial()->getDiffuseTextureSampler();

	VkDescriptorImageInfo normalImageInfo = {};
	normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	normalImageInfo.imageView = *obj->getMaterial()->getNormalTextureImageView();
	normalImageInfo.sampler = *obj->getMaterial()->getNormalTextureSampler();

	VkDescriptorImageInfo metallicImageInfo = {};
	metallicImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	metallicImageInfo.imageView = *obj->getMaterial()->getMetallicTextureImageView();
	metallicImageInfo.sampler = *obj->getMaterial()->getMetallicTextureSampler();

	VkDescriptorImageInfo roughnessImageInfo = {};
	roughnessImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	roughnessImageInfo.imageView = *obj->getMaterial()->getRoughnessTextureImageView();
	roughnessImageInfo.sampler = *obj->getMaterial()->getRoughnessTextureSampler();

	VkDescriptorImageInfo AOImageInfo = {};
	AOImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	AOImageInfo.imageView = *obj->getMaterial()->getAOTextureImageView();
	AOImageInfo.sampler = *obj->getMaterial()->getAOTextureSampler();

	std::array<VkWriteDescriptorSet, 10> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &objectInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pBufferInfo = &cameraInfo;

	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[2].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[2].dstBinding = 2;
	descriptorWrites[2].dstArrayElement = 0;
	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[2].descriptorCount = 1;
	descriptorWrites[2].pBufferInfo = &lightsInfo;

	descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[3].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[3].dstBinding = 3;
	descriptorWrites[3].dstArrayElement = 0;
	descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[3].descriptorCount = 1;
	descriptorWrites[3].pBufferInfo = &shadowsInfo;

	descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[4].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[4].dstBinding = 4;
	descriptorWrites[4].dstArrayElement = 0;
	descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[4].descriptorCount = static_cast<uint32_t>(shadowsImageInfos.size());
	descriptorWrites[4].pImageInfo = shadowsImageInfos.data();

	descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[5].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[5].dstBinding = 5;
	descriptorWrites[5].dstArrayElement = 0;
	descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[5].descriptorCount = 1;
	descriptorWrites[5].pImageInfo = &diffuseImageInfo;

	descriptorWrites[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[6].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[6].dstBinding = 6;
	descriptorWrites[6].dstArrayElement = 0;
	descriptorWrites[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[6].descriptorCount = 1;
	descriptorWrites[6].pImageInfo = &normalImageInfo;

	descriptorWrites[7].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[7].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[7].dstBinding = 7;
	descriptorWrites[7].dstArrayElement = 0;
	descriptorWrites[7].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[7].descriptorCount = 1;
	descriptorWrites[7].pImageInfo = &metallicImageInfo;

	descriptorWrites[8].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[8].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[8].dstBinding = 8;
	descriptorWrites[8].dstArrayElement = 0;
	descriptorWrites[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[8].descriptorCount = 1;
	descriptorWrites[8].pImageInfo = &roughnessImageInfo;

	descriptorWrites[9].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[9].dstSet = obj->getDescriptorSets()->at(frame);
	descriptorWrites[9].dstBinding = 9;
	descriptorWrites[9].dstArrayElement = 0;
	descriptorWrites[9].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[9].descriptorCount = 1;
	descriptorWrites[9].pImageInfo = &AOImageInfo;

	vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void GraphicsEngine::updateShadowsDescriptorSets(Object* obj, int frame) {
	VkDescriptorBufferInfo objectInfo = {};
	objectInfo.buffer = obj->getObjectBuffers()->at(frame);
	objectInfo.offset = 0;
	objectInfo.range = sizeof(ObjectBufferObject);

	VkDescriptorBufferInfo shadowsInfo = {};
	shadowsInfo.buffer = shadowsBuffers[frame];
	shadowsInfo.offset = 0;
	shadowsInfo.range = sizeof(ShadowsBufferObject);

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = obj->getShadowsDescriptorSets()->at(frame);
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &objectInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = obj->getShadowsDescriptorSets()->at(frame);
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pBufferInfo = &shadowsInfo;

	vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void GraphicsEngine::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		frameEvents(window);
		drawFrame();
	}

	vkDeviceWaitIdle(device);
}

void GraphicsEngine::drawFrame() {
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		updateUniformBuffer(elements.front()->getObject(), imageIndex);
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	// Skybox
	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		updateUniformBuffer(skybox, imageIndex);
	}

	void *data;

	// Camera
	Camera *camera = scene->getCamera();
	CameraBufferObject cbo = {};
	cbo.view = glm::lookAt(glm::vec3(camera->getPositionX(), camera->getPositionY(), camera->getPositionZ()), glm::vec3(camera->getPositionX() + camera->getFrontX(), camera->getPositionY() + camera->getFrontY(), camera->getPositionZ() + camera->getFrontZ()), glm::vec3(camera->getUpX(), camera->getUpY(), camera->getUpZ()));
	cbo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
	cbo.proj[1][1] *= -1;
	cbo.pos = glm::vec3(camera->getPositionX(), camera->getPositionY(), camera->getPositionZ());

	vkMapMemory(device, cameraBuffersMemory[imageIndex], 0, sizeof(cbo), 0, &data);
	memcpy(data, &cbo, sizeof(cbo));
	vkUnmapMemory(device, cameraBuffersMemory[imageIndex]);

	// Lights
	LightsBufferObject lbo = {};
	std::vector<DirectionalLight*> dirLights = scene->getDirectionalLights();
	std::vector<PointLight*> pointLights = scene->getPointLights();
	std::vector<SpotLight*> spotLights = scene->getSpotLights();
	lbo.numLights.x = dirLights.size();
	lbo.numLights.y = pointLights.size();
	lbo.numLights.z = spotLights.size();
	for (size_t i = 0; i < lbo.numLights.x; i++) {
		lbo.dirLightsDir[i] = glm::vec3(dirLights[i]->getDirectionX(), dirLights[i]->getDirectionY(), dirLights[i]->getDirectionZ());
		lbo.dirLightsColor[i] = glm::vec3(dirLights[i]->getColorR(), dirLights[i]->getColorG(), dirLights[i]->getColorB());
	}
	for (size_t i = 0; i < lbo.numLights.y; i++) {
		lbo.pointLightsPos[i] = glm::vec3(pointLights[i]->getPositionX(), pointLights[i]->getPositionY(), pointLights[i]->getPositionZ());
		lbo.pointLightsColor[i] = glm::vec3(pointLights[i]->getColorR(), pointLights[i]->getColorG(), pointLights[i]->getColorB());
	}
	for (size_t i = 0; i < lbo.numLights.z; i++) {
		lbo.spotLightsPos[i] = glm::vec3(spotLights[i]->getPositionX(), spotLights[i]->getPositionY(), spotLights[i]->getPositionZ());
		lbo.spotLightsDir[i] = glm::vec3(spotLights[i]->getDirectionX(), spotLights[i]->getDirectionY(), spotLights[i]->getDirectionZ());
		lbo.spotLightsColor[i] = glm::vec3(spotLights[i]->getColorR(), spotLights[i]->getColorG(), spotLights[i]->getColorB());
		lbo.spotLightsCutoffs[i] = glm::vec2(spotLights[i]->getCutoff(), spotLights[i]->getOutCutoff());
	}

	vkMapMemory(device, lightsBuffersMemory[imageIndex], 0, sizeof(lbo), 0, &data);
	memcpy(data, &lbo, sizeof(lbo));
	vkUnmapMemory(device, lightsBuffersMemory[imageIndex]);

	// Shadows
	ShadowsBufferObject sbo = {};
	sbo.numLights.x = dirLights.size();
	sbo.numLights.y = pointLights.size();
	sbo.numLights.z = spotLights.size();
	glm::mat4 shadowsProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
	for (int i = 0; i < sbo.numLights.x; i++) {
		glm::mat4 shadowsView = glm::lookAt(glm::vec3(-dirLights[i]->getDirectionX(), -dirLights[i]->getDirectionY(), -dirLights[i]->getDirectionZ()), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		sbo.dirLightsSpace[i] = shadowsProj * shadowsView;
	}
	for (int i = 0; i < sbo.numLights.z; i++) {
		shadowsProj = glm::perspective(glm::acos(spotLights[i]->getOutCutoff()), SHADOWMAP_WIDTH / (float)SHADOWMAP_HEIGHT, 0.1f, 20.0f);
		glm::mat4 shadowsView = glm::lookAt(glm::vec3(spotLights[i]->getPositionX(), spotLights[i]->getPositionY(), spotLights[i]->getPositionZ()), glm::vec3(spotLights[i]->getPositionX() + spotLights[i]->getDirectionX(), spotLights[i]->getPositionY() + spotLights[i]->getDirectionY(), spotLights[i]->getPositionZ() + spotLights[i]->getDirectionZ()), glm::vec3(0.0f, 0.0f, 1.0f));
		sbo.spotLightsSpace[i] = shadowsProj * shadowsView;
	}

	vkMapMemory(device, shadowsBuffersMemory[imageIndex], 0, sizeof(sbo), 0, &data);
	memcpy(data, &sbo, sizeof(sbo));
	vkUnmapMemory(device, shadowsBuffersMemory[imageIndex]);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void GraphicsEngine::cleanup() {
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	cleanupSwapChain();

	std::vector<SGNode*> elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Material *material = elements.front()->getObject()->getMaterial();
		if (!material->isDestructed()) {
			vkDestroySampler(device, *material->getDiffuseTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getDiffuseTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getDiffuseTextureImage(), nullptr);
			vkFreeMemory(device, *material->getDiffuseTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getNormalTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getNormalTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getNormalTextureImage(), nullptr);
			vkFreeMemory(device, *material->getNormalTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getMetallicTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getMetallicTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getMetallicTextureImage(), nullptr);
			vkFreeMemory(device, *material->getMetallicTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getRoughnessTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getRoughnessTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getRoughnessTextureImage(), nullptr);
			vkFreeMemory(device, *material->getRoughnessTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getAOTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getAOTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getAOTextureImage(), nullptr);
			vkFreeMemory(device, *material->getAOTextureImageMemory(), nullptr);

			material->destructedTrue();
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		Material *material = skybox->getMaterial();
		if (!material->isDestructed()) {
			vkDestroySampler(device, *material->getDiffuseTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getDiffuseTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getDiffuseTextureImage(), nullptr);
			vkFreeMemory(device, *material->getDiffuseTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getNormalTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getNormalTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getNormalTextureImage(), nullptr);
			vkFreeMemory(device, *material->getNormalTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getMetallicTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getMetallicTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getMetallicTextureImage(), nullptr);
			vkFreeMemory(device, *material->getMetallicTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getRoughnessTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getRoughnessTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getRoughnessTextureImage(), nullptr);
			vkFreeMemory(device, *material->getRoughnessTextureImageMemory(), nullptr);

			vkDestroySampler(device, *material->getAOTextureSampler(), nullptr);
			vkDestroyImageView(device, *material->getAOTextureImageView(), nullptr);
			vkDestroyImage(device, *material->getAOTextureImage(), nullptr);
			vkFreeMemory(device, *material->getAOTextureImageMemory(), nullptr);

			material->destructedTrue();
		}
	}

	vkDestroySampler(device, shadowsSampler, nullptr);

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	vkDestroyDescriptorSetLayout(device, shadowsDescriptorSetLayout, nullptr);

	elements = scene->getRoot()->getChildren();
	while (!elements.empty()) {
		Mesh *mesh = elements.front()->getObject()->getMesh();
		if (!mesh->isDestructed()) {
			vkDestroyBuffer(device, *mesh->getVertexBuffer(), nullptr);
			vkFreeMemory(device, *mesh->getVertexBufferMemory(), nullptr);
			vkDestroyBuffer(device, *mesh->getIndexBuffer(), nullptr);
			vkFreeMemory(device, *mesh->getIndexBufferMemory(), nullptr);

			mesh->destructedTrue();
		}
		for (SGNode* child : elements.front()->getChildren()) {
			elements.insert(elements.end(), child);
		}
		elements.erase(elements.begin());
	}

	if (scene->getSkybox()) {
		Object* skybox = scene->getSkybox();
		Mesh *mesh = skybox->getMesh();
		if (!mesh->isDestructed()) {
			vkDestroyBuffer(device, *skybox->getMesh()->getIndexBuffer(), nullptr);
			vkFreeMemory(device, *skybox->getMesh()->getIndexBufferMemory(), nullptr);
			vkDestroyBuffer(device, *skybox->getMesh()->getVertexBuffer(), nullptr);
			vkFreeMemory(device, *skybox->getMesh()->getVertexBufferMemory(), nullptr);

			mesh->destructedTrue();
		}
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

int GraphicsEngine::start() {
	try {
		run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
