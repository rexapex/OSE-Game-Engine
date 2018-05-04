#pragma once
#include "../../stdafx.h"
#include <functional>

// Thanks to StackOverflow answer https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work

// Convert any data into a null-terminated string
#define TO_STRING(x) #x

//****************
// COMPONENT_DECLERATION
//
// This macro must be included in the declaration of any subclass of Component.
// It declares variables used in type checking.
//****************
#define COMPONENT_DECLERATION( classname )                                                  \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool isClassType(const std::size_t classType) const override;                   \

//****************
// COMPONENT_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define COMPONENT_DEFINITION( parentclass, childclass )                                     \
const std::size_t childclass::Type = std::hash<std::string>()( TO_STRING(childclass) );		\
bool childclass::isClassType(const std::size_t classType) const {							\
	if(classType == childclass::Type)														\
		return true;																		\
	return parentclass::isClassType(classType);												\
}

namespace ose::entity
{
	class Component
	{
	public:
		static const std::size_t Type;
		virtual bool isClassType(const std::size_t classType) const {
			return classType == Type; 
		}

		//fields shared by all component types
		std::string name;



		//constructor
		Component(const std::string & n) : name(n) {}

		//allow sub classes to implement destructors
		virtual ~Component() {}



		//copy constructor
		Component(const Component & other) noexcept : name(other.name) {}

		//copy assignment constructor
		Component & operator=(const Component & other) noexcept
		{
			name = other.name;
			return *this;
		}

		//move constructor
		Component(const Component && other) noexcept : name(std::move(other.name)) {}

		//move assignment constructor
		Component & operator=(const Component && other) noexcept
		{
			name = std::move(other.name);
			return *this;
		}

		//clone method which can be overwritten by base classes
		virtual std::unique_ptr<Component> clone() const
		{
			return std::make_unique<Component>(*this);
		}
	};
}

// include component classes after defining component macros and declaring component class
#include "TextureFilter.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
