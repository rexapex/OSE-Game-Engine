#include "pch.h"
#include "PhysicalDeviceVK.h"
#include "InstanceVK.h"
#include "OSE-Core/Windowing/WindowManager.h"

namespace ose::rendering
{
	PhysicalDeviceVK::PhysicalDeviceVK(WindowManager const & window_manager, InstanceVK const & instance)
		: window_manager_(window_manager), instance_(instance), device_(VK_NULL_HANDLE)
	{
		uint32_t device_count { 0 };
		vkEnumeratePhysicalDevices(instance_.Get(), &device_count, nullptr);

		if(device_count == 0)
			throw std::exception("Failed to create physical device, no devices available");

		std::vector<VkPhysicalDevice> devices(device_count);
		vkEnumeratePhysicalDevices(instance_.Get(), &device_count, devices.data());

		for(auto const & device : devices)
		{
			TrySelectDevice(device);
		}
	}

	PhysicalDeviceVK::~PhysicalDeviceVK()
	{
		vkDestroySurfaceKHR(instance_.Get(), surface_, nullptr);
	}

	void PhysicalDeviceVK::TrySelectDevice(VkPhysicalDevice const & device)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(device, &props);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		QueueFamilyIndices queue_family_indices = FindQueueFamilies(device);

		// TODO - Refactor into a rating system to find the best device
		if(DoesDeviceSupportRequiredExtensions(device) && queue_family_indices.IsComplete() && features.geometryShader)
		{
			SwapChainSupportDetails swap_chain_details = QuerySwapChainSupport(device);
			VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat(swap_chain_details.formats_);
			if(!swap_chain_details.formats_.empty() && !swap_chain_details.present_modes_.empty())
			{
				if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					if(properties_.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
						|| properties_.limits.maxImageDimension2D < props.limits.maxImageDimension2D)
					{
						device_ = device;
						properties_ = props;
						features_ = features;
						queue_family_indices_ = queue_family_indices;
					}
				}
				else if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
					&& device_ == VK_NULL_HANDLE)
				{
					device_ = device;
					properties_ = props;
					features_ = features;
					queue_family_indices_ = queue_family_indices;
				}
			}
		}
	}

	SwapChainSupportDetails PhysicalDeviceVK::QuerySwapChainSupport(VkPhysicalDevice const & device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities_);

		uint32_t format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, nullptr);

		if(format_count != 0)
		{
			details.formats_.resize(format_count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, details.formats_.data());
		}

		uint32_t present_mode_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_mode_count, nullptr);

		if(present_mode_count != 0)
		{
			details.present_modes_.resize(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_mode_count, details.present_modes_.data());
		}

		return details;
	}

	bool PhysicalDeviceVK::DoesDeviceSupportRequiredExtensions(VkPhysicalDevice const & device)
	{
		uint32_t extension_count;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> available_extensions(extension_count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

		std::set<std::string> required_extensions { instance_.GetRequiredDeviceExtensions().begin(), instance_.GetRequiredDeviceExtensions().end() };

		for(auto const & ext : available_extensions)
		{
			required_extensions.erase(ext.extensionName);
		}

		return required_extensions.empty();
	}

	QueueFamilyIndices PhysicalDeviceVK::FindQueueFamilies(VkPhysicalDevice const & device)
	{
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_family_props(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_props.data());

		QueueFamilyIndices indices;

		// Find a queue which can render graphics and a queue which can present to the surface
		for(size_t i = 0; i < queue_family_props.size(); ++i)
		{
			auto const & props = queue_family_props[i];
			if(props.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphics_family_ = i;
			}

			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &present_support);
			if(present_support)
			{
				indices.present_family_ = i;
			}

			if(indices.IsComplete())
			{
				break;
			}
		}

		return indices;
	}

	VkSurfaceFormatKHR PhysicalDeviceVK::ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const & formats)
	{
		for(auto const & format : formats)
		{
			if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;
		}
		return formats[0];
	}

	VkPresentModeKHR PhysicalDeviceVK::ChooseSwapPresentMode(std::vector<VkPresentModeKHR> const & present_modes)
	{
		for(auto const & mode : present_modes)
		{
			if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
				return mode;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D PhysicalDeviceVK::ChooseSwapExtent(VkSurfaceCapabilitiesKHR const & caps)
	{
		if(caps.currentExtent.width != UINT32_MAX)
		{
			return caps.currentExtent;
		}
		else
		{
			VkExtent2D extent = {
				static_cast<uint32_t>(window_manager_.GetFramebufferWidth()),
				static_cast<uint32_t>(window_manager_.GetFramebufferHeight())
			};

			extent.width = std::clamp(extent.width, caps.minImageExtent.width, caps.maxImageExtent.width);
			extent.height = std::clamp(extent.height, caps.minImageExtent.height, caps.maxImageExtent.height);

			return extent;
		}
	}
}
