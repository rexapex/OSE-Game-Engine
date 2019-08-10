#pragma once
#include "stdafx.h"
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
    virtual bool IsClassType(const std::size_t classType) const override;                   \
																							\
	virtual std::unique_ptr<Component> Clone() const override								\
	{																						\
		return std::make_unique<classname>(*this);											\
	}																						\
private:


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
bool childclass::IsClassType(const std::size_t classType) const {							\
	if(classType == childclass::Type)														\
		return true;																		\
	return parentclass::IsClassType(classType);												\
}

namespace ose::entity
{
	class Component
	{
	public:
		static const std::size_t Type;
		virtual bool IsClassType(const std::size_t classType) const { return classType == Type; }
		
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

// include component classes after defining component macros and defining component class
#include "TextureFilter.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SpriteRenderer.h"
