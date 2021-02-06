#pragma once

namespace ose::rendering
{
	class InstanceVK
	{
	public:
		InstanceVK(std::vector<char const *> const & extensions)
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

			InitExtensions(create_info, extensions);

			std::vector<char const *> const validation_layers { "VK_LAYER_KHRONOS_validation" };
			if(!InitValidationLayers(create_info, validation_layers))
				throw std::exception("Failed to initialise validation layers");

			VkResult result = vkCreateInstance(&create_info, nullptr, &instance_);
			if(result != VK_SUCCESS)
				throw std::exception("Failed to create Vulkan instance: %d", result);
		}

		~InstanceVK()
		{
			vkDestroyInstance(instance_, nullptr);
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

	private:
		VkInstance instance_ {};
	};
}
