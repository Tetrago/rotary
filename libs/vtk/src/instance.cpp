#include "vtk/instance.hpp"

#include <algorithm>
#include <ranges>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <diag/diag.hpp>

namespace vtk
{
	namespace
	{
		VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			const VkDebugUtilsMessengerCallbackDataEXT* pData,
			void* pUser)
		{
			diag::LogLevel level = [&severity]()
			{
				switch(severity)
				{
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return diag::LogLevel::Trace;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return diag::LogLevel::Info;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return diag::LogLevel::Warn;
				default:
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return diag::LogLevel::Error;
				}
			}();

			diag::logger("vtk").log(level, "[Vulkan] {}", pData->pMessage);

			return VK_FALSE;
		}

		VkDebugUtilsMessengerCreateInfoEXT get_messenger_create_info()
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debug_callback;

			return createInfo;
		}

		template<typename T>
		std::optional<std::vector<std::string_view>> find_missing_elements(std::span<T const> extensions, std::span<std::string_view const> names, const char*(*getter)(const T&)) noexcept
		{
			std::vector<std::string_view> missing(names.begin(), names.end());

			// Remove any element names that we can find in the list
			// This leaves any missing element behind
			std::erase_if(missing, [&extensions, &getter](std::string_view name)
			{
				// If there are no matching extensions in the list, then we want to keep the current name in the missing list
				return !std::ranges::filter_view(extensions, [&name, &getter](const T& props){ return getter(props) == name; }).empty();
			});

			// Return the missing element, or an empty optional
			return missing.empty() ? std::nullopt : std::optional(missing);
		}
	}

	Instance::Instance(const InstanceBuilder& builder)
	{
		// Check for all the requested instance extensions
		if(auto missing = find_missing_elements<VkExtensionProperties>(get_instance_extensions(), builder.mExtensionNames, [](const VkExtensionProperties& props){ return props.extensionName; }))
		{
			std::ranges::for_each(missing.value(), [](std::string_view name)
			{
				diag::logger("vtk").error("Missing instance extension `{}`", name);
			});

			throw std::runtime_error("Not all required instance extensions are present");
		}

		// Check for all the requested instance layers
		if(auto missing = find_missing_elements<VkLayerProperties>(get_instance_layers(), builder.mLayerNames, [](const VkLayerProperties& props){ return props.layerName; }))
		{
			std::ranges::for_each(missing.value(), [](std::string_view name)
			{
				diag::logger("vtk").error("Missing instance layer `{}`", name);
			});

			throw std::runtime_error("Not all required instance layers are present");
		}

		std::vector<const char*> extensions;
		extensions.reserve(builder.mExtensionNames.size());
		std::ranges::transform(builder.mExtensionNames, std::back_inserter(extensions), [](std::string_view name){ return name.data(); }); // Convert string_views to raw C strings
		extensions.erase(std::get<0>(std::ranges::unique(extensions)), extensions.end()); // Remove any duplicates

		std::vector<const char*> layers;
		layers.reserve(builder.mLayerNames.size());
		std::ranges::transform(builder.mLayerNames, std::back_inserter(layers), [](std::string_view name){ return name.data(); }); // Convert string_views to raw C strings
		layers.erase(std::get<0>(std::ranges::unique(layers)), layers.end()); // Remove any duplicates

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &builder.mAppInfo;
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = layers.size();
		createInfo.ppEnabledLayerNames = layers.data();

		VkDebugUtilsMessengerCreateInfoEXT messengerInfo = get_messenger_create_info();
		if(builder.mDebug)
		{
			createInfo.pNext = &messengerInfo;
		}

		if(vkCreateInstance(&createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance");
		}

		if(builder.mDebug)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo = get_messenger_create_info();

			if(get<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT")(mHandle, &createInfo, nullptr, &mMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Vulkan debug messenger");
			}
		}
	}

	Instance::~Instance() noexcept
	{
		if(mHandle == VK_NULL_HANDLE) return;

		if(mMessenger != VK_NULL_HANDLE)
		{
			get<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT")(mHandle, mMessenger, nullptr);
		}

		vkDestroyInstance(mHandle, nullptr);
	}

	Instance::Instance(Instance&& other) noexcept
		: mHandle(other.mHandle)
		, mMessenger(other.mMessenger)
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	InstanceBuilder::InstanceBuilder() noexcept
	{
		mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	}

	InstanceBuilder& InstanceBuilder::application(std::string_view name, int major, int minor, int patch) noexcept
	{
		mAppInfo.pApplicationName = name.data();
		mAppInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);

		return *this;
	}

	InstanceBuilder& InstanceBuilder::engine(std::string_view name, int major, int minor, int patch) noexcept
	{
		mAppInfo.pEngineName = name.data();
		mAppInfo.engineVersion = VK_MAKE_VERSION(major, minor, patch);

		return *this;
	}

	InstanceBuilder& InstanceBuilder::extensions(std::span<std::string_view const> names) noexcept
	{
		std::ranges::transform(names, std::back_inserter(mExtensionNames), [](std::string_view name){ return name.data(); });

		return *this;
	}

	InstanceBuilder& InstanceBuilder::debug() noexcept
	{
		if(!mDebug)
		{
			mDebug = true;
			
			mExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			mLayerNames.push_back("VK_LAYER_KHRONOS_validation");
		}

		return *this;
	}

	Ref<Instance> InstanceBuilder::build() const
	{
		return Ref<Instance>(new Instance(*this));
	}

	std::vector<VkExtensionProperties> get_instance_extensions() noexcept
	{
		uint32_t count;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

		return extensions;
	}

	std::vector<VkLayerProperties> get_instance_layers() noexcept
	{
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);
		std::vector<VkLayerProperties> layers(count);
		vkEnumerateInstanceLayerProperties(&count, layers.data());

		return layers;
	}
}    
