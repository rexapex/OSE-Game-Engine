#pragma once

namespace ose::resources
{
	struct CustomObject
	{
		std::unordered_map<
			std::string,
			std::variant<
				int64_t,
				double,
				bool,
				std::string,
				std::unique_ptr<CustomObject>,
				std::vector<int64_t>,
				std::vector<double>,
				std::vector<bool>,
				std::vector<std::string>,
				std::vector<std::unique_ptr<CustomObject>>
			>
		> data_;
	};
}
