#pragma once

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace shcc
{
	enum class DataType
	{
		Float,
		Int
	};

	size_t get_size_of(DataType type) noexcept;

	struct VertexInput
	{
		std::string name;
		DataType type;
		int count;
	};

	struct Resources
	{
		std::vector<VertexInput> vertexInputs;
	};

	enum class Language
	{
		Spirv
	};

	enum class Stage
	{
		Vertex,
		Fragment
	};

	std::string_view get_entry_point(Stage stage) noexcept;

	class Package
	{
		friend class Runtime;
	public:
		std::vector<uint8_t> compile(Stage stage, Language language) const;

		const Resources& resources() const noexcept { return mResources; }
	private:
		Package(std::shared_ptr<Runtime const> runtime, const std::string& source, const std::unordered_set<Stage>& stages);

		void parse(const std::string& source, const std::unordered_set<Stage>& stages);
		void reflect();

		std::shared_ptr<Runtime const> mRuntime;
		Resources mResources{};
		std::unordered_map<Stage, std::vector<uint8_t>> mSpirv;
	};

	using Callback = std::function<void(const std::string&)>;

	class Runtime : public std::enable_shared_from_this<Runtime>
	{
		friend std::shared_ptr<Runtime> runtime(Callback callback) noexcept;
	public:
		~Runtime() noexcept;

		Runtime(const Runtime&) = delete;
		Runtime& operator=(const Runtime&) = delete;

		Package package(const std::string& source, const std::unordered_set<Stage>& stages) const;
		void log(const std::string& message) const noexcept;
	private:
		Runtime(Callback callback) noexcept;

		Callback mCallback;
	};

	std::shared_ptr<Runtime> runtime(Callback callback) noexcept;
}
