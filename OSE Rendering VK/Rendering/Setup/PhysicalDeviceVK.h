#pragma once
#include "SwapChainSupportDetails.h"
#include "QueueFamilyIndices.h"

namespace ose
{
	class WindowManager;
}

namespace ose::rendering
{
	class InstanceVK;

	class PhysicalDeviceVK final
	{
	public:
		PhysicalDeviceVK(WindowManager const & window_manager, InstanceVK const & instance);
		~PhysicalDeviceVK();

		VkPhysicalDevice const & Get() const { return device_; }

		VkSurfaceKHR const & GetSurface() const { return surface_; }

		QueueFamilyIndices const & GetQueueFamilyIndices() const { return queue_family_indices_; }

	private:
		void TrySelectDevice(VkPhysicalDevice const & device);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice const & device);

		bool DoesDeviceSupportRequiredExtensions(VkPhysicalDevice const & device);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice const & device);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const & formats);

		VkPresentModeKHR ChooseSwapPresentMode(std::vector<VkPresentModeKHR> const & present_modes);

		VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR const & caps);

	private:
		WindowManager const & window_manager_;
		InstanceVK const & instance_;

		VkPhysicalDevice device_ {};
		VkSurfaceKHR surface_;

		VkPhysicalDeviceProperties properties_;
		VkPhysicalDeviceFeatures features_;

		QueueFamilyIndices queue_family_indices_;
	};
}
