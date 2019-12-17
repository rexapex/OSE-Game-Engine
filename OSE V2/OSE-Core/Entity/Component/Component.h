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
		static const std::size_t type { std::hash<std::string>()( TO_STRING(ClassName) ) };	\
		return type;																		\
	}																						\
																							\
	virtual bool IsClassType(const std::size_t classType) const {							\
		if(classType == GetClassType())														\
			return true;																	\
		return ParentClass::IsClassType(classType);											\
	}																						\
																							\
	virtual std::unique_ptr<Component> Clone() const override								\
	{																						\
		return std::make_unique<ClassName>(*this);											\
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



namespace ose::entity
{
	// Dev Note - Can't make Component abstract even though it should be due to the way Entity clones components
	class Component
	{
	public:
		Component(const std::string & name);
		virtual ~Component();
		Component(const Component & other) noexcept;
		Component & operator=(const Component & other) noexcept;
		Component(Component && other) noexcept = default;
		Component & operator=(Component && other) noexcept = default;

		// Get the class type of Component
		static size_t GetClassType() {
			static const std::size_t type { std::hash<std::string>()( TO_STRING(Component) ) };
			return type;
		}

		// Test whether this class has the same class type as the one passed
		virtual bool IsClassType(const std::size_t classType) const {
			return classType == GetClassType();
		}

		// clone method which can be overwritten by base classes
		virtual std::unique_ptr<Component> Clone() const;

		// initialise the component, should only be called from the main thread
		virtual void Init() {}

		// disable the component (i.e. remove it from corresponding engine data pool
		virtual void Disable();

		// enable the component (i.e. add it to its corresponding engine data pool)
		virtual void Enable();

	private:
		// fields shared by all component types
		std::string name_;

		// true iff the component has been added to some engine data pool
		bool enabled_;
	};
}

