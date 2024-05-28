#include "gepch.h"
#include "BoundingBox.h"

namespace GEngine
{
	void BoundingBox::DrawBoundingBox(const glm::mat4& transform)
	{

	}

	void BoundingBox::DrawLine(const glm::vec3& p0, const glm::vec3& p1)
	{
		m_LineVertexBufferPtr = p0;

	}
}