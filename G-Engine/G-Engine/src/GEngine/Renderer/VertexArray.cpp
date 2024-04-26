#include "gepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "GEngine/Platform/OpenGL/OpenGLVertexArray.h"

namespace GEngine {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPIType::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}