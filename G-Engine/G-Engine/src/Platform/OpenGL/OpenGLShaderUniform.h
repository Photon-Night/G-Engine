#pragma once
#include "Renderer/ShaderUniform.h"
namespace GEngine
{
	class OpenGLShaderResourceDeclaration : public ShaderResourceDeclaration
	{
	public:
		enum class Type
		{
			None = 0,
			Texture2D,
			TextureCube,
		};
	private:
		friend class OpenGLShader;
	private:
		std::string m_Name;
		uint32_t m_Register = 0;
		uint32_t m_Count;
		Type m_Type;
	public:
		OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count);

		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetRegister() const override { return m_Register; }
		inline uint32_t GetCount() const override { return m_Count;}

		inline Type GetType() const { return m_Type; }

	public:
		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);
	};
}


