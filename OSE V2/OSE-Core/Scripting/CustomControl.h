#pragma once

namespace ose
{
	class CustomControl
	{
	private:

		// The name of the control script
		// Only one control of a given type can exist
		// The control_type_name_ determines the script to use
		std::string control_type_name_;

		// The data used to initialise the control script
		std::initializer_list<std::any> data_;

	public:

		// Get the type name of the control
		std::string const & GetControlTypeName() const { return control_type_name_; }

		// Set the type name of the control
		void SetControlTypeName(std::string const & name) { control_type_name_ = name; }

		// Initialise the control script
		CustomControl(std::string const & type_name) : control_type_name_(type_name) {}

		// Does nothing
		virtual ~CustomControl() noexcept {}

		// Default move constructor
		CustomControl(CustomControl && other) noexcept = default;

		// Need to manually define copy constructor since std::string in non-copyable
		CustomControl(CustomControl const & other) noexcept
			: control_type_name_(other.control_type_name_) {}

		CustomControl & operator=(CustomControl & other) noexcept
		{
			control_type_name_ = other.control_type_name_;
			data_ = other.data_;
			return *this;
		}

		CustomControl & operator=(CustomControl &&) noexcept = delete;
	};
}
