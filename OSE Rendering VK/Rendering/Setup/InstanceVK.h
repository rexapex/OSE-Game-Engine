#pragma once

namespace ose
{
	class WindowManager;
}

namespace ose::rendering
{
	class PhysicalDeviceVK;

	class InstanceVK final
	{
	public:
		InstanceVK(WindowManager const & window_manager);
		~InstanceVK();

		VkInstance const & Get() const { return instance_; }

		VkSurfaceKHR const & GetSurface() const { return surface_; }

		std::vector<char const *> const & GetRequiredDeviceExtensions() const { return required_device_extensions_; }

	private:
		void InitExtensions(VkInstanceCreateInfo & create_info, std::vector<char const *> const & extensions);

		bool InitValidationLayers(VkInstanceCreateInfo & create_info, std::vector<char const *> const & validation_layers);

		bool InitSurface(WindowManager const & window_manager);

		bool InitLogicalDevice();

	private:
		VkInstance instance_ {};
		VkSurfaceKHR surface_ {};
		uptr<PhysicalDeviceVK> physical_device_;
		VkDevice logical_device_ {};

		VkQueue graphics_queue_;
		VkQueue present_queue_;

		std::vector<char const *> required_device_extensions_;
	};
}
