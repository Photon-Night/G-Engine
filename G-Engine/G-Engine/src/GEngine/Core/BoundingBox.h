#pragma once

#include <glm/glm.hpp>
#include "GEngine/Renderer/VertexArray.h"
#include "GEngine/Renderer/Shader.h"
#include "GEngine/Renderer/Renderer.h"

namespace GEngine
{
	class BoundingBox
	{
	public:
		glm::vec3 Min, Max;

		BoundingBox()
			: Min(0.0f), Max(0.0f) {}

		BoundingBox(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}

		void DrawBoundingBox(const glm::mat4& transform);
	private:
		void DrawLine(const glm::vec3& p0, const glm::vec3& p1);

	private:
		Ref<VertexArray> m_LineVertexArray;
		Ref<VertexBuffer> m_LineVertexBuffer;
		Ref<Shader> m_LineShader;

		uint32_t m_LineIndexCount = 0;
		glm::vec3* m_LineVertexBufferBase;
		glm::vec3* m_LineVertexBufferPtr;
	};
}