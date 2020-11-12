#pragma once

namespace ose
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
				uptr<CustomObject>,
				std::vector<int64_t>,
				std::vector<double>,
				std::vector<bool>,
				std::vector<std::string>,
				std::vector<uptr<CustomObject>>
			>
		> data_;
	};
}
