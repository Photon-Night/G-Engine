#pragma once

#include <glm/glm.hpp>

namespace GEngine
{
	struct BoundingBox
	{
		glm::vec3 Min, Max;

		BoundingBox()
			: Min(0.0f), Max(0.0f) {}

		BoundingBox(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}

	};
}