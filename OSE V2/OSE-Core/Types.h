#pragma once

namespace ose
{
	template <typename T>
	using uptr = std::unique_ptr<T>;

	template <typename T>
	using sptr = std::shared_ptr<T>;

	template <typename T>
	using wptr = std::weak_ptr<T>;

	template <typename T, typename... Args>
	inline uptr<T> make_unique(Args &&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	inline sptr<T> make_shared(Args &&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
