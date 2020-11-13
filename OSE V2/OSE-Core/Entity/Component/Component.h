#pragma once
#include "stdafx.h"
#include <functional>

// Convert any data into a null-terminated string
#define TO_STRING(x) #x

// Macro required for defining components
// Macro should be the first line of the classes definition
// IMPORTANT - Only works for single inheritance RTTI
// Based on the StackOverflow answer https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work
#define COMPONENT( ClassName, ParentClass )													\
public:                                                                                     \
	static size_t GetClassType() {															\
		static std::size_t const type { std::hash<std::string>()( TO_STRING(ClassName) ) };	\
		return type;																		\
	}																						\
																							\
	virtual bool IsClassType(std::size_t const classType) const {							\
		if(classType == GetClassType())														\
			return true;																	\
		return ParentClass::IsClassType(classType);											\
	}																						\
																							\
	virtual uptr<Component> Clone() const override											\
	{																						\
		return ose::make_unique<ClassName>(*this);											\
	}																						\
private:/*																					\
	/* Private delete means cannot be deleted other than by friend classes /				\
	static void operator delete(void * p) {													\
		::operator delete(p);																\
	}																						\
																							\
	/* Private new means cannot be constructed other than by friend classes /				\
	static void * operator new(size_t size) {												\
		return new char[size];																\
	}
*/



namespace ose
{
	// Dev Note - Can't make Component abstract even though it should be due to the way Entity clones components
	class Component
	{
	public:
		Component(std::string const & name);
		virtual ~Component();
		Component(Component const & other) noexcept;
		Component(Component && other) noexcept = default;
		Component & operator=(Component &) noexcept = delete;
		Component & operator=(Component &&) noexcept = delete;

		// Get the class type of Component
		static size_t GetClassType() {
			static std::size_t const type { std::hash<std::string>()( TO_STRING(Component) ) };
			return type;
		}

		// Test whether this class has the same class type as the one passed
		virtual bool IsClassType(std::size_t const classType) const {
			return classType == GetClassType();
		}

		// clone method which can be overwritten by base classes
		virtual uptr<Component> Clone() const;

		// initialise the component, should only be called from the main thread
		virtual void Init() {}

		// Get the name of the component
		std::string const & GetName() const { return name_; }

		// disable the component (i.e. remove it from corresponding engine data pool
		virtual void Disable();

		// enable the component (i.e. add it to its corresponding engine data pool)
		virtual void Enable();

		// Set engine data
		// NOTE - Should NEVER be called from a script
		void SetEngineData(std::any && data) { engine_data_ = data; }

		// Get engine data
		std::any GetEngineData() const { return engine_data_; }

	private:
		// fields shared by all component types
		std::string name_;

		// true iff the component has been added to some engine data pool
		bool enabled_;

		// Data used by an engine to identify which engine object belongs to which components
		// Each engine can define data to be any type they want
		std::any engine_data_;
	};
}

