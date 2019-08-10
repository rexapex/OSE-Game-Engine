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
		static const std::size_t type = std::hash<std::string>()( TO_STRING(Component) );	\
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
private:




namespace ose::entity
{
	class Component
	{
	public:
		// Get the class type of Component
		static size_t GetClassType() {
			static const std::size_t type = std::hash<std::string>()( TO_STRING(Component) );
			return type;
		}
		// Test whether this class has the same class type as the one passed
		virtual bool IsClassType(const std::size_t classType) const {
			return classType == GetClassType();
		}
		
		Component(const std::string & name);
		virtual ~Component();
		Component(const Component & other) noexcept;
		Component & operator=(const Component & other) noexcept;
		Component(const Component && other) noexcept;
		Component & operator=(const Component && other) noexcept;

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

