#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Idra {
	struct TransformComponent 
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		mutable bool Dirty = true; // Flag to indicate if the transform has changed
		mutable glm::mat4 CachedTransformMatrix = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: Position(position), Rotation(rotation), Scale(scale) {}
		TransformComponent(const glm::vec3& position)
			: Position(position) {}


		const glm::mat4& getTransformMatrix() const
		{
			IDRA_PROFILE_FUNCTION();

			// @TODO: Move this to a system function for all transforms
			if (!Dirty)
				return CachedTransformMatrix;

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			Dirty = false;

			CachedTransformMatrix = glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);

			return CachedTransformMatrix;
		}
	};
}