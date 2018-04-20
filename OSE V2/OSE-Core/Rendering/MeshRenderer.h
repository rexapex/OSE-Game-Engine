#pragma once
#include "../../stdafx.h"
#include "../Entity/Component.h"
#include "Renderer.h"

namespace origami_sheep_engine
{
	struct MeshRenderer : public Renderer
	{
		//data
		//shader reference ?
		//pointer to material data ?



		//allocate rendering engine data
		MeshRenderer(const std::string & name) : Renderer(name) {}

		//de-allocate rendering engine data
		virtual ~MeshRenderer() {}



		//copy constructor
		MeshRenderer(const MeshRenderer & other) noexcept : Renderer(other) {}

		//copy assignment constructor
		MeshRenderer & operator=(const MeshRenderer & other) noexcept
		{
			Renderer::operator=(other);
			return *this;
		}

		//move constructor
		MeshRenderer(const MeshRenderer && other) noexcept : Renderer(other) {}

		//move assignment constructor
		MeshRenderer & operator=(const MeshRenderer && other) noexcept
		{
			Renderer::operator=(other);
			return *this;
		}

		//clone method which can be overwritten by base classes
		/*virtual std::unique_ptr<Component> clone() const
		{
			return std::make_unique<MeshRenderer>(*this);
		}*/
	};
}
