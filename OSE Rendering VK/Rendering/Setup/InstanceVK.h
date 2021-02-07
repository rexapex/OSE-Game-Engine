#pragma once
#include "OSE-Core/Windowing/WindowManager.h"

namespace ose::rendering
{
	class InstanceVK
	{
	public:
		InstanceVK(WindowManager const & window_manager)
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

			if(!InitPhysicalDevice())
				throw std::exception("Failed to find a physical device capable of supporting required Vulkan features");

			if(!InitLogicalDevice())
				throw std::exception("Failed to create a logical device");
		}

		~InstanceVK()
		{
			vkDestroySurfaceKHR(instance_, *surface_, nullptr);
			vkDestroyInstance(instance_, nullptr);
			vkDestroyDevice(logical_device_, nullptr);
		}

	private:
		void InitExtensions(VkInstanceCreateInfo & create_info, std::vector<char const *> const & extensions)
		{
			create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			create_info.ppEnabledExtensionNames = extensions.data();
		}

		bool InitValidationLayers(VkInstanceCreateInfo & create_info, std::vector<char const *> const & validation_layers)
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

		bool InitSurface(WindowManager const & window_manager)
		{
			try
			{
				VkSurfaceKHR * surface_ptr = static_cast<VkSurfaceKHR *>(window_manager.CreateSurface(&instance_));
				surface_ = uptr<VkSurfaceKHR>(surface_ptr);
			}
			catch(std::exception & e)
			{
				LOG_ERROR("Failed to initialise surface: ", e.what());
				return false;
			}
			return true;
		}

		bool InitPhysicalDevice()
		{
			VkPhysicalDevice physical_device = VK_NULL_HANDLE;
			VkPhysicalDeviceProperties physical_device_props;
			VkPhysicalDeviceFeatures physical_device_features;
			uint32_t device_count { 0 };
			vkEnumeratePhysicalDevices(instance_, &device_count, nullptr);

			if(device_count == 0)
				return false;

			std::vector<VkPhysicalDevice> devices(device_count);
			vkEnumeratePhysicalDevices(instance_, &device_count, devices.data());

			auto find_queue_families = [this](VkPhysicalDevice const & device) {
				uint32_t queue_family_count = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

				std::vector<VkQueueFamilyProperties> queue_family_props(queue_family_count);
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_props.data());

				std::optional<uint32_t> graphics_family;
				std::optional<uint32_t> present_family;

				// Find a queue which can render graphics and a queue which can present to the surface
				for(size_t i = 0; i < queue_family_props.size(); ++i)
				{
					auto const & props = queue_family_props[i];
					if(props.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						graphics_family = i;
					}

					VkBool32 present_support = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface_, &present_support);
					if(present_support)
					{
						present_family = i;
					}

					if(graphics_family.has_value() && present_family.has_value())
					{
						break;
					}
				}

				return std::make_pair(graphics_family, present_family);
			};

			auto set_best_device = [this, &find_queue_families, &physical_device, &physical_device_props, &physical_device_features](VkPhysicalDevice const & device) {
				VkPhysicalDeviceProperties props;
				vkGetPhysicalDeviceProperties(device, &props);

				VkPhysicalDeviceFeatures features;
				vkGetPhysicalDeviceFeatures(device, &features);

				auto const & [graphics_family, present_family] = find_queue_families(device);

				// TODO - Refactor into a rating system to find the best device
				if(graphics_family.has_value() && present_family.has_value() && features.geometryShader)
				{
					if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
					{
						if(physical_device_props.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
							|| physical_device_props.limits.maxImageDimension2D < props.limits.maxImageDimension2D)
						{
							physical_device = device;
							physical_device_props = props;
							physical_device_features = features;
							graphics_family_ = graphics_family.value();
							present_family_ = present_family.value();
						}
					}
					else if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
						&& physical_device == VK_NULL_HANDLE)
					{
						physical_device = device;
						physical_device_props = props;
						physical_device_features = features;
						graphics_family_ = graphics_family.value();
						present_family_ = present_family.value();
					}
				}
			};

			for(auto const & device : devices)
			{
				set_best_device(device);
			}

			physical_device_ = physical_device;
			return physical_device_ != VK_NULL_HANDLE;
		}

		bool InitLogicalDevice()
		{
			std::set<uint32_t> queue_families { graphics_family_, present_family_ };
			std::vector<VkDeviceQueueCreateInfo> queue_create_info_vec;

			float priority = 1.0f;
			for(uint32_t family : queue_families)
			{
				VkDeviceQueueCreateInfo queue_create_info {};
				queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_create_info.queueFamilyIndex = graphics_family_;
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

			// TODO - Set validation layers to support older Vulkan versions where the feature isn't deprecated

			VkResult result = vkCreateDevice(physical_device_, &device_create_info, nullptr, &logical_device_);
			if(result != VK_SUCCESS)
				return false;

			// queueIndex is the index within the queue family so value 0 for both makes sense
			vkGetDeviceQueue(logical_device_, graphics_family_, 0, &graphics_queue_);
			vkGetDeviceQueue(logical_device_, present_family_, 0, &present_queue_);

			return true;
		}

	private:
		VkInstance instance_ {};
		VkPhysicalDevice physical_device_ {};
		VkDevice logical_device_ {};

		uptr<VkSurfaceKHR> surface_;

		uint32_t graphics_family_;
		uint32_t present_family_;
		VkQueue graphics_queue_;
		VkQueue present_queue_;
	};
}
