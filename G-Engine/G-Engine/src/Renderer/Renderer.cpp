#include "gepch.h"
#include "Renderer.h"
#include <glad/glad.h>
#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"

namespace GEngine
{
	RendererAPIType RendererAPI::s_CurrentRenderreAPI = RendererAPIType::OpenGL;

	struct RendererData
	{
		RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLib;
		Ref<VertexArray> m_FullScreenQuadVertexArray;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.m_ShaderLib = CreateScope<ShaderLibrary>();
		Renderer::Submit([]() {RendererAPI::Init();});

		float x = -1;
		float y = -1;
		float width = 2, height = 2;
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
		};

		QuadVertex* data = new QuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0.1f);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0.1f);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0.1f);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0.1f);
		data[3].TexCoord = glm::vec2(0, 1);

		s_Data.m_FullScreenQuadVertexArray = VertexArray::Create();
		auto quadVB = VertexBuffer::Create(data, 4 * sizeof(QuadVertex));
		quadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float3, "a_TexCoord"}
		});

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0, };
		auto quadIB = IndexBuffer::Create(indices, 6 * sizeof(uint32_t));

		s_Data.m_FullScreenQuadVertexArray->AddVertexBuffer(quadVB);
		s_Data.m_FullScreenQuadVertexArray->SetIndexBuffer(quadIB);
	}

	const Scope<ShaderLibrary>& Renderer::GetShaderLibrary()
	{
		return s_Data.m_ShaderLib;
	}

	void Renderer::Clear()
	{
		Renderer::Submit([]() {
			RendererAPI::Clear(1.0f, 1.0f, 1.0f, 1.0f);
		});
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		Renderer::Submit([=]() {
			RendererAPI::Clear(r, g, b, a);
		});
	}

	void Renderer::ClearMagenta()
	{
		Clear(1, 0, 1);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
	}

	void Renderer::DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest)
	{
		Renderer::Submit([=]() {
			RendererAPI::DrawIndexed(count, type, depthTest);
			});
	}

	void Renderer::SetLineThickness(float thickness)
	{
		Renderer::Submit([=]() {
			RendererAPI::SetLineThickness(thickness);
			});
	}

	void Renderer::WaitAndRender()
	{
		s_Data.m_CommandQueue.Execute();
	}

	RenderCommandQueue& Renderer::GetRenderCommandQueue()
	{
		return s_Data.m_CommandQueue;
	}
}
