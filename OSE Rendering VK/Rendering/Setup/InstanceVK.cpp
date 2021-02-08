#include "pch.h"
#include "InstanceVK.h"
#include "PhysicalDeviceVK.h"
#include "OSE-Core/Windowing/WindowManager.h"

namespace ose::rendering
{
	InstanceVK::InstanceVK(WindowManager const & window_manager)
	{
		VkApplicationInfo app_info {};
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = "OSE Game Engine";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "OSE Game Engine";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo create_info {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;

		std::vector<char const *> extensions { window_manager.GetExtensions() };
		InitExtensions(create_info, extensions);

		std::vector<char const *> const validation_layers { "VK_LAYER_KHRONOS_validation" };
		if(!InitValidationLayers(create_info, validation_layers))
			throw std::exception("Failed to initialise validation layers");

		VkResult result = vkCreateInstance(&create_info, nullptr, &instance_);
		if(result != VK_SUCCESS)
			throw std::exception("Failed to create Vulkan instance: %d", result);

		if(!InitSurface(window_manager))
			throw std::exception("Failed to create a surface to render to");

		physical_device_ = ose::make_unique<PhysicalDeviceVK>(window_manager, *this);

		if(!InitLogicalDevice())
			throw std::exception("Failed to create a logical device");
	}

	InstanceVK::~InstanceVK()
	{
		physical_device_ = nullptr;	// Force destructor to be called before destroying instance
		vkDestroyInstance(instance_, nullptr);
		vkDestroyDevice(logical_device_, nullptr);
	}

	void InstanceVK::InitExtensions(VkInstanceCreateInfo & create_info, std::vector<char const *> const & extensions)
	{
		// Instance extensions
		create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();

		// Physical device extensions
		required_device_extensions_.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	bool InstanceVK::InitValidationLayers(VkInstanceCreateInfo & create_info, std::vector<char const *> const & validation_layers)
	{
		create_info.enabledLayerCount = 0;

		// If in a non-debug build, return success without adding any validation layers
#		ifdef NDEBUG
		return true;
#		endif

		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		// Return false if any of the requested validation layers are unavailable
		for(char const * layer_name : validation_layers)
		{
			auto iter = std::find_if(available_layers.begin(), available_layers.end(), [layer_name](auto const & props) { return strcmp(props.layerName, layer_name); });
			if(iter == available_layers.end())
				return false;
		}

		// Add the validation layers to the instance create info
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();

		return true;
	}

	bool InstanceVK::InitSurface(WindowManager const & window_manager)
	{
		try
		{
			VkSurfaceKHR * surface_ptr = static_cast<VkSurfaceKHR *>(window_manager.CreateSurface(instance_));
			uptr<VkSurfaceKHR> surface = uptr<VkSurfaceKHR>(surface_ptr);
			surface_ = *surface;
		}
		catch(std::exception & e)
		{
			LOG_ERROR("Failed to initialise surface: ", e.what());
			return false;
		}
		return true;
	}

	bool InstanceVK::InitLogicalDevice()
	{
		uint32_t graphics_family = physical_device_->GetQueueFamilyIndices().graphics_family_.value();
		uint32_t present_family = physical_device_->GetQueueFamilyIndices().present_family_.value();

		std::set<uint32_t> queue_families { graphics_family, present_family };
		std::vector<VkDeviceQueueCreateInfo> queue_create_info_vec;

		float priority = 1.0f;
		for(uint32_t family : queue_families)
		{
			VkDeviceQueueCreateInfo queue_create_info {};
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex = graphics_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &priority;
			queue_create_info_vec.push_back(queue_create_info);
		}

		VkPhysicalDeviceFeatures physical_device_features {};

		VkDeviceCreateInfo device_create_info {};
		device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.pQueueCreateInfos = queue_create_info_vec.data();
		device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_info_vec.size());
		device_create_info.pEnabledFeatures = &physical_device_features;
		device_create_info.ppEnabledExtensionNames = required_device_extensions_.data();
		device_create_info.enabledExtensionCount = static_cast<uint32_t>(required_device_extensions_.size());

		// TODO - Set validation layers to support older Vulkan versions where the feature isn't deprecated

		VkResult result = vkCreateDevice(physical_device_->Get(), &device_create_info, nullptr, &logical_device_);
		if(result != VK_SUCCESS)
			return false;

		// queueIndex is the index within the queue family so value 0 for both makes sense
		vkGetDeviceQueue(logical_device_, graphics_family, 0, &graphics_queue_);
		vkGetDeviceQueue(logical_device_, present_family, 0, &present_queue_);

		return true;
	}
}
