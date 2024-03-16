#include "gepch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
namespace GEngine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case GEngine::ShaderDataType::Float:    return GL_FLOAT;
		case GEngine::ShaderDataType::Float2:   return GL_FLOAT;
		case GEngine::ShaderDataType::Float3:   return GL_FLOAT;
		case GEngine::ShaderDataType::Float4:   return GL_FLOAT;
		case GEngine::ShaderDataType::Mat3:     return GL_FLOAT;
		case GEngine::ShaderDataType::Mat4:     return GL_FLOAT;
		case GEngine::ShaderDataType::Int:      return GL_INT;
		case GEngine::ShaderDataType::Int2:     return GL_INT;
		case GEngine::ShaderDataType::Int3:     return GL_INT;
		case GEngine::ShaderDataType::Int4:     return GL_INT;
		case GEngine::ShaderDataType::Bool:     return GL_BOOL;
		}

		GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
}
