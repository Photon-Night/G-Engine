#pragma once

#include <glm/glm.hpp>
#include "GEngine/Renderer/VertexArray.h"
#include "GEngine/Renderer/Shader.h"
#include "GEngine/Renderer/Renderer.h"

namespace GEngine
{
	struct BoundingBox
	{
	public:
		glm::vec3 Min, Max;

		BoundingBox()
			: Min(0.0f), Max(0.0f) {}

		BoundingBox(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max)
		{
			m_LineVertexBufferBase = new glm::vec3[MaxLineVertices];
			m_LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");
			m_LineVertexArray = VertexArray::Create();


			m_LineVertexBuffer = VertexBuffer::Create(MaxLineVertices * sizeof(glm::vec3));
			m_LineVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
				});
			m_LineVertexArray->AddVertexBuffer(m_LineVertexBuffer);

			m_LineVertexBufferBase = new glm::vec3[MaxLineVertices];

			uint32_t* lineIndices = new uint32_t[MaxLineIndices];

			for (uint32_t i = 0; i < MaxLineIndices; i++)
				lineIndices[i] = i;

			Ref<IndexBuffer> lineIB = IndexBuffer::Create(lineIndices, MaxLineIndices);
			m_LineVertexArray->SetIndexBuffer(lineIB);
			delete[] lineIndices;
		}

		void DrawBoundingBox(const glm::mat4& transform, const glm::mat4& viewProj)
		{
			m_LineIndexCount = 0;
			m_LineVertexBufferPtr = m_LineVertexBufferBase;



			uint32_t dataSize = (uint8_t*)m_LineVertexBufferPtr - (uint8_t*)m_LineVertexBufferBase;
			if (dataSize)
			{
				m_LineVertexBuffer->SetData(m_LineVertexBufferBase, dataSize);

				m_LineShader->Bind();
				m_LineShader->SetMat4("u_ViewProjection", viewProj);

				m_LineVertexArray->Bind();
				Renderer::SetLineThickness(2.0f);
				Renderer::DrawIndexed(m_LineIndexCount, PrimitiveType::Lines, true);
			}
		}
	private:
		void DrawLine(const glm::vec3& p0, const glm::vec3& p1)
		{
			*m_LineVertexBufferPtr = p0;
			m_LineVertexBufferPtr++;

			*m_LineVertexBufferPtr = p1;
			m_LineVertexBufferPtr++;

			m_LineIndexCount += 2;
		}

	private:
		Ref<VertexArray> m_LineVertexArray;
		Ref<VertexBuffer> m_LineVertexBuffer;
		Ref<Shader> m_LineShader;

		uint32_t m_LineIndexCount = 0;
		glm::vec3* m_LineVertexBufferBase;
		glm::vec3* m_LineVertexBufferPtr;

		static const uint32_t MaxLines = 10000;
		static const uint32_t MaxLineVertices = MaxLines * 2;
		static const uint32_t MaxLineIndices = MaxLines * 6;
	};
}