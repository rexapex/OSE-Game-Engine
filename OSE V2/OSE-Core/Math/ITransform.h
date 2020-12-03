#pragma once

namespace ose
{
	class ITransform
	{
	protected:
		ITransform() {}

	public:
		virtual ~ITransform() {}

		// Defaults copy/move constructors
		ITransform(ITransform const & other) noexcept = default;
		ITransform & operator=(ITransform const & other) noexcept = default;
		ITransform(ITransform && other) noexcept = default;
		ITransform & operator=(ITransform && other) noexcept = default;

		virtual glm::vec3 const & GetTranslation() const = 0;
		virtual glm::quat const & GetOrientation() const = 0;
		virtual glm::vec3 const & GetScale() const = 0;

		virtual glm::mat4 GetTranslationMatrix() const = 0;
		virtual glm::mat4 GetOrientationMatrix() const = 0;
		virtual glm::mat4 GetScaleMatrix() const = 0;

		virtual glm::mat4 GetTransformMatrix() const = 0;
		virtual glm::mat4 GetInverseTransformMatrix() const = 0;

		virtual glm::vec3 GetUp() const = 0;
		virtual glm::vec3 GetForward() const = 0;
		virtual glm::vec3 GetRight() const = 0;
	};
}
