#pragma once
#include "Renderer/Shader.h"
#include <glad/glad.h>

namespace GEngine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader() = default;
		OpenGLShader(const std::string& filepath);
		static Ref<OpenGLShader> CreateFromSource(const std::string& source);

		virtual void Reload() override;
		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;
	
		virtual void Bind() override;
		virtual RendererID GetRendererID() const override {return m_RendererID;}
	
		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;
		
		virtual void SetVSMaterialUniformBuffer(Buffer buffer) override;
		virtual void SetPSMaterialUniformBuffer(Buffer buffer) override;
	
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetMat4FromRenderThread(const std::string& name, const glm::mat4& value, bool bind = true) override;

		virtual void SetIntArray(const std::string& name, int* values, uint32_t size) override;

		virtual const std::string& GetName() const override { return m_Name; }
	
	private:
		void load(const std::string& source);

		std::string ReadShaderFromFile(const std::string& filePath) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Parse();
		void ParseUniform(const std::string& statement, ShaderDomain domain);
		void ParseUniformStruct(const std::string& block, ShaderDomain domain);
		ShaderStruct* FindStruct(const std::string& name);
	private:
		RendererID m_RendererID = 0;
		bool m_Loaded = false;
		bool m_IsCompute = false;
	
		std::string m_Name, m_AssetPath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;
		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
		ShaderUniformBufferList m_VSRendererUniformBuffers;
		ShaderUniformBufferList m_PSRendererUniformBuffers;

	};
}


